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

uniform sampler2D uImage00; // g-buffer depth texture
uniform sampler2D uImage01; // g-buffer position texture
uniform sampler2D uImage02; // g-buffer normal texture
							   
uniform sampler2D uImage03;		// SSAO noise
uniform vec3 uSSAOKernel[64];	// SSAO kernel

uniform mat4 uP;
uniform mat4 uPB_inv;
uniform vec2 uSize;

layout (location = 0) out vec4 rtFragColor;
layout (location = 1) out vec4 rtPosition;
layout (location = 2) out vec4 rtNormal;
layout (location = 3) out vec4 rtRandomVec;
layout (location = 4) out vec4 rtTangent;
layout (location = 5) out vec4 rtBitangent;
layout (location = 6) out vec4 rtOffset;
layout (location = 7) out vec4 rtNewSamp;


vec2 noiseScale = vec2(uSize.x / 4.0, uSize.y / 4.0);	// Used to tile the noise over the whole screen

const float radius = 0.8;	//Used to tweak strength of SSAO calculations
const float bias = 0.025;
const float magnitude = 2.0; // lighten or darken map
const float contrast = 1.0;

vec3 CalculatePosition(vec2 coord)
{
	vec3 sampledPos = texture(uImage01, coord).rgb; // gives us position previously saved
	//that data's [0,1], when we need [-x,x]
	vec4 sampledDepth = texture(uImage00, coord);

	vec4 recalculatedPos = vec4(sampledPos.x, sampledPos.y, sampledDepth.z, 1.0);
	//recalculatedPos.z = 2.0 * recalculatedPos.z - 1.0;	// reset depth value to [-1, 1]
	recalculatedPos = uPB_inv * recalculatedPos;

	return (recalculatedPos / recalculatedPos.w).xyz;
}


void main()
{
	vec3 position = CalculatePosition(vTexcoord.xy);
	vec4 normal = vec4(texture(uImage02, vTexcoord.xy).xyz, 1.0) * 2.0f - vec4(1.0f); //uncompress
	vec3 randomVector = texture(uImage03, vTexcoord.xy * noiseScale).xyz;
	//randomVector = randomVector * 2.0f - 1.0f;

	vec3 tangent = normalize(randomVector - normal.xyz * dot(randomVector, normal.xyz));
	vec3 bitangent = cross(normal.xyz, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal.xyz);	// tangent, bitangent, normal matrix to transform any vector into view space, with a slight random rotation

	float occlusion = 64.0;
	vec4 offset;
	vec4 newSamp;
	for(int i = 0; i < 64; ++i)
	{
		vec3 samp = TBN * uSSAOKernel[i];	// Tangent to view space
		samp = position + samp * radius;

		offset = vec4(samp, 1.0);	// the sample is the offset, just need to put it into NDC
		offset = uP * offset;	// into clip space
		offset.xyz /= offset.w;	// persp divide
		offset.xyz = offset.xyz * 0.5 + 0.5;	// into range  0.0 - 1.0 (compressed)

		newSamp = vec4(CalculatePosition(offset.xy), 1.0f);
		float sampDepth = newSamp.z;
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(position.z - sampDepth));

		float occluded = 0;
		occluded = (sampDepth + bias >= samp.z ? 1.0 : 0.0);	// see if the current sample's depth is larger than the stored value, plus bias

		occluded *= rangeCheck;
		occlusion -= occluded;
	}

	occlusion = 0.5 + (occlusion / (64.0 * magnitude));	// normalize by kernel size, multiply a modifier, and do a half inversion

	//Outputting a color to the screen now works
	rtFragColor = vec4(vec3(occlusion), 1.0) * contrast;
	rtPosition = vec4(position, 1.0f);
	rtNormal = vec4(normal.xyz, 1.0f);
	rtRandomVec = vec4(normalize(randomVector), 1.0f);
	rtTangent = vec4(tangent, 1.0f);
	rtBitangent = vec4(bitangent, 1.0f);
	rtOffset = vec4(offset.xy, 0.0f, 1.0f);
	rtNewSamp = vec4(newSamp.xyz - position.xyz, 1.0f);
	//rtFragColor = vec4(randomVector, 1.0);
	//rtFragColor = texture(uImage03, vTexcoord.xy);
	//rtFragColor = texture(uImage03, vTexcoord.xy * noiseScale);
	//rtFragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
