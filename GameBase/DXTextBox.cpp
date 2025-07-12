#include "stdafx.h"
#include "DXTextBox.h"
#include "DXEngine.h"



C_DX_TEXT_BOX::C_DX_TEXT_BOX( C_TINY_XML_ELEMENT * pElement )
{
	SetObject( pElement );

	pSlider = new C_DX_LIST_VERTICAL_SLIDER( C_DARK_POINT( GetWidth() - LIST_SLIDER_WIDTH + 2, 0 ), GetHeight() );

	for( C_TINY_XML_ELEMENT * pString = pElement->FirstChildElement( _T("Row") ); pString; pString = pString->NextSiblingElement( _T("Row") ) )
		AddString( pString->GetText() );

	SetThemeElement( DXENGINE()->GetThemeElement( _T("TextBox") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("TextBox"), 0 );
	else
		SetObjectState( _T("Normal") );
}

void C_DX_TEXT_BOX::Display()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	DXENGINE()->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

	int iAddHeight = DXENGINE()->GetFont()->GetStringHeight();
	if( m_vStrings.size() )
		for( int i = pSlider->GetValue(), iHeight = 0; i <= pSlider->GetMaxValue() && iHeight < GetHeight() - DXENGINE()->GetFont()->GetStringHeight(); i++ )
		{
			DXENGINE()->GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + iHeight, 0, pString->GetD3DCOLOR(), m_vStrings[ i ], GetWidth() - LIST_SLIDER_WIDTH );
			iHeight += iAddHeight;
		}

	pSlider->Display( Pos );
}

void C_DX_TEXT_BOX::Calculate()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	pSlider->Calculate(Pos);
}

void C_DX_TEXT_BOX::MouseMove(  )
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	SetMouseOver( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) );

	pSlider->MouseMove( Pos );
}

void C_DX_TEXT_BOX::KeyEvent( _KEY sKey )
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	if( GetMouseOver() || ( !sKey.bDown && !DXMOUSE()->GetWheel() )  )
		pSlider->KeyEvent( Pos, sKey );
}

void C_DX_TEXT_BOX::AddString( _tstring sString )
{
	if( !sString.length() )
		return;

	std::vector<_tstring> vPending;
	int iLength = static_cast<int>( sString.length() );
	for( int i = iLength - 1; i > 0; i-- )
	{
		if( sString[ i ] == '\n' )
		{
			sString[ i ] = '\0';
			
			if( i + 1 < iLength )
			{
				if( sString[ i + 1 ] == '\r' )
				{
					if( i + 2 < iLength )
						vPending.push_back( &sString.c_str()[ i + 2 ] );
				}
				else
					vPending.push_back( &sString.c_str()[ i + 1 ] );
			}
		}
	}

	pSlider->SetMaxValue( m_vStrings.size() );
	m_vStrings.push_back( sString.c_str() );

	int iHeight = 0;
	for( int i = pSlider->GetValue(); i <= pSlider->GetMaxValue(); i++ )
	{
		float fWidth = static_cast<float>( DXENGINE()->GetFont()->GetStringWidth( m_vStrings[ i ].c_str() ) );
		int iLines = static_cast<int>( ceilf( fWidth / ( GetWidth() - LIST_SLIDER_WIDTH ) ) );

		int iTempHeight = iLines*DXENGINE()->GetFont()->GetStringHeight();
		iHeight += iTempHeight;
	
		while( iHeight > GetHeight() - DXENGINE()->GetFont()->GetStringHeight() )
		{
			pSlider->SetValue( pSlider->GetValue() + iLines );
			iHeight -= iTempHeight;
		}
	}

	for( std::vector<_tstring>::reverse_iterator iIter = vPending.rbegin(); iIter != vPending.rend(); iIter++ )
		AddString( *iIter );
}

void C_DX_TEXT_BOX::Clear()
{
	m_vStrings.clear();
}

void C_DX_TEXT_BOX::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	pString = pState->GetColor( _T("String") );
	pInner = pState->GetColor( _T("Inner") );
	pBorder = pState->GetColor( _T("Border") );
}