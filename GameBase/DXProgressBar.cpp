#include "stdafx.h"
#include "DXProgressBar.h"
#include "DXEngine.h"



C_PROGRESS_BAR::C_PROGRESS_BAR( C_TINY_XML_ELEMENT * pElement )
{
	SetObject( pElement );

	int iProgress = 0;
	pElement->QueryIntAttribute( _T("value"), &iProgress );
	SetProgress( iProgress );

	SetThemeElement( DXENGINE()->GetThemeElement( _T("ProgressBar") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("ProgressBar"), 0 );
	else
		SetObjectState( _T("Normal") );
}

void C_PROGRESS_BAR::Display()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	DXENGINE()->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

	if( GetProgress() )
		pProgressBar->Display( C_DARK_POINT( Pos.GetX() + 2, Pos.GetY() + 2 ), static_cast<int>( ( static_cast<float>( GetWidth() ) - 4 ) / 100 * GetProgress() ), GetHeight() - 4 );

	DXENGINE()->GetFont()->DrawString( Pos.GetX() + GetWidth() / 2, Pos.GetY() + GetHeight() / 2, FT_CENTER|FT_VCENTER, pString->GetD3DCOLOR(), m_sBuf );
}

int C_PROGRESS_BAR::GetProgress()
{
	return m_iProgress;
}

void C_PROGRESS_BAR::SetProgress( int iProgress )
{
	if( iProgress > 100 )
		iProgress = 100;
	else if( iProgress < 0 )
		iProgress = 0;

	m_iProgress = iProgress;

	_tstringstream sStream;
	sStream << GetProgress() << static_cast<char>( 37 );
	m_sBuf = sStream.str();

	m_iStrWidth = DXENGINE()->GetFont()->GetStringWidth( m_sBuf.c_str() ) / 2;
	m_iStrHeight = DXENGINE()->GetFont()->GetStringHeight() / 2;
}

void C_PROGRESS_BAR::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	pInner = pState->GetColor( _T("Inner") );
	pBorder = pState->GetColor( _T("Border") );
	pString = pState->GetColor( _T("String") );

	pProgressBar = pState->GetTexture( _T("ProgressBar") );
}