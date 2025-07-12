#include "stdafx.h"
#include "DXVerticalSliderBar.h"
#include "DXEngine.h"



C_DX_VERTICAL_SLIDER_BAR::C_DX_VERTICAL_SLIDER_BAR( C_TINY_XML_ELEMENT * pElement )
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

void C_DX_VERTICAL_SLIDER_BAR::Display()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	D3DCOLOR d3dLineColor = pLines->GetD3DCOLOR();
	
	DXENGINE()->DrawLine( Pos.GetX() + VERTICAL_SLIDER_WIDTH / 2, Pos.GetY(),					Pos.GetX() + VERTICAL_SLIDER_WIDTH / 2,		Pos.GetY() + GetHeight(), 1,		d3dLineColor );
	DXENGINE()->DrawLine( Pos.GetX() + VERTICAL_SLIDER_WIDTH / 4, Pos.GetY(),					Pos.GetX() + VERTICAL_SLIDER_WIDTH / 4 * 3,	Pos.GetY(), 1,						d3dLineColor );
	DXENGINE()->DrawLine( Pos.GetX() + VERTICAL_SLIDER_WIDTH / 4, Pos.GetY() + GetHeight(),		Pos.GetX() + VERTICAL_SLIDER_WIDTH / 4 * 3,	Pos.GetY() + GetHeight(), 1,		d3dLineColor );
	DXENGINE()->DrawLine( Pos.GetX() + VERTICAL_SLIDER_WIDTH / 4, Pos.GetY() + GetHeight() / 2,	Pos.GetX() + VERTICAL_SLIDER_WIDTH / 4 * 3,	Pos.GetY() + GetHeight() / 2, 1,	d3dLineColor );

	pSlider->Display( C_DARK_POINT( Pos.GetX() + 2, Pos.GetY() + GetHeight() - static_cast<int>( floor( static_cast<float>( GetHeight() ) / GetMaxValue() * GetValue() ) ) - 5 ), WINDOW_CLOSE_BUTTON_HEIGHT, 10 );

	DXENGINE()->GetFont()->DrawString( Pos.GetX() - VERTICAL_SLIDER_WIDTH + ( VERTICAL_SLIDER_WIDTH * 3 ) / 2, Pos.GetY() + GetHeight() + 5, FT_CENTER, pString->GetD3DCOLOR(), GetFormatted() );
}

void C_DX_VERTICAL_SLIDER_BAR::MouseMove(  )
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

		if( mousePos.GetY() < Pos.GetY() )
		{
			SetValue( GetMaxValue() );
		}
		else if( mousePos.GetY() > Pos.GetY() + GetHeight() )
		{
			SetValue( GetMinValue() );
		}
		else
		{
			for( int iIndex = GetMinValue(); iIndex < GetMaxValue(); iIndex++ )
			{
				if( mousePos.GetY() >= Pos.GetY() + floor( static_cast<float>( GetHeight() ) / GetMaxValue() * iIndex ) && mousePos.GetY() <= Pos.GetY() + floor( (float)GetHeight() / GetMaxValue() * ( iIndex + 1 ) ) )
				{
					SetValue( GetMaxValue() - iIndex );
					break;
				}
			}
		}

		if( GetCallback() )
			GetCallback()->Calculate(this, reinterpret_cast<LPTSTR>( GetValue() ));

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
		SetObjectState( SetMouseOver( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY(), VERTICAL_SLIDER_WIDTH, GetHeight() ) )?_T("MouseOver"):_T("Normal") );
	}
}

void C_DX_VERTICAL_SLIDER_BAR::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	pLines = pState->GetColor( _T("Lines") );
	pString = pState->GetColor( _T("String") );

	pSlider = pState->GetTexture( _T("Slider") );
}