/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein

	a3_Demo_Pipelines.h
	Demo mode interface: pipelines.

	********************************************
	*** THIS IS ONE DEMO MODE'S HEADER FILE  ***
	********************************************
*/

#ifndef __ANIMAL3D_DEMO_PIPELINES_H
#define __ANIMAL3D_DEMO_PIPELINES_H


//-----------------------------------------------------------------------------

#include "animal3D/animal3D.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_Demo_Pipelines					a3_Demo_Pipelines;
	typedef enum a3_Demo_Pipelines_PipelineName			a3_Demo_Pipelines_PipelineName;
	typedef enum a3_Demo_Pipelines_RenderProgramName	a3_Demo_Pipelines_RenderProgramName;
	typedef enum a3_Demo_Pipelines_DisplayProgramName	a3_Demo_Pipelines_DisplayProgramName;
	typedef enum a3_Demo_Pipelines_ActiveCameraName		a3_Demo_Pipelines_ActiveCameraName;
	typedef enum a3_Demo_Pipelines_PassName				a3_Demo_Pipelines_PassName;
	typedef enum a3_Demo_Pipelines_TargetName_Shadow	a3_Demo_Pipelines_TargetName_Shadow;
	typedef enum a3_Demo_Pipelines_TargetName_Scene		a3_Demo_Pipelines_TargetName_Scene;
	typedef enum a3_Demo_Pipelines_TargetName_Composite	a3_Demo_Pipelines_TargetName_Composite;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// pipeline names
	enum a3_Demo_Pipelines_PipelineName
	{
		pipelines_forward,				// forward lighting pipeline

		pipelines_pipeline_max
	};

	// scene object rendering program names
	enum a3_Demo_Pipelines_RenderProgramName
	{
		pipelines_renderPhong,			// Phong shading
		pipelines_renderPhongShadow,	// Phong shading with shadows
		pipelines_renderNP,
		pipelines_renderNPShadow,
		pipelines_render_max
	};

	// final display modes
	enum a3_Demo_Pipelines_DisplayProgramName
	{
		pipelines_displayTexture,		// display simple texture
		pipelines_displayOutline,		// display texture with outlines

		pipelines_display_max
	};

	// active camera names
	enum a3_Demo_Pipelines_ActiveCameraName
	{
		pipelines_cameraSceneViewer,	// scene viewing camera
		pipelines_cameraShadowLight,	// shadow map capturing light

		pipelines_camera_max
	};

	// render passes
	enum a3_Demo_Pipelines_PassName
	{
		pipelines_passShadow,			// capture shadow map
		pipelines_passScene,			// render scene objects
		pipelines_passComposite,		// composite for post-processing stage

		pipelines_pass_max
	};

	// render target names - shadow pass
	enum a3_Demo_Pipelines_TargetName_Shadow
	{
		pipelines_shadow_fragdepth,		// fragment depth

		pipelines_target_shadow_max
	};

	// render target names - scene pass
	enum a3_Demo_Pipelines_TargetName_Scene
	{
		pipelines_scene_finalcolor,		// final display color
		pipelines_scene_position,		// position attribute
		pipelines_scene_normal,			// normal attribute
		pipelines_scene_texcoord,		// texcoord attribute
		pipelines_scene_shadowcoord,	// shadow coordinate
		pipelines_scene_shadowtest,		// result of shadow test
		pipelines_scene_diffuseLight,	// diffuse light total
		pipelines_scene_specularLight,	// specular light total
		pipelines_scene_fragdepth,		// fragment depth

		pipelines_target_scene_max
	};

	// render target names - composite pass
	enum a3_Demo_Pipelines_TargetName_Composite
	{
		pipelines_composite_finalcolor,	// final display color

		pipelines_target_composite_max
	};


//-----------------------------------------------------------------------------

	// demo mode for basic shading
	struct a3_Demo_Pipelines
	{
		a3_Demo_Pipelines_PipelineName pipeline;
		a3_Demo_Pipelines_RenderProgramName render;
		a3_Demo_Pipelines_DisplayProgramName display;
		a3_Demo_Pipelines_ActiveCameraName activeCamera;
		a3_Demo_Pipelines_PassName pass;
		a3_Demo_Pipelines_TargetName_Shadow target_shadow;
		a3_Demo_Pipelines_TargetName_Scene target_scene;
		a3_Demo_Pipelines_TargetName_Composite target_composite;
	};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMO_PIPELINES_H