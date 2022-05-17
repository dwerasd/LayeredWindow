#pragma once


#include "DXDef.h"
#include "DXObjectBase.h"
//#include "DXButton.h"
#include "DXButtonCross.h"


class C_DX9_WINDOW
	: public C_DX9_OBJECT
{
private:
	BOOL bMinimumWindow, bDragging;

	dk::DPOINT dpMoveWindow;

	std::vector<C_DX9_OBJECT *> vObjects;
	C_DX9_OBJECT * pFocussedObject;

	dk::DCOLOR * pTitle, * pBodyInner, * pBodyBorder;
	C_DX9_TEXTURE * pTitlebar;

	long nTitlebarSize, nCloseBtnSize;
	C_DX9_CROSS_BUTTON *pCloseBtn;

public:
	C_DX9_WINDOW( long width = 100, long height = 100, long nTitlebarSize = 24);
	C_DX9_WINDOW( C_TINY_XML_ELEMENT * pElement, long nTitlebarSize = 24);
	~C_DX9_WINDOW();

	void AddTitleObject( C_DX9_OBJECT * pElement );
	void AddObject( C_DX9_OBJECT * pElement );

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

	void SetCloseBtnCallback(C_DX9_CALLBACK_BASE *p);
	C_DX9_CALLBACK_BASE *GetCloseBtnCallback();

	void SetFocussedObject( C_DX9_OBJECT * pElement );
	C_DX9_OBJECT *GetFocussedObject();
	C_DX9_OBJECT *GetNextObject( C_DX9_OBJECT * pElement );
	C_DX9_OBJECT *GetObjectByString( LPCTSTR pszString, int iIndex = 0 );
	C_DX9_OBJECT *GetObjectByName( LPCTSTR pszString);

	long GetTitlebarSize() { return(nTitlebarSize); }
	long GetCloseButtonSize() { return(nCloseBtnSize); }

	void BringToTop( C_DX9_OBJECT * pElement );

	void UpdateObject( int iIndex );
};