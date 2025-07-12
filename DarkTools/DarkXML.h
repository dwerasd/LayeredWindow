#ifndef TINY_XML_INCLUDED
#define TINY_XML_INCLUDED



#pragma warning( push )


#include <stdio.h>
#include <stdlib.h>


#include "DarkString.h"



class C_TINY_XML_DOCUMENT;
class C_TINY_XML_ELEMENT;
class C_TINY_XML_COMMENT;
class C_TINY_XML_UNKNOWN;
class C_TINY_XML_ATTRIBUTE;
class C_TINY_XML_TEXT;
class C_TINY_XML_DECLARATION;
class C_TINY_XML_PARSING_DATA;

const int TINY_XML_MAJOR_VERSION = 2;
const int TINY_XML_MINOR_VERSION = 5;
const int TINY_XML_PATCH_VERSION = 3;

const unsigned char TINY_XML_UTF_LEAD_0 = 0xefU;
const unsigned char TINY_XML_UTF_LEAD_1 = 0xbbU;
const unsigned char TINY_XML_UTF_LEAD_2 = 0xbfU;


/*	Internal structure for tracking location of items 
	in the XML file.
*/
struct _TINY_XML_CURSOR
{
	_TINY_XML_CURSOR()		{ Clear(); }
	void Clear()		{ row = col = -1; }

	int row;
	int col;
};

class C_TINY_XML_VISITOR
{
public:
	C_TINY_XML_VISITOR()
	{

	}
	virtual ~C_TINY_XML_VISITOR()
	{

	}

	virtual BOOL VisitEnter( const C_TINY_XML_DOCUMENT& )			{ return(TRUE); }
	virtual BOOL VisitExit( const C_TINY_XML_DOCUMENT& )			{ return(TRUE); }
	virtual BOOL VisitEnter( const C_TINY_XML_ELEMENT& , const C_TINY_XML_ATTRIBUTE* )	{ return(TRUE); }
	virtual BOOL VisitExit( const C_TINY_XML_ELEMENT&  )		{ return(TRUE); }

	virtual BOOL Visit( const C_TINY_XML_DECLARATION& )	{ return(TRUE); }
	virtual BOOL Visit( const C_TINY_XML_TEXT& )					{ return(TRUE); }
	virtual BOOL Visit( const C_TINY_XML_COMMENT& )			{ return(TRUE); }
	virtual BOOL Visit( const C_TINY_XML_UNKNOWN& )			{ return(TRUE); }
};

// Only used by Attribute::Query functions
enum 
{ 
	TINY_XML_SUCCESS,
	TINY_XML_NO_ATTRIBUTE,
	TINY_XML_WRONG_TYPE
};


// Used by the parsing routines.
enum _TINY_XML_ENCODING_
{
	TINY_XML_ENCODING_UNKNOWN,
	TINY_XML_ENCODING_UTF8,
	TINY_XML_ENCODING_LEGACY
};

const _TINY_XML_ENCODING_ TINY_XML_DEFAULT_ENCODING = TINY_XML_ENCODING_UNKNOWN;


class C_TINY_XML_BASE
{
	friend class C_TINY_XML_NODE;
	friend class C_TINY_XML_ELEMENT;
	friend class C_TINY_XML_DOCUMENT;

private:
	C_TINY_XML_BASE( const C_TINY_XML_BASE& );				// not implemented.
	void operator=( const C_TINY_XML_BASE& base );	// not allowed.

	struct Entity
	{
		LPCSTR     str;
		unsigned int	strLength;
		char		    chr;
	};
	enum
	{
		NUM_ENTITY = 5,
		MAX_ENTITY_LENGTH = 6

	};
	static Entity entity[ NUM_ENTITY ];
	static BOOL condenseWhiteSpace;

protected:
	enum
	{
		TINY_XML_NO_ERROR = 0,
		TINY_XML_ERROR,
		TINY_XML_ERROR_OPENING_FILE,
		TINY_XML_ERROR_OUT_OF_MEMORY,
		TINY_XML_ERROR_PARSING_ELEMENT,
		TINY_XML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		TINY_XML_ERROR_READING_ELEMENT_VALUE,
		TINY_XML_ERROR_READING_ATTRIBUTES,
		TINY_XML_ERROR_PARSING_EMPTY,
		TINY_XML_ERROR_READING_END_TAG,
		TINY_XML_ERROR_PARSING_UNKNOWN,
		TINY_XML_ERROR_PARSING_COMMENT,
		TINY_XML_ERROR_PARSING_DECLARATION,
		TINY_XML_ERROR_DOCUMENT_EMPTY,
		TINY_XML_ERROR_EMBEDDED_NULL,
		TINY_XML_ERROR_PARSING_CDATA,
		TINY_XML_ERROR_DOCUMENT_TOP_ONLY,
		TINY_XML_ERROR_STRING_COUNT
	};

	_TINY_XML_CURSOR location;
	LPVOID userData;

	inline static BOOL IsWhiteSpace( char c )		
	{ 
		return ( isspace( (unsigned char) c ) || c == '\n' || c == '\r' ); 
	}
	inline static BOOL IsWhiteSpace( int c )
	{
		if ( c < 256 )
		{
			return IsWhiteSpace( (char) c );
		}
		return(FALSE);
	}
	inline static LPCSTR GetChar( LPCSTR p, char* _value, int* length, _TINY_XML_ENCODING_ encoding )
	{
		if ( encoding == TINY_XML_ENCODING_UTF8 )
		{
			*length = utf8ByteTable[ *((const unsigned char*)p) ];
		}
		else
		{
			*length = 1;
		}

		if ( *length == 1 )
		{
			if ( *p == '&' )
				return GetEntity( p, _value, length, encoding );
			*_value = *p;
			return p+1;
		}
		else if ( *length )
		{
			for( int i=0; p[i] && i<*length; ++i )
			{
				_value[i] = p[i];
			}
			return p + (*length);
		}
		else
		{
			return 0;
		}
	}
	inline static int ToLower( int v, _TINY_XML_ENCODING_ encoding )
	{
		if ( encoding == TINY_XML_ENCODING_UTF8 )
		{
			if ( v < 128 ) return tolower( v );
			return v;
		}
		else
		{
			return tolower( v );
		}
	}
	static LPCSTR SkipWhiteSpace( LPCSTR, _TINY_XML_ENCODING_ encoding );
	static LPCSTR ReadName( LPCSTR p, C_DARK_STRING* name, _TINY_XML_ENCODING_ encoding );
	static LPCSTR ReadText(	LPCSTR in, C_DARK_STRING* text, BOOL ignoreWhiteSpace, LPCSTR endTag, BOOL ignoreCase, _TINY_XML_ENCODING_ encoding );
	static LPCSTR GetEntity( LPCSTR in, char* value, int* length, _TINY_XML_ENCODING_ encoding );
	static BOOL StringEqual(LPCSTR p, LPCSTR endTag, BOOL ignoreCase, _TINY_XML_ENCODING_ encoding );
	static LPCSTR errorString[ TINY_XML_ERROR_STRING_COUNT ];
	static int IsAlpha( unsigned char anyByte, _TINY_XML_ENCODING_ encoding );
	static int IsAlphaNum( unsigned char anyByte, _TINY_XML_ENCODING_ encoding );
	static void ConvertUTF32ToUTF8( unsigned long input, char* output, int* length );

public:
	C_TINY_XML_BASE() : userData(0)
	{

	}
	virtual ~C_TINY_XML_BASE()
	{

	}
	
	static const int utf8ByteTable[256];

	static void SetCondenseWhiteSpace( BOOL condense )		{ condenseWhiteSpace = condense; }
	static BOOL IsWhiteSpaceCondensed()						{ return condenseWhiteSpace; }

	int Row() const			{ return location.row + 1; }
	int Column() const		{ return location.col + 1; }

	void  SetUserData( LPVOID user ) { userData = user; }
	LPVOID GetUserData() { return userData; }
	const LPVOID GetUserData() const { return userData; }
	
	virtual void Print( FILE* cfile, int depth ) const = 0;
	virtual LPCSTR Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding /*= TINY_XML_ENCODING_UNKNOWN */ ) = 0;

	static void EncodeString( const C_DARK_STRING& str, C_DARK_STRING* out );
};


class C_TINY_XML_NODE : public C_TINY_XML_BASE
{
	friend class C_TINY_XML_DOCUMENT;
	friend class C_TINY_XML_ELEMENT;

private:
	C_TINY_XML_NODE( const C_TINY_XML_NODE& );
	void operator=( const C_TINY_XML_NODE& base );

protected:
	enum NodeType
	{
		DOCUMENT,
		ELEMENT,
		COMMENT,
		UNKNOWN,
		TEXT,
		DECLARATION,
		TYPECOUNT
	};

	C_TINY_XML_NODE( NodeType _type );

	NodeType			type;
	C_DARK_STRING		value;

	C_TINY_XML_NODE *parent, *firstChild, *lastChild, *prev, *next;

	void CopyTo( C_TINY_XML_NODE* target ) const;
	C_TINY_XML_NODE* Identify( LPCSTR start, _TINY_XML_ENCODING_ encoding );

public:
	virtual ~C_TINY_XML_NODE();

	const char *Value() const { return value.c_str (); }
	const C_DARK_STRING& ValueTStr() const { return value; }

	void SetValue(const char * _value) { value = _value;}
	void Clear();

	C_TINY_XML_NODE* Parent()							{ return parent; }
	const C_TINY_XML_NODE* Parent() const				{ return parent; }

	const C_TINY_XML_NODE* FirstChild()	const			{ return firstChild; }
	C_TINY_XML_NODE* FirstChild()						{ return firstChild; }
	const C_TINY_XML_NODE* FirstChild( const char * value ) const;
	
	C_TINY_XML_NODE* FirstChild( const char * _value )
	{
		return const_cast< C_TINY_XML_NODE* > ((const_cast< const C_TINY_XML_NODE* >(this))->FirstChild( _value ));
	}
	const C_TINY_XML_NODE* LastChild() const			{ return lastChild; }
	C_TINY_XML_NODE* LastChild()						{ return lastChild; }
	
	const C_TINY_XML_NODE* LastChild( const char * value ) const;
	C_TINY_XML_NODE* LastChild( const char * _value )
	{
		return const_cast< C_TINY_XML_NODE* > ((const_cast< const C_TINY_XML_NODE* >(this))->LastChild( _value ));
	}

	const C_TINY_XML_NODE* IterateChildren( const C_TINY_XML_NODE* previous ) const;
	C_TINY_XML_NODE* IterateChildren( const C_TINY_XML_NODE* previous ) {
		return const_cast< C_TINY_XML_NODE* >( (const_cast< const C_TINY_XML_NODE* >(this))->IterateChildren( previous ) );
	}

	const C_TINY_XML_NODE* IterateChildren( const char * value, const C_TINY_XML_NODE* previous ) const;
	C_TINY_XML_NODE* IterateChildren( const char * _value, const C_TINY_XML_NODE* previous ) {
		return const_cast< C_TINY_XML_NODE* >( (const_cast< const C_TINY_XML_NODE* >(this))->IterateChildren( _value, previous ) );
	}

	C_TINY_XML_NODE* InsertEndChild( const C_TINY_XML_NODE& addThis );
	C_TINY_XML_NODE* LinkEndChild( C_TINY_XML_NODE* addThis );
	C_TINY_XML_NODE* InsertBeforeChild( C_TINY_XML_NODE* beforeThis, const C_TINY_XML_NODE& addThis );
	C_TINY_XML_NODE* InsertAfterChild(  C_TINY_XML_NODE* afterThis, const C_TINY_XML_NODE& addThis );
	C_TINY_XML_NODE* ReplaceChild( C_TINY_XML_NODE* replaceThis, const C_TINY_XML_NODE& withThis );

	BOOL RemoveChild( C_TINY_XML_NODE* removeThis );
	const C_TINY_XML_NODE* PreviousSibling() const			{ return prev; }
	C_TINY_XML_NODE* PreviousSibling()						{ return prev; }
	const C_TINY_XML_NODE* PreviousSibling( const char * ) const;
	C_TINY_XML_NODE* PreviousSibling( const char *_prev )
	{
		return const_cast< C_TINY_XML_NODE* >( (const_cast< const C_TINY_XML_NODE* >(this))->PreviousSibling( _prev ) );
	}
	const C_TINY_XML_NODE* NextSibling() const				{ return next; }
	C_TINY_XML_NODE* NextSibling()							{ return next; }
	const C_TINY_XML_NODE* NextSibling( const char * ) const;
	C_TINY_XML_NODE* NextSibling( LPCSTR _next ) {
		return const_cast< C_TINY_XML_NODE* >( (const_cast< const C_TINY_XML_NODE* >(this))->NextSibling( _next ) );
	}

	const C_TINY_XML_ELEMENT* NextSiblingElement() const;
	C_TINY_XML_ELEMENT* NextSiblingElement() {
		return const_cast< C_TINY_XML_ELEMENT* >( (const_cast< const C_TINY_XML_NODE* >(this))->NextSiblingElement() );
	}

	const C_TINY_XML_ELEMENT* NextSiblingElement( const char * ) const;
	C_TINY_XML_ELEMENT* NextSiblingElement( const char *_next ) {
		return const_cast< C_TINY_XML_ELEMENT* >( (const_cast< const C_TINY_XML_NODE* >(this))->NextSiblingElement( _next ) );
	}

	const C_TINY_XML_ELEMENT* FirstChildElement()	const;
	C_TINY_XML_ELEMENT* FirstChildElement() {
		return const_cast< C_TINY_XML_ELEMENT* >( (const_cast< const C_TINY_XML_NODE* >(this))->FirstChildElement() );
	}

	const C_TINY_XML_ELEMENT* FirstChildElement( const char * _value ) const;
	C_TINY_XML_ELEMENT* FirstChildElement( const char * _value ) {
		return const_cast< C_TINY_XML_ELEMENT* >( (const_cast< const C_TINY_XML_NODE* >(this))->FirstChildElement( _value ) );
	}
	int Type() const	{ return type; }
	const C_TINY_XML_DOCUMENT* GetDocument() const;
	C_TINY_XML_DOCUMENT* GetDocument() {
		return const_cast< C_TINY_XML_DOCUMENT* >( (const_cast< const C_TINY_XML_NODE* >(this))->GetDocument() );
	}

	BOOL NoChildren() const						{ return !firstChild; }

	virtual const C_TINY_XML_DOCUMENT*		ToDocument()    const { return 0; }
	virtual const C_TINY_XML_ELEMENT*		ToElement()     const { return 0; }
	virtual const C_TINY_XML_COMMENT*		ToComment()     const { return 0; }
	virtual const C_TINY_XML_UNKNOWN*		ToUnknown()     const { return 0; }
	virtual const C_TINY_XML_TEXT*			ToText()        const { return 0; }
	virtual const C_TINY_XML_DECLARATION*	ToDeclaration() const { return 0; }

	virtual C_TINY_XML_DOCUMENT*			ToDocument()    { return 0; }
	virtual C_TINY_XML_ELEMENT*				ToElement()	    { return 0; }
	virtual C_TINY_XML_COMMENT*				ToComment()     { return 0; }
	virtual C_TINY_XML_UNKNOWN*				ToUnknown()	    { return 0; }
	virtual C_TINY_XML_TEXT*				ToText()        { return 0; }
	virtual C_TINY_XML_DECLARATION*			ToDeclaration() { return 0; }

	virtual C_TINY_XML_NODE* Clone() const = 0;

	virtual BOOL Accept( C_TINY_XML_VISITOR* visitor ) const = 0;
};


class C_TINY_XML_ATTRIBUTE : public C_TINY_XML_BASE
{
	friend class C_TINY_XML_ATTRIBUTE_SET;

private:
	C_TINY_XML_ATTRIBUTE( const C_TINY_XML_ATTRIBUTE& );				// not implemented.
	void operator=( const C_TINY_XML_ATTRIBUTE& base );	// not allowed.
	
	C_DARK_STRING name, value;
	C_TINY_XML_DOCUMENT *document;	// for error reporting.
	C_TINY_XML_ATTRIBUTE *prev, *next;

public:
	C_TINY_XML_ATTRIBUTE() : C_TINY_XML_BASE()
	{
		document = 0;
		prev = next = 0;
	}
	C_TINY_XML_ATTRIBUTE( const char * _name, const char * _value )
	{
		name = _name;
		value = _value;
		document = 0;
		prev = next = 0;
	}

	LPCSTR Name()  const { return name.c_str(); }
	LPCSTR Value() const { return value.c_str(); }
	int	IntValue() const;
	double DoubleValue() const;

	const C_DARK_STRING& NameTStr() const { return name; }

	int QueryIntValue( int* _value ) const;
	int QueryDoubleValue( double* _value ) const;

	void SetName( LPCSTR _name )	{ name = _name; }
	void SetValue( LPCSTR _value )	{ value = _value; }

	void SetIntValue( int _value );
	void SetDoubleValue( double _value );

	const C_TINY_XML_ATTRIBUTE* Next() const;
	C_TINY_XML_ATTRIBUTE* Next() {
		return const_cast< C_TINY_XML_ATTRIBUTE* >( (const_cast< const C_TINY_XML_ATTRIBUTE* >(this))->Next() ); 
	}

	const C_TINY_XML_ATTRIBUTE* Previous() const;
	C_TINY_XML_ATTRIBUTE* Previous() {
		return const_cast< C_TINY_XML_ATTRIBUTE* >( (const_cast< const C_TINY_XML_ATTRIBUTE* >(this))->Previous() ); 
	}

	BOOL operator==( const C_TINY_XML_ATTRIBUTE& rhs ) const { return rhs.name == name; }
	BOOL operator<( const C_TINY_XML_ATTRIBUTE& rhs )	 const { return name < rhs.name; }
	BOOL operator>( const C_TINY_XML_ATTRIBUTE& rhs )  const { return name > rhs.name; }

	virtual LPCSTR Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding );

	virtual void Print( FILE* cfile, int depth ) const
	{
		Print( cfile, depth, 0 );
	}
	void Print( FILE* cfile, int depth, C_DARK_STRING* str ) const;
	void SetDocument( C_TINY_XML_DOCUMENT* doc )	{ document = doc; }
};


class C_TINY_XML_ATTRIBUTE_SET
{
private:
	C_TINY_XML_ATTRIBUTE_SET( const C_TINY_XML_ATTRIBUTE_SET& );	// not allowed
	void operator=( const C_TINY_XML_ATTRIBUTE_SET& );	// not allowed (as C_TINY_XML_ATTRIBUTE)

	C_TINY_XML_ATTRIBUTE sentinel;

public:
	C_TINY_XML_ATTRIBUTE_SET();
	~C_TINY_XML_ATTRIBUTE_SET();

	void Add( C_TINY_XML_ATTRIBUTE* attribute );
	void Remove( C_TINY_XML_ATTRIBUTE* attribute );

	const C_TINY_XML_ATTRIBUTE* First()	const	{ return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
	C_TINY_XML_ATTRIBUTE* First()					{ return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
	const C_TINY_XML_ATTRIBUTE* Last() const		{ return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }
	C_TINY_XML_ATTRIBUTE* Last()					{ return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }

	const C_TINY_XML_ATTRIBUTE*	Find( LPCSTR _name ) const;
	C_TINY_XML_ATTRIBUTE*	Find( LPCSTR _name ) {
		return const_cast< C_TINY_XML_ATTRIBUTE* >( (const_cast< const C_TINY_XML_ATTRIBUTE_SET* >(this))->Find( _name ) );
	}
};


class C_TINY_XML_ELEMENT : public C_TINY_XML_NODE
{
private:
	C_TINY_XML_ATTRIBUTE_SET attributeSet;

protected:
	void CopyTo( C_TINY_XML_ELEMENT* target ) const;
	void ClearThis();
	LPCSTR ReadValue( LPCSTR in, C_TINY_XML_PARSING_DATA* prevData, _TINY_XML_ENCODING_ encoding );

public:
	C_TINY_XML_ELEMENT (const char * in_value);

	C_TINY_XML_ELEMENT( const C_TINY_XML_ELEMENT& );

	void operator=( const C_TINY_XML_ELEMENT& base );

	virtual ~C_TINY_XML_ELEMENT();

	LPCSTR Attribute( LPCSTR name ) const;
	LPCSTR Attribute( LPCSTR name, int* i ) const;
	LPCSTR Attribute( LPCSTR name, double* d ) const;

	int QueryIntAttribute( LPCSTR name, int* _value ) const;
	int QueryDoubleAttribute( LPCSTR name, double* _value ) const;
	int QueryFloatAttribute( LPCSTR name, float* _value ) const
	{
		double d;
		int result = QueryDoubleAttribute( name, &d );
		if ( result == TINY_XML_SUCCESS )
		{
			*_value = (float)d;
		}
		return result;
	}
	void SetAttribute( LPCSTR name, const char * _value );
	void SetAttribute( const char * name, int value );
	void SetDoubleAttribute( const char * name, double value );
	void RemoveAttribute( const char * name );
    
	const C_TINY_XML_ATTRIBUTE* FirstAttribute() const	{ return attributeSet.First(); }
	C_TINY_XML_ATTRIBUTE* FirstAttribute() 				{ return attributeSet.First(); }
	const C_TINY_XML_ATTRIBUTE* LastAttribute()	const 	{ return attributeSet.Last(); }
	C_TINY_XML_ATTRIBUTE* LastAttribute()					{ return attributeSet.Last(); }

	LPCSTR GetText() const;

	virtual C_TINY_XML_NODE* Clone() const;
	virtual void Print( FILE* cfile, int depth ) const;
	virtual LPCSTR Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding );
	virtual const C_TINY_XML_ELEMENT*     ToElement()     const { return this; }
	virtual C_TINY_XML_ELEMENT*           ToElement()	          { return this; }
	virtual BOOL Accept( C_TINY_XML_VISITOR* visitor ) const;
};

class C_TINY_XML_COMMENT : public C_TINY_XML_NODE
{
protected:
	void CopyTo( C_TINY_XML_COMMENT* target ) const;

public:
	C_TINY_XML_COMMENT() : C_TINY_XML_NODE( C_TINY_XML_NODE::COMMENT ) {}
	C_TINY_XML_COMMENT( LPCSTR _value ) : C_TINY_XML_NODE( C_TINY_XML_NODE::COMMENT )
	{
		SetValue( _value );
	}
	C_TINY_XML_COMMENT( const C_TINY_XML_COMMENT& );
	void operator=( const C_TINY_XML_COMMENT& base );

	virtual ~C_TINY_XML_COMMENT()	{}
	virtual C_TINY_XML_NODE* Clone() const;
	virtual void Print( FILE* cfile, int depth ) const;
	virtual LPCSTR Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding );
	virtual const C_TINY_XML_COMMENT*  ToComment() const { return this; }
	virtual C_TINY_XML_COMMENT*  ToComment() { return this; }
	virtual BOOL Accept( C_TINY_XML_VISITOR* visitor ) const;
};

class C_TINY_XML_TEXT : public C_TINY_XML_NODE
{
	friend class C_TINY_XML_ELEMENT;

private:
	BOOL cdata;

protected :
	void CopyTo( C_TINY_XML_TEXT* target ) const;
	BOOL Blank() const;

	virtual C_TINY_XML_NODE* Clone() const;
	
public:
	C_TINY_XML_TEXT (const char * initValue ) : C_TINY_XML_NODE (C_TINY_XML_NODE::TEXT)
	{
		SetValue( initValue );
		cdata = FALSE;
	}
	C_TINY_XML_TEXT( const C_TINY_XML_TEXT& copy ) : C_TINY_XML_NODE( C_TINY_XML_NODE::TEXT )
	{
		copy.CopyTo( this );
	}
	virtual ~C_TINY_XML_TEXT()
	{

	}

	BOOL CDATA() const				{ return cdata; }
	void SetCDATA( BOOL _cdata )	{ cdata = _cdata; }

	void operator=( const C_TINY_XML_TEXT& base )							 	{ base.CopyTo( this ); }

	virtual void Print( FILE* cfile, int depth ) const;
	virtual LPCSTR Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding );
	virtual const C_TINY_XML_TEXT* ToText() const { return this; }
	virtual C_TINY_XML_TEXT*       ToText()       { return this; }
	virtual BOOL Accept( C_TINY_XML_VISITOR* content ) const;
};

class C_TINY_XML_DECLARATION : public C_TINY_XML_NODE
{
private:
	C_DARK_STRING version, encoding, standalone;
	
protected:
	void CopyTo( C_TINY_XML_DECLARATION* target ) const;

public:
	C_TINY_XML_DECLARATION() : C_TINY_XML_NODE( C_TINY_XML_NODE::DECLARATION )
	{

	}
	C_TINY_XML_DECLARATION(	LPCSTR _version, LPCSTR _encoding, LPCSTR _standalone );
	C_TINY_XML_DECLARATION( const C_TINY_XML_DECLARATION& copy );
	virtual ~C_TINY_XML_DECLARATION()
	{

	}
	
	const char *Version() const			{ return version.c_str (); }
	const char *Encoding() const		{ return encoding.c_str (); }
	const char *Standalone() const		{ return standalone.c_str (); }

	void operator=( const C_TINY_XML_DECLARATION& copy );

	virtual C_TINY_XML_NODE* Clone() const;
	virtual void Print( FILE* cfile, int depth, C_DARK_STRING* str ) const;
	virtual void Print( FILE* cfile, int depth ) const { Print( cfile, depth, 0 ); }
	virtual LPCSTR Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding );
	virtual const C_TINY_XML_DECLARATION* ToDeclaration() const { return this; }
	virtual C_TINY_XML_DECLARATION*       ToDeclaration()       { return this; }
	virtual BOOL Accept( C_TINY_XML_VISITOR* visitor ) const;
};

class C_TINY_XML_UNKNOWN : public C_TINY_XML_NODE
{
protected:
	void CopyTo( C_TINY_XML_UNKNOWN* target ) const;

public:
	C_TINY_XML_UNKNOWN() : C_TINY_XML_NODE( C_TINY_XML_NODE::UNKNOWN )
	{

	}
	C_TINY_XML_UNKNOWN( const C_TINY_XML_UNKNOWN& copy ) : C_TINY_XML_NODE( C_TINY_XML_NODE::UNKNOWN )
	{
		copy.CopyTo( this );
	}
	virtual ~C_TINY_XML_UNKNOWN()
	{

	}

	void operator=( const C_TINY_XML_UNKNOWN& copy )
	{
		copy.CopyTo( this );
	}

	virtual C_TINY_XML_NODE* Clone() const;
	virtual void Print( FILE* cfile, int depth ) const;
	virtual LPCSTR Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding );
	virtual const C_TINY_XML_UNKNOWN*     ToUnknown()     const { return this; }
	virtual C_TINY_XML_UNKNOWN*           ToUnknown()	    { return this; }
	virtual BOOL Accept( C_TINY_XML_VISITOR* content ) const;
};


class C_TINY_XML_DOCUMENT : public C_TINY_XML_NODE
{
private:
	C_DARK_STRING errorDesc;
	_TINY_XML_CURSOR errorLocation;

	BOOL error, useMicrosoftBOM;
	int  errorId, tabsize;
	
	void CopyTo( C_TINY_XML_DOCUMENT* target ) const;

protected :
	virtual C_TINY_XML_NODE* Clone() const;

public:
	C_TINY_XML_DOCUMENT();
	C_TINY_XML_DOCUMENT( const char * documentName );
	C_TINY_XML_DOCUMENT( const C_TINY_XML_DOCUMENT& copy );
	virtual ~C_TINY_XML_DOCUMENT() {}

	BOOL LoadFile( _TINY_XML_ENCODING_ encoding = TINY_XML_DEFAULT_ENCODING );
	BOOL SaveFile() const;
	BOOL LoadFile( const char * filename, _TINY_XML_ENCODING_ encoding = TINY_XML_DEFAULT_ENCODING );
	BOOL SaveFile( const char * filename ) const;
	BOOL LoadFile( FILE*, _TINY_XML_ENCODING_ encoding = TINY_XML_DEFAULT_ENCODING );
	BOOL SaveFile( FILE* ) const;

	void operator=( const C_TINY_XML_DOCUMENT& copy );

	virtual LPCSTR Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data = 0, _TINY_XML_ENCODING_ encoding = TINY_XML_DEFAULT_ENCODING );

	const C_TINY_XML_ELEMENT* RootElement() const		{ return FirstChildElement(); }
	C_TINY_XML_ELEMENT* RootElement()					{ return FirstChildElement(); }

	BOOL Error() const						{ return error; }
	const char * ErrorDesc() const	{ return errorDesc.c_str (); }
	int ErrorId() const				{ return errorId; }
	int ErrorRow() const	{ return errorLocation.row+1; }
	int ErrorCol() const	{ return errorLocation.col+1; }
	void SetTabSize( int _tabsize )		{ tabsize = _tabsize; }
	int TabSize() const	{ return tabsize; }
	void ClearError()
	{
		error = FALSE; 
		errorId = 0; 
		errorDesc = ""; 
		errorLocation.row = errorLocation.col = 0; 
	}

	void Print() const						{ Print( stdout, 0 ); }
	void SetError( int err, LPCSTR errorLocation, C_TINY_XML_PARSING_DATA* prevData, _TINY_XML_ENCODING_ encoding );

	virtual void Print( FILE* cfile, int depth = 0 ) const;
	virtual const C_TINY_XML_DOCUMENT*    ToDocument() const { return this; }
	virtual C_TINY_XML_DOCUMENT*          ToDocument() { return this; }
	virtual BOOL Accept( C_TINY_XML_VISITOR* content ) const;
};


class C_TINY_XML_HANDLE
{
private:
	C_TINY_XML_NODE* node;

public:
	C_TINY_XML_HANDLE( C_TINY_XML_NODE* _node )					{ this->node = _node; }
	C_TINY_XML_HANDLE( const C_TINY_XML_HANDLE& ref )			{ this->node = ref.node; }
	C_TINY_XML_HANDLE operator=( const C_TINY_XML_HANDLE& ref ) { this->node = ref.node; return *this; }

	C_TINY_XML_HANDLE FirstChild() const;
	C_TINY_XML_HANDLE FirstChild( const char * value ) const;
	C_TINY_XML_HANDLE FirstChildElement() const;
	C_TINY_XML_HANDLE FirstChildElement( const char * value ) const;
	C_TINY_XML_HANDLE Child( LPCSTR value, int index ) const;
	C_TINY_XML_HANDLE Child( int index ) const;
	C_TINY_XML_HANDLE ChildElement( LPCSTR value, int index ) const;
	C_TINY_XML_HANDLE ChildElement( int index ) const;
	C_TINY_XML_NODE* ToNode() const			{ return node; } 
	C_TINY_XML_ELEMENT* ToElement() const		{ return ( ( node && node->ToElement() ) ? node->ToElement() : 0 ); }
	C_TINY_XML_TEXT* ToText() const			{ return ( ( node && node->ToText() ) ? node->ToText() : 0 ); }
	C_TINY_XML_UNKNOWN* ToUnknown() const		{ return ( ( node && node->ToUnknown() ) ? node->ToUnknown() : 0 ); }
	C_TINY_XML_NODE* Node() const			{ return ToNode(); } 
	C_TINY_XML_ELEMENT* Element() const	{ return ToElement(); }
	C_TINY_XML_TEXT* Text() const			{ return ToText(); }
	C_TINY_XML_UNKNOWN* Unknown() const	{ return ToUnknown(); }

};


class DarkXmlPrinter : public C_TINY_XML_VISITOR
{
private:
	C_DARK_STRING buffer, indent, lineBreak;
	BOOL simpleTextPrint;
	int depth;
	
	void DoIndent()
	{
		for( int i=0; i<depth; ++i )
		{
			buffer += indent;
		}
	}
	void DoLineBreak()
	{
		buffer += lineBreak;
	}
	
public:
	DarkXmlPrinter() : depth( 0 ), simpleTextPrint( FALSE ), buffer(), indent( "    " ), lineBreak( "\n" )
	{

	}

	void SetIndent( LPCSTR _indent ) { indent = _indent ? _indent : "" ; }
	LPCSTR Indent() { return indent.c_str(); }
	void SetLineBreak( LPCSTR _lineBreak ) { lineBreak = _lineBreak ? _lineBreak : ""; }
	LPCSTR LineBreak() { return lineBreak.c_str(); }
	void SetStreamPrinting()
	{
		indent = "";
		lineBreak = "";
	}	
	LPCSTR CStr() { return buffer.c_str(); }
	size_t Size() { return buffer.size(); }

	virtual BOOL VisitEnter( const C_TINY_XML_DOCUMENT& doc );
	virtual BOOL VisitExit( const C_TINY_XML_DOCUMENT& doc );

	virtual BOOL VisitEnter( const C_TINY_XML_ELEMENT& element, const C_TINY_XML_ATTRIBUTE* firstAttribute );
	virtual BOOL VisitExit( const C_TINY_XML_ELEMENT& element );

	virtual BOOL Visit( const C_TINY_XML_DECLARATION& declaration );
	virtual BOOL Visit( const C_TINY_XML_TEXT& text );
	virtual BOOL Visit( const C_TINY_XML_COMMENT& comment );
	virtual BOOL Visit( const C_TINY_XML_UNKNOWN& unknown );
};

#pragma warning( pop )

#endif

