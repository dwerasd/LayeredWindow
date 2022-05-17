#pragma once


class C_DX9_OBJECT;

#include "DXDef.h"
#include "DXObjectBase.h"



class C_DX9_MOUSE
{
private:
	C_DX9_MOUSE();
	~C_DX9_MOUSE();

	dk::DPOINT m_pos, m_bpos;
	dk::DCOLOR *m_pInnerColor, * m_pBorderColor;
	dk::C_TIMER tLeftButton, m_tRightButton, m_tMiddleButton;

	LPDIRECT3DDEVICE9 pDevice;
	C_DX9_OBJECT *m_pDraggingElement;

	int nLeftButton, m_iRightButton, m_iMiddleButton, m_iWheel;

public:
	static C_DX9_MOUSE *GetInstance()
	{
		static C_DX9_MOUSE *pDXMouse = 0;
		if (0 == pDXMouse)
		{
			pDXMouse = new C_DX9_MOUSE();
		}
		return(pDXMouse);
	}

	BOOL Calculate( unsigned int uMsg, WPARAM wParam, LPARAM lParam );

	void SetPos( int iX, int iY );
	void SetPos( dk::DPOINT cPos );
	dk::DPOINT GetPos();

	BOOL InArea( int iX, int iY, int iWidth, int iHeight );
	BOOL InArea( C_DX9_OBJECT * pElement, int iHeight = 0 );

	void Display();

	int GetLeftButton( int iState = -1 );
	int GetRightButton( int iState = -1 );
	int GetMiddleButton( int iState = -1 );
	int GetWheel( int iState = -1 );

	void SetLeftButton( int iState );
	void SetRightButton( int iState );
	void SetMiddleButton( int iState );
	void SetWheel( int iState );

	void SetDragging( C_DX9_OBJECT * pElement );
	C_DX9_OBJECT * GetDragging();

	void SavePos();
	void LoadPos();
	dk::DPOINT GetSavedPos();
};

#define DXMOUSE C_DX9_MOUSE::GetInstance