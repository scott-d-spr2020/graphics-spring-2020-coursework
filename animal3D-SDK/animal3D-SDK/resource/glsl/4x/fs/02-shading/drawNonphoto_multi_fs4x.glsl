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

in CoordData
{
	vec2 texCoord;
	vec4 mvPosition;
	vec4 mvNormal;
} coordData;

out vec4 rtFragColor;

uniform int uLightCt;
uniform int uLightSz;
uniform int uLightSzInvSq;
uniform vec4 uLightPos[maxLightCount];	//In render.c, lines 456-460, the lighting uniforms are sent. Lines 459 and 460 actually send arrays of uLightPos and uLightCol witht the "->v"
uniform vec4 uLightCol[maxLightCount];

//General uniforms (GLSL forces sampler2D to be outside of blocks).
uniform sampler2D uTex_dm_ramp;	//Ramp texture for cell shading, only sample X coord
uniform sampler2D mainTex;
uniform vec4 uColor;

float CalculateDiffuseCoefficient(vec4 norm, int index)
{
	//Just like with the normal vector, the light vector needs to be normalized for a proper dot product
	vec4 L_vector = normalize(uLightPos[index]- coordData.mvPosition);

	//Like in the book, we need to take the dot product which is the intensity. Color cannot be negative, hence the max.
	float dotProd = max(0.0f, dot(norm, L_vector));

	/*
	For regular diffuse lighting
	//Multiply the light color by the dot product scalar to get the color
	//vec4 diffuseResult = lightData.uLightCol[index] * dotProd;
	//return diffuseResult;
	*/
	return dotProd;
}

void main()
{
	//normalize normal vector to account for scale
	vec4 outNormal_normalized = normalize(coordData.mvNormal);

	float diffuseCoeff = 0.0;
	for(int i = 0; i < uLightCt; i++)
	{
		diffuseCoeff += CalculateDiffuseCoefficient(outNormal_normalized, i);
	}

	rtFragColor = texture(mainTex, coordData.texCoord) * texture(uTex_dm_ramp, vec2(diffuseCoeff, 0.0));
}
