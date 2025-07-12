#pragma once


#include "DXDef.h"
#include "DXObjectBase.h"
//#include "DXButton.h"
#include "DXButtonCross.h"


class C_DX_WINDOW
	: public C_DX_OBJECT_BASE
{
private:
	BOOL bMinimumWindow, bDragging;

	C_DARK_POINT dpMoveWindow;

	std::vector<C_DX_OBJECT_BASE *> vObjects;
	C_DX_OBJECT_BASE * pFocussedObject;

	C_DARK_COLOR * pTitle, * pBodyInner, * pBodyBorder;
	C_DX_TEXTURE * pTitlebar;

	long nTitlebarSize, nCloseBtnSize;
	C_DX_CROSS_BUTTON *pCloseBtn;

public:
	C_DX_WINDOW( long width = 100, long height = 100, long nTitlebarSize = 24);
	C_DX_WINDOW( C_TINY_XML_ELEMENT * pElement, long nTitlebarSize = 24);
	~C_DX_WINDOW();

	void AddTitleObject( C_DX_OBJECT_BASE * pElement );
	void AddObject( C_DX_OBJECT_BASE * pElement );

	void Calculate();
	void DisplayTitlebar();
	void DisplayBoard();
	void Display();
	
	void MouseMove(  );
	void KeyEvent( _KEY sKey );

	void SetMaximumWindow( BOOL bMaximized );
	BOOL GetMaximumWindow();

	void SetDragging( BOOL bDragging );
	BOOL GetDragging();

	void SetCloseBtnCallback(C_DX_CALLBACK_BASE *p);
	C_DX_CALLBACK_BASE *GetCloseBtnCallback();

	void SetFocussedObject( C_DX_OBJECT_BASE * pElement );
	C_DX_OBJECT_BASE *GetFocussedObject();
	C_DX_OBJECT_BASE *GetNextObject( C_DX_OBJECT_BASE * pElement );
	C_DX_OBJECT_BASE *GetObjectByString( LPCTSTR pszString, int iIndex = 0 );
	C_DX_OBJECT_BASE *GetObjectByName( LPCTSTR pszString);

	long GetTitlebarSize() { return(nTitlebarSize); }
	long GetCloseButtonSize() { return(nCloseBtnSize); }

	void BringToTop( C_DX_OBJECT_BASE * pElement );

	void UpdateObject( int iIndex );
};