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


void a3materialParseFile(a3_RenderMaterial* mat, const a3byte* data)
{
	int uniformCount = 14;

	mat->numPasses = 1;
	mat->passes = malloc(sizeof(a3_RenderPass) * mat->numPasses);

	char* token;
	size_t position = 0;

	printf("%s\n", data);

	token = strtok((char*)data, "\n");
	position += strlen(token);

	while (token != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			if (strstr(token, (char*)keywords[i]))
			{
				token = strtok(NULL, "\n");
				a3materialParserHandleKeyword(keywords[i], token, mat);
			}
		}

		token = strtok(NULL, "\n");
	}
}

void a3materialParserHandleKeyword(const a3byte* keyword, const a3byte* data, a3_RenderMaterial* mat)
{
	if (keyword == keywords[0])
	{
		a3materialParserHandleProgram(data, mat);
	}
	else if (strcmp((char*)keyword, (char*)keywords[1]) != 0)
	{
		
	}
	else if (strcmp((char*)keyword, (char*)keywords[2]) != 0)
	{
		
	}
}

void a3materialParserHandleProgram(const a3byte* data, a3_RenderMaterial* mat)
{
	if (strstr((char*)data, (char*)shaderProgNames[0]))
	{
		
	}
}