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
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	enum a3_UniformSwitch
	{
		uniform = 0,
		uniformMat = 1,
		uniformBuffer = 2,
		textureUnit = 3
	};

	struct a3_RenderPass
	{
		a3_Framebuffer* writeFBO;
		a3_ShaderProgram* shaderProgram;
		a3i32* uniformHandles;						//the handle (or in some cases texture unit) to pass uniforms to
		a3i32* uniformTypes;
		a3_UniformSwitch* uniformFlags;			//we're combining uniform and uniformMat, so this distinguishes what to parse the uniformType as
		void** sources;								//sources can be ANYTHING. We're relying on the uniform type to help distinguish things.
		a3ui32* unifDataCounts;						//how many of a uniform to pass. Will usually be 1.
		a3ui32* unifSourceTargets;					//the target for texture units
		a3ui32 numUniforms;
	};
	
	struct a3_RenderMaterial
	{
		a3_RenderPass** passes;						//array of POINTERS to render passes. Not sure if passes can be reused yet. Probably?
		a3_Texture** maps;							//TODO array of maps for each pass. Needs restructuring probably. Maybe map[] and a bitstring that's sizeof(passes)/sizeof(passes[0]) long?
		a3ui32 numPasses;
		a3ui32 numMaps;
	};

//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_MATERIAL_H