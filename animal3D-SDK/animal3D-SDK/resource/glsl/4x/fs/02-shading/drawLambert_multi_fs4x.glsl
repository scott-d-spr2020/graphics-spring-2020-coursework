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

const int maxLightCount = 4;

in PassData
{
	vec4 outNormal;
	vec2 outTexCoord;
	vec4 outViewPosition;
} inData;


out vec4 rtFragColor;


uniform LightingUniforms
{
	int uLightCt;
	int uLightSz;
	int uLightSzInvSq;
	vec4 uLightPos[maxLightCount];	//In render.c, lines 456-460, the lighting uniforms are sent. Lines 459 and 460 actually send arrays of uLightPos and uLightCol witht the "->v"
	vec4 uLightCol[maxLightCount];
} lightData;

//General uniforms (GLSL forces sampler2D to be outside of blocks).
uniform sampler2D mainTex;
uniform vec4 uColor;

vec4 CalculateDiffuse(vec4 norm, int index)
{
	//Just like with the normal vector, the light vector needs to be normalized for a proper dot product
	vec4 L_vector = normalize(lightData.uLightPos[index]- inData.outViewPosition);

	//Like in the book, we need to take the dot product which is the direction between the normal and light vectors. Color cannot be negative, hence the max.
	float dotProd = max(0.0f, dot(norm, L_vector));

	//Multiply the light color by the dot product scalar to get the color
	vec4 diffuseResult = lightData.uLightCol[index] * dotProd;

	return diffuseResult;
}

void main()
{
	//normalize normal vector to account for scale
	vec4 outNormal_normalized = normalize(inData.outNormal);

	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);	//Basic vector that will be the end result of all light diffuse coloring

	//Loop through all lights in the scene and calculate the diffuse color for each light, then add it to the result vector
	for(int i = 0; i < lightData.uLightCt; i++)
	{
		vec4 tempDiff = CalculateDiffuse(outNormal_normalized, i);

		diffuse += tempDiff;
	}

	//Multiply the texture color by the diffuse color to mix them
	rtFragColor = texture(mainTex, inData.outTexCoord) * diffuse;
}
