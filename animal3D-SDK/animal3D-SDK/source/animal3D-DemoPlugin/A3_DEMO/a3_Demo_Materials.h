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

	a3_Demo_Keyframes.h
	Demo mode interface: keyframes, curves & interpolation.

	********************************************
	*** THIS IS ONE DEMO MODE'S HEADER FILE  ***
	********************************************
*/

#ifndef __ANIMAL3D_DEMO_MATERIALS_H
#define __ANIMAL3D_DEMO_MATERIALS_H


//-----------------------------------------------------------------------------

#include "animal3D/animal3D.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_Demo_Materials						a3_Demo_Materials;
	typedef enum a3_Demo_Materials_RenderProgramName		a3_Demo_Materials_RenderProgramName;
	typedef enum a3_Demo_Materials_DisplayProgramName		a3_Demo_Materials_DisplayProgramName;
	typedef enum a3_Demo_Materials_ActiveCameraName			a3_Demo_Materials_ActiveCameraName;
	typedef enum a3_Demo_Materials_PipelineName				a3_Demo_Materials_PipelineName;
	typedef enum a3_Demo_Materials_PassName					a3_Demo_Materials_PassName;
	typedef enum a3_Demo_Materials_TargetName				a3_Demo_Materials_TargetName;
	typedef enum a3_Demo_Materials_InterpolationModeName	a3_Demo_Materials_InterpolationModeName;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// scene object rendering program names
	enum a3_Demo_Materials_RenderProgramName
	{
		materials_renderPhong,			// Phong shading

		materials_render_max
	};
    
	// final display modes
	enum a3_Demo_Materials_DisplayProgramName
	{
		materials_displayTexture,		// display simple texture
		materials_displayOutline,		// display texture with outlines

		materials_display_max
	};
    
	// active camera names
	enum a3_Demo_Materials_ActiveCameraName
	{
		materials_cameraSceneViewer,	// scene viewing camera
		materials_cameraShadowLight,	// shadow map capturing light

		materials_camera_max
	};
    
    
	// pipeline names
	enum a3_Demo_Materials_PipelineName
	{
		materials_forward,				// forward lighting pipeline

		materials_pipeline_max
	};
    
	// render passes
	enum a3_Demo_Materials_PassName
	{
		materials_passShadow,			// capture shadow map

		materials_passScene,			// render scene objects

		materials_passComposite,		// composite for post-processing stage

		materials_passBright_2,		// bright pass for bloom (half screen size)
		materials_passBlurH_2,			// horizontal blur for bloom (half screen size)
		materials_passBlurV_2,			// horizontal blur for bloom (half screen size)
		materials_passBright_4,		// bright pass for bloom (quarter screen size)
		materials_passBlurH_4,			// horizontal blur for bloom (quarter screen size)
		materials_passBlurV_4,			// horizontal blur for bloom (quarter screen size)
		materials_passBright_8,		// bright pass for bloom (eighth screen size)
		materials_passBlurH_8,			// horizontal blur for bloom (eighth screen size)
		materials_passBlurV_8,			// horizontal blur for bloom (eighth screen size)
		materials_passBlend,			// bloom composite pass

		materials_pass_max
	};
    
	// render target names
	enum a3_Demo_Materials_TargetName
	{
		materials_shadow_fragdepth = 0,		// fragment depth
		materials_target_shadow_max, 

		materials_scene_finalcolor = 0,		// final display color
		materials_scene_position,				// position attribute
		materials_scene_normal,				// normal attribute
		materials_scene_texcoord,				// texcoord attribute
		materials_scene_shadowcoord,			// shadow coordinate
		materials_scene_shadowtest,			// result of shadow test
		materials_scene_diffuseLight,			// diffuse light total
		materials_scene_specularLight,			// specular light total
		materials_scene_fragdepth,				// fragment depth
		materials_target_scene_max, 

		materials_composite_finalcolor = 0,	// final display color
		materials_target_composite_max,

		materials_bright_finalcolor = 0,		// final display color
		materials_bright_luminance,			// luminance of color
		materials_target_bright_max,

		materials_blur_finalcolor = 0,			// final display color
		materials_target_blur_max,

		materials_display_finalcolor = 0,		// final display color
		materials_target_display_max,
	};
    
    
	// interpolation mode
	enum a3_Demo_Materials_InterpolationModeName
	{
		materials_interpNone,
		materials_interpLerp,
		materials_interpBezier,
		materials_interpCatmullRom,
		materials_interpCubicHermite,

		materials_interp_max
	};
    
    
//---------------------------------------------------------------------------
    
	// demo mode for basic shading
	struct a3_Demo_Materials
	{
		a3_Demo_Materials_RenderProgramName render;
		a3_Demo_Materials_DisplayProgramName display;
		a3_Demo_Materials_ActiveCameraName activeCamera;

		a3_Demo_Materials_PipelineName pipeline;
		a3_Demo_Materials_PassName pass;
		a3_Demo_Materials_TargetName targetIndex[materials_pass_max], targetCount[materials_pass_max];

		a3_Demo_Materials_InterpolationModeName interp;
    
		// skeletal controls
		a3ui32 editSkeletonIndex;
		a3ui32 editJointIndex;
		a3boolean editingJoint;
		a3boolean animating;
    
		a3boolean editingCreeper;
	};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMO_MATERIALS_H
