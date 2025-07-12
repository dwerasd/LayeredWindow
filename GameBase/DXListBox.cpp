#include "stdafx.h"
#include "DXListBox.h"
#include "DXEngine.h"



C_DX_LIST_BOX::C_DX_LIST_BOX( C_TINY_XML_ELEMENT * pElement )
{
	SetObject( pElement );
	m_iMouseOverIndex = -1;

	pSlider = new C_DX_LIST_VERTICAL_SLIDER( C_DARK_POINT( GetWidth() - LIST_SLIDER_WIDTH + 2, 0 ), GetHeight() );

	for( C_TINY_XML_ELEMENT * pColumnElement = pElement->FirstChildElement( _T("Row") ); pColumnElement; pColumnElement = pColumnElement->NextSiblingElement( _T("Row") ) )
		AddRow( pColumnElement->GetText() );

	SetThemeElement( DXENGINE()->GetThemeElement( _T("ListBox") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("ListBox"), 0 );
	else
		SetObjectState( _T("Normal") );
}

void C_DX_LIST_BOX::Display()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	DXENGINE()->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

	int iAddHeight = DXENGINE()->GetFont()->GetStringHeight();
	if( m_vRows.size() )
		for( int i = pSlider->GetValue(), iHeight = 0; i < static_cast<int>( m_vRows.size() ) && iHeight < GetHeight() - DXENGINE()->GetFont()->GetStringHeight(); i++ )
		{
			C_DARK_COLOR * pColor = 0;

			if( i == m_iMouseOverIndex && GetMouseOver() )
				pColor = pMouseOverString;
			else
				pColor = pString;

			DXENGINE()->GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + iHeight, 0, pColor->GetD3DCOLOR(), m_vRows[ i ].c_str(), GetWidth() - LIST_SLIDER_WIDTH );
			iHeight += iAddHeight;
		}

	pSlider->Display( Pos );
}

void C_DX_LIST_BOX::Calculate()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	pSlider->Calculate(Pos);
}

void C_DX_LIST_BOX::MouseMove(  )
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	SetMouseOver( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) );

	m_iMouseOverIndex = -1;
	for( int i = pSlider->GetValue(), iHeight = 0, iStringHeight = DXENGINE()->GetFont()->GetStringHeight(); i < static_cast<int>( m_vRows.size() ) || iHeight < GetHeight(); i++ )
	{
		if( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY() + iHeight, GetWidth() - WINDOW_CLOSE_BUTTON_HEIGHT, iStringHeight ) )
			m_iMouseOverIndex = i;

		iHeight += iStringHeight;
	}

	pSlider->MouseMove( Pos );
}

void C_DX_LIST_BOX::KeyEvent( _KEY sKey )
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	if( !sKey.cKey )
	{
		if( GetMouseOver() )
		{
			if( m_iMouseOverIndex >= 0 && GetCallback() && DXMOUSE()->GetLeftButton() )
			{
				GetCallback()->Calculate(this, reinterpret_cast<LPTSTR>( m_iMouseOverIndex ));
			}
		}
	}

	if( GetMouseOver() || ( !sKey.bDown && !DXMOUSE()->GetWheel() )  )
	{
		pSlider->KeyEvent( Pos, sKey );
		MouseMove( );
	}
}

void C_DX_LIST_BOX::AddRow( _tstring sString )
{
	pSlider->SetMaxValue( m_vRows.size() );
	m_vRows.push_back( sString );
}

_tstring C_DX_LIST_BOX::GetRow( int iIndex )
{
	if( iIndex >= 0 && iIndex < static_cast<int>( m_vRows.size() ) )
		return m_vRows[ iIndex ];
	return _tstring();
}

void C_DX_LIST_BOX::Clear()
{
	m_vRows.clear();
}

void C_DX_LIST_BOX::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	pInner = pState->GetColor( _T("Inner") );
	pBorder = pState->GetColor( _T("Border") );
	pString = pState->GetColor( _T("String") );
	pMouseOverString = pState->GetColor( _T("MouseOverString") );
}