#pragma once


#include "DXDef.h"
#include "DXFont.h"
#include "DXTexture.h"
#include "DXObjectBase.h"
#include "DXWindow.h"
#include "DXVariant.h"



class C_DX_ENGINE
{
private:
	C_DX_ENGINE();
	~C_DX_ENGINE();

	BOOL bVisible, bWindowMode, m_bReload, bMoveWindow;

	HWND hMainWnd;

	C_DX_FONT * pFont;

	LPDIRECT3D9			pDirect3D;		// 다이렉트 3D의 객체
	LPDIRECT3DDEVICE9	pDevice;		// 3D의 디바이스 장치
	LPD3DXSPRITE		pSprite;
	LPD3DXLINE			pLine;
	LPDIRECT3DVERTEXBUFFER9		pVertexBuffer;

	BYTE bytAlphaBlend;

	C_DX_WINDOW *pMainWindow;
	RECT rcMainWindow, rcMainTitle;
	POINT ptMainWindow, ptMoveWindow;

	// 팝업 관리.
	std::vector<C_DX_WINDOW *> vDXWindows;

	_tstring tstrThemeName;

	typedef std::map<_tstring, _OBJECT_BASE*> tTheme;
	std::map<_tstring, tTheme> mThemes;

public:
	static C_DX_ENGINE *GetInstance()
	{
		static C_DX_ENGINE *pDXEngine = 0;
		if (0 == pDXEngine)
		{
			pDXEngine = new C_DX_ENGINE();
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

	C_DX_WINDOW *AddWindow( C_DX_WINDOW * pWindow );
	void BringToTop( C_DX_WINDOW * pWindow );
	C_DX_WINDOW *GetTopWindow();
	void CloseTopWindow();

	void BeginScene();
	void EndScene();
	void Clear(DWORD Flags, D3DCOLOR Color, float Z = 1.0f, DWORD dwStencil = NULL);
	void Present(LPRECT pSrc = NULL, LPRECT pDst = NULL, HWND hDestWindowOverride = NULL, RGNDATA *pDirtyRegion = NULL);

	void MouseMove();
	BOOL KeyEvent( _KEY sKey );

	void OnLostDevice();
	void OnResetDevice( IDirect3DDevice9 * pDevice );

	//C_DX_MOUSE * GetMouse();
	//CKeyboard * GetKeyboard();

	LPDIRECT3DDEVICE9 GetDevice();
	C_DX_FONT * GetFont();
	ID3DXSprite * GetSprite();

	C_DX_WINDOW *GetWindowByString( _tstring sString, int iIndex = 0 );
	C_DX_WINDOW *GetWindowByName( _tstring sString);
	
	_OBJECT_BASE * GetThemeElement( _tstring sElement );

	C_DX_WINDOW *GetMainWindow() { return(pMainWindow); }
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

	std::map<_tstring,C_DX_VARIANT*> Cvars;
};

//extern C_DX_ENGINE * DDRENGINE;

#define DXENGINE	C_DX_ENGINE::GetInstance