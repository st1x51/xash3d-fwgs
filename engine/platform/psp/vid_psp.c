#include "platform/platform.h"
#include "vid_common.h"
#include "client.h"

#if XASH_VIDEO == VIDEO_PSP
#include <pspdisplay.h>
#include <pspgu.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272
#define BUFFER_WIDTH 512

static qboolean vsync;
void *fbp0;
void *fbp1;
static unsigned int static_offset = 0;
unsigned int __attribute__((aligned(16))) list[262144];

static unsigned int GU_GetMemorySize( unsigned int width, unsigned int height, unsigned int psm )
{
	switch (psm)
	{
	case GU_PSM_T4: return ( ( width * height ) >> 1);
	case GU_PSM_T8: return width * height;
	case GU_PSM_5650:
	case GU_PSM_5551:
	case GU_PSM_4444:
	case GU_PSM_T16: return 2 * width * height;
	case GU_PSM_8888:
	case GU_PSM_T32: return 4 * width * height;
	default: return 0;
	}
}

void* GU_GetStaticVramBuffer( unsigned int width, unsigned int height, unsigned int psm )
{
	unsigned int mem_size = GU_GetMemorySize( width, height, psm );
	void* result = (void*)static_offset;
	static_offset += mem_size;
	return result;
}

int GL_SetAttribute( int attr, int val )
{
	return 0;
}

int GL_GetAttribute( int attr, int *val )
{
	return 0;
}

void* GL_GetProcAddress( const char *name )
{
	return NULL;
}

void GL_SwapBuffers( void )
{

}

int R_MaxVideoModes( void )
{
	return 0;
}

vidmode_t* R_GetVideoMode( int num )
{
	return NULL;
}

void R_Free_Video( void )
{
	ref.dllFuncs.GL_ClearExtensions();
}

qboolean R_Init_Video( const int type )
{
	qboolean retval;

	VID_StartupGamma();

	if( type != REF_SOFTWARE )
		return false;

	if( !(retval = VID_SetMode()) )
	{
		return retval;
	}

	host.renderinfo_changed = false;

	return true;
}

qboolean VID_SetMode( void )
{
	R_ChangeDisplaySettings( 0, 0, false );

	return true;
}

rserr_t R_ChangeDisplaySettings( int width, int height, qboolean fullscreen )
{
	int render_w, render_h;
	uint rotate = vid_rotate->value;

	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	render_w = width;
	render_h = height;

	Con_Reportf( "R_ChangeDisplaySettings: forced resolution to %dx%d)\n", width, height );

	if( ref.dllFuncs.R_SetDisplayTransform( rotate, 0, 0, vid_scale->value, vid_scale->value ) )
	{
		if( rotate & 1 )
		{
			int swap = render_w;

			render_w = render_h;
			render_h = swap;
		}

		render_h /= vid_scale->value;
		render_w /= vid_scale->value;
	}
	else
	{
		Con_Printf( S_WARN "R_ChangeDisplaySettings: failed to setup screen transform\n" );
	}

	R_SaveVideoMode( width, height, render_w, render_h );

	return rserr_ok;
}

qboolean SW_CreateBuffer( int width, int height, uint *stride, uint *bpp, uint *r, uint *g, uint *b )
{
	*stride = BUFFER_WIDTH;
	*bpp = 4;
	*r = 256 & 0xFF;
	*g = 256 >> 8 & 0xFF;
	*b = 256 >> 16 & 0xFF;

	fbp0 = GU_GetStaticVramBuffer( BUFFER_WIDTH, SCREEN_HEIGHT, GU_PSM_8888 );
	fbp1 = GU_GetStaticVramBuffer( BUFFER_WIDTH, SCREEN_HEIGHT, GU_PSM_8888 );

	sceGuInit();
	sceGuStart( GU_DIRECT, list );
	sceGuDrawBuffer( GU_PSM_8888, fbp0, BUFFER_WIDTH );
	sceGuDispBuffer( SCREEN_WIDTH, SCREEN_HEIGHT, fbp1, BUFFER_WIDTH );
	//sceGuScissor( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	sceGuFinish();
	sceGuSync(0, 0);

	sceDisplayWaitVblankStart();
	sceGuDisplay( GU_TRUE );

	return true;
}

void *SW_LockBuffer( void )
{
	sceGuStart( GU_DIRECT, list );
	//sceGuClear( GU_COLOR_BUFFER_BIT );
	return fbp0;
}

void SW_UnlockBuffer( void )
{
	sceGuFinish();
	sceGuSync( 0, 0 );
	fbp0 = sceGuSwapBuffers();
}

void GL_UpdateSwapInterval( void )
{
	if( cls.state < ca_active )
	{
		vsync = false;
		SetBits( gl_vsync->flags, FCVAR_CHANGED );
	}
	else if( FBitSet( gl_vsync->flags, FCVAR_CHANGED ))
	{
		ClearBits( gl_vsync->flags, FCVAR_CHANGED );
		vsync = true;
	}
}
#endif