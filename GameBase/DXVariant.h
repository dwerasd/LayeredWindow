#pragma once

//class C_DX9_VARIANT;


#include "DXDef.h"
#include "DXObjectBase.h"



class C_DX9_VARIANT
{
private:
	double m_dValue;
	std::_tstring m_sString;

	//typedef std::_tstring ( __cdecl * tAction )( LPCTSTR pszArgs, C_DX9_OBJECT * pElement );
	//tAction m_pAction;

	C_DX9_CALLBACK_BASE *pCallback;

public:
	C_DX9_VARIANT( double dValue );
	C_DX9_VARIANT( float fValue );
	C_DX9_VARIANT( int iValue );
	C_DX9_VARIANT( std::_tstring sString );
	C_DX9_VARIANT( C_DX9_CALLBACK_BASE *p );

	void SetDouble( double dValue );
	void SetFloat( float fValue );
	void SetInteger( int iValue );
	void SetBoolean( BOOL bValue );
	void SetString( std::_tstring sString );

	void SetCallback( C_DX9_CALLBACK_BASE *p );
	C_DX9_CALLBACK_BASE *GetCallback();

	double GetDouble();
	float GetFloat();
	int GetInteger();
	BOOL GetBoolean();
	std::_tstring GetString();
	//std::_tstring GetActionString( LPCTSTR pszString, C_DX9_OBJECT * pElement = 0 );
	
};

class CCommand
{
public:
};