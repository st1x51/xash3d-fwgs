#include "platform/platform.h"
#include "client.h"
#include "vid_common.h"

#include <pspgu.h>
#include <pspgum.h>
#include <pspdisplay.h>

static unsigned int staticOffset = 0;

static unsigned int getMemorySize(unsigned int width, unsigned int height, unsigned int psm)
{
	switch (psm)
	{
		case GU_PSM_T4:
			return (width * height) >> 1;

		case GU_PSM_T8:
			return width * height;

		case GU_PSM_5650:
		case GU_PSM_5551:
		case GU_PSM_4444:
		case GU_PSM_T16:
			return 2 * width * height;

		case GU_PSM_8888:
		case GU_PSM_T32:
			return 4 * width * height;

		default:
			return 0;
	}
}

void* getStaticVramBuffer(unsigned int width, unsigned int height, unsigned int psm)
{
	unsigned int memSize = getMemorySize(width,height,psm);
	void* result = (void*)staticOffset;
	staticOffset += memSize;

	return result;
}

void* getStaticVramTexture(unsigned int width, unsigned int height, unsigned int psm)
{
	void* result = getStaticVramBuffer(width,height,psm);
	return (void*)(((unsigned int)result) + ((unsigned int)sceGeEdramGetAddr()));
}

static unsigned int __attribute__((aligned(16))) display_list[262144];

#define SCR_WIDTH 480
#define SCR_HEIGHT 272
#define SCR_BPP 16

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
	R_ChangeDisplaySettings( SCR_HEIGHT, SCR_WIDTH, true );
	return true;
}

rserr_t R_ChangeDisplaySettings( int width, int height, qboolean fullscreen )
{
	Con_Reportf( "R_ChangeDisplaySettings: Setting video mode to %dx%d %s\n", width, height, fullscreen ? "fullscreen" : "windowed" );

	glw_state.desktopBitsPixel = SCR_BPP;
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

	sceGuInit();

	void* fbp0 = getStaticVramBuffer(512, SCR_HEIGHT, GU_PSM_8888);
	void* fbp1 = getStaticVramBuffer(512, SCR_HEIGHT, GU_PSM_8888);
	void* zbp = getStaticVramBuffer(512, SCR_HEIGHT, GU_PSM_4444);

	sceGuStart(GU_DIRECT, display_list);
	sceGuDrawBuffer(GU_PSM_8888, fbp0, 512);
	sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, fbp1, 512);
	sceGuDepthBuffer(zbp, 512);

	sceGuOffset(2048 - (SCR_WIDTH/2), 2048 - (SCR_HEIGHT/2));
	sceGuFinish();
	sceGuSync(0,0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);

	sceGuStart(GU_DIRECT, display_list);

	host.renderinfo_changed = false;
	return true;
}

void R_Free_Video( void )
{
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
	sceGuFinish();
	sceGuSync(0,0);
	sceDisplayWaitVblankStart();
	sceGuSwapBuffers();
	sceGuStart(GU_DIRECT, display_list);
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