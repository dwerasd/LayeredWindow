#pragma once

class C_DARK_POINT
{
	int m_iX, m_iY;

public:

	C_DARK_POINT( C_DARK_POINT * pPos );
	C_DARK_POINT( int iX, int iY );
	C_DARK_POINT();

	~C_DARK_POINT();

	int GetX();
	int GetY();

	void SetX( int iX );
	void SetY( int iY );

	C_DARK_POINT operator + ( C_DARK_POINT otherPos );
	C_DARK_POINT operator - ( C_DARK_POINT otherPos );
};