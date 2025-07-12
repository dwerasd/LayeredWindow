#pragma once


#include "DXDef.h"



class C_DX_FONT
{
private:
	C_DARK_COLOR m_cColors[9];
	LPD3DXFONT pFont;

public:
	C_DX_FONT( IDirect3DDevice9 * pDevice, int iHeight, LPTSTR pszFaceName );
	~C_DX_FONT();

	void OnLostDevice();
	void OnResetDevice();

	void DrawString( int iX, int iY, DWORD dwFlags, DWORD dwColor, std::string sString, int iWidth = 0 );
	void DrawString( int iX, int iY, DWORD dwFlags, DWORD dwColor, std::wstring sString, int iWidth = 0 );
	void DrawStringEx( RECT rcDisplay, DWORD dwFlags, DWORD dwColor, LPSTR pszString, ... );

	int GetStringWidth( std::string pszString );
	int GetStringWidth( std::wstring pszString );
	int GetStringHeight();

	void SetColor( int iIndex, C_DARK_COLOR cColor );
	C_DARK_COLOR & GetColor( int iIndex );

	void CutString( int iWidth, std::string & rString );
	void CutString( int iWidth, std::wstring & rString );
};