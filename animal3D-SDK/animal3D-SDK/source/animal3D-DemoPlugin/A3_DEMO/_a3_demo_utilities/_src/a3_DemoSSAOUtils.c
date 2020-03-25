/*
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
	animal3D SDK: Minimal 3D Animation Framework, SSAO Plugin
	By Cameron Schneider
	
	a3_DemoSSAOUtils.c
	Generate randomosity for SSAO
*/

#include "..//a3_DemoSSAOUtils.h"

//-----------------------------------------------------------------------------

// OpenGL
#ifdef _WIN32
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32

#include <stdio.h>


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
	for (int i = 0; i < kernelSize; ++i)
	{
		a3real3 temp;
		temp[0] = a3randomSymmetric();
		temp[1] = a3randomSymmetric();
		temp[2] = a3randomNormalized();

		a3real3Normalize(temp);

		a3real randomScalar = a3randomNormalized();
		//multiply by a new random float between 0.0, 1.0
		temp[0] *= randomScalar;
		temp[1] *= randomScalar;
		temp[2] *= randomScalar;
		
		float scale = (float)i / (float)kernelSize;

		scale = lerp(0.1f, 1.0f, scale * scale);
		temp[0] *= scale;
		temp[1] *= scale;
		temp[2] *= scale;

		//printf("Kernel: %f, %f, %f\n", temp[0], temp[1], temp[2]);

		kern[i][0] = temp[0];
		kern[i][1] = temp[1];
		kern[i][2] = temp[2];
	}
}


/*
	A length 16 is best, must be power of 2
*/
void genNoise(a3real3 *arr, int arrSize)
{
	for (int i = 0; i < arrSize; i++)
	{
		a3real3 temp;
		temp[0] = a3randomNormalized(); //[-0,1] so we can expand it in the FS
		temp[1] = a3randomNormalized();
		temp[2] = 0.0f;

		arr[i][0] = temp[0];
		arr[i][1] = temp[1];
		arr[i][2] = temp[2];

		//printf("Noise: %f, %f, %f\n", temp[0], temp[1], temp[2]);
	}
}


//-----------------------------------------------------------------------------
