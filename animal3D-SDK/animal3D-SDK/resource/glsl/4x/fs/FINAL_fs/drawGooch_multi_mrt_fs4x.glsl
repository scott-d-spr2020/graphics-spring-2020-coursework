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
	
	drawPhong_multi_fs4x.glsl
	Draw Gooch shading model for multiple lights. Imported from lab 3 because we never merged lab 3 -> 4
*/

#version 410

const int maxLightCount = 4;

in CoordData
{
	vec2 texCoord;
	vec4 mvPosition;
	vec4 mvNormal;
} coordData;

uniform sampler2D mainTex;
uniform int uLightCt;
uniform int uLightSz;
uniform int uLightSzInvSq;
uniform vec4 uLightPos[maxLightCount];
uniform vec4 uLightCol[maxLightCount];
uniform vec4 uColor;

uniform mat4 uMV;

const vec4 cHighlight = vec4(1.0f, 1.0f, 1.0f, 1.0f);
const vec4 cCool = vec4(0.0f, 0.0f, 0.55f, 1.0f);
const vec4 cWarm = vec4(0.3f, 0.3f, 0.0f, 1.0f);

// ****TO-DO: 
//	1) declare uniform variables for textures; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement Phong shading model
//	Note: test all data and inbound values before using them!

layout (location = 0) out vec4 rtFragColor;
layout (location = 1) out vec4 rtViewPosition;
layout (location = 2) out vec4 rtNormal;
layout (location = 3) out vec4 rtTexCoord;
layout (location = 4) out vec4 rtDiffuseMap;
layout (location = 5) out vec4 rtSpecularMap;
layout (location = 6) out vec4 rtDiffuseTotal;
layout (location = 7) out vec4 rtSpecularTotal;


vec4 CalculateGooch(vec4 n_vector, int index)
{
	vec3 NVec3d = n_vector.xyz;
	vec3 LVec3d = normalize(uLightPos[index].xyz - coordData.mvPosition.xyz);
	vec3 VVec3d = normalize(-coordData.mvPosition.xyz);
	vec3 RVec3d = (2.0f * max(0.0f,dot(NVec3d, LVec3d)) * NVec3d) - LVec3d;

	float t = (dot(NVec3d, LVec3d) + 1.0f)/2.0f;
	float s = clamp(100.0f * dot(RVec3d, VVec3d) - 97.0f, 0.0f, 1.0f);

	vec4 surfaceColor = 0.25f * texture(mainTex, coordData.texCoord);
	vec4 cShaded = s * cHighlight + (1.0f-s)*(t*(cWarm+surfaceColor) + (1.0f-t)*(cCool+surfaceColor));
	return cShaded;
}


void main()
{
	//this part's the same as Lambert
	vec4 mvNormal_normalized = normalize(coordData.mvNormal);

	vec4 gooch = vec4(0.0, 0.0, 0.0, 1.0);

	for(int i = 0; i < min(uLightCt, 2); i++)	//it looks terrible with more than two lights. We're willing to compromise photorealism.
	{
		vec4 tempGooch = CalculateGooch(mvNormal_normalized, i);
		gooch += (tempGooch);
	}

	rtFragColor = vec4(gooch.rgb, 1.0f);
	rtTexCoord = vec4(coordData.texCoord, 0.0, 1.0);
	rtNormal = vec4(mvNormal_normalized.xyz, 1.0);
	rtViewPosition = coordData.mvPosition;
	rtDiffuseMap = texture(mainTex, coordData.texCoord);
	rtSpecularMap = vec4(0,0,0,1);
	rtDiffuseTotal = vec4(1);
	rtSpecularTotal = vec4(vec3(uLightCt/float(maxLightCount)),1);
}
