#include "stdafx.h"
#include "DarkXML.h"


BOOL C_DARK_XML_BASE::condenseWhiteSpace = true;

const char* C_DARK_XML_BASE::errorString[ DARK_XML_ERROR_STRING_COUNT ] =
{
	"No error",
	"Error",
	"Failed to open file",
	"Memory allocation failed.",
	"Error parsing Element.",
	"Failed to read Element name",
	"Error reading Element value.",
	"Error reading Attributes.",
	"Error: empty tag.",
	"Error reading end tag.",
	"Error parsing Unknown.",
	"Error parsing Comment.",
	"Error parsing Declaration.",
	"Error document empty.",
	"Error null (0) or unexpected EOF found in input stream.",
	"Error parsing CDATA.",
	"Error when C_DARK_XML_DOCUMENT added to document, because C_DARK_XML_DOCUMENT can only be at the root.",
};

// Microsoft compiler security
FILE* DarkXmlFOpen( const char* filename, const char* mode )
{
	#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
		FILE* fp = 0;
		errno_t err = fopen_s( &fp, filename, mode );
		if ( !err && fp )
			return fp;
		return 0;
	#else
		return fopen( filename, mode );
	#endif
}

void C_DARK_XML_BASE::EncodeString( const std::string& str, std::string* outString )
{
	int i=0;

	while( i<(int)str.length() )
	{
		unsigned char c = (unsigned char) str[i];

		if (    c == '&' 
		     && i < ( (int)str.length() - 2 )
			 && str[i+1] == '#'
			 && str[i+2] == 'x' )
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while ( i<(int)str.length()-1 )
			{
				outString->append( str.c_str() + i, 1 );
				++i;
				if ( str[i] == ';' )
					break;
			}
		}
		else if ( c == '&' )
		{
			outString->append( entity[0].str, entity[0].strLength );
			++i;
		}
		else if ( c == '<' )
		{
			outString->append( entity[1].str, entity[1].strLength );
			++i;
		}
		else if ( c == '>' )
		{
			outString->append( entity[2].str, entity[2].strLength );
			++i;
		}
		else if ( c == '\"' )
		{
			outString->append( entity[3].str, entity[3].strLength );
			++i;
		}
		else if ( c == '\'' )
		{
			outString->append( entity[4].str, entity[4].strLength );
			++i;
		}
		else if ( c < 32 )
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[ 32 ];
			
			#if defined(DARK_XML_SNPRINTF)		
				DARK_XML_SNPRINTF( buf, sizeof(buf), "&#x%02X;", (unsigned) ( c & 0xff ) );
			#else
				sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
			#endif		

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append( buf, (int)strlen( buf ) );
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append( &realc, 1 );
			*outString += (char) c;	// somewhat more efficient function call.
			++i;
		}
	}
}


C_DARK_XML_NODE::C_DARK_XML_NODE( NodeType _type ) : C_DARK_XML_BASE()
{
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}


C_DARK_XML_NODE::~C_DARK_XML_NODE()
{
	C_DARK_XML_NODE* node = firstChild;
	C_DARK_XML_NODE* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	
}


void C_DARK_XML_NODE::CopyTo( C_DARK_XML_NODE* target ) const
{
	target->SetValue (value.c_str() );
	target->userData = userData; 
}


void C_DARK_XML_NODE::Clear()
{
	C_DARK_XML_NODE* node = firstChild;
	C_DARK_XML_NODE* temp = 0;

	while ( node )
	{
		temp = node;
		node = node->next;
		delete temp;
	}	

	firstChild = 0;
	lastChild = 0;
}


C_DARK_XML_NODE* C_DARK_XML_NODE::LinkEndChild( C_DARK_XML_NODE* node )
{
	if ( node->Type() == C_DARK_XML_NODE::DOCUMENT )
	{
		delete node;
		if ( GetDocument() ) GetDocument()->SetError( DARK_XML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, DARK_XML_ENCODING_UNKNOWN );
		return 0;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if ( lastChild )
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}


C_DARK_XML_NODE* C_DARK_XML_NODE::InsertEndChild( const C_DARK_XML_NODE& addThis )
{
	if ( addThis.Type() == C_DARK_XML_NODE::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( DARK_XML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, DARK_XML_ENCODING_UNKNOWN );
		return 0;
	}
	C_DARK_XML_NODE* node = addThis.Clone();
	if ( !node )
		return 0;

	return LinkEndChild( node );
}


C_DARK_XML_NODE* C_DARK_XML_NODE::InsertBeforeChild( C_DARK_XML_NODE* beforeThis, const C_DARK_XML_NODE& addThis )
{	
	if ( !beforeThis || beforeThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == C_DARK_XML_NODE::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( DARK_XML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, DARK_XML_ENCODING_UNKNOWN );
		return 0;
	}

	C_DARK_XML_NODE* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if ( beforeThis->prev )
	{
		beforeThis->prev->next = node;
	}
	else
	{
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}


C_DARK_XML_NODE* C_DARK_XML_NODE::InsertAfterChild( C_DARK_XML_NODE* afterThis, const C_DARK_XML_NODE& addThis )
{
	if ( !afterThis || afterThis->parent != this ) {
		return 0;
	}
	if ( addThis.Type() == C_DARK_XML_NODE::DOCUMENT )
	{
		if ( GetDocument() ) GetDocument()->SetError( DARK_XML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, DARK_XML_ENCODING_UNKNOWN );
		return 0;
	}

	C_DARK_XML_NODE* node = addThis.Clone();
	if ( !node )
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if ( afterThis->next )
	{
		afterThis->next->prev = node;
	}
	else
	{
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}


C_DARK_XML_NODE* C_DARK_XML_NODE::ReplaceChild( C_DARK_XML_NODE* replaceThis, const C_DARK_XML_NODE& withThis )
{
	if ( replaceThis->parent != this )
		return 0;

	C_DARK_XML_NODE* node = withThis.Clone();
	if ( !node )
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if ( replaceThis->next )
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if ( replaceThis->prev )
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}


BOOL C_DARK_XML_NODE::RemoveChild( C_DARK_XML_NODE* removeThis )
{
	if ( removeThis->parent != this )
	{	
		return false;
	}

	if ( removeThis->next )
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if ( removeThis->prev )
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

const C_DARK_XML_NODE* C_DARK_XML_NODE::FirstChild( const char * _value ) const
{
	const C_DARK_XML_NODE* node;
	for ( node = firstChild; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const C_DARK_XML_NODE* C_DARK_XML_NODE::LastChild( const char * _value ) const
{
	const C_DARK_XML_NODE* node;
	for ( node = lastChild; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const C_DARK_XML_NODE* C_DARK_XML_NODE::IterateChildren( const C_DARK_XML_NODE* previous ) const
{
	if ( !previous )
	{
		return FirstChild();
	}
	else
	{
		return previous->NextSibling();
	}
}


const C_DARK_XML_NODE* C_DARK_XML_NODE::IterateChildren( const char * val, const C_DARK_XML_NODE* previous ) const
{
	if ( !previous )
	{
		return FirstChild( val );
	}
	else
	{
		return previous->NextSibling( val );
	}
}


const C_DARK_XML_NODE* C_DARK_XML_NODE::NextSibling( const char * _value ) const 
{
	const C_DARK_XML_NODE* node;
	for ( node = next; node; node = node->next )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


const C_DARK_XML_NODE* C_DARK_XML_NODE::PreviousSibling( const char * _value ) const
{
	const C_DARK_XML_NODE* node;
	for ( node = prev; node; node = node->prev )
	{
		if ( strcmp( node->Value(), _value ) == 0 )
			return node;
	}
	return 0;
}


void C_DARK_XML_ELEMENT::RemoveAttribute( const char * name )
{
	C_DARK_XML_ATTRIBUTE* node = attributeSet.Find( name );
	if ( node )
	{
		attributeSet.Remove( node );
		delete node;
	}
}

const C_DARK_XML_ELEMENT* C_DARK_XML_NODE::FirstChildElement() const
{
	const C_DARK_XML_NODE* node;

	for (	node = FirstChild();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const C_DARK_XML_ELEMENT* C_DARK_XML_NODE::FirstChildElement( const char * _value ) const
{
	const C_DARK_XML_NODE* node;

	for (	node = FirstChild( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const C_DARK_XML_ELEMENT* C_DARK_XML_NODE::NextSiblingElement() const
{
	const C_DARK_XML_NODE* node;

	for (	node = NextSibling();
			node;
			node = node->NextSibling() )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const C_DARK_XML_ELEMENT* C_DARK_XML_NODE::NextSiblingElement( const char * _value ) const
{
	const C_DARK_XML_NODE* node;

	for (	node = NextSibling( _value );
			node;
			node = node->NextSibling( _value ) )
	{
		if ( node->ToElement() )
			return node->ToElement();
	}
	return 0;
}


const C_DARK_XML_DOCUMENT* C_DARK_XML_NODE::GetDocument() const
{
	const C_DARK_XML_NODE* node;

	for( node = this; node; node = node->parent )
	{
		if ( node->ToDocument() )
			return node->ToDocument();
	}
	return 0;
}


C_DARK_XML_ELEMENT::C_DARK_XML_ELEMENT (const char * _value) : C_DARK_XML_NODE( C_DARK_XML_NODE::ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}


C_DARK_XML_ELEMENT::C_DARK_XML_ELEMENT( const std::string& _value ) : C_DARK_XML_NODE( C_DARK_XML_NODE::ELEMENT )
{
	firstChild = lastChild = 0;
	value = _value;
}


C_DARK_XML_ELEMENT::C_DARK_XML_ELEMENT( const C_DARK_XML_ELEMENT& copy)
	: C_DARK_XML_NODE( C_DARK_XML_NODE::ELEMENT )
{
	firstChild = lastChild = 0;
	copy.CopyTo( this );	
}


void C_DARK_XML_ELEMENT::operator=( const C_DARK_XML_ELEMENT& base )
{
	ClearThis();
	base.CopyTo( this );
}


C_DARK_XML_ELEMENT::~C_DARK_XML_ELEMENT()
{
	ClearThis();
}


void C_DARK_XML_ELEMENT::ClearThis()
{
	Clear();
	while( attributeSet.First() )
	{
		C_DARK_XML_ATTRIBUTE* node = attributeSet.First();
		attributeSet.Remove( node );
		delete node;
	}
}


const char* C_DARK_XML_ELEMENT::Attribute( const char* name ) const
{
	const C_DARK_XML_ATTRIBUTE* node = attributeSet.Find( name );
	if ( node )
		return node->Value();
	return 0;
}


const std::string* C_DARK_XML_ELEMENT::Attribute( const std::string& name ) const
{
	const C_DARK_XML_ATTRIBUTE* node = attributeSet.Find( name );
	if ( node )
	{
		return &node->ValueStr();
	}
	return 0;
}

const char* C_DARK_XML_ELEMENT::Attribute( const char* name, int* i ) const
{
	const char* s = Attribute( name );
	if ( i )
	{
		if ( s ) {
			*i = atoi( s );
		}
		else {
			*i = 0;
		}
	}
	return s;
}

const std::string* C_DARK_XML_ELEMENT::Attribute( const std::string& name, int* i ) const
{
	const std::string* s = Attribute( name );
	if ( i )
	{
		if ( s ) {
			*i = atoi( s->c_str() );
		}
		else {
			*i = 0;
		}
	}
	return s;
}

const char* C_DARK_XML_ELEMENT::Attribute( const char* name, double* d ) const
{
	const char* s = Attribute( name );
	if ( d )
	{
		if ( s ) {
			*d = atof( s );
		}
		else {
			*d = 0;
		}
	}
	return s;
}

const std::string* C_DARK_XML_ELEMENT::Attribute( const std::string& name, double* d ) const
{
	const std::string* s = Attribute( name );
	if ( d )
	{
		if ( s ) {
			*d = atof( s->c_str() );
		}
		else {
			*d = 0;
		}
	}
	return s;
}

int C_DARK_XML_ELEMENT::QueryIntAttribute( const char* name, int* ival ) const
{
	const C_DARK_XML_ATTRIBUTE* node = attributeSet.Find( name );
	if ( !node )
		return DARK_XML_NO_ATTRIBUTE;
	return node->QueryIntValue( ival );
}

int C_DARK_XML_ELEMENT::QueryIntAttribute( const std::string& name, int* ival ) const
{
	const C_DARK_XML_ATTRIBUTE* node = attributeSet.Find( name );
	if ( !node )
		return DARK_XML_NO_ATTRIBUTE;
	return node->QueryIntValue( ival );
}

int C_DARK_XML_ELEMENT::QueryDoubleAttribute( const char* name, double* dval ) const
{
	const C_DARK_XML_ATTRIBUTE* node = attributeSet.Find( name );
	if ( !node )
		return DARK_XML_NO_ATTRIBUTE;
	return node->QueryDoubleValue( dval );
}

int C_DARK_XML_ELEMENT::QueryDoubleAttribute( const std::string& name, double* dval ) const
{
	const C_DARK_XML_ATTRIBUTE* node = attributeSet.Find( name );
	if ( !node )
		return DARK_XML_NO_ATTRIBUTE;
	return node->QueryDoubleValue( dval );
}

void C_DARK_XML_ELEMENT::SetAttribute( const char * name, int val )
{	
	char buf[64];
	#if defined(DARK_XML_SNPRINTF)		
		DARK_XML_SNPRINTF( buf, sizeof(buf), "%d", val );
	#else
		sprintf( buf, "%d", val );
	#endif
	SetAttribute( name, buf );
}

void C_DARK_XML_ELEMENT::SetAttribute( const std::string& name, int val )
{	
   std::ostringstream oss;
   oss << val;
   SetAttribute( name, oss.str() );
}

void C_DARK_XML_ELEMENT::SetDoubleAttribute( const char * name, double val )
{	
	char buf[256];
	#if defined(DARK_XML_SNPRINTF)		
		DARK_XML_SNPRINTF( buf, sizeof(buf), "%f", val );
	#else
		sprintf( buf, "%f", val );
	#endif
	SetAttribute( name, buf );
}


void C_DARK_XML_ELEMENT::SetAttribute( const char * cname, const char * cvalue )
{
	C_DARK_XML_ATTRIBUTE* node = attributeSet.Find( cname );
	if ( node )
	{
		node->SetValue( cvalue );
		return;
	}
	C_DARK_XML_ATTRIBUTE* attrib = new C_DARK_XML_ATTRIBUTE( cname, cvalue );
	if ( attrib )
	{
		attributeSet.Add( attrib );
	}
	else
	{
		C_DARK_XML_DOCUMENT* document = GetDocument();
		if ( document ) document->SetError( DARK_XML_ERROR_OUT_OF_MEMORY, 0, 0, DARK_XML_ENCODING_UNKNOWN );
	}
}

void C_DARK_XML_ELEMENT::SetAttribute( const std::string& name, const std::string& _value )
{
	C_DARK_XML_ATTRIBUTE* node = attributeSet.Find( name );
	if ( node )
	{
		node->SetValue( _value );
		return;
	}

	C_DARK_XML_ATTRIBUTE* attrib = new C_DARK_XML_ATTRIBUTE( name, _value );
	if ( attrib )
	{
		attributeSet.Add( attrib );
	}
	else
	{
		C_DARK_XML_DOCUMENT* document = GetDocument();
		if ( document ) document->SetError( DARK_XML_ERROR_OUT_OF_MEMORY, 0, 0, DARK_XML_ENCODING_UNKNOWN );
	}
}


void C_DARK_XML_ELEMENT::Print( FILE* cfile, int depth ) const
{
	int i;
	for ( i=0; i<depth; i++ ) {
		fprintf( cfile, "    " );
	}

	fprintf( cfile, "<%s", value.c_str() );

	const C_DARK_XML_ATTRIBUTE* attrib;
	for ( attrib = attributeSet.First(); attrib; attrib = attrib->Next() )
	{
		fprintf( cfile, " " );
		attrib->Print( cfile, depth );
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	C_DARK_XML_NODE* node;
	if ( !firstChild )
	{
		fprintf( cfile, " />" );
	}
	else if ( firstChild == lastChild && firstChild->ToText() )
	{
		fprintf( cfile, ">" );
		firstChild->Print( cfile, depth + 1 );
		fprintf( cfile, "</%s>", value.c_str() );
	}
	else
	{
		fprintf( cfile, ">" );

		for ( node = firstChild; node; node=node->NextSibling() )
		{
			if ( !node->ToText() )
			{
				fprintf( cfile, "\n" );
			}
			node->Print( cfile, depth+1 );
		}
		fprintf( cfile, "\n" );
		for( i=0; i<depth; ++i ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "</%s>", value.c_str() );
	}
}


void C_DARK_XML_ELEMENT::CopyTo( C_DARK_XML_ELEMENT* target ) const
{
	// superclass:
	C_DARK_XML_NODE::CopyTo( target );

	// Element class: 
	// Clone the attributes, then clone the children.
	const C_DARK_XML_ATTRIBUTE* attribute = 0;
	for(	attribute = attributeSet.First();
	attribute;
	attribute = attribute->Next() )
	{
		target->SetAttribute( attribute->Name(), attribute->Value() );
	}

	C_DARK_XML_NODE* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}
}

BOOL C_DARK_XML_ELEMENT::Accept( C_DARK_XML_VISITOR* visitor ) const
{
	if ( visitor->VisitEnter( *this, attributeSet.First() ) ) 
	{
		for ( const C_DARK_XML_NODE* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}


C_DARK_XML_NODE* C_DARK_XML_ELEMENT::Clone() const
{
	C_DARK_XML_ELEMENT* clone = new C_DARK_XML_ELEMENT( Value() );
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


const char* C_DARK_XML_ELEMENT::GetText() const
{
	const C_DARK_XML_NODE* child = this->FirstChild();
	if ( child ) {
		const C_DARK_XML_TEXT* childText = child->ToText();
		if ( childText ) {
			return childText->Value();
		}
	}
	return 0;
}


C_DARK_XML_DOCUMENT::C_DARK_XML_DOCUMENT() : C_DARK_XML_NODE( C_DARK_XML_NODE::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError();
}

C_DARK_XML_DOCUMENT::C_DARK_XML_DOCUMENT( const char * documentName ) : C_DARK_XML_NODE( C_DARK_XML_NODE::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}

C_DARK_XML_DOCUMENT::C_DARK_XML_DOCUMENT( const std::string& documentName ) : C_DARK_XML_NODE( C_DARK_XML_NODE::DOCUMENT )
{
	tabsize = 4;
	useMicrosoftBOM = false;
    value = documentName;
	ClearError();
}

C_DARK_XML_DOCUMENT::C_DARK_XML_DOCUMENT( const C_DARK_XML_DOCUMENT& copy ) : C_DARK_XML_NODE( C_DARK_XML_NODE::DOCUMENT )
{
	copy.CopyTo( this );
}


void C_DARK_XML_DOCUMENT::operator=( const C_DARK_XML_DOCUMENT& copy )
{
	Clear();
	copy.CopyTo( this );
}


BOOL C_DARK_XML_DOCUMENT::LoadFile( DarkXmlEncoding encoding )
{
	// See STL_STRING_BUG below.
	//StringToBuffer buf( value );

	return LoadFile( Value(), encoding );
}


BOOL C_DARK_XML_DOCUMENT::SaveFile() const
{
	// See STL_STRING_BUG below.
//	StringToBuffer buf( value );
//
//	if ( buf.buffer && SaveFile( buf.buffer ) )
//		return true;
//
//	return false;
	return SaveFile( Value() );
}

BOOL C_DARK_XML_DOCUMENT::LoadFile( const char* _filename, DarkXmlEncoding encoding )
{
	// There was a really terrifying little bug here. The code:
	//		value = filename
	// in the STL case, cause the assignment method of the std::string to
	// be called. What is strange, is that the std::string had the same
	// address as it's c_str() method, and so bad things happen. Looks
	// like a bug in the Microsoft STL implementation.
	// Add an extra string to avoid the crash.
	std::string filename( _filename );
	value = filename;

	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = DarkXmlFOpen( value.c_str (), "rb" );	

	if ( file )
	{
		BOOL result = LoadFile( file, encoding );
		fclose( file );
		return result;
	}
	else
	{
		SetError( DARK_XML_ERROR_OPENING_FILE, 0, 0, DARK_XML_ENCODING_UNKNOWN );
		return false;
	}
}

BOOL C_DARK_XML_DOCUMENT::LoadFile( FILE* file, DarkXmlEncoding encoding )
{
	if ( !file ) 
	{
		SetError( DARK_XML_ERROR_OPENING_FILE, 0, 0, DARK_XML_ENCODING_UNKNOWN );
		return false;
	}

	// Delete the existing data:
	Clear();
	location.Clear();

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek( file, 0, SEEK_END );
	length = ftell( file );
	fseek( file, 0, SEEK_SET );

	// Strange case, but good to handle up front.
	if ( length <= 0 )
	{
		SetError( DARK_XML_ERROR_DOCUMENT_EMPTY, 0, 0, DARK_XML_ENCODING_UNKNOWN );
		return false;
	}

	// If we have a file, assume it is all one big XML file, and read it in.
	// The document parser may decide the document ends sooner than the entire file, however.
	std::string data;
	data.reserve( length );

	// Subtle bug here. TinyXml did use fgets. But from the XML spec:
	// 2.11 End-of-Line Handling
	// <snip>
	// <quote>
	// ...the XML processor MUST behave as if it normalized all line breaks in external 
	// parsed entities (including the document entity) on input, before parsing, by translating 
	// both the two-character sequence #xD #xA and any #xD that is not followed by #xA to 
	// a single #xA character.
	// </quote>
	//
	// It is not clear fgets does that, and certainly isn't clear it works cross platform. 
	// Generally, you expect fgets to translate from the convention of the OS to the c/unix
	// convention, and not work generally.

	/*
	while( fgets( buf, sizeof(buf), file ) )
	{
		data += buf;
	}
	*/

	char* buf = new char[ length+1 ];
	buf[0] = 0;

	if ( fread( buf, length, 1, file ) != 1 ) {
		delete [] buf;
		SetError( DARK_XML_ERROR_OPENING_FILE, 0, 0, DARK_XML_ENCODING_UNKNOWN );
		return false;
	}

	const char* lastPos = buf;
	const char* p = buf;

	buf[length] = 0;
	while( *p ) {
		if ( *p == 0xa ) {
			// Newline character. No special rules for this. Append all the characters
			// since the last string, and include the newline.
			data.append( lastPos, (p-lastPos+1) );	// append, include the newline
			++p;									// move past the newline
			lastPos = p;							// and point to the new buffer (may be 0)
		}
		else if ( *p == 0xd ) {
			// Carriage return. Append what we have so far, then
			// handle moving forward in the buffer.
			if ( (p-lastPos) > 0 ) {
				data.append( lastPos, p-lastPos );	// do not add the CR
			}
			data += (char)0xa;						// a proper newline

			if ( *(p+1) == 0xa ) {
				// Carriage return - new line sequence
				p += 2;
				lastPos = p;
			}
			else {
				// it was followed by something else...that is presumably characters again.
				++p;
				lastPos = p;

			}
		}
		else {
			++p;
		}
	}
	// Handle any left over characters.
	if ( p-lastPos ) {
		data.append( lastPos, p-lastPos );
	}		
	delete [] buf;
	buf = 0;

	Parse( data.c_str(), 0, encoding );

	if (  Error() )
        return false;
    else
		return true;
}


BOOL C_DARK_XML_DOCUMENT::SaveFile( const char * filename ) const
{
	// The old c stuff lives on...
	FILE* fp = DarkXmlFOpen( filename, "w" );
	if ( fp )
	{
		BOOL result = SaveFile( fp );
		fclose( fp );
		return result;
	}
	return false;
}


BOOL C_DARK_XML_DOCUMENT::SaveFile( FILE* fp ) const
{
	if ( useMicrosoftBOM ) 
	{
		const unsigned char DARK_XML_UTF_LEAD_0 = 0xefU;
		const unsigned char DARK_XML_UTF_LEAD_1 = 0xbbU;
		const unsigned char DARK_XML_UTF_LEAD_2 = 0xbfU;

		fputc( DARK_XML_UTF_LEAD_0, fp );
		fputc( DARK_XML_UTF_LEAD_1, fp );
		fputc( DARK_XML_UTF_LEAD_2, fp );
	}
	Print( fp, 0 );
	return (ferror(fp) == 0);
}


void C_DARK_XML_DOCUMENT::CopyTo( C_DARK_XML_DOCUMENT* target ) const
{
	C_DARK_XML_NODE::CopyTo( target );

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	C_DARK_XML_NODE* node = 0;
	for ( node = firstChild; node; node = node->NextSibling() )
	{
		target->LinkEndChild( node->Clone() );
	}	
}


C_DARK_XML_NODE* C_DARK_XML_DOCUMENT::Clone() const
{
	C_DARK_XML_DOCUMENT* clone = new C_DARK_XML_DOCUMENT();
	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void C_DARK_XML_DOCUMENT::Print( FILE* cfile, int depth ) const
{
	for ( const C_DARK_XML_NODE* node=FirstChild(); node; node=node->NextSibling() )
	{
		node->Print( cfile, depth );
		fprintf( cfile, "\n" );
	}
}


BOOL C_DARK_XML_DOCUMENT::Accept( C_DARK_XML_VISITOR* visitor ) const
{
	if ( visitor->VisitEnter( *this ) )
	{
		for ( const C_DARK_XML_NODE* node=FirstChild(); node; node=node->NextSibling() )
		{
			if ( !node->Accept( visitor ) )
				break;
		}
	}
	return visitor->VisitExit( *this );
}


const C_DARK_XML_ATTRIBUTE* C_DARK_XML_ATTRIBUTE::Next() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}

/*
C_DARK_XML_ATTRIBUTE* C_DARK_XML_ATTRIBUTE::Next()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}
*/

const C_DARK_XML_ATTRIBUTE* C_DARK_XML_ATTRIBUTE::Previous() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}

/*
C_DARK_XML_ATTRIBUTE* C_DARK_XML_ATTRIBUTE::Previous()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}
*/

void C_DARK_XML_ATTRIBUTE::Print( FILE* cfile, int /*depth*/, std::string* str ) const
{
	std::string n, v;

	EncodeString( name, &n );
	EncodeString( value, &v );

	if (value.find ('\"') == std::string::npos) {
		if ( cfile ) {
		fprintf (cfile, "%s=\"%s\"", n.c_str(), v.c_str() );
		}
		if ( str ) {
			(*str) += n; (*str) += "=\""; (*str) += v; (*str) += "\"";
		}
	}
	else {
		if ( cfile ) {
		fprintf (cfile, "%s='%s'", n.c_str(), v.c_str() );
		}
		if ( str ) {
			(*str) += n; (*str) += "='"; (*str) += v; (*str) += "'";
		}
	}
}


int C_DARK_XML_ATTRIBUTE::QueryIntValue( int* ival ) const
{
	if ( DARK_XML_SSCANF( value.c_str(), "%d", ival ) == 1 )
		return DARK_XML_SUCCESS;
	return DARK_XML_WRONG_TYPE;
}

int C_DARK_XML_ATTRIBUTE::QueryDoubleValue( double* dval ) const
{
	if ( DARK_XML_SSCANF( value.c_str(), "%lf", dval ) == 1 )
		return DARK_XML_SUCCESS;
	return DARK_XML_WRONG_TYPE;
}

void C_DARK_XML_ATTRIBUTE::SetIntValue( int _value )
{
	char buf [64];
	#if defined(DARK_XML_SNPRINTF)		
		DARK_XML_SNPRINTF(buf, sizeof(buf), "%d", _value);
	#else
		sprintf (buf, "%d", _value);
	#endif
	SetValue (buf);
}

void C_DARK_XML_ATTRIBUTE::SetDoubleValue( double _value )
{
	char buf [256];
	#if defined(DARK_XML_SNPRINTF)		
		DARK_XML_SNPRINTF( buf, sizeof(buf), "%lf", _value);
	#else
		sprintf (buf, "%lf", _value);
	#endif
	SetValue (buf);
}

int C_DARK_XML_ATTRIBUTE::IntValue() const
{
	return atoi (value.c_str ());
}

double  C_DARK_XML_ATTRIBUTE::DoubleValue() const
{
	return atof (value.c_str ());
}


C_DARK_XML_COMMENT::C_DARK_XML_COMMENT( const C_DARK_XML_COMMENT& copy ) : C_DARK_XML_NODE( C_DARK_XML_NODE::COMMENT )
{
	copy.CopyTo( this );
}


void C_DARK_XML_COMMENT::operator=( const C_DARK_XML_COMMENT& base )
{
	Clear();
	base.CopyTo( this );
}


void C_DARK_XML_COMMENT::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
	{
		fprintf( cfile,  "    " );
	}
	fprintf( cfile, "<!--%s-->", value.c_str() );
}


void C_DARK_XML_COMMENT::CopyTo( C_DARK_XML_COMMENT* target ) const
{
	C_DARK_XML_NODE::CopyTo( target );
}


BOOL C_DARK_XML_COMMENT::Accept( C_DARK_XML_VISITOR* visitor ) const
{
	return visitor->Visit( *this );
}


C_DARK_XML_NODE* C_DARK_XML_COMMENT::Clone() const
{
	C_DARK_XML_COMMENT* clone = new C_DARK_XML_COMMENT();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void C_DARK_XML_TEXT::Print( FILE* cfile, int depth ) const
{
	if ( cdata )
	{
		int i;
		fprintf( cfile, "\n" );
		for ( i=0; i<depth; i++ ) {
			fprintf( cfile, "    " );
		}
		fprintf( cfile, "<![CDATA[%s]]>\n", value.c_str() );	// unformatted output
	}
	else
	{
		std::string buffer;
		EncodeString( value, &buffer );
		fprintf( cfile, "%s", buffer.c_str() );
	}
}


void C_DARK_XML_TEXT::CopyTo( C_DARK_XML_TEXT* target ) const
{
	C_DARK_XML_NODE::CopyTo( target );
	target->cdata = cdata;
}


BOOL C_DARK_XML_TEXT::Accept( C_DARK_XML_VISITOR* visitor ) const
{
	return visitor->Visit( *this );
}


C_DARK_XML_NODE* C_DARK_XML_TEXT::Clone() const
{	
	C_DARK_XML_TEXT* clone = 0;
	clone = new C_DARK_XML_TEXT( "" );

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


C_DARK_XML_DECLARATION::C_DARK_XML_DECLARATION( const char * _version, const char * _encoding, const char * _standalone ) : C_DARK_XML_NODE( C_DARK_XML_NODE::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}

C_DARK_XML_DECLARATION::C_DARK_XML_DECLARATION(	const std::string& _version, const std::string& _encoding, const std::string& _standalone ) : C_DARK_XML_NODE( C_DARK_XML_NODE::DECLARATION )
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}

C_DARK_XML_DECLARATION::C_DARK_XML_DECLARATION( const C_DARK_XML_DECLARATION& copy )
	: C_DARK_XML_NODE( C_DARK_XML_NODE::DECLARATION )
{
	copy.CopyTo( this );	
}


void C_DARK_XML_DECLARATION::operator=( const C_DARK_XML_DECLARATION& copy )
{
	Clear();
	copy.CopyTo( this );
}


void C_DARK_XML_DECLARATION::Print( FILE* cfile, int /*depth*/, std::string* str ) const
{
	if ( cfile ) fprintf( cfile, "<?xml " );
	if ( str )	 (*str) += "<?xml ";

	if ( !version.empty() ) {
		if ( cfile ) fprintf (cfile, "version=\"%s\" ", version.c_str ());
		if ( str ) { (*str) += "version=\""; (*str) += version; (*str) += "\" "; }
	}
	if ( !encoding.empty() ) {
		if ( cfile ) fprintf (cfile, "encoding=\"%s\" ", encoding.c_str ());
		if ( str ) { (*str) += "encoding=\""; (*str) += encoding; (*str) += "\" "; }
	}
	if ( !standalone.empty() ) {
		if ( cfile ) fprintf (cfile, "standalone=\"%s\" ", standalone.c_str ());
		if ( str ) { (*str) += "standalone=\""; (*str) += standalone; (*str) += "\" "; }
	}
	if ( cfile ) fprintf( cfile, "?>" );
	if ( str )	 (*str) += "?>";
}


void C_DARK_XML_DECLARATION::CopyTo( C_DARK_XML_DECLARATION* target ) const
{
	C_DARK_XML_NODE::CopyTo( target );

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}


BOOL C_DARK_XML_DECLARATION::Accept( C_DARK_XML_VISITOR* visitor ) const
{
	return visitor->Visit( *this );
}


C_DARK_XML_NODE* C_DARK_XML_DECLARATION::Clone() const
{	
	C_DARK_XML_DECLARATION* clone = new C_DARK_XML_DECLARATION();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


void C_DARK_XML_UNKNOWN::Print( FILE* cfile, int depth ) const
{
	for ( int i=0; i<depth; i++ )
		fprintf( cfile, "    " );
	fprintf( cfile, "<%s>", value.c_str() );
}


void C_DARK_XML_UNKNOWN::CopyTo( C_DARK_XML_UNKNOWN* target ) const
{
	C_DARK_XML_NODE::CopyTo( target );
}


BOOL C_DARK_XML_UNKNOWN::Accept( C_DARK_XML_VISITOR* visitor ) const
{
	return visitor->Visit( *this );
}


C_DARK_XML_NODE* C_DARK_XML_UNKNOWN::Clone() const
{
	C_DARK_XML_UNKNOWN* clone = new C_DARK_XML_UNKNOWN();

	if ( !clone )
		return 0;

	CopyTo( clone );
	return clone;
}


DarkXmlAttributeSet::DarkXmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


DarkXmlAttributeSet::~DarkXmlAttributeSet()
{

}


void DarkXmlAttributeSet::Add( C_DARK_XML_ATTRIBUTE* addMe )
{
	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev      = addMe;
}

void DarkXmlAttributeSet::Remove( C_DARK_XML_ATTRIBUTE* removeMe )
{
	C_DARK_XML_ATTRIBUTE* node;

	for( node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node == removeMe )
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
}

const C_DARK_XML_ATTRIBUTE* DarkXmlAttributeSet::Find( const std::string& name ) const
{
	for( const C_DARK_XML_ATTRIBUTE* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}

const C_DARK_XML_ATTRIBUTE* DarkXmlAttributeSet::Find( const char* name ) const
{
	for( const C_DARK_XML_ATTRIBUTE* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( strcmp( node->name.c_str(), name ) == 0 )
			return node;
	}
	return 0;
}

/*
C_DARK_XML_ATTRIBUTE*	DarkXmlAttributeSet::Find( const char* name )
{
	for( C_DARK_XML_ATTRIBUTE* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( strcmp( node->name.c_str(), name ) == 0 )
			return node;
	}
	return 0;
}
*/

std::istream& operator>> (std::istream & in, C_DARK_XML_NODE & base)
{
	std::string tag;
	tag.reserve( 8 * 1000 );
	base.StreamIn( &in, &tag );

	base.Parse( tag.c_str(), 0, DARK_XML_DEFAULT_ENCODING );
	return in;
}

std::ostream& operator<< (std::ostream & out, const C_DARK_XML_NODE & base)
{
	DarkXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out << printer.Str();

	return out;
}

std::string& operator<< (std::string& out, const C_DARK_XML_NODE& base )
{
	DarkXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept( &printer );
	out.append( printer.Str() );

	return out;
}

DarkXmlHandle DarkXmlHandle::FirstChild() const
{
	if ( node )
	{
		C_DARK_XML_NODE* child = node->FirstChild();
		if ( child )
			return DarkXmlHandle( child );
	}
	return DarkXmlHandle( 0 );
}


DarkXmlHandle DarkXmlHandle::FirstChild( const char * value ) const
{
	if ( node )
	{
		C_DARK_XML_NODE* child = node->FirstChild( value );
		if ( child )
			return DarkXmlHandle( child );
	}
	return DarkXmlHandle( 0 );
}


DarkXmlHandle DarkXmlHandle::FirstChildElement() const
{
	if ( node )
	{
		C_DARK_XML_ELEMENT* child = node->FirstChildElement();
		if ( child )
			return DarkXmlHandle( child );
	}
	return DarkXmlHandle( 0 );
}


DarkXmlHandle DarkXmlHandle::FirstChildElement( const char * value ) const
{
	if ( node )
	{
		C_DARK_XML_ELEMENT* child = node->FirstChildElement( value );
		if ( child )
			return DarkXmlHandle( child );
	}
	return DarkXmlHandle( 0 );
}


DarkXmlHandle DarkXmlHandle::Child( int count ) const
{
	if ( node )
	{
		int i;
		C_DARK_XML_NODE* child = node->FirstChild();
		for (	i=0;
				child && i<count;
				child = child->NextSibling(), ++i )
		{
			// nothing
		}
		if ( child )
			return DarkXmlHandle( child );
	}
	return DarkXmlHandle( 0 );
}


DarkXmlHandle DarkXmlHandle::Child( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		C_DARK_XML_NODE* child = node->FirstChild( value );
		for (	i=0;
				child && i<count;
				child = child->NextSibling( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return DarkXmlHandle( child );
	}
	return DarkXmlHandle( 0 );
}


DarkXmlHandle DarkXmlHandle::ChildElement( int count ) const
{
	if ( node )
	{
		int i;
		C_DARK_XML_ELEMENT* child = node->FirstChildElement();
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement(), ++i )
		{
			// nothing
		}
		if ( child )
			return DarkXmlHandle( child );
	}
	return DarkXmlHandle( 0 );
}


DarkXmlHandle DarkXmlHandle::ChildElement( const char* value, int count ) const
{
	if ( node )
	{
		int i;
		C_DARK_XML_ELEMENT* child = node->FirstChildElement( value );
		for (	i=0;
				child && i<count;
				child = child->NextSiblingElement( value ), ++i )
		{
			// nothing
		}
		if ( child )
			return DarkXmlHandle( child );
	}
	return DarkXmlHandle( 0 );
}


BOOL DarkXmlPrinter::VisitEnter( const C_DARK_XML_DOCUMENT& )
{
	return true;
}

BOOL DarkXmlPrinter::VisitExit( const C_DARK_XML_DOCUMENT& )
{
	return true;
}

BOOL DarkXmlPrinter::VisitEnter( const C_DARK_XML_ELEMENT& element, const C_DARK_XML_ATTRIBUTE* firstAttribute )
{
	DoIndent();
	buffer += "<";
	buffer += element.Value();

	for( const C_DARK_XML_ATTRIBUTE* attrib = firstAttribute; attrib; attrib = attrib->Next() )
	{
		buffer += " ";
		attrib->Print( 0, 0, &buffer );
	}

	if ( !element.FirstChild() ) 
	{
		buffer += " />";
		DoLineBreak();
	}
	else 
	{
		buffer += ">";
		if (    element.FirstChild()->ToText()
			  && element.LastChild() == element.FirstChild()
			  && element.FirstChild()->ToText()->CDATA() == false )
		{
			simpleTextPrint = true;
			// no DoLineBreak()!
		}
		else
		{
			DoLineBreak();
		}
	}
	++depth;	
	return true;
}


BOOL DarkXmlPrinter::VisitExit( const C_DARK_XML_ELEMENT& element )
{
	--depth;
	if ( !element.FirstChild() ) 
	{
		// nothing.
	}
	else 
	{
		if ( simpleTextPrint )
		{
			simpleTextPrint = false;
		}
		else
		{
			DoIndent();
		}
		buffer += "</";
		buffer += element.Value();
		buffer += ">";
		DoLineBreak();
	}
	return true;
}


BOOL DarkXmlPrinter::Visit( const C_DARK_XML_TEXT& text )
{
	if ( text.CDATA() )
	{
		DoIndent();
		buffer += "<![CDATA[";
		buffer += text.Value();
		buffer += "]]>";
		DoLineBreak();
	}
	else if ( simpleTextPrint )
	{
		std::string str;
		C_DARK_XML_BASE::EncodeString( text.ValueTStr(), &str );
		buffer += str;
	}
	else
	{
		DoIndent();
		std::string str;
		C_DARK_XML_BASE::EncodeString( text.ValueTStr(), &str );
		buffer += str;
		DoLineBreak();
	}
	return true;
}


BOOL DarkXmlPrinter::Visit( const C_DARK_XML_DECLARATION& declaration )
{
	DoIndent();
	declaration.Print( 0, 0, &buffer );
	DoLineBreak();
	return true;
}


BOOL DarkXmlPrinter::Visit( const C_DARK_XML_COMMENT& comment )
{
	DoIndent();
	buffer += "<!--";
	buffer += comment.Value();
	buffer += "-->";
	DoLineBreak();
	return true;
}


BOOL DarkXmlPrinter::Visit( const C_DARK_XML_UNKNOWN& unknown )
{
	DoIndent();
	buffer += "<";
	buffer += unknown.Value();
	buffer += ">";
	DoLineBreak();
	return true;
}

