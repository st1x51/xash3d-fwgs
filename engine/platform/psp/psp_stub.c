#include "psp_stub.h"
#include <stdio.h>
#include <GL/gl.h>

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

__attribute__((__no_instrument_function__))
void __cyg_profile_func_enter(void *func,  void *caller)
{
 	printf("e %p %p %lu\n", func, caller, time(NULL) );
}

__attribute__((__no_instrument_function__))
void __cyg_profile_func_exit(void *func, void *caller)
{
	printf("x %p %p %lu\n", func, caller, time(NULL) );
}

void glHint( GLenum target, GLenum mode )
{
}

static GLboolean fog_enabled = GL_TRUE;

GLboolean glIsEnabled( GLenum cap )
{
	return fog_enabled;
}

void glClipPlane( GLenum plane, const GLdouble *equation )
{
}

void glTexImage1D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
	glTexImage2D( GL_TEXTURE_2D, level, internalformat, width, 1, border, format, type, pixels );
}

void glTexImage3D( GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
	glTexImage2D( GL_TEXTURE_2D, level, internalformat, width, height, border, format, type, pixels );
}

void glTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels )
{
	glTexSubImage2D( target, level, xoffset, 0, width, 1, format, type, pixels );
}

void glTexSubImage3D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels )
{
	glTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
}

void glPointSize( GLfloat size )
{
}

void glDebugMessageControlARB( GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled )
{
}

void glDebugMessageCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
}
