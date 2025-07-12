#include "stdafx.h"
#include "DarkColor.h"



C_DARK_COLOR::C_DARK_COLOR()
{
}

C_DARK_COLOR::C_DARK_COLOR( int iRed, int iGreen, int iBlue, int iAlpha )
{
	SetRed( iRed );
	SetGreen( iGreen );
	SetBlue( iBlue );
	SetAlpha( iAlpha );
}

C_DARK_COLOR::C_DARK_COLOR( DWORD d3dColor )
{
	SetD3DCOLOR( d3dColor );
}

C_DARK_COLOR::C_DARK_COLOR( C_TINY_XML_ELEMENT * pElement )
{
	int iColors[ 4 ] = { 0 };

	pElement->QueryIntAttribute( _T("r"), &iColors[ 0 ] );
	pElement->QueryIntAttribute( _T("g"), &iColors[ 1 ] );
	pElement->QueryIntAttribute( _T("b"), &iColors[ 2 ] );
	pElement->QueryIntAttribute( _T("a"), &iColors[ 3 ] );

	SetRed( iColors[ 0 ] );
	SetGreen( iColors[ 1 ] );
	SetBlue( iColors[ 2 ] );
	SetAlpha( iColors[ 3 ] );
}

C_DARK_COLOR::~C_DARK_COLOR()
{
}

C_DARK_COLOR C_DARK_COLOR::operator / ( int iDivisor )
{
	return C_DARK_COLOR( GetRed() / iDivisor, GetGreen() / iDivisor, GetBlue() / iDivisor, GetAlpha() );
}

C_DARK_COLOR C_DARK_COLOR::operator - ( C_DARK_COLOR cSubColor )
{
	return C_DARK_COLOR( GetRed() - cSubColor.GetRed(), GetGreen() - cSubColor.GetGreen(), GetBlue() - cSubColor.GetBlue(), GetAlpha() );
}

C_DARK_COLOR C_DARK_COLOR::operator + ( C_DARK_COLOR cAddColor )
{
	return C_DARK_COLOR( GetRed() + cAddColor.GetRed(), GetGreen() + cAddColor.GetGreen(), GetBlue() + cAddColor.GetBlue(), GetAlpha() );
}

C_DARK_COLOR C_DARK_COLOR::operator * ( int iMultiplicator )
{
	return C_DARK_COLOR( GetRed() * iMultiplicator, GetGreen() * iMultiplicator, GetBlue() * iMultiplicator, GetAlpha() );
}

void C_DARK_COLOR::SetD3DCOLOR( DWORD d3dColor )
{
	SetAlpha( d3dColor >> 24 );
	SetRed( ( d3dColor >> 16 )&0xFF );
	SetGreen( ( d3dColor >> 8 )&0xFF );
	SetBlue( d3dColor&0xFF ); 
}

void C_DARK_COLOR::SetRed( int iRed )
{
	m_iRed = iRed;
}

void C_DARK_COLOR::SetGreen( int iGreen )
{
	m_iGreen = iGreen;
}

void C_DARK_COLOR::SetBlue( int iBlue )
{
	m_iBlue = iBlue;
}

void C_DARK_COLOR::SetAlpha( int iAlpha )
{
	m_iAlpha = iAlpha;
}

DWORD C_DARK_COLOR::GetD3DCOLOR()
{
	return DARK_COLOR_RGBA( GetRed(), GetGreen(), GetBlue(), GetAlpha() );
}

int C_DARK_COLOR::GetRed()
{
	return m_iRed;
}

int C_DARK_COLOR::GetGreen()
{
	return m_iGreen;
}

int C_DARK_COLOR::GetBlue()
{
	return m_iBlue;
}

int C_DARK_COLOR::GetAlpha()
{
	return m_iAlpha;
}
