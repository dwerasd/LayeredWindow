#ifndef __DBGPRINT__
#define __DBGPRINT__


#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <windows.h>
#include <stdio.h> 
#include <tchar.h>

//#include "DarkVS6.h"


#define DBGPRINT_BUFFER_SIZE		(1 << 12)

namespace DarkTools
{
	class C_DEBUG_VIEWER
	{
	public:
		C_DEBUG_VIEWER() { }
		~C_DEBUG_VIEWER() { }

		static void StringFormatPrint(LPCWSTR lpszFormat, ...)	// DBGPRINT, Format Trace
		{
			WCHAR wszBuffer[DBGPRINT_BUFFER_SIZE];
			va_list fmtList = NULL;
			va_start(fmtList, lpszFormat);
			_vsnwprintf_s(wszBuffer, DBGPRINT_BUFFER_SIZE - 1, DBGPRINT_BUFFER_SIZE - 1, lpszFormat, fmtList);
			va_end(fmtList);
			wszBuffer[sizeof(wszBuffer) - 1] = 0;
			OutputDebugStringW(wszBuffer);
		}
		static void StringFormatPrint(LPCSTR lpszFormat, ...)	// DBGPRINT, Format Trace
		{
			char szBuffer[DBGPRINT_BUFFER_SIZE];
			va_list fmtList = NULL;
			va_start(fmtList, lpszFormat);
			_vsnprintf_s(szBuffer, DBGPRINT_BUFFER_SIZE - 1, DBGPRINT_BUFFER_SIZE - 1, lpszFormat, fmtList);
			va_end(fmtList);
			szBuffer[sizeof(szBuffer) - 1] = 0;
			OutputDebugStringA(szBuffer);
		}
	};
}

#define DBGPRINT	DarkTools::C_DEBUG_VIEWER::StringFormatPrint

#endif