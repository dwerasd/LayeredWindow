#pragma once


#include "DXDef.h"
#include "DXTexture.h"
#include "DXHorizontalSliderBar.h"


class C_DX9_LIST_VERTICAL_SLIDER : public C_DX9_HORIZONTAL_SLIDER_BAR
{
	BOOL bMouseOver[ 3 ], m_bPressed[ 2 ], m_bDragged;
	dk::C_TIMER m_tUpArrow, m_tDownArrow;

	_OBJECT_BASE * m_pThemeElement[ 3 ];
	_OBJECT_STATE * m_pElementState[ 3 ];

	void SetDragged( BOOL bDragged );
	BOOL GetDragged();

	dk::DCOLOR * pInner, * pBorder;
	C_DX9_TEXTURE * pSlider, * pUpArrow, * pDownArrow;

public:
	C_DX9_LIST_VERTICAL_SLIDER( dk::DPOINT relPos, int iHeight );

	void Calculate(dk::DPOINT basePos);
	void Display( dk::DPOINT basePos );
	void MouseMove( dk::DPOINT basePos);
	BOOL KeyEvent( dk::DPOINT basePos, _KEY sKey );

	void UpdateObject( int iIndex );
};