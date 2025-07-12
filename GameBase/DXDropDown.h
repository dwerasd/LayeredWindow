#pragma once


#include "DXDef.h"
#include "DXObjectBase.h"
#include "DXTexture.h"



class C_DX_DROP_DOWN : public C_DX_OBJECT_BASE
{
private:
	BOOL m_bDropped;
	int m_iMouseOverIndex, m_iSelected;
	
	struct SEntry
	{
		_tstring m_sString, m_sValue;

		SEntry( _tstring sString, _tstring sValue )
		{
			m_sString = sString;
			m_sValue = sValue;
		}
	};
	std::vector<SEntry> m_vEntrys;

	C_DARK_COLOR * pInner, * pBorder, * pString, * pSelectedInner, * pSelectedString;
	C_DX_TEXTURE * pButton;

public:
	C_DX_DROP_DOWN( C_TINY_XML_ELEMENT * pElement );

	void Display();
	void MouseMove(  );
	void KeyEvent( _KEY sKey );

	void AddObject( _tstring sElem, _tstring sValue );
	_tstring GetValue();

	void UpdateObject( int iIndex );
};