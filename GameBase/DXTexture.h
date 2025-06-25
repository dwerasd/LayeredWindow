#pragma once


#include "DXDef.h"



class C_DX9_TEXTURE
{
private:
	LPDIRECT3DTEXTURE9 pTexture;
	D3DSURFACE_DESC TexDesc;

public:
	~C_DX9_TEXTURE();
	C_DX9_TEXTURE( LPCTSTR szPath );

	IDirect3DTexture9 * GetTexture()
	{
		return pTexture;
	}

	void Display( dk::DPOINT cpPos, int iWidth, int iHeight );
};


struct _OBJECT_BASE;

struct _OBJECT_STATE
{
	_OBJECT_BASE * pParent;

	dk::DCOLOR * GetColor( std::_tstring sString );
	C_DX9_TEXTURE * GetTexture( std::_tstring sString );

	std::map<std::_tstring, dk::DCOLOR*> mColors;
	std::map<std::_tstring, C_DX9_TEXTURE*> mTextures;
};

struct _OBJECT_BASE
{
	std::_tstring tstrDefaultState;
	std::map<std::_tstring, _OBJECT_STATE *> m_mStates;
};