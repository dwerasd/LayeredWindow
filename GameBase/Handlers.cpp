#include "stdafx.h"
#include "Handlers.h"
#include "DXEngine.h"
#include "GameBase.h"



std::_tstring C_MAIN_CLOSE_BUTTON::Calculate(C_DX9_OBJECT *pElement, LPCTSTR pszString)
{
	C_DX9_WINDOW *pWindow = DXENGINE()->GetWindowByName(_T("ExitWindow"));
	if (0 != pWindow)
	{
		pWindow->SetAbsolutePos(dk::DPOINT((DXENGINE()->GetMainWindow()->GetWidth() - pWindow->GetWidth()) / 2, (DXENGINE()->GetMainWindow()->GetHeight() - pWindow->GetHeight()) / 2));

		//////////////////////////////////////////////////////////////////////////
		// 최상위가 아니라면 최상위로

		if (DXENGINE()->GetTopWindow() != pWindow)
		{
			DXENGINE()->BringToTop(pWindow);
			pWindow->SetVisible(TRUE);
			pWindow->SetFocussedObject(pWindow->GetObjectByName(_T("button1")));
		}
		else
		{
			pWindow->SetVisible(!pWindow->GetVisible());
		}
	}
	return _T("");
}

//////////////////////////////////////////////////////////////////////////
//

std::_tstring C_EXIT_BUTTON1::Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam)
{
	ExitProcess(0);
	return _T("");
}
std::_tstring C_EXIT_BUTTON2::Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam)
{
	C_DX9_WINDOW *pWindow = DXENGINE()->GetWindowByName(_T("ExitWindow"));
	pWindow->SetVisible(FALSE);
	return _T("");
}

//////////////////////////////////////////////////////////////////////////
//

std::_tstring C_LOGIN_EDITBOX1::Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam)
{
	MAINCLASS()->SetUserID(ptszParam);

	C_DX9_WINDOW *pWindow = DXENGINE()->GetWindowByName(_T("LoginWindow"));
	pWindow->SetFocussedObject(pWindow->GetObjectByName(_T("editbox2")));

	return _T("");
}
std::_tstring C_LOGIN_EDITBOX2::Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam)
{
	MAINCLASS()->SetUserPW(ptszParam);

	C_DX9_WINDOW *pWindow = DXENGINE()->GetWindowByName(_T("LoginWindow"));
	pWindow->SetFocussedObject(pWindow->GetObjectByName(_T("button1")));

	pWindow->GetObjectByName(_T("button1"))->GetCallback()->Calculate(pWindow);

	return _T("");
}

#include "DXEditBox.h"
std::_tstring C_LOGIN_BUTTON1::Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam)
{
	C_DX9_WINDOW *pLoginWindow = DXENGINE()->GetWindowByName(_T("LoginWindow"));
	if (4 > MAINCLASS()->GetUserID().length())
	{
		C_DX9_EDIT_BOX *pEditBox = (C_DX9_EDIT_BOX *)pLoginWindow->GetObjectByName(_T("editbox1"));
		pEditBox->SetIndex(0);
		pEditBox->SetStart(0);
		pEditBox->SetString(_T(""));
		pLoginWindow->SetFocussedObject(pEditBox);
	}
	else if (4 > MAINCLASS()->GetUserPW().length())
	{
		C_DX9_EDIT_BOX *pEditBox = (C_DX9_EDIT_BOX *)pLoginWindow->GetObjectByName(_T("editbox2"));
		pEditBox->SetIndex(0);
		pEditBox->SetStart(0);
		pEditBox->SetString(_T(""));
		pLoginWindow->SetFocussedObject(pEditBox);
	}
	else
	{
		DXENGINE()->GetWindowByName(_T("LoginWindow"))->SetVisible(FALSE);
		MAINCLASS()->Login();
	}
	return _T("");
}

std::_tstring C_LOGIN_BUTTON2::Calculate(C_DX9_OBJECT *pObject, LPCTSTR ptszParam)
{
	//DBGPRINT(_T("로긴2눌림"));
	return _T("");
}

//////////////////////////////////////////////////////////////////////////
//