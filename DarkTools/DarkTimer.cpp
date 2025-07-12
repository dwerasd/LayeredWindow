#include "stdafx.h"
#include "DarkTimer.h"



C_DARK_TIMER::C_DARK_TIMER()
{
	dwStop = 0;
}

C_DARK_TIMER::~C_DARK_TIMER()
{
}

void C_DARK_TIMER::Start( float fSec )
{
	dwStop = (DWORD)( fSec * 1000 ) + timeGetTime();
}

void C_DARK_TIMER::Stop()
{
	dwStop = 0;
}

float C_DARK_TIMER::TimeLeft()
{
	if( Running() )
		return ( ( (float)( dwStop - timeGetTime() ) / 1000 ) );
	return 0; 
}

BOOL C_DARK_TIMER::Running()
{
	return ( dwStop > timeGetTime() );
}