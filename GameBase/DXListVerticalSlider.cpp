#include "stdafx.h"
#include "DXListVerticalSlider.h"
#include "DXEngine.h"



void C_DX_LIST_VERTICAL_SLIDER::SetDragged( BOOL b )
{
	m_bDragged = b;
}

BOOL C_DX_LIST_VERTICAL_SLIDER::GetDragged()
{
	return m_bDragged;
}

C_DX_LIST_VERTICAL_SLIDER::C_DX_LIST_VERTICAL_SLIDER( C_DARK_POINT relPos, int iHeight )
{
	SetRelPos( relPos );
	SetHeight( iHeight );

	bMouseOver[ 0 ] = bMouseOver[ 1 ] = bMouseOver[ 2 ] = FALSE;
	SetDragged( FALSE );

	SetThemeElement( DXENGINE()->GetThemeElement( _T("HelperSlider") ) );
	SetThemeElement( DXENGINE()->GetThemeElement( _T("HelperSlider") ), 2 );

	if( !GetThemeElement() )
		MessageBox( 0, _T("Theme element invalid."), _T("HelperSlider"), 0 );
	else
	{
		SetObjectState( _T("Normal") );
		SetObjectState( _T("Normal"), 2 );
	}

	SetThemeElement( DXENGINE()->GetThemeElement( _T("HorizontalSliderBar") ), 1 );

	if( !GetThemeElement( 1 ) )
		MessageBox( 0, _T("Theme element invalid."), _T("HorizontalSliderBar"), 0 );
	else
		SetObjectState( _T("Normal"), 1 );
}

void C_DX_LIST_VERTICAL_SLIDER::Display( C_DARK_POINT basePos )
{
	C_DARK_POINT Pos = basePos + *GetRelPos();

	DXENGINE()->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), LIST_SLIDER_WIDTH, GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

	pUpArrow->Display( C_DARK_POINT( Pos.GetX() + 1, Pos.GetY() + 1 ), LIST_SLIDER_WIDTH - 2, LIST_SLIDER_WIDTH - 2 );

	pDownArrow->Display( C_DARK_POINT( Pos.GetX() + 1, Pos.GetY() + 1 + GetHeight() - LIST_SLIDER_WIDTH ), LIST_SLIDER_WIDTH - 2, LIST_SLIDER_WIDTH - 2 );

	if( GetMaxValue() < 2 )
		return;

	pSlider->Display( C_DARK_POINT( Pos.GetX() + 1, Pos.GetY() - 1 + LIST_SLIDER_WIDTH + static_cast<int>( floor( static_cast<float>( ( GetHeight() - ( ( LIST_SLIDER_WIDTH - 2 ) * 3 ) ) ) / GetMaxValue() * GetValue() ) ) ), LIST_SLIDER_WIDTH - 2, LIST_SLIDER_WIDTH - 2 );
}

void C_DX_LIST_VERTICAL_SLIDER::Calculate(C_DARK_POINT basePos)
{
	C_DARK_POINT Pos = basePos + *GetRelPos();

	if( m_bPressed[ 0 ] && !m_tUpArrow.Running() )	// 눌렸고, 마우스 오버 상태라면?
	{
		if ( bMouseOver[ 0 ] )
		{
			if( GetValue() > GetMinValue() )
			{
				SetValue( GetValue() - 1 );
			}
			m_bPressed[ 0 ] = TRUE;
			SetObjectState( _T("Pressed") );
			m_tUpArrow.Start( 0.2f );
		}
		else
		{
			m_bPressed[ 0 ] = FALSE;
			SetObjectState( bMouseOver[ 0 ]?_T("MouseOver"):_T("Normal") );
		}
	}
	if( m_bPressed[ 1 ] && !m_tDownArrow.Running() )
	{
		if( bMouseOver[ 1 ] )
		{
			if( GetValue() < GetMaxValue() )
			{
				SetValue( GetValue() + 1 );
			}

			m_bPressed[ 1 ] = TRUE;
			SetObjectState( _T("Pressed"), 2 );

			m_tDownArrow.Start( 0.2f );
		}
		else
		{
			m_bPressed[ 1 ] = FALSE;
			SetObjectState( bMouseOver[ 1 ]?_T("MouseOver"):_T("Normal"), 2 );
		}
	}
	if ( !DXMOUSE()->GetLeftButton())
	{
		SetDragged( FALSE );
		SetObjectState( ( ( bMouseOver[ 2 ]	= DXMOUSE()->InArea( Pos.GetX(),  Pos.GetY() + LIST_SLIDER_WIDTH + static_cast<int>( floor( static_cast<float>( ( GetHeight() - ( LIST_SLIDER_WIDTH * 3 ) ) ) / GetMaxValue() * GetValue() ) ), LIST_SLIDER_WIDTH, LIST_SLIDER_WIDTH ) ) != 0 )?_T("MouseOver"):_T("Normal"), 1 );
	}
}

void C_DX_LIST_VERTICAL_SLIDER::MouseMove( C_DARK_POINT basePos )
{
	C_DARK_POINT Pos = basePos + *GetRelPos();
	SetObjectState( ( ( bMouseOver[ 0 ]	= DXMOUSE()->InArea( Pos.GetX() + 1, Pos.GetY() + 1, LIST_SLIDER_WIDTH - 2, LIST_SLIDER_WIDTH - 2 ) ) != 0 )?_T("MouseOver"):_T("Normal") );
	SetObjectState( ( ( bMouseOver[ 1 ]	= DXMOUSE()->InArea( Pos.GetX() + 1, Pos.GetY() + 1 + GetHeight() - LIST_SLIDER_WIDTH, LIST_SLIDER_WIDTH - 2, LIST_SLIDER_WIDTH - 2 ) ) != 0 )?_T("MouseOver"):_T("Normal"), 2 );

	
	if( GetMaxValue() < 2 )
	{
		return;
	}
	if( GetDragged() )
	{
		C_DARK_POINT mPos = DXMOUSE()->GetPos();

		if( mPos.GetX() == -1 && mPos.GetY() == -1 )
		{
			mPos = DXMOUSE()->GetSavedPos();
		}
		if( mPos.GetY() < Pos.GetY() + LIST_SLIDER_WIDTH )
		{
			SetValue( GetMinValue() );
		}
		else if( mPos.GetY() > Pos.GetY() + GetHeight() - LIST_SLIDER_WIDTH )
		{
			SetValue( GetMaxValue() );
		}
		else
		{
			for( int i = GetMinValue(); i <= GetMaxValue(); i++ )
			{
				int iY = Pos.GetY() - 1 + LIST_SLIDER_WIDTH + static_cast<int>( floor( static_cast<float>( ( GetHeight() - ( ( LIST_SLIDER_WIDTH - 2 ) * 3 ) ) ) / GetMaxValue() * i ) );
				if( mPos.GetY() >= iY && mPos.GetY() <= iY + LIST_SLIDER_WIDTH / 2 )
				{
					SetValue( i );
					break;
				}
			}
		}
	}
	else
	{
		SetObjectState( ( ( bMouseOver[ 2 ]	= DXMOUSE()->InArea( Pos.GetX(),  Pos.GetY() + LIST_SLIDER_WIDTH + static_cast<int>( floor( static_cast<float>( ( GetHeight() - ( LIST_SLIDER_WIDTH * 3 ) ) ) / GetMaxValue() * GetValue() ) ), LIST_SLIDER_WIDTH, LIST_SLIDER_WIDTH ) ) != 0 )?_T("MouseOver"):_T("Normal"), 1 );
	}
}

BOOL C_DX_LIST_VERTICAL_SLIDER::KeyEvent( C_DARK_POINT basePos, _KEY sKey )
{
	C_DARK_POINT Pos = basePos + *GetRelPos();

	if( !sKey.cKey )
	{
		if( DXMOUSE()->GetLeftButton() )
		{
			if( bMouseOver[ 0 ] )
			{
				if( GetValue() > GetMinValue() )
				{
					SetValue( GetValue() - 1 );
				}
				m_bPressed[ 0 ] = TRUE;
				SetObjectState( _T("Pressed") );

				m_tUpArrow.Start( 0.7f );
			}
			else if( bMouseOver[ 1 ] )
			{
				if( GetValue() < GetMaxValue() )
				{
					SetValue( GetValue() + 1 );
				}

				m_bPressed[ 1 ] = TRUE;
				SetObjectState( _T("Pressed"), 2 );

				m_tDownArrow.Start( 0.7f );
			}
			else if( DXMOUSE()->InArea( Pos.GetX() + 1, Pos.GetY() + LIST_SLIDER_WIDTH, LIST_SLIDER_WIDTH, GetHeight() - LIST_SLIDER_WIDTH * 2 ) )
			{
				SetObjectState( _T("Pressed"), 1 );

				SetDragged( TRUE );
				MouseMove( Pos );
			}
		}
		else
		{
			SetObjectState( bMouseOver[ 2 ]?_T("MouseOver"):_T("Normal"), 1 );
			SetDragged( FALSE );
		}

		if( !GetDragged() && DXMOUSE()->GetWheel() )
		{
			int iState = DXMOUSE()->GetWheel( 0 );

			int iTenth = ( GetMaxValue() / 10 )?GetMaxValue() / 10:1;

			if( iState == 1 )
				SetValue( GetValue() - iTenth );
			else if( iState == 2 )
				SetValue( GetValue() + iTenth );	
		}
	}
	return(TRUE);
}

void C_DX_LIST_VERTICAL_SLIDER::UpdateObject( int iIndex )
{
	_OBJECT_STATE * pState = GetElementState( iIndex );

	if( !iIndex )
	{
		pInner = pState->GetColor( _T("Inner") );
		pBorder = pState->GetColor( _T("Border") );

		pUpArrow = pState->GetTexture( _T("UpArrow") );
	}
	else if( iIndex == 1 )
		pSlider = pState->GetTexture( _T("Slider") );
	else
		pDownArrow = pState->GetTexture( _T("DownArrow") );
}