#include "stdafx.h"
#include "DXObjectBase.h"
#include "DXEngine.h"




void C_DX9_OBJECT::SetObject( C_TINY_XML_ELEMENT * pElement, long width, long height)
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

	SetRelativePos( dk::DPOINT( iTempX, iTempY ) );

	if( TINY_XML_NO_ATTRIBUTE == pElement->QueryIntAttribute( _T("absX"), &iTempX ) )
	{
		iTempX = 0;
	}
	if( TINY_XML_NO_ATTRIBUTE == pElement->QueryIntAttribute( _T("absY"), &iTempY ) )
	{
		iTempY = 0;
	}

	dk::DPOINT Pos(DXENGINE()->GetMainTitle()->left,  DXENGINE()->GetMainTitle()->bottom);
	SetAbsolutePos( dk::DPOINT( iTempX, iTempY ) + Pos);
	//SetAbsolutePos( dk::DPOINT( iTempX, iTempY ));

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
		C_DX9_VARIANT * pCvar = DXENGINE()->Cvars[ pszCallback ];
	
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

void C_DX9_OBJECT::SetParent( LPVOID p )
{
	pParent = p;
}

LPVOID C_DX9_OBJECT::GetParent()
{
	return pParent;
}

void C_DX9_OBJECT::SetCallback(C_DX9_CALLBACK_BASE *p)
{
	pCallback = p;
}

C_DX9_CALLBACK_BASE *C_DX9_OBJECT::GetCallback()
{
	return pCallback;
}

void C_DX9_OBJECT::SetRelativePos( dk::DPOINT relPos )
{
	ptRelative = relPos;
}

dk::DPOINT * C_DX9_OBJECT::GetRelPos()
{
	return &ptRelative;
}

void C_DX9_OBJECT::SetAbsolutePos( dk::DPOINT absPos )
{
	ptAbsolute = absPos;
}

dk::DPOINT * C_DX9_OBJECT::GetAbsolutePos()
{
	return &ptAbsolute;
}

void C_DX9_OBJECT::SetWidth( int iWidth )
{
	m_iWidth = iWidth;
}

int C_DX9_OBJECT::GetWidth()
{
	return m_iWidth;
}

void C_DX9_OBJECT::SetHeight( int iHeight )
{
	m_iHeight = iHeight;
}

int C_DX9_OBJECT::GetHeight()
{
	return m_iHeight;
}

BOOL C_DX9_OBJECT::HasFocus()
{
	C_DX9_WINDOW *pWindow = (C_DX9_WINDOW *)GetParent();
	return pWindow->GetFocussedObject() == this;
}

void C_DX9_OBJECT::SetString( std::_tstring sString, int iIndex )
{
	if( static_cast<int>( sString.length() ) > 255 )
		return;

	tstrSource[ iIndex ] = sString;
}

std::_tstring C_DX9_OBJECT::GetString( BOOL bReplaceVars, int iIndex )
{
	tstrDisplay[ iIndex ] = tstrSource[ iIndex ];

	if( bReplaceVars && tstrSource[ iIndex ].find( _T("$") ) != std::_tstring::npos )
	{
		for( std::map<std::_tstring,C_DX9_VARIANT*>::reverse_iterator iIter = DXENGINE()->Cvars.rbegin(); iIter != DXENGINE()->Cvars.rend(); iIter++ )
		{
			int iPos = tstrDisplay[ iIndex ].find( iIter->first );
			if( iPos != static_cast<int>( std::_tstring::npos ) )
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
							C_DX9_CALLBACK_BASE *p = iIter->second->GetCallback();
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

std::_tstring C_DX9_OBJECT::GetFormatted( int iIndex )
{
	return tstrDisplay[ iIndex ];
}

BOOL C_DX9_OBJECT::GetMouseOver()
{
	return bMouseOver;
}

BOOL C_DX9_OBJECT::SetMouseOver( BOOL b )
{
	return bMouseOver = b;
}

_OBJECT_BASE * C_DX9_OBJECT::SetThemeElement( _OBJECT_BASE * pThemeElement, int iIndex )
{
	return m_pThemeElement[ iIndex ] = pThemeElement;
}

_OBJECT_BASE * C_DX9_OBJECT::GetThemeElement( int iIndex )
{
	return m_pThemeElement[ iIndex ];
}

void C_DX9_OBJECT::SetObjectState( std::_tstring sState, int iIndex )
{
	m_pElementState[ iIndex ] = GetThemeElement( iIndex )->m_mStates[ sState ];

	if( !m_pElementState )
		m_pElementState[ iIndex ] = GetThemeElement( iIndex )->m_mStates[ GetThemeElement( iIndex )->tstrDefaultState ];

	UpdateObject( iIndex );
}

_OBJECT_STATE * C_DX9_OBJECT::GetElementState( int iIndex )
{
	return m_pElementState[ iIndex ];
}

void C_DX9_OBJECT::UpdateObject( int )
{
}

void C_DX9_OBJECT::Display()
{
}

void C_DX9_OBJECT::Calculate()
{
}

void C_DX9_OBJECT::MouseMove( )
{
}

void C_DX9_OBJECT::KeyEvent( _KEY )
{
}
