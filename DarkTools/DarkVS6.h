#ifndef __DARK_DEFINED_FOR_VS6__
#define __DARK_DEFINED_FOR_VS6__


#if (_MSC_VER < 1310)	// Visual studio 6 이하라면


#define		wcscat_s(a, b, c)					wcscat(a, c)
#define		strcat_s(a, b, c)					strcat(a, c)
#define		_tcscat_s(a, b, c)					_tcscat(a, c)
#define		_tcsncat_s(a,b,c,d)					_tcsncat(a,c,d)

#define		_vsntprintf_s(a, b, c, d, e)		_vsntprintf(a, b, d, e)
#define		_vstprintf_s(a, b, c)				_vstprintf(a, b, c)
#define		_vsnwprintf_s(a, b, c, d, e)		_vsnwprintf(a, b, d, e)
#define		_vsnprintf_s(a, b, c, d, e)			_vsnprintf(a, b, d, e)
#define		vswprintf_s(a, b, c, d)				vswprintf(a, c, d)

#define		wcscpy_s(a,b,c)						wcscpy(a,c)
#define		strcpy_s(a,b,c)						strcpy(a,c)
#define		_tcscpy_s(a,b,c)					_tcscpy(a,c)

#define		_tcslwr_s(a)						_tcslwr(a)
#define		wcsncpy_s(a,b,c,d)					wcsncpy(a,c,d)
#define		strncpy_s(a,b,c,d)					strncpy(a,c,d)
#define		_tcsncpy_s(a,b,c,d);				_tcsncpy(a,c,d)
#define		memcpy_s(a,b,c,d)					memcpy(a,c,b)

#define		_itot_s(a,b,c)						_itot(a,b,c)
#define		_i64tot_s(a,b,c,d)					_i64tot(a,b,d)
#define		_tscanf_s(a, b, c)

//#define		ULONG_PTR							unsigned long
#endif


#endif	// __DARK_DEFINED_FOR_VS6__