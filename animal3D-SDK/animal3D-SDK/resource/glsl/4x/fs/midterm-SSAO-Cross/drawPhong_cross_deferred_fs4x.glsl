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
uniform sampler2D uImage03; // g-buffer texcoord texture
							   
uniform sampler2D uImage04; // ambient modifier
uniform sampler2D uImage05; // crosslower
uniform sampler2D uImage06; // crossupper

uniform int uLightCt;
uniform int uLightSz;
uniform int uLightSzInvSq;
uniform vec4 uLightPos[MAX_LIGHTS];
uniform vec4 uLightCol[MAX_LIGHTS];


uniform mat4 uPB_inv;


struct LambertData
{
	vec4 LVec;
	float dotProd_LN;
};


layout (location = 0) out vec4 rtFragColor;
layout (location = 1) out vec4 rtPosition;
layout (location = 2) out vec4 rtNormal;
layout (location = 3) out vec4 rtTexcoord;
layout (location = 4) out vec4 rtDiffuseMapSample;
layout (location = 5) out vec4 rtSpecularMapSample;
layout (location = 6) out vec4 rtDiffuseLightTotal;
layout (location = 7) out vec4 rtSpecularLightTotal;


vec4 CalculateDiffuse(vec4 NVec, int index, vec4 position, out LambertData lambert)
{
	vec4 LVec = normalize(uLightPos[index] - position); //w coord is zero, probably
	float dotProd_LN = dot(NVec, LVec);
	lambert = LambertData(LVec, dotProd_LN);
	float dotProd = max(0.0f, dotProd_LN);

	vec4 diffuseResult = uLightCol[index] * dotProd;

	return diffuseResult;
}


//calculates specular highlight.
vec4 CalculateSpecular(vec4 NVec, int index, LambertData lambert, vec3 VVec3d)
{
	vec3 NVec3d = NVec.xyz;
	vec3 LVec3d = lambert.LVec.xyz; //unsure if this is actually necessary
	vec3 RVec3d = (2.0f * lambert.dotProd_LN * NVec3d) - LVec3d;

	//pow is 16
	float tempSpecVal = max(0.0f, dot(VVec3d, RVec3d));
	float powVal = tempSpecVal * tempSpecVal; //^2
	powVal = powVal * powVal; //^4
	powVal = powVal * powVal; //^8
	powVal = powVal * powVal; //^16
	return powVal * uLightCol[index];
}


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

//borrowed from bloom, used for brightness
float relativeLuminance(vec3 color)
{
	return (0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b);
}


float hatch(vec2 coord, float lumin)
{
	vec4 color00 = texture(uImage05, coord.xy);
	vec4 color01 = texture(uImage06, coord.xy);

	float mergeColor;
	if (lumin <= 0.0f)
	{
		mergeColor = color00.r;
	}
	if (lumin > 0.0f && lumin < (1.0f/6.0f))
	{
		mergeColor = mix(color00.r, color00.g, mod(lumin, (1.0f/6.0f)) * 6f);
	}
	else if (lumin > (1.0f/6.0f) && lumin < (2.0f/6.0f))
	{
		mergeColor = mix(color00.g, color00.b, mod(lumin, (1.0f/6.0f)) * 6f);
	}
	else if (lumin >= (2.0f/6.0f) && lumin < 0.5)
	{
		mergeColor = mix(color00.b, color01.r, mod(lumin, (1.0f/6.0f)) * 6f);
	}
	else if (lumin >= 0.5 && lumin < (4.0f/6.0f))
	{
		mergeColor = mix(color01.r, color01.g, mod(lumin, (1.0f/6.0f)) * 6f);
	}
	else if (lumin >= (4.0f/6.0f) && lumin < (5.0f/6.0f))
	{
		mergeColor = mix(color01.g, color01.b, mod(lumin, (1.0f/6.0f)) * 6f);
	}
	else if (lumin >= (5.0f/6.0f) && lumin < 1.0)
	{
		mergeColor = mix(color01.b, 1.0f, mod(lumin, (1.0f/6.0f)) * 6f);
	}
	else if (lumin >= 1.0)
	{
		mergeColor = 1.0f;
	}
	return mergeColor;
}

void main()
{
	vec2 texCoord = texture(uImage03, vTexcoord.xy).rg; // Indidivual texture coords are stored in this texture's rg channels
	vec3 position = CalculatePosition();
	vec4 normal = vec4(texture(uImage02, vTexcoord.xy).xyz, 1.0) * 2.0f - vec4(1.0f); //uncompress

	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);

	vec4 ambient = texture(uImage04, vTexcoord.xy);
	vec3 VVec3d = normalize(-position.xyz);

	for(int i = 0; i < uLightCt; i++)
	{
		LambertData lambert;
		vec4 tempDiff = CalculateDiffuse(normal, i, vec4(position, 1.0), lambert);
		vec4 tempSpec = CalculateSpecular(normal, i, lambert, VVec3d);
		specular += tempSpec;
		diffuse += tempDiff;
	}

	rtTexcoord = vec4(texCoord, 0.0, 1.0);
	rtNormal = normal;
	rtPosition = vec4(position, 1.0);

	rtDiffuseMapSample = vec4(0.4 * (diffuse.rgb + specular.rgb) + (0.5f * ambient.rgb), 1.0);
	//rtDiffuseMapSample = vec4(ambient.rgb, 1.0);

	float mergeColor = hatch(16 * texCoord.xy, relativeLuminance(rtDiffuseMapSample.rgb));
	rtFragColor = vec4(vec3(mergeColor), 1.0);
	float merge2 = hatch(16 * texCoord.xy, relativeLuminance(ambient.rgb));
	rtSpecularMapSample = vec4(vec3(merge2), 1.0f);
	rtDiffuseLightTotal = diffuse;
	rtSpecularLightTotal = specular;
}
