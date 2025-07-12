#ifndef TINY_XML_STRING_INCLUDED
#define TINY_XML_STRING_INCLUDED

#include <windows.h>


typedef size_t size_type;


class C_DARK_STRING
{
private:
	struct Rep
	{
		size_type size, capacity;
		char str[1];
	} *rep_;
	static Rep nullrep_;

	void init(size_type sz) { init(sz, sz); }
	void set_size(size_type sz) { rep_->str[ rep_->size = sz ] = '\0'; }
	char* start() const { return rep_->str; }
	char* finish() const { return rep_->str + rep_->size; }

	void init(size_type sz, size_type cap)
	{
		if (0 != cap)
		{
			const size_type bytesNeeded = sizeof(Rep) + cap;
			const size_type intsNeeded = ( bytesNeeded + sizeof(int) - 1 ) / sizeof( int ); 
			rep_ = reinterpret_cast<Rep*>( new int[ intsNeeded ] );

			rep_->str[ rep_->size = sz ] = '\0';
			rep_->capacity = cap;
		}
		else
		{
			rep_ = &nullrep_;
		}
	}

	void quit()
	{
		if (rep_ != &nullrep_)
		{
			delete [] ( reinterpret_cast<int*>( rep_ ) );
		}
	}

public :
	C_DARK_STRING () : rep_(&nullrep_)
	{

	}
	C_DARK_STRING ( const C_DARK_STRING & copy) : rep_(0)
	{
		init(copy.length());
		memcpy(start(), copy.data(), length());
	}

	explicit C_DARK_STRING ( const char * copy) : rep_(0)
	{
		init( static_cast<size_type>( lstrlenA(copy) ));
		memcpy(start(), copy, length());
	}

	explicit C_DARK_STRING ( const char * str, size_type len) : rep_(0)
	{
		init(len);
		memcpy(start(), str, len);
	}

	~C_DARK_STRING ()
	{
		quit();
	}

	static const size_type npos; // = -1;


	const char * c_str () const { return rep_->str; }
	const char * data () const { return rep_->str; }
	size_type length () const { return rep_->size; }
	size_type size () const { return rep_->size; }
	BOOL empty () const { return rep_->size == 0; }
	size_type capacity () const { return rep_->capacity; }
	size_type find (char lookup) const
	{
		return find(lookup, 0);
	}
	size_type find (char tofind, size_type offset) const
	{
		if (offset >= length()) return npos;

		for (LPCSTR p = c_str() + offset; *p != '\0'; ++p)
		{
			if (*p == tofind) return static_cast< size_type >( p - c_str() );
		}
		return npos;
	}
	void clear ()
	{
		quit();
		init(0,0);
	}
	void reserve (size_type cap);
	C_DARK_STRING& assign (LPCSTR str, size_type len);
	C_DARK_STRING& append (LPCSTR str, size_type len);
	void swap (C_DARK_STRING& other)
	{
		Rep* r = rep_;
		rep_ = other.rep_;
		other.rep_ = r;
	}

	C_DARK_STRING& operator = (const char * copy)
	{
		return assign( copy, (size_type)lstrlenA(copy));
	}

	C_DARK_STRING& operator = (const C_DARK_STRING & copy)
	{
		return assign(copy.start(), copy.length());
	}

	C_DARK_STRING& operator += (const char * suffix)
	{
		return append(suffix, static_cast<size_type>( lstrlenA(suffix) ));
	}

	C_DARK_STRING& operator += (char single)
	{
		return append(&single, 1);
	}

	C_DARK_STRING& operator += (const C_DARK_STRING & suffix)
	{
		return append(suffix.data(), suffix.length());
	}

	const char& at (size_type index) const
	{
		return rep_->str[ index ];
	}
	char& operator [] (size_type index) const
	{
		return rep_->str[ index ];
	}
};


inline BOOL operator == (const C_DARK_STRING & a, const C_DARK_STRING & b)
{
	return ( a.length() == b.length() ) && ( strcmp(a.c_str(), b.c_str()) == 0 );
}
inline BOOL operator < (const C_DARK_STRING & a, const C_DARK_STRING & b)
{
	return strcmp(a.c_str(), b.c_str()) < 0;
}

inline BOOL operator != (const C_DARK_STRING & a, const C_DARK_STRING & b) { return !(a == b); }
inline BOOL operator >  (const C_DARK_STRING & a, const C_DARK_STRING & b) { return b < a; }
inline BOOL operator <= (const C_DARK_STRING & a, const C_DARK_STRING & b) { return !(b < a); }
inline BOOL operator >= (const C_DARK_STRING & a, const C_DARK_STRING & b) { return !(a < b); }

inline BOOL operator == (const C_DARK_STRING & a, LPCSTR b) { return strcmp(a.c_str(), b) == 0; }
inline BOOL operator == (LPCSTR a, const C_DARK_STRING & b) { return b == a; }
inline BOOL operator != (const C_DARK_STRING & a, LPCSTR b) { return !(a == b); }
inline BOOL operator != (LPCSTR a, const C_DARK_STRING & b) { return !(b == a); }

C_DARK_STRING operator + (const C_DARK_STRING & a, const C_DARK_STRING & b);
C_DARK_STRING operator + (const C_DARK_STRING & a, LPCSTR b);
C_DARK_STRING operator + (LPCSTR a, const C_DARK_STRING & b);



class C_DARK_WSTRING
{
private:
	struct Rep
	{
		size_type size, capacity;
		wchar_t str[1];
	} *rep_;
	static Rep nullrep_;

	void init(size_type sz) { init(sz, sz); }
	void set_size(size_type sz) { rep_->str[ rep_->size = sz ] = '\0'; }
	wchar_t* start() const { return rep_->str; }
	wchar_t* finish() const { return rep_->str + rep_->size; }

	void init(size_type sz, size_type cap)
	{
		if (0 != cap)
		{
			const size_type bytesNeeded = sizeof(Rep) + cap;
			const size_type intsNeeded = ( bytesNeeded + sizeof(int) - 1 ) / sizeof( int ); 
			rep_ = reinterpret_cast<Rep*>( new int[ intsNeeded ] );

			rep_->str[ rep_->size = sz ] = '\0';
			rep_->capacity = cap;
		}
		else
		{
			rep_ = &nullrep_;
		}
	}

	void quit()
	{
		if (rep_ != &nullrep_)
		{
			delete [] ( reinterpret_cast<int*>( rep_ ) );
		}
	}

public :
	C_DARK_WSTRING () : rep_(&nullrep_)
	{

	}
	C_DARK_WSTRING ( const C_DARK_WSTRING & copy) : rep_(0)
	{
		init(copy.length());
		memcpy(start(), copy.data(), length());
	}

	explicit C_DARK_WSTRING ( LPCWSTR copy) : rep_(0)
	{
		init( static_cast<size_type>( lstrlenW(copy) ));
		memcpy(start(), copy, length());
	}

	explicit C_DARK_WSTRING ( LPCWSTR str, size_type len) : rep_(0)
	{
		init(len);
		memcpy(start(), str, len);
	}

	~C_DARK_WSTRING ()
	{
		quit();
	}

	static const size_type npos; // = -1;


	LPCWSTR c_str () const { return rep_->str; }
	LPCWSTR data () const { return rep_->str; }
	size_type length () const { return rep_->size; }
	size_type size () const { return rep_->size; }
	BOOL empty () const { return rep_->size == 0; }
	size_type capacity () const { return rep_->capacity; }
	size_type find (wchar_t lookup) const
	{
		return find(lookup, 0);
	}
	size_type find (wchar_t tofind, size_type offset) const
	{
		if (offset >= length())
		{
			return npos;
		}
		for (LPCWSTR p = c_str() + offset; *p != '\0'; ++p)
		{
			if (*p == tofind) return static_cast< size_type >( p - c_str() );
		}
		return npos;
	}
	void clear ()
	{
		quit();
		init(0,0);
	}
	void reserve (size_type cap);
	C_DARK_WSTRING& assign (LPCWSTR str, size_type len);
	C_DARK_WSTRING& append (LPCWSTR str, size_type len);
	void swap (C_DARK_WSTRING& other)
	{
		Rep* r = rep_;
		rep_ = other.rep_;
		other.rep_ = r;
	}

	C_DARK_WSTRING& operator = (LPCWSTR copy)
	{
		return assign( copy, (size_type)lstrlenW(copy));
	}

	C_DARK_WSTRING& operator = (const C_DARK_WSTRING & copy)
	{
		return assign(copy.start(), copy.length());
	}

	C_DARK_WSTRING& operator += (LPCWSTR suffix)
	{
		return append(suffix, static_cast<size_type>( lstrlenW(suffix) ));
	}

	C_DARK_WSTRING& operator += (wchar_t single)
	{
		return append(&single, 1);
	}

	C_DARK_WSTRING& operator += (const C_DARK_WSTRING & suffix)
	{
		return append(suffix.data(), suffix.length());
	}

	const wchar_t& at (size_type index) const
	{
		return rep_->str[ index ];
	}
	wchar_t& operator [] (size_type index) const
	{
		return rep_->str[ index ];
	}
};


inline BOOL operator == (const C_DARK_WSTRING & a, const C_DARK_WSTRING & b)
{
	return ( a.length() == b.length() )	&& ( wcscmp(a.c_str(), b.c_str()) == 0 );
}
inline BOOL operator < (const C_DARK_WSTRING & a, const C_DARK_WSTRING & b)
{
	return wcscmp(a.c_str(), b.c_str()) < 0;
}

inline BOOL operator != (const C_DARK_WSTRING & a, const C_DARK_WSTRING & b) { return !(a == b); }
inline BOOL operator >  (const C_DARK_WSTRING & a, const C_DARK_WSTRING & b) { return b < a; }
inline BOOL operator <= (const C_DARK_WSTRING & a, const C_DARK_WSTRING & b) { return !(b < a); }
inline BOOL operator >= (const C_DARK_WSTRING & a, const C_DARK_WSTRING & b) { return !(a < b); }

inline BOOL operator == (const C_DARK_WSTRING & a, LPCWSTR b) { return wcscmp(a.c_str(), b) == 0; }
inline BOOL operator == (LPCWSTR a, const C_DARK_WSTRING & b) { return b == a; }
inline BOOL operator != (const C_DARK_WSTRING & a, LPCWSTR b) { return !(a == b); }
inline BOOL operator != (LPCWSTR a, const C_DARK_WSTRING & b) { return !(b == a); }

C_DARK_WSTRING operator + (const C_DARK_WSTRING & a, const C_DARK_WSTRING & b);
C_DARK_WSTRING operator + (const C_DARK_WSTRING & a, LPCWSTR b);
C_DARK_WSTRING operator + (LPCWSTR a, const C_DARK_WSTRING & b);


#if defined(_UNICODE)
	typedef C_DARK_WSTRING DARK_STRING;
#else
	typedef C_DARK_STRING DARK_STRING;
#endif

#endif	// TINY_XML_STRING_INCLUDED
