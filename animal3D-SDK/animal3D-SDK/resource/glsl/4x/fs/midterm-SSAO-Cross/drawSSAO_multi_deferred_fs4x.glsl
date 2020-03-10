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
	
	drawPhong_multi_deferred_fs4x.glsl
	Draw Phong shading model by sampling from input textures instead of 
		data received from vertex shader.
*/

#version 410

#define MAX_LIGHTS 4

// ****TO-DO: 
//	0) copy original forward Phong shader
//	1) declare g-buffer textures as uniform samplers
//	2) declare light data as uniform block
//	3) replace geometric information normally received from fragment shader 
//		with samples from respective g-buffer textures; use to compute lighting
//			-> position calculated using reverse perspective divide; requires 
//				inverse projection-bias matrix and the depth map
//			-> normal calculated by expanding range of normal sample
//			-> surface texture coordinate is used as-is once sampled

in vec4 vTexcoord;

const vec3 ambientColor = vec3(0.1f);

uniform sampler2D uImage00; // g-buffer depth texture
uniform sampler2D uImage01; // g-buffer position texture
uniform sampler2D uImage02; // g-buffer normal texture
							   
uniform sampler2D uImage03; // SSAO noise

uniform mat4 uP_inv;
uniform mat4 uPB_inv;


layout (location = 0) out vec4 rtFragColor;
layout (location = 1) out vec4 rtPosition;
layout (location = 2) out vec4 rtNormal;


vec3 CalculatePosition()
{
	vec3 sampledPos = texture(uImage01, vTexcoord.xy).rgb; //gives us position previously saved
	//that data's [0,1], when we need [-x,x]
	vec4 sampledDepth = texture(uImage00, vTexcoord.xy);

	vec4 recalculatedPos = vec4(sampledPos.x, sampledPos.y, sampledDepth.z, 1.0);
	//recalculatedPos.z = 2.0 * recalculatedPos.z - 1.0;	//reset depth value to [-1, 1]
	recalculatedPos = uPB_inv * recalculatedPos;

	return (recalculatedPos / recalculatedPos.w).xyz;
}


void main()
{
	vec3 position = CalculatePosition();
	vec4 normal = vec4(texture(uImage02, vTexcoord.xy).xyz, 1.0) * 2.0f - vec4(1.0f); //uncompress

	//Outputting a color to the screen now works
	rtFragColor = vec4(position, 1.0);
	//rtFragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
