#pragma once


#include "DXDef.h"



class C_DX_EDIT_BOX : public C_DX_OBJECT_BASE
{
private:
	BOOL bCursorState, bPassword;

	int nStart, nIndex, nFakeStart, nFakeIndex;
	int nCursorX, nFakeCursorX;
	
	C_DARK_TIMER tCursorTimer;
	C_DARK_COLOR * pInner, * pBorder, * pString, * pCursor;

public:
	C_DX_EDIT_BOX( C_TINY_XML_ELEMENT * pElement );

	void Display();

	void Calculate();
	void MouseMove(  );
	void KeyEvent( _KEY sKey );

	int GetIndex();
	void SetIndex( int iIndex );

	int GetStart();
	void SetStart( int iStart );

	void UpdateObject( int iIndex );
};