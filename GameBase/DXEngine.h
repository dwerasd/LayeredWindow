#pragma once


#include "DXDef.h"
#include "DXFont.h"
#include "DXTexture.h"
#include "DXObjectBase.h"
#include "DXWindow.h"
#include "DXVariant.h"



class C_DX9_ENGINE
{
private:
	C_DX9_ENGINE();
	~C_DX9_ENGINE();

	BOOL bVisible, bWindowMode, m_bReload, bMoveWindow;

	HWND hMainWnd;

	C_DX9_FONT * pFont;

	LPDIRECT3D9			pDirect3D;		// 다이렉트 3D의 객체
	LPDIRECT3DDEVICE9	pDevice;		// 3D의 디바이스 장치
	LPD3DXSPRITE		pSprite;
	LPD3DXLINE			pLine;

	BYTE bytAlphaBlend;

	C_DX9_WINDOW *pMainWindow;
	RECT rcMainWindow, rcMainTitle;
	POINT ptMainWindow, ptMoveWindow;

	std::vector<C_DX9_WINDOW *> vWindow;

	std::_tstring tstrThemeName;

	typedef std::map<std::_tstring, _OBJECT_BASE*> tTheme;
	std::map<std::_tstring, tTheme> mThemes;

public:
	static C_DX9_ENGINE *GetInstance()
	{
		static C_DX9_ENGINE *pDXEngine = 0;
		if (0 == pDXEngine)
		{
			pDXEngine = new C_DX9_ENGINE();
		}
		return(pDXEngine);
	}

	BOOL Init(HWND hWnd, long nWidth, long nHeight, BOOL bWinMode );
	void Create();
	void Calculate();
	void DisplayMainTitle();
	void DisplayMainBoard();
	void Display();

#if defined(LAYERED_WINDOW)
	struct _IMAGE
	{
		int nWidth;
		int nHeight;
		int nPitch;
		HDC hdc;
		HBITMAP hBitmap;
		BITMAPINFO info;
		BYTE *pPixels;
	} imgBack;

	BOOL bInitLayeredWindow;
	LPDIRECT3DTEXTURE9 pLayeredTexture;
	LPDIRECT3DSURFACE9 pSurface, pRenderTargetSurface, pDepthStencilSurface;
	BOOL ImageCreate(_IMAGE *pImage, int nWidth, int nHeight);
	void ImageDestroy(_IMAGE *pImage);
	BOOL InitLayeredTexture(D3DFORMAT format, D3DFORMAT depthStencil);
	void CopyLayeredTextureImage();
	void RedrawLayeredWindow16();
#endif

	void LoadInterfaceFromFile( LPTSTR pszFilePath );

	void FillArea( int iX, int iY, int iWidth, int iHeight, D3DCOLOR d3dColor );
	void DrawLine( int iStartX, int iStartY, int iEndX, int iEndY, int iWidth, D3DCOLOR d3dColor );
	void DrawOutlinedBox( int iX, int iY, int iWidth, int iHeight, D3DCOLOR d3dInnerColor, D3DCOLOR d3dBorderColor );

	C_DX9_WINDOW *AddWindow( C_DX9_WINDOW * pWindow );
	void BringToTop( C_DX9_WINDOW * pWindow );
	C_DX9_WINDOW *GetTopWindow();
	void CloseTopWindow();

	void BeginScene();
	void EndScene();
	void Clear(DWORD Flags, D3DCOLOR Color, float Z = 1.0f, DWORD dwStencil = NULL);
	void Present(LPRECT pSrc = NULL, LPRECT pDst = NULL, HWND hDestWindowOverride = NULL, RGNDATA *pDirtyRegion = NULL);

	void MouseMove();
	BOOL KeyEvent( _KEY sKey );

	void OnLostDevice();
	void OnResetDevice( IDirect3DDevice9 * pDevice );

	//C_DX9_MOUSE * GetMouse();
	//CKeyboard * GetKeyboard();

	LPDIRECT3DDEVICE9 GetDevice();
	C_DX9_FONT * GetFont();
	ID3DXSprite * GetSprite();

	C_DX9_WINDOW *GetWindowByString( std::_tstring sString, int iIndex = 0 );
	C_DX9_WINDOW *GetWindowByName( std::_tstring sString);
	
	_OBJECT_BASE * GetThemeElement( std::_tstring sElement );

	C_DX9_WINDOW *GetMainWindow() { return(pMainWindow); }
	LPRECT GetMainTitle() { return(&rcMainTitle); }

	HWND GetMainWnd() { return(hMainWnd); }
	BOOL GetWindowMode() { return(bWindowMode); }
	void SetWindowPoint(long x,long y)
	{
		ptMainWindow.x = x; ptMainWindow.y = y;
	}
	POINT GetWindowPos() { return(ptMainWindow); }
	void SetMoveWindow(BOOL b) { bMoveWindow = b; }
	BOOL GetMoveWindow() { return(bMoveWindow); }
	LPPOINT GetMovePos() { return(&ptMoveWindow); }

	void SetVisible( BOOL bVisible );
	BOOL IsVisible();

	BOOL ShouldReload();
	void Reload();

	std::map<std::_tstring,C_DX9_VARIANT*> Cvars;
};

//extern C_DX9_ENGINE * DDRENGINE;

#define DXENGINE	C_DX9_ENGINE::GetInstance