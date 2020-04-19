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
		//start of loading uniforms, not sure what else I'm missing or if this works.
		switch (pass->uniformFlags[i])
		{
		case uniformSwitch_Int:
			a3shaderUniformSendInt(pass->uniformTypes[i], pass->uniformHandles[i], pass->unifDataCounts[i], (const a3i32*)pass->sources[i]);
			break;
		case uniformSwitch_Float:
			a3shaderUniformSendFloat(pass->uniformTypes[i], pass->uniformHandles[i], pass->unifDataCounts[i], (const a3f32*)pass->sources[i]);
			break;
		case uniformSwitch_Double:
			a3shaderUniformSendDouble(pass->uniformTypes[i], pass->uniformHandles[i], pass->unifDataCounts[i], (const a3f64*)pass->sources[i]);
			break;
		case uniformSwitch_FloatMat: //this only supports floats. How do we support doubles?
			a3shaderUniformSendFloatMat(pass->uniformTypes[i], 0, pass->uniformHandles[i], pass->unifDataCounts[i], (const a3f32*)pass->sources[i]);
			break;
		case uniformSwitch_DoubleMat:
			a3shaderUniformSendDoubleMat(pass->uniformTypes[i], 0, pass->uniformHandles[i], pass->unifDataCounts[i], (const a3f64*)pass->sources[i]);
			break;
		case uniformSwitch_Buffer:
			//if it's a uniform buffer, the handle is just the index to bind it at. Usually a 0, 1, or 4. I'm not sure how this would be programmatically solved.
			//look up ubTransformStack and ubo_transformStack_model for more information.
			a3shaderUniformBufferActivate((const a3_UniformBuffer*)pass->sources[i], pass->uniformHandles[i]);
			break;
		case uniformSwitch_TextureUnit:
			//binds a source texture to the correct color unit
			a3framebufferBindColorTexture((const a3_Framebuffer*)pass->sources[i], pass->uniformHandles[i], pass->unifSourceTargets[i]);
			break;
		}
	}
	return 0;
}
