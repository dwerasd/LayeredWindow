#include "stdafx.h"
#include "DXTexture.h"
#include "DXEngine.h"



C_DX_TEXTURE::C_DX_TEXTURE( LPCTSTR szPath )
{
	D3DXCreateTextureFromFile( DXENGINE()->GetDevice(), szPath, &pTexture );

	if( !pTexture )
	{
		_tstringstream sStream;
		sStream << _T("Failed to load texture: ") << szPath;
		MessageBox( 0, sStream.str().c_str(), _T("C_DX_TEXTURE::C_DX_TEXTURE( ... )"), 0 );
		return;
	}

	pTexture->GetLevelDesc( 0, &TexDesc );
	
}

C_DX_TEXTURE::~C_DX_TEXTURE()
{
	SAFE_RELEASE( pTexture );
}

void C_DX_TEXTURE::Display( C_DARK_POINT cpPos, int iWidth, int iHeight )
{
	D3DXVECTOR2 vTransformation = D3DXVECTOR2( static_cast<float>( cpPos.GetX() ), static_cast<float>( cpPos.GetY() ) );
	D3DXVECTOR2 vScaling( ( 1.0f / TexDesc.Width ) * iWidth, ( 1.0f / TexDesc.Height ) * iHeight );

	D3DXMATRIX mainMatrix;
	D3DXMatrixTransformation2D( &mainMatrix, 0, 0, &vScaling, 0, 0, &vTransformation );
	
	DXENGINE()->GetSprite()->SetTransform( &mainMatrix );
	DXENGINE()->GetSprite()->Begin( D3DXSPRITE_ALPHABLEND );
	DXENGINE()->GetSprite()->Draw( pTexture, 0, 0, 0, 0xFFFFFFFF );
	DXENGINE()->GetSprite()->End();
}


C_DARK_COLOR * _OBJECT_STATE::GetColor( _tstring sString )
{
	C_DARK_COLOR * pRet = mColors[ sString ];

	if( !pRet )
		pRet = pParent->m_mStates[ pParent->tstrDefaultState ]->mColors[ sString ];

	if( !pRet )
		MessageBox( 0, _T("Color not found."), sString.c_str(), 0 );

	return pRet;
}

C_DX_TEXTURE * _OBJECT_STATE::GetTexture( _tstring sString )
{
	C_DX_TEXTURE * pRet = mTextures[ sString ];

	if( !pRet )
		pRet = pParent->m_mStates[ pParent->tstrDefaultState ]->mTextures[ sString ];

	return pRet;
}
