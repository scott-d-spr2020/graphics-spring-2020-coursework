#include "..\a3_DemoMaterialUtils.h"
#include <assert.h>

a3ret initRenderPass(a3_RenderPass* pass, a3ui32 uniformCount, a3_Framebuffer* writeFBO, a3_DemoStateShaderProgram* shaderProgram)
{
	pass->numUniforms = uniformCount;
	pass->uniformHandles = calloc(uniformCount, sizeof(a3i32));
	pass->uniformTypes = calloc(uniformCount, sizeof(a3i32));
	pass->uniformFlags = calloc(uniformCount, sizeof(a3_UniformSwitch));
	pass->sources = calloc(uniformCount, sizeof(void*));
	pass->sourceFunctions = calloc(uniformCount, sizeof(a3_UnifFunction));
	pass->sourceFunctionFlags = calloc(uniformCount, sizeof(a3boolean));
	pass->unifDataCounts = calloc(uniformCount, sizeof(a3ui32));
	pass->unifSourceTargets = calloc(uniformCount, sizeof(a3ui32));
	pass->writeFBO = writeFBO;
	pass->shaderProgram = shaderProgram;
	return 0;
}

a3ret addRenderUniform(a3_RenderPass* pass, int unifIndex, a3_UniformSwitch unifSwitch, a3i32 unifType, a3i32 unifHandle, a3ui32 count, void* source, a3_UnifFunction sourceFunction, a3boolean functionFlag)
{
	pass->uniformFlags[unifIndex] = unifSwitch;
	pass->uniformTypes[unifIndex] = unifType;
	pass->uniformTypes[unifIndex] = unifType;
	pass->uniformHandles[unifIndex] = unifHandle;
	pass->unifDataCounts[unifIndex] = count;
	pass->sources[unifIndex] = source;
	pass->sourceFunctions[unifIndex] = sourceFunction;
	pass->sourceFunctionFlags[unifIndex] = functionFlag;
	return 0;
}

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
	a3framebufferActivate(pass->writeFBO);
	//a3framebufferBindDepthTexture(demoState->fbo_shadow_d32, a3tex_unit00); //this needs to be configurable because phong uses 06.
	for (a3ui32 i = 0; i < pass->numUniforms; ++i)
	{
		if ((pass->sources[i] != NULL && pass->sources[i] != (void*)0xcdcdcdcdcdcdcdcd)
			|| (pass->sourceFunctions[i] != NULL && pass->sourceFunctions[i] != (a3_UnifFunction)0xcdcdcdcdcdcdcdcd)) //uninitialized memory
		{
			void* source = pass->sourceFunctionFlags[i] ? pass->sourceFunctions[i](demoState) : pass->sources[i];
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
			case uniformSwitch_UniformBuffer:
				//if it's a uniform buffer, the handle is just the index to bind it at. Usually a 0, 1, or 4. I'm not sure how this would be programmatically solved.
				//look up ubTransformStack and ubo_transformStack_model for more information.
				a3shaderUniformBufferActivate((const a3_UniformBuffer*)source, pass->uniformHandles[i]);
				break;
			case uniformSwitch_TextureUnit:
				a3textureActivate((const a3_Texture*)source, pass->uniformHandles[i]);
				break;
			case uniformSwitch_ColorBuffer:
				//binds a source texture to the correct color unit
				a3framebufferBindColorTexture((const a3_Framebuffer*)source, pass->uniformHandles[i], pass->unifSourceTargets[i]);
				break;
			case uniformSwitch_DepthBuffer:
				a3framebufferBindDepthTexture((const a3_Framebuffer*)source, pass->uniformHandles[i]);
				break;
			}
		}
	}
	a3vertexDrawableActivateAndRender(drawable);
	return 0;
}

//----------------------
//Uniform Retrieval
//

a3_DemoProjector* getActiveCamera(a3_DemoState* demoState)
{
	return (demoState->projector + demoState->activeCamera);
}

void* uniform_retrieveActiveCamProjMat(a3_DemoState* demoState)
{
	return (void*)getActiveCamera(demoState)->projectionMat.mm;
}

void* uniform_retrieveActiveCamProjMatInv(a3_DemoState* demoState)
{
	return (void*)getActiveCamera(demoState)->projectionMatInv.mm;
}

void* uniform_retrieveActiveCamProjBiasMat(a3_DemoState* demoState)
{
	const a3mat4 bias = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
	};

	a3_DemoProjector* activeCamera = getActiveCamera(demoState);
	a3mat4 viewProjectionBiasMat_other = demoState->shadowLight->viewProjectionMat;
	a3mat4* projectionBiasMatPointer = malloc(sizeof(a3mat4));
	*projectionBiasMatPointer = activeCamera->projectionMat;

	a3real4x4ConcatR(bias.m, viewProjectionBiasMat_other.m);

	a3real4x4Product(projectionBiasMatPointer->m, bias.m, activeCamera->projectionMat.m);
	return (void*)projectionBiasMatPointer->mm;
}

void* uniform_retrieveActiveCamProjBiasMatInv(a3_DemoState* demoState)
{
	const a3mat4 unbias = {
		 2.0f,  0.0f,  0.0f, 0.0f,
		 0.0f,  2.0f,  0.0f, 0.0f,
		 0.0f,  0.0f,  2.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f,
	};
	a3_DemoProjector* activeCamera = getActiveCamera(demoState);
	a3mat4 projectionBiasMat_inv = activeCamera->projectionMatInv;

	a3real4x4Product(projectionBiasMat_inv.m, activeCamera->projectionMatInv.m, unbias.m);
	return (void*)activeCamera->projectionMatInv.mm;
}

void* uniform_retrieveTotalRenderTime(a3_DemoState* demoState)
{
	return (void*)&demoState->renderTimer->totalTime;
}

void* uniform_retrieveLightCount(a3_DemoState* demoState)
{
	return (void*)&demoState->forwardLightCount;
}

/**
 Returns -1 if the pass doesn't have enough space for the uniforms.
*/
a3ret registerCommonUniforms(a3_DemoState* demoState, a3_RenderPass* pass)
{
	if (pass->numUniforms < 16)
	{
		return -1;
	}
	//uP
	addRenderUniform(pass, 0, uniformSwitch_FloatMat, a3unif_mat4, pass->shaderProgram->uP, 1, NULL, uniform_retrieveActiveCamProjMat, 1);
	//uP_inv
	addRenderUniform(pass, 1, uniformSwitch_FloatMat, a3unif_mat4, pass->shaderProgram->uP_inv, 1, NULL, uniform_retrieveActiveCamProjMatInv, 1);
	//uPB (needs to get projectionBiasMat, which is calculated at 407 in idle render using active camera)
	addRenderUniform(pass, 2, uniformSwitch_FloatMat, a3unif_mat4, pass->shaderProgram->uPB, 1, NULL, uniform_retrieveActiveCamProjBiasMat, 1);
	//uPB_inv (needs to get projectionBiasMatInv, which is calculated at 408 in idle render using active camera)
	addRenderUniform(pass, 3, uniformSwitch_FloatMat, a3unif_mat4, pass->shaderProgram->uPB_inv, 1, NULL, uniform_retrieveActiveCamProjBiasMatInv, 1);
	//uAtlas
	addRenderUniform(pass, 4, uniformSwitch_FloatMat, a3unif_mat4, pass->shaderProgram->uAtlas, 1, (void*)a3mat4_identity.mm, NULL, 0);
	//uTime
	addRenderUniform(pass, 5, uniformSwitch_Double, a3unif_single, pass->shaderProgram->uTime, 1, NULL, uniform_retrieveTotalRenderTime, 1);

	//uColor
	a3vec4* skyBlueArr = malloc(sizeof(a3vec4));
	skyBlueArr->r = 0.0f;
	skyBlueArr->g = 0.5f;
	skyBlueArr->b = 1.0f;
	skyBlueArr->a = 1.0f;
	addRenderUniform(pass, 6, uniformSwitch_Float, a3unif_vec4, pass->shaderProgram->uColor, 1, skyBlueArr->v, NULL, 0);
	//uImage04 - Ramp DM
	addRenderUniform(pass, 7, uniformSwitch_TextureUnit, -1, a3tex_unit04, 1, demoState->tex_ramp_dm, NULL, 0);
	//uImage05 - Ramp SM
	addRenderUniform(pass, 8, uniformSwitch_TextureUnit, -1, a3tex_unit05, 1, demoState->tex_ramp_sm, NULL, 0);
	//uImage06 - Depth Buffer
	addRenderUniform(pass, 9, uniformSwitch_DepthBuffer, -1, a3tex_unit06, 1, demoState->fbo_shadow_d32, NULL, 0);
	//uImage07 - Earth		 
	addRenderUniform(pass, 10, uniformSwitch_TextureUnit, -1, a3tex_unit07, 1, demoState->tex_earth_dm, NULL, 0);
	//uLightCt
	addRenderUniform(pass, 11, uniformSwitch_Int, a3unif_single, pass->shaderProgram->uLightCt, 1, &demoState->forwardLightCount, NULL, 0);

	return 0;
}
