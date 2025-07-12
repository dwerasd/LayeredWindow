#include "stdafx.h"
#include "DXFont.h"
#include "DXEngine.h"



C_DX_FONT::C_DX_FONT( IDirect3DDevice9 * pDevice, int iHeight, LPTSTR pszFaceName )
{
	HRESULT hResult = D3DXCreateFont( pDevice, -MulDiv( iHeight, GetDeviceCaps( GetDC( 0 ), LOGPIXELSY ), 72 ), 0, FW_NORMAL, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, pszFaceName, &pFont );
	if( FAILED( hResult ) )
	{
		//MessageBoxA( 0, DXGetErrorDescriptionA( hResult ), "D3DXCreateFontA failed", 0 );
	}
	pFont->PreloadCharacters( 0, 255 );
}

C_DX_FONT::~C_DX_FONT()
{
	SAFE_RELEASE( pFont );
}

void C_DX_FONT::OnLostDevice()
{
	pFont->OnLostDevice();
}

void C_DX_FONT::OnResetDevice()
{
	pFont->OnResetDevice();
}

void C_DX_FONT::DrawString( int iX, int iY, DWORD dwFlags, DWORD dwColor, std::string sString, int iWidth )
{
	if( iWidth )
		CutString( iWidth, sString );


	DXENGINE()->GetSprite()->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	D3DXMATRIX mat;
	D3DXMatrixTranslation( &mat, static_cast<float>( iX ), static_cast<float>( iY ), 0 );
	DXENGINE()->GetSprite()->SetTransform( &mat );

	RECT drawRect = { 0 };
	DWORD dwDrawFlags = DT_NOCLIP | ( ( dwFlags & FT_CENTER ) ? DT_CENTER : 0 ) | ( ( dwFlags & FT_VCENTER ) ? DT_VCENTER : 0 );
	pFont->DrawTextA( DXENGINE()->GetSprite(), sString.c_str(), -1, &drawRect, dwDrawFlags, dwColor );

	DXENGINE()->GetSprite()->End();
}

void C_DX_FONT::DrawString( int iX, int iY, DWORD dwFlags, DWORD dwColor, std::wstring sString, int iWidth )
{
	if( iWidth )
		CutString( iWidth, sString );


	RECT drawRect = { 0 };
	DWORD dwDrawFlags = DT_NOCLIP | ( ( dwFlags & FT_CENTER ) ? DT_CENTER : 0 ) | ( ( dwFlags & FT_VCENTER ) ? DT_VCENTER : 0 );
	D3DXMATRIX mat;

	DXENGINE()->GetSprite()->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	//////////////////////////////////////////////////////////////////////////
	// 그림자를 먼저 그리고

	D3DXMatrixTranslation( &mat, static_cast<float>( iX+1 ), static_cast<float>( iY+1 ), 0 );
	DXENGINE()->GetSprite()->SetTransform( &mat );
	pFont->DrawTextW( DXENGINE()->GetSprite(), sString.c_str(), -1, &drawRect, dwDrawFlags, D3DCOLOR_XRGB(8,8,8) );

	//////////////////////////////////////////////////////////////////////////
	// 글씨를 쓴다.

	D3DXMatrixTranslation( &mat, static_cast<float>( iX ), static_cast<float>( iY ), 0 );
	DXENGINE()->GetSprite()->SetTransform( &mat );
	pFont->DrawTextW( DXENGINE()->GetSprite(), sString.c_str(), -1, &drawRect, dwDrawFlags, dwColor );

	DXENGINE()->GetSprite()->End();
}


void C_DX_FONT::DrawStringEx( RECT rcDisplay, DWORD dwFlags, DWORD dwColor, LPSTR pszString, ... )
{
	char szBuffer[MAX_PATH];

	va_list va_alist;
	va_start(va_alist, pszString);
	_vsnprintf_s(szBuffer, sizeof(szBuffer) - 1, sizeof(szBuffer) - 1, pszString, va_alist);
	va_end(va_alist);

	pFont->DrawTextA( NULL, szBuffer, -1, &rcDisplay, DT_NOCLIP, dwColor );
}

void C_DX_FONT::SetColor( int iIndex, C_DARK_COLOR cColor )
{
	if( iIndex < 0 || iIndex > 8 )
		return;

	m_cColors[ iIndex ] = cColor;
}

C_DARK_COLOR & C_DX_FONT::GetColor( int iIndex )
{
	if( iIndex < 0 || iIndex > 8 )
		return m_cColors[ 0 ];

	return m_cColors[ iIndex ];
}

int C_DX_FONT::GetStringWidth( std::string sString )
{
	for( int i = 0; i <= static_cast<int>( sString.size() ); i++ )
	{
		if( sString[i] == ' ' )
		{
			sString[i] = '.';
		}
	}
	RECT rRect = { 0 };
	pFont->DrawTextA( 0, sString.c_str(), -1, &rRect, DT_CALCRECT, 0 );
	return rRect.right - rRect.left;
}

int C_DX_FONT::GetStringWidth( std::wstring sString )
{
	for( int i = 0; i <= static_cast<int>( sString.size() ); i++ )
	{
		if( sString[i] == ' ' )
		{
			sString[i] = '.';
		}
	}
	RECT rRect = { 0 };
	pFont->DrawTextW( 0, sString.c_str(), -1, &rRect, DT_CALCRECT, 0 );
	return rRect.right - rRect.left;
}

int C_DX_FONT::GetStringHeight()
{
	RECT rRect = { 0 };
	pFont->DrawText( 0, _T("Y"), -1, &rRect, DT_CALCRECT, 0 );

	return rRect.bottom - rRect.top;
}

void C_DX_FONT::CutString( int iMaxWidth, std::string & rString )
{
	int iIndex = 0, iLength = rString.length();

	for( int iWidth = 0; iIndex < iLength && iWidth + 10 < iMaxWidth; )
	{
		char szCurrent[ 2 ] = { rString.c_str()[ iIndex ], 0 };
		iWidth += GetStringWidth( szCurrent );
		iIndex++;
	}

	if( iIndex < iLength )
		rString[ iIndex - 1 ] = '\0';
}

void C_DX_FONT::CutString( int iMaxWidth, std::wstring & rString )
{
	int iIndex = 0, iLength = rString.length();

	for( int iWidth = 0; iIndex < iLength && iWidth + 10 < iMaxWidth; )
	{
		wchar_t szCurrent[ 2 ] = { rString.c_str()[ iIndex ], 0 };
		iWidth += GetStringWidth( szCurrent );
		iIndex++;
	}

	if( iIndex < iLength )
		rString[ iIndex - 1 ] = '\0';
}