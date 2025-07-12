#pragma once


#include "DXDef.h"



class C_KEYBD
{
private:
	C_KEYBD()
	{

	}
	~C_KEYBD()
	{

	}
	_KEY _Key;

public:
	static C_KEYBD *GetInstance()
	{
		static C_KEYBD *pDXKeyboard = 0;
		if (0 == pDXKeyboard)
		{
			pDXKeyboard = new C_KEYBD();
		}
		return(pDXKeyboard);
	}
	BOOL Calculate( unsigned int uMsg, __w64 unsigned int wParam, _w64 long lParam );
	void KeyDown(TCHAR c, LPARAM l = 0);
	void KeyUp(TCHAR c, LPARAM l = 0);
};

#define DXKEYBD C_KEYBD::GetInstance