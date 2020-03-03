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
	
	a3_DemoRenderUtils.h
	Drawing utilities for demo state.
*/

//-----------------------------------------------------------------------------
// animal3D framework includes

#include <stdlib.h>
#include <math.h>
#include "animal3D/animal3D.h"
#include "animal3D-A3DG/animal3D-A3DG.h"
#include "animal3D-A3DM/animal3D-A3DM.h"
#include "animal3D-A3DM/a3math/a3random.h"
#include "animal3D-A3DM/a3math/a3sqrt.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
#endif	// __cplusplus

	
//-----------------------------------------------------------------------------

//SSAO noise generation utilities

void genKernel(a3real3* kern, int kernelSize);

void genNoise(a3real3* arr, int arrSize);

//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus

