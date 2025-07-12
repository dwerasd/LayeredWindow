#pragma once

//class C_DX_VARIANT;


#include "DXDef.h"
#include "DXObjectBase.h"



class C_DX_VARIANT
{
private:
	double m_dValue;
	_tstring m_sString;

	//typedef _tstring ( __cdecl * tAction )( LPCTSTR pszArgs, C_DX_OBJECT_BASE * pElement );
	//tAction m_pAction;

	C_DX_CALLBACK_BASE *pCallback;

public:
	C_DX_VARIANT( double dValue );
	C_DX_VARIANT( float fValue );
	C_DX_VARIANT( int iValue );
	C_DX_VARIANT( _tstring sString );
	C_DX_VARIANT( C_DX_CALLBACK_BASE *p );

	void SetDouble( double dValue );
	void SetFloat( float fValue );
	void SetInteger( int iValue );
	void SetBoolean( BOOL bValue );
	void SetString( _tstring sString );

	void SetCallback( C_DX_CALLBACK_BASE *p );
	C_DX_CALLBACK_BASE *GetCallback();

	double GetDouble();
	float GetFloat();
	int GetInteger();
	BOOL GetBoolean();
	_tstring GetString();
	//_tstring GetActionString( LPCTSTR pszString, C_DX_OBJECT_BASE * pElement = 0 );
	
};

class CCommand
{
public:
};