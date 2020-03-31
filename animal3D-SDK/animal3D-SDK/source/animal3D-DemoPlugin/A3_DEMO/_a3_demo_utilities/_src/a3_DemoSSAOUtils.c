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
		// Create random kernel vector in a hemispheric shape
		a3real3 temp;
		temp[0] = a3randomSymmetric();
		temp[1] = a3randomSymmetric();
		temp[2] = a3randomNormalized();

		a3real3Normalize(temp);

		// multiply by a new random float between 0.0, 1.0
		a3real randomScalar = a3randomNormalized();
		temp[0] *= randomScalar;
		temp[1] *= randomScalar;
		temp[2] *= randomScalar;
		
		// scale by the kernel size to place more weight on samples closer to center
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


/*
	A length 16 is best, must be power of 2
*/
void genNoise(a3real3 *arr, int arrSize)
{
	for (int i = 0; i < arrSize; i++)
	{
		// Create random rotation noise vectors around z-axis
		a3real3 temp;
		temp[0] = a3randomNormalized();
		temp[1] = a3randomNormalized();
		temp[2] = 0.0f;

		arr[i][0] = temp[0];
		arr[i][1] = temp[1];
		arr[i][2] = temp[2];
	}
}

//lerp between vecs a and b by a value t
a3vec3 vecLerp(a3vec3 a, a3vec3 b, float t)
{
	a3vec3 ret;
	ret.x = a.x + t * (b.x - a.x);
	ret.y = a.y + t * (b.y - a.y);
	ret.z = a.z + t * (b.z - a.z);
	return ret;
}

//recursive bezier function
a3vec3 bezierN(a3vec3* arr, int length, float t)
{
	a3vec3 ret;
	if (length == 2)
	{
		ret = vecLerp(arr[0], arr[1], t);
	}
	else
	{
		ret = vecLerp(bezierN(arr, length - 1, t), bezierN(arr + 1, length - 1, t), t);
	}
	return ret;
}

//proper modulus. Source: https://stackoverflow.com/questions/11720656/modulo-operation-with-negative-numbers/19288271#19288271
a3real mathMod(a3real num, a3real mod)
{
	a3real r = (a3real)fmod(num, mod);
	return r < 0 ? r + mod : r;
}


//-----------------------------------------------------------------------------
