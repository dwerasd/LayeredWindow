#pragma once


#include "DXObjectBase.h"



class C_DX9_CALLBACK_BASE
{
public:
	virtual std::_tstring Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam = 0)
	{
		return _T("");
	}
};