#pragma once


#include "DXDef.h"
#include "DXTexture.h"
#include "DXObjectBase.h"



class C_DX9_BUTTON : public C_DX9_OBJECT
{
private:
	BOOL bPressed;

	dk::C_TIMER tPressed;
	dk::DCOLOR *pString;
	C_DX9_TEXTURE *pTexture;

	//C_DX9_CALLBACK_BASE *pCallback;

public:
	C_DX9_BUTTON(C_TINY_XML_ELEMENT *pElement, long width = 20, long height = 20 );

	void Calculate();
	void Display();
	
	//void SetCallback(C_DX9_CALLBACK_BASE *p);
	//C_DX9_CALLBACK_BASE *GetCallback();

	void MouseMove(  );
	void KeyEvent( _KEY sKey );

	void UpdateObject( int iIndex );
};
