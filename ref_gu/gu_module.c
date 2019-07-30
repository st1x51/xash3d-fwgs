#include <pspkernel.h>
#include "const.h"
#include "cvar.h"
#include "ref_api.h"

PSP_MODULE_INFO("ref_gl", PSP_MODULE_USER, 1, 0);
PSP_NO_CREATE_MAIN_THREAD();

typedef struct dllexport_s
{
	const char *name;
	void *func;
} dllexport_t;

//static void CL_FillRGBABlend( float _x, float _y, float _w, float _h, int r, int g, int b, int a );
int GAME_EXPORT GetRefAPI( int version, ref_interface_t *funcs, ref_api_t *engfuncs, ref_globals_t *globals );
void GAME_EXPORT VGUI_DrawInit( void );
void GAME_EXPORT VGUI_DrawShutdown( void );
int GAME_EXPORT VGUI_GenerateTexture( void );
void GAME_EXPORT VGUI_UploadTexture( int id, const char *buffer, int width, int height );
void GAME_EXPORT VGUI_CreateTexture( int id, int width, int height );
void GAME_EXPORT VGUI_UploadTextureBlock( int id, int drawX, int drawY, const byte *rgba, int blockWidth, int blockHeight );
void GAME_EXPORT VGUI_SetupDrawingRect( int *pColor );
void GAME_EXPORT VGUI_SetupDrawingText( int *pColor );
void GAME_EXPORT VGUI_SetupDrawingImage( int *pColor );
void GAME_EXPORT VGUI_BindTexture( int id );
void GAME_EXPORT VGUI_GetTextureSizes( int *width, int *height );
void GAME_EXPORT VGUI_EnableTexture( qboolean enable );
void GAME_EXPORT VGUI_DrawQuad( const vpoint_t *ul, const vpoint_t *lr );

dllexport_t exports[] =
{
//	{ "CL_FillRGBABlend", (void*)CL_FillRGBABlend },
	{ "GetRefAPI", (void*)GetRefAPI },
	{ "VGUI_DrawInit", (void*)VGUI_DrawInit },
	{ "VGUI_DrawShutdown", (void*)VGUI_DrawShutdown },
	{ "VGUI_GenerateTexture", (void*)VGUI_GenerateTexture },
	{ "VGUI_UploadTexture", (void*)VGUI_UploadTexture },
	{ "VGUI_CreateTexture", (void*)VGUI_CreateTexture },
	{ "VGUI_UploadTextureBlock", (void*)VGUI_UploadTextureBlock },
	{ "VGUI_SetupDrawingRect", (void*)VGUI_SetupDrawingRect },
	{ "VGUI_SetupDrawingText", (void*)VGUI_SetupDrawingText },
	{ "VGUI_SetupDrawingImage", (void*)VGUI_SetupDrawingImage },
	{ "VGUI_BindTexture", (void*)VGUI_BindTexture },
	{ "VGUI_GetTextureSizes", (void*)VGUI_GetTextureSizes },
	{ "VGUI_EnableTexture", (void*)VGUI_EnableTexture },
	{ "VGUI_DrawQuad", (void*)VGUI_DrawQuad },
	{ NULL, NULL },
};

int module_start( SceSize arglen, void *argp )
{
	//__global_ctors();
	return 0;
}

int module_stop( SceSize arglen, void *argp )
{
	return 0;
}

const char *load_refgu_exports(dllexport_t* exp[])
{
	*exp = exports;
	return "EXPORTS LOADED";
}