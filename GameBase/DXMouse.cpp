#include "stdafx.h"
#include "DXMouse.h"
#include "DXEngine.h"



C_DX_MOUSE::C_DX_MOUSE()
{
	//m_pInnerColor = new C_DARK_COLOR( 255, 255, 255, 255 );
	//m_pBorderColor = new C_DARK_COLOR( 0, 0, 0, 255 );

	SetLeftButton( 0 );
	SetRightButton( 0 );
	SetMiddleButton( 0 );
	SetWheel( 0 );

	SetDragging( 0 );
}

C_DX_MOUSE::~C_DX_MOUSE()
{
}

BOOL C_DX_MOUSE::Calculate( unsigned int _nMessage, WPARAM wParam, LPARAM lParam )
{
	if( _nMessage < WM_MOUSEFIRST || _nMessage > WM_MOUSELAST )
	{
		return(0);
	}
	BOOL bDown = FALSE;

	switch( _nMessage )
	{
	case WM_MOUSEMOVE:
		{
			SetPos( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
			DXENGINE()->MouseMove();
		}
		return(FALSE);
	case WM_LBUTTONDOWN:
		SetLeftButton( 1 );
		bDown = TRUE;
		break;
	case WM_RBUTTONDOWN:
		SetRightButton( 1 );
		bDown = TRUE;
		break;
	case WM_MBUTTONDOWN:
		SetMiddleButton( 1 );
		bDown = TRUE;
		break;

	case WM_LBUTTONUP:
		SetLeftButton( 0 );
		break;
	case WM_RBUTTONUP:
		SetRightButton( 0 );
		break;
	case WM_MBUTTONUP:
		SetMiddleButton( 0 );
		break;

	case WM_MOUSEWHEEL:
		float fDelta = GET_WHEEL_DELTA_WPARAM( wParam );
		
		if( fDelta > 0.0f )
			SetWheel( 1 );
		else if( fDelta < 0.0f )
			SetWheel( 2 );
		else
			SetWheel( 0 );

		break;
	}
	_KEY kRet(0, bDown, lParam);

	return DXENGINE()->KeyEvent( kRet );
}

void C_DX_MOUSE::SetPos( C_DARK_POINT cPos )
{
	m_pos = cPos;
}

void C_DX_MOUSE::SetPos( int iX, int iY )
{
	m_pos.SetX( iX );
	m_pos.SetY( iY );
}

C_DARK_POINT C_DX_MOUSE::GetPos()
{
	return m_pos;
}

BOOL C_DX_MOUSE::InArea( int iX, int iY, int iWidth, int iHeight )
{
	return ( m_pos.GetX() >= iX && m_pos.GetX() <= iX + iWidth && m_pos.GetY() >= iY && m_pos.GetY() <= iY + iHeight );
}

BOOL C_DX_MOUSE::InArea( C_DX_OBJECT_BASE * pElement, int iHeight )
{
	if( !iHeight )
		iHeight = pElement->GetHeight();

	return InArea( pElement->GetAbsolutePos()->GetX(), pElement->GetAbsolutePos()->GetY(), pElement->GetWidth(), iHeight );
}

void C_DX_MOUSE::Display()
{
	//static C_DARK_COLOR * pColor = new C_DARK_COLOR( 255, 1, 1, 255 );
	//DXENGINE()->FillArea( m_pos.GetX(), m_pos.GetY(), 10, 10, pColor->GetD3DCOLOR() );
}

int C_DX_MOUSE::GetLeftButton( int iState )
{
	int iRet = nLeftButton;

	if( iState != -1 )
		SetLeftButton( iState );

	return iRet;
}

int C_DX_MOUSE::GetRightButton( int iState )
{
	int iRet = m_iRightButton;

	if( iState != -1 )
		SetRightButton( iState );

	return iRet;
}

int C_DX_MOUSE::GetMiddleButton( int iState )
{
	int iRet = m_iMiddleButton;

	if( iState != -1 )
		SetMiddleButton( iState );

	return iRet;
}

int C_DX_MOUSE::GetWheel( int iState )
{
	int iRet = m_iWheel;

	if( iState != -1 )
		SetWheel( iState );

	return iRet;
}

void C_DX_MOUSE::SetLeftButton( int iState )
{
	if( iState == 1 )
	{
		if( tLeftButton.Running() )
		{
			nLeftButton = 2;
			tLeftButton.Stop();
		}
		else
		{
			nLeftButton = 1;
			tLeftButton.Start( DBL_CLICK_TIME );
		}
	}
	else
	{
		nLeftButton = iState;
	}
}

void C_DX_MOUSE::SetRightButton( int iState )
{
	if( iState == 1 )
	{
		if( m_tRightButton.Running() )
		{
			m_iRightButton = 2;
			m_tRightButton.Stop();
		}
		else
		{
			m_iRightButton = 1;
			m_tRightButton.Start( DBL_CLICK_TIME );
		}
	}
	else
	{
		m_iRightButton = iState;
	}
}

void C_DX_MOUSE::SetMiddleButton( int iState )
{
	if( iState == 1 )
	{
		if( m_tMiddleButton.Running() )
		{
			m_iMiddleButton = 2;
			m_tMiddleButton.Stop();
		}
		else
		{
			m_iMiddleButton = 1;
			m_tMiddleButton.Start( DBL_CLICK_TIME );
		}
	}
	else
		m_iMiddleButton = iState;
}

void C_DX_MOUSE::SetWheel( int iState )
{
	m_iWheel = iState;
}

void C_DX_MOUSE::SetDragging( C_DX_OBJECT_BASE * pElement )
{
	m_pDraggingElement = pElement;
}

C_DX_OBJECT_BASE * C_DX_MOUSE::GetDragging()
{
	return m_pDraggingElement;
}

void C_DX_MOUSE::SavePos()
{
	m_bpos = m_pos;
}

void C_DX_MOUSE::LoadPos()
{
	m_pos = m_bpos;
}

C_DARK_POINT C_DX_MOUSE::GetSavedPos()
{
	return m_bpos;
}