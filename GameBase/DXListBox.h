#pragma once



#include "DXDef.h"
#include "DXObjectBase.h"
#include "DXListVerticalSlider.h"



class C_DX9_LIST_BOX : public C_DX9_OBJECT
{
private:
	int m_iMouseOverIndex;

	C_DX9_LIST_VERTICAL_SLIDER * pSlider;

	std::vector<std::_tstring> m_vRows;

	dk::DCOLOR * pInner, * pBorder, * pString, * pMouseOverString;

public:
	C_DX9_LIST_BOX( C_TINY_XML_ELEMENT * pElement );

	void Display();
	void Calculate();
	void MouseMove(  );
	void KeyEvent( _KEY sKey );

	void AddRow( std::_tstring sString );
	std::_tstring GetRow( int iIndex );

	void Clear();

	void UpdateObject( int iIndex );
};