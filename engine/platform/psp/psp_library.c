#include "common.h"
#include "library.h"
#include "filesystem.h"
#include "server.h"
#include <pspkernel.h>
#include <pspctrl.h>
#include <pspsdk.h>
#include <pspuser.h>

PSP_MODULE_INFO("engine", PSP_MODULE_USER, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-16 * 1024);

typedef struct dll_s
{
	SceUID handle;
	char name[MAX_DLNAMELEN];
	int refcnt;
	dllexport_t *exp;
	struct dll_s *next;
} dll_t;

static dll_t *dll_list;
static char *dll_err = NULL;
static char dll_err_buf[1024];

static void *dlfind( const char *name )
{
	dll_t *d = NULL;
	for( d = dll_list; d; d = d->next )
		if( !Q_strcmp( d->name, name ) )
			break;
	return d;
}

static const char *dlname( void *handle )
{
	dll_t *d = NULL;
	for( d = dll_list; d; d = d->next ) if( d == handle ) break;
	return d ? d->name : NULL;
}

void *dlopen( const char *name, int flag )
{
	if( !name ) return NULL;

	dll_t *old = dlfind( name );
	if( old ) { old->refcnt++; return old; }

	char fullpath[MAX_SYSPATH];
	Q_snprintf( fullpath, sizeof(fullpath), "%s", name );

	int status = 0;
	SceUID h = pspSdkLoadStartModule( fullpath, PSP_MEMORY_PARTITION_USER );

	if( !h ) { dll_err = "dlopen(): something went wrong"; return NULL; }
	if( h < 0 )
	{
		snprintf( dll_err_buf, sizeof( dll_err_buf ), "dlopen(%s): error 0x%X\n", name, h );
		dll_err = dll_err_buf;
		return NULL;
	}
	else
		Con_Printf ( "dlopen(%s): success!\n", fullpath );

	dll_t *new = calloc( 1, sizeof( dll_t ) );
	if( !new ) { dll_err = "dlopen(): out of memory";  return NULL; }
	snprintf( new->name, MAX_DLNAMELEN, name );
	new->handle = h;

	int i = 0;
//	if(strstr(name, "ref_gl"))
//	{
//		printf("DLOPEN STATUS: %s\n", load_refgu_exports(&(new->exp)));
//	}
	
	new->refcnt = 1;

	new->next = dll_list;
	dll_list = new;

	return new;
}

void *dlsym( void *handle, const char *symbol )
{
	if( !handle || !symbol ) { dll_err = "dlsym(): NULL args"; return NULL; }
	if( !dlname( handle ) ) { dll_err = "dlsym(): unknown handle"; return NULL; }
	dll_t *d = handle;
	if( !d->refcnt ) { dll_err = "dlsym(): call dlopen() first"; return NULL; }
	dllexport_t *f = NULL;
	for( f = d->exp; f && f->func; f++ )
		if( !Q_strcmp( f->name, symbol ) )
			break;

	if( f && f->func )
	{
		return f->func;
	}
	else
	{
		dll_err = "dlsym(): symbol not found in dll";
		return NULL;
	}
}

int dlclose( void *handle )
{
	if( !handle ) { dll_err = "dlclose(): NULL arg"; return -1; }
	if( !dlname( handle ) ) { dll_err = "dlclose(): unknown handle"; return -2; }

	dll_t *d = handle;
	d->refcnt--;
	if( d->refcnt <= 0 )
	{
		int status = 0;
		int ret = 0;
		//int ret = sceKernelStopModule( d->handle, 0, NULL, 0, NULL, &status );
		if( ret != SCE_KERNEL_ERROR_OK )
		{
			snprintf( dll_err_buf, sizeof( dll_err_buf ), "dlclose(): error %d", ret );
			dll_err = dll_err_buf;
		}
		else if( status == SCE_KERNEL_ERROR_NOT_STOPPED )
		{
			dll_err = "dlclose(): module doesn't want to stop";
			return -3;
		}

		if( d == dll_list )
			dll_list = NULL;
		else
		{
			dll_t *pd;
			for( pd = dll_list; pd; pd = pd->next )
			{
				if( pd->next == d )
				{
					pd->next = d->next;
					break;
				}
			}
		}
		free( d );
	}
	return 0;
}

char *dlerror(void)
{
	char *err = dll_err;
	dll_err = NULL;
	return err;
}

int dladdr( const void *addr, Dl_info *info )
{
	dll_t *d = NULL;
	dllexport_t *f = NULL;
	for( d = dll_list; d; d = d->next )
		for( f = d->exp; f && f->func; f++ )
			if( f->func == addr ) goto for_end;
for_end:
	if( d && f && f->func )
	{
		if( info )
		{
			info->dli_fhandle = d;
			info->dli_sname = f->name;
			info->dli_saddr = addr;
		}
		return 1;
	}
	return 0;
}

qboolean COM_CheckLibraryDirectDependency( const char *name, const char *depname, qboolean directpath )
{
	return true;
}

void *COM_LoadLibrary( const char *dllname, int build_ordinals_table, qboolean directpath )
{
	printf("Free Memory: %i bytes\n", sceKernelTotalFreeMemSize());
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
