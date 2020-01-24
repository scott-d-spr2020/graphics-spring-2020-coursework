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
	
	drawLambert_multi_fs4x.glsl
	Draw Lambert shading model for multiple lights.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variable for texture; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement Lambert shading model
//	Note: test all data and inbound values before using them!

in vec4 outNormal;
in vec2 outTexCoord;
in vec4 outViewPosition;

out vec4 rtFragColor;

uniform sampler2D mainTex;
uniform int uLightCt;
uniform int uLightSz;
uniform int uLightSzInvSq;
uniform vec4 uLightPos;
uniform vec4 uLightCol;
uniform vec4 uColor;

vec4 CalculateDiffuse(vec4 norm)
{
	vec4 L_vector = normalize(uLightPos- outViewPosition);

	float dotProd = max(0.0f, dot(norm, L_vector));

	vec4 diffuseResult = uLightCol * dotProd;

	return diffuseResult;
}

void main()
{
	//normalize normal vector
	vec4 outNormal_normalized = normalize(outNormal);

	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);

	for(int i = 0; i < uLightCt; i++)
	{
		vec4 tempDiff = CalculateDiffuse(outNormal_normalized);

		diffuse += tempDiff;
	}

	rtFragColor = texture(mainTex, outTexCoord) * diffuse;
}
