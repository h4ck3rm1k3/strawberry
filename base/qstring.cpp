#include "i2-base.h"

using namespace icinga;

const size_t String::NPos = std::string::npos;

String::String(void)
	: m_Data()
{ }

String::String(const char *data)
	: m_Data(data)
{ }

String::String(const std::string& data)
	: m_Data(data)
{ }

String::String(const String& other)
	: m_Data(other.m_Data)
{ }

String& String::operator=(const String& rhs)
{
	m_Data = rhs.m_Data;
	return *this;
}

String& String::operator=(const std::string& rhs)
{
	m_Data = rhs;
	return *this;
}

String& String::operator=(const char *rhs)
{
	m_Data = rhs;
	return *this;
}

const char& String::operator[](size_t pos) const
{
	return m_Data[pos];
}

char& String::operator[](size_t pos)
{
	return m_Data[pos];
}

String& String::operator+=(const String& rhs)
{
	m_Data += rhs.m_Data;
	return *this;
}

String& String::operator+=(const char *rhs)
{
	m_Data += rhs;
	return *this;
}

bool String::IsEmpty(void) const
{
	return m_Data.empty();
}

bool String::operator<(const String& rhs) const
{
	return m_Data < rhs.m_Data;
}

String::operator const std::string&(void) const
{
	return m_Data;
}

const char *String::CStr(void) const
{
	return m_Data.c_str();
}

void String::Clear(void)
{
		m_Data.clear();
	}

size_t String::GetLength(void) const
{
	return m_Data.size();
}

size_t String::FindFirstOf(const char *s, size_t pos) const
{
	return m_Data.find_first_of(s, pos);
}

String String::SubStr(size_t first, size_t second) const
{
	return m_Data.substr(first, second);
}

void String::Replace(size_t first, size_t second, const String& str)
{
	m_Data.replace(first, second, str);
}

void String::Trim(void)
{
	boost::algorithm::trim(m_Data);
}

void String::swap(String& str)
{
	m_Data.swap(str.m_Data);
}

String::Iterator String::erase(String::Iterator first, String::Iterator last)
{
	return m_Data.erase(first, last);
}

String::Iterator String::Begin(void)
{
	return m_Data.begin();
}

String::ConstIterator String::Begin(void) const
{
	return m_Data.begin();
}

String::Iterator String::End(void)
{
	return m_Data.end();
}

String::ConstIterator String::End(void) const
{
	return m_Data.end();
}

ostream& icinga::operator<<(ostream& stream, const String& str)
{
	stream << static_cast<std::string>(str);
	return stream;
}

istream& icinga::operator>>(istream& stream, String& str)
{
	std::string tstr;
	stream >> tstr;
	str = tstr;
	return stream;
}

String icinga::operator+(const String& lhs, const String& rhs)
{
	return static_cast<std::string>(lhs) + static_cast<std::string>(rhs);
}

String icinga::operator+(const String& lhs, const char *rhs)
{
	return static_cast<std::string>(lhs) + rhs;
}

String icinga::operator+(const char *lhs, const String& rhs)
{
	return lhs + static_cast<std::string>(rhs);
}

bool icinga::operator==(const String& lhs, const String& rhs)
{
	return static_cast<std::string>(lhs) == static_cast<std::string>(rhs);
}

bool icinga::operator==(const String& lhs, const char *rhs)
{
	return static_cast<std::string>(lhs) == rhs;
}

bool icinga::operator==(const char *lhs, const String& rhs)
{
	return lhs == static_cast<std::string>(rhs);
}

bool icinga::operator!=(const String& lhs, const String& rhs)
{
	return static_cast<std::string>(lhs) != static_cast<std::string>(rhs);
}

bool icinga::operator!=(const String& lhs, const char *rhs)
{
	return static_cast<std::string>(lhs) != rhs;
}

bool icinga::operator!=(const char *lhs, const String& rhs)
{
	return lhs != static_cast<std::string>(rhs);
}

bool icinga::operator<(const String& lhs, const char *rhs)
{
	return static_cast<std::string>(lhs) < rhs;
}

bool icinga::operator<(const char *lhs, const String& rhs)
{
	return lhs < static_cast<std::string>(rhs);
}

bool icinga::operator>(const String& lhs, const char *rhs)
{
	return static_cast<std::string>(lhs) > rhs;
}

bool icinga::operator>(const char *lhs, const String& rhs)
{
	return lhs > static_cast<std::string>(rhs);
}

String::Iterator icinga::range_begin(String& x)
{
	return x.Begin();
}

String::ConstIterator icinga::range_begin(const String& x)
{
	return x.Begin();
}

String::Iterator icinga::range_end(String& x)
{
	return x.End();
}

String::ConstIterator icinga::range_end(const String& x)
{
	return x.End();
}

