#include "stdafx.h"
#include "DarkXMLParser.h"



void C_TINY_XML_PARSING_DATA::Stamp( LPCSTR now, _TINY_XML_ENCODING_ encoding )
{
	if ( tabsize < 1 )
	{
		return;
	}

	int row = cursor.row;
	int col = cursor.col;
	LPCSTR p = stamp;

	while ( p < now )
	{
		const unsigned char* pU = (const unsigned char*)p;

		switch (*pU)
		{
			case 0:
				return;

			case '\r':
				++row;
				col = 0;				
				++p;

				if (*p == '\n')
				{
					++p;
				}
				break;

			case '\n':
				++row;
				col = 0;
				++p;

				if (*p == '\r')
				{
					++p;
				}
				break;

			case '\t':
				++p;
				col = (col / tabsize + 1) * tabsize;
				break;
			case TINY_XML_UTF_LEAD_0:
				if ( encoding == TINY_XML_ENCODING_UTF8 )
				{
					if ( *(p+1) && *(p+2) )
					{
						if ( *(pU+1)==TINY_XML_UTF_LEAD_1 && *(pU+2)==TINY_XML_UTF_LEAD_2 )
						{
							p += 3;
						}
						else if ( *(pU+1)==0xbfU && *(pU+2)==0xbeU )
						{
							p += 3;	
						}
						else if ( *(pU+1)==0xbfU && *(pU+2)==0xbfU )
						{
							p += 3;	
						}
						else
						{
							p +=3; ++col;
						}
					}
				}
				else
				{
					++p;
					++col;
				}
				break;

			default:
				if ( encoding == TINY_XML_ENCODING_UTF8 )
				{
					int step = C_TINY_XML_BASE::utf8ByteTable[*((const unsigned char*)p)];
					if ( step == 0 )
					{
						step = 1;
					}
					p += step;
					++col;
				}
				else
				{
					++p;
					++col;
				}
				break;
		}
	}
	cursor.row = row;
	cursor.col = col;
	stamp = p;
}


LPCSTR C_TINY_XML_BASE::SkipWhiteSpace( LPCSTR p, _TINY_XML_ENCODING_ encoding )
{
	if ( !p || !*p )
	{
		return 0;
	}
	if ( encoding == TINY_XML_ENCODING_UTF8 )
	{
		while ( *p )
		{
			const unsigned char* pU = (const unsigned char*)p;
			
			if ( *(pU+0)==TINY_XML_UTF_LEAD_0 && *(pU+1)==TINY_XML_UTF_LEAD_1 && *(pU+2)==TINY_XML_UTF_LEAD_2 )
			{
				p += 3;
				continue;
			}
			else if(*(pU+0)==TINY_XML_UTF_LEAD_0 && *(pU+1)==0xbfU && *(pU+2)==0xbeU )
			{
				p += 3;
				continue;
			}
			else if(*(pU+0)==TINY_XML_UTF_LEAD_0 && *(pU+1)==0xbfU && *(pU+2)==0xbfU )
			{
				p += 3;
				continue;
			}
			if ( IsWhiteSpace( *p ) || *p == '\n' || *p =='\r' )
			{
				++p;
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		while ( *p && IsWhiteSpace( *p ) || *p == '\n' || *p =='\r' )
		{
			++p;
		}
	}
	return p;
}


LPCSTR C_TINY_XML_BASE::ReadName( LPCSTR p, C_DARK_STRING * name, _TINY_XML_ENCODING_ encoding )
{
	*name = "";
	if (0 != p && *p && ( IsAlpha( (unsigned char) *p, encoding ) || *p == '_' ) )
	{
		LPCSTR start = p;
		while(0 != p && *p && ( IsAlphaNum( (unsigned char ) *p, encoding ) || *p == '_' || *p == '-' || *p == '.' || *p == ':' ) )
		{
			++p;
		}
		if ( p-start > 0 )
		{
			name->assign( start, p-start );
		}
		return p;
	}
	return 0;
}

LPCSTR C_TINY_XML_BASE::GetEntity( LPCSTR p, char* value, int* length, _TINY_XML_ENCODING_ encoding )
{
    C_DARK_STRING ent;
	*length = 0;

	if ( *(p+1) && *(p+1) == '#' && *(p+2) )
	{
		unsigned long ucs = 0;
		ptrdiff_t delta = 0;
		unsigned mult = 1;

		if ( *(p+2) == 'x' )
		{
			// Hexadecimal.
			if ( !*(p+3) )
			{
				return 0;
			}

			LPCSTR q = p+3;
			q = strchr( q, ';' );

			if ( !q || !*q )
			{
				return 0;
			}

			delta = q-p;
			--q;

			while ( *q != 'x' )
			{
				if ( *q >= '0' && *q <= '9' )
				{
					ucs += mult * (*q - '0');
				}
				else if ( *q >= 'a' && *q <= 'f' )
				{
					ucs += mult * (*q - 'a' + 10);
				}
				else if ( *q >= 'A' && *q <= 'F' )
				{
					ucs += mult * (*q - 'A' + 10 );
				}
				else
				{
					return 0;
				}
				mult *= 16;
				--q;
			}
		}
		else
		{
			// Decimal.
			if ( !*(p+2) )
			{
				return 0;
			}

			LPCSTR q = p+2;
			q = strchr( q, ';' );

			if ( !q || !*q )
			{
				return 0;
			}

			delta = q-p;
			--q;

			while ( *q != '#' )
			{
				if ( *q >= '0' && *q <= '9' )
				{
					ucs += mult * (*q - '0');
				}
				else 
				{
					return 0;
				}
				mult *= 10;
				--q;
			}
		}
		if ( encoding == TINY_XML_ENCODING_UTF8 )
		{
			ConvertUTF32ToUTF8( ucs, value, length );	// convert the UCS to UTF-8
		}
		else
		{
			*value = (char)ucs;
			*length = 1;
		}
		return p + delta + 1;
	}

	for(int i=0; i<NUM_ENTITY; ++i )
	{
		if ( strncmp( entity[i].str, p, entity[i].strLength ) == 0 )
		{
			*value = entity[i].chr;
			*length = 1;
			return ( p + entity[i].strLength );
		}
	}
	*value = *p;
	return p+1;
}


BOOL C_TINY_XML_BASE::StringEqual( LPCSTR p, LPCSTR tag, BOOL ignoreCase, _TINY_XML_ENCODING_ encoding )
{
	if ( !p || !*p )
	{
		return(FALSE);
	}

	LPCSTR q = p;

	if ( ignoreCase )
	{
		while ( *q && *tag && ToLower( *q, encoding ) == ToLower( *tag, encoding ) )
		{
			++q;
			++tag;
		}

		if ( *tag == 0 )
		{
			return(TRUE);
		}
	}
	else
	{
		while ( *q && *tag && *q == *tag )
		{
			++q;
			++tag;
		}

		if ( *tag == 0 )
		{
			return(TRUE);
		}
	}
	return(FALSE);
}

LPCSTR C_TINY_XML_BASE::ReadText(LPCSTR p, C_DARK_STRING * text, BOOL trimWhiteSpace, LPCSTR endTag, BOOL caseInsensitive, _TINY_XML_ENCODING_ encoding )
{
    *text = "";
	if ( !trimWhiteSpace || !condenseWhiteSpace )
	{
		while (0 != p && *p && !StringEqual( p, endTag, caseInsensitive, encoding ))
		{
			int len;
			char cArr[4] = { 0 };
			p = GetChar( p, cArr, &len, encoding );
			text->append( cArr, len );
		}
	}
	else
	{
		BOOL whitespace = FALSE;
		p = SkipWhiteSpace( p, encoding );
		while (0 != p && *p && !StringEqual( p, endTag, caseInsensitive, encoding ) )
		{
			if ( *p == '\r' || *p == '\n' )
			{
				whitespace = TRUE;
				++p;
			}
			else if ( IsWhiteSpace( *p ) )
			{
				whitespace = TRUE;
				++p;
			}
			else
			{
				if ( whitespace )
				{
					(*text) += ' ';
					whitespace = FALSE;
				}
				int len;
				char cArr[4] = { 0 };
				p = GetChar( p, cArr, &len, encoding );
				if ( len == 1 )
				{
					(*text) += cArr[0];	// more efficient
				}
				else
				{
					text->append( cArr, len );
				}
			}
		}
	}
	if ( p ) 
		p += lstrlenA( endTag );
	return p;
}

LPCSTR C_TINY_XML_DOCUMENT::Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* prevData, _TINY_XML_ENCODING_ encoding )
{
	ClearError();
	if ( !p || !*p )
	{
		SetError( TINY_XML_ERROR_DOCUMENT_EMPTY, 0, 0, TINY_XML_ENCODING_UNKNOWN );
		return 0;
	}

	location.Clear();
	if (0 != prevData )
	{
		location.row = prevData->cursor.row;
		location.col = prevData->cursor.col;
	}
	else
	{
		location.row = 0;
		location.col = 0;
	}
	C_TINY_XML_PARSING_DATA data( p, TabSize(), location.row, location.col );
	location = data.Cursor();

	if (TINY_XML_ENCODING_UNKNOWN == encoding)
	{
		// Check for the Microsoft UTF-8 lead bytes.
		const unsigned char* pU = (const unsigned char*)p;
		if (	*(pU+0) && *(pU+0) == TINY_XML_UTF_LEAD_0
			 && *(pU+1) && *(pU+1) == TINY_XML_UTF_LEAD_1
			 && *(pU+2) && *(pU+2) == TINY_XML_UTF_LEAD_2 )
		{
			encoding = TINY_XML_ENCODING_UTF8;
			useMicrosoftBOM = TRUE;
		}
	}

    p = SkipWhiteSpace( p, encoding );
	if ( !p )
	{
		SetError( TINY_XML_ERROR_DOCUMENT_EMPTY, 0, 0, TINY_XML_ENCODING_UNKNOWN );
		return 0;
	}

	while (0 != p && *p )
	{
		C_TINY_XML_NODE* node = Identify( p, encoding );
		if ( node )
		{
			p = node->Parse( p, &data, encoding );
			LinkEndChild( node );
		}
		else
		{
			break;
		}

		// Did we get encoding info?
		if ( TINY_XML_ENCODING_UNKNOWN == encoding && node->ToDeclaration() )
		{
			C_TINY_XML_DECLARATION* dec = node->ToDeclaration();
			LPCSTR enc = dec->Encoding();

			if ( 0 == *enc )
			{
				encoding = TINY_XML_ENCODING_UTF8;
			}
			else if ( StringEqual( enc, "UTF-8", TRUE, TINY_XML_ENCODING_UNKNOWN ) )
			{
				encoding = TINY_XML_ENCODING_UTF8;
			}
			else if ( StringEqual( enc, "UTF8", TRUE, TINY_XML_ENCODING_UNKNOWN ) )
			{
				encoding = TINY_XML_ENCODING_UTF8;	// incorrect, but be nice
			}
			else
			{
				encoding = TINY_XML_ENCODING_LEGACY;
			}
		}
		p = SkipWhiteSpace( p, encoding );
	}
	if ( !firstChild )
	{
		SetError( TINY_XML_ERROR_DOCUMENT_EMPTY, 0, 0, encoding );
		return 0;
	}
	return p;
}

void C_TINY_XML_DOCUMENT::SetError( int err, LPCSTR pError, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding )
{	
	if (FALSE == error )
	{
		error = TRUE;
		errorId = err;
		errorDesc = errorString[ errorId ];

		errorLocation.Clear();
		if ( pError && data )
		{
			data->Stamp( pError, encoding );
			errorLocation = data->Cursor();
		}
	}
}


C_TINY_XML_NODE* C_TINY_XML_NODE::Identify( LPCSTR p, _TINY_XML_ENCODING_ encoding )
{
	C_TINY_XML_NODE* returnNode = 0;

	p = SkipWhiteSpace( p, encoding );
	if( !p || !*p || *p != '<' )
	{
		return 0;
	}

	C_TINY_XML_DOCUMENT* doc = GetDocument();
	p = SkipWhiteSpace( p, encoding );

	if ( !p || !*p )
	{
		return 0;
	}

	LPCSTR xmlHeader = { "<?xml" };
	LPCSTR commentHeader = { "<!--" };
	LPCSTR dtdHeader = { "<!" };
	LPCSTR cdataHeader = { "<![CDATA[" };

	if ( StringEqual( p, xmlHeader, TRUE, encoding ) )
	{
		returnNode = new C_TINY_XML_DECLARATION();
	}
	else if ( StringEqual( p, commentHeader, FALSE, encoding ) )
	{
		returnNode = new C_TINY_XML_COMMENT();
	}
	else if ( StringEqual( p, cdataHeader, FALSE, encoding ) )
	{
		C_TINY_XML_TEXT* text = new C_TINY_XML_TEXT( "" );
		text->SetCDATA( TRUE );
		returnNode = text;
	}
	else if ( StringEqual( p, dtdHeader, FALSE, encoding ) )
	{
		returnNode = new C_TINY_XML_UNKNOWN();
	}
	else if ( IsAlpha( *(p+1), encoding ) || *(p+1) == '_' )
	{
		returnNode = new C_TINY_XML_ELEMENT( "" );
	}
	else
	{
		returnNode = new C_TINY_XML_UNKNOWN();
	}

	if ( returnNode )
	{
		returnNode->parent = this;
	}
	else
	{
		if ( doc )
		{
			doc->SetError( TINY_XML_ERROR_OUT_OF_MEMORY, 0, 0, TINY_XML_ENCODING_UNKNOWN );
		}
	}
	return returnNode;
}

LPCSTR C_TINY_XML_ELEMENT::Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding )
{
	p = SkipWhiteSpace( p, encoding );
	C_TINY_XML_DOCUMENT* document = GetDocument();

	if ( !p || !*p )
	{
		if ( document ) document->SetError( TINY_XML_ERROR_PARSING_ELEMENT, 0, 0, encoding );
		return 0;
	}

	if ( data )
	{
		data->Stamp( p, encoding );
		location = data->Cursor();
	}

	if ( *p != '<' )
	{
		if ( document ) document->SetError( TINY_XML_ERROR_PARSING_ELEMENT, p, data, encoding );
		return 0;
	}

	p = SkipWhiteSpace( p+1, encoding );

	LPCSTR pErr = p;
    p = ReadName( p, &value, encoding );
	if ( !p || !*p )
	{
		if ( document )	document->SetError( TINY_XML_ERROR_FAILED_TO_READ_ELEMENT_NAME, pErr, data, encoding );
		return 0;
	}

    C_DARK_STRING endTag ("</");
	endTag += value;
	endTag += ">";

	while ( p && *p )
	{
		pErr = p;
		p = SkipWhiteSpace( p, encoding );
		if ( !p || !*p )
		{
			if (0 != document )
			{
				document->SetError( TINY_XML_ERROR_READING_ATTRIBUTES, pErr, data, encoding );
			}
			return 0;
		}
		if ( *p == '/' )
		{
			++p;
			if ( *p  != '>' )
			{
				if (0 != document )
				{
					document->SetError( TINY_XML_ERROR_PARSING_EMPTY, p, data, encoding );
				}
				return 0;
			}
			return (p+1);
		}
		else if ( *p == '>' )
		{
			++p;
			p = ReadValue( p, data, encoding );
			if ( !p || !*p )
			{
				if (0 != document )
				{
					document->SetError( TINY_XML_ERROR_READING_END_TAG, p, data, encoding );
				}
				return 0;
			}

			// We should find the end tag now
			if ( StringEqual( p, endTag.c_str(), FALSE, encoding ) )
			{
				p += endTag.length();
				return p;
			}
			else
			{
				if (0 != document )
				{
					document->SetError( TINY_XML_ERROR_READING_END_TAG, p, data, encoding );
				}
				return 0;
			}
		}
		else
		{
			// Try to read an attribute:
			C_TINY_XML_ATTRIBUTE* attrib = new C_TINY_XML_ATTRIBUTE();
			if ( !attrib )
			{
				if (0 != document )
				{
					document->SetError( TINY_XML_ERROR_OUT_OF_MEMORY, pErr, data, encoding );
				}
				return 0;
			}

			attrib->SetDocument( document );
			pErr = p;
			p = attrib->Parse( p, data, encoding );

			if ( !p || !*p )
			{
				if (0 != document )
				{
					document->SetError( TINY_XML_ERROR_PARSING_ELEMENT, pErr, data, encoding );
				}
				delete attrib;
				return 0;
			}

			// Handle the strange case of double attributes:
			C_TINY_XML_ATTRIBUTE* node = attributeSet.Find( attrib->Name() );
			if ( node )
			{
				node->SetValue( attrib->Value() );
				delete attrib;
				return 0;
			}

			attributeSet.Add( attrib );
		}
	}
	return p;
}


LPCSTR C_TINY_XML_ELEMENT::ReadValue( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding )
{
	C_TINY_XML_DOCUMENT* document = GetDocument();

	// Read in text and elements in any order.
	LPCSTR pWithWhiteSpace = p;
	p = SkipWhiteSpace( p, encoding );

	while ( p && *p )
	{
		if ( *p != '<' )
		{
			// Take what we have, make a text element.
			C_TINY_XML_TEXT* textNode = new C_TINY_XML_TEXT( "" );

			if ( !textNode )
			{
				if (0 != document )
				{
					document->SetError( TINY_XML_ERROR_OUT_OF_MEMORY, 0, 0, encoding );
				}
				return 0;
			}

			if ( C_TINY_XML_BASE::IsWhiteSpaceCondensed() )
			{
				p = textNode->Parse( p, data, encoding );
			}
			else
			{
				p = textNode->Parse( pWithWhiteSpace, data, encoding );
			}

			if ( !textNode->Blank() )
			{
				LinkEndChild( textNode );
			}
			else
			{
				delete textNode;
			}
		} 
		else 
		{
			if ( StringEqual( p, "</", FALSE, encoding ) )
			{
				return p;
			}
			else
			{
				C_TINY_XML_NODE* node = Identify( p, encoding );
				if (0 != node )
				{
					p = node->Parse( p, data, encoding );
					LinkEndChild( node );
				}				
				else
				{
					return 0;
				}
			}
		}
		pWithWhiteSpace = p;
		p = SkipWhiteSpace( p, encoding );
	}

	if ( !p )
	{
		if (0 != document )
		{
			document->SetError( TINY_XML_ERROR_READING_ELEMENT_VALUE, 0, 0, encoding );
		}
	}	
	return p;
}

LPCSTR C_TINY_XML_UNKNOWN::Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding )
{
	C_TINY_XML_DOCUMENT* document = GetDocument();
	p = SkipWhiteSpace( p, encoding );

	if ( data )
	{
		data->Stamp( p, encoding );
		location = data->Cursor();
	}
	if ( !p || !*p || *p != '<' )
	{
		if (0 != document )
		{
			document->SetError( TINY_XML_ERROR_PARSING_UNKNOWN, p, data, encoding );
		}
		return 0;
	}
	++p;
    value = "";

	while ( p && *p && *p != '>' )
	{
		value += *p;
		++p;
	}

	if ( !p )
	{
		if (0 != document )
		{
			document->SetError( TINY_XML_ERROR_PARSING_UNKNOWN, 0, 0, encoding );
		}
	}
	if ( *p == '>' )
		return p+1;
	return p;
}

LPCSTR C_TINY_XML_COMMENT::Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding )
{
	C_TINY_XML_DOCUMENT* document = GetDocument();
	value = "";

	p = SkipWhiteSpace( p, encoding );

	if ( data )
	{
		data->Stamp( p, encoding );
		location = data->Cursor();
	}
	LPCSTR startTag = "<!--";
	LPCSTR endTag   = "-->";

	if ( !StringEqual( p, startTag, FALSE, encoding ) )
	{
		document->SetError( TINY_XML_ERROR_PARSING_COMMENT, p, data, encoding );
		return 0;
	}
	p += lstrlenA( startTag );
    value = "";

	while (	p && *p && !StringEqual( p, endTag, FALSE, encoding ) )
	{
		value.append( p, 1 );
		++p;
	}
	if ( p )
	{
		p += lstrlenA( endTag );
	}

	return p;
}


LPCSTR C_TINY_XML_ATTRIBUTE::Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding )
{
	p = SkipWhiteSpace( p, encoding );
	if ( !p || !*p ) return 0;

	if (0 != data )
	{
		data->Stamp( p, encoding );
		location = data->Cursor();
	}
	// Read the name, the '=' and the value.
	LPCSTR pErr = p;
	p = ReadName( p, &name, encoding );
	if ( !p || !*p )
	{
		if (0 != document )
		{
			document->SetError( TINY_XML_ERROR_READING_ATTRIBUTES, pErr, data, encoding );
		}
		return 0;
	}
	p = SkipWhiteSpace( p, encoding );
	if ( !p || !*p || *p != '=' )
	{
		if (0 != document )
		{
			document->SetError( TINY_XML_ERROR_READING_ATTRIBUTES, p, data, encoding );
		}
		return 0;
	}

	++p;	// skip '='
	p = SkipWhiteSpace( p, encoding );
	if ( !p || !*p )
	{
		if (0 != document )
		{
			document->SetError( TINY_XML_ERROR_READING_ATTRIBUTES, p, data, encoding );
		}
		return 0;
	}
	
	LPCSTR end;
	const char SINGLE_QUOTE = '\'';
	const char DOUBLE_QUOTE = '\"';

	if ( *p == SINGLE_QUOTE )
	{
		++p;
		end = "\'";		// single quote in string
		p = ReadText( p, &value, FALSE, end, FALSE, encoding );
	}
	else if ( *p == DOUBLE_QUOTE )
	{
		++p;
		end = "\"";		// double quote in string
		p = ReadText( p, &value, FALSE, end, FALSE, encoding );
	}
	else
	{
		value = "";
		while (0 != p && *p	&& !IsWhiteSpace( *p ) && *p != '\n' && *p != '\r' && *p != '/' && *p != '>' )
		{
			if ( *p == SINGLE_QUOTE || *p == DOUBLE_QUOTE )
			{
				if (0 != document )
				{
					document->SetError( TINY_XML_ERROR_READING_ATTRIBUTES, p, data, encoding );
				}
				return 0;
			}
			value += *p;
			++p;
		}
	}
	return p;
}

LPCSTR C_TINY_XML_TEXT::Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ encoding )
{
	value = "";
	C_TINY_XML_DOCUMENT* document = GetDocument();

	if (0 != data)
	{
		data->Stamp( p, encoding );
		location = data->Cursor();
	}

	LPCSTR const startTag = "<![CDATA[";
	LPCSTR const endTag   = "]]>";

	if ( cdata || StringEqual( p, startTag, FALSE, encoding ) )
	{
		cdata = TRUE;

		if ( !StringEqual( p, startTag, FALSE, encoding ) )
		{
			document->SetError( TINY_XML_ERROR_PARSING_CDATA, p, data, encoding );
			return 0;
		}
		p += lstrlenA( startTag );

		// Keep all the white space, ignore the encoding, etc.
		while (0 != p && *p && !StringEqual( p, endTag, FALSE, encoding ))
		{
			value += *p;
			++p;
		}

		C_DARK_STRING dummy;
		p = ReadText( p, &dummy, FALSE, endTag, FALSE, encoding );
		return p;
	}
	else
	{
		BOOL ignoreWhite = TRUE;

		LPCSTR end = "<";
		p = ReadText( p, &value, ignoreWhite, end, FALSE, encoding );
		if ( p )
		{
			return p-1;	// don't truncate the '<'
		}
		return 0;
	}
}

LPCSTR C_TINY_XML_DECLARATION::Parse( LPCSTR p, C_TINY_XML_PARSING_DATA* data, _TINY_XML_ENCODING_ _encoding )
{
	p = SkipWhiteSpace( p, _encoding );
	// Find the beginning, find the end, and look for
	// the stuff in-between.
	C_TINY_XML_DOCUMENT* document = GetDocument();
	if ( !p || !*p || !StringEqual( p, "<?xml", TRUE, _encoding ) )
	{
		if ( document ) document->SetError( TINY_XML_ERROR_PARSING_DECLARATION, 0, 0, _encoding );
		return 0;
	}
	if (0 != data )
	{
		data->Stamp( p, _encoding );
		location = data->Cursor();
	}
	p += 5;

	version = "";
	encoding = "";
	standalone = "";

	while ( p && *p )
	{
		if ( *p == '>' )
		{
			++p;
			return p;
		}

		p = SkipWhiteSpace( p, _encoding );
		if ( StringEqual( p, "version", TRUE, _encoding ) )
		{
			C_TINY_XML_ATTRIBUTE attrib;
			p = attrib.Parse( p, data, _encoding );		
			version = attrib.Value();
		}
		else if ( StringEqual( p, "encoding", TRUE, _encoding ) )
		{
			C_TINY_XML_ATTRIBUTE attrib;
			p = attrib.Parse( p, data, _encoding );		
			encoding = attrib.Value();
		}
		else if ( StringEqual( p, "standalone", TRUE, _encoding ) )
		{
			C_TINY_XML_ATTRIBUTE attrib;
			p = attrib.Parse( p, data, _encoding );		
			standalone = attrib.Value();
		}
		else
		{
			// Read over whatever it is.
			while( p && *p && *p != '>' && !IsWhiteSpace( *p ) )
			{
				++p;
			}
		}
	}
	return 0;
}

BOOL C_TINY_XML_TEXT::Blank() const
{
	for ( unsigned i=0; i<value.length(); i++ )
	{
		if ( !IsWhiteSpace( value[i] ) )
		{
			return(FALSE);
		}
	}
	return(TRUE);
}

