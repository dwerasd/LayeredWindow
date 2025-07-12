#pragma once


class C_DX_OBJECT_BASE;

#include "DXDef.h"
#include "DXObjectBase.h"



class C_DX_MOUSE
{
private:
	C_DX_MOUSE();
	~C_DX_MOUSE();

	C_DARK_POINT m_pos, m_bpos;
	//C_DARK_COLOR *m_pInnerColor, * m_pBorderColor;
	C_DARK_TIMER tLeftButton, m_tRightButton, m_tMiddleButton;

	LPDIRECT3DDEVICE9 pDevice;
	C_DX_OBJECT_BASE *m_pDraggingElement;

	int nLeftButton, m_iRightButton, m_iMiddleButton, m_iWheel;

public:
	static C_DX_MOUSE *GetInstance()
	{
		static C_DX_MOUSE *pDXMouse = 0;
		if (0 == pDXMouse)
		{
			pDXMouse = new C_DX_MOUSE();
		}
		return(pDXMouse);
	}

	BOOL Calculate( unsigned int uMsg, WPARAM wParam, LPARAM lParam );

	void SetPos( int iX, int iY );
	void SetPos( C_DARK_POINT cPos );
	C_DARK_POINT GetPos();

	BOOL InArea( int iX, int iY, int iWidth, int iHeight );
	BOOL InArea( C_DX_OBJECT_BASE * pElement, int iHeight = 0 );

	void Display();

	int GetLeftButton( int iState = -1 );
	int GetRightButton( int iState = -1 );
	int GetMiddleButton( int iState = -1 );
	int GetWheel( int iState = -1 );

	void SetLeftButton( int iState );
	void SetRightButton( int iState );
	void SetMiddleButton( int iState );
	void SetWheel( int iState );

	void SetDragging( C_DX_OBJECT_BASE * pElement );
	C_DX_OBJECT_BASE * GetDragging();

	void SavePos();
	void LoadPos();
	C_DARK_POINT GetSavedPos();
};

#define DXMOUSE C_DX_MOUSE::GetInstance