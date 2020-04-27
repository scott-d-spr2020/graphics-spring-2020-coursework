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
	
	passTangentBasis_transform_instanced_vs4x.glsl
	Transforms all attributes by their respective matrix, sends full 
		tangent basis and other lighting data.
*/

#version 430

#define MAX_INSTANCES	16
#define MAX_MODELS		 8

// ****TO-DO: 
//	0) nothing
//TALK 7
layout (location = 0) in vec4 aPosition;
layout (location = 2) in vec4 aNormal;
layout (location = 8) in vec4 inTexCoord0;
layout (location = 10) in vec4 aTangent;
layout (location = 11) in vec4 aBitangent;


out CoordData
{
	vec2 texCoord;
	vec4 mvPosition;
	mat4 TBN;
} coordData;

uniform mat4 uMV;
uniform mat4 uMV_nrm;
uniform mat4 uP;
uniform mat4 uAtlas;

void main()
{
	coordData.mvPosition = uMV * aPosition;
	coordData.texCoord = (uAtlas * inTexCoord0).xy;

	//coordData.TBN = mat3(normalize(uMV_nrm * aTangent), normalize(uMV_nrm * aBitangent), normalize(uMV_nrm * aNormal));
	coordData.TBN = mat4(aTangent, aBitangent, aNormal, aPosition);
	coordData.TBN = uMV_nrm * coordData.TBN;

	gl_Position = uP * coordData.mvPosition;
}