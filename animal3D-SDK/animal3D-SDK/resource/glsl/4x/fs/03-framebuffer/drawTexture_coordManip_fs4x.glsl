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
	
	drawTexture_coordManip_fs4x.glsl
	Draw texture sample after manipulating texcoord.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variable for texture; see demo code for hints
//	2) declare inbound varying for texture coordinate
//	3) modify texture coordinate in some creative way
//	4) sample texture using modified texture coordinate
//	5) assign sample to output color

out vec4 rtFragColor;
uniform sampler2D mainTex;
in vec2 outTexCoord;
const float radius = 0.25f;
const vec2 center = vec2(0.5f);
uniform double uTime;


vec2 ccwRot(vec2 coord, vec2 origin, float time)
{
    vec2 currDir = coord - origin;
    float currAngle = atan(currDir.y/currDir.x);
	float dist = length(currDir);
	vec2 newDir = vec2(cos(currAngle + time), sin(currAngle + time));
	return origin + (newDir * dist);
}

vec2 cwRot(vec2 coord, vec2 origin, float time)
{
    vec2 currDir = coord - origin;
    float currAngle = atan(currDir.y/currDir.x);
	float dist = length(currDir);
	vec2 newDir = vec2(cos(currAngle + time), -sin(currAngle + time));
	return origin + (newDir * dist);
}



void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE DARK GREY
	float stepOutput = step(distance(outTexCoord, center), 0.25f);
	float fTime = 0.1f * float(uTime);
	vec2 ccwTexCoord = ccwRot(outTexCoord, center, fTime);
	vec2 cwTexCoord = cwRot(outTexCoord, center, fTime);
	vec2 tempTexCoord = (stepOutput * ccwTexCoord + ((1.0f - stepOutput) * cwTexCoord));
	rtFragColor = texture(mainTex, tempTexCoord);
}