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
	
	a3_DemoSSAOUtils.c
	Generate randomosity for SSAO
*/

#include <stdlib.h>
#include <math.h>
#include "animal3D/animal3D.h"

//-----------------------------------------------------------------------------

// OpenGL
#ifdef _WIN32
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32


//-----------------------------------------------------------------------------
// SSAO Generation utilities

float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

/*
	A kernel at a length of 64 is best, must be power of 2
*/
void genKernel(a3real3 *kern, int kernelSize)
{
	for (int i = 0; i < kernelSize; i++)
	{
		a3real3 temp;
		temp[0] = (float)rand() / (float)(RAND_MAX / 1.0) * 2.0 - 1.0;
		temp[1] = (float)rand() / (float)(RAND_MAX / 1.0) * 2.0 - 1.0;
		temp[2] = (float)rand() / (float)(RAND_MAX / 1.0);

		//Normalize sample
		double mag = sqrt(((double)((double)temp[0] * temp[0])) + (double)((double)temp[1] * temp[1]) + (double)((double)temp[2] * temp[2]));
		temp[0] /= mag;
		temp[1] /= mag;
		temp[2] /= mag;

		//multiply by a new random float between 0.0, 1.0
		temp[0] = (float)rand() / (float)(RAND_MAX / 1.0);
		temp[1] = (float)rand() / (float)(RAND_MAX / 1.0);
		temp[2] = (float)rand() / (float)(RAND_MAX / 1.0);
		
		float scale = (float)i / (float)kernelSize;

		scale = lerp(0.1f, 1.0f, scale * scale);
		temp[0] *= scale;
		temp[1] *= scale;
		temp[2] *= scale;

		kern[i][0] = temp[0];
		kern[i][1] = temp[1];
		kern[i][2] = temp[2];
	}
}

void genNoise()
{

}


//-----------------------------------------------------------------------------
