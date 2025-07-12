#pragma once


#include "DarkXML.h"



class C_TINY_XML_PARSING_DATA
{
	friend class C_TINY_XML_DOCUMENT;

private:
	C_TINY_XML_PARSING_DATA( LPCSTR start, int _tabsize, int row, int col )
	{
		stamp = start;
		tabsize = _tabsize;
		cursor.row = row;
		cursor.col = col;
	}

	_TINY_XML_CURSOR	cursor;
	LPCSTR				stamp;
	int					tabsize;

public:
	void Stamp( LPCSTR now, _TINY_XML_ENCODING_ encoding );
	const _TINY_XML_CURSOR& Cursor()	{ return cursor; }

};