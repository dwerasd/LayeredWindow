#pragma once

#include "resource.h"


#include <DarkTimer.h>


#include "DXEngine.h"



class C_MAIN_CLASS
{
private:
	C_MAIN_CLASS() : hMainWnd(0), dwFrame(0), dwFPS(0), bLogin(0)
	{
		
	}
	~C_MAIN_CLASS()
	{

	}
	WNDPROC hPrevProc;
	BOOL bWindowMode, bLogin;

	HINSTANCE hInstance;
	HWND hMainWnd;

	dk::C_TIMER tFrame;
	DWORD dwFrame,  dwFPS;

	std::_tstring tstrUserID;
	std::_tstring tstrUserPW;
	
public:
	static C_MAIN_CLASS *GetInstance()
	{
		static C_MAIN_CLASS *pClient = NULL;
		if (NULL == pClient)
		{
			pClient = new C_MAIN_CLASS();
		}
		return(pClient);
	}

	long Init(HINSTANCE h, BOOL bWinMode, long nWidth, long nHeight);
	long Create();
	long Calculate();
	void Display();
	void Destory();

	HWND GetMainWnd() { return(hMainWnd); }

	void SetUserID(LPCTSTR ptszUserID) { tstrUserID = ptszUserID; };
	std::_tstring GetUserID() { return(tstrUserID); }
	void SetUserPW(LPCTSTR ptszUserPW) { tstrUserPW = ptszUserPW; };
	std::_tstring GetUserPW() { return(tstrUserPW); }

	void Login();
};

#define MAINCLASS	C_MAIN_CLASS::GetInstance
