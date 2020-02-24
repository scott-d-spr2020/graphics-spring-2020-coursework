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
	
	drawPhongComposite_fs4x.glsl
	Draw final Phong shading model by compositing lighting with textures.
*/

#version 410

// ****TO-DO: 
//	1) declare texture coordinate g-buffer to get surface UVs
//	2) declare diffuse and specular maps as uniform samplers
//	3) declare diffuse and specular light textures as uniform samplers
//	4) declare ambient color as uniform vector
//	5) calculate Phong shading model by sampling from light buffers and 
//		multiplying by the respective surface texture sample and summing 
//		the components: 
//			PHONG =	ambient light 
//					+ diffuse light * diffuse sample 
//					+ specular light * specular sample 

in vec4 vTexcoord;

uniform sampler2D uImage01; // Diffuse light texture
uniform sampler2D uImage02;	// Specular light texture
uniform sampler2D uImage03; // Texcoord texture
uniform sampler2D uImage04; // Diffuse map
uniform sampler2D uImage05; // Specular map

uniform vec4 uColor; // New ambient color

layout (location = 0) out vec4 rtFragColor;
layout (location = 4) out vec4 rtDiffuseMapSample;
layout (location = 5) out vec4 rtSpecularMapSample;
layout (location = 6) out vec4 rtDiffuseTotal;
layout (location = 7) out vec4 rtSpecularTotal;

void main()
{
	vec2 sampleTexCoord = texture(uImage03, vTexcoord.xy).rg;

	vec4 diffuseLight = texture(uImage01, vTexcoord.xy);
	vec4 specularLight = texture(uImage02, vTexcoord.xy);
	vec4 diffuseSample = texture(uImage04, sampleTexCoord.xy);
	vec4 specularSample = texture(uImage05, sampleTexCoord.xy);

	rtFragColor = uColor * 0.03f + (diffuseLight * diffuseSample) + (specularLight * specularSample);
	rtDiffuseMapSample = diffuseSample;
	rtSpecularMapSample = specularSample;
	rtDiffuseTotal = vec4(diffuseLight.rgb, 1.0f);
	rtSpecularTotal = specularLight;
}
