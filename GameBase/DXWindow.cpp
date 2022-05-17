#include "stdafx.h"
#include "DXWindow.h"
#include "DXHorizontalSliderBar.h"
#include "DXVerticalSliderBar.h"
#include "DXListVerticalSlider.h"
#include "DXButton.h"
#include "DXButtonCross.h"
#include "DXCheckBox.h"
#include "DXProgressBar.h"
#include "DXText.h"
#include "DXEditBox.h"
#include "DXDropDown.h"
#include "DXTextBox.h"
#include "DXListBox.h"

#include "DXEngine.h"


C_DX9_WINDOW::C_DX9_WINDOW( long width, long height, long nTSize) : nTitlebarSize(nTSize), pCloseBtn(0)
{
	SetMaximumWindow( TRUE );
	SetFocussedObject( 0 );
	dpMoveWindow = dk::DPOINT();
	bDragging = FALSE;
	SetMouseOver( FALSE );

	SetRelativePos( dk::DPOINT( 0, 0 ) );
	SetAbsolutePos( dk::DPOINT( 0, 0 ) );
	SetWidth( width );
	SetHeight( height );

	SetVisible( TRUE );

	//////////////////////////////////////////////////////////////////////////
	// 오브젝트 추가는 윈도우 만든곳에서 하자.

	SetThemeElement( DXENGINE()->GetThemeElement( _T("Window") ) );
	SetObjectState( _T("Normal") );

	float fltSize = (float)nTitlebarSize;
	// 80%의 사이즈를 구함.
	fltSize = (fltSize / 10) * 8;
	// 홀수라면 올림.
	if (((long)fltSize % 2)) { fltSize++; }
	// 버튼을 생성함.
	pCloseBtn = new C_DX9_CROSS_BUTTON(width, height);
	// ex) 1024 - 30 + (6 / 2)
	pCloseBtn->SetRelativePos(dk::DPOINT(width - nTitlebarSize + ((nTitlebarSize - (long)fltSize) / 2) , (nTitlebarSize - (long)fltSize) / 2));
	pCloseBtn->SetWidth((long)fltSize);
	pCloseBtn->SetHeight((long)fltSize);

	pCloseBtn->SetParent( this );
	//vObjects.push_back( pCloseBtn );

	MouseMove( );
}

C_DX9_WINDOW::C_DX9_WINDOW( C_TINY_XML_ELEMENT * pElement, long nTSize) : nTitlebarSize(nTSize), pCloseBtn(0)
{
	SetMaximumWindow( TRUE );
	
	dpMoveWindow = dk::DPOINT();
	bDragging = FALSE;
	SetMouseOver( FALSE );

	SetObject( pElement );

	/*
	for( C_TINY_XML_ELEMENT * pElementElement = pElement->FirstChildElement( _T("CrossButton") ); pElementElement; pElementElement = pElementElement->NextSiblingElement( _T("CrossButton") ) )
	{
		AddTitleObject( new C_DX9_CROSS_BUTTON( pElementElement ) );
	}
	*/
	#define LOAD_ELEMENTS( Class, String ) \
	for( C_TINY_XML_ELEMENT * pElementElement = pElement->FirstChildElement( String ); pElementElement; pElementElement = pElementElement->NextSiblingElement( String ) ) \
		AddObject( new Class( pElementElement ) );

	LOAD_ELEMENTS( C_DX9_EDIT_BOX, _T("EditBox") )
	LOAD_ELEMENTS( C_DX9_BUTTON, _T("Button") )
	LOAD_ELEMENTS( C_DX9_CHECK_BOX, _T("CheckBox") )
	LOAD_ELEMENTS( C_DX9_TEXT_BOX, _T("TextBox") )
	LOAD_ELEMENTS( C_DX9_LIST_BOX, _T("ListBox") )
	LOAD_ELEMENTS( C_DX9_DROP_DOWN, _T("DropDown") )
	LOAD_ELEMENTS( C_PROGRESS_BAR, _T("ProgressBar") )
	LOAD_ELEMENTS( C_DX9_HORIZONTAL_SLIDER_BAR, _T("HorizontalSliderBar") )
	LOAD_ELEMENTS( C_DX9_VERTICAL_SLIDER_BAR, _T("VerticalSliderBar") )
	LOAD_ELEMENTS( C_DX9_TEXT, _T("Text") )

	SetThemeElement( DXENGINE()->GetThemeElement( _T("Window") ) );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("Window"), 0 );
	else
		SetObjectState( _T("Normal") );

	float fltSize = (float)nTitlebarSize;
	// 80%의 사이즈를 구함.
	nCloseBtnSize = (long)((fltSize / 10) * 8);
	// 홀수라면 올림.
	if (nCloseBtnSize % 2)
	{
		nCloseBtnSize++;
	}
	// 버튼을 생성함.
	pCloseBtn = new C_DX9_CROSS_BUTTON(GetWidth(), nTitlebarSize);
	// ex) 1024 - 30 + (6 / 2)
	pCloseBtn->SetRelativePos(dk::DPOINT(GetWidth() - nTitlebarSize + ((nTitlebarSize - nCloseBtnSize) / 2) , (nTitlebarSize - nCloseBtnSize) / 2));
	pCloseBtn->SetWidth((long)nCloseBtnSize);
	pCloseBtn->SetHeight((long)nCloseBtnSize);

	pCloseBtn->SetParent( this );
	//vObjects.push_back( pCloseBtn );

	//////////////////////////////////////////////////////////////////////////
	// 오브젝트 중에 탭스톱이 있으면 첫번째 녀석한테 포커스를 준다.

	for( int i = 0; i < static_cast<int>( vObjects.size() ); i++ )
	{
		if(FALSE != vObjects[i]->GetTabStop())
		{
			//DBGPRINT(_T("탭스톱: %s"), vObjects[i]->GetObjectName().c_str());
			SetFocussedObject(vObjects[i]);
			break;
		}
	}
}

C_DX9_WINDOW::~C_DX9_WINDOW()
{
	for( int iIndex = 0; iIndex < static_cast<int>( vObjects.size() ); iIndex++ )
	{
		SAFE_DELETE( vObjects[ iIndex ] )
	}
	vObjects.clear();
}

void C_DX9_WINDOW::AddTitleObject( C_DX9_OBJECT * pElement )
{
	pElement->SetParent( this );
	vObjects.push_back( pElement );
}

void C_DX9_WINDOW::AddObject( C_DX9_OBJECT * pElement )
{
	pElement->SetRelativePos( *pElement->GetRelPos() + dk::DPOINT( 0, GetTitlebarSize() ) );
	pElement->SetParent( this );

	vObjects.push_back( pElement );
}

void C_DX9_WINDOW::Calculate()
{
	GetString( TRUE );
	if (0 != pCloseBtn)
	{
		pCloseBtn->Calculate();
	}
	if( GetMaximumWindow() )
	{
		for( int iIndex = 0; iIndex < static_cast<int>( vObjects.size() ); iIndex++ )
		{
			vObjects[ iIndex ]->Calculate();
		}
	}
}

void C_DX9_WINDOW::DisplayTitlebar()
{	
	if (this == DXENGINE()->GetMainWindow() || FALSE != DXENGINE()->GetMainWindow()->GetMaximumWindow())
	{
		pTitlebar->Display( *GetAbsolutePos(), GetWidth(), GetTitlebarSize() );

		int nSize = GetFormatted().length();
		if (0 < nSize)
		{
			wchar_t wszBuffer[MAX_PATH];
			MultiByteToWideChar(CP_UTF8, 0, GetFormatted().c_str(), nSize + 1, wszBuffer, nSize + 1);

			DXENGINE()->GetFont()->DrawString( GetAbsolutePos()->GetX() + 5, GetAbsolutePos()->GetY() + 5, 0, pTitle->GetD3DCOLOR(), wszBuffer );
		}
		if (0 != pCloseBtn)
		{
			pCloseBtn->Display();
		}
	}
}
void C_DX9_WINDOW::DisplayBoard()
{	
	if (this == DXENGINE()->GetMainWindow() || FALSE != DXENGINE()->GetMainWindow()->GetMaximumWindow())
	{
		if( GetMaximumWindow() )
		{
			DXENGINE()->DrawOutlinedBox(
				GetAbsolutePos()->GetX()
				, GetAbsolutePos()->GetY() + GetTitlebarSize()
				, GetWidth()
				, GetHeight() - GetTitlebarSize() + 1
				,  pBodyInner->GetD3DCOLOR()
				, pBodyBorder->GetD3DCOLOR()
			);

			for( int iIndex = 0; iIndex < static_cast<int>( vObjects.size() ); iIndex++ )
			{
				if (FALSE != vObjects[iIndex]->GetVisible())
				{
					vObjects[iIndex]->Display();
				}
			}
		}
	}
}

void C_DX9_WINDOW::Display()
{
	DisplayTitlebar();
	DisplayBoard();
}


void C_DX9_WINDOW::MouseMove()
{
	if (0 != pCloseBtn)
	{
		pCloseBtn->MouseMove();
	}
	if( GetDragging() )
	{
		if( !dpMoveWindow.GetX() )
		{
			dpMoveWindow = *GetAbsolutePos() - DXMOUSE()->GetPos();
		}
		else
		{
			dk::DPOINT mPos = DXMOUSE()->GetPos();
			if( mPos.GetX() == -1 && mPos.GetY() == -1 )
			{
				mPos = DXMOUSE()->GetSavedPos();
			}
			SetAbsolutePos( mPos + dpMoveWindow );
		}
	}
	if( GetMaximumWindow() )
	{
		for( int iIndex = 0; iIndex < static_cast<int>( vObjects.size() ); iIndex++ )
		{
			vObjects[ iIndex ]->MouseMove();
		}
	}
}

void C_DX9_WINDOW::KeyEvent( _KEY sKey )
{
	if (0 != pCloseBtn)
	{
		pCloseBtn->KeyEvent(sKey);
	}
	if (this != DXENGINE()->GetMainWindow())
	{
		if( DXMOUSE()->GetLeftButton() )
		{
			SetFocussedObject( 0 );

			if( DXMOUSE()->InArea( GetAbsolutePos()->GetX(), GetAbsolutePos()->GetY(), GetWidth() - GetTitlebarSize(), GetTitlebarSize() ) )
			{
				if( !DXMOUSE()->GetDragging() )
				{
					if( DXMOUSE()->GetLeftButton() == 1 )
					{
						DXENGINE()->BringToTop( this );

						SetDragging( TRUE );
						DXMOUSE()->SetDragging( this );

						SetObjectState( _T("Dragging") );
					}
					else
					{
						SetMaximumWindow( !GetMaximumWindow() );
						SetObjectState( GetMaximumWindow()?_T("Normal"):_T("Minimized") );

						DXENGINE()->BringToTop( this );
					}
				}
			}
			else if(  DXMOUSE()->InArea( GetAbsolutePos()->GetX(), GetAbsolutePos()->GetY(), GetWidth(), GetHeight() ) )
			{
				DXENGINE()->BringToTop( this );
			}
		}
		else
		{
			dpMoveWindow.SetX( 0 );

			DXMOUSE()->SetDragging( 0 );
			SetDragging( FALSE );

			SetObjectState( GetMaximumWindow()?_T("Normal"):_T("Minimized") );

			if (VK_ESCAPE == sKey.cKey && FALSE != sKey.bDown)
			{
				DXENGINE()->CloseTopWindow();
			}
		}
	}
	else
	{
		if (0 != DXMOUSE()->GetLeftButton() && FALSE != DXENGINE()->GetWindowMode())
		{
			if( DXMOUSE()->InArea( GetAbsolutePos()->GetX(), GetAbsolutePos()->GetY(), GetWidth() - GetTitlebarSize(), GetTitlebarSize() ) )
			{
				if (1 == DXMOUSE()->GetLeftButton())
				{
					DXENGINE()->SetMoveWindow(TRUE);

					RECT rc;
					GetWindowRect(DXENGINE()->GetMainWnd(), &rc);
					DXENGINE()->SetWindowPoint(rc.left, rc.top);
					GetCursorPos(DXENGINE()->GetMovePos());

					SetObjectState( _T("Dragging") );
				}
				else
				{
					SetMaximumWindow(!GetMaximumWindow());
					SetObjectState(GetMaximumWindow() ? _T("Normal") : _T("Minimized"));
				}
			}
		}
		else
		{
			if (DXENGINE()->GetMoveWindow())
			{
				DXENGINE()->SetMoveWindow(FALSE);
				SetObjectState(GetMaximumWindow() ? _T("Normal") : _T("Minimized"));
			}
		}
	}
	if( GetMaximumWindow() )
	{
		for( int iIndex = 0; iIndex < static_cast<int>( vObjects.size() ); iIndex++ )
		{
			vObjects[ iIndex ]->KeyEvent( sKey );
		}
	}
}

void C_DX9_WINDOW::SetMaximumWindow( BOOL bMaximized )
{
	bMinimumWindow = bMaximized;
}

BOOL C_DX9_WINDOW::GetMaximumWindow()
{
	return bMinimumWindow;
}

void C_DX9_WINDOW::SetDragging( BOOL b )
{
	bDragging = b;
}

BOOL C_DX9_WINDOW::GetDragging()
{
	return bDragging;
}

void C_DX9_WINDOW::SetCloseBtnCallback(C_DX9_CALLBACK_BASE *p)
{
	pCloseBtn->SetCallback(p);
}

C_DX9_CALLBACK_BASE *C_DX9_WINDOW::GetCloseBtnCallback()
{
	return(pCloseBtn->GetCallback());
}

void C_DX9_WINDOW::SetFocussedObject( C_DX9_OBJECT * pElement )
{
	pFocussedObject = pElement;
	if( pElement )
	{
		BringToTop( pElement );
	}
}
/*
void C_DX9_WINDOW::SetFocusNextObject()
{
	C_DX9_OBJECT *pNextObject = 0;
	int i = 0;
	if (0 != pFocussedObject)
	{
		for( ; i < static_cast<int>( vObjects.size() ); i++ )
		{
			if( vObjects[i] == pFocussedObject )
			{
				break;
			}
		}
	}
	for(i++; i < static_cast<int>( vObjects.size() ); i++ )
	{
		if(FALSE != vObjects[i]->GetTabStop() )
		{
			SetFocussedObject(vObjects[i]);
			break;
		}
	}
}
*/
C_DX9_OBJECT * C_DX9_WINDOW::GetFocussedObject()
{
	return pFocussedObject;
}

C_DX9_OBJECT * C_DX9_WINDOW::GetNextObject( C_DX9_OBJECT * pElement )
{
	for( int i = 0; i < static_cast<int>( vObjects.size() ); i++ )
	{
		if( vObjects[i] == pElement )
		{
			return vObjects[ i + 1 ];
		}
	}
	return *vObjects.begin();
}

C_DX9_OBJECT * C_DX9_WINDOW::GetObjectByString( LPCTSTR pszString, int iIndex )
{
	for( int i = 0; i < static_cast<int>( vObjects.size() ); i++ )
	{
		if( vObjects[ i ]->GetString( FALSE, iIndex ) == pszString )
		{
			return vObjects[ i ];
		}
	}
	return 0;
}

C_DX9_OBJECT *C_DX9_WINDOW::GetObjectByName(LPCTSTR pszString)
{
	for( int i = 0; i < static_cast<int>( vObjects.size() ); i++ )
	{
		if( vObjects[ i ]->GetObjectName() == pszString )
		{
			return vObjects[ i ];
		}
	}
	return 0;
}
void C_DX9_WINDOW::BringToTop( C_DX9_OBJECT * pElement )
{
	for( int i = 0; i < static_cast<int>( vObjects.size() ); i++ )
	{
		if( vObjects[i] == pElement )
		{
			vObjects.erase( vObjects.begin() + i );
		}
	}
	vObjects.insert(  vObjects.end(), pElement );
}

void C_DX9_WINDOW::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );
	if( !iIndex )
	{
		pTitle = pState->GetColor( _T("Title") );
		pBodyInner = pState->GetColor( _T("Board") );
		pBodyBorder = pState->GetColor( _T("Line") );

		pTitlebar = pState->GetTexture( _T("Titlebar") );
	}
}