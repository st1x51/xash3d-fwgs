#include <pspkernel.h>
#include "gl_local.h"

PSP_MODULE_INFO("ref_gl", PSP_MODULE_USER, 1, 0);
PSP_NO_CREATE_MAIN_THREAD();

//static void GAME_EXPORT CL_FillRGBABlend( float _x, float _y, float _w, float _h, int r, int g, int b, int a );
int EXPORT GetRefAPI( int version, ref_interface_t *funcs, ref_api_t *engfuncs, ref_globals_t *globals );
void EXPORT GetRefHumanReadableName( char *out, size_t size );

dllexport_t ref_exports[] =
{
//	{ "CL_FillRGBABlend", (void *)CL_FillRGBABlend },
	{ "GetRefAPI", (void *)GetRefAPI },
	{ "GetRefHumanReadableName", (void *)GetRefHumanReadableName },
	{ NULL, NULL },
};

int module_start( SceSize argc, void *argv )
{
	return 0;
}

int module_stop( SceSize argc, void *argv )
{
	return 0;
}

dllexport_t* get_ref_exports( void )
{
	printf( "ref_gl: giving exports...\n" );
	return &ref_exports;
}