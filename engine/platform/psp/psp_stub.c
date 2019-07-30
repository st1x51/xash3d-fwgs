#include "psp_stub.h"

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