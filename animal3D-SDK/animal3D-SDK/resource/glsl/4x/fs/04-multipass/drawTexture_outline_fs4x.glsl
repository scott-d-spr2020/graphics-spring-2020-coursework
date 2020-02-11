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
	
	drawTexture_outline_fs4x.glsl
	Draw texture sample with outlines.
*/

#version 410

// ****TO-DO: 
//	0) copy existing texturing shader
//	1) implement outline algorithm - see render code for uniform hints

out vec4 rtFragColor;

uniform sampler2D mainTex;

in vec2 outTexCoord;

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE DARK GREY
	vec2 scale = vec2(1.0f)/textureSize(mainTex, 0);
	vec4 leftPix = texture(mainTex, outTexCoord - vec2(scale.x, 0));
	vec4 rightPix = texture(mainTex, outTexCoord + vec2(scale.x, 0));
	rtFragColor = vec4(0.2, 0.2, 0.2, 1.0);
}
