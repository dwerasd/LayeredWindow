#pragma once


#include "DXDef.h"



class C_DX_TEXTURE
{
private:
	LPDIRECT3DTEXTURE9 pTexture;
	D3DSURFACE_DESC TexDesc;

public:
	~C_DX_TEXTURE();
	C_DX_TEXTURE( LPCTSTR szPath );

	IDirect3DTexture9 * GetTexture()
	{
		return pTexture;
	}

	void Display( C_DARK_POINT cpPos, int iWidth, int iHeight );
};


struct _OBJECT_BASE;

struct _OBJECT_STATE
{
	_OBJECT_BASE * pParent;

	C_DARK_COLOR * GetColor( _tstring sString );
	C_DX_TEXTURE * GetTexture( _tstring sString );

	std::map<_tstring, C_DARK_COLOR*> mColors;
	std::map<_tstring, C_DX_TEXTURE*> mTextures;
};

struct _OBJECT_BASE
{
	_tstring tstrDefaultState;
	std::map<_tstring, _OBJECT_STATE *> m_mStates;
};