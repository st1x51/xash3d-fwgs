#include "sys_psp.h"
#include "platform/platform.h"
#include <stdio.h>
#include <pspkernel.h> 
#include <pspsdk.h> 

int gethostname(char *name, size_t len)
{
	return 0;
}

void Platform_GetMousePos( int *x, int *y )
{
	
}

void Platform_SetMousePos( int x, int y )
{

}

void SNDDMA_BeginPainting()
{

}

void SNDDMA_Submit()
{
}

int SNDDMA_GetSoundtime()
{
	return 0;
}

qboolean SNDDMA_Init()
{
	return false;
}

void SNDDMA_Shutdown()
{

}

void Platform_Vibrate( float time, char flags )
{

}

void Platform_MessageBox( const char *title, const char *text, qboolean parentMainWindow )
{

}

void Platform_GetClipboardText( char *buffer, size_t size )
{
	if( size ) buffer[0] = 0;
}

void Platform_SetClipboardText( const char *buffer, size_t size )
{

}

void Platform_EnableTextInput( qboolean enable )
{

}

void Platform_RunEvents( void )
{

}

void* Platform_GetNativeObject( const char *name )
{
	return NULL;
}

int dup (int oldfd)
{ 
   return (fcntl (oldfd, F_DUPFD, 0)); 
} 

void Platform_ShellExecute( const char *path, const char *parms )
{
}

int Platform_JoyInit( int numjoy )
{
	return 0;
}

void Platform_PreCreateMove( void )
{

}

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