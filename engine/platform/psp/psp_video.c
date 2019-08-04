#include "platform/platform.h"
#include "client.h"
#include "vid_common.h"

#include <pspgu.h>
#include <pspgum.h>
#include <pspdisplay.h>
#include <GLES/egl.h>
#include <GLES/gl.h>

static EGLDisplay dpy;
static EGLConfig config;
static EGLint num_configs;
static EGLContext ctx;
static EGLSurface surface;

static const EGLint attrib_list [] = {
	EGL_RED_SIZE, 1,
	EGL_GREEN_SIZE, 1,
	EGL_BLUE_SIZE, 1,
	EGL_ALPHA_SIZE, 0,
	EGL_DEPTH_SIZE, 0,
	EGL_NONE
};

void GL_UpdateSwapInterval( void )
{
	if( cls.state < ca_active )
	{
		//
		SetBits( gl_vsync->flags, FCVAR_CHANGED );
	}
	else if( FBitSet( gl_vsync->flags, FCVAR_CHANGED ))
	{
		ClearBits( gl_vsync->flags, FCVAR_CHANGED );
		//
	}
}

int R_MaxVideoModes()
{
	return 0;
}

vidmode_t* R_GetVideoMode( int num )
{
	return NULL;
}

qboolean VID_SetMode( void )
{
	R_ChangeDisplaySettings( 480, 272, true );
	return true;
}

rserr_t R_ChangeDisplaySettings( int width, int height, qboolean fullscreen )
{
	Con_Reportf( "R_ChangeDisplaySettings: Setting video mode to %dx%d %s\n", width, height, fullscreen ? "fullscreen" : "windowed" );

	glw_state.desktopBitsPixel = 16;
	glw_state.desktopWidth = width;
	glw_state.desktopHeight = height;
	refState.fullScreen = fullscreen;

	R_SaveVideoMode( width, height );

	return rserr_ok;
}

void* GL_GetProcAddress( const char *name )
{
	return NULL;
}

qboolean R_Init_Video( const int type )
{
	VID_StartupGamma();

	dpy = eglGetDisplay(0);
	eglInitialize(dpy, NULL, NULL);

	eglChooseConfig(dpy, attrib_list, &config, 1, &num_configs);

	ctx = eglCreateContext(dpy, config, NULL, NULL);
	surface = eglCreateWindowSurface(dpy, config, 0, NULL);
	eglMakeCurrent(dpy, surface, surface, ctx);

	host.renderinfo_changed = false;
	return true;
}

void R_Free_Video( void )
{
	eglTerminate(dpy);
	return;
}

int GL_SetAttribute( int attr, int val )
{
	return 0;
}

int GL_GetAttribute( int attr, int *val )
{
	return 0;
}

void GL_SwapBuffers()
{
	eglSwapBuffers(dpy, surface);
}

void *SW_LockBuffer()
{
	return NULL;
}

void SW_UnlockBuffer()
{
}

qboolean SW_CreateBuffer( int width, int height, uint *stride, uint *bpp, uint *r, uint *g, uint *b )
{
	return false;
}