#ifndef TLSCLIENT_H
#define TLSCLIENT_H

namespace icinga
{

struct I2_BASE_API VerifyCertificateEventArgs : public EventArgs
{
	bool ValidCertificate;
	X509_STORE_CTX *Context;
	shared_ptr<X509> Certificate;
};

class I2_BASE_API TLSClient : public TCPClient
{
private:
	shared_ptr<SSL_CTX> m_SSLContext;
	shared_ptr<SSL> m_SSL;

	bool m_BlockRead;
	bool m_BlockWrite;

	static int m_SSLIndex;
	static bool m_SSLIndexInitialized;

	virtual int ReadableEventHandler(const EventArgs& ea);
	virtual int WritableEventHandler(const EventArgs& ea);

	virtual void CloseInternal(bool from_dtor);

	static void NullCertificateDeleter(X509 *certificate);

	static int SSLVerifyCertificate(int ok, X509_STORE_CTX *x509Context);

protected:
	void HandleSSLError(void);

public:
	TLSClient(TCPClientRole role, shared_ptr<SSL_CTX> sslContext);

	shared_ptr<X509> GetClientCertificate(void) const;
	shared_ptr<X509> GetPeerCertificate(void) const;

	virtual void Start(void);

	virtual bool WantsToRead(void) const;
	virtual bool WantsToWrite(void) const;

	Event<VerifyCertificateEventArgs> OnVerifyCertificate;
};

TCPClient::Ptr TLSClientFactory(TCPClientRole role, shared_ptr<SSL_CTX> sslContext);

}

#endif /* TLSCLIENT_H */