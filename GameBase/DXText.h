#pragma once


#include "DXDef.h"
#include "DXObjectBase.h"



class C_DX9_TEXT : public C_DX9_OBJECT
{
private:
	dk::DCOLOR * pString;

public:
	C_DX9_TEXT( C_TINY_XML_ELEMENT * pElement );

	void Display();
	void Calculate();
	void MouseMove(  );

	void UpdateObject( int iIndex );
};