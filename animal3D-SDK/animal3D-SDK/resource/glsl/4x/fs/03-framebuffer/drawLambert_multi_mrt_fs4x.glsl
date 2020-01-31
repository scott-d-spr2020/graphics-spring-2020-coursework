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
	
	drawLambert_multi_mrt_fs4x.glsl
	Draw Lambert shading model for multiple lights with MRT output.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variable for texture; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement Lambert shading model
//	Note: test all data and inbound values before using them!
//	5) set location of final color render target (location 0)
//	6) declare render targets for each attribute and shading component

const int maxLightCount = 4;

in CoordData
{
	vec2 texCoord;
	vec4 mvPosition;
	vec4 mvNormal;
} coordData;

layout (location = 0) out vec4 rtFragColor;
layout (location = 3) out vec4 rtTexCoord;


uniform sampler2D mainTex;
uniform int uLightCt;
uniform int uLightSz;
uniform int uLightSzInvSq;
uniform vec4 uLightPos[maxLightCount];
uniform vec4 uLightCol[maxLightCount];
uniform vec4 uColor;

vec4 CalculateDiffuse(vec4 norm, int index)
{
	vec4 L_vector = normalize(uLightPos[index]- coordData.mvPosition);

	float dotProd = max(0.0f, dot(norm, L_vector));

	vec4 diffuseResult = uLightCol[index] * dotProd;

	return diffuseResult;
}

void main()
{
	//normalize normal vector
	vec4 mvNormal_normalized = normalize(coordData.mvNormal);

	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);

	for(int i = 0; i < uLightCt; i++)
	{
		vec4 tempDiff = CalculateDiffuse(mvNormal_normalized, i);

		diffuse += tempDiff;
	}

	rtFragColor = texture(mainTex, coordData.texCoord) * diffuse;
	rtTexCoord = vec4(coordData.texCoord, 0.0, 1.0);
}
