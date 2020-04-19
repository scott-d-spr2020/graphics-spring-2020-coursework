#ifndef A3_DEMOMATERIALUTILS_H
#define A3_DEMOMATERIALUTILS_H

#include "../a3_DemoState.h"

a3ret drawMaterial(a3_DemoState const* demoState, const a3_VertexDrawable* drawable, const a3_RenderMaterial* mat);
a3ret drawPass(a3_DemoState const* demoState, const a3_VertexDrawable* drawable, const a3_RenderPass* pass);

//uniform retrieval functions!

void* uniform_retrieveActiveCamera(a3_DemoState* demoState);
#endif