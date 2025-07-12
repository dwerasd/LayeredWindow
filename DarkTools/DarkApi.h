//------------------------------------------------------------------------
//
//	Purpose		: Hard C0ding Function's
//	Developer	: KOHSEUNGKEUN
//	e-mail		:dwerasd@gmail.com
//	Day created	: August 10th, 2015
//
//	Copyright (C) 2015 by KOHSEUNGKEUN. All Rights Reserved.
//------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#ifndef DARK_TOOLS_DARK_API_INCLUDED__
#define DARK_TOOLS_DARK_API_INCLUDED__

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include <windows.h>
#include <stdlib.h>
#include <tchar.h>

#if (_MSC_VER < 1310)	// Visual studio 6 юлго
#include <xstring>
//#include <string.h>
#else
#include <string>
#endif

#include <algorithm>	// for std::reverse
#include <sstream>

#define MAX_BUFFER (1 << 9)

#if defined(_UNICODE)
#define _tstring std::wstring
#define _tstringstream std::wstringstream
#define _tostringstream std::wostringstream
#define _tostream std::wostream
#define _tistream std::wistream
#else
#define _tstring std::string
#define _tstringstream std::stringstream
#define _tostringstream std::ostringstream
#define _tostream std::ostream
#define _tistream std::istream
#endif

//#include "DarkException.h"



namespace DarkTools
{
	namespace DarkApi
	{
		unsigned int randomize(int nMin, long int max);
		void SetByte(LPVOID dst, BYTE src, size_t n);
		void CopyByte(LPVOID dst, LPVOID src, SIZE_T nCnt );

		LPSTR DecryptStringA(LPSTR lpszString);
		int _EnctZ(LPSTR lpszString);
		LPSTR _DectZ(LPSTR lpszString);

		ULONG_PTR FindCode(ULONG_PTR ulStart, ULONG ulEnd, LPBYTE pbytPattern, ULONG ulPatternSize);

		void OutputDebugStringPtr(LPWSTR lpszFormat, ...);
		void OutputDebugStringPtr(LPSTR lpszFormat, ...);

		WCHAR UpChar(WCHAR Ch);
		CHAR UpChar(CHAR Ch);
		int tcsicmp(LPWSTR s1, LPWSTR s2);
		int tcsicmp(LPSTR s1, LPSTR s2);

		int StringFormatA(std::string &txt, LPSTR lpszFormat, ...);
		
		template<typename T, typename V, typename R>
		char* itoa(const T &in, V *res, R base);
		size_t strlcat(char *dst, const char *src, size_t siz);
		size_t strlcpy(char *dst, const char *src, size_t siz);
		int charlcat(char* x, char y, int z);
		bool charremove(char* mp, char remove);
	}
} // End of namespace DarkTools 

using namespace DarkTools;

#define ZeroByte(a,b)		DarkApi::SetByte(a,0, b)
#define WipeCode(a,b)		DarkApi::SetByte(a,0xcc, b)
#define	OutputString		DarkApi::OutputDebugStringPtr
#define DecryptString		DarkApi::DecryptStringA


#endif // DARK_TOOLS_DARK_API_INCLUDED__
