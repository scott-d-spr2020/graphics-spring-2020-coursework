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
	
	drawTexture_colorManip_fs4x.glsl
	Draw texture sample and manipulate result.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variable for texture; see demo code for hints
//	2) declare inbound varying for texture coordinate
//	3) sample texture using texture coordinate
//	4) modify sample in some creative way
//	5) assign modified sample to output color

const float radius = 0.25f;
const vec2 center = vec2(0.5f);

out vec4 rtFragColor;
uniform double uTime;
uniform sampler2D mainTex;
in vec2 outTexCoord;

vec4 CalculateManipulatedColor(float fTime)
{
	float r = sin(fTime);
	float g = -sin(0.5f * fTime);
	float b = cos(fTime);
	float a = 0.5f*sin(1.0f * fTime) + 0.5f;

	return vec4(r, g, b, a);
}

vec2 CalculateManipulatedCoord(float fTime)
{
vec2 currDir = outTexCoord - center;
    float currAngle = atan(currDir.y,currDir.x);
	float dist = length(currDir);
	float ccwAngle = currAngle + fTime;
	float cwAngle = currAngle - fTime;
	//two new texcoords
	vec2 ccwTexCoord = center + vec2(cos(ccwAngle), sin(ccwAngle)) * dist;
	vec2 cwTexCoord = center + vec2(cos(cwAngle), sin(cwAngle)) * dist;
	float stepOutput = step(distance(outTexCoord, center), radius);

	vec2 tempTexCoord = (stepOutput * ccwTexCoord + ((1.0f - stepOutput) * cwTexCoord));
	return tempTexCoord;
}

void main()
{
	float fTime = float(uTime);
	vec4 manipColor = CalculateManipulatedColor(fTime);
	
	fTime = 0.1f * float(uTime);
	vec2 manipCoord = CalculateManipulatedCoord(fTime);

	rtFragColor = texture(mainTex, manipCoord) * manipColor;
}
