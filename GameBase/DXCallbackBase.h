#pragma once


#include "DXObjectBase.h"



class C_DX_CALLBACK_BASE
{
public:
	virtual _tstring Calculate(C_DX_OBJECT_BASE *pObject, LPCTSTR ptszParam = 0)
	{
		return _T("");
	}
};