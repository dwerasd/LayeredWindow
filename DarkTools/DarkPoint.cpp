#include "stdafx.h"
#include "DarkPoint.h"



C_DARK_POINT::C_DARK_POINT( C_DARK_POINT * pPos )
{
	m_iX = pPos->GetX();
	m_iY = pPos->GetY();
}

C_DARK_POINT::C_DARK_POINT( int iX, int iY )
{
	m_iX = iX;
	m_iY = iY;
}

C_DARK_POINT::C_DARK_POINT()
{
	m_iX = 0;
	m_iY = 0;
}

C_DARK_POINT::~C_DARK_POINT()
{
}

int C_DARK_POINT::GetX()
{
	return m_iX;
}

int C_DARK_POINT::GetY()
{
	return m_iY;
}

void C_DARK_POINT::SetX( int iX )
{
	m_iX = iX;
}

void C_DARK_POINT::SetY( int iY )
{
	m_iY = iY;
}

C_DARK_POINT C_DARK_POINT::operator + ( C_DARK_POINT otherPos )
{
	return C_DARK_POINT( GetX() + otherPos.GetX(), GetY() + otherPos.GetY() );
}

C_DARK_POINT C_DARK_POINT::operator - ( C_DARK_POINT otherPos )
{
	return C_DARK_POINT( GetX() - otherPos.GetX(), GetY() - otherPos.GetY() );
}