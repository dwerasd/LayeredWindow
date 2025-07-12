#pragma once


#include <windows.h>

#include <MMSystem.h>
#pragma comment (lib,"winmm.lib")


class C_DARK_TIMER
{
public:

	C_DARK_TIMER();
	~C_DARK_TIMER();

	void Start( float fSec );
	void Stop();

	float TimeLeft();
	BOOL Running();

protected:
	DWORD dwStop;
};