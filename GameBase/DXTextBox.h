#pragma once



#include "DXDef.h"
#include "DXObjectBase.h"
#include "DXListVerticalSlider.h"



class C_DX_TEXT_BOX : public C_DX_OBJECT_BASE
{
private:
	C_DARK_TIMER m_tUpArrow, m_tDownArrow;
	C_DARK_COLOR * pString, * pInner, * pBorder;

	int nStart;
	BOOL m_bUpArrow_MouseOver, m_bDownArrow_MouseOver, m_bSlider_MouseOver, bDragging;

	std::vector<_tstring> m_vStrings;

	C_DX_LIST_VERTICAL_SLIDER * pSlider;

public:
	C_DX_TEXT_BOX( C_TINY_XML_ELEMENT * pElement );

	void Display();
	void Calculate();
	void MouseMove(  );
	void KeyEvent( _KEY sKey );

	void AddString( _tstring sString );
	void Clear();

	void UpdateObject( int iIndex );
};