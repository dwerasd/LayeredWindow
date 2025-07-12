// GameBase.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "GameBase.h"
#include "DXKeyboard.h"
#include "Handlers.h"



TCHAR tszWindowName[] = { _T("게임 1.0") };
TCHAR tszClassName[] = { _T("클라스이름") };


LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (0 != MAINCLASS()->GetMainWnd())
	{
		if( DXMOUSE()->Calculate( uMsg, wParam, lParam ) || DXKEYBD()->Calculate( uMsg, wParam, lParam ) )
		{
			return 0;
		}
	}
	
	switch( uMsg )
	{
	case WM_CREATE:
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		{
			DestroyWindow( hWnd );
			UnregisterClass( "WoWXGui", GetModuleHandle( 0 ) );
			ExitProcess(0);
			return 0;
		}
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void MessageHandler()
{
	MSG Msg;
	while( PeekMessage( &Msg, NULL, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &Msg );
		DispatchMessage( &Msg );
		ZeroMemory( &Msg, sizeof( MSG ) );
	}
}




long C_MAIN_CLASS::Init(HINSTANCE h, BOOL bWinMode, long nWidth, long nHeight)
{
	hInstance = h;
	//CONSOLE()->Create(_T("클라이언트 1.0"), YELLOW);
	//SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE); 

	bWindowMode = bWinMode;

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), 0 };

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEBASE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = tszClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if (FALSE != RegisterClassEx(&wcex))
	{
		if (FALSE != bWindowMode)
		{
#if defined(LAYERED_WINDOW)
			hMainWnd = CreateWindowEx(WS_EX_LAYERED /*| WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW*/,
#else
			hMainWnd = CreateWindowEx(NULL, 
#endif
				tszClassName, tszWindowName,
				WS_POPUP | WS_OVERLAPPED | WS_CLIPCHILDREN | WS_VISIBLE,
				((GetSystemMetrics(SM_CXFULLSCREEN) - nWidth) / 2),
				((GetSystemMetrics(SM_CYFULLSCREEN) - nHeight) / 2),
				nWidth,
				nHeight,
				NULL, NULL, hInstance, NULL);
		}
		else
		{
			hMainWnd = CreateWindow(tszClassName, tszWindowName, WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
				NULL, NULL, hInstance, NULL);
		}
		UpdateWindow(hMainWnd);

		/*
		RECT rcClient;
		GetClientRect( MAINCLASS()->GetMainWnd(),&rcClient );
		MoveWindow( MAINCLASS()->GetMainWnd(), 0, 0, nWidth + ( nWidth - rcClient.right ), nHeight + ( nHeight - rcClient.bottom ), 1 );
		*/
		return DXENGINE()->Init(hMainWnd, nWidth, nHeight, TRUE);
	}
	return(FALSE);
}

long C_MAIN_CLASS::Create()
{
	
	DXENGINE()->Cvars[_T("$MainCloseButton")] = new C_DX9_VARIANT( new C_MAIN_CLOSE_BUTTON() );
	DXENGINE()->Cvars[_T("$ExitWindow_button1")] = new C_DX9_VARIANT( new C_EXIT_BUTTON1() );
	DXENGINE()->Cvars[_T("$ExitWindow_button2")] = new C_DX9_VARIANT( new C_EXIT_BUTTON2() );
	DXENGINE()->Cvars[_T("$LoginWindow_editbox1")] = new C_DX9_VARIANT( new C_LOGIN_EDITBOX1() );
	DXENGINE()->Cvars[_T("$LoginWindow_editbox2")] = new C_DX9_VARIANT( new C_LOGIN_EDITBOX2() );
	DXENGINE()->Cvars[_T("$LoginWindow_button1")] = new C_DX9_VARIANT( new C_LOGIN_BUTTON1() );
	DXENGINE()->Cvars[_T("$LoginWindow_button2")] = new C_DX9_VARIANT( new C_LOGIN_BUTTON2() );

	//////////////////////////////////////////////////////////////////////////
	// 디렉윈도우를 만들자.

	DXENGINE()->LoadInterfaceFromFile( _T("ColorThemes.xml") );
	DXENGINE()->SetVisible( TRUE );
	DXENGINE()->Create();

	//////////////////////////////////////////////////////////////////////////
	// 함수들을 등록하자.

	
	DXENGINE()->GetMainWindow()->SetCloseBtnCallback( DXENGINE()->Cvars[_T("$MainCloseButton")]->GetCallback() );
	DXENGINE()->GetWindowByName(_T("ExitWindow"))->GetObjectByName(_T("button1"))->SetCallback( DXENGINE()->Cvars[_T("$ExitWindow_button1")]->GetCallback() );
	DXENGINE()->GetWindowByName(_T("ExitWindow"))->GetObjectByName(_T("button2"))->SetCallback( DXENGINE()->Cvars[_T("$ExitWindow_button2")]->GetCallback() );
	DXENGINE()->GetWindowByName(_T("LoginWindow"))->GetObjectByName(_T("editbox1"))->SetCallback( DXENGINE()->Cvars[_T("$LoginWindow_editbox1")]->GetCallback() );
	DXENGINE()->GetWindowByName(_T("LoginWindow"))->GetObjectByName(_T("editbox2"))->SetCallback( DXENGINE()->Cvars[_T("$LoginWindow_editbox2")]->GetCallback() );
	DXENGINE()->GetWindowByName(_T("LoginWindow"))->GetObjectByName(_T("button1"))->SetCallback( DXENGINE()->Cvars[_T("$LoginWindow_button1")]->GetCallback() );
	DXENGINE()->GetWindowByName(_T("LoginWindow"))->GetObjectByName(_T("button2"))->SetCallback( DXENGINE()->Cvars[_T("$LoginWindow_button2")]->GetCallback() );


	C_DX9_WINDOW *pWindow = DXENGINE()->GetWindowByName(_T("LoginWindow"));
	pWindow->SetAbsolutePos(dk::DPOINT((DXENGINE()->GetMainWindow()->GetWidth() - pWindow->GetWidth()) / 2, (DXENGINE()->GetMainWindow()->GetHeight() - pWindow->GetHeight()) / 2));

	//////////////////////////////////////////////////////////////////////////
	// 최상위가 아니라면 최상위로

	if (DXENGINE()->GetTopWindow() != pWindow)
	{
		DXENGINE()->BringToTop(pWindow);
		pWindow->SetVisible(TRUE);
	}

	return(TRUE);
}
long C_MAIN_CLASS::Calculate()
{
	MessageHandler();
	DXENGINE()->Calculate();
	return(0);
}

void C_MAIN_CLASS::Display()
{
	if (!tFrame.Running())
	{
		dwFPS = dwFrame;
		dwFrame = 0;
		tFrame.Start(1.0f);
	}

	DXENGINE()->Clear(D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f));

	//////////////////////////////////////////////////////////////////////////
	// 메인 윈도우를 그린다.

	
	DXENGINE()->DisplayMainBoard();

	//////////////////////////////////////////////////////////////////////////
	// 여기에서 게임 내용들을 그리면 된다.

	DXENGINE()->BeginScene();







	DXENGINE()->EndScene();

	//////////////////////////////////////////////////////////////////////////
	// 팝업을 그린다

	DXENGINE()->Display();

	DXENGINE()->DisplayMainTitle();

	DXENGINE()->BeginScene();

	RECT rcFPS;
	SetRect(&rcFPS, DXENGINE()->GetMainTitle()->right - DXENGINE()->GetMainTitle()->bottom - 20, DXENGINE()->GetMainTitle()->top + 8, 0, 0);

	DXENGINE()->GetFont()->DrawStringEx(rcFPS, FT_VCENTER, D3DCOLOR_XRGB(8, 8, 8), _T("FPS: %d"), dwFPS);
	rcFPS.left--;
	rcFPS.top--;
	DXENGINE()->GetFont()->DrawStringEx(rcFPS, FT_VCENTER, D3DCOLOR_XRGB(255, 0, 0), _T("FPS: %d"), dwFPS);
	DXENGINE()->EndScene();

	//DXENGINE()->GetDevice()->Present( 0, 0, 0, 0 );
	DXENGINE()->Present();

	dwFrame++;
}

void C_MAIN_CLASS::Destory()
{

}

void C_MAIN_CLASS::Login()
{
	DBGPRINT(_T("%s / %s"), tstrUserID.c_str(), tstrUserPW.c_str());
	bLogin = TRUE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	if(FALSE != MAINCLASS()->Init(hInstance, TRUE, 1024, 768))
	{
		if (FALSE != MAINCLASS()->Create())
		{
			do 
			{
				MAINCLASS()->Calculate();
				MAINCLASS()->Display();
			} while (TRUE);
		}
	}
	return 0;
}
