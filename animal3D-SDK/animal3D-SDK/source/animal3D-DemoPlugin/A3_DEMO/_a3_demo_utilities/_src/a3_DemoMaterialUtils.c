#include "..\a3_DemoMaterialUtils.h"
#include <assert.h>

a3ret drawMaterial(a3_DemoState const* demoState, const a3_VertexDrawable* drawable, const a3_RenderMaterial* mat)
{
	for (a3ui32 i = 0; i < mat->numPasses; ++i)
	{
		drawPass(demoState, drawable, mat->passes[i]);
	}
	return 0;
}

a3ret drawPass(a3_DemoState const* demoState, const a3_VertexDrawable* drawable, const a3_RenderPass* pass)
{
	//a3shaderProgramActivate(pass->shaderProgram->program);
	//a3framebufferActivate(pass->writeFBO);
	//a3framebufferBindDepthTexture(demoState->fbo_shadow_d32, a3tex_unit00); //this needs to be configurable because phong uses 06.
	for (a3ui32 i = 0; i < pass->numUniforms; ++i)
	{
		void* source = pass->sourceFunctionFlags ? pass->sourceFunctions[i](demoState) : pass->sources[i];
		//start of loading uniforms, not sure what else I'm missing or if this works.
		switch (pass->uniformFlags[i])
		{
		case uniformSwitch_Int:
			a3shaderUniformSendInt(pass->uniformTypes[i], pass->uniformHandles[i], pass->unifDataCounts[i], (const a3i32*)source);
			break;
		case uniformSwitch_Float:
			a3shaderUniformSendFloat(pass->uniformTypes[i], pass->uniformHandles[i], pass->unifDataCounts[i], (const a3f32*)source);
			break;
		case uniformSwitch_Double:
			a3shaderUniformSendDouble(pass->uniformTypes[i], pass->uniformHandles[i], pass->unifDataCounts[i], (const a3f64*)source);
			break;
		case uniformSwitch_FloatMat: //this only supports floats. How do we support doubles?
			a3shaderUniformSendFloatMat(pass->uniformTypes[i], 0, pass->uniformHandles[i], pass->unifDataCounts[i], (const a3f32*)source);
			break;
		case uniformSwitch_DoubleMat:
			a3shaderUniformSendDoubleMat(pass->uniformTypes[i], 0, pass->uniformHandles[i], pass->unifDataCounts[i], (const a3f64*)source);
			break;
		case uniformSwitch_Buffer:
			//if it's a uniform buffer, the handle is just the index to bind it at. Usually a 0, 1, or 4. I'm not sure how this would be programmatically solved.
			//look up ubTransformStack and ubo_transformStack_model for more information.
			a3shaderUniformBufferActivate((const a3_UniformBuffer*)source, pass->uniformHandles[i]);
			break;
		case uniformSwitch_TextureUnit:
			assert(i != 0); //can't bind to a3tex_unit00 because that's the depth texture!
			//binds a source texture to the correct color unit
			a3framebufferBindColorTexture((const a3_Framebuffer*)source, pass->uniformHandles[i], pass->unifSourceTargets[i]);
			break;
		}
	}
	a3vertexDrawableActivateAndRender(drawable);
	return 0;
}

void* uniform_retrieveActiveCamera(a3_DemoState* demoState)
{
	return (void*)(demoState->projector + demoState->activeCamera);
}
