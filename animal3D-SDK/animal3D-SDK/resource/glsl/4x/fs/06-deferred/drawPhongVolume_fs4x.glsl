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
	
	drawPhongVolume_fs4x.glsl
	Draw Phong lighting components to render targets (diffuse & specular).
*/

#version 410

#define MAX_LIGHTS 1024

// ****TO-DO: 
//	0) copy deferred Phong shader
//	1) declare g-buffer textures as uniform samplers
//	2) declare lighting data as uniform block
//	3) calculate lighting components (diffuse and specular) for the current 
//		light only, output results (they will be blended with previous lights)
//			-> use reverse perspective divide for position using scene depth
//			-> use expanded normal once sampled from normal g-buffer
//			-> do not use texture coordinate g-buffer

in vec4 vBiasedClipCoord;
flat in int vInstanceID;

layout (location = 0) in vec4 depthBuffer;
layout (location = 1) in vec4 position;
layout (location = 2) in vec4 normal;

struct pointLight
{
	vec4 worldPos;		// position in world space
	vec4 viewPos;		// position in viewer space
	vec4 color;			// RGB color with padding
	float radius;		// radius (distance of effect from center)
	float radiusInvSq;	// radius inverse squared (attenuation factor)
	float pad[2];		// padding
};

uniform ubPointLight
{
	pointLight[] lights;
};

layout (location = 6) out vec4 rtDiffuseLight;
layout (location = 7) out vec4 rtSpecularLight;


struct LambertData
{
	vec4 LVec;
	float dotProd_LN;
};


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




void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE MAGENTA
	rtDiffuseLight = vec4(1.0, 0.0, 1.0, 1.0);
	rtSpecularLight = vec4(1.0, 0.0, 1.0, 1.0);
}
