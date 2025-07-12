#include "stdafx.h"
#include "DXObjectBase.h"
#include "DXEngine.h"




void C_DX_OBJECT_BASE::SetObject( C_TINY_XML_ELEMENT * pElement, long width, long height)
{
	//////////////////////////////////////////////////////////////////////////
	// 오브젝트 좌표를 얻어온다.

	int iTempX = 0, iTempY = 0;
	if( TINY_XML_NO_ATTRIBUTE == pElement->QueryIntAttribute( _T("relX"), &iTempX ) )
	{
		iTempX = 0;
	}
	if( TINY_XML_NO_ATTRIBUTE == pElement->QueryIntAttribute( _T("relY"), &iTempY ) )
	{
		iTempY = 0;
	}

	SetRelPos( C_DARK_POINT( iTempX, iTempY ) );

	if( TINY_XML_NO_ATTRIBUTE == pElement->QueryIntAttribute( _T("absX"), &iTempX ) )
	{
		iTempX = 0;
	}
	if( TINY_XML_NO_ATTRIBUTE == pElement->QueryIntAttribute( _T("absY"), &iTempY ) )
	{
		iTempY = 0;
	}

	C_DARK_POINT Pos(DXENGINE()->GetMainTitle()->left,  DXENGINE()->GetMainTitle()->bottom);
	SetAbsolutePos( C_DARK_POINT( iTempX, iTempY ) + Pos);
	//SetAbsolutePos( C_DARK_POINT( iTempX, iTempY ));

	if( TINY_XML_NO_ATTRIBUTE == pElement->QueryIntAttribute( _T("width"), &iTempX ) )
	{
		iTempX = 0;
	}
	if( TINY_XML_NO_ATTRIBUTE == pElement->QueryIntAttribute( _T("height"), &iTempY ) )
	{
		iTempY = 0;
	}
	
	SetWidth( iTempX );
	SetHeight( iTempY );

	LPCTSTR pszString = pElement->Attribute( _T("name") );
	if( pszString )
	{
		SetObjectName(pszString);
	}
	pszString = pElement->Attribute( _T("visible") );
	if( pszString )
	{
		SetVisible( !_tcscmp(pszString, _T("true")) );
	}
	pszString = pElement->Attribute( _T("tabstop") );
	if( pszString )
	{
		SetTabStop( !_tcscmp(pszString, _T("true")) );
	}
	pszString = pElement->Attribute( _T("string") );
	if( pszString )
	{
		SetString( pszString );
	}
	pszString = pElement->Attribute( _T("string2") );
	if( pszString )
	{
		SetString( pszString, 1 );
	}
	LPCTSTR pszCallback = pElement->Attribute( _T("callback") );
	if( pszCallback )
	{
		C_DX_VARIANT * pCvar = DXENGINE()->Cvars[ pszCallback ];
	
		if( pCvar )
		{
			SetCallback( pCvar->GetCallback() );
		}
		else
		{
			SetCallback( 0 );
			MessageBox( 0, _T("Cvar invalid"), pszCallback, 0 );
		}
	}
	else
	{
		SetCallback( 0 );
	}

	SetMouseOver( FALSE );
}

void C_DX_OBJECT_BASE::SetParent( LPVOID p )
{
	pParent = p;
}

LPVOID C_DX_OBJECT_BASE::GetParent()
{
	return pParent;
}

void C_DX_OBJECT_BASE::SetCallback(C_DX_CALLBACK_BASE *p)
{
	pCallback = p;
}

C_DX_CALLBACK_BASE *C_DX_OBJECT_BASE::GetCallback()
{
	return pCallback;
}

void C_DX_OBJECT_BASE::SetRelPos( C_DARK_POINT relPos )
{
	PosRelease = relPos;
}

C_DARK_POINT * C_DX_OBJECT_BASE::GetRelPos()
{
	return &PosRelease;
}

void C_DX_OBJECT_BASE::SetAbsolutePos( C_DARK_POINT absPos )
{
	PosAbsolute = absPos;
}

C_DARK_POINT * C_DX_OBJECT_BASE::GetAbsolutePos()
{
	return &PosAbsolute;
}

void C_DX_OBJECT_BASE::SetWidth( int iWidth )
{
	m_iWidth = iWidth;
}

int C_DX_OBJECT_BASE::GetWidth()
{
	return m_iWidth;
}

void C_DX_OBJECT_BASE::SetHeight( int iHeight )
{
	m_iHeight = iHeight;
}

int C_DX_OBJECT_BASE::GetHeight()
{
	return m_iHeight;
}

BOOL C_DX_OBJECT_BASE::HasFocus()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	return pWindow->GetFocussedObject() == this;
}

void C_DX_OBJECT_BASE::SetString( _tstring sString, int iIndex )
{
	if( static_cast<int>( sString.length() ) > 255 )
		return;

	tstrSource[ iIndex ] = sString;
}

_tstring C_DX_OBJECT_BASE::GetString( BOOL bReplaceVars, int iIndex )
{
	tstrDisplay[ iIndex ] = tstrSource[ iIndex ];

	if( bReplaceVars && tstrSource[ iIndex ].find( _T("$") ) != _tstring::npos )
	{
		for( std::map<_tstring,C_DX_VARIANT*>::reverse_iterator iIter = DXENGINE()->Cvars.rbegin(); iIter != DXENGINE()->Cvars.rend(); iIter++ )
		{
			int iPos = tstrDisplay[ iIndex ].find( iIter->first );
			if( iPos != static_cast<int>( _tstring::npos ) )
			{
				int iLen = iIter->first.length();
				if( static_cast<int>( tstrDisplay[ iIndex ].length() ) - iPos >= iLen )
				{
					BOOL bReplace = TRUE;

					for( int i = 0; i < iLen; i++ )
					{
						if( iIter->first[ i ] != tstrDisplay[ iIndex ][ iPos + i ] )
						{
							bReplace = FALSE;
						}
					}
					if( bReplace && iIter->second )
					{
						if( iIter->second->GetString().length() )
						{
							tstrDisplay[ iIndex ].replace( iPos, iIter->first.length(), iIter->second->GetString() );
						}
						else
						{
							C_DX_CALLBACK_BASE *p = iIter->second->GetCallback();
							if (0 != p)
							{
								tstrDisplay[ iIndex ].replace( iPos, iIter->first.length(), p->Calculate(this) );
							}
						}
					}
				}
			}
		}
		return tstrDisplay[ iIndex ];
	}
	return tstrSource[ iIndex ];
}

_tstring C_DX_OBJECT_BASE::GetFormatted( int iIndex )
{
	return tstrDisplay[ iIndex ];
}

BOOL C_DX_OBJECT_BASE::GetMouseOver()
{
	return bMouseOver;
}

BOOL C_DX_OBJECT_BASE::SetMouseOver( BOOL b )
{
	return bMouseOver = b;
}

_OBJECT_BASE * C_DX_OBJECT_BASE::SetThemeElement( _OBJECT_BASE * pThemeElement, int iIndex )
{
	return m_pThemeElement[ iIndex ] = pThemeElement;
}

_OBJECT_BASE * C_DX_OBJECT_BASE::GetThemeElement( int iIndex )
{
	return m_pThemeElement[ iIndex ];
}

void C_DX_OBJECT_BASE::SetObjectState( _tstring sState, int iIndex )
{
	m_pElementState[ iIndex ] = GetThemeElement( iIndex )->m_mStates[ sState ];

	if( !m_pElementState )
		m_pElementState[ iIndex ] = GetThemeElement( iIndex )->m_mStates[ GetThemeElement( iIndex )->tstrDefaultState ];

	UpdateObject( iIndex );
}

_OBJECT_STATE * C_DX_OBJECT_BASE::GetElementState( int iIndex )
{
	return m_pElementState[ iIndex ];
}

void C_DX_OBJECT_BASE::UpdateObject( int )
{
}

void C_DX_OBJECT_BASE::Display()
{
}

void C_DX_OBJECT_BASE::Calculate()
{
}

void C_DX_OBJECT_BASE::MouseMove( )
{
}

void C_DX_OBJECT_BASE::KeyEvent( _KEY )
{
}
