#ifndef A3_DEMOMATERIALUTILS_H
#define A3_DEMOMATERIALUTILS_H

#include "../a3_DemoState.h"

a3ret drawMaterial(const a3_VertexDrawable* drawable, const a3_RenderMaterial* mat);
a3ret drawPass(const a3_VertexDrawable* drawable, const a3_RenderPass* pass);
#endif