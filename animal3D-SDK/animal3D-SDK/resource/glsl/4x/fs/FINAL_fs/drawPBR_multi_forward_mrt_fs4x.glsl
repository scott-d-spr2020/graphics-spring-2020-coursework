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
	
	drawPhong_multi_forward_mrt_fs4x.glsl
	Draw Phong shading model using forward light set.
*/

#version 430

#define MAX_LIGHTS 4

// ****TO-DO: 
//	0) nothing
//TALK 8
const int maxLightCount = 4;

in CoordData
{
	vec2 texCoord;
	vec4 mvPosition;
	mat4 TBN;
};

uniform sampler2D uImage00;	// color
uniform sampler2D uImage01;	// normal
uniform sampler2D uImage02;	// metallic
uniform sampler2D uImage03;	// roughness

uniform int uLightCt;
uniform int uLightSz;
uniform int uLightSzInvSq;
uniform vec4 uLightPos[maxLightCount];
uniform vec4 uLightCol[maxLightCount];
uniform vec4 uColor;

uniform mat4 uMV;

layout (location = 0) out vec4 rtFragColor;
layout (location = 1) out vec4 rtViewPosition;
layout (location = 2) out vec4 rtNormal;
layout (location = 3) out vec4 rtTexCoord;
layout (location = 4) out vec4 rtDiffuseMap;
layout (location = 5) out vec4 rtSpecularMap;
layout (location = 6) out vec4 rtDiffuseTotal;
layout (location = 7) out vec4 rtSpecularTotal;

const int power = 16;
const vec3 ambientColor = vec3(0.1f);

struct LambertData
{
	vec4 LVec;
	float dotProd_LN;
};

vec4 CalculateDiffuse(vec4 NVec, int index, out LambertData lambert)
{
	vec4 LVec = normalize(uLightPos[index]- mvPosition); //w coord is zero, probably
	float dotProd_LN = dot(NVec, LVec);
	lambert = LambertData(LVec, dotProd_LN);
	float dotProd = max(0.0f, dotProd_LN);

	vec4 diffuseResult = uLightCol[index] * dotProd;

	return diffuseResult;
}

//calculates specular highlight.
vec4 CalculateSpecular(vec4 NVec, int index, LambertData lambert, vec3 VVec3d, out float specValue)
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


void main()
{
	mat4 tangentBasis_view = mat4(
		normalize(TBN[0]),
		normalize(TBN[1]),
		normalize(TBN[2]),
		TBN[3]
	);
	
	vec4 T = tangentBasis_view[0];
	vec4 B = tangentBasis_view[1];
	vec4 N = tangentBasis_view[2];
	vec4 P = tangentBasis_view[3];

	mat3 TBN_norm = mat3(T, B, N);

	// Use an expanded normal map texture and the TBN matrix multiplication to use per-frag normal maps
	vec3 mapped_normal = texture(uImage02, texCoord).xyz;
	mapped_normal = mapped_normal * 2.0 - 1.0;
	mapped_normal = normalize(TBN_norm * mapped_normal);

	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
	float specVal = 0.0f;
	vec3 VVec3d = normalize(-mvPosition.xyz);
	for(int i = 0; i < uLightCt; i++)
	{
		LambertData lambert;
		vec4 tempDiff = CalculateDiffuse(vec4(mapped_normal, 1.0), i, lambert);
		vec4 tempSpec = CalculateSpecular(vec4(mapped_normal, 1.0), i, lambert, VVec3d, specVal);
		specular += tempSpec;
		diffuse += tempDiff;
	}
	vec4 diffColor = texture(uImage00, texCoord) * diffuse;
	vec4 specularColor = texture(uImage01, texCoord) * specular;

	rtFragColor = vec4(diffColor.rgb + specularColor.rgb + (0.3f * ambientColor), 1.0);
	//rtFragColor = vec4(vec3(uLightCt), 1.0);
	//rtFragColor = vec4(mapped_normal.xyz, 1.0);

	rtViewPosition = mvPosition;
	rtNormal =  vec4(mapped_normal, 1.0);
	rtTexCoord = vec4(texCoord, 0.0, 1.0);
	rtDiffuseMap = texture(uImage00, texCoord);
	rtSpecularMap = texture(uImage01, texCoord);
	rtDiffuseTotal = diffuse;
	rtSpecularTotal = specular;
}
