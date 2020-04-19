#include "..\a3_DemoMaterialUtils.h"

a3ret drawMaterial(const a3_VertexDrawable* drawable, const a3_RenderMaterial* mat)
{
	for (a3ui32 i = 0; i < mat->numPasses; ++i)
	{
		drawPass(drawable, mat->passes[i]);
	}
	return 0;
}

a3ret drawPass(const a3_VertexDrawable* drawable, const a3_RenderPass* pass)
{
	for (a3ui32 i = 0; i < pass->numUniforms; ++i)
	{
		switch (pass->uniformFlags[i])
		{
		case uniform:
			switch (pass->uniformTypes[i])
			{

			}
			break;
		case uniformMat:
			switch (pass->uniformTypes[i])
			{

			}
			break;
		case uniformBuffer:
			switch (pass->uniformTypes[i])
			{

			}
			break;
		case textureUnit:
			a3framebufferBindColorTexture((const a3_Framebuffer*)pass->sources[i], pass->uniformTypes[i], pass->unifSourceTargets[i]);
			break;
		}
	}
	return 0;
}
