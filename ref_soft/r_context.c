/*
vid_sdl.c - SDL vid component
Copyright (C) 2018 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "r_local.h"

ref_api_t      gEngfuncs;
ref_globals_t *gpGlobals;
ref_instance_t RI;
gl_globals_t tr;
ref_speeds_t r_stats;
byte *r_temppool;
cvar_t *gl_emboss_scale;
cvar_t *r_drawentities;
cvar_t *r_norefresh;
cvar_t *vid_gamma;
cvar_t	*vid_brightness;
viddef_t vid;
static void R_ClearScreen( void )
{

}

static qboolean IsNormalPass( void )
{
	return RP_NORMALPASS();
}

static void R_IncrementSpeedsCounter( int type )
{
	switch( type )
	{
	case RS_ACTIVE_TENTS:
		r_stats.c_active_tents_count++;
		break;
	default:
		gEngfuncs.Host_Error( "R_IncrementSpeedsCounter: unsupported type %d\n", type );
	}
}

static const byte *R_GetTextureOriginalBuffer( unsigned int idx )
{
	/*gl_texture_t *glt = R_GetTexture( idx );

	if( !glt || !glt->original || !glt->original->buffer )
		return NULL;*/

	return NULL;
}

static int R_GetBuiltinTexture( enum ref_shared_texture_e type )
{
	switch( type )
	{
	case REF_DEFAULT_TEXTURE: return tr.defaultTexture;
	case REF_GRAY_TEXTURE: return tr.grayTexture;
	case REF_WHITE_TEXTURE: return tr.whiteTexture;
	case REF_SOLIDSKY_TEXTURE: return tr.solidskyTexture;
	case REF_ALPHASKY_TEXTURE: return tr.alphaskyTexture;
	default: gEngfuncs.Host_Error( "R_GetBuiltinTexture: unsupported type %d\n", type );
	}

	return 0;
}

static void R_FreeSharedTexture( enum ref_shared_texture_e type )
{
	int num = 0;

	switch( type )
	{
	case REF_SOLIDSKY_TEXTURE:
		num = tr.solidskyTexture;
		tr.solidskyTexture = 0;
		break;
	case REF_ALPHASKY_TEXTURE:
		num = tr.alphaskyTexture;
		tr.alphaskyTexture = 0;
		break;
	case REF_DEFAULT_TEXTURE:
	case REF_GRAY_TEXTURE:
	case REF_WHITE_TEXTURE:
		gEngfuncs.Host_Error( "R_FreeSharedTexture: invalid type %d\n", type );
	default: gEngfuncs.Host_Error( "R_FreeSharedTexture: unsupported type %d\n", type );
	}

	GL_FreeTexture( num );
}

/*
=============
CL_FillRGBA

=============
*/
static void CL_FillRGBA( float _x, float _y, float _w, float _h, int r, int g, int b, int a )
{

}

/*
=============
pfnFillRGBABlend

=============
*/
static void GAME_EXPORT CL_FillRGBABlend( float _x, float _y, float _w, float _h, int r, int g, int b, int a )
{

}


qboolean Mod_ProcessRenderData( model_t *mod, qboolean create, const byte *buf )
{
	qboolean loaded = true;

	if( create )
	{


		switch( mod->type )
		{
			case mod_studio:
				// Mod_LoadStudioModel( mod, buf, loaded );
				break;
			case mod_sprite:
				//Mod_LoadSpriteModel( mod, buf, &loaded, mod->numtexinfo );
				break;
			case mod_alias:
				//Mod_LoadAliasModel( mod, buf, &loaded );
				break;
			case mod_brush:
				// Mod_LoadBrushModel( mod, buf, loaded );
				break;

			default: gEngfuncs.Host_Error( "Mod_LoadModel: unsupported type %d\n", mod->type );
		}
	}

	if( loaded && gEngfuncs.drawFuncs->Mod_ProcessUserData )
		gEngfuncs.drawFuncs->Mod_ProcessUserData( mod, create, buf );

	//if( !create )
		//Mod_UnloadTextures( mod );

	return loaded;
}

static int GL_RenderGetParm( int parm, int arg )
{
	image_t *glt;

	switch( parm )
	{
	case PARM_TEX_WIDTH:
		glt = R_GetTexture( arg );
		return glt->width;
	case PARM_TEX_HEIGHT:
		glt = R_GetTexture( arg );
		return glt->height;
	case PARM_TEX_SRC_WIDTH:
		glt = R_GetTexture( arg );
		return glt->srcWidth;
	case PARM_TEX_SRC_HEIGHT:
		glt = R_GetTexture( arg );
		return glt->srcHeight;
	case PARM_TEX_MIPCOUNT:
		glt = R_GetTexture( arg );
		return glt->numMips;
	case PARM_TEX_DEPTH:
		glt = R_GetTexture( arg );
		return glt->depth;
	case PARM_TEX_SKYBOX:
		Assert( arg >= 0 && arg < 6 );
		return tr.skyboxTextures[arg];
	case PARM_TEX_SKYTEXNUM:
		return tr.skytexturenum;
	case PARM_TEX_LIGHTMAP:
		arg = bound( 0, arg, MAX_LIGHTMAPS - 1 );
		return tr.lightmapTextures[arg];
	case PARM_WIDESCREEN:
		return gpGlobals->wideScreen;
	case PARM_FULLSCREEN:
		return gpGlobals->fullScreen;
	case PARM_SCREEN_WIDTH:
		return gpGlobals->width;
	case PARM_SCREEN_HEIGHT:
		return gpGlobals->height;
	case PARM_TEX_FLAGS:
		glt = R_GetTexture( arg );
		return glt->flags;
	case PARM_LIGHTSTYLEVALUE:
		arg = bound( 0, arg, MAX_LIGHTSTYLES - 1 );
		return tr.lightstylevalue[arg];
	case PARM_MAX_IMAGE_UNITS:
		return 1;
	case PARM_REBUILD_GAMMA:
		return 0;//glConfig.softwareGammaUpdate;
	case PARM_SURF_SAMPLESIZE:
		if( arg >= 0 && arg < WORLDMODEL->numsurfaces )
			return gEngfuncs.Mod_SampleSizeForFace( &WORLDMODEL->surfaces[arg] );
		return LM_SAMPLE_SIZE;
	case PARM_SKY_SPHERE:
		return ENGINE_GET_PARM_( parm, arg ) && !tr.fCustomSkybox;
	default:
		return ENGINE_GET_PARM_( parm, arg );
	}
	return 0;
}

static void R_GetDetailScaleForTexture( int texture, float *xScale, float *yScale )
{
	image_t *glt = R_GetTexture( texture );

	if( xScale ) *xScale = glt->xscale;
	if( yScale ) *yScale = glt->yscale;
}

static void R_GetExtraParmsForTexture( int texture, byte *red, byte *green, byte *blue, byte *density )
{
	image_t *glt = R_GetTexture( texture );

	if( red ) *red = glt->fogParams[0];
	if( green ) *green = glt->fogParams[1];
	if( blue ) *blue = glt->fogParams[2];
	if( density ) *density = glt->fogParams[3];
}


static void R_SetCurrentEntity( cl_entity_t *ent )
{
	RI.currententity = ent;

	// set model also
	if( RI.currententity != NULL )
	{
		RI.currentmodel = RI.currententity->model;
	}
}

static void R_SetCurrentModel( model_t *mod )
{
	RI.currentmodel = mod;
}

static float R_GetFrameTime( void )
{
	return tr.frametime;
}

static const char *GL_TextureName( unsigned int texnum )
{
	return "";//return R_GetTexture( texnum )->name;
}

const byte *GL_TextureData( unsigned int texnum )
{
//	rgbdata_t *pic = R_GetTexture( texnum )->original;

	//if( pic != NULL )
		//return pic->buffer;
	return NULL;
}

void Mod_BrushUnloadTextures( model_t *mod )
{
	int i;

	for( i = 0; i < mod->numtextures; i++ )
	{
		texture_t *tx = mod->textures[i];
		if( !tx || tx->gl_texturenum == tr.defaultTexture )
			continue;	// free slot

		//GL_FreeTexture( tx->gl_texturenum );	// main texture
		//GL_FreeTexture( tx->fb_texturenum );	// luma texture
	}
}

void Mod_UnloadTextures( model_t *mod )
{
	int		i, j;

	Assert( mod != NULL );

	switch( mod->type )
	{
	case mod_studio:
		//Mod_StudioUnloadTextures( mod->cache.data );
		break;
	case mod_alias:
		//Mod_AliasUnloadTextures( mod->cache.data );
		break;
	case mod_brush:
		Mod_BrushUnloadTextures( mod );
		break;
	case mod_sprite:
		//Mod_SpriteUnloadTextures( mod->cache.data );
		break;
	default: gEngfuncs.Host_Error( "Mod_UnloadModel: unsupported type %d\n", mod->type );
	}
}

void R_ProcessEntData( qboolean allocate )
{

}

// stubs

void GL_SetTexCoordArrayMode()
{

}

void GL_OnContextCreated()
{
	R_InitBlit();

}

void GL_InitExtensions()
{

}
void GL_ClearExtensions()
{

}

void GL_BackendStartFrame()
{

}

void GL_BackendEndFrame()
{

}


void GL_SetRenderMode(int mode)
{
	vid.rendermode = mode;
	/// TODO: table shading/blending???
	/// maybe, setup block drawing function pointers here
}

void CL_AddCustomBeam(cl_entity_t *pEnvBeam)
{
	// same for beams
}

void R_ShowTextures()
{
	// textures undone too
}

void R_ShowTree()
{
	// do we really need this here???
}

void R_SetupSky(const char *skyboxname)
{

}

qboolean VID_ScreenShot(const char *filename, int shot_type)
{

}

qboolean VID_CubemapShot(const char *base, uint size, const float *vieworg, qboolean skyshot)
{
	// cubemaps? in my softrender???
}

void R_DecalShoot(int textureIndex, int entityIndex, int modelIndex, vec3_t pos, int flags, float scale)
{

}

void R_DecalRemoveAll(int texture)
{

}

int R_CreateDecalList(decallist_t *pList)
{
	return 0;
}

void R_ClearAllDecals()
{

}

float R_StudioEstimateFrame(cl_entity_t *e, mstudioseqdesc_t *pseqdesc)
{
	return 0;
}

void R_StudioLerpMovement(cl_entity_t *e, double time, vec3_t origin, vec3_t angles)
{

}

void CL_InitStudioAPI()
{

}

void R_InitSkyClouds(mip_t *mt, texture_t *tx, qboolean custom_palette)
{

}

void GL_SubdivideSurface(msurface_t *fa)
{

}

void Mod_LoadMapSprite(model_t *mod, const void *buffer, size_t size, qboolean *loaded)
{

}

void Mod_StudioLoadTextures(model_t *mod, void *data)
{

}

void CL_DrawParticles(double frametime, particle_t *cl_active_particles, float partsize)
{

}

void CL_DrawBeams(int fTrans, BEAM *active_beams)
{

}

void CL_DrawTracers(double frametime, particle_t *cl_active_tracers)
{

}

qboolean R_BeamCull(const vec3_t start, const vec3_t end, qboolean pvsOnly)
{
	return false;
}

void DrawSingleDecal(decal_t *pDecal, msurface_t *fa)
{

}

float *R_DecalSetupVerts(decal_t *pDecal, msurface_t *surf, int texture, int *outCount)
{
	return NULL;
}

void R_EntityRemoveDecals(model_t *mod)
{

}

void GL_SelectTexture(int texture)
{

}

void GL_LoadTexMatrixExt(const float *glmatrix)
{

}

void GL_LoadIdentityTexMatrix()
{

}

void GL_CleanUpTextureUnits(int last)
{

}

void GL_TexGen(unsigned int coord, unsigned int mode)
{

}

void GL_TextureTarget(uint target)
{

}

void CL_DrawParticlesExternal(const ref_viewpass_t *rvp, qboolean trans_pass, float frametime)
{
	// no renderapi support
}

struct mstudiotex_s *R_StudioGetTexture(cl_entity_t *e)
{
	return NULL;
}

void GL_BuildLightmaps()
{

}

void Mod_SetOrthoBounds(const float *mins, const float *maxs)
{

}

qboolean R_SpeedsMessage(char *out, size_t size)
{
	return false;
}

byte *Mod_GetCurrentVis()
{
	return NULL;
}

void R_ScreenToWorld(const vec3_t screen, vec3_t point)
{

}
void GL_SetupAttributes( int safegl )
{
	gEngfuncs.Con_Reportf( "Creating an extended GL context for debug...\n" );
	gEngfuncs.GL_SetAttribute( REF_GL_CONTEXT_FLAGS, REF_GL_CONTEXT_DEBUG_FLAG );

	// untill we have any blitter in ref api, setup GL
	gEngfuncs.GL_SetAttribute( REF_GL_CONTEXT_PROFILE_MASK, REF_GL_CONTEXT_PROFILE_COMPATIBILITY );
	gEngfuncs.GL_SetAttribute( REF_GL_DOUBLEBUFFER, 1 );

	gEngfuncs.GL_SetAttribute( REF_GL_RED_SIZE, 5 );
	gEngfuncs.GL_SetAttribute( REF_GL_GREEN_SIZE, 6 );
	gEngfuncs.GL_SetAttribute( REF_GL_BLUE_SIZE, 5 );
}

ref_interface_t gReffuncs =
{
	R_Init,
	R_Shutdown,

	GL_SetupAttributes,
	GL_OnContextCreated,
	GL_InitExtensions,
	GL_ClearExtensions,

	R_BeginFrame,
	R_RenderScene,
	R_EndFrame,
	R_PushScene,
	R_PopScene,
	GL_BackendStartFrame,
	GL_BackendEndFrame,

	R_ClearScreen,
	R_AllowFog,
	GL_SetRenderMode,

	R_AddEntity,
	CL_AddCustomBeam,
	R_ProcessEntData,

	IsNormalPass,

	R_ShowTextures,
	R_ShowTree,
	R_IncrementSpeedsCounter,

	R_GetTextureOriginalBuffer,
	GL_LoadTextureFromBuffer,
	R_GetBuiltinTexture,
	R_FreeSharedTexture,
	GL_ProcessTexture,
	R_SetupSky,

	R_Set2DMode,
	R_DrawStretchRaw,
	R_DrawStretchPic,
	R_DrawTileClear,
	CL_FillRGBA,
	CL_FillRGBABlend,

	VID_ScreenShot,
	VID_CubemapShot,

	R_LightPoint,

	R_DecalShoot,
	R_DecalRemoveAll,
	R_CreateDecalList,
	R_ClearAllDecals,

	R_StudioEstimateFrame,
	R_StudioLerpMovement,
	CL_InitStudioAPI,

	R_InitSkyClouds,
	GL_SubdivideSurface,
	CL_RunLightStyles,

	R_GetSpriteParms,
	R_GetSpriteTexture,

	Mod_LoadMapSprite,
	Mod_ProcessRenderData,
	Mod_StudioLoadTextures,

	CL_DrawParticles,
	CL_DrawTracers,
	CL_DrawBeams,
	R_BeamCull,

	GL_RenderGetParm,
	R_GetDetailScaleForTexture,
	R_GetExtraParmsForTexture,
	R_GetFrameTime,

	R_SetCurrentEntity,
	R_SetCurrentModel,

	GL_FindTexture,
	GL_TextureName,
	GL_TextureData,
	GL_LoadTexture,
	GL_CreateTexture,
	GL_LoadTextureArray,
	GL_CreateTextureArray,
	GL_FreeTexture,

	DrawSingleDecal,
	R_DecalSetupVerts,
	R_EntityRemoveDecals,

	R_UploadStretchRaw,

	GL_Bind,
	GL_SelectTexture,
	GL_LoadTexMatrixExt,
	GL_LoadIdentityTexMatrix,
	GL_CleanUpTextureUnits,
	GL_TexGen,
	GL_TextureTarget,
	GL_SetTexCoordArrayMode,
	GL_UpdateTexSize,
	NULL,
	NULL,

	CL_DrawParticlesExternal,
	R_LightVec,
	R_StudioGetTexture,

	R_RenderFrame,
	GL_BuildLightmaps,
	Mod_SetOrthoBounds,
	R_SpeedsMessage,
	Mod_GetCurrentVis,
	R_NewMap,
	R_ClearScene,

	TriRenderMode,
	TriBegin,
	TriEnd,
	_TriColor4f,
	TriColor4ub,
	TriTexCoord2f,
	TriVertex3fv,
	TriVertex3f,
	TriWorldToScreen,
	TriFog,
	R_ScreenToWorld,
	TriGetMatrix,
	TriFogParams,
	TriCullFace,

	VGUI_DrawInit,
	VGUI_DrawShutdown,
	VGUI_SetupDrawingText,
	VGUI_SetupDrawingRect,
	VGUI_SetupDrawingImage,
	VGUI_BindTexture,
	VGUI_EnableTexture,
	VGUI_CreateTexture,
	VGUI_UploadTexture,
	VGUI_UploadTextureBlock,
	VGUI_DrawQuad,
	VGUI_GetTextureSizes,
	VGUI_GenerateTexture,
};

int GAME_EXPORT GetRefAPI( int version, ref_interface_t *funcs, ref_api_t *engfuncs, ref_globals_t *globals )
{
	if( version != REF_API_VERSION )
		return 0;

	// fill in our callbacks
	memcpy( funcs, &gReffuncs, sizeof( ref_interface_t ));
	memcpy( &gEngfuncs, engfuncs, sizeof( ref_api_t ));
	gpGlobals = globals;

	return REF_API_VERSION;
}
