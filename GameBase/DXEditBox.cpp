#include "stdafx.h"
#include "DXEditBox.h"
#include "DXEngine.h"



C_DX_EDIT_BOX::C_DX_EDIT_BOX( C_TINY_XML_ELEMENT * pElement ) : bPassword(0)
{
	SetObject( pElement );
	SetHeight( WINDOW_CLOSE_BUTTON_HEIGHT );

	nStart = 0;
	SetIndex( 0 );
	bCursorState = FALSE;
	SetCallback( 0 );

	LPCTSTR pszString = pElement->Attribute( _T("password") );
	if( pszString )
	{
		bPassword = (!_tcscmp(pszString, _T("true")));
	}

	pszString = pElement->Attribute( _T("callback") );
	if( pszString )
	{
		C_DX_VARIANT * pCvar = DXENGINE()->Cvars[ pszString ];
	
		if(0 != pCvar)
		{
			SetCallback( pCvar->GetCallback() );
		}
		else
		{
			MessageBox( 0, _T("Cvar invalid"), GetString().c_str(), 0 );
		}
	}

	SetThemeElement( DXENGINE()->GetThemeElement( _T("EditBox") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("EditBox"), 0 );
	else
		SetObjectState( _T("Normal") );
}

void C_DX_EDIT_BOX::Display()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	_OBJECT_STATE * pState = GetElementState();
	if( pState )
	{
		DXENGINE()->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );
		
		DXENGINE()->GetFont()->DrawString( Pos.GetX() + 4, Pos.GetY() + GetHeight() / 2, FT_VCENTER, pString->GetD3DCOLOR(), &GetString()[ GetStart() ], GetWidth() );
		if( bCursorState && HasFocus() )
		{
			DXENGINE()->FillArea( Pos.GetX() + 2 + nCursorX, Pos.GetY() + 2, 2, GetHeight() - 4, pCursor->GetD3DCOLOR() );
		}
	}
}

void C_DX_EDIT_BOX::Calculate()
{
	if( !tCursorTimer.Running() )
	{
		bCursorState = !bCursorState;
		tCursorTimer.Start( 0.6f );
	}
}

void C_DX_EDIT_BOX::MouseMove(  )
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	SetMouseOver( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) );
}

void C_DX_EDIT_BOX::KeyEvent( _KEY sKey )
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	if( !sKey.cKey )
	{
		if( DXMOUSE()->GetLeftButton() )
		{
			if( GetMouseOver() )
			{
				int iX = DXMOUSE()->GetPos().GetX();
				int iAbsX = ( *pWindow->GetAbsolutePos() + *GetRelPos() ).GetX();

				_tstring sString( &GetString()[ GetStart() ] );
				
				if( iX >= iAbsX + DXENGINE()->GetFont()->GetStringWidth( sString.c_str() ) )
				{
					SetIndex( sString.length() );
				}
				else
				{
					for( int i = 0; i <= static_cast<int>( sString.length() ); i++ )
					{
						if( iX <= iAbsX + DXENGINE()->GetFont()->GetStringWidth( sString.c_str() ) )
						{
							sString[ i ] = 0;
							if( iX > iAbsX + DXENGINE()->GetFont()->GetStringWidth( sString.c_str() ) )
								SetIndex( i );
						}
						sString = &GetString()[ GetStart() ];
					}
				}

				pWindow->SetFocussedObject( this );
			}
		}
	}
	else if( sKey.bDown && this == pWindow->GetFocussedObject() && HasFocus() )
	{
		switch( sKey.cKey )
		{
		case VK_END:
			{
				_tstring sString = GetString();

				SetIndex( lstrlen( &sString[ GetStart() ] ) );

				while( DXENGINE()->GetFont()->GetStringWidth( &sString.c_str()[ GetStart() ] ) > GetWidth() - 5 || nCursorX > GetWidth() - 5 )
				{
					SetStart( GetStart() + 1 );
					SetIndex( GetIndex() - 1 );
				}
				break;
			}
		case VK_HOME:
			{
				SetStart( 0 );
				SetIndex( 0 );

				break;
			}
		case VK_BACK:
			{
				if( GetIndex() )
				{
					_tstring sString = GetString();

					sString.erase( GetStart() + GetIndex() - 1,  1 );

					SetString( sString );
					SetIndex( GetIndex() - 1 );
				}
				else if( GetStart() )
				{
					SetStart( GetStart() - 1 );
					SetIndex( 1 );
				}

				break;
			}
		case VK_DELETE:
			{
				_tstring sString = GetString();

				if( GetIndex() <= static_cast<int>( sString.length() ) )
					sString.erase( GetStart() + nIndex, 1 );

				SetString( const_cast<LPTSTR>( sString.c_str() ) );

				break;
			}
		case VK_LEFT:
			{
				if( !GetIndex() && GetStart() )
					SetStart( GetStart() - 1 );
				else if( GetIndex() )
					SetIndex( GetIndex() - 1 );

				break;
			}
		case VK_RIGHT:
			{
				SetIndex( GetIndex() + 1 );

				_tstring sString = GetString();
				sString[ GetIndex() ] = 0;

				while( DXENGINE()->GetFont()->GetStringWidth( &sString.c_str()[ GetStart() ] ) > GetWidth() - 5 || nCursorX > GetWidth() - 5 )
				{
					SetStart( GetStart() + 1 );
					SetIndex( GetIndex() - 1 );
				}

				break;
			}
		case VK_TAB:
		case VK_RETURN:
			{
				pWindow->SetFocussedObject( 0 );

				if(0 != GetCallback())
				{
					GetCallback()->Calculate(this, GetString().c_str());
				}
				break;
			}
		default:
			{
				_tstring sString = GetString();

				int iPrevLen = sString.length();

				BYTE bKeys[256] = { 0 };
				GetKeyboardState( bKeys );

				WORD wKey = 0;
				ToAscii( sKey.cKey, HIWORD( sKey.lParam )&0xFF, bKeys, &wKey, 0 );

				TCHAR szKey[2] = { static_cast<TCHAR>( wKey ), 0 };
				if( GetStart() + nIndex >= 0 && GetStart() + nIndex <= iPrevLen )
				{
					if( wKey != 22 )
					{
						sString.insert( GetStart() + nIndex, szKey );
					}
					else
					{
						if( !OpenClipboard( 0 ) )
						{
							break;
						}
						HANDLE hData = GetClipboardData( CF_TEXT );
						LPTSTR pszBuffer = static_cast<LPTSTR>( GlobalLock( hData ) );
						
						if( pszBuffer )
						{
							sString.insert( GetStart() + nIndex, pszBuffer );
						}

						GlobalUnlock( hData );
						CloseClipboard();
					}
				}

				SetString( const_cast<LPTSTR>( sString.c_str() ) );
				if( sKey.cKey == ' ' )
				{
					SetIndex( GetIndex() + 1 );
				}
				else
				{
					SetIndex( GetIndex() + sString.length() - iPrevLen );
				}

				while( DXENGINE()->GetFont()->GetStringWidth( &GetString().c_str()[ GetStart() ] ) > GetWidth() - 5 )
				{
					SetStart( GetStart() + 1 );
					SetIndex( GetIndex() - 1 );
				}

				break;
			}
		}
	}
}

int C_DX_EDIT_BOX::GetIndex()
{
	return nIndex;
}

void C_DX_EDIT_BOX::SetIndex( int iIndex )
{
	_tstring sString( &GetString()[ GetStart() ] );

	if( iIndex > static_cast<int>( sString.length() ) || iIndex < 0 )
	{
		return;
	}

	sString[ iIndex ] = 0;
	nCursorX = DXENGINE()->GetFont()->GetStringWidth( sString.c_str() );
	nIndex = iIndex;
}

int C_DX_EDIT_BOX::GetStart()
{
	return nStart;
}

void C_DX_EDIT_BOX::SetStart( int iStart )
{
	nStart = iStart;
}

void C_DX_EDIT_BOX::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	pInner = pState->GetColor( _T("Inner") );
	pBorder = pState->GetColor( _T("Border") );
	pString = pState->GetColor( _T("String") );
	pCursor = pState->GetColor( _T("Cursor") );
}