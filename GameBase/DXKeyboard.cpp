#include "stdafx.h"
#include "DXKeyboard.h"
#include "DXMouse.h"
#include "DXEngine.h"



BOOL C_KEYBD::Calculate( unsigned int _nMessage, __w64 unsigned int wParam, _w64 long lParam )
{
	if( _nMessage < WM_KEYFIRST || _nMessage > WM_KEYLAST || DXMOUSE()->GetLeftButton() )
	{
		return(0);
	}
	switch( _nMessage )
	{
	case WM_KEYDOWN:
		{
			_Key.cKey = static_cast<TCHAR>( wParam );
			_Key.bDown = TRUE;
			_Key.lParam = lParam;
		}
		break;
	case WM_KEYUP:
		{
			_Key.cKey = static_cast<TCHAR>( wParam );
			_Key.bDown = FALSE;
			_Key.lParam = lParam;
		}
		break;
	}
	_KEY kRet(_Key.cKey, _Key.bDown, _Key.lParam);

	_Key.cKey = static_cast<TCHAR>( wParam );
	_Key.bDown = FALSE;
	_Key.lParam = lParam;

	return DXENGINE()->KeyEvent( kRet );
}

void C_KEYBD::KeyDown(TCHAR c, LPARAM lParam)
{
	_Key.cKey = static_cast<TCHAR>(c);
	_Key.bDown = TRUE;
	_Key.lParam = lParam;

	DXENGINE()->KeyEvent( _Key );
}

void C_KEYBD::KeyUp(TCHAR c, LPARAM lParam)
{
	_Key.cKey = static_cast<TCHAR>(c);
	_Key.bDown = FALSE;
	_Key.lParam = lParam;

	DXENGINE()->KeyEvent( _Key );
}