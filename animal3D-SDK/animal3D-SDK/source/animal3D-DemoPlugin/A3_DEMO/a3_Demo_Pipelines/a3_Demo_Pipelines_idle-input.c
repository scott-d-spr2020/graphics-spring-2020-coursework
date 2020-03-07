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

	a3_Demo_Pipelines_idle-input.c
	Demo mode implementations: pipelines input.

	********************************************
	*** INPUT PROCESSING FOR PIPELINES MODE  ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_Demo_Pipelines.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"


//-----------------------------------------------------------------------------
// CALLBACKS

// main demo mode callback
void a3pipelinesCB_input_keyCharPress(a3_DemoState const* demoState, a3_Demo_Pipelines* demoMode, a3i32 asciiKey)
{
	switch (asciiKey)
	{
		// toggle render program
		a3demoCtrlCasesLoop(demoMode->render, pipelines_render_max, 'k', 'j');

		// toggle display program
		a3demoCtrlCasesLoop(demoMode->display, pipelines_display_max, 'K', 'J');

		// toggle active camera
		a3demoCtrlCasesLoop(demoMode->activeCamera, pipelines_camera_max, 'v', 'c');

		// toggle pipeline mode
		a3demoCtrlCasesLoop(demoMode->pipeline, pipelines_pipeline_max, ']', '[');

		// toggle target
		a3demoCtrlCasesLoop(demoMode->targetIndex[demoMode->pass], demoMode->targetCount[demoMode->pass], '}', '{');

		// toggle pass to display
	//	a3demoCtrlCasesLoop(demoMode->pass, pipelines_pass_max, ')', '(');
	case ')':
		a3demoCtrlIncLoop(demoMode->pass, pipelines_pass_max);
	case 'I':	// if toggle is switched, do the following
		if (demoState->skipIntermediatePasses)
		{
			a3demoCtrlIncClamp(demoMode->pass, pipelines_passBlend, pipelines_passComposite);
			a3demoCtrlIncClamp(demoMode->pass, pipelines_passComposite, pipelines_passScene);
			a3demoCtrlIncClamp(demoMode->pass, pipelines_passScene, -1);
		}
		break;
	case '(':
		a3demoCtrlDecLoop(demoMode->pass, pipelines_pass_max);
		if (demoState->skipIntermediatePasses)
		{
			a3demoCtrlDecClamp(demoMode->pass, pipelines_passBlend, pipelines_passComposite);
			a3demoCtrlDecClamp(demoMode->pass, pipelines_passComposite, pipelines_passScene);
			a3demoCtrlDecClamp(demoMode->pass, pipelines_passScene, -1);
			demoMode->pass = (demoMode->pass + pipelines_pass_max) % pipelines_pass_max;
		}
		break;
	}


	// skip passes if not on the correct pipeline
	switch (asciiKey)
	{
	case ']':
	case '[':
	case ')':
		if (demoMode->pass == pipelines_passLighting)
		{
			switch (demoMode->pipeline)
			{
			case pipelines_deferred_lighting:
				demoMode->pass = pipelines_passScene + 1;
				break;
			case pipelines_deferred_ssao:
				demoMode->pass = pipelines_passScene + 2;
				break;
			default:
				demoMode->pass = pipelines_passComposite;
			}
		}
	case '(':
		if (demoMode->pass == pipelines_passLighting && demoMode->pipeline != pipelines_deferred_lighting)
			demoMode->pass = pipelines_passScene;
		break;
	}
}


//-----------------------------------------------------------------------------
