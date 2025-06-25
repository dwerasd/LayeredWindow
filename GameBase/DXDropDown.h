#pragma once


#include "DXDef.h"
#include "DXObjectBase.h"
#include "DXTexture.h"



class C_DX9_DROP_DOWN : public C_DX9_OBJECT
{
private:
	BOOL m_bDropped;
	int m_iMouseOverIndex, m_iSelected;
	
	struct SEntry
	{
		std::_tstring m_sString, m_sValue;

		SEntry( std::_tstring sString, std::_tstring sValue )
		{
			m_sString = sString;
			m_sValue = sValue;
		}
	};
	std::vector<SEntry> m_vEntrys;

	dk::DCOLOR * pInner, * pBorder, * pString, * pSelectedInner, * pSelectedString;
	C_DX9_TEXTURE * pButton;

public:
	C_DX9_DROP_DOWN( C_TINY_XML_ELEMENT * pElement );

	void Display();
	void MouseMove(  );
	void KeyEvent( _KEY sKey );

	void AddObject( std::_tstring sElem, std::_tstring sValue );
	std::_tstring GetValue();

	void UpdateObject( int iIndex );
};