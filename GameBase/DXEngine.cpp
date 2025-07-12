#include "stdafx.h"
#include "DXEngine.h"
#include "DXHorizontalSliderBar.h"


//C_DX_ENGINE * DDRENGINE = 0;

C_DX_ENGINE::C_DX_ENGINE() : bWindowMode(FALSE), bMoveWindow(FALSE), bytAlphaBlend(255)
{

}

C_DX_ENGINE::~C_DX_ENGINE()
{
	for( std::map<_tstring,C_DX_VARIANT*>::iterator iIter = Cvars.begin(); iIter != Cvars.end(); iIter++ )
		SAFE_DELETE( iIter->second )

		SAFE_DELETE( pFont )
		//SAFE_DELETE( m_pMouse )
		//SAFE_DELETE( m_pKeyboard )

		SAFE_RELEASE( pSprite )

		SAFE_RELEASE( pLine )

		for( size_t i = 0; i < vDXWindows.size(); i++ )
		{
			SAFE_DELETE( vDXWindows[i] )
		}
}

BOOL C_DX_ENGINE::Init(HWND hWnd, long nWidth, long nHeight, BOOL bWinMode )
{
	//if( !pDevice )
	//	MessageBoxA( 0, "pDevice invalid.", 0, 0 );

	hMainWnd = hWnd;

	SetRect(&rcMainWindow, 0, 0, nWidth, nHeight);
	SetRect(&rcMainTitle, 0, 0, rcMainWindow.right - (24 + 4), 30);


	bWindowMode = bWinMode;

	// Create the D3D object.
	if (NULL == (pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		//DBGPRINT(_T("Direct3DCreate9 실패"));
		return(FALSE);
	}

	D3DPRESENT_PARAMETERS d3dpp = { 0 };
	d3dpp.SwapEffect = bWindowMode ? D3DSWAPEFFECT_DISCARD : D3DSWAPEFFECT_FLIP;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

	d3dpp.hDeviceWindow = hMainWnd;  //This is our main (and only) window 
	d3dpp.BackBufferCount = 1;  //We only need a single back buffer 
	d3dpp.BackBufferWidth = rcMainWindow.right;
	d3dpp.BackBufferHeight = rcMainWindow.bottom;

	if (FALSE != bWindowMode)
	{
		d3dpp.Windowed = TRUE;

		D3DDISPLAYMODE d3ddm;
		if (FAILED(pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		{
			//DBGPRINT(_T("GetAdapterDisplayMode실패"));
			return(FALSE);
		}
		d3dpp.BackBufferFormat = d3ddm.Format;

	}
	else
	{
		d3dpp.Windowed = FALSE;
		d3dpp.FullScreen_RefreshRateInHz = 60;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;//D3DFMT_R5G6B5;//D3DFMT_R5G6B5 D3DFMT_X1R5G5B5
		//d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; 		
	}
	/*
	if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hMainWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &d3dpp, &pDevice)))
	{
	return(FALSE);
	}
	*/
	// Create the D3DDevice
	if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hMainWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice)))
	{
		if (FALSE == bWindowMode)
		{
			d3dpp.FullScreen_RefreshRateInHz = 0;
		}
		if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hMainWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice)))
		{

			if (FALSE == bWindowMode)
			{
				d3dpp.FullScreen_RefreshRateInHz = 60;
				d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;//D3DFMT_X1R5G5B5 D3DFMT_R5G6B5 D3DFMT_X1R5G5B5
			}
			if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hMainWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice)))
			{
				if (FALSE == bWindowMode)
				{
					d3dpp.FullScreen_RefreshRateInHz = 0;
					d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;//D3DFMT_X1R5G5B5 D3DFMT_R5G6B5 D3DFMT_X1R5G5B5
				}
				if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hMainWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice)))
				{
					//DBGPRINT(_T("CreateDevice 실패"));
					return(FALSE);
				}
			}
		}
	}
#if defined(LAYERED_WINDOW)
	bInitLayeredWindow = InitLayeredTexture(D3DFMT_A8R8G8B8, D3DFMT_D24S8);
	if (FALSE != ImageCreate(&imgBack, rcMainWindow.right, rcMainWindow.bottom))
	{

	}
#endif
	// Turn off culling
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Turn off D3D lighting
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Turn on the zbuffer
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	if (FAILED(D3DXCreateSprite(pDevice, &pSprite)))
		return(FALSE);

	if (FAILED(D3DXCreateLine(pDevice, &pLine)))
		return(FALSE);

	//D3DXCreateSprite( pDevice, &pSprite );
	//D3DXCreateLine( pDevice, &pLine );


	pFont = 0;

	Cvars[ _T("$Value") ] = new C_DX_VARIANT( new C_SET_SLIDER_VALUE() );
	Cvars[ _T("$MaxValue") ] = new C_DX_VARIANT( new C_SET_MAX_VALUE() );
	Cvars[ _T("$MinValue") ] = new C_DX_VARIANT( new C_SET_MIN_VALUE() );


	SetVisible( FALSE );
	m_bReload = FALSE;
	return(TRUE);
}

void C_DX_ENGINE::Create()
{
	pMainWindow = new C_DX_WINDOW(rcMainWindow.right, rcMainWindow.bottom, rcMainTitle.bottom);
}

void C_DX_ENGINE::Calculate()
{
	if (FALSE != GetMoveWindow())
	{
		//if (FALSE != DXMOUSE()->GetLeftButton())
		if (FALSE != KEY_DOWN(VK_LBUTTON))
		{
			POINT Pos;
			::GetCursorPos(&Pos);

			ptMainWindow.x = ptMainWindow.x + (Pos.x - ptMoveWindow.x);
			ptMainWindow.y = ptMainWindow.y + (Pos.y - ptMoveWindow.y);

			ptMoveWindow = Pos;

			::SetWindowPos(GetMainWnd(), NULL, ptMainWindow.x, ptMainWindow.y, 0, 0, SWP_NOSIZE);
		}
		else
		{
			SetMoveWindow(FALSE);
		}
	}
	pMainWindow->Calculate();
	for( int iIndex = static_cast<int>( vDXWindows.size() ) - 1; iIndex >= 0; iIndex-- )
	{
		if(FALSE != vDXWindows[ iIndex ]->GetVisible() )
		{
			vDXWindows[ iIndex ]->Calculate();
		}
	}
}

void C_DX_ENGINE::DisplayMainTitle()
{
	pMainWindow->DisplayTitlebar();
}

void C_DX_ENGINE::DisplayMainBoard()
{
	pMainWindow->DisplayBoard();
}

void C_DX_ENGINE::Display()
{
	if( IsVisible() )
	{
		for( int iIndex = 0; iIndex < static_cast<int>( vDXWindows.size() ); iIndex++ )
		{
			if( FALSE != vDXWindows[ iIndex ]->GetVisible() )
			{
				vDXWindows[ iIndex ]->Display();
			}
		}
		DXMOUSE()->Display();	
	}
}

#if defined(LAYERED_WINDOW)
BOOL C_DX_ENGINE::ImageCreate(_IMAGE *pImage, int nWidth, int nHeight)
{
	BOOL bResult = FALSE;
	if (pImage)
	{
		pImage->nWidth = nWidth;
		pImage->nHeight = nHeight;
		pImage->nPitch = ((nWidth * 32 + 31) & ~31) >> 3;
		pImage->pPixels = NULL;

		pImage->hdc = CreateCompatibleDC(NULL);
		if (pImage->hdc)
		{
			ZeroMemory(&pImage->info, sizeof(pImage->info));
			pImage->info.bmiHeader.biSize = sizeof(pImage->info.bmiHeader);
			pImage->info.bmiHeader.biBitCount = 32;
			pImage->info.bmiHeader.biWidth = nWidth;
			pImage->info.bmiHeader.biHeight = -nHeight;
			pImage->info.bmiHeader.biCompression = BI_RGB;
			pImage->info.bmiHeader.biPlanes = 1;
			pImage->hBitmap = CreateDIBSection(pImage->hdc, &pImage->info, DIB_RGB_COLORS, (LPVOID*)&pImage->pPixels, NULL, 0);

			if (pImage->hBitmap)
			{
				//DBGPRINT(_T("이미지 생성 ㅇㅋ"));
				GdiFlush();
				bResult = TRUE;
			}
			else
			{
				ImageDestroy(pImage);
			}
		}
	}
	return bResult;
}

void C_DX_ENGINE::ImageDestroy(_IMAGE *pImage)
{
	if (pImage)
	{
		pImage->nWidth = 0;
		pImage->nHeight = 0;
		pImage->nPitch = 0;
		if (pImage->hBitmap)
		{
			DeleteObject(pImage->hBitmap);
			pImage->hBitmap = 0;
		}
		if (pImage->hdc)
		{
			DeleteDC(pImage->hdc);
			pImage->hdc = 0;
		}
		memset(&pImage->info, 0, sizeof(pImage->info));
		pImage->pPixels = 0;
	}
}

BOOL C_DX_ENGINE::InitLayeredTexture(D3DFORMAT format, D3DFORMAT depthStencil)
{
	BOOL bResult = FALSE;
	HRESULT hResult = D3DXCreateTexture(pDevice, rcMainWindow.right, rcMainWindow.bottom, 0, D3DUSAGE_RENDERTARGET, format, D3DPOOL_DEFAULT, &pLayeredTexture);
	if (SUCCEEDED(hResult))	// 만약 실패라면
	{
		hResult = pLayeredTexture->GetSurfaceLevel(0, &pRenderTargetSurface);
	}
	if (SUCCEEDED(hResult))	// 이걸 안한다
	{
		hResult = pDevice->CreateDepthStencilSurface(rcMainWindow.right, rcMainWindow.bottom, depthStencil, D3DMULTISAMPLE_NONE, 0, TRUE, &pDepthStencilSurface, 0);
	}
	if (SUCCEEDED(hResult)) // 맨위에꺼 실패면 또 안한다
	{
		hResult = pDevice->CreateOffscreenPlainSurface(rcMainWindow.right, rcMainWindow.bottom, format, D3DPOOL_SYSTEMMEM, &pSurface, 0);
		bResult = TRUE;
	}
	return bResult;
}

void C_DX_ENGINE::CopyLayeredTextureImage()
{
	HRESULT hResult = 0;
	D3DLOCKED_RECT rcLock = { 0 };
	hResult = pDevice->GetRenderTargetData(pRenderTargetSurface, pSurface);
	if (SUCCEEDED(hResult))
	{
		hResult = pSurface->LockRect(&rcLock, 0, 0);
		if (SUCCEEDED(hResult))
		{
			const BYTE *pSrc = (const BYTE *)rcLock.pBits;
			BYTE *pDest = imgBack.pPixels;
			int srcPitch = rcLock.Pitch;
			int destPitch = imgBack.nPitch;

			if (srcPitch == destPitch)
			{
				memcpy(pDest, pSrc, destPitch * imgBack.nHeight);
			}
			else
			{
				for (int i = 0; i < imgBack.nHeight; ++i)
				{
					memcpy(&pDest[destPitch * i], &pSrc[srcPitch * i], destPitch);
				}
			}
			pSurface->UnlockRect();
		}
	}
}

void C_DX_ENGINE::RedrawLayeredWindow16()
{
	HDC hDC = ::GetDC(hMainWnd);
	if (0 != hDC)
	{
		HGDIOBJ hPrevObj = 0;
		POINT ptDest = { 0, 0 };
		POINT ptSrc = { 0, 0 };
		SIZE sizeClient = { imgBack.nWidth, imgBack.nHeight };
		BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, bytAlphaBlend, AC_SRC_ALPHA };

		hPrevObj = ::SelectObject(imgBack.hdc, imgBack.hBitmap);
		::ClientToScreen(hMainWnd, &ptDest);
		//DBGPRINT("ptDest: %i / %i", ptDest.x, ptDest.y);

		UpdateLayeredWindow(hMainWnd, hDC, &ptDest, &sizeClient, imgBack.hdc, &ptSrc, 0, &blendFunc, ULW_ALPHA);
		::SelectObject(imgBack.hdc, hPrevObj);
		::ReleaseDC(hMainWnd, hDC);
	}
}
#endif

void C_DX_ENGINE::LoadInterfaceFromFile( LPTSTR pszFilePath )
{
	C_TINY_XML_DOCUMENT Document;

	if( !Document.LoadFile( pszFilePath ) )
	{
		MessageBox( 0, Document.ErrorDesc(), _T("XML Error"), 0 );
		return;
	}

	C_TINY_XML_HANDLE hDoc( &Document );

	C_TINY_XML_ELEMENT * pGUI = hDoc.FirstChildElement( _T("GUI") ).Element();
	if( pGUI )
	{
		C_TINY_XML_ELEMENT * pFontElement = pGUI->FirstChildElement( _T("Font") );
		if( pFontElement )
		{
			int iSize = 0;
			pFontElement->QueryIntAttribute( _T("size"), &iSize );

			LPCTSTR pszFace = pFontElement->Attribute( _T("face") );
			pFont = new C_DX_FONT( GetDevice(), iSize, pszFace?pszFace:_T("Tahoma") );

			for( C_TINY_XML_ELEMENT * pColorElement = pFontElement->FirstChildElement( _T("Color") ); pColorElement; pColorElement = pColorElement->NextSiblingElement( _T("Color") ) )
			{
				int iColorIndex = 0;
				pColorElement->QueryIntAttribute( _T("index"), &iColorIndex );

				int iColors[4] = { 0 };
				pColorElement->QueryIntAttribute( _T("r"), &iColors[0] );
				pColorElement->QueryIntAttribute( _T("g"), &iColors[1] );
				pColorElement->QueryIntAttribute( _T("b"), &iColors[2] );
				pColorElement->QueryIntAttribute( _T("a"), &iColors[3] );

				pFont->SetColor( iColorIndex, C_DARK_COLOR( iColors[0], iColors[1], iColors[2], iColors[3] ) );

				//pFont->SetColor( iColorIndex, C_DARK_COLOR( pColorElement ) );
			}
		}

		C_TINY_XML_ELEMENT * pCvars = pGUI->FirstChildElement( _T("Cvars") );
		if( pCvars )
		{
			for( C_TINY_XML_ELEMENT * pCvarElement = pCvars->FirstChildElement(); pCvarElement; pCvarElement = pCvarElement->NextSiblingElement() )
			{
				LPCTSTR pszType = pCvarElement->Attribute( _T("type") );
				if( pszType == 0 || !lstrcmpi( pszType, _T("number") ) )
				{
					double dValue = 0.0;
					pCvarElement->QueryDoubleAttribute( _T("value"), &dValue );
					Cvars[ pCvarElement->Attribute( _T("string") ) ] = new C_DX_VARIANT( dValue );
				}
				else if( !lstrcmpi( pszType ,_T("string") ) )
				{
					LPCTSTR pszValue = pCvarElement->Attribute( _T("value") );
					Cvars[ pCvarElement->Attribute( _T("string") ) ] = new C_DX_VARIANT( _tstring( pszValue ) );
				}
				else if( !lstrcmpi( pszType, _T("BOOL") ) )
				{
					LPCTSTR pszValue = pCvarElement->Attribute( _T("value") );

					BOOL bValue = FALSE;
					if( !lstrcmpi( pszValue, _T("TRUE") ) )
						bValue = TRUE;

					Cvars[ pCvarElement->Attribute( _T("string") ) ] = new C_DX_VARIANT( bValue );
				}
			}
		}

		C_TINY_XML_ELEMENT * pColorThemes = pGUI->FirstChildElement( _T("ColorThemes") );
		if( pColorThemes )
		{
			LPCTSTR pszDefaultTheme = pColorThemes->Attribute( _T("default") );
			if( pszDefaultTheme )
			{
				// <ColorThemes default="Standard">
				tstrThemeName = pszDefaultTheme;
			}
			for( C_TINY_XML_ELEMENT * pThemeElement = pColorThemes->FirstChildElement(); pThemeElement; pThemeElement = pThemeElement->NextSiblingElement() )
			{
				//////////////////////////////////////////////////////////////////////////
				// 아래의 반복문에서 모든 오브젝트 텍스쳐 정보를 로드한다.

				for( C_TINY_XML_ELEMENT * pElementElement = pThemeElement->FirstChildElement(); pElementElement; pElementElement = pElementElement->NextSiblingElement() )
				{
					// <Window default="Normal"> , <CrossButton default="Normal"> , <Button default="Normal">
					_OBJECT_BASE * _CurrentElement = new _OBJECT_BASE();

					LPCTSTR pszDefault = pElementElement->Attribute( _T("default") );
					if( pszDefault )
					{
						_CurrentElement->tstrDefaultState = _tstring( pszDefault );
					}
					for( C_TINY_XML_ELEMENT * pStateElement = pElementElement->FirstChildElement( _T("State") ); pStateElement; pStateElement = pStateElement->NextSiblingElement( _T("State") ) )
					{
						// <State string="Normal">
						LPCTSTR pszString = pStateElement->Attribute( _T("string") );

						if( !pszString )
							continue;

						_OBJECT_STATE * pObjectState = _CurrentElement->m_mStates[ pszString ] = new _OBJECT_STATE();

						pObjectState->pParent = _CurrentElement;

						for( C_TINY_XML_ELEMENT * pColorElement = pStateElement->FirstChildElement( _T("Color") ); pColorElement; pColorElement = pColorElement->NextSiblingElement( _T("Color") ) )
						{
							// <Color string="Title" r="255" g="255" b="255" a="255"/>
							pszString = pColorElement->Attribute( _T("string") );

							if( !pszString )
								continue;

							int iColors[4] = { 0 };
							pColorElement->QueryIntAttribute( _T("r"), &iColors[0] );
							pColorElement->QueryIntAttribute( _T("g"), &iColors[1] );
							pColorElement->QueryIntAttribute( _T("b"), &iColors[2] );
							pColorElement->QueryIntAttribute( _T("a"), &iColors[3] );

							pObjectState->mColors[ pszString ] = new C_DARK_COLOR( iColors[0], iColors[1], iColors[2], iColors[3] );

							//pObjectState->mColors[ pszString ] = new C_DARK_COLOR( pColorElement );
						}
						for( C_TINY_XML_ELEMENT * pTextureElement = pStateElement->FirstChildElement( _T("Texture") ); pTextureElement; pTextureElement = pTextureElement->NextSiblingElement( _T("Texture") ) )
						{
							// <Texture string="Titlebar" path="HorizontalBar_Norm.bmp"/>
							_tstringstream sStream;

							sStream << pThemeElement->Value() << _T("/") << pTextureElement->Attribute( _T("path") );

							pObjectState->mTextures[ pTextureElement->Attribute( _T("string") ) ] = new C_DX_TEXTURE( sStream.str().c_str() );
						}

						mThemes[ pThemeElement->Value() ][ pElementElement->Value() ] = _CurrentElement;
					}
				}
			}
		}

		C_TINY_XML_ELEMENT * pWindows = pGUI->FirstChildElement( _T("Windows") );
		if( pWindows )
		{
			for( C_TINY_XML_ELEMENT * pWindowElement = pWindows->FirstChildElement(); pWindowElement; pWindowElement = pWindowElement->NextSiblingElement() )
			{
				AddWindow( new C_DX_WINDOW( pWindowElement ) );
			}
		}
	}
}

void C_DX_ENGINE::FillArea( int iX, int iY, int iWidth, int iHeight, D3DCOLOR d3dColor )
{
	DrawLine( iX + iWidth / 2, iY, iX + iWidth / 2, iY + iHeight, iWidth, d3dColor );
}

void C_DX_ENGINE::DrawLine( int iStartX, int iStartY, int iEndX, int iEndY, int iWidth, D3DCOLOR d3dColor )
{
	pLine->SetWidth( static_cast<float>( iWidth ) );

	D3DXVECTOR2 d3dxVector[2];

	d3dxVector[0] = D3DXVECTOR2( static_cast<float>( iStartX ), static_cast<float>( iStartY ) );
	d3dxVector[1] = D3DXVECTOR2( static_cast<float>( iEndX ), static_cast<float>( iEndY ) );

	pLine->Begin();
	pLine->Draw( d3dxVector, 2, d3dColor );
	pLine->End();
}

void C_DX_ENGINE::DrawOutlinedBox( int iX, int iY, int iWidth, int iHeight, D3DCOLOR d3dInnerColor, D3DCOLOR d3dBorderColor )
{
	FillArea( iX + 1, iY + 1, iWidth - 2,  iHeight - 2, d3dInnerColor );

	DrawLine( iX,				iY,					iX,					iY + iHeight,		1, d3dBorderColor );
	DrawLine( iX + 1,			iY,					iX + iWidth - 1,	iY,					1, d3dBorderColor );
	DrawLine( iX + 1,			iY + iHeight - 1,	iX + iWidth - 1,	iY + iHeight - 1,	1, d3dBorderColor );
	DrawLine( iX + iWidth - 1,	iY,					iX + iWidth - 1,	iY + iHeight,		1, d3dBorderColor );
}

C_DX_WINDOW *C_DX_ENGINE::AddWindow( C_DX_WINDOW * pWindow ) 
{
	vDXWindows.push_back( pWindow );
	return vDXWindows.back();
}

void C_DX_ENGINE::BringToTop( C_DX_WINDOW * pWindow )
{
	for( int i = 0; i < static_cast<int>( vDXWindows.size() ); i++ )
	{
		if( vDXWindows[i] == pWindow )
		{
			vDXWindows.erase( vDXWindows.begin() + i );
		}
	}
	vDXWindows.insert(vDXWindows.end(), pWindow );
}

C_DX_WINDOW *C_DX_ENGINE::GetTopWindow()
{
	return vDXWindows.back();
}

void C_DX_ENGINE::CloseTopWindow()
{
	vDXWindows.back()->SetVisible(FALSE);
}

void C_DX_ENGINE::BeginScene()
{
	pDevice->BeginScene();
}

void C_DX_ENGINE::EndScene()
{
	pDevice->EndScene();
}

void C_DX_ENGINE::Clear(DWORD Flags, D3DCOLOR Color, float Z, DWORD dwStencil)
{
#if defined(LAYERED_WINDOW)
	pDevice->SetDepthStencilSurface(pDepthStencilSurface);
	pDevice->SetRenderTarget(0, pRenderTargetSurface);
#endif
	pDevice->Clear(0, NULL, Flags, Color, Z, dwStencil);
}

void C_DX_ENGINE::Present(LPRECT pSrc, LPRECT pDst, HWND hDestWindowOverride, RGNDATA *pDirtyRegion)
{
#if defined(LAYERED_WINDOW)
	CopyLayeredTextureImage();
	RedrawLayeredWindow16();
#endif
	//////////////////////////////////////////////////////////////////////////
	// 후면 버퍼를 보이는 버퍼로, 호출하면 fps가 60정도로 된다.

	pDevice->Present(pSrc, pDst, hDestWindowOverride, pDirtyRegion);
}

void C_DX_ENGINE::MouseMove()
{
	pMainWindow->MouseMove();
	C_DX_OBJECT_BASE * pDragging = DXMOUSE()->GetDragging();
	if( !pDragging )
	{
		BOOL bGotWindow = FALSE;
		for( int iIndex = static_cast<int>( vDXWindows.size() ) - 1; iIndex >= 0; iIndex-- )
		{
			if(vDXWindows[ iIndex ]->GetVisible())
			{
				int iHeight = 0;
				if( !vDXWindows[ iIndex ]->GetMaximumWindow() )
				{
					iHeight = vDXWindows[iIndex]->GetTitlebarSize();
				}
				if( !bGotWindow && DXMOUSE()->InArea( vDXWindows[ iIndex ], iHeight ) )
				{
					vDXWindows[ iIndex ]->MouseMove( );
					bGotWindow = TRUE;
				}
				else
				{
					DXMOUSE()->SavePos();
					DXMOUSE()->SetPos( -1, -1 );
					vDXWindows[ iIndex ]->MouseMove( );
					DXMOUSE()->LoadPos();
				}
			}
		}
	}
	else
	{
		pDragging->MouseMove();
	}
}

BOOL C_DX_ENGINE::KeyEvent( _KEY sKey )
{
	BOOL bTop = FALSE;
	pMainWindow->KeyEvent(sKey);
	if( !sKey.cKey && ( sKey.bDown || ( DXMOUSE()->GetWheel() && !sKey.bDown ) ) )
	{
		std::vector<C_DX_WINDOW*> vRepeat;

		for( int iIndex = static_cast<int>( vDXWindows.size() ) - 1; iIndex >= 0; iIndex-- )
		{
			if( !vDXWindows[ iIndex ]->GetVisible() )
				continue;

			if( !bTop )
			{
				int iHeight = 0;

				if( !vDXWindows[ iIndex ]->GetMaximumWindow() )
				{
					iHeight = vDXWindows[iIndex]->GetTitlebarSize();
				}

				if( DXMOUSE()->InArea( vDXWindows[ iIndex ], iHeight ) && !bTop )
				{
					vDXWindows[ iIndex ]->KeyEvent( sKey );
					bTop = TRUE;
				}
				else
				{
					vRepeat.push_back( vDXWindows[ iIndex ] );
				}
			}
			else
			{
				DXMOUSE()->SavePos();
				DXMOUSE()->SetPos( C_DARK_POINT( -1, -1 ) );
				vDXWindows[ iIndex ]->KeyEvent( sKey );
				DXMOUSE()->LoadPos();
			}
		}

		for( int iIndex = 0; iIndex < static_cast<int>( vRepeat.size() ); iIndex++ )
		{
			DXMOUSE()->SavePos();
			DXMOUSE()->SetPos( C_DARK_POINT( -1, -1 ) );
			vRepeat[ iIndex ]->KeyEvent( sKey );
			DXMOUSE()->LoadPos();
		}
	}
	else
	{
		bTop = FALSE;

		for( int iIndex = static_cast<int>( vDXWindows.size() ) - 1; iIndex >= 0; iIndex-- )
		{
			if( vDXWindows[ iIndex ]->GetVisible() )
			{
				if( vDXWindows[ iIndex ]->GetFocussedObject() && vDXWindows[ iIndex ]->GetMaximumWindow() )
				{
					bTop = TRUE;
				}
				vDXWindows[ iIndex ]->KeyEvent( sKey );
			}
		}
		if( !sKey.bDown )
		{
			bTop = FALSE;
		}
	}

	return bTop;
}

void C_DX_ENGINE::OnLostDevice()
{
	pDevice = 0;

	if( GetFont() )
	{
		GetFont()->OnLostDevice();
	}
	GetSprite()->OnLostDevice();

	pLine->OnLostDevice();
}

void C_DX_ENGINE::OnResetDevice( IDirect3DDevice9 * pDevice )
{
	pDevice = pDevice;

	if( GetFont() )
		GetFont()->OnResetDevice();

	GetSprite()->OnResetDevice();

	pLine->OnResetDevice();
}
/*
C_DX_MOUSE * C_DX_ENGINE::GetMouse()
{
return m_pMouse;
}

CKeyboard * C_DX_ENGINE::GetKeyboard()
{
return m_pKeyboard;
}
*/
LPDIRECT3DDEVICE9 C_DX_ENGINE::GetDevice()
{
	return pDevice;
}

C_DX_FONT * C_DX_ENGINE::GetFont()
{
	return pFont;
}

ID3DXSprite * C_DX_ENGINE::GetSprite()
{
	return pSprite;
}

C_DX_WINDOW * C_DX_ENGINE::GetWindowByString( _tstring sString, int iIndex )
{
	for( int i = 0; i < static_cast<int>( vDXWindows.size() ); i++ )
		if( vDXWindows[ i ]->GetString( FALSE, iIndex ) == sString )
			return vDXWindows[ i ];
	return 0;
}

C_DX_WINDOW *C_DX_ENGINE::GetWindowByName( _tstring sString)
{
	for( int i = 0; i < static_cast<int>( vDXWindows.size() ); i++ )
	{
		if( vDXWindows[i]->GetObjectName() == sString )
		{
			return vDXWindows[i];
		}
	}
	return 0;
}

_OBJECT_BASE * C_DX_ENGINE::GetThemeElement( _tstring sElement )
{
	return mThemes[ tstrThemeName ][ sElement ];
}

void C_DX_ENGINE::SetVisible( BOOL b )
{
	bVisible = b;
}

BOOL C_DX_ENGINE::IsVisible()
{
	return bVisible;
}

BOOL C_DX_ENGINE::ShouldReload()
{
	return m_bReload;
}

void C_DX_ENGINE::Reload()
{
	m_bReload = TRUE;
}