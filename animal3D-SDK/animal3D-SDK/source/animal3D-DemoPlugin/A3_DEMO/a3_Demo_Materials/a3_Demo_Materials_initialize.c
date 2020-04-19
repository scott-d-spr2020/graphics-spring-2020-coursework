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

	a3_Demo_Materials_initialize.c
	Demo mode implementations: curves & interpolation initialization.

	********************************************
	*** INITIALIZATION FOR CURVE INTERP MODE ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_Demo_Materials.h"

typedef struct a3_DemoState a3_DemoState;
//#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------

void a3materials_init(a3_DemoState const* demoState, a3_Demo_Materials* demoMode)
{
	demoMode->render = materials_renderPhong;
	demoMode->display = materials_displayTexture;
	demoMode->activeCamera = materials_cameraSceneViewer;

	demoMode->pipeline = materials_forward;
	demoMode->pass = materials_passComposite;

	demoMode->targetIndex[materials_passShadow] = materials_shadow_fragdepth;
	demoMode->targetIndex[materials_passScene] = materials_scene_finalcolor;
	demoMode->targetIndex[materials_passComposite] = materials_composite_finalcolor;
	demoMode->targetIndex[materials_passBright_2] = materials_bright_finalcolor;
	demoMode->targetIndex[materials_passBlurH_2] = materials_blur_finalcolor;
	demoMode->targetIndex[materials_passBlurV_2] = materials_blur_finalcolor;
	demoMode->targetIndex[materials_passBright_4] = materials_bright_finalcolor;
	demoMode->targetIndex[materials_passBlurH_4] = materials_blur_finalcolor;
	demoMode->targetIndex[materials_passBlurV_4] = materials_blur_finalcolor;
	demoMode->targetIndex[materials_passBright_8] = materials_bright_finalcolor;
	demoMode->targetIndex[materials_passBlurH_8] = materials_blur_finalcolor;
	demoMode->targetIndex[materials_passBlurV_8] = materials_blur_finalcolor;
	demoMode->targetIndex[materials_passBlend] = materials_display_finalcolor;

	demoMode->targetCount[materials_passShadow] = materials_target_shadow_max;
	demoMode->targetCount[materials_passScene] = materials_target_scene_max;
	demoMode->targetCount[materials_passComposite] = materials_target_composite_max;
	demoMode->targetCount[materials_passBright_2] = materials_target_bright_max;
	demoMode->targetCount[materials_passBlurH_2] = materials_target_blur_max;
	demoMode->targetCount[materials_passBlurV_2] = materials_target_blur_max;
	demoMode->targetCount[materials_passBright_4] = materials_target_bright_max;
	demoMode->targetCount[materials_passBlurH_4] = materials_target_blur_max;
	demoMode->targetCount[materials_passBlurV_4] = materials_target_blur_max;
	demoMode->targetCount[materials_passBright_8] = materials_target_bright_max;
	demoMode->targetCount[materials_passBlurH_8] = materials_target_blur_max;
	demoMode->targetCount[materials_passBlurV_8] = materials_target_blur_max;
	demoMode->targetCount[materials_passBlend] = materials_target_display_max;

	demoMode->interp = materials_interpNone;
}


//-----------------------------------------------------------------------------
