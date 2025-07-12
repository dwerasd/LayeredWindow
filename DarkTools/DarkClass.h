#ifndef DARK_TOOLS_CLASS__
#define DARK_TOOLS_CLASS__

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <windows.h>

#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")


#include "DarkApi.h"


namespace DarkTools
{
	class C_COPY_PREVENT
	{
	protected:
		C_COPY_PREVENT(const C_COPY_PREVENT &rhs);
		C_COPY_PREVENT &operator=(const C_COPY_PREVENT &rhs);

	protected:
		C_COPY_PREVENT()
		{
			
		}
		virtual ~C_COPY_PREVENT() { }
	};

	class C_CLASS_BASE
	{
	private:
		time_t age;

	public:
		C_CLASS_BASE() : age(timeGetTime())
		{
			
		}
		virtual ~C_CLASS_BASE() { }

		time_t GetTime() { return(age); }
	};
} // End of namespace DarkTools 

#endif // DARK_TOOLS_EXCEPTION__

