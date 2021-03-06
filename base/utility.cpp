/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012 Icinga Development Team (http://www.icinga.org/)        *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#include "i2-base.h"
#include <mmatch.h>

using namespace icinga;

bool I2_EXPORT Utility::m_SSLInitialized = false;

/**
 * Returns a human-readable type name of a type_info object.
 *
 * @param ti A type_info object.
 * @returns The type name of the object.
 */
String Utility::GetTypeName(const type_info& ti)
{
	String klass = ti.name();

#ifdef HAVE_GCC_ABI_DEMANGLE
	int status;
	char *realname = abi::__cxa_demangle(klass.CStr(), 0, 0, &status);

	if (realname != NULL) {
		klass = String(realname);
		free(realname);
	}
#endif /* HAVE_GCC_ABI_DEMANGLE */

	return klass;
}


/**
 * Detaches from the controlling terminal.
 */
void Utility::Daemonize(void) {
#ifndef _WIN32
	pid_t pid;
	int fd;

	pid = fork();
	if (pid < 0)
		throw_exception(PosixException("fork() failed", errno));

	if (pid)
		exit(0);

	fd = open("/dev/null", O_RDWR);

	if (fd < 0)
		throw_exception(PosixException("open() failed", errno));

	if (fd != STDIN_FILENO)
		dup2(fd, STDIN_FILENO);

	if (fd != STDOUT_FILENO)
		dup2(fd, STDOUT_FILENO);

	if (fd != STDERR_FILENO)
		dup2(fd, STDERR_FILENO);

	if (fd > STDERR_FILENO)
		close(fd);

	if (setsid() < 0)
		throw_exception(PosixException("setsid() failed", errno));
#endif
}

/**
 * Initializes the OpenSSL library.
 */
void Utility::InitializeOpenSSL(void)
{
	if (m_SSLInitialized)
		return;

	SSL_library_init();
	SSL_load_error_strings();

	m_SSLInitialized = true;
}

/**
 * Initializes an SSL context using the specified certificates.
 *
 * @param pubkey The public key.
 * @param privkey The matching private key.
 * @param cakey CA certificate chain file.
 * @returns An SSL context.
 */
shared_ptr<SSL_CTX> Utility::MakeSSLContext(String pubkey, String privkey, String cakey)
{
	InitializeOpenSSL();

	shared_ptr<SSL_CTX> sslContext = shared_ptr<SSL_CTX>(SSL_CTX_new(TLSv1_method()), SSL_CTX_free);

	SSL_CTX_set_mode(sslContext.get(), SSL_MODE_ENABLE_PARTIAL_WRITE | SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);

	if (!SSL_CTX_use_certificate_chain_file(sslContext.get(), pubkey.CStr()))
		throw_exception(OpenSSLException("Could not load public X509 key file", ERR_get_error()));

	if (!SSL_CTX_use_PrivateKey_file(sslContext.get(), privkey.CStr(), SSL_FILETYPE_PEM))
		throw_exception(OpenSSLException("Could not load private X509 key file", ERR_get_error()));

	if (!SSL_CTX_load_verify_locations(sslContext.get(), cakey.CStr(), NULL))
		throw_exception(OpenSSLException("Could not load public CA key file", ERR_get_error()));

	STACK_OF(X509_NAME) *cert_names;

	cert_names = SSL_load_client_CA_file(cakey.CStr());
	if (cert_names == NULL)
		throw_exception(OpenSSLException("SSL_load_client_CA_file() failed", ERR_get_error()));

	SSL_CTX_set_client_CA_list(sslContext.get(), cert_names);

	return sslContext;
}

/**
 * Retrieves the common name for an X509 certificate.
 *
 * @param certificate The X509 certificate.
 * @returns The common name.
 */
String Utility::GetCertificateCN(const shared_ptr<X509>& certificate)
{
	char buffer[256];

	int rc = X509_NAME_get_text_by_NID(X509_get_subject_name(certificate.get()), NID_commonName, buffer, sizeof(buffer));

	if (rc == -1)
		throw_exception(OpenSSLException("X509 certificate has no CN attribute", ERR_get_error()));

	return buffer;
}

/**
 * Retrieves an X509 certificate from the specified file.
 *
 * @param pemfile The filename.
 * @returns An X509 certificate.
 */
shared_ptr<X509> Utility::GetX509Certificate(String pemfile)
{
	X509 *cert;
	BIO *fpcert = BIO_new(BIO_s_file());

	if (fpcert == NULL)
		throw_exception(OpenSSLException("BIO_new failed", ERR_get_error()));

	if (BIO_read_filename(fpcert, pemfile.CStr()) < 0)
		throw_exception(OpenSSLException("BIO_read_filename failed", ERR_get_error()));

	cert = PEM_read_bio_X509_AUX(fpcert, NULL, NULL, NULL);
	if (cert == NULL)
		throw_exception(OpenSSLException("PEM_read_bio_X509_AUX failed", ERR_get_error()));

	BIO_free(fpcert);

	return shared_ptr<X509>(cert, X509_free);
}

/**
 * Performs wildcard pattern matching.
 *
 * @param pattern The wildcard pattern.
 * @param text The String that should be checked.
 * @returns true if the wildcard pattern matches, false otherwise.
 */
bool Utility::Match(String pattern, String text)
{
	return (match(pattern.CStr(), text.CStr()) == 0);
}

/**
 * Returns the directory component of a path. See dirname(3) for details.
 *
 * @param path The full path.
 * @returns The directory.
 */
String Utility::DirName(const String& path)
{
	char *dir = strdup(path.CStr());
	String result;

	if (dir == NULL)
		throw_exception(bad_alloc());

#ifndef _WIN32
	result = dirname(dir);
#else /* _WIN32 */
	if (!PathRemoveFileSpec(dir)) {
		free(dir);
		throw_exception(Win32Exception("PathRemoveFileSpec() failed", GetLastError()));
	}

	result = dir;
#endif /* _WIN32 */

	free(dir);

	return result;
}

/**
 * Returns the file component of a path. See basename(3) for details.
 *
 * @param path The full path.
 * @returns The filename.
 */
String Utility::BaseName(const String& path)
{
	char *dir = strdup(path.CStr());
	String result;

	if (dir == NULL)
		throw_exception(bad_alloc());

#ifndef _WIN32
	result = basename(dir);
#else /* _WIN32 */
	result = PathFindFileName(dir);
#endif /* _WIN32 */

	free(dir);

	return result;
}

/**
 * Null deleter. Used as a parameter for the shared_ptr constructor.
 *
 * @param -- The object that should be deleted.
 */
void Utility::NullDeleter(void *)
{
	/* Nothing to do here. */
}

/**
 * Returns the current UNIX timestamp including fractions of seconds.
 *
 * @returns The current time.
 */
double Utility::GetTime(void)
{
#ifdef _WIN32
	FILETIME cft;
	GetSystemTimeAsFileTime(&cft);

	ULARGE_INTEGER ucft;
	ucft.HighPart = cft.dwHighDateTime;
	ucft.LowPart = cft.dwLowDateTime;

	SYSTEMTIME est = { 1970, 1, 4, 1, 0, 0, 0, 0};
	FILETIME eft;
	SystemTimeToFileTime(&est, &eft);

	ULARGE_INTEGER ueft;
	ueft.HighPart = eft.dwHighDateTime;
	ueft.LowPart = eft.dwLowDateTime;

	return ((ucft.QuadPart - ueft.QuadPart) / 10000) / 1000.0;
#else /* _WIN32 */
	struct timeval tv;

	if (gettimeofday(&tv, NULL) < 0)
		throw_exception(PosixException("gettimeofday() failed", errno));

	return tv.tv_sec + tv.tv_usec / 1000000.0;
#endif /* _WIN32 */
}
