#include "stdafx.h"
#include "DXText.h"
#include "DXEngine.h"



C_DX_TEXT::C_DX_TEXT( C_TINY_XML_ELEMENT * pElement )
{
	SetObject( pElement );

	SetThemeElement( DXENGINE()->GetThemeElement( _T("Text") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("Text"), 0 );
	else
		SetObjectState( _T("Normal") );
}

void C_DX_TEXT::Display()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();
	
	//DXENGINE()->GetFont()->DrawString( Pos.GetX(), Pos.GetY(), 0, pString->GetD3DCOLOR(), DarkApi::TStr2WStr(GetFormatted()), GetWidth() );
	wchar_t wszBuffer[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, GetFormatted().c_str(), GetFormatted().length() + 1, wszBuffer, GetFormatted().length() + 1);

	DXENGINE()->GetFont()->DrawString( Pos.GetX(), Pos.GetY(), 0, pString->GetD3DCOLOR(), wszBuffer, GetWidth() );
}

void C_DX_TEXT::Calculate()
{
	GetString( TRUE );
}

void C_DX_TEXT::MouseMove(  )
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	SetObjectState( SetMouseOver( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY(), DXENGINE()->GetFont()->GetStringWidth( GetFormatted().c_str() ), DXENGINE()->GetFont()->GetStringHeight() ) )?_T("MouseOver"):_T("Normal") );
}

void C_DX_TEXT::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	pString = pState->GetColor( _T("String") );
}