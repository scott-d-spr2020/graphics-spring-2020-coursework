#ifndef A3_DEMOMATERIALUTILS_H
#define A3_DEMOMATERIALUTILS_H

#include "../a3_DemoState.h"

a3ret initRenderPass(a3_RenderPass* pass, a3ui32 numUniforms, a3_Framebuffer* writeFBO, a3_DemoStateShaderProgram* shaderProgram);
a3ret addRenderUniform(a3_RenderPass* pass, int unifIndex, a3_UniformSwitch unifSwitch, a3i32 unifType, a3i32 unifHandle, a3ui32 count, void* source, a3_UnifFunction sourceFunction, a3boolean functionFlag);

a3ret drawMaterial(a3_DemoState const* demoState, const a3_VertexDrawable* drawable, const a3_RenderMaterial* mat);
a3ret drawPass(a3_DemoState const* demoState, const a3_VertexDrawable* drawable, const a3_RenderPass* pass);

//uniform retrieval functions!

void* uniform_retrieveActiveCamProjMat(a3_DemoState* demoState);
void* uniform_retrieveActiveCamProjMatInv(a3_DemoState* demoState);
void* uniform_retrieveActiveCamProjBiasMat(a3_DemoState* demoState);
void* uniform_retrieveActiveCamProjBiasMatInv(a3_DemoState* demoState);
void* uniform_retrieveTotalRenderTime(a3_DemoState* demoState);
#endif