#include "..\..\materials\a3_MaterialEditor.h"
#include <string.h>
#include <stdio.h>


a3byte const* keywords[3] = {
	"shaderprogram",
	"texture",
	"renderpackage"
};

a3byte const* shaderProgNames[32] = {
	"prog:draw-Phong-mul-fwd-mrt",
	"prog:draw-nonphoto-multi-mrt"
};


void a3materialParseFile(ParserData* pData, a3byte const* data)
{
	//size_t fileLength = strlen((char*)data);

	//char* origData = malloc(fileLength);
	//strcpy(origData, (char*)data);

	pData->mat->numPasses = 1;
	pData->mat->passes = malloc(sizeof(a3_RenderPass) * pData->mat->numPasses);

	char* token;
	size_t position = 0;

	token = strtok((char*)data, "\n");
	position += strlen(token);

	while (token != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			if (strstr(token, (char*)keywords[i]))
			{
				token = strtok(NULL, "\n");
				a3materialParserHandleKeyword(keywords[i], token, pData);
				break;
			}
		}

		token = strtok(NULL, "\n");
	}
}

void a3materialParserHandleKeyword(const a3byte* keyword, const a3byte* data, ParserData* pData)
{
	if (keyword == keywords[0])
	{
		a3materialParserHandleProgram(data, pData);
	}
	else if (keyword = keywords[1])
	{
		
	}
	else if (keyword = keywords[2])
	{
		
	}
}

void a3materialParserHandleProgram(const a3byte* data, ParserData* pData)
{
	for (int i = 0; i < 2; i++)
	{
		if (strstr((char*)data, (char*)shaderProgNames[i]))
		{
			initRenderPass(pData->mat->passes[0], pData->numUnifs, pData->state->fbo_scene_c16d24s8_mrt, pData->state->prog_drawPhong_multi_forward_mrt);
			break;
		}
	}
}

void a3materialParserHandleTexture(const a3byte* data, ParserData* pData)
{
	// This will LOAD in a texture from file just like it does in loading, BUT...
	// we need each material to have its own set of texture handles (like demoState).
	// These would be tex_color, tex_normal, tex_metallic, tex_roughness, etc.
	// This means we can probably ditch the a3_MaterialTexture stuff
	// Refer to 994-1004 in demoState loading for what I think we should do
	// This way, we can dynamically load in textures, and not have to worry about demoState having handles
}