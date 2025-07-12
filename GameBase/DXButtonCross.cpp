#include "stdafx.h"
#include "DXButtonCross.h"
#include "DXEngine.h"



C_DX_CROSS_BUTTON::C_DX_CROSS_BUTTON(long width, long height ) : bPressed(FALSE)
{
	SetWidth(width);
	SetHeight(height);

	SetThemeElement( DXENGINE()->GetThemeElement( _T("CrossButton") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("CrossButton"), 0 );
	else
		SetObjectState( _T("Normal") );
}

C_DX_CROSS_BUTTON::C_DX_CROSS_BUTTON( C_TINY_XML_ELEMENT *pElement, long width, long height ) : bPressed(FALSE)
{
	SetObject(pElement);
	if ( 0 == GetWidth() )
	{
		SetWidth(width);
	}
	if ( 0 == GetHeight() )
	{
		SetHeight(height);
	}

	SetThemeElement( DXENGINE()->GetThemeElement( _T("CrossButton") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("CrossButton"), 0 );
	else
		SetObjectState( _T("Normal") );
}

void C_DX_CROSS_BUTTON::Calculate()
{
	if(!bPressed)
	{
		C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
		C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

		SetObjectState( GetMouseOver()?_T("MouseOver"):_T("Normal") );
	}
}

void C_DX_CROSS_BUTTON::Display()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	pTexture->Display( Pos, GetWidth(), GetHeight() );
}

void C_DX_CROSS_BUTTON::MouseMove()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();
	if( DXMOUSE()->GetLeftButton() )
	{
		SetObjectState( SetMouseOver( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) )?_T("Pressed"):_T("Normal") );
	}
	else
	{
		SetObjectState( SetMouseOver( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) )?_T("MouseOver"):_T("Normal") );
	}
}

void C_DX_CROSS_BUTTON::KeyEvent( _KEY sKey )
{
	if( !sKey.cKey )
	{
		C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
		C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

		if( GetMouseOver() && DXMOUSE()->GetLeftButton() )
		{
			bPressed = TRUE;
			SetObjectState( _T("Pressed") );
		}
		else if (SetMouseOver(DXMOUSE()->InArea(Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight())) && FALSE != bPressed)
		{
			if(0 != GetCallback())
			{
				GetCallback()->Calculate(this);
			}
			bPressed = FALSE;
			pWindow->SetVisible(FALSE);
		}
		else
		{
			bPressed = FALSE;
		}
	}
}

void C_DX_CROSS_BUTTON::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	pTexture = pState->GetTexture( _T("CrossButton") );
}