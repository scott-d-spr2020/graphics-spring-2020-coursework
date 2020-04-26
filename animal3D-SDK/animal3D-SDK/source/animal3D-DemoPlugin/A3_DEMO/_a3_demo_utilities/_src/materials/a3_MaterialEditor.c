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

//a3byte const* shaderProgNames[32] = {
//	"prog:draw-Phong-mul-fwd-mrt",
//	"prog:draw-nonphoto-multi-mrt"
//}
a3byte const* shaderProgNames[32] = {
	"Phong",
	"Cel",
	"Gooch",
	"PBR",
	"Shadow-Ph",
	"Shadow-C"
};

int matNum = 0;

void a3materialReset()
{
	matNum = 0;
}

void a3materialParseFile(ParserData* pData, a3byte const* data)
{
	pData->state->materials[matNum].numPasses = 1; //Arbitrary for now, should we define it in the file?
	pData->state->materials[matNum].passes = calloc(pData->state->materials[matNum].numPasses, sizeof(a3_RenderPass *));
	for (a3ui32 i = 0; i < pData->state->materials[matNum].numPasses; i++)
	{
		pData->state->materials[matNum].passes[i] = calloc(1, sizeof(a3_RenderPass));
	}
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
	++matNum;
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
	int success = 0;
	if (strstr((char*)data, (char*)shaderProgNames[0]))
	{
		initRenderPass(pData->state->materials[matNum].passes[0], pData->numUnifs, pData->state->fbo_scene_c16d24s8_mrt, pData->state->prog_drawPhong_multi_mrt);
		success = 1;
	}
	else if (strstr((char*)data, (char*)shaderProgNames[1]))
	{
		initRenderPass(pData->state->materials[matNum].passes[0], pData->numUnifs, pData->state->fbo_scene_c16d24s8_mrt, pData->state->prog_drawNonphoto_multi_mrt);
		success = 1;
	}
	else if (strstr((char*)data, (char*)shaderProgNames[2]))
	{
		//gooch
		//initRenderPass(pData->state->materials[matNum].passes[0], pData->numUnifs, pData->state->fbo_scene_c16d24s8_mrt, pData->state->prog_drawNonphoto_multi_mrt);
		//success = 1;
	}
	else if (strstr((char*)data, (char*)shaderProgNames[3]))
	{
		//pbr
		initRenderPass(pData->state->materials[matNum].passes[0], pData->numUnifs, pData->state->fbo_scene_c16d24s8_mrt, pData->state->prog_PBR_rendering);
		success = 1;
	}
	else if (strstr((char*)data, (char*)shaderProgNames[4]))
	{
		//phong-shadow
		initRenderPass(pData->state->materials[matNum].passes[0], pData->numUnifs, pData->state->fbo_scene_c16d24s8_mrt, pData->state->prog_drawPhong_multi_shadow_mrt);
		success = 1;
	}
	else if (strstr((char*)data, (char*)shaderProgNames[5]))
	{
		//cel-shadow
		initRenderPass(pData->state->materials[matNum].passes[0], pData->numUnifs, pData->state->fbo_scene_c16d24s8_mrt, pData->state->prog_drawNP_multi_shadow_mrt);
		success = 1;
	}
	else
	{
		printf("\nERROR: %s is not a valid shader program for material use!\n", (char*)data);
	}
	if (success)
	{
		registerCommonUniforms(pData->state, pData->state->materials[matNum].passes[0]);
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

	//char* texType = strtok((char*)data, " ");
	//char* dirtyFilePath = strtok(NULL, "\n");

	char* temp = calloc(1, strlen(data));
	strcpy(temp, data);
	temp = temp + 1; //clear the \t
	int i;
	for (i = 0; temp[i] != ' '; ++i) //wind through till we find the space
	{

	}
	temp[i] = '\0'; //separate the string into two
	char* texType = temp;
	char* dirtyFilePath = temp + i + 1;
	char* filePath = calloc(1, strlen(dirtyFilePath));

	strcpy(filePath, dirtyFilePath);

	char c = filePath[strlen(filePath) - 1];
	if (c == '\r' || c == '\n')
	{
		filePath[strlen(filePath) - 1] = '\0';
	}

	char* relativePath = "../../../../";
	char* tempPath = calloc(strlen(relativePath) + strlen(filePath) + 1, sizeof(char));
	a3_Texture* tex;

	strcat(tempPath, relativePath);
	strcat(tempPath, filePath);

	if (strstr(texType, texTypeStrings[0]))
	{
		int flag = a3textureCreateFromFile(pData->state->materials[matNum].matTex_color, "tex:material-color", tempPath);

		if (flag <= 0)
		{
			printf("ERROR: texture %s not found, code %i !\n", filePath, flag);
			return;
		}

		a3textureActivate(pData->state->materials[matNum].matTex_color, a3tex_unit00);
		a3textureDefaultSettings();

		tex = pData->state->materials[matNum].matTex_color;
	}
	else if (strstr(texType, texTypeStrings[1]))
	{
		int flag = a3textureCreateFromFile(pData->state->materials[matNum].matTex_normal, "tex:material-normal", tempPath);

		if (flag <= 0)
		{
			printf("ERROR: texture %s not found, code %i !\n", filePath, flag);
			return;
		}

		a3textureActivate(pData->state->materials[matNum].matTex_normal, a3tex_unit00);
		a3textureDefaultSettings();

		tex = pData->state->materials[matNum].matTex_normal;
	}
	else if (strstr(texType, texTypeStrings[2]))
	{
		int flag = a3textureCreateFromFile(pData->state->materials[matNum].matTex_metallic, "tex:material-metallic", tempPath);

		if (flag <= 0)
		{
			printf("ERROR: texture %s not found, code %i !\n", filePath, flag);
			return;
		}

		a3textureActivate(pData->state->materials[matNum].matTex_metallic, a3tex_unit00);
		a3textureDefaultSettings();

		tex = pData->state->materials[matNum].matTex_metallic;
	}
	else if (strstr(texType, texTypeStrings[3]))
	{
		int flag = a3textureCreateFromFile(pData->state->materials[matNum].matTex_roughness, "tex:material-roughness", tempPath);

		if (flag <= 0)
		{
			printf("ERROR: texture %s not found, code %i !\n", filePath, flag);
			return;
		}

		a3textureActivate(pData->state->materials[matNum].matTex_roughness, a3tex_unit00);
		a3textureDefaultSettings();

		tex = pData->state->materials[matNum].matTex_roughness;
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