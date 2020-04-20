#ifndef A3_DEMOMATERIALUTILS_H
#define A3_DEMOMATERIALUTILS_H

#include "../a3_DemoState.h"

a3ret drawMaterial(a3_DemoState const* demoState, const a3_VertexDrawable* drawable, const a3_RenderMaterial* mat);
a3ret drawPass(a3_DemoState const* demoState, const a3_VertexDrawable* drawable, const a3_RenderPass* pass);

//uniform retrieval functions!

void* uniform_retrieveActiveCamProjMat(a3_DemoState* demoState);
void* uniform_retrieveActiveCamProjMatInv(a3_DemoState* demoState);
void* uniform_retrieveActiveCamProjBiasMat(a3_DemoState* demoState);
void* uniform_retrieveActiveCamProjBiasMatInv(a3_DemoState* demoState);
void* uniform_retrieveTotalRenderTime(a3_DemoState* demoState);
#endif