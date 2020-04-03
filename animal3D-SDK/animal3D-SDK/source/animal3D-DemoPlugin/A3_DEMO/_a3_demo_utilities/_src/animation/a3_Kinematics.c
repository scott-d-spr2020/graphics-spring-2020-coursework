/*
	Copyright 2011-2019 Daniel S. Buckstein

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
	
	a3_Kinematics.c
	Implementation of kinematics solvers.
*/

#include "../../animation/a3_Kinematics.h"


//-----------------------------------------------------------------------------

// FK solver
extern inline a3i32 a3kinematicsSolveForward(const a3_HierarchyState *hierarchyState)
{
	return a3kinematicsSolveForwardPartial(hierarchyState, 0, hierarchyState->poseGroup->hierarchy->numNodes);
}

// partial FK solver
extern inline a3i32 a3kinematicsSolveForwardPartial(const a3_HierarchyState *hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->poseGroup && 
		firstIndex < hierarchyState->poseGroup->hierarchy->numNodes && nodeCount)
	{
		a3i32 parentIndex;
		a3ui32 i, end = firstIndex + nodeCount;
		end = a3minimum(end, hierarchyState->poseGroup->hierarchy->numNodes);

		for (i = firstIndex; i < end; ++i)
		{
			parentIndex = hierarchyState->poseGroup->hierarchy->nodes[i].parentIndex;	// sets parent index to current node's parent

			if (parentIndex >= 0)
			{
				// multiplies parent node object space transform and current node local space transform, setting the current node object space transform to the result

				// altered subscripts. They seemingly work now.
				a3real4x4Product(hierarchyState->objectSpace[0].transform[i].m, hierarchyState->objectSpace[0].transform[parentIndex].m, hierarchyState->localSpace[0].transform[i].m);
			}
			else
			{
				// if this is the root, set the object space to the local space
				a3real4x4SetReal4x4(hierarchyState->objectSpace[0].transform[0].m, hierarchyState->localSpace[0].transform[0].m);
			}

			/*
				Need to figure out how to identify root node, dan said root node index is negative:
					hierarchyState->poseGroup->hierarchy->nodes[i].parentIndex >= 0
				This says if the current node's parent index is 0 or greater, then we are not the root node, since the root node's parent is negative

				if(current node != root node)
				{
					current node object space = parent node object space * current node local space
				}
				else
				{
					current node object space = current node local space
				}
			*/
		}

		// done, return number of nodes updated
		return (end - firstIndex);
	}
	return -1;
}


//-----------------------------------------------------------------------------

// IK solver
extern inline a3i32 a3kinematicsSolveInverse(const a3_HierarchyState *hierarchyState)
{
	return a3kinematicsSolveInversePartial(hierarchyState, 0, hierarchyState->poseGroup->hierarchy->numNodes);
}

// partial IK solver
extern inline a3i32 a3kinematicsSolveInversePartial(const a3_HierarchyState *hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->poseGroup &&
		firstIndex < hierarchyState->poseGroup->hierarchy->numNodes && nodeCount)
	{
	//	a3i32 parentIndex;
		a3ui32 i, end = firstIndex + nodeCount;
		end = a3minimum(end, hierarchyState->poseGroup->hierarchy->numNodes);

		for (i = firstIndex; i < end; ++i)
		{
			// ****TO-DO: implement inverse kinematics algorithm

		}

		// done, return number of nodes updated
		return (end - firstIndex);
	}
	return -1;
}


//-----------------------------------------------------------------------------
