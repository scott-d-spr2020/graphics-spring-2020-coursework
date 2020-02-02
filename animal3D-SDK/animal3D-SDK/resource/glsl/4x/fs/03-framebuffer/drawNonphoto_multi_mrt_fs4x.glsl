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
	
	drawNonphoto_multi_fs4x.glsl
	Draw nonphotorealistic shading model for multiple lights.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variables for textures; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement nonphotorealistic shading model
//	Note: test all data and inbound values before using them!

const int maxLightCount = 4;
const int power = 256;
const vec3 ambientColor = vec3(1.0, 0.8, 0.65);

in CoordData
{
	vec2 texCoord;
	vec4 mvPosition;
	vec4 mvNormal;
} coordData;

struct LambertData
{
	vec4 LVec;
	float dotProd_LN;
};

layout (location = 0) out vec4 rtFragColor;
layout (location = 1) out vec4 rtViewPosition;
layout (location = 2) out vec4 rtNormal;
layout (location = 3) out vec4 rtTexCoord;
layout (location = 4) out vec4 rtDiffuseMap;
layout (location = 5) out vec4 rtSpecularMap;
layout (location = 6) out vec4 rtDiffuseTotal;
layout (location = 7) out vec4 rtSpecularTotal;

uniform int uLightCt;
uniform int uLightSz;
uniform int uLightSzInvSq;
uniform vec4 uLightPos[maxLightCount];	//In render.c, lines 456-460, the lighting uniforms are sent. Lines 459 and 460 actually send arrays of uLightPos and uLightCol witht the "->v"
uniform vec4 uLightCol[maxLightCount];

//General uniforms (GLSL forces sampler2D to be outside of blocks).
uniform sampler2D uTex_dm_ramp;	//Ramp texture for cell shading, only sample X coord
uniform sampler2D uTex_sm;
uniform sampler2D mainTex;
uniform vec4 uColor;

vec4 CalculateDiffuse(vec4 NVec, int index, out LambertData lambert)
{
	vec4 LVec = normalize(uLightPos[index]- coordData.mvPosition); //w coord is zero, probably
	float dotProd_LN = dot(NVec, LVec);
	lambert = LambertData(LVec, dotProd_LN);
	float dotProd = max(0.0f, dotProd_LN);

	vec4 diffuseResult = uLightCol[index] * dotProd;

	return diffuseResult;
}

vec4 CalculateSpecular(vec4 NVec, int index, LambertData lambert, vec3 VVec3d, out float specValue)
{
	vec3 NVec3d = NVec.xyz;
	vec3 LVec3d = lambert.LVec.xyz; //unsure if this is actually necessary
	vec3 RVec3d = (2.0f * lambert.dotProd_LN * NVec3d) - LVec3d;
	specValue = pow(max(0.0f, dot(VVec3d, RVec3d)), power);
	return specValue * uLightCol[index];
}

void main()
{
	//normalize normal vector to account for scale
	vec4 outNormal_normalized = normalize(coordData.mvNormal);

	vec4 diffuseLighting = vec4(0.0, 0.0, 0.0, 1.0);
	float diffuseCoeff = 0.0;

	vec4 specularLighting = vec4(0.0, 0.0, 0.0, 1.0);
	float specularCoeff = 0.0f;
	vec3 VVec3d = normalize(-coordData.mvPosition.xyz);

	for(int i = 0; i < uLightCt; i++)
	{
		LambertData lambert;
		float tempDiffuseCoeff, tempSpecCoeff;

		diffuseLighting += CalculateDiffuse(outNormal_normalized, i, lambert);
		specularLighting += CalculateSpecular(outNormal_normalized, i, lambert, VVec3d, tempSpecCoeff);

		diffuseCoeff += lambert.dotProd_LN;
		specularCoeff += tempSpecCoeff;
	}

	vec4 mainSample = texture(mainTex, coordData.texCoord);
	vec4 rampSample = texture(uTex_dm_ramp, vec2(diffuseCoeff, 0.0));
	vec4 specularSample = texture(uTex_sm, coordData.texCoord);

	vec4 diffColor = mainSample * diffuseLighting;
	vec4 specularColor = specularSample * specularLighting;

	rtFragColor = vec4((rampSample.rgb * diffColor.rgb) + specularColor.rgb + (0.05f * ambientColor), 1.0);
	rtViewPosition = coordData.mvPosition;
	rtNormal = vec4(outNormal_normalized.xyz, 1.0);
	rtTexCoord = vec4(coordData.texCoord, 0.0, 1.0);
	rtDiffuseMap = mainSample;
	rtSpecularMap = specularSample;
	rtDiffuseTotal = rampSample * diffuseLighting;
	rtSpecularTotal = specularColor;
}
