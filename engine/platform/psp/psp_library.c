#define _GNU_SOURCE

#include "common.h"
#include "library.h"
#include "filesystem.h"
#include "server.h"
#include "dlfcn.h"
#include <pspkernel.h>
#include <pspctrl.h>

PSP_MODULE_INFO("engine", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-16 * 1024);

qboolean COM_CheckLibraryDirectDependency( const char *name, const char *depname, qboolean directpath )
{
	return true;
}

void *COM_LoadLibrary( const char *dllname, int build_ordinals_table, qboolean directpath )
{
	dll_user_t *hInst = NULL;
	void *pHandle = NULL;

	COM_ResetLibraryError();

	hInst = FS_FindLibrary( dllname, directpath );
	if( !hInst )
	{
		if( !pHandle )
		{
			pHandle = dlopen( dllname, RTLD_LAZY );
			if( pHandle )
				return pHandle;

			COM_PushLibraryError( va( "Failed to find library %s", dllname ));
			COM_PushLibraryError( dlerror() );
			return NULL;
		}
	}

	if( hInst->custom_loader )
	{
		COM_PushLibraryError( va( "Custom library loader is not available. Extract library %s and fix gameinfo.txt!", hInst->fullPath ));
		Mem_Free( hInst );
		return NULL;
	}

	if( !( hInst->hInstance = dlopen( hInst->fullPath, RTLD_LAZY ) ) )
	{
		COM_PushLibraryError( dlerror() );
		Mem_Free( hInst );
		return NULL;
	}

	pHandle = hInst->hInstance;

	Mem_Free( hInst );

	return pHandle;
}

void COM_FreeLibrary( void *hInstance )
{
	dlclose( hInstance );
}

void *COM_GetProcAddress( void *hInstance, const char *name )
{
	return dlsym( hInstance, name );
}

void *COM_FunctionFromName( void *hInstance, const char *pName )
{
	void *function;
	if( !( function = COM_GetProcAddress( hInstance, pName ) ) )
	{
		Con_Reportf( S_ERROR "FunctionFromName: Can't get symbol %s: %s\n", pName, dlerror());
	}
	return function;
}

const char *COM_NameForFunction( void *hInstance, void *function )
{
	{
		Dl_info info = {0};
		dladdr((void*)function, &info);
		if(info.dli_sname)
			return info.dli_sname;
	}
	return NULL;
}
