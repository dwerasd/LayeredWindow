#pragma once


#include "DXDef.h"
#include "DXObjectBase.h"



class C_DX_CHECK_BOX : public C_DX_OBJECT_BASE
{
private:
	BOOL m_bChecked;

	C_DARK_COLOR * pInner, * pBorder, * pString, * pCross;

public:
	C_DX_CHECK_BOX( C_TINY_XML_ELEMENT * pElement );

	BOOL GetChecked();
	void SetChecked( BOOL bChecked );

	void Display();
	void Calculate();
	void MouseMove(  );
	void KeyEvent( _KEY sKey );

	void UpdateObject( int iIndex );
};