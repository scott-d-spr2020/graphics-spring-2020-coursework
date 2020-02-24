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

const vec3 ambientColor = vec3(0.1f);

uniform sampler2D uImage00; // g-buffer depth texture
uniform sampler2D uImage01; // g-buffer position texture
uniform sampler2D uImage02; // g-buffer normal texture

uniform mat4 uPB_inv;

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
	pointLight[MAX_LIGHTS] lights;
};

layout (location = 0) out vec4 rtColor;
layout (location = 1) out vec4 rtViewPosition;
layout (location = 2) out vec4 rtNormal;
layout (location = 6) out vec4 rtDiffuseLight;
layout (location = 7) out vec4 rtSpecularLight;


struct LambertData
{
	vec4 LVec;
	float dotProd_LN;
};


vec4 CalculateDiffuse(vec4 NVec, vec4 position, out LambertData lambert)
{
	vec4 LVec = normalize(lights[vInstanceID].worldPos - position); //w coord is zero, probably
	float dotProd_LN = dot(NVec, LVec);
	lambert = LambertData(LVec, dotProd_LN);
	float dotProd = max(0.0f, dotProd_LN);

	vec4 diffuseResult = lights[vInstanceID].color * dotProd;

	return diffuseResult;
}


//calculates specular highlight.
vec4 CalculateSpecular(vec4 NVec, LambertData lambert, vec3 VVec3d)
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
	return powVal * lights[vInstanceID].color;
}


vec3 CalculatePosition(vec4 sampleCoord)
{
	//TODO: Perspective divide on vbiasedClipCoord
	vec3 sampledPos = texture(uImage01, vBiasedClipCoord.xy).rgb; //gives us position previously saved

	//that data's [0,1], when we need [-x,x]
	vec4 sampledDepth = texture(uImage00, vBiasedClipCoord.xy);

	vec4 recalculatedPos = vec4(sampledPos.x, sampledPos.y, sampledDepth.z, 1.0);

	recalculatedPos = uPB_inv * recalculatedPos;

	return (recalculatedPos / recalculatedPos.w).xyz;
}


void main()
{
	vec4 screenCoord = vBiasedClipCoord / vBiasedClipCoord.w;
	vec4 position_view = uPB_inv * screenCoord;
	position_view = position_view / position_view.w;
//	vec3 position = CalculatePosition(position_view);
	vec4 normal = vec4(texture(uImage02, screenCoord.xy).xyz, 1.0);
	normal = 2.0f * normal - 1.0f;
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
	vec3 VVec3d = normalize(-screenCoord.xyz);

	LambertData lambert;
	vec4 tempDiff = CalculateDiffuse(normal, vec4(position_view.xyz, 1.0), lambert);
	vec4 tempSpec = CalculateSpecular(normal, lambert, VVec3d);
	specular += tempSpec;
	diffuse += tempDiff;

	//rtViewPosition = vec4(position, 1.0f);
	//rtViewPosition = vBiasedClipCoord;
	//rtViewPosition = normalize(vBiasedClipCoord/vBiasedClipCoord.w);

	//our samplecoord IS a position
	rtViewPosition = vec4(position_view.rgb, 1.0f);
	rtDiffuseLight = diffuse;
	rtSpecularLight = specular;
	rtNormal = normal;
}
