#include "stdafx.h"
#include "DXVariant.h"


C_DX9_VARIANT::C_DX9_VARIANT( double dValue ) : pCallback(0)
{
	SetDouble( dValue );
	SetString( _T("") );
}

C_DX9_VARIANT::C_DX9_VARIANT( float fValue ) : pCallback(0)
{
	SetFloat( fValue );
	SetString( _T("") );
}

C_DX9_VARIANT::C_DX9_VARIANT( int iValue ) : pCallback(0)
{
	SetInteger( iValue );
	SetString( _T("") );
}

C_DX9_VARIANT::C_DX9_VARIANT( std::_tstring sString ) : pCallback(0)
{
	SetString( sString );
	SetDouble( 0.0 );
}

C_DX9_VARIANT::C_DX9_VARIANT(C_DX9_CALLBACK_BASE *p) : pCallback(p)
{
	SetDouble( 0.0 );
	SetString( _T("") );
}

void C_DX9_VARIANT::SetDouble( double dValue )
{
	m_dValue = dValue;
}

void C_DX9_VARIANT::SetFloat( float fValue )
{
	m_dValue = static_cast<double>( fValue );
}

void C_DX9_VARIANT::SetInteger( int iValue )
{
	m_dValue = static_cast<double>( iValue );
}

void C_DX9_VARIANT::SetBoolean( BOOL bValue )
{
	if( bValue )
		m_dValue = 1.0;
	else
		m_dValue = 0.0;
}

void C_DX9_VARIANT::SetString( std::_tstring sString )
{
	m_sString = sString;
}

void C_DX9_VARIANT::SetCallback(C_DX9_CALLBACK_BASE *p)
{
	pCallback = p;
}

double C_DX9_VARIANT::GetDouble()
{
	return m_dValue;
}

float C_DX9_VARIANT::GetFloat()
{
	return static_cast<float>( m_dValue );
}

int C_DX9_VARIANT::GetInteger()
{
	return static_cast<int>( m_dValue );
}

BOOL C_DX9_VARIANT::GetBoolean()
{
	return ( m_dValue != 0.0 );
}

std::_tstring C_DX9_VARIANT::GetString()
{
	return m_sString;
}
/*
std::_tstring C_DX9_VARIANT::GetActionString( LPCTSTR pszString, C_DX9_OBJECT * pElement )
{
	if(0 != pCallback)
	{
		return pCallback(pElement, pszString);
	}
	return std::_tstring();
}
*/
C_DX9_CALLBACK_BASE *C_DX9_VARIANT::GetCallback()
{
	return pCallback;
}