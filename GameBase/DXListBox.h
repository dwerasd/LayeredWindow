#pragma once



#include "DXDef.h"
#include "DXObjectBase.h"
#include "DXListVerticalSlider.h"



class C_DX_LIST_BOX : public C_DX_OBJECT_BASE
{
private:
	int m_iMouseOverIndex;

	C_DX_LIST_VERTICAL_SLIDER * pSlider;

	std::vector<_tstring> m_vRows;

	C_DARK_COLOR * pInner, * pBorder, * pString, * pMouseOverString;

public:
	C_DX_LIST_BOX( C_TINY_XML_ELEMENT * pElement );

	void Display();
	void Calculate();
	void MouseMove(  );
	void KeyEvent( _KEY sKey );

	void AddRow( _tstring sString );
	_tstring GetRow( int iIndex );

	void Clear();

	void UpdateObject( int iIndex );
};