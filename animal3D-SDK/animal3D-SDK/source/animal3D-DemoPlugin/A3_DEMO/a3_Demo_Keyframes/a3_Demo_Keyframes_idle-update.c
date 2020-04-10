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
	
	a3_Demo_Keyframes_idle-update.c
	Demo mode implementations: curves & interpolation update.

	********************************************
	*** UPDATE FOR CURVE INTERP DEMO MODE    ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_Demo_Keyframes.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoMacros.h"
#include "../_a3_demo_utilities/a3_DemoPoseUtils.h"


//-----------------------------------------------------------------------------
// UPDATE

int poseVal = 0;
void a3keyframes_update(a3_DemoState* demoState, a3_Demo_Keyframes* demoMode, a3f64 dt)
{
	a3ui32 i, j;

	const a3_DemoProjector* activeCamera = demoState->projector + demoState->activeCamera;
	const a3_DemoSceneObject* activeCameraObject = activeCamera->sceneObject;
	a3_DemoPointLight* pointLight;

	a3_DemoModelMatrixStack matrixStack[demoStateMaxCount_sceneObject];

	// other objects
	a3_HierarchyState* currentHierarchyState;
	const a3_HierarchyPoseGroup* currentHierarchyPoseGroup;
	const a3_Hierarchy* currentHierarchy;

	// update matrix stack data
	for (i = 0; i < demoStateMaxCount_sceneObject; ++i)
	{
		a3demo_updateModelMatrixStack(matrixStack + i,
			activeCamera->projectionMat.m, activeCameraObject->modelMat.m, activeCameraObject->modelMatInv.m,
			(demoState->sceneObject + i)->modelMat.m, a3mat4_identity.m);
	}

	// send matrix stack data
	a3bufferRefill(demoState->ubo_transformStack_model, 0, sizeof(matrixStack), matrixStack);

	// send point light data
	pointLight = demoState->forwardPointLight;
	a3bufferRefill(demoState->ubo_pointLight, 0, demoState->forwardLightCount * sizeof(a3_DemoPointLight), pointLight);

	// send curve data
	i = a3bufferRefill(demoState->ubo_curveWaypoint, 0, sizeof(demoState->curveWaypoint), demoState->curveWaypoint);
	a3bufferRefillOffset(demoState->ubo_curveWaypoint, 0, i, sizeof(demoState->curveHandle), demoState->curveHandle);


	// update animation
	if (demoMode->interp)
	{
		a3ui32 k[4] = { 0 };
	//	a3vec4 m[2] = { 0 };

		// update controller
		if (demoState->updateAnimation)
			demoState->segmentTime += (a3real)dt;
		if (demoState->segmentTime >= demoState->segmentDuration)
		{
			demoState->segmentTime -= demoState->segmentDuration;
			demoState->segmentIndex = (demoState->segmentIndex + 1) % demoState->segmentCount;
		}
		demoState->segmentParam = demoState->segmentTime * demoState->segmentDurationInv;

		// set key indices
		k[0] = i = demoState->segmentIndex;
		k[1] = (i + 1) % demoState->segmentCount;
		k[2] = (i + 2) % demoState->segmentCount;
		k[3] = (i + demoState->segmentCount - 1) % demoState->segmentCount;

		// perform position interpolation on current segment
		switch (demoMode->interp)
		{
		case keyframes_interpLerp:
			a3real3Lerp(demoState->skeletonObject->position.v,
				demoState->curveWaypoint[k[0]].v,
				demoState->curveWaypoint[k[1]].v,
				demoState->segmentParam);
			break;
		case keyframes_interpBezier:
			a3real3Bezier3(demoState->skeletonObject->position.v,
				demoState->curveWaypoint[k[0]].v,
				demoState->curveWaypoint[k[1]].v,
				demoState->curveWaypoint[k[2]].v,
				demoState->curveWaypoint[k[3]].v,
				demoState->segmentParam);
			break;
		case keyframes_interpCatmullRom:
			a3real3CatmullRom(demoState->skeletonObject->position.v,
				demoState->curveWaypoint[k[3]].v,
				demoState->curveWaypoint[k[0]].v,
				demoState->curveWaypoint[k[1]].v,
				demoState->curveWaypoint[k[2]].v,
				demoState->segmentParam);
			break;
		case keyframes_interpCubicHermite:
			a3real3HermiteControl(demoState->skeletonObject->position.v,
				demoState->curveWaypoint[k[0]].v,
				demoState->curveWaypoint[k[1]].v,
				demoState->curveHandle[k[0]].v,
				demoState->curveHandle[k[1]].v,
				demoState->segmentParam);
		//	a3real4Diff(m[0].v, demoState->curveHandle[k[0]].v, demoState->curveWaypoint[k[0]].v);
		//	a3real4Diff(m[1].v, demoState->curveHandle[k[1]].v, demoState->curveWaypoint[k[1]].v);
		//	a3real3HermiteTangent(demoState->skeletonObject->position.v,
		//		demoState->curveWaypoint[k[0]].v,
		//		demoState->curveWaypoint[k[1]].v,
		//		m[0].v,
		//		m[1].v,
		//		demoState->segmentParam);
		}
	}
	else
	{
		// reset position
		demoState->skeletonObject->position = demoState->curveWaypoint[0].xyz;
		demoState->creeperObject->position = demoState->curveWaypoint[0].xyz;
	}


	// update animation: 
	//	-> copy pose from set to state (pro tip: seems pointless but it is not)
	//	-> convert the current pose to transforms
	//	-> forward kinematics
	//	-> skinning matrices
	currentHierarchyState = demoState->hierarchyState_skel + demoMode->editSkeletonIndex;
	currentHierarchyPoseGroup = currentHierarchyState->poseGroup;
	currentHierarchy = currentHierarchyPoseGroup->hierarchy;


	a3hierarchyPoseCopy(currentHierarchyState->localPose,
		currentHierarchyPoseGroup->pose + poseVal, currentHierarchy->numNodes);

	if (demoMode->animating)
	{
		// we lerp things here
		demoState->animPos += (a3real) dt;
		demoState->animPos = mathMod(demoState->animPos, 2.0f);
		a3real pos = (a3real)(1.0 - fabs(1.0 - mathMod((a3real)(2.0 - demoState->animPos), 2.0)));
		lerpAssign(currentHierarchyState->localPose, currentHierarchyPoseGroup->pose + 0, currentHierarchyPoseGroup->pose + 1, pos, currentHierarchy->numNodes);
	}
	else
	{
		poseVal = 0;
		demoState->animPos = 0;	
	}

	//create interpPose, copy that to local space, proceed as normal
	//a3hierarchyPoseCopy(tempPose, pose[0], numNodes) or however we create a pose
	//lerpAssign(tempPose, pose[0], pose[1], count) the first one allocates space.
	//poseCopy with tempPose

	// ALWAYS DO THIS
	a3hierarchyPoseConvert(currentHierarchyState->localSpace,
		currentHierarchyState->localPose, currentHierarchy->numNodes, 0);
	a3kinematicsSolveForward(demoState->hierarchyState_skel);


	currentHierarchyState = demoState->hierarchyState_skel_creeper + demoMode->editSkeletonIndex;
	currentHierarchyPoseGroup = currentHierarchyState->poseGroup;
	currentHierarchy = currentHierarchyPoseGroup->hierarchy;


	a3hierarchyPoseCopy(currentHierarchyState->localPose,
		currentHierarchyPoseGroup->pose + poseVal, currentHierarchy->numNodes);

	if (demoMode->animating)
	{
		// we lerp things here
		demoState->animPos += (a3real)dt;
		demoState->animPos = mathMod(demoState->animPos, 2.0f);
		a3real pos = (a3real)(1.0 - fabs(1.0 - mathMod((a3real)(2.0 - demoState->animPos), 2.0)));
		lerpAssign(currentHierarchyState->localPose, currentHierarchyPoseGroup->pose + 0, currentHierarchyPoseGroup->pose + 1, pos, currentHierarchy->numNodes);
	}
	else
	{
		poseVal = 0;
		demoState->animPos = 0;
	}

	//create interpPose, copy that to local space, proceed as normal
	//a3hierarchyPoseCopy(tempPose, pose[0], numNodes) or however we create a pose
	//lerpAssign(tempPose, pose[0], pose[1], count) the first one allocates space.
	//poseCopy with tempPose

	// ALWAYS DO THIS
	a3hierarchyPoseConvert(currentHierarchyState->localSpace,
		currentHierarchyState->localPose, currentHierarchy->numNodes, 0);
	a3kinematicsSolveForward(demoState->hierarchyState_skel_creeper);


	// update buffers: 
	//	-> calculate and store bone transforms
	//	-> calculate and store joint transforms
	//	-> calculate and store skinning transforms
	{
		a3mat4 modelViewProjectionMat, localModelViewProjectionMat[128] = { 0 };

		// update common MVP
		a3real4x4Product(modelViewProjectionMat.m, activeCamera->viewProjectionMat.m, demoState->skeletonObject->modelMat.m);

		// calculate and send bone transforms
		// need to make the bone point from parent to current
		a3real4x4SetScale(localModelViewProjectionMat[0].m, a3real_zero);
		a3real4x4ConcatR(modelViewProjectionMat.m, localModelViewProjectionMat[0].m);
		for (i = 1; i < currentHierarchy->numNodes; ++i)
		{
			j = currentHierarchy->nodes[i].parentIndex;
			localModelViewProjectionMat[i] = currentHierarchyState->objectSpace->transform[j];
			a3real3Diff(localModelViewProjectionMat[i].v2.v,
				currentHierarchyState->objectSpace->transform[i].v3.v,
				currentHierarchyState->objectSpace->transform[j].v3.v);
			a3real3CrossUnit(localModelViewProjectionMat[i].v0.v,
				(a3isNotNearZero(localModelViewProjectionMat[i].m20) || a3isNotNearZero(localModelViewProjectionMat[i].m21)) ? a3vec3_z.v : a3vec3_y.v,
				localModelViewProjectionMat[i].v2.v);
			a3real3CrossUnit(localModelViewProjectionMat[i].v1.v,
				localModelViewProjectionMat[i].v2.v,
				localModelViewProjectionMat[i].v0.v);
			a3real4x4ConcatR(modelViewProjectionMat.m, localModelViewProjectionMat[i].m);
		}
		a3bufferRefill(demoState->ubo_transformLMVP_bone, 0, currentHierarchy->numNodes * sizeof(a3mat4), localModelViewProjectionMat);

		// calculate and send joint transforms
		for (i = 0; i < currentHierarchy->numNodes; ++i)
		{
			a3real4x4SetScale(localModelViewProjectionMat[i].m, 0.2f);
			a3real4x4ConcatR(currentHierarchyState->objectSpace->transform[i].m, localModelViewProjectionMat[i].m);
			a3real4x4ConcatR(modelViewProjectionMat.m, localModelViewProjectionMat[i].m);
		}
		a3bufferRefill(demoState->ubo_transformLMVP_joint, 0, currentHierarchy->numNodes * sizeof(a3mat4), localModelViewProjectionMat);

		// calculate and send skinning matrices
	//	a3hierarchyStateUpdateObjectBindToCurrent(currentHierarchyState, ???);
	//	for (i = 0; i < currentHierarchy->numNodes; ++i)
	//	{
	//		localModelViewProjectionMat[i] = currentHierarchyState->objectSpaceBindToCurrent->transform[i];
	//		a3real4x4ConcatR(modelViewProjectionMat.m, localModelViewProjectionMat[i].m);
	//	}
	//	a3bufferRefill(demoState->???, 0, currentHierarchy->numNodes * sizeof(a3mat4), localModelViewProjectionMat);

		// send hierarchical info
		a3bufferRefill(demoState->ubo_hierarchy, 0, currentHierarchy->numNodes * sizeof(a3_HierarchyNode), currentHierarchy->nodes);
	}
}


//-----------------------------------------------------------------------------
