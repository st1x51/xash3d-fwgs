#include "platform/platform.h"
#include <pspkernel.h> 
#include <pspsdk.h> 

#if XASH_TIMER == TIMER_PSP
double Platform_DoubleTime( void )
{
	static struct timeval st;
	static qboolean timerInit = false;
	struct timeval tv;
	
	if(!timerInit)
	{
		gettimeofday(&st, NULL);
		timerInit = true;
	}
	
	gettimeofday(&tv, NULL);
	return (double) (tv.tv_sec - st.tv_sec) + ((double) (tv.tv_usec - st.tv_usec))/1000000.0;
}

void Platform_Sleep( int msec )
{
	sceKernelDelayThread( msec * 1000 );
}
#endif