/*
	Copyright 2011-2019 Daniel S. Buckstein

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
	
	a3_Hierarchy.h
	Node tree structure forming a hierarchy. Nodes belonging to the same 
		hierarchy know the index of their parent node, but not child nodes.
*/

#ifndef __ANIMAL3D_RENDERMATERIAL_H
#define __ANIMAL3D_RENDERMATERIAL_H


#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3utility/a3_Stream.h"
#include "animal3D-A3DG/a3graphics/a3_Framebuffer.h"
#include "animal3D-A3DG/a3graphics/a3_ShaderProgram.h"
#include "animal3D-A3DG/a3graphics/a3_Texture.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_RenderMaterial			a3_RenderMaterial;
	typedef struct a3_RenderPass				a3_RenderPass;
	typedef enum a3_UniformSwitch				a3_UniformSwitch;
	typedef enum a3_MaterialTextureType			a3_MaterialTextureType;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	enum a3_UniformSwitch
	{
		uniformSwitch_Int = 0,
		uniformSwitch_Float = 1,
		uniformSwitch_Double = 2,
		uniformSwitch_FloatMat = 3,
		uniformSwitch_DoubleMat = 4,
		uniformSwitch_Buffer = 5,
		uniformSwitch_TextureUnit = 6
	};

	enum a3_MaterialTextureType
	{
		textype_color = 0,
		textype_normal,
		textype_metallic,
		textype_roughness,

		total_textypes
	};

	struct a3_RenderPass //TODO we only support floats currently. How do we support doubles and ints?
	{
		a3_Framebuffer* writeFBO;					// output
		a3_ShaderProgram* shaderProgram;			// shader program to use
		a3i32* uniformHandles;						// the handle (or in some cases texture unit) to pass uniforms to. Used as the index for UBOs
		a3i32* uniformTypes;						// is it a vec3? a mat4? used in conjunction with uniformFlags.
		a3_UniformSwitch* uniformFlags;				// we're combining uniform and uniformMat, so this distinguishes what to parse the uniformType as
		void** sources;								// sources can be ANYTHING. We're relying on the uniform type to help distinguish things.
		a3ui32* unifDataCounts;						// how many of a uniform to pass. Will usually be 1.
		a3ui32* unifSourceTargets;					// the target for texture units
		a3ui32 numUniforms;
		a3_MaterialTextureType requiredTextures[a3tex_unitMax];		// list of required textures for this pass
		a3ui32 numReqTextures;
	};
	
	struct a3_RenderMaterial
	{
		a3_RenderPass** passes;						//array of POINTERS to render passes. Not sure if passes can be reused yet. Probably?
		a3_MaterialTexture maps[a3tex_unitMax];		//TODO array of maps for each pass. Needs restructuring probably. Maybe map[] and a bitstring that's sizeof(passes)/sizeof(passes[0]) long?
		a3ui32 numPasses;
		a3ui32 numMaps;
	};

//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_MATERIAL_H