#pragma once


#include "DXDef.h"
#include "DXTexture.h"
#include "DXObjectBase.h"




class C_DX_CROSS_BUTTON
	: public C_DX_OBJECT_BASE
{
private:
	BOOL bPressed;
	C_DX_TEXTURE *pTexture;

	//C_DX_CALLBACK_BASE *pCallback;

public:
	C_DX_CROSS_BUTTON(long width = 20, long height = 20 );
	C_DX_CROSS_BUTTON(C_TINY_XML_ELEMENT * pElement, long width = 20, long height = 20 );

	void Calculate();
	void Display();

	//void SetCallback(C_DX_CALLBACK_BASE *p);
	//C_DX_CALLBACK_BASE *GetCallback();

	void MouseMove(  );
	void KeyEvent( _KEY sKey );

	void UpdateObject( int iIndex );
};