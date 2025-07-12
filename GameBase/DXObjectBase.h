#pragma once

struct _OBJECT_BASE;
struct _OBJECT_STATE;

class C_DX_CALLBACK_BASE;

#include "DXDef.h"
#include "DXCallbackBase.h"
#include "DXTexture.h"



class C_DX_OBJECT_BASE
{
private:
	BOOL bHasFocus, bMouseOver, bVisible, bTabstop;
	_tstring tstrObjectName;
	_tstring tstrSource[2], tstrDisplay[2];

	int m_iWidth, m_iHeight;

	C_DARK_POINT PosRelease, PosAbsolute;
	LPVOID pParent;

	C_DX_CALLBACK_BASE *pCallback;

	_OBJECT_BASE * m_pThemeElement[ 3 ];
	_OBJECT_STATE * m_pElementState[ 3 ];

public:
	C_DX_OBJECT_BASE() : pCallback(0), bVisible(0), bTabstop(0)
	{

	}
	~C_DX_OBJECT_BASE()
	{

	}
	
	void SetObject( C_TINY_XML_ELEMENT * pElement, long width = 20, long height = 20);

	void SetParent( LPVOID p );
	LPVOID GetParent();

	void SetCallback(C_DX_CALLBACK_BASE *p);
	C_DX_CALLBACK_BASE *GetCallback();

	void SetRelPos( C_DARK_POINT relPos );
	void SetAbsolutePos( C_DARK_POINT absPos );

	C_DARK_POINT * GetRelPos();
	C_DARK_POINT * GetAbsolutePos();

	void SetWidth( int iWidth );
	void SetHeight( int iHeight );

	int GetWidth();
	int GetHeight();

	BOOL HasFocus();

	void SetString( _tstring sString, int iIndex = 0 );
	_tstring GetString( BOOL bReplaceVars = FALSE, int iIndex = 0 );
	_tstring GetFormatted( int iIndex = 0 );

	BOOL GetMouseOver();
	BOOL SetMouseOver( BOOL bMouseOver );

	_OBJECT_BASE * SetThemeElement( _OBJECT_BASE * pThemeElement, int iIndex = 0 );
	_OBJECT_BASE * GetThemeElement( int iIndex = 0 );

	void SetObjectState( _tstring sState, int iIndex = 0 );
	_OBJECT_STATE * GetElementState( int iIndex = 0 );

	//////////////////////////////////////////////////////////////////////////
	// 모든 오브젝트는 비지블 옵션을 가진다.

	void SetVisible(BOOL b) { bVisible = b; }
	BOOL GetVisible() { return(bVisible); }

	void SetTabStop(BOOL b) { bTabstop = b; }
	BOOL GetTabStop() { return(bTabstop); }


	//////////////////////////////////////////////////////////////////////////
	// 오브젝트 이름으로 찾을 수 있도록 모든 오브젝트에 다른 이름을 부여하자.

	void SetObjectName(LPCTSTR ptszObjectName) { tstrObjectName = ptszObjectName; }
	_tstring GetObjectName() { return(tstrObjectName); }

	virtual void Display();
	virtual void Calculate();
	virtual void MouseMove(  );
	virtual void KeyEvent( _KEY sKey );

	virtual void UpdateObject( int iIndex );
};