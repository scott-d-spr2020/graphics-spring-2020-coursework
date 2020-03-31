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

	a3_Demo_Pipelines_initialize.c
	Demo mode implementations: pipelines initialization.

	********************************************
	*** INITIALIZATION FOR PIPELINES MODE    ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_Demo_Pipelines.h"

typedef struct a3_DemoState a3_DemoState;
//#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------

void a3pipelines_init(a3_DemoState const* demoState, a3_Demo_Pipelines* demoMode)
{
	demoMode->render = pipelines_renderPhong;
	demoMode->display = pipelines_displayTexture;
	demoMode->activeCamera = pipelines_cameraSceneViewer;

	demoMode->pipeline = pipelines_forward;
	demoMode->pass = pipelines_passScene;

	demoMode->targetIndex[pipelines_passShadow] = pipelines_shadow_fragdepth;
	demoMode->targetIndex[pipelines_passScene] = pipelines_scene_finalcolor;
	demoMode->targetIndex[pipelines_passLighting] = pipelines_composite_finalcolor;
	demoMode->targetIndex[pipelines_passSSAO] = pipelines_ssao_finalcolor;
	demoMode->targetIndex[pipelines_passBlurSSAOH] = pipelines_ssao_finalcolor;
	demoMode->targetIndex[pipelines_passBlurSSAOV] = pipelines_ssao_finalcolor;
	demoMode->targetIndex[pipelines_passSSAO] = pipelines_ssao_finalcolor;
	demoMode->targetIndex[pipelines_passComposite] = pipelines_composite_finalcolor;
	demoMode->targetIndex[pipelines_passBright_2] = pipelines_bright_finalcolor;
	demoMode->targetIndex[pipelines_passBlurH_2] = pipelines_blur_finalcolor;
	demoMode->targetIndex[pipelines_passBlurV_2] = pipelines_blur_finalcolor;
	demoMode->targetIndex[pipelines_passBright_4] = pipelines_bright_finalcolor;
	demoMode->targetIndex[pipelines_passBlurH_4] = pipelines_blur_finalcolor;
	demoMode->targetIndex[pipelines_passBlurV_4] = pipelines_blur_finalcolor;
	demoMode->targetIndex[pipelines_passBright_8] = pipelines_bright_finalcolor;
	demoMode->targetIndex[pipelines_passBlurH_8] = pipelines_blur_finalcolor;
	demoMode->targetIndex[pipelines_passBlurV_8] = pipelines_blur_finalcolor;
	demoMode->targetIndex[pipelines_passBlend] = pipelines_display_finalcolor;

	demoMode->targetCount[pipelines_passShadow] = pipelines_target_shadow_max;
	demoMode->targetCount[pipelines_passScene] = pipelines_target_scene_max;
	demoMode->targetCount[pipelines_passLighting] = pipelines_target_composite_max;
	demoMode->targetCount[pipelines_passSSAO] = pipelines_target_ssao_max;
	demoMode->targetCount[pipelines_passBlurSSAOH] = pipelines_target_ssao_max;
	demoMode->targetCount[pipelines_passBlurSSAOV] = pipelines_target_ssao_max;
	demoMode->targetCount[pipelines_passComposite] = pipelines_target_composite_max;
	demoMode->targetCount[pipelines_passBright_2] = pipelines_target_bright_max;
	demoMode->targetCount[pipelines_passBlurH_2] = pipelines_target_blur_max;
	demoMode->targetCount[pipelines_passBlurV_2] = pipelines_target_blur_max;
	demoMode->targetCount[pipelines_passBright_4] = pipelines_target_bright_max;
	demoMode->targetCount[pipelines_passBlurH_4] = pipelines_target_blur_max;
	demoMode->targetCount[pipelines_passBlurV_4] = pipelines_target_blur_max;
	demoMode->targetCount[pipelines_passBright_8] = pipelines_target_bright_max;
	demoMode->targetCount[pipelines_passBlurH_8] = pipelines_target_blur_max;
	demoMode->targetCount[pipelines_passBlurV_8] = pipelines_target_blur_max;
	demoMode->targetCount[pipelines_passBlend] = pipelines_target_display_max;
}


//-----------------------------------------------------------------------------
