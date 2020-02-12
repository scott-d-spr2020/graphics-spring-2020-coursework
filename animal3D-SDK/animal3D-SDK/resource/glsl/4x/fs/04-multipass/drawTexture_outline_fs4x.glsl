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

//we need to use sampler uImage1 or uImage2
uniform sampler2D uImage1;
uniform sampler2D uImage2;
uniform vec2 uSize;
uniform vec4 uColor;
uniform vec2 uAxis;

mat3 sx = mat3(
1.0, 2.0, 1.0,
0.0, 0.0, 0.0,
-1.0, -2.0, -1.0
);

mat3 sy = mat3(
1.0, 0.0, -1.0,
2.0, 0.0, -2.0,
1.0, 0.0, -1.0
);

in vec2 outTexCoord;

void main()
{
	mat3 I;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			float x = (outTexCoord.x + (float(i-1) / uSize.x));
			float y = (outTexCoord.y + (float(j-1) / uSize.y));
			//switch to texture(sampler, texCoord)
			vec3 samp = texelFetch(uImage2, ivec2(gl_FragCoord) + ivec2(i-1 + uAxis.x, j-1 + uAxis.y), 0).rgb;

			//Does something cool but definitely not right
			//vec3 samp = texture(uImage1, vec2(x, y)).rgb;
			I[i][j] = length(samp);
		}
	}

	float gx = 0, gy = 0;
	for(int i = 0; i < 3; i++)
	{
		gx += dot(sx[i], I[i]);
		gy += dot(sy[i], I[i]);
	}

	float g = sqrt((gx * gx) + (gy * gy));
	vec4 mainSample = texture(mainTex, outTexCoord);
	vec4 depthSample = texture(uImage1, outTexCoord);
	vec4 normalSample = texture(uImage2, outTexCoord);

	//rtFragColor = vec4(mainSample.rgb - vec3(g), 1.0);
	rtFragColor = vec4(mainSample.rgb - vec3(g), 1.0);
	//rtFragColor = vec4(normalSample.rgb, 1.0);
}
