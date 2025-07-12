#include "stdafx.h"
#include "DarkVS6.h"
#include "DarkApi.h"


namespace DarkTools
{
	namespace DarkApi
	{
		/// random part
#define  mmm (unsigned long)2207030490
#define  qqq (unsigned long)117178

#define  aaa (unsigned int)13245
#define  rrr (unsigned int)1728


		static unsigned long seed = 2391;
		void srandom(unsigned long initial_seed)
		{
			seed = initial_seed;
		}

		unsigned long random()
		{
			register int lo, hi, test;

			hi = seed / qqq;
			lo = seed % qqq;

			test = aaa * lo - rrr * hi;

			if (test > 0)
				seed = test;
			else
				seed = test + mmm;

			return seed;
		}
		unsigned int randomize(int nMin, long int max)
		{
			srandom(GetTickCount());
			double rVal = .0, Result = .0;
			for (unsigned int i = 1; i <= 0x20; i++)
			{
				rVal = ((double)random() / ((double)(0x7fff) + (double)(1)));
				Result = (rVal * max);
			}
			return (unsigned int)Result + nMin;
		}
		void SetByte(LPVOID dst, BYTE src, size_t n)
		{
#if defined(_WIN64)
			for ( UINT i = 0 ; i < n ; i++ )
			{
				*(LPBYTE)((LPBYTE)dst+i) = src;
			}
#else
			__asm
			{
				mov edi, dword ptr [dst]
				mov ecx, dword ptr [n]
				mov al, byte ptr [src]
				rep stosb
			}
#endif
		}
		void CopyByte(LPVOID dst, LPVOID src, SIZE_T nCnt )
		{
#if defined(_WIN64)
			for ( SIZE_T i = 0 ; i < nCnt ; i++ )
			{
				*(LPBYTE)((LPBYTE)dst+i) = *(LPBYTE)((LPBYTE)src+i);
			}
#else
			__asm
			{
				mov edi, dword ptr [dst]
				mov esi, dword ptr [src]
				mov ecx, dword ptr [nCnt]
				cld
					rep movsb
			}
#endif
		}

		LPSTR DecryptStringA(LPSTR lpszString)
		{
			if (0x01 == lpszString[0])
			{
				int nSeed = lpszString[1];
				nSeed *= 3;
				lpszString[2] ^= (nSeed++ + 'e');
				nSeed *= 3;
				lpszString[3] ^= (nSeed++ + 'e');
				int nLengthBuffer = ((BYTE)lpszString[2] << 8) | (BYTE)lpszString[3];

				for (int i = 0; i < nLengthBuffer; ++i)
				{
					nSeed *= 3;
					lpszString[i] = lpszString[i + 4] ^ (nSeed++ + 'e');
				}
				lpszString[nLengthBuffer] = 0;
			}
			return lpszString;
		}

		int _EnctZ(LPSTR lpszString)
		{
			int len = _tcslen(lpszString);
			__try
			{
				char buf[1024] = {0};

				int l1 = 0, l2 = 0;

				if ( len )
				{
					int nSeed = randomize(1, 255);
					buf[0] = 1;
					buf[1] = nSeed;

					nSeed *= 3;
					l1 = nSeed++ + 'z';
					nSeed *= 3;
					l2 = nSeed++ + 'z';

					int j = 4;
					for (int i = 0; i < len; ++i)
					{
						char c = lpszString[i];

						nSeed *= 3;
						buf[j++] = c ^ (nSeed++ + 'z');
					}
					buf[j] = 0;
					len = j - 4;
					buf[2] = len >> 8;
					buf[3] = len;
					buf[2] ^= l1;
					buf[3] ^= l2;
				}
				// 암호화해서 원래의 문자열에 덮어씌운다.
				memcpy_s(lpszString, 1024, buf, len+4);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{

			}
			return (len+4);
		}

		LPSTR _DectZ(LPSTR lpszString)
		{
			__try
			{
				if (lpszString[0] == 1)
				{
					int nSeed = lpszString[1];
					nSeed *= 3;
					lpszString[2] ^= (nSeed++ + 'z');
					nSeed *= 3;
					lpszString[3] ^= (nSeed++ + 'z');
					int len = ((BYTE)lpszString[2] << 8) | (BYTE)lpszString[3];
					for (int i = 0; i < len; ++i)
					{
						nSeed *= 3;
						lpszString[i] = lpszString[i+4] ^ (nSeed++ + 'z');
					}
					lpszString[len] = 0;
				}
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{

			}
			return lpszString;
		}
		ULONG_PTR FindCode(ULONG_PTR ulStart, ULONG ulMemorySize, LPBYTE pbytPattern, ULONG ulPatternSize)
		{
			ULONG i = 0, j = 0;
			do 
			{
				LPBYTE bytData = (LPBYTE)((LPBYTE)ulStart+i);			// 검색할 주소는 한번 비교에 1씩 증가한다.
				for (j = 0; j < ulPatternSize; j++)				// 한번 비교는 j 패턴사이즈만큼 체크하는 거다.
				{
					if (pbytPattern[j] == '?')					// 0x91 이라면 다음 다음 패턴 비교
					{
						continue;
					}
					if (pbytPattern[j] != bytData[j])			// 동일하지 않으면 비교 중지.
					{
						break;
					}
				}
				if (j == ulPatternSize)
				{
					return(ulStart+i);							// j가 nPatternSize와 같다는건 모두 비교에 성공. 찾았다는거다.
				}					
			} while (++i < ulMemorySize);
			return(0);
		}
		void OutputDebugStringPtr(LPWSTR lpszFormat, ...)	// TRACEF, Format Trace
		{
			va_list fmtList = NULL;
			va_start(fmtList,lpszFormat);

			WCHAR wszBuffer[1 << 10];
			ZeroByte(wszBuffer, sizeof(wszBuffer));

			while ( *lpszFormat )
			{
				if ( wcsncmp(lpszFormat, L"%d", 2) == 0 )
				{
					int i = va_arg(fmtList, int);
					WCHAR wszTmp[(1 << 5)];
					wsprintfW(wszTmp, L"%d", i);
					wcscat_s(wszBuffer, sizeof(wszBuffer), wszTmp);
					lpszFormat = lpszFormat + 2;
				}
				else if ( wcsncmp(lpszFormat, L"%f", 2) == 0 )
				{
					double f = va_arg(fmtList, double);
					WCHAR wszTmp[(1 << 5)];
					wsprintfW(wszTmp, L"%lf", f);
					wcscat_s(wszBuffer, sizeof(wszBuffer), wszTmp);
					lpszFormat = lpszFormat + 2;
				}
				else if ( wcsncmp(lpszFormat, L"%s", 2) == 0 )
				{
					LPWSTR str = va_arg(fmtList, LPWSTR);
					WCHAR wszTmp[(1 << 5)];
					wsprintfW(wszTmp, L"%s", str);
					wcscat_s(wszBuffer, sizeof(wszBuffer), wszTmp);
					lpszFormat = lpszFormat + 2;
				}
				else if ( wcsncmp(lpszFormat, L"%x", 2) == 0 )
				{
					UINT i = va_arg(fmtList, unsigned int);
					WCHAR wszTmp[(1 << 5)];
					wsprintfW(wszTmp, L"%x", i);
					wcscat_s(wszBuffer, sizeof(wszBuffer), wszTmp);
					lpszFormat = lpszFormat + 2;
				}
				else
				{
					WCHAR wszTmp[(1 << 5)];
					wsprintfW(wszTmp, L"%c", *lpszFormat);
					wcscat_s(wszBuffer, sizeof(wszBuffer), wszTmp);
					lpszFormat = lpszFormat + 1 ;
				}
			}
			va_end(fmtList);
			OutputDebugStringW(wszBuffer);
		}

		void OutputDebugStringPtr(LPSTR lpszFormat, ...)	// TRACEF, Format Trace
		{
			char szBuffer[1<<10] = {0};

			LPSTR data; /* 가변 인자 처리 리스트 */
			int i;
			double f;
			char *str;

			va_start(data,lpszFormat);

			char szTmp[1<<5] = {0};
			while ( *lpszFormat )
			{
				if ( strncmp(lpszFormat, "%d", 2) == 0 )
				{
					i = va_arg(data, int);
					wsprintfA(szTmp, "%d", i);
					strcat_s(szBuffer, sizeof(szBuffer), szTmp);
					lpszFormat = lpszFormat + 2;
				}
				else if ( strncmp(lpszFormat, "%f", 2) == 0 )
				{
					f = va_arg(data, double);
					wsprintfA(szTmp, "%lf", f);
					strcat_s(szBuffer, sizeof(szBuffer), szTmp);
					lpszFormat = lpszFormat + 2;
				}
				else if ( strncmp(lpszFormat, "%s", 2) == 0 )
				{
					str = va_arg(data, char *);
					wsprintfA(szTmp, "%s", str);
					strcat_s(szBuffer, sizeof(szBuffer), szTmp);
					lpszFormat = lpszFormat + 2;
				}
				else if ( strncmp(lpszFormat, "%x", 2) == 0 )
				{
					i = va_arg(data, unsigned int);
					wsprintfA(szTmp, "%x", i);
					strcat_s(szBuffer, sizeof(szBuffer), szTmp);
					lpszFormat = lpszFormat + 2;
				}
				else
				{
					wsprintfA(szTmp, "%c", *lpszFormat);
					strcat_s(szBuffer, sizeof(szBuffer), szTmp);
					lpszFormat = lpszFormat + 1 ;
				}
			}
			va_end(data);
			OutputDebugStringA(szBuffer);
		}

		WCHAR UpChar(WCHAR Ch)
		{
			if ((Ch >= 'a') && (Ch <= 'z'))
			{
				return(Ch - 32);
			}
			return (Ch);
		}

		CHAR UpChar(CHAR Ch)
		{
			if ((Ch >= 'a') && (Ch <= 'z'))
			{
				return(Ch - 32);
			}
			return (Ch);
		}

		int tcsicmp(LPWSTR s1, LPWSTR s2)
		{
			while (*s1)
			{
				if (!(*s1 == *s2 || (!((*s1 & 0x80)*(*s2 & 0x80)) && ((*s1 & 0xDF) == (*s2 & 0xDF))))) { break; }
				s1++;
				s2++;
			}
			return WORD(UpChar(*s1)) - WORD(UpChar(*s2));
		}

		int tcsicmp(LPSTR s1, LPSTR s2)
		{
			while (*s1)
			{
				if (!(*s1 == *s2 || (!((*s1 & 0x80)*(*s2 & 0x80)) && ((*s1 & 0xDF) == (*s2 & 0xDF))))) { break; }
				s1++;
				s2++;
			}
			return BYTE(UpChar(*s1)) - BYTE(UpChar(*s2));
		}

		int StringFormatA(std::string &txt, LPSTR lpszFormat, ...)
		{
			char szBuffer[(1 << 12)];
			va_list argList;
			va_start(argList, lpszFormat);
			int nResult = _vsnprintf_s(szBuffer, (1 << 12) - 1, (1 << 12) - 1, lpszFormat, argList);
			va_end(argList);
			txt.assign(szBuffer, nResult);
			return nResult;
		}
		char *scan_string(const char *str, int base)
		{
			char *str_ptr = (char*)str;

			switch (base)
			{
			case 10:
				while (!(isdigit(*str_ptr) || *str_ptr == '-' || *str_ptr == 0x0))
				{
					str_ptr++;
				}
				break;
			case 16:
				while (!(isxdigit(*str_ptr) || *str_ptr == 0x0))
				{
					str_ptr++;
				}
				break;
			}

			return str_ptr;
		}
		int dark_sscanf(const char *str, const char *fmt, ...)
		{
			int ret;
			va_list ap;
			char *format_ptr = (char*)fmt;
			char *str_ptr = (char*)str;

			int *p_int;
			long *p_long;

			ret = 0;

			va_start(ap, fmt);

			while ((*format_ptr != 0x0) && (*str_ptr != 0x0))
			{
				if (*format_ptr == '%')
				{
					format_ptr++;

					if (*format_ptr != 0x0)
					{
						switch (*format_ptr)
						{
						case 'd':       // expect an int
						case 'i':
							p_int = va_arg(ap, int *);
							str_ptr = scan_string(str_ptr, 10);
							if (*str_ptr == 0x0) goto end_parse;
							*p_int = (int)strtol(str_ptr, &str_ptr, 10);
							ret++;
							break;
						case 'D':
						case 'I':       // expect a long
							p_long = va_arg(ap, long *);
							str_ptr = scan_string(str_ptr, 10);
							if (*str_ptr == 0x0) goto end_parse;
							*p_long = strtol(str_ptr, &str_ptr, 10);
							ret++;
							break;
						case 'x':       // expect an int in hexadecimal format
							p_int = va_arg(ap, int *);
							str_ptr = scan_string(str_ptr, 16);
							if (*str_ptr == 0x0) goto end_parse;
							*p_int = (int)strtol(str_ptr, &str_ptr, 16);
							ret++;
							break;
						case 'X':  // expect a long in hexadecimal format       
							p_long = va_arg(ap, long *);
							str_ptr = scan_string(str_ptr, 16);
							if (*str_ptr == 0x0) goto end_parse;
							*p_long = strtol(str_ptr, &str_ptr, 16);
							ret++;
							break;
						}
					}
				}

				format_ptr++;
			}

end_parse:
			va_end(ap);

			if (*str_ptr == 0x0) ret = -1;
			return ret;
		}

		template<typename T, typename V, typename R>
		char* itoa(const T &in, V *res, R base)
		{
			if (base < 2 || base > 16) { *res = 0; return res; }
			char* out = res;
			int quotient = in;
			while (quotient) {
				*out = "0123456789abcdef"[std::abs(quotient % base)];
				++out;
				quotient /= base;
			}
			if (in < 0 && base == 10) *out++ = '-';
			std::reverse(res, out);
			*out = 0;
			return res;
		}
		size_t strlcat(char *dst, const char *src, size_t siz)
		{
			char *d = dst;
			const char *s = src;
			size_t n = siz, dlen;

			while (n-- != 0 && *d != '\0')
				d++;

			dlen = d - dst;
			n = siz - dlen;

			if (n == 0)
				return(dlen + lstrlenA(s));

			while (*s != '\0')
			{
				if (n != 1)
				{
					*d++ = *s;
					n--;
				}

				s++;
			}

			*d = '\0';
			return(dlen + (s - src)); /* count does not include NUL */
		}

		size_t strlcpy(char *dst, const char *src, size_t siz)
		{
			char *d = dst;
			const char *s = src;
			size_t n = siz;

			/* Copy as many bytes as will fit */
			if (n != 0 && --n != 0)
			{
				do
				{
					if ((*d++ = *s++) == 0)
						break;
				} while (--n != 0);
			}

			/* Not enough room in dst, add NUL and traverse rest of src */
			if (n == 0)
			{
				if (siz != 0)
					*d = '\0'; /* NUL-terminate dst */
				while (*s++);
			}

			return(s - src - 1); /* count does not include NUL */
		}
		int charlcat(char* x, char y, int z)
		{
			char* x__n = x;
			int v = 0;

			while (*x__n++)
				v++;

			if (v < z - 1)
			{
				*--x__n = y;
				*++x__n = 0;
			}

			return v;
		}

		bool charremove(char* mp, char remove)
		{
			char* mptr = mp;
			bool shift_down = FALSE;

			while (*mptr)
			{
				if (*mptr == remove)
					shift_down = TRUE;

				if (shift_down)
					*mptr = *(mptr + 1);

				mptr++;
			}

			return shift_down;
		}
	}

}