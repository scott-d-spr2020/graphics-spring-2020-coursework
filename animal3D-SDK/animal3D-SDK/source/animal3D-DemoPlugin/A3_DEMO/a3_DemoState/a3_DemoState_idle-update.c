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
	
	a3_DemoState_idle-update.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     UPDATING THE STATE in this file.         ***
	****************************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"
#include "../_a3_demo_utilities/a3_DemoSSAOUtils.h"
#include <stdio.h>


//-----------------------------------------------------------------------------
// GENERAL UTILITIES

inline void a3demo_applyScale_internal(a3_DemoSceneObject *sceneObject, a3real4x4p s)
{
	if (sceneObject->scaleMode)
	{
		if (sceneObject->scaleMode == 1)
		{
			s[0][0] = s[1][1] = s[2][2] = sceneObject->scale.x;
			a3real4x4ConcatL(sceneObject->modelMat.m, s);
			a3real4x4TransformInverseUniformScale(sceneObject->modelMatInv.m, sceneObject->modelMat.m);
		}
		else
		{
			s[0][0] = sceneObject->scale.x;
			s[1][1] = sceneObject->scale.y;
			s[2][2] = sceneObject->scale.z;
			a3real4x4ConcatL(sceneObject->modelMat.m, s);
			a3real4x4TransformInverse(sceneObject->modelMatInv.m, sceneObject->modelMat.m);
		}
	}
	else
		a3real4x4TransformInverseIgnoreScale(sceneObject->modelMatInv.m, sceneObject->modelMat.m);
}


//-----------------------------------------------------------------------------
// UPDATE SUB-ROUTINES

// scene update
void a3demo_update_scene(a3_DemoState *demoState, a3f64 dt)
{
	a3ui32 i;

	const a3f32 dr = demoState->updateAnimation ? (a3f32)dt * 15.0f : 0.0f;

	const a3i32 useVerticalY = demoState->verticalAxis;

	// model transformations (if needed)
	const a3mat4 convertY2Z = {
		+1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, +1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, +1.0f,
	};
	const a3mat4 convertZ2Y = {
		+1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, +1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, +1.0f,
	};
	const a3mat4 convertZ2X = {
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, +1.0f, 0.0f, 0.0f,
		+1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, +1.0f,
	};


	// bias matrix
	const a3mat4 bias = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
	};


	// tmp matrix for scale
	a3mat4 scaleMat = a3mat4_identity;

	// active camera
	a3_DemoProjector *activeCamera = demoState->projector + demoState->activeCamera;
	a3_DemoSceneObject *activeCameraObject = activeCamera->sceneObject;
	a3_DemoSceneObject *currentSceneObject;

	// light pointers
	a3_DemoPointLight* pointLight;


	demoState->lerpPos += (a3real)(demoState->updateAnimation ? dt / 5.0f : 0.0f);
	//oscillation function. f(x) = 1 - abs(1 - mod(2-x, 2)). I've manually derived this function a few times (the zigzag function from googling is MUCH more complex and this works fine)
	//demoState->lerpPos = (a3real) (1.0 - fabs(1.0 - fmod(2.0 - demoState->lerpPos, 2.0)));
	float pos = (a3real)(1.0 - fabs(1.0 - mathMod((a3real)(2.0 - demoState->lerpPos), 2.0)));

	// do simple animation
	if (useVerticalY)
	{
		for (i = 0, currentSceneObject = demoState->sphereObject;
			i < 4; ++i, ++currentSceneObject)
		{
			currentSceneObject->euler.y += dr;
			currentSceneObject->euler.y = a3trigValid_sind(currentSceneObject->euler.y);
		}
	}
	else
	{
		for (i = 0, currentSceneObject = demoState->sphereObject;
			i < 4; ++i, ++currentSceneObject)
		{
			currentSceneObject->euler.z += dr;
			currentSceneObject->euler.z = a3trigValid_sind(currentSceneObject->euler.z);
		}
	}

	//sets the position to a bezier curve
	demoState->teapotObject->position = bezierN(demoState->controlPoints, 4, pos);


	// update scene objects
	for (i = 0; i < demoStateMaxCount_sceneObject; ++i)
		a3demo_updateSceneObject(demoState->sceneObject + i, 0);
	for (i = 0; i < demoStateMaxCount_cameraObject; ++i)
		a3demo_updateSceneObject(demoState->cameraObject + i, 1);
	for (i = 0; i < demoStateMaxCount_lightObject; ++i)
		a3demo_updateSceneObject(demoState->lighObject + i, 1);

	// update cameras/projectors
	for (i = 0; i < demoStateMaxCount_projector; ++i)
		a3demo_updateProjectorViewProjectionMat(demoState->projector + i);


	// apply corrections if required
	// grid
	demoState->gridTransform = useVerticalY ? convertZ2Y : a3mat4_identity;

	// skybox position
	demoState->skyboxObject->modelMat.v3 = activeCameraObject->modelMat.v3;


	// grid lines highlight
	// if Y axis is up, give it a greenish hue
	// if Z axis is up, a bit of blue
	demoState->gridColor = a3vec4_w;
	if (useVerticalY)
		demoState->gridColor.g = 0.25f;
	else
		demoState->gridColor.b = 0.25f;


	// update lights
	pointLight = demoState->forwardPointLight;
	pointLight->worldPos.xyz = demoState->mainLightObject->position;

	// update lights view positions for current camera
	for (i = 0, pointLight = demoState->forwardPointLight + i;
		i < demoState->forwardLightCount;
		++i, ++pointLight)
	{
		// convert to view space and retrieve view position
		a3real4Real4x4Product(pointLight->viewPos.v, activeCameraObject->modelMatInv.m, pointLight->worldPos.v);
	}


	// correct rotations as needed
	if (useVerticalY)
	{
		// plane's axis is Z
		a3real4x4ConcatL(demoState->planeObject->modelMat.m, convertZ2Y.m);

		// sphere's axis is Z
		a3real4x4ConcatL(demoState->sphereObject->modelMat.m, convertZ2Y.m);
	}
	else
	{
		// need to rotate skybox if Z-up
		a3real4x4ConcatL(demoState->skyboxObject->modelMat.m, convertY2Z.m);

		// teapot's axis is Y
		a3real4x4ConcatL(demoState->teapotObject->modelMat.m, convertY2Z.m);
	}


	// apply scales
	a3demo_applyScale_internal(demoState->sphereObject, scaleMat.m);
	a3demo_applyScale_internal(demoState->cylinderObject, scaleMat.m);
	a3demo_applyScale_internal(demoState->torusObject, scaleMat.m);
	a3demo_applyScale_internal(demoState->teapotObject, scaleMat.m);
}


//-----------------------------------------------------------------------------
// UPDATE

void a3pipelines_update(a3_DemoState* demoState, a3_Demo_Pipelines* demoMode, a3f64 dt);
void a3keyframes_update(a3_DemoState* demoState, a3_Demo_Keyframes* demoMode, a3f64 dt);
void a3materials_update(a3_DemoState* demoState, a3_Demo_Materials* demoMode, a3f64 dt);

void a3demo_update(a3_DemoState *demoState, a3f64 dt)
{
	// update scene
	a3demo_update_scene(demoState, dt);

	// update for specific mode
	switch (demoState->demoMode)
	{
	case demoState_shading:
		break;
	case demoState_pipelines:
		a3pipelines_update(demoState, demoState->demoMode_pipelines, dt);
		break;
	case demoState_keyframes:
		a3keyframes_update(demoState, demoState->demoMode_keyframes, dt);
		break;
	case demoState_materials:
		a3materials_update(demoState, demoState->demoMode_materials, dt);
		break;
	}
}


//-----------------------------------------------------------------------------
