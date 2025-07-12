#pragma once


#include <Windows.h>
#include <windowsx.h>
#include <shlwapi.h>
#include <tchar.h>

#include <d3d9.h>
#pragma comment( lib, "d3d9.lib" )

#include <d3dx9.h>
#pragma comment( lib, "d3dx9.lib" )

#include <map>
#include <set>
#include <limits>
#include <iomanip>
#include <sstream>
#include <vector>

#include <DarkApi.h>
#include <DarkTimer.h>
#include <DarkPoint.h>
#include <DarkColor.h>

#include <DarkXML.h>


#define FCR_NONE	0x0
#define FCR_BOLD 	0x1
#define FCR_ITALICS 0x2

#define FT_NONE		0x0
#define FT_CENTER	0x1
#define FT_BORDER	0x2
#define FT_VCENTER	0x4
#define FT_SINGLELINE 0x8

//FVF Macros
#define D3DFVF_BITMAPFONT	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_PRIMITIVES	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

//Vertex Types
struct d3dfont_s
{
	float x,y,z,rhw; DWORD colour; float tu,tv;
};

typedef struct
{
	float x,y,z,rhw; DWORD colour;
} d3dprimitives_s;


#define WINDOW_CLOSE_BUTTON_HEIGHT	20

#define HORIZONTAL_SLIDER_HEIGHT 24
#define VERTICAL_SLIDER_WIDTH 24

#define LIST_SLIDER_WIDTH 22

#define SAFE_DELETE( pData ) if( pData ) { delete pData; pData = 0; }

//Initialization Macros
#undef SAFE_RELEASE
#define SAFE_RELEASE( pInterface ) if( pInterface ) { pInterface->Release(); pInterface = 0; }


struct _KEY
{
	TCHAR cKey;
	BOOL bDown;
	LPARAM lParam;

	_KEY()
	{
		cKey = 0;
		bDown = FALSE;
		lParam = 0;
	}
	_KEY( char vKey, BOOL b )
	{
		cKey = vKey;
		bDown = b;
		lParam = 0;
	}
	_KEY( char vKey, BOOL b, LPARAM lParam )
	{
		cKey = vKey;
		bDown = b;
		lParam = lParam;
	}
};

#define DBL_CLICK_TIME 0.3f


#define LAYERED_WINDOW


#define KEY_DOWN(vk_code) ( (GetFocus())?((GetAsyncKeyState(vk_code) & 0x8000)? 1: 0):0  )//
#define KEY_UP(vk_code) ( (GetFocus())?((GetAsyncKeyState(vk_code) & 0x8000)? 0: 1):0 )


#include "DXMouse.h"
#include "DXKeyboard.h"
#include "DXObjectBase.h"
#include "DXCallbackBase.h"