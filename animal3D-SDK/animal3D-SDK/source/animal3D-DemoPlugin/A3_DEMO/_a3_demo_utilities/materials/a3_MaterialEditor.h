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

#ifndef __ANIMAL3D_MATERIALEDITOR_H
#define __ANIMAL3D_MATERIALEDITOR_H


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
// We store materials under DemoState rather than per-object so we aren't loading copies of materials.
struct ParserData
{
	a3_DemoState* state;
	//a3_RenderMaterial* mat;
	int numUnifs;
};

//Just reset the material number to 0 on reload
//it's...... it's just... oh god it's just data
void a3materialReset();

// Parsing function callback used by a3streamObjetRead for loading a material
//Params:
//parserData: pointer to a struct created in loading and passed in. Init parserData with the demoState and target number of uniforms.
//data: the a3FileStream data, which is the file contents. It's just data.
void a3materialParseFile(ParserData* parserData, a3byte const* data);

//Handle keywords encountered by the main parsing loop
//Params:
//keyword: the single word that the parser found and identified as a keyword
//data: the line of data following the keyword (contains the data pertinent to the keyword, i.e a shader program or texture)
//pData: the parser data containing demoState and num uniforms
//It's all just data.
void a3materialParserHandleKeyword(const a3byte* keyword, const a3byte* data, ParserData* pData);

//Handle a shader program found in the file, and load it to the corresponding material along with all common uniforms
//Params:
//data: the name of the shader program
//pData: the same parser data with demoState and num uniforms.
//Still all just data.
void a3materialParserHandleProgram(const a3byte* data, ParserData* pData);

//Handle a texture found in the file, and load it to the corresponding material's texture handle according to its type
//Params:
//data: line containing the texture type and file path (always under resources), separated by a space
//pData: the same parser data with demoState and num uniforms
//JUST DATA JUST DATA JUST DATA JUST DATA
void a3materialParserHandleTexture(const a3byte* data, ParserData* pData);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_MATERIAL_H