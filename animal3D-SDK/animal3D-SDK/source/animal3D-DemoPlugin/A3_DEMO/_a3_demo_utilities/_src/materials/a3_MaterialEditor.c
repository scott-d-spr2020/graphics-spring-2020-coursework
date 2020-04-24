#include "..\..\materials\a3_MaterialEditor.h"
#include <string.h>
#include <stdio.h>


a3byte const* keywords[3] = {
	"shaderprogram",
	"texture",
	"renderpackage"
};

a3byte const* texTypeStrings[4] =
{
	"color",
	"normal",
	"metallic",
	"roughness"
};

a3byte const* shaderProgNames[32] = {
	"prog:draw-Phong-mul-fwd-mrt",
	"prog:draw-nonphoto-multi-mrt"
};


void a3materialParseFile(ParserData* pData, a3byte const* data)
{
	pData->state->materials[0].numPasses = 1; //Arbitrary for now, should we define it in the file?
	pData->state->materials[0].passes = malloc(sizeof(a3_RenderPass) * pData->mat->numPasses);

	char* token;

	// Just a delimiter function, using it to pull out whole lines, since the material file is formatted in a certain way
	token = strtok((char*)data, "\n");

	while (token != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			// lots of strstr() is used because escape characters are in the data, so finding substrings is the easiest way
			if (strstr(token, (char*)keywords[i]))
			{
				// If you pass NULL, it uses the data from the last strtok call
				token = strtok(NULL, "\n");
				a3materialParserHandleKeyword(keywords[i], token, pData);	//This passes the next line of data (retrieved just above) which holds the actual user-defined data
				break;
			}
		}

		token = strtok(NULL, "\n");
	}
}

void a3materialParserHandleKeyword(const a3byte* keyword, const a3byte* data, ParserData* pData)
{
	// Can do address comparisons here because I'm sending in the keyword from the array anyways, this just determines which one it is

	if (keyword == keywords[0])
	{
		a3materialParserHandleProgram(data, pData);
	}
	else if (keyword == keywords[1])
	{
		a3materialParserHandleTexture(data, pData);
	}
	else if (keyword == keywords[2])
	{
		
	}
}

void a3materialParserHandleProgram(const a3byte* data, ParserData* pData)
{
	// Need to figure out better way of defining number of program names and keywords

	if (strstr((char*)data, (char*)shaderProgNames[0]))
	{
		initRenderPass(&pData->state->passes[0], pData->numUnifs, pData->state->fbo_scene_c16d24s8_mrt, pData->state->prog_drawPhong_multi_forward_mrt);
	}
	else if (strstr((char*)data, (char*)shaderProgNames[1]))
	{
		initRenderPass(&pData->state->passes[0], pData->numUnifs, pData->state->fbo_scene_c16d24s8_mrt, pData->state->prog_drawNonphoto_multi_mrt);
	}
	else
	{
		printf("\nERROR: %s is not a valid shader program for material use!\n", (char*)data);
	}
}

// Data here will contain the texture type and the file path under the resource folder (we'll require it to be in the resources folder somewhere), separated by a space
void a3materialParserHandleTexture(const a3byte* data, ParserData* pData)
{
	// This will LOAD in a texture from file just like it does in loading, BUT...
	// we need each material to have its own set of texture handles (like demoState).
	// These would be tex_color, tex_normal, tex_metallic, tex_roughness, etc.
	// Refer to 994-1004 in demoState loading for what I think we should do
	// This way, we can dynamically load in textures, and not have to worry about demoState having handles

	char* texType = strtok((char*)data, " ");
	char* filePath = strtok(NULL, "\n");
	char* relativePath = "../../../../resource/";
	a3_Texture* tex;

	filePath = strcat(relativePath, filePath);

	if (strstr(texType, texTypeStrings[0]))
	{
		int flag = a3textureCreateFromFile(pData->mat->matTex_color, "tex:material-color", filePath);

		if (flag <= 0)
		{
			printf("\nERROR: could not find texture %s , code %i !\n", filePath, flag);
		}

		a3textureActivate(pData->mat->matTex_color, a3tex_unit00);
		a3textureDefaultSettings();

		tex = pData->mat->matTex_color;
	}
	else if (strstr(texType, texTypeStrings[1]))
	{
		int flag = a3textureCreateFromFile(pData->mat->matTex_normal, "tex:material-normal", filePath);

		if (flag <= 0)
		{
			printf("\nERROR: could not find texture %s , code %i !\n", filePath, flag);
		}

		a3textureActivate(pData->mat->matTex_normal, a3tex_unit00);
		a3textureDefaultSettings();

		tex = pData->mat->matTex_normal;
	}
	else if (strstr(texType, texTypeStrings[2]))
	{
		int flag = a3textureCreateFromFile(pData->mat->matTex_metallic, "tex:material-metallic", filePath);

		if (flag <= 0)
		{
			printf("\nERROR: could not find texture %s , code %i !\n", filePath, flag);
		}

		a3textureActivate(pData->mat->matTex_metallic, a3tex_unit00);
		a3textureDefaultSettings();

		tex = pData->mat->matTex_metallic;
	}
	else if (strstr(texType, texTypeStrings[3]))
	{
		int flag = a3textureCreateFromFile(pData->mat->matTex_roughness, "tex:material-roughness", filePath);

		if (flag <= 0)
		{
			printf("\nERROR: could not find texture %s , code %i !\n", filePath, flag);
		}

		a3textureActivate(pData->mat->matTex_roughness, a3tex_unit00);
		a3textureDefaultSettings();

		tex = pData->mat->matTex_roughness;
	}
	else
	{
		tex = NULL;
		printf("\nERROR: %s is not a valid texture type for material use!\n", texType);
	}

	if (tex != NULL)
	{
		a3textureActivate(tex, a3tex_unit00);
		a3textureChangeFilterMode(a3tex_filterLinear);

		a3textureDeactivate(a3tex_unit00);
	}
}