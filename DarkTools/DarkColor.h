#pragma once


#include "DarkXML.h"


// maps unsigned 8 bits/channel to DARK_COLOR
#define DARK_COLOR_ARGB(a,r,g,b) \
	((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define DARK_COLOR_RGBA(r,g,b,a) DARK_COLOR_ARGB(a,r,g,b)
#define DARK_COLOR_XRGB(r,g,b)   DARK_COLOR_ARGB(0xff,r,g,b)

#define DARK_COLOR_XYUV(y,u,v)   DARK_COLOR_ARGB(0xff,y,u,v)
#define DARK_COLOR_AYUV(a,y,u,v) DARK_COLOR_ARGB(a,y,u,v)

// maps floating point channels (0.f to 1.f range) to DARK_COLOR
#define DARK_COLOR_COLORVALUE(r,g,b,a) \
	DARK_COLOR_RGBA((DWORD)((r)*255.f),(DWORD)((g)*255.f),(DWORD)((b)*255.f),(DWORD)((a)*255.f))


class C_DARK_COLOR
{
private:
	int m_iRed, m_iGreen, m_iBlue, m_iAlpha;

public:
	C_DARK_COLOR();
	C_DARK_COLOR( int iRed, int iGreen, int iBlue, int iAlpha );
	C_DARK_COLOR( DWORD d3dColor );
	C_DARK_COLOR( C_TINY_XML_ELEMENT * pElement );
	~C_DARK_COLOR();

	void SetD3DCOLOR( DWORD d3dColor );
	void SetRed( int iRed );
	void SetGreen( int iGreen );
	void SetBlue( int iBlue );
	void SetAlpha( int iAlpha );

	DWORD GetD3DCOLOR();
	int GetRed();
	int GetGreen();
	int GetBlue();
	int GetAlpha();

	C_DARK_COLOR operator / ( int iDivisor );
	C_DARK_COLOR operator * ( int iMultiplicator );

	C_DARK_COLOR operator - ( C_DARK_COLOR cSubColor );
	C_DARK_COLOR operator + ( C_DARK_COLOR cAddColor );
};

