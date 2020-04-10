#include "../a3_DemoPoseUtils.h"
#include "../a3_DemoSSAOUtils.h"

void lerpAssign(a3_HierarchyPose* outPose, a3_HierarchyPose* pose0, a3_HierarchyPose* pose1, a3real t, a3ui32 numNodes)
{
	for (a3ui32 i = 0; i < numNodes; ++i)
	{
		a3_HierarchyNodePose* outNode = outPose->nodePose + i;
		a3_HierarchyNodePose* node0 = pose0->nodePose + i;
		a3_HierarchyNodePose* node1 = pose1->nodePose + i;
		outNode->orientation = vec4Lerp(node0->orientation, node1->orientation, t);
		outNode->scale = vec4Lerp(node0->scale, node1->scale, t);
		outNode->translation = vec4Lerp(node0->translation, node1->translation, t);
	}
}
