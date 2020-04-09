#ifndef A3_DEMOPOSEUTILS_H
#define A3_DEMOPOSEUTILS_H

#include "animation/a3_HierarchyState.h"

void lerpAssign(a3_HierarchyPose* outPose, a3_HierarchyPose* pose0, a3_HierarchyPose* pose1, a3real t, a3ui32 numNodes);
#endif