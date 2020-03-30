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

#ifndef __ANIMAL3D_DEMO_KEYFRAMES_H
#define __ANIMAL3D_DEMO_KEYFRAMES_H


//-----------------------------------------------------------------------------

#include "animal3D/animal3D.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_Demo_Keyframes						a3_Demo_Keyframes;
	typedef enum a3_Demo_Keyframes_RenderProgramName		a3_Demo_Keyframes_RenderProgramName;
	typedef enum a3_Demo_Keyframes_DisplayProgramName		a3_Demo_Keyframes_DisplayProgramName;
	typedef enum a3_Demo_Keyframes_ActiveCameraName			a3_Demo_Keyframes_ActiveCameraName;
	typedef enum a3_Demo_Keyframes_PipelineName				a3_Demo_Keyframes_PipelineName;
	typedef enum a3_Demo_Keyframes_PassName					a3_Demo_Keyframes_PassName;
	typedef enum a3_Demo_Keyframes_TargetName				a3_Demo_Keyframes_TargetName;
	typedef enum a3_Demo_Keyframes_InterpolationModeName	a3_Demo_Keyframes_InterpolationModeName;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// scene object rendering program names
	enum a3_Demo_Keyframes_RenderProgramName
	{
		keyframes_renderPhong,			// Phong shading

		keyframes_render_max
	};

	// final display modes
	enum a3_Demo_Keyframes_DisplayProgramName
	{
		keyframes_displayTexture,		// display simple texture
		keyframes_displayOutline,		// display texture with outlines

		keyframes_display_max
	};

	// active camera names
	enum a3_Demo_Keyframes_ActiveCameraName
	{
		keyframes_cameraSceneViewer,	// scene viewing camera
		keyframes_cameraShadowLight,	// shadow map capturing light

		keyframes_camera_max
	};


	// pipeline names
	enum a3_Demo_Keyframes_PipelineName
	{
		keyframes_forward,				// forward lighting pipeline

		keyframes_pipeline_max
	};

	// render passes
	enum a3_Demo_Keyframes_PassName
	{
		keyframes_passShadow,			// capture shadow map

		keyframes_passScene,			// render scene objects

		keyframes_passComposite,		// composite for post-processing stage

		keyframes_passBright_2,		// bright pass for bloom (half screen size)
		keyframes_passBlurH_2,			// horizontal blur for bloom (half screen size)
		keyframes_passBlurV_2,			// horizontal blur for bloom (half screen size)
		keyframes_passBright_4,		// bright pass for bloom (quarter screen size)
		keyframes_passBlurH_4,			// horizontal blur for bloom (quarter screen size)
		keyframes_passBlurV_4,			// horizontal blur for bloom (quarter screen size)
		keyframes_passBright_8,		// bright pass for bloom (eighth screen size)
		keyframes_passBlurH_8,			// horizontal blur for bloom (eighth screen size)
		keyframes_passBlurV_8,			// horizontal blur for bloom (eighth screen size)
		keyframes_passBlend,			// bloom composite pass

		keyframes_pass_max
	};

	// render target names
	enum a3_Demo_Keyframes_TargetName
	{
		keyframes_shadow_fragdepth = 0,		// fragment depth
		keyframes_target_shadow_max, 

		keyframes_scene_finalcolor = 0,		// final display color
		keyframes_scene_position,				// position attribute
		keyframes_scene_normal,				// normal attribute
		keyframes_scene_texcoord,				// texcoord attribute
		keyframes_scene_shadowcoord,			// shadow coordinate
		keyframes_scene_shadowtest,			// result of shadow test
		keyframes_scene_diffuseLight,			// diffuse light total
		keyframes_scene_specularLight,			// specular light total
		keyframes_scene_fragdepth,				// fragment depth
		keyframes_target_scene_max, 

		keyframes_composite_finalcolor = 0,	// final display color
		keyframes_target_composite_max,

		keyframes_bright_finalcolor = 0,		// final display color
		keyframes_bright_luminance,			// luminance of color
		keyframes_target_bright_max,

		keyframes_blur_finalcolor = 0,			// final display color
		keyframes_target_blur_max,

		keyframes_display_finalcolor = 0,		// final display color
		keyframes_target_display_max,
	};


	// interpolation mode
	enum a3_Demo_Keyframes_InterpolationModeName
	{
		keyframes_interpNone,
		keyframes_interpLerp,
		keyframes_interpBezier,
		keyframes_interpCatmullRom,
		keyframes_interpCubicHermite,

		keyframes_interp_max
	};


//-----------------------------------------------------------------------------

	// demo mode for basic shading
	struct a3_Demo_Keyframes
	{
		a3_Demo_Keyframes_RenderProgramName render;
		a3_Demo_Keyframes_DisplayProgramName display;
		a3_Demo_Keyframes_ActiveCameraName activeCamera;

		a3_Demo_Keyframes_PipelineName pipeline;
		a3_Demo_Keyframes_PassName pass;
		a3_Demo_Keyframes_TargetName targetIndex[keyframes_pass_max], targetCount[keyframes_pass_max];

		a3_Demo_Keyframes_InterpolationModeName interp;

		// skeletal controls
		a3ui32 editSkeletonIndex;
		a3ui32 editJointIndex;
		a3boolean editingJoint;
	};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMO_KEYFRAMES_H