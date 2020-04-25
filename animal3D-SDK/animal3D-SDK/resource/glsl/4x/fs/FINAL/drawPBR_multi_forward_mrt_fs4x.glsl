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
	
	drawPhong_multi_forward_mrt_fs4x.glsl
	Draw Phong shading model using forward light set.
*/

#version 430

#define MAX_LIGHTS 4

// ****TO-DO: 
//	0) nothing

in vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
	//vec4 vLocalReflectedSurfaceToViewerDir;	// For reflections
	flat int vVertexID, vInstanceID, vModelID;
};


struct sPointLight
{
	vec4 worldPos;
	vec4 viewPos;
	vec4 color;
	float radius;
	float radiusInvSq;
	float radiusInv;
	float radiusSq;
};


uniform ubPointLight {
	sPointLight uPointLight[MAX_LIGHTS];
};

uniform int uLightCt;
uniform vec4 uColor;

uniform sampler2D uImage00;	// color
uniform sampler2D uImage01;	// normal
uniform sampler2D uImage02;	// metallic
uniform sampler2D uImage03; // roughness
uniform samplerCube uImage04; // skybox cubemap


// final color
layout (location = 0) out vec4 rtFragColor;

// attribute data
layout (location = 1) out vec4 rtAtlasTexcoord;
layout (location = 2) out vec4 rtViewTangent;
layout (location = 3) out vec4 rtViewBitangent;
layout (location = 4) out vec4 rtViewNormal;
layout (location = 5) out vec4 rtViewPosition;

// lighting data
layout (location = 6) out vec4 rtDiffuseLightTotal;
layout (location = 7) out vec4 rtSpecularLightTotal;


float pow64(float v)
{
	v *= v;	// ^2
	v *= v;	// ^4
	v *= v;	// ^8
	v *= v;	// ^16
	v *= v;	// ^32
	v *= v;	// ^64
	return v;
}


vec3 refl(in vec3 v, in vec3 n, in float d)
{
	return ((2.0 * d) * n - v);
}


float calcDiffuseCoefficient(
	out vec3 lightVec, out float lightDist, out float lightDistSq,
	in vec3 lightPos, in vec3 fragPos, in vec3 fragNrm)
{
	lightVec = lightPos - fragPos;
	lightDistSq = dot(lightVec, lightVec);
	lightDist = sqrt(lightDistSq);
	lightVec /= lightDist;
	return dot(lightVec, fragNrm);
}


float calcSpecularCoefficient(
	out vec3 reflVec, out vec3 eyeVec,
	in vec3 lightVec, in float diffuseCoefficient,
	in vec3 fragPos, in vec3 fragNrm, in vec3 eyePos)
{
	reflVec = refl(lightVec, fragNrm, diffuseCoefficient);
	eyeVec = normalize(eyePos - fragPos);
	return dot(reflVec, eyeVec);
}


float calcAttenuation(
	float lightDist, float lightDistSq,
	float lightSz, float lightSzInvSq, float lightSzInv, float lightSzSq)
{
//	float atten = max(0.0, (1.0 - lightDistSq * lightSzInvSq));
	float atten = (1.0 / (1.0 + 2.0 * lightDist * lightSzInv + lightDistSq * lightSzInvSq));
	return atten;
}


void addPhongComponents(
	inout vec3 diffuseLightTotal, out float diffuseCoefficient,
	inout vec3 specularLightTotal, out float specularCoefficient,
	in vec3 lightPos, in vec3 lightCol,
	in float lightSz, in float lightSzInvSq, in float lightSzInv, in float lightSzSq,
	in vec3 fragPos, in vec3 fragNrm, in vec3 eyePos)
{
	float lightDist, lightDistSq, attenuation;
	vec3 lightVec, reflVec, eyeVec;
	vec3 attenuationColor;

	diffuseCoefficient = calcDiffuseCoefficient(
		lightVec, lightDist, lightDistSq,
		lightPos, fragPos, fragNrm);
	specularCoefficient = calcSpecularCoefficient(
		reflVec, eyeVec,
		lightVec, diffuseCoefficient,
		fragPos, fragNrm, eyePos);
	attenuation = calcAttenuation(
		lightDist, lightDistSq,
		lightSz, lightSzInvSq, lightSzInv, lightSzSq);

	diffuseCoefficient = max(0.0, diffuseCoefficient);
	specularCoefficient = pow64(max(0.0, specularCoefficient));

	attenuationColor = attenuation * lightCol;
	diffuseLightTotal += attenuationColor * diffuseCoefficient;
	specularLightTotal += attenuationColor * specularCoefficient;
}


void main()
{
	// DUMMY OUTPUT: all fragments are colored based on model index
//	vec4 color[6] = vec4[6] ( vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 1.0, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 1.0), vec4(0.0, 1.0, 1.0, 1.0), vec4(0.0, 0.0, 1.0, 1.0), vec4(1.0, 0.0, 1.0, 1.0) );
//	rtFragColor = color[vModelID % 6];

	//vec4 normalizedReflectedSurfaceToViewerDir = normalize(vLocalReflectedSurfaceToViewerDir);
	//vec3 reflectionSourceRGB = texture(uImage04, normalizedReflectedSurfaceToViewerDir.xyz).rgb;	//reflections all need cube map
	//vec3 reflectionRGB = reflectionSourceRGB;

	mat4 tangentBasis_view = mat4(
		normalize(vTangentBasis_view[0]),
		normalize(vTangentBasis_view[1]),
		normalize(vTangentBasis_view[2]),
		vTangentBasis_view[3]
	);

	vec4 T = tangentBasis_view[0];
	vec4 B = tangentBasis_view[1];
	vec4 N = tangentBasis_view[2];
	vec4 P = tangentBasis_view[3];

	mat3 TBN = mat3(T, B, N);

	vec3 map_normal = texture(uImage01, vTexcoord_atlas.xy).rgb;
	map_normal = map_normal * 2.0 - 1.0;
	map_normal = normalize(TBN * map_normal);

	int i;
	sPointLight light;
	float kd, ks;
	vec3 eyePos = vec3(0.0);
	vec3 ambient = uColor.rgb * 0.1,
		diffuseLightTotal = vec3(0.0),
		specularLightTotal = diffuseLightTotal;

	for (i = 0; i < uLightCt; ++i)
	{
		light = uPointLight[i];
		addPhongComponents(
			diffuseLightTotal, kd,
			specularLightTotal,ks,
			light.viewPos.xyz, light.color.rgb,
			light.radius, light.radiusInvSq, light.radiusInv, light.radiusSq,
			P.xyz, map_normal.xyz, eyePos);
	}


	// textures
	vec4 sample_dm = texture(uImage00, vTexcoord_atlas.xy);
	vec3 sample_sm = texture(uImage02, vTexcoord_atlas.xy).rgb;

	//reflectionRGB *= sample_sm;


	// final color
	rtFragColor.rgb = ambient
					+ sample_dm.rgb * diffuseLightTotal
					+ sample_sm.rgb * specularLightTotal;
	rtFragColor.a = sample_dm.a;

	//rtFragColor = vec4(map_normal.rgb, 1.0);
	//rtFragColor = vec4(reflectionRGB, 1.0);	// This will work as soon as Animal has cube map support

	// output attributes
	rtAtlasTexcoord = vTexcoord_atlas;
	rtViewTangent = vec4(T.xyz * 0.5 + 0.5, 1.0);
	rtViewBitangent = vec4(B.xyz * 0.5 + 0.5, 1.0);
	//rtViewNormal = vec4(N.xyz * 0.5 + 0.5, 1.0);
	rtViewNormal = vec4(map_normal.xyz, 1.0);
	rtViewPosition = P;

	// output lighting
	rtDiffuseLightTotal = vec4(diffuseLightTotal, 1.0);
	rtSpecularLightTotal = vec4(specularLightTotal, 1.0);
}
