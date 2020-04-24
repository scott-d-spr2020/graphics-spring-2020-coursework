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

#ifndef __ANIMAL3D_RENDERPASS_H
#define __ANIMAL3D_RENDERPASS_H


#include "animal3D/a3/a3types_integer.h"
#include "animal3D/a3utility/a3_Stream.h"
#include "a3_RenderMaterial.h"
#include "..//a3_DemoMaterialUtils.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct ParserData				ParserData;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

//Data holder for the parser to access certain things
//Necessary to get both the proper uniform count, demoState (for programs), and renderMaterial from loading to the parser
struct ParserData
{
	a3_DemoState* state;
	a3_RenderMaterial* mat;
	int numUnifs;
};



void a3materialParseFile(ParserData* parserData, a3byte const* data);

void a3materialParserHandleKeyword(const a3byte* keyword, const a3byte* data, ParserData* pData);

void a3materialParserHandleProgram(const a3byte* data, ParserData* pData);

void a3materialParserHandleTexture(const a3byte* data, ParserData* pData);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_MATERIAL_H