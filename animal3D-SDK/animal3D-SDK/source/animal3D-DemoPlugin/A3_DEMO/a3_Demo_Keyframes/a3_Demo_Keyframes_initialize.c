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

	a3_Demo_Keyframes_initialize.c
	Demo mode implementations: curves & interpolation initialization.

	********************************************
	*** INITIALIZATION FOR CURVE INTERP MODE ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_Demo_Keyframes.h"

typedef struct a3_DemoState a3_DemoState;
//#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------

void a3keyframes_init(a3_DemoState const* demoState, a3_Demo_Keyframes* demoMode)
{
	demoMode->render = keyframes_renderPhong;
	demoMode->display = keyframes_displayTexture;
	demoMode->activeCamera = keyframes_cameraSceneViewer;

	demoMode->pipeline = keyframes_forward;
	demoMode->pass = keyframes_passComposite;

	demoMode->targetIndex[keyframes_passShadow] = keyframes_shadow_fragdepth;
	demoMode->targetIndex[keyframes_passScene] = keyframes_scene_finalcolor;
	demoMode->targetIndex[keyframes_passComposite] = keyframes_composite_finalcolor;
	demoMode->targetIndex[keyframes_passBright_2] = keyframes_bright_finalcolor;
	demoMode->targetIndex[keyframes_passBlurH_2] = keyframes_blur_finalcolor;
	demoMode->targetIndex[keyframes_passBlurV_2] = keyframes_blur_finalcolor;
	demoMode->targetIndex[keyframes_passBright_4] = keyframes_bright_finalcolor;
	demoMode->targetIndex[keyframes_passBlurH_4] = keyframes_blur_finalcolor;
	demoMode->targetIndex[keyframes_passBlurV_4] = keyframes_blur_finalcolor;
	demoMode->targetIndex[keyframes_passBright_8] = keyframes_bright_finalcolor;
	demoMode->targetIndex[keyframes_passBlurH_8] = keyframes_blur_finalcolor;
	demoMode->targetIndex[keyframes_passBlurV_8] = keyframes_blur_finalcolor;
	demoMode->targetIndex[keyframes_passBlend] = keyframes_display_finalcolor;

	demoMode->targetCount[keyframes_passShadow] = keyframes_target_shadow_max;
	demoMode->targetCount[keyframes_passScene] = keyframes_target_scene_max;
	demoMode->targetCount[keyframes_passComposite] = keyframes_target_composite_max;
	demoMode->targetCount[keyframes_passBright_2] = keyframes_target_bright_max;
	demoMode->targetCount[keyframes_passBlurH_2] = keyframes_target_blur_max;
	demoMode->targetCount[keyframes_passBlurV_2] = keyframes_target_blur_max;
	demoMode->targetCount[keyframes_passBright_4] = keyframes_target_bright_max;
	demoMode->targetCount[keyframes_passBlurH_4] = keyframes_target_blur_max;
	demoMode->targetCount[keyframes_passBlurV_4] = keyframes_target_blur_max;
	demoMode->targetCount[keyframes_passBright_8] = keyframes_target_bright_max;
	demoMode->targetCount[keyframes_passBlurH_8] = keyframes_target_blur_max;
	demoMode->targetCount[keyframes_passBlurV_8] = keyframes_target_blur_max;
	demoMode->targetCount[keyframes_passBlend] = keyframes_target_display_max;

	demoMode->interp = keyframes_interpNone;
}


//-----------------------------------------------------------------------------
