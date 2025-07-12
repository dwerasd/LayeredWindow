#include "stdafx.h"
#include "DarkString.h"

// Error value for find primitive
const size_type C_DARK_STRING::npos = static_cast< size_type >(-1);


// Null rep.
C_DARK_STRING::Rep C_DARK_STRING::nullrep_ = { 0, 0, { '\0' } };


void C_DARK_STRING::reserve (size_type cap)
{
	if (cap > capacity())
	{
		C_DARK_STRING tmp;
		tmp.init(length(), cap);
		memcpy(tmp.start(), data(), length());
		swap(tmp);
	}
}


C_DARK_STRING& C_DARK_STRING::assign(LPCSTR str, size_type len)
{
	size_type cap = capacity();
	if (len > cap || cap > 3*(len + 8))
	{
		C_DARK_STRING tmp;
		tmp.init(len);
		memcpy(tmp.start(), str, len);
		swap(tmp);
	}
	else
	{
		memmove(start(), str, len);
		set_size(len);
	}
	return *this;
}


C_DARK_STRING& C_DARK_STRING::append(LPCSTR str, size_type len)
{
	size_type newsize = length() + len;
	if (newsize > capacity())
	{
		reserve (newsize + capacity());
	}
	memmove(finish(), str, len);
	set_size(newsize);
	return *this;
}


C_DARK_STRING operator + (const C_DARK_STRING & a, const C_DARK_STRING & b)
{
	C_DARK_STRING tmp;
	tmp.reserve(a.length() + b.length());
	tmp += a;
	tmp += b;
	return tmp;
}

C_DARK_STRING operator + (const C_DARK_STRING & a, LPCSTR b)
{
	C_DARK_STRING tmp;
	size_type b_len = static_cast<size_type>( lstrlenA(b) );
	tmp.reserve(a.length() + b_len);
	tmp += a;
	tmp.append(b, b_len);
	return tmp;
}

C_DARK_STRING operator + (LPCSTR a, const C_DARK_STRING & b)
{
	C_DARK_STRING tmp;
	size_type a_len = static_cast<size_type>( lstrlenA(a) );
	tmp.reserve(a_len + b.length());
	tmp.append(a, a_len);
	tmp += b;
	return tmp;
}


const size_type C_DARK_WSTRING::npos = static_cast< size_type >(-1);


// Null rep.
C_DARK_WSTRING::Rep C_DARK_WSTRING::nullrep_ = { 0, 0, { '\0' } };


void C_DARK_WSTRING::reserve (size_type cap)
{
	if (cap > capacity())
	{
		C_DARK_WSTRING tmp;
		tmp.init(length(), cap);
		memcpy(tmp.start(), data(), length());
		swap(tmp);
	}
}


C_DARK_WSTRING& C_DARK_WSTRING::assign(LPCWSTR str, size_type len)
{
	size_type cap = capacity();
	if (len > cap || cap > 3*(len + 8))
	{
		C_DARK_WSTRING tmp;
		tmp.init(len);
		memcpy(tmp.start(), str, len);
		swap(tmp);
	}
	else
	{
		memmove(start(), str, len);
		set_size(len);
	}
	return *this;
}


C_DARK_WSTRING& C_DARK_WSTRING::append(LPCWSTR str, size_type len)
{
	size_type newsize = length() + len;
	if (newsize > capacity())
	{
		reserve (newsize + capacity());
	}
	memmove(finish(), str, len);
	set_size(newsize);
	return *this;
}


C_DARK_WSTRING operator + (const C_DARK_WSTRING & a, const C_DARK_WSTRING & b)
{
	C_DARK_WSTRING tmp;
	tmp.reserve(a.length() + b.length());
	tmp += a;
	tmp += b;
	return tmp;
}

C_DARK_WSTRING operator + (const C_DARK_WSTRING & a, LPCWSTR b)
{
	C_DARK_WSTRING tmp;
	size_type b_len = static_cast<size_type>( lstrlenW(b) );
	tmp.reserve(a.length() + b_len);
	tmp += a;
	tmp.append(b, b_len);
	return tmp;
}

C_DARK_WSTRING operator + (LPCWSTR a, const C_DARK_WSTRING & b)
{
	C_DARK_WSTRING tmp;
	size_type a_len = static_cast<size_type>( lstrlenW(a) );
	tmp.reserve(a_len + b.length());
	tmp.append(a, a_len);
	tmp += b;
	return tmp;
}