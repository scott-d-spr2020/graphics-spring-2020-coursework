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
	
	a3_DemoState_initialize.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     INITIALIZATION in this file.             ***
	****************************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"
#include <stdio.h>


//-----------------------------------------------------------------------------

void a3demo_initScene(a3_DemoState* demoState);
void a3demo_initSceneRefresh(a3_DemoState* demoState);


//-----------------------------------------------------------------------------

void a3shading_init(a3_DemoState const* demoState, a3_Demo_Shading* demoMode);
void a3pipelines_init(a3_DemoState const* demoState, a3_Demo_Pipelines* demoMode);


//-----------------------------------------------------------------------------
// INITIALIZE

// initialize non-asset objects
void a3demo_initScene(a3_DemoState *demoState)
{
	a3ui32 i;
	a3_DemoProjector* projector;
	a3_DemoPointLight* pointLight;

	// camera's starting orientation depends on "vertical" axis
	// we want the exact same view in either case
	const a3real sceneCameraAxisPos = 20.0f;
	const a3vec3 sceneCameraStartPos = {
		demoState->verticalAxis ? +sceneCameraAxisPos : +sceneCameraAxisPos,
		demoState->verticalAxis ? +sceneCameraAxisPos : -sceneCameraAxisPos,
		demoState->verticalAxis ? +sceneCameraAxisPos : +sceneCameraAxisPos,
	};
	const a3vec3 sceneCameraStartEuler = {
		demoState->verticalAxis ? -35.0f : +55.0f,
		demoState->verticalAxis ? +45.0f : + 0.0f,
		demoState->verticalAxis ? + 0.0f : +45.0f,
	};


	// refresh scene objects
	a3demo_initSceneRefresh(demoState);


	// all objects
	for (i = 0; i < demoStateMaxCount_sceneObject; ++i)
		a3demo_initSceneObject(demoState->sceneObject + i);
	for (i = 0; i < demoStateMaxCount_cameraObject; ++i)
		a3demo_initSceneObject(demoState->cameraObject + i);

	// cameras
	a3demo_initProjector(demoState->sceneCamera);
	a3demo_initProjector(demoState->shadowLight);

	// scene cameras
	projector = demoState->sceneCamera + 0;
	projector->perspective = a3true;
	projector->fovy = a3real_fortyfive;
	projector->znear = 1.0f;
	projector->zfar = 1000.0f;
	projector->ctrlMoveSpeed = 10.0f;
	projector->ctrlRotateSpeed = 5.0f;
	projector->ctrlZoomSpeed = 5.0f;
	projector->sceneObject->position = sceneCameraStartPos;
	projector->sceneObject->euler = sceneCameraStartEuler;

	// additional cameras
	projector = demoState->shadowLight + 0;
	projector->perspective = a3true;
	projector->fovy = a3real_ninety;
	projector->znear = 10.0f;
	projector->zfar = 100.0f;
	projector->ctrlMoveSpeed = 10.0f;
	projector->ctrlRotateSpeed = 5.0f;
	projector->ctrlZoomSpeed = 5.0f;
	projector->sceneObject->position = sceneCameraStartPos;
	projector->sceneObject->euler = sceneCameraStartEuler;


	// init transforms
	if (demoState->verticalAxis)
	{
		// vertical axis is Y
	}
	else
	{
		// vertical axis is Z
	}


	// initialize other objects and settings
	demoState->displayGrid = a3true;
	demoState->displayWorldAxes = a3true;
	demoState->displayObjectAxes = a3true;
	demoState->displayTangentBases = a3false;
	demoState->displaySkybox = a3true;
	demoState->displayHiddenVolumes = a3true;
	demoState->displayPipeline = a3false;
	demoState->updateAnimation = a3true;
	demoState->stencilTest = a3false;
	demoState->skipIntermediatePasses = a3false;


	// lights
	demoState->forwardLightCount = demoStateMaxCount_lightObject;
	demoState->deferredLightCount = demoStateMaxCount_lightVolumePerBlock / 32;

	// first light position is hard-coded (starts at camera)
	demoState->mainLightObject->position = demoState->mainCameraObject->position;
	demoState->mainLightObject->euler = demoState->mainCameraObject->euler;
	demoState->mainLightObject->scale = a3vec3_one;
	pointLight = demoState->forwardPointLight;
	pointLight->worldPos = a3vec4_w;
	pointLight->worldPos.xyz = demoState->mainLightObject->position;
	pointLight->radius = 100.0f;
	pointLight->radiusInvSq = a3recip(pointLight->radius * pointLight->radius);
	pointLight->color = a3vec4_one;

	// all other lights are pseudo-random
	a3randomSetSeed(2048);	// 0, 512, 2048, 8192, 65536
	for (i = 1, pointLight = demoState->forwardPointLight + i;
		i < demoStateMaxCount_lightObject;
		++i, ++pointLight)
	{
		// set to zero vector
		pointLight->worldPos = a3vec4_w;

		// random positions
		pointLight->worldPos.x = a3randomRange(-10.0f, +10.0f);
		if (demoState->verticalAxis)
		{
			pointLight->worldPos.z = -a3randomRange(-10.0f, +10.0f);
			pointLight->worldPos.y = -a3randomRange(-2.0f, +8.0f);
		}
		else
		{
			pointLight->worldPos.y = a3randomRange(-10.0f, +10.0f);
			pointLight->worldPos.z = a3randomRange(-2.0f, +8.0f);
		}

		// random colors
		pointLight->color.r = a3randomNormalized();
		pointLight->color.g = a3randomNormalized();
		pointLight->color.b = a3randomNormalized();
		pointLight->color.a = a3real_one;

		// random radius
		pointLight->radius = a3randomRange(10.0f, 50.0f);
		pointLight->radiusInvSq = a3recip(pointLight->radius * pointLight->radius);
	}

	// deferred lights
	for (i = 0, pointLight = demoState->deferredPointLight + i;
		i < demoStateMaxCount_lightVolume;
		++i, ++pointLight)
	{
		// set to zero vector
		pointLight->worldPos = a3vec4_w;

		// random positions
		pointLight->worldPos.x = a3randomRange(-6.0f, +6.0f);
		if (demoState->verticalAxis)
		{
			pointLight->worldPos.z = -a3randomRange(-6.0f, +6.0f);
			pointLight->worldPos.y = -a3randomRange(-2.0f, +4.0f);
		}
		else
		{
			pointLight->worldPos.y = a3randomRange(-6.0f, +6.0f);
			pointLight->worldPos.z = a3randomRange(-2.0f, +4.0f);
		}

		// random colors
		pointLight->color.r = a3randomNormalized();
		pointLight->color.g = a3randomNormalized();
		pointLight->color.b = a3randomNormalized();
		pointLight->color.a = a3real_one;

		// random radius: they should be small!
		pointLight->radius = a3randomRange(0.25f, 0.50f);
		pointLight->radiusInvSq = a3recip(pointLight->radius * pointLight->radius);
	}


	// position scene objects
	demoState->sphereObject->scale.x = 2.0f;
	demoState->cylinderObject->scale.x = 4.0f;
	demoState->cylinderObject->scale.y = demoState->cylinderObject->scale.z = 2.0f;
	demoState->torusObject->scale.x = 2.0f;
	demoState->sphereObject->scaleMode = 1;		// uniform
	demoState->cylinderObject->scaleMode = 2;	// non-uniform
	demoState->torusObject->scaleMode = 1;		// uniform

	demoState->sphereObject->position.x = +6.0f;
	demoState->torusObject->position.x = -6.0f;
	if (demoState->verticalAxis)
	{
		demoState->planeObject->position.y = -2.0f;
		demoState->sphereObject->position.y = +1.0f;
		demoState->cylinderObject->position.y = +1.0f;
		demoState->torusObject->position.y = +1.0f;
		demoState->cylinderObject->position.z = -6.0f;
		demoState->teapotObject->position.z = +6.0f;
	}
	else
	{
		demoState->planeObject->position.z = -2.0f;
		demoState->sphereObject->position.z = +1.0f;
		demoState->cylinderObject->position.z = +1.0f;
		demoState->torusObject->position.z = +1.0f;
		demoState->cylinderObject->position.y = +6.0f;
		demoState->teapotObject->position.y = -6.0f;
	}

	//initialize control points for bezier
	demoState->controlPoint0 = demoState->teapotObject->position; //not a teapot, actually a crate
	demoState->controlPoint1 = demoState->torusObject->position;
	demoState->controlPoint2 = demoState->cylinderObject->position;
	demoState->controlPoint3 = demoState->sphereObject->position;
	printf("%f, %f, %f\n", demoState->controlPoint0.x, demoState->controlPoint0.y, demoState->controlPoint0.z);
	printf("%f, %f, %f\n", demoState->controlPoint1.x, demoState->controlPoint1.y, demoState->controlPoint1.z);
	printf("%f, %f, %f\n", demoState->controlPoint2.x, demoState->controlPoint2.y, demoState->controlPoint2.z);
	printf("%f, %f, %f\n", demoState->controlPoint3.x, demoState->controlPoint3.y, demoState->controlPoint3.z);

	// Generate the kernel once, always use this one
	genKernel(demoState->kernel, 64);

	// demo modes
	a3shading_init(demoState, demoState->demoMode_shading);
	a3pipelines_init(demoState, demoState->demoMode_pipelines);
	demoState->demoMode = demoState_pipelines;

	// active camera params
	demoState->activeCamera = demoState->demoMode_pipelines->activeCamera;
}

// refresh non-asset scene objects (e.g. re-link pointers)
void a3demo_initSceneRefresh(a3_DemoState *demoState)
{
	a3demo_setProjectorSceneObject(demoState->sceneCamera, demoState->mainCameraObject);
	a3demo_setProjectorSceneObject(demoState->shadowLight, demoState->mainLightObject);
}


//-----------------------------------------------------------------------------
