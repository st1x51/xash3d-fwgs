#include "platform/platform.h"
#include <pspkernel.h>

PSP_MODULE_INFO( "xash3d", PSP_MODULE_USER, 1, 0 );
PSP_MAIN_THREAD_ATTR( PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU );
PSP_HEAP_SIZE_KB( -2 * 1024 );

#if XASH_TIMER == TIMER_PSP
double Platform_DoubleTime( void )
{
	static struct timeval st;
	static qboolean timer_init = false;
	struct timeval tv;
	
	if ( !timer_init )
	{
		gettimeofday( &st, NULL );
		timer_init = true;
	}
	
	gettimeofday( &tv, NULL );
	return (double)( tv.tv_sec - st.tv_sec ) + ( (double)( tv.tv_usec - st.tv_usec ) ) / 1000000.0;
}

void Platform_Sleep( int msec )
{
	sceKernelDelayThread( msec * 1000 );
}
#endif

void *Platform_GetNativeObject( const char *name )
{
	return NULL;
}

void Platform_Vibrate(float life, char flags)
{

}

void Platform_GetClipboardText( char *buffer, size_t size )
{

}

void Platform_SetClipboardText( const char *buffer, size_t size )
{

}

void Platform_ShellExecute( const char *path, const char *parms )
{

}

void Platform_Init( void )
{

}

void Platform_Shutdown( void )
{

}