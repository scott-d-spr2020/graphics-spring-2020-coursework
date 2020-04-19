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
	
	passLightingData_shadowCoord_transform_vs4x.glsl
	Vertex shader that prepares and passes lighting data. Outputs transformed 
		position attribute and all others required for lighting. Also computes 
		and passes shadow coordinate.
*/

#version 410

// ****TO-DO: 
//	0) copy previous lighting data vertex shader
//	1) declare MVPB matrix for light
//	2) declare varying for shadow coordinate
//	3) calculate and pass shadow coordinate

layout (location = 0) in vec4 aPosition;
layout (location = 2) in vec4 aNormal;
layout (location = 8) in vec4 inTexCoord0;
layout (location = 10) in vec4 aTangent;
layout (location = 11) in vec4 aBitangent;


out CoordData
{
	vec2 texCoord;
	vec4 mvPosition;
	mat3 TBN;
	vec4 shadowCoord;
} coordData;

uniform mat4 uMV;
uniform mat4 uMV_nrm;
uniform mat4 uP;
uniform mat4 uAtlas;
uniform mat4 uMVPB_other;

void main()
{
	coordData.mvPosition = uMV * aPosition;
	coordData.texCoord = (uAtlas * inTexCoord0).xy;

	coordData.shadowCoord = uMVPB_other * aPosition;

	coordData.TBN = mat3(normalize(uMV_nrm * aTangent), normalize(uMV_nrm * aBitangent), normalize(uMV_nrm * aNormal));

	gl_Position = uP * coordData.mvPosition;
}