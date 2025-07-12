#pragma once


#include "DXDef.h"
#include "DXObjectBase.h"



class C_DX_TEXT : public C_DX_OBJECT_BASE
{
private:
	C_DARK_COLOR * pString;

public:
	C_DX_TEXT( C_TINY_XML_ELEMENT * pElement );

	void Display();
	void Calculate();
	void MouseMove(  );

	void UpdateObject( int iIndex );
};