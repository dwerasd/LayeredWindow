#include "stdafx.h"
#include "DXHorizontalSliderBar.h"
#include "DXEngine.h"



C_DX_HORIZONTAL_SLIDER_BAR::C_DX_HORIZONTAL_SLIDER_BAR()
{
	SetDragged( FALSE );
	SetCallback( 0 );
	m_iMinValue = 0, m_iMaxValue = 0, m_iValue = 0;
}

C_DX_HORIZONTAL_SLIDER_BAR::C_DX_HORIZONTAL_SLIDER_BAR( C_TINY_XML_ELEMENT * pElement )
{
	SetDragged( FALSE );
	SetCallback( 0 );
	m_iMinValue = 0, m_iMaxValue = 0, m_iValue = 0;

	SetSliderElement( pElement );

	SetThemeElement( DXENGINE()->GetThemeElement( _T("HorizontalSliderBar") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("HorizontalSliderBar"), 0 );
	else
		SetObjectState( _T("Normal") );
}

void C_DX_HORIZONTAL_SLIDER_BAR::SetSliderElement( C_TINY_XML_ELEMENT * pElement )
{
	SetObject( pElement );

	if( pElement->Attribute( _T("string") ) )
		SetShowString( FALSE );
	else
		SetShowString( TRUE );

	int iTempValue = 0;

	if( pElement->QueryIntAttribute( _T("minvalue"), &iTempValue ) != TINY_XML_NO_ATTRIBUTE )
		SetMinValue( iTempValue );
	else
		SetMinValue( 0 );

	if( pElement->QueryIntAttribute( _T("maxvalue"), &iTempValue ) != TINY_XML_NO_ATTRIBUTE )
		SetMaxValue( iTempValue );
	else
		SetMaxValue( 0 );

	if( pElement->QueryIntAttribute( _T("step"), &iTempValue ) != TINY_XML_NO_ATTRIBUTE )
		SetStep( iTempValue );
	else
		SetMaxValue( 0 );

	if( pElement->QueryIntAttribute( _T("value"), &iTempValue ) != TINY_XML_NO_ATTRIBUTE )
		SetValue( iTempValue );
	else
		SetMaxValue( 0 );

	C_TINY_XML_ELEMENT * pUpdater = pElement->FirstChildElement( _T("Updater") );
	if( pUpdater )
	{
		if( pUpdater->GetText() )
		{
			C_DX_VARIANT * pCvar = DXENGINE()->Cvars[ pUpdater->GetText() ];

			if( pCvar )
				pCallback = pCvar->GetCallback();
			else
				pCallback = 0;
		}
		else
			pCallback = 0;
	}
	else
		pCallback = 0;
}

void C_DX_HORIZONTAL_SLIDER_BAR::Display()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	_OBJECT_STATE * pState = GetElementState();
	if( pState )
	{
		D3DCOLOR d3dLineColor = pLines->GetD3DCOLOR();

		DXENGINE()->DrawLine( Pos.GetX(),					Pos.GetY() + HORIZONTAL_SLIDER_HEIGHT / 2,Pos.GetX() + GetWidth(),		Pos.GetY() + HORIZONTAL_SLIDER_HEIGHT / 2,		1, d3dLineColor );
		DXENGINE()->DrawLine( Pos.GetX(),					Pos.GetY() + HORIZONTAL_SLIDER_HEIGHT / 4, Pos.GetX(),					Pos.GetY() + HORIZONTAL_SLIDER_HEIGHT / 4 * 3,	1, d3dLineColor );
		DXENGINE()->DrawLine( Pos.GetX() + GetWidth(),		Pos.GetY() + HORIZONTAL_SLIDER_HEIGHT / 4, Pos.GetX() + GetWidth(),		Pos.GetY() + HORIZONTAL_SLIDER_HEIGHT / 4 * 3,	1, d3dLineColor );
		DXENGINE()->DrawLine( Pos.GetX() + GetWidth() / 2,	Pos.GetY() + HORIZONTAL_SLIDER_HEIGHT / 4, Pos.GetX() + GetWidth() / 2,	Pos.GetY() + HORIZONTAL_SLIDER_HEIGHT / 4 * 3,	1, d3dLineColor );

		pSlider->Display( C_DARK_POINT( Pos.GetX() + static_cast<int>( floor( static_cast<float>( GetWidth() ) / ( GetMaxValue() - GetMinValue() ) * ( GetValue() - GetMinValue() ) ) ) - 5, Pos.GetY() + 2 ), 10, HORIZONTAL_SLIDER_HEIGHT - 4 );

		DXENGINE()->GetFont()->DrawString( Pos.GetX() + GetWidth() / 2, Pos.GetY() - 15, FT_CENTER, pString->GetD3DCOLOR(), GetFormatted() );
	}
}

void C_DX_HORIZONTAL_SLIDER_BAR::Calculate()
{
	if( !GetFormatted().length() )
		GetString( !GetShowString() );

	if( pCallback )
		pCallback->Calculate(this, reinterpret_cast<LPCTSTR>( GetValue() ));
}

void C_DX_HORIZONTAL_SLIDER_BAR::MouseMove(  )
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	if( GetDragged() )
	{
		C_DARK_POINT mousePos = DXMOUSE()->GetPos();
		if( mousePos.GetX() == -1 && mousePos.GetY() == -1 )
		{
			mousePos = DXMOUSE()->GetSavedPos();
		}
		if( mousePos.GetX() < Pos.GetX() )
		{
			SetValue( GetMinValue() );
		}
		else if( mousePos.GetX() > Pos.GetX() + GetWidth() )
		{
			SetValue( GetMaxValue() );
		}
		else
		{
			for( int iIndex = GetMinValue(); iIndex < GetMaxValue(); iIndex++ )
			{
				if( mousePos.GetX() >= Pos.GetX() + floor( static_cast<float>( GetWidth() ) / ( GetMaxValue() - GetMinValue() ) * ( iIndex - GetMinValue() ) ) && mousePos.GetX() <= Pos.GetX() + floor( static_cast<float>( GetWidth() ) / ( GetMaxValue() - GetMinValue() ) * ( iIndex + 1 - GetMinValue() ) ) )
				{
					SetValue( iIndex );
					break;
				}
			}
		}
		if( GetCallback() )
		{
			GetCallback()->Calculate(this, reinterpret_cast<LPCTSTR>( GetValue() ));
		}
		if( GetDragged() )
		{
			SetObjectState( SetMouseOver( DXMOUSE()->InArea( pWindow->GetAbsolutePos()->GetX(), pWindow->GetAbsolutePos()->GetY(), pWindow->GetWidth(), pWindow->GetHeight() ) )?_T("Pressed"):_T("Normal") );
		}
		else
		{
			SetObjectState( SetMouseOver( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) )?_T("MouseOver"):_T("Normal") );
		}
	}
	else
	{
		SetObjectState( SetMouseOver( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), HORIZONTAL_SLIDER_HEIGHT ) )?_T("MouseOver"):_T("Normal") );
	}
}

void C_DX_HORIZONTAL_SLIDER_BAR::KeyEvent( _KEY sKey )
{
	if( !sKey.cKey )
	{
		SetDragged( GetMouseOver() && DXMOUSE()->GetLeftButton() );

		

		//SetObjectState( GetDragged()?_T("Pressed"):( GetMouseOver()?_T("MouseOver"):_T("Normal") ) );

		if( GetDragged() )
			MouseMove( );
	}
}

int C_DX_HORIZONTAL_SLIDER_BAR::GetMinValue()
{
	return m_iMinValue;
}

int C_DX_HORIZONTAL_SLIDER_BAR::GetMaxValue()
{
	return m_iMaxValue;
}

int C_DX_HORIZONTAL_SLIDER_BAR::GetValue()
{
	return m_iValue;
}

int C_DX_HORIZONTAL_SLIDER_BAR::GetStep()
{
	return m_iStep;
}

void C_DX_HORIZONTAL_SLIDER_BAR::SetMinValue( int iMinValue )
{
	m_iMinValue = iMinValue;
}

void C_DX_HORIZONTAL_SLIDER_BAR::SetMaxValue( int iMaxValue )
{
	m_iMaxValue = iMaxValue;
}

void C_DX_HORIZONTAL_SLIDER_BAR::SetValue( int iValue )
{
	if( iValue > GetMaxValue() )
		m_iValue = GetMaxValue();
	else if( iValue < GetMinValue() )
		m_iValue = GetMinValue();
	else
		m_iValue = iValue;

	if( GetShowString() )
	{
		_tstringstream sStream;
		sStream << GetValue() << _T("/") << GetMaxValue();
		SetString( sStream.str() );
		GetString();
	}
	else
		GetString( TRUE );
}

void C_DX_HORIZONTAL_SLIDER_BAR::SetStep( int iStep )
{
	m_iStep = iStep;
}

BOOL C_DX_HORIZONTAL_SLIDER_BAR::GetDragged()
{
	return m_bDragged;
}

void C_DX_HORIZONTAL_SLIDER_BAR::SetDragged( BOOL bDragged )
{
	m_bDragged = bDragged;

	if( m_bDragged )
		SetObjectState( _T("Pressed") );
}

void C_DX_HORIZONTAL_SLIDER_BAR::SetShowString( BOOL bShow )
{
	m_bShowString = bShow;
}

BOOL C_DX_HORIZONTAL_SLIDER_BAR::GetShowString()
{
	return m_bShowString;
}

void C_DX_HORIZONTAL_SLIDER_BAR::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	pLines = pState->GetColor( _T("Lines") );
	pString = pState->GetColor( _T("String") );

	pSlider = pState->GetTexture( _T("Slider") );
}

// SliderBar specific GUI cfunctions
_tstring C_SET_MIN_VALUE::Calculate(C_DX_OBJECT_BASE * pElement, LPCTSTR)
{
	_tstringstream sStream;

	if( pElement )
	{
		C_DX_HORIZONTAL_SLIDER_BAR * pSlider = (C_DX_HORIZONTAL_SLIDER_BAR*)pElement;

		try
		{
			sStream << pSlider->GetMinValue();
		}
		catch( ... )
		{
			MessageBox( 0, _T("$MinValue failed"), pElement->GetString().c_str(), 0 );
		}
	}
	return sStream.str();
}

_tstring C_SET_MAX_VALUE::Calculate( C_DX_OBJECT_BASE * pElement, LPCTSTR )
{
	_tstringstream sStream;

	if( pElement )
	{
		C_DX_HORIZONTAL_SLIDER_BAR * pSlider = (C_DX_HORIZONTAL_SLIDER_BAR*)pElement;

		try
		{
			sStream << pSlider->GetMaxValue();
		}
		catch( ... )
		{
			MessageBox( 0, _T("$MaxValue failed"), pElement->GetString().c_str(), 0 );
		}
	}
	return sStream.str();
}

_tstring C_SET_SLIDER_VALUE::Calculate( C_DX_OBJECT_BASE * pElement, LPCTSTR )
{
	_tstringstream sStream;

	if( pElement )
	{
		C_DX_HORIZONTAL_SLIDER_BAR * pSlider = (C_DX_HORIZONTAL_SLIDER_BAR*)pElement;

		try
		{
			sStream << pSlider->GetValue();
		}
		catch( ... )
		{
			MessageBox( 0, _T("$Value failed"), pElement->GetString().c_str(), 0 );
		}
	}
	return sStream.str();
}
// SliderBar specific GUI cfunctions