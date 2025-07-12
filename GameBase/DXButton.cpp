#include "stdafx.h"
#include "DXButton.h"
#include "DXEngine.h"



C_DX_BUTTON::C_DX_BUTTON( C_TINY_XML_ELEMENT *pElement, long width, long height ) : bPressed(FALSE)
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
	SetThemeElement( DXENGINE()->GetThemeElement( _T("Button") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("Button"), 0 );
	else
		SetObjectState( _T("Normal") );
}

void C_DX_BUTTON::Calculate()
{
	if(!bPressed)
	{
		C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
		C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

		SetObjectState( GetMouseOver()?_T("MouseOver"):_T("Normal") );
	}
}

void C_DX_BUTTON::Display()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	pTexture->Display( Pos, GetWidth(), GetHeight() );

	int nSize = GetString().length();
	if (0 < nSize)
	{
		wchar_t wszBuffer[MAX_PATH];
		MultiByteToWideChar(CP_UTF8, 0, GetString().c_str(), nSize + 1, wszBuffer, nSize + 1);

		DXENGINE()->GetFont()->DrawString( Pos.GetX() + GetWidth() / 2, Pos.GetY() + GetHeight() / 2, FT_CENTER|FT_VCENTER, pString->GetD3DCOLOR(), wszBuffer );
	}
}

void C_DX_BUTTON::MouseMove()
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

void C_DX_BUTTON::KeyEvent( _KEY sKey )
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	if( !sKey.cKey )
	{
		C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

		if( GetMouseOver() && DXMOUSE()->GetLeftButton() )
		{
			bPressed = TRUE;

			SetObjectState( _T("Pressed") );
			//tPressed.Start( 0.1f );
		}
		else if (GetMouseOver() && FALSE != bPressed)
		{
			if( GetCallback() )
			{
				GetCallback()->Calculate(this);
			}
			bPressed = FALSE;
		}
		else
		{
			bPressed = FALSE;
		}
	}
	else if( sKey.bDown && this == pWindow->GetFocussedObject() && HasFocus() )
	{
		switch( sKey.cKey )
		{
		case VK_RETURN:
			{
				pWindow->SetFocussedObject( 0 );

				if(0 != GetCallback())
				{
					GetCallback()->Calculate(this, GetString().c_str());
				}
				break;
			}
		case VK_ESCAPE:
			{
				pWindow->SetVisible(FALSE);
			}
		}
	}
}

void C_DX_BUTTON::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	pTexture = pState->GetTexture( _T("Button") );
	pString = pState->GetColor( _T("String") );
}