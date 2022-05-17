#pragma once



#include "DXDef.h"



//////////////////////////////////////////////////////////////////////////
// 

class C_MAIN_CLOSE_BUTTON : public C_DX9_CALLBACK_BASE
{
public:
	std::_tstring Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam = 0);
};

class C_EXIT_BUTTON1 : public C_DX9_CALLBACK_BASE
{
public:
	std::_tstring Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam = 0);
};

class C_EXIT_BUTTON2 : public C_DX9_CALLBACK_BASE
{
public:
	std::_tstring Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam = 0);
};

//////////////////////////////////////////////////////////////////////////
//

class C_LOGIN_EDITBOX1 : public C_DX9_CALLBACK_BASE
{
public:
	std::_tstring Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam = 0);
};

class C_LOGIN_EDITBOX2 : public C_DX9_CALLBACK_BASE
{
public:
	std::_tstring Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam = 0);
};

class C_LOGIN_BUTTON1 : public C_DX9_CALLBACK_BASE
{
public:
	std::_tstring Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam = 0);
};

class C_LOGIN_BUTTON2 : public C_DX9_CALLBACK_BASE
{
public:
	std::_tstring Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam = 0);
};

//////////////////////////////////////////////////////////////////////////