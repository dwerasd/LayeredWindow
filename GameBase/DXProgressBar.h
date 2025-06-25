#pragma once


#include "DXDef.h"
#include "DXTexture.h"



class C_PROGRESS_BAR : public C_DX9_OBJECT
{
private:
	int m_iProgress, m_iStrWidth, m_iStrHeight;
	std::_tstring m_sBuf;

	dk::DCOLOR * pInner, * pBorder, * pString;
	C_DX9_TEXTURE * pProgressBar;

public:
	C_PROGRESS_BAR( C_TINY_XML_ELEMENT * pElement );

	void Display();

	int GetProgress();
	void SetProgress( int iProgress );

	void UpdateObject( int iIndex );
};