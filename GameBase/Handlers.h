#pragma once



#include "DXDef.h"



//////////////////////////////////////////////////////////////////////////
// 

class C_MAIN_CLOSE_BUTTON : public C_DX_CALLBACK_BASE
{
public:
	_tstring Calculate(C_DX_OBJECT_BASE *pObject, LPCTSTR ptszParam = 0);
};

class C_EXIT_BUTTON1 : public C_DX_CALLBACK_BASE
{
public:
	_tstring Calculate(C_DX_OBJECT_BASE *pObject, LPCTSTR ptszParam = 0);
};

class C_EXIT_BUTTON2 : public C_DX_CALLBACK_BASE
{
public:
	_tstring Calculate(C_DX_OBJECT_BASE *pObject, LPCTSTR ptszParam = 0);
};

//////////////////////////////////////////////////////////////////////////
//

class C_LOGIN_EDITBOX1 : public C_DX_CALLBACK_BASE
{
public:
	_tstring Calculate(C_DX_OBJECT_BASE *pObject, LPCTSTR ptszParam = 0);
};

class C_LOGIN_EDITBOX2 : public C_DX_CALLBACK_BASE
{
public:
	_tstring Calculate(C_DX_OBJECT_BASE *pObject, LPCTSTR ptszParam = 0);
};

class C_LOGIN_BUTTON1 : public C_DX_CALLBACK_BASE
{
public:
	_tstring Calculate(C_DX_OBJECT_BASE *pObject, LPCTSTR ptszParam = 0);
};

class C_LOGIN_BUTTON2 : public C_DX_CALLBACK_BASE
{
public:
	_tstring Calculate(C_DX_OBJECT_BASE *pObject, LPCTSTR ptszParam = 0);
};

//////////////////////////////////////////////////////////////////////////