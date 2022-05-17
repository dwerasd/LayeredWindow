#pragma once



#include "DXDef.h"
#include "DXTexture.h"
#include "DXObjectBase.h"
#include "DXVerticalSliderBar.h"



class C_DX9_HORIZONTAL_SLIDER_BAR : public C_DX9_OBJECT
{
private:
	friend class C_DX9_VERTICAL_SLIDER_BAR;

	int m_iMinValue, m_iMaxValue, m_iValue, m_iStep;
	BOOL m_bDragged, m_bShowString;

	//typedef std::_tstring ( __cdecl * tAction )( LPCTSTR pszArgs, C_DX9_OBJECT * pElement );
	//tAction m_pUpdater;

	C_DX9_CALLBACK_BASE *pCallback;

	dk::DCOLOR * pLines, * pString;
	C_DX9_TEXTURE * pSlider;

public:
	C_DX9_HORIZONTAL_SLIDER_BAR();
	C_DX9_HORIZONTAL_SLIDER_BAR( C_TINY_XML_ELEMENT * pElement );

	void SetSliderElement( C_TINY_XML_ELEMENT * pElement );

	void Display();
	void Calculate();
	void MouseMove(  );
	void KeyEvent( _KEY sKey );

	int GetMinValue();
	int GetMaxValue();
	int GetValue();
	int GetStep();

	void SetMinValue( int iMinValue );
	void SetMaxValue( int iMaxValue );
	void SetValue( int iValue );
	void SetStep( int iStep );

	BOOL GetDragged();
	void SetDragged( BOOL bDragged );

	void SetShowString( BOOL bShow );
	BOOL GetShowString();

	void UpdateObject( int iIndex );
};

class C_DX9_VERTICAL_SLIDER_BAR : public C_DX9_HORIZONTAL_SLIDER_BAR
{
private:
	dk::DCOLOR * pLines, * pString;
	C_DX9_TEXTURE * pSlider;

public:
	C_DX9_VERTICAL_SLIDER_BAR( C_TINY_XML_ELEMENT * pElement );

	void Display();
	void MouseMove(  );

	void UpdateObject( int iIndex );

};

class C_SET_MIN_VALUE : public C_DX9_CALLBACK_BASE
{
public:
	std::_tstring Calculate(C_DX9_OBJECT * pElement, LPCTSTR);
};

class C_SET_MAX_VALUE : public C_DX9_CALLBACK_BASE
{
public:
	std::_tstring Calculate(C_DX9_OBJECT * pElement, LPCTSTR);
};

class C_SET_SLIDER_VALUE : public C_DX9_CALLBACK_BASE
{
public:
	std::_tstring Calculate(C_DX9_OBJECT * pElement, LPCTSTR);
};

//std::_tstring MaxValue( LPCTSTR, C_DX9_OBJECT * pElement );
//std::_tstring SliderValue( LPCTSTR, C_DX9_OBJECT * pElement );