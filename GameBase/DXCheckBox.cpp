#include "stdafx.h"
#include "DXCheckBox.h"
#include "DXEngine.h"



C_DX_CHECK_BOX::C_DX_CHECK_BOX( C_TINY_XML_ELEMENT * pElement )
{
	SetObject( pElement );

	int iChecked = 0;
	pElement->QueryIntAttribute( _T("checked"), &iChecked );

	SetChecked( iChecked == 1 );

	SetThemeElement( DXENGINE()->GetThemeElement( _T("CheckBox") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("No color scheme element found."), _T("CheckBox"), 0 );
	else
		SetObjectState( _T("Normal") );
}

BOOL C_DX_CHECK_BOX::GetChecked()
{
	return m_bChecked;
}
void C_DX_CHECK_BOX::SetChecked( BOOL bChecked )
{
	m_bChecked = bChecked;
}

void C_DX_CHECK_BOX::Display()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	DXENGINE()->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), 15, 15, pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

	int nSize = GetFormatted().length();
	if (0 < nSize)
	{
		wchar_t wszBuffer[MAX_PATH];
		MultiByteToWideChar(CP_UTF8, 0, GetFormatted().c_str(), nSize + 1, wszBuffer, nSize + 1);

		DXENGINE()->GetFont()->DrawString( Pos.GetX() + 20, Pos.GetY(), 0, pString->GetD3DCOLOR(), wszBuffer );
	}
	if( GetChecked() )
	{
		D3DCOLOR d3dCrossColor = pCross->GetD3DCOLOR();

		DXENGINE()->DrawLine( Pos.GetX(), Pos.GetY(), Pos.GetX() + 14, Pos.GetY() + 14, 1, d3dCrossColor );
		DXENGINE()->DrawLine( Pos.GetX() + 14, Pos.GetY(), Pos.GetX(), Pos.GetY() + 14, 1, d3dCrossColor );
	}
}

void C_DX_CHECK_BOX::Calculate()
{
	GetString( TRUE );
}

void C_DX_CHECK_BOX::MouseMove(  )
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	SetObjectState( SetMouseOver( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY(), DXENGINE()->GetFont()->GetStringWidth( GetFormatted().c_str() ) + 20, DXENGINE()->GetFont()->GetStringHeight() ) )?_T("MouseOver"):_T("Normal") );
}

void C_DX_CHECK_BOX::KeyEvent( _KEY sKey )
{
	if( !sKey.cKey )
	{
		if( GetMouseOver() && DXMOUSE()->GetLeftButton( 0 ) )
		{
			m_bChecked = !m_bChecked;
			
			if( GetCallback() )
				GetCallback()->Calculate(this, (LPCTSTR)m_bChecked);
		}
	}
}

void C_DX_CHECK_BOX::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	pInner = pState->GetColor( _T("Inner") );
	pBorder = pState->GetColor( _T("Border") );
	pString = pState->GetColor( _T("String") );
	pCross = pState->GetColor( _T("Cross") );
}