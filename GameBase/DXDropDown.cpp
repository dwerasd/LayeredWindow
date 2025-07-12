#include "stdafx.h"
#include "DXDropDown.h"
#include "DXEngine.h"




C_DX_DROP_DOWN::C_DX_DROP_DOWN( C_TINY_XML_ELEMENT * pElement )
{
	SetObject( pElement );
	SetHeight( WINDOW_CLOSE_BUTTON_HEIGHT );

	m_bDropped = FALSE;
	m_iSelected = m_iMouseOverIndex = 0;

	for( C_TINY_XML_ELEMENT * pEntryElement = pElement->FirstChildElement( _T("Entry") ); pEntryElement; pEntryElement = pEntryElement->NextSiblingElement( _T("Entry") ) )
		AddObject( pEntryElement->Attribute( _T("string") ), pEntryElement->Attribute( _T("value") ) );

	SetThemeElement( DXENGINE()->GetThemeElement( _T("DropDown") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("DropDown"), 0 );
	else
		SetObjectState( _T("Normal") );
}

void C_DX_DROP_DOWN::Display()
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos();

	_OBJECT_STATE * pState = GetElementState();

	if( pState )
	{
		DXENGINE()->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );
		DXENGINE()->DrawLine( Pos.GetX() + GetWidth() - 20, Pos.GetY() + 1, Pos.GetX() + GetWidth() - 20, Pos.GetY() + GetHeight() - 1, 1, pBorder->GetD3DCOLOR() );
		
		int nSize = m_vEntrys[ m_iSelected ].m_sString.length();
		if (0 < nSize)
		{
			wchar_t wszBuffer[MAX_PATH];
			MultiByteToWideChar(CP_UTF8, 0, m_vEntrys[m_iSelected].m_sString.c_str(), nSize + 1, wszBuffer, nSize + 1);

			DXENGINE()->GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + GetHeight() / 2, FT_VCENTER, pString->GetD3DCOLOR(), wszBuffer );
		}
		
		if( m_bDropped && m_vEntrys.size() )
		{
			DXENGINE()->DrawOutlinedBox( Pos.GetX(), Pos.GetY() + GetHeight(), GetWidth(), GetHeight() * m_vEntrys.size(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

			for( int iIndex = 0; iIndex < static_cast<int>( m_vEntrys.size() ); iIndex++ )
			{
				wchar_t wszBuffer[MAX_PATH] = { 0 };
				DWORD dwColor = 0;
				int nSize = m_vEntrys[ iIndex ].m_sString.length();
				if (0 < nSize)
				{
					MultiByteToWideChar(CP_UTF8, 0, m_vEntrys[iIndex].m_sString.c_str(), nSize + 1, wszBuffer, nSize + 1);
				}
				if( iIndex == m_iMouseOverIndex )
				{
					dwColor = pSelectedString->GetD3DCOLOR();
					DXENGINE()->FillArea( Pos.GetX() + 1, Pos.GetY() + GetHeight() * ( iIndex + 1 ), GetWidth() - 2, GetHeight(),pSelectedInner->GetD3DCOLOR() );
				}
				else
				{
					dwColor = pString->GetD3DCOLOR();
					
				}
				DXENGINE()->GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + GetHeight() * ( iIndex + 1 ) + GetHeight() / 2, FT_VCENTER, pString->GetD3DCOLOR(), wszBuffer );
			}
		}

		pButton->Display( C_DARK_POINT( Pos.GetX() + GetWidth() - 19, Pos.GetY() + 1 ), 18, 18 );
	}
}

void C_DX_DROP_DOWN::MouseMove(  )
{
	C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
	C_DARK_POINT Pos = *pWindow->GetAbsolutePos() + *GetRelPos(), mPos = DXMOUSE()->GetPos();

	int iHeight = 0;
	if( m_bDropped )
	{
		iHeight = GetHeight() * ( m_vEntrys.size() + 1 );

		if( mPos.GetX() == -1 && mPos.GetY() == -1 )
			DXMOUSE()->LoadPos();
	}
	else
		iHeight = GetHeight();

	SetObjectState( SetMouseOver( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), iHeight ) )?_T("MouseOver"):_T("Normal") );

	if( GetMouseOver() )
		for( int iIndex = 0; iIndex < static_cast<int>( m_vEntrys.size() ); iIndex++ )
			if( DXMOUSE()->InArea( Pos.GetX(), Pos.GetY() + GetHeight() * ( iIndex + 1 ), GetWidth(), GetHeight() ) )
			{
				m_iMouseOverIndex = iIndex;
				break;
			}

	DXMOUSE()->SetPos( mPos );
}

void C_DX_DROP_DOWN::KeyEvent( _KEY sKey )
{
	if( !sKey.cKey )
	{
		if( DXMOUSE()->GetLeftButton() )
		{
			if( GetMouseOver() )
			{
				if( m_bDropped )
				{	
					m_iSelected = m_iMouseOverIndex;
					if( GetCallback() )
					{
						GetCallback()->Calculate(this, m_vEntrys[ m_iSelected ].m_sValue.c_str());
					}

					m_bDropped = FALSE;
					SetObjectState( _T("Normal") );
				}
				else
				{
					C_DX_WINDOW *pWindow = (C_DX_WINDOW *)GetParent();
					if (pWindow != DXENGINE()->GetMainWindow())
					{
						pWindow->BringToTop( this );
					}

					m_bDropped = TRUE;
					SetObjectState( _T("Pressed") );
				}

				DXMOUSE()->SetLeftButton( 0 );
			}
			else
				m_bDropped = FALSE;
		}
	}
}

void C_DX_DROP_DOWN::AddObject( _tstring sElem, _tstring sValue )
{
	m_vEntrys.push_back( SEntry( sElem, sValue ) );
}

_tstring C_DX_DROP_DOWN::GetValue()
{
	return m_vEntrys[ m_iSelected ].m_sValue;
}

void C_DX_DROP_DOWN::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	pString = pState->GetColor( _T("String") );
	pInner = pState->GetColor( _T("Inner") );
	pBorder = pState->GetColor( _T("Border") );
	pSelectedInner = pState->GetColor( _T("SelectedInner") );
	pSelectedString = pState->GetColor( _T("SelectedString") );

	pButton = pState->GetTexture( _T("Button") );
}