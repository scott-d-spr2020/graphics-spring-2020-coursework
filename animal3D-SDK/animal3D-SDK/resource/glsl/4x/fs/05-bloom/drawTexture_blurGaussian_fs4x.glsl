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
	
	drawTexture_blurGaussian_fs4x.glsl
	Draw texture with Gaussian blurring.
*/

#version 410

// ****TO-DO: 
//	0) copy existing texturing shader
//	1) declare uniforms for pixel size and sampling axis
//	2) implement Gaussian blur function using a 1D kernel (hint: Pascal's triangle)
//	3) sample texture using Gaussian blur function and output result

uniform sampler2D uImage00;
uniform vec2 uAxis;
uniform vec2 uSize;

layout (location = 0) out vec4 rtFragColor;
layout (location = 3) out vec4 rtTexCoord;

in vec2 outTexCoord;

const float weights[] = float[](1,4,6,4,1);

void main()
{
	rtFragColor = texture(uImage00, outTexCoord);
	vec4 totalSamp;
	for (int i = 0; i < 5; i++)
	{
		float xCoord = outTexCoord.x + (i-2) * (1.0f/uAxis.x) * uSize.x;
		float yCoord = outTexCoord.y + (i-2) * (1.0f/uAxis.y) * uSize.y;
		totalSamp += texture(uImage00, vec2(xCoord, yCoord));
	}
	totalSamp /= (1+4+6+4+1);
	rtTexCoord = totalSamp;
	rtTexCoord = vec4(1.0f,0.0f,0.0f,1.0f);
}