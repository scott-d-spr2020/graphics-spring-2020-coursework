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
	
	a3_Demo_Materials_idle-update.c
	Demo mode implementations: curves & interpolation update.

	********************************************
	*** UPDATE FOR CURVE INTERP DEMO MODE    ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_Demo_Materials.h"

typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"
#include "../_a3_demo_utilities/a3_DemoPoseUtils.h"


//-----------------------------------------------------------------------------
// UPDATE

int poseVal = 0;
void a3materials_update(a3_DemoState* demoState, a3_Demo_Materials* demoMode, a3f64 dt)
{
	a3ui32 i, j;

	const a3_DemoProjector* activeCamera = demoState->projector + demoState->activeCamera;
	const a3_DemoSceneObject* activeCameraObject = activeCamera->sceneObject;
	a3_DemoPointLight* pointLight;

	
	
	
}


//-----------------------------------------------------------------------------
