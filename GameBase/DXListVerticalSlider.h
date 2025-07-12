#pragma once


#include "DXDef.h"
#include "DXTexture.h"
#include "DXHorizontalSliderBar.h"


class C_DX_LIST_VERTICAL_SLIDER : public C_DX_HORIZONTAL_SLIDER_BAR
{
	BOOL bMouseOver[ 3 ], m_bPressed[ 2 ], m_bDragged;
	C_DARK_TIMER m_tUpArrow, m_tDownArrow;

	_OBJECT_BASE * m_pThemeElement[ 3 ];
	_OBJECT_STATE * m_pElementState[ 3 ];

	void SetDragged( BOOL bDragged );
	BOOL GetDragged();

	C_DARK_COLOR * pInner, * pBorder;
	C_DX_TEXTURE * pSlider, * pUpArrow, * pDownArrow;

public:
	C_DX_LIST_VERTICAL_SLIDER( C_DARK_POINT relPos, int iHeight );

	void Calculate(C_DARK_POINT basePos);
	void Display( C_DARK_POINT basePos );
	void MouseMove( C_DARK_POINT basePos);
	BOOL KeyEvent( C_DARK_POINT basePos, _KEY sKey );

	void UpdateObject( int iIndex );
};