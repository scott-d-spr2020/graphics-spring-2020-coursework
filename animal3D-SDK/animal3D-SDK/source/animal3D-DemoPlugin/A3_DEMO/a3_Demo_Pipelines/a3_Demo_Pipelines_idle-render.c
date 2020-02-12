/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein

	a3_Demopipelines_idle-render.c
	Demo mode implementations: shading rendering.

	********************************************
	*** RENDERING CODE FOR SHADING DEMO MODE ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_Demo_Pipelines.h"

#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoRenderUtils.h"


// OpenGL
#ifdef _WIN32
#include <gl/glew.h>
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32


//-----------------------------------------------------------------------------

// controls for pipelines mode
void a3pipelines_render_controls(a3_DemoState const* demoState, a3_Demo_Pipelines const* demoMode,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	// display mode info
	a3byte const* pipelineText[pipelines_pipeline_max] = {
		"Forward rendering",
	};

	// forward pipeline names
	a3byte const* renderProgramName[pipelines_render_max] = {
		"Phong shading",
		"Phong shading with shadow mapping",
		"Nonphoto Shading",
		"Nonphoto Shading with shadow mapping"
	};

	// forward display names
	a3byte const* displayProgramName[pipelines_display_max] = {
		"Texture",
		"Texture with outlines",
	};

	// active camera name
	a3byte const* cameraText[pipelines_camera_max] = {
		"Scene camera",
		"Shadow map light",
	};

	// pass names
	a3byte const* passName[pipelines_pass_max] = {
		"Pass: Capture shadow map",
		"Pass: Render scene objects",
		"Pass: Composite",
	};
	a3byte const* targetText_shadow[pipelines_target_shadow_max] = {
		"Depth buffer",
	};
	a3byte const* targetText_scene[pipelines_target_scene_max] = {
		"Color target 0: FINAL DISPLAY COLOR",
		"Color target 1: Attrib data: view position",
		"Color target 2: Attrib data: view normal",
		"Color target 3: Attrib data: atlas texcoord",
		"Color target 4: Shadow coord or diffuse map",
		"Color target 5: Shadow test or specular map",
		"Color target 6: Lighting: diffuse total",
		"Color target 7: Lighting: specular total",
		"Depth buffer",
	};
	a3byte const* targetText_composite[pipelines_target_composite_max] = {
		"Color target 0: FINAL DISPLAY COLOR",
	};
	a3byte const* const* targetText[pipelines_pass_max] = {
		targetText_shadow,
		targetText_scene,
		targetText_composite,
	};
	a3ui32 const target[pipelines_pass_max] = {
		demoMode->target_shadow,
		demoMode->target_scene,
		demoMode->target_composite,
	};
	a3ui32 const targetMax[pipelines_pass_max] = {
		pipelines_target_shadow_max,
		pipelines_target_scene_max,
		pipelines_target_composite_max,
	};

	// text color
	a3vec4 const col = { a3real_half, a3real_zero, a3real_half, a3real_one };

	// pipeline and target
	a3_Demo_Pipelines_PipelineName const pipeline = demoMode->pipeline;
	a3_Demo_Pipelines_RenderProgramName const render = demoMode->render;
	a3_Demo_Pipelines_DisplayProgramName const display = demoMode->display;
	a3_Demo_Pipelines_ActiveCameraName const activeCamera = demoMode->activeCamera;
	a3_Demo_Pipelines_PassName const pass = demoMode->pass;
	a3ui32 const targetIndex = target[pass];
	a3ui32 const targetCount = targetMax[pass];

	// demo modes
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Pipeline (%u / %u) ('[' | ']'): %s", pipeline + 1, pipelines_pipeline_max, pipelineText[pipeline]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Display pass (%u / %u) ('(' | ')'): %s", pass + 1, pipelines_pass_max, passName[pass]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"        Target (%u / %u) ('{' | '}'): %s", targetIndex + 1, targetCount, targetText[pass][targetIndex]);

	// lighting modes
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Rendering mode (%u / %u) ('j' | 'k'): %s", render + 1, pipelines_render_max, renderProgramName[render]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Display mode (%u / %u) ('J' | 'K'): %s", display + 1, pipelines_display_max, displayProgramName[display]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Active camera (%u / %u) ('c' prev | next 'v'): %s", activeCamera + 1, pipelines_camera_max, cameraText[activeCamera]);
}


//-----------------------------------------------------------------------------

// ****TO-DO: 
//	-> 2.1: LOOK HERE - read this whole function please
//	-> 3.1: LOOK HERE - go over it again
//	-> 4.1: LOOK HERE - yes, one more time
// sub-routine for rendering the demo state using the shading pipeline
void a3pipelines_render(a3_DemoState const* demoState, a3_Demo_Pipelines const* demoMode)
{
	// pointers
	const a3_VertexDrawable* currentDrawable;
	const a3_DemoStateShaderProgram* currentDemoProgram;
	const a3_DemoPointLight* pointLight;

	// framebuffers
	const a3_Framebuffer* currentReadFBO, * currentWriteFBO;

	// indices
	a3ui32 i, j, k;

	// RGB
	const a3vec4 rgba4[] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f },	// red
		{ 0.0f, 1.0f, 0.0f, 1.0f },	// green
		{ 0.0f, 0.0f, 1.0f, 1.0f },	// blue
		{ 0.0f, 1.0f, 1.0f, 1.0f },	// cyan
		{ 1.0f, 0.0f, 1.0f, 1.0f },	// magenta
		{ 1.0f, 1.0f, 0.0f, 1.0f },	// yellow
		{ 1.0f, 0.5f, 0.0f, 1.0f },	// orange
		{ 0.0f, 0.5f, 1.0f, 1.0f },	// sky blue
		{ 0.5f, 0.5f, 0.5f, 1.0f },	// solid grey
		{ 0.5f, 0.5f, 0.5f, 0.5f },	// translucent grey
	};
	const a3real
		* const red = rgba4[0].v, * const green = rgba4[1].v, * const blue = rgba4[2].v,
		* const cyan = rgba4[3].v, * const magenta = rgba4[4].v, * const yellow = rgba4[5].v,
		* const orange = rgba4[6].v, * const skyblue = rgba4[7].v,
		* const grey = rgba4[8].v, * const grey_t = rgba4[9].v;

	// camera used for drawing
	const a3_DemoProjector* activeCamera = demoState->projector + demoState->activeCamera;
	const a3_DemoSceneObject* activeCameraObject = activeCamera->sceneObject;

	// shadow caster
	const a3_DemoProjector* activeShadowCaster = demoState->shadowLight;

	// current scene object being rendered, for convenience
	const a3_DemoSceneObject* currentSceneObject, * endSceneObject;

	// temp drawable pointers
	const a3_VertexDrawable* drawable[] = {
		demoState->draw_plane,
		demoState->draw_sphere,
		demoState->draw_cylinder,
		demoState->draw_torus,
		demoState->draw_teapot,
	};

	// temp texture pointers
	const a3_Texture* texture_dm[] = {
		demoState->tex_stone_dm,
		demoState->tex_earth_dm,
		demoState->tex_stone_dm,
		demoState->tex_mars_dm,
		demoState->tex_checker,
	};
	const a3_Texture* texture_sm[] = {
		demoState->tex_stone_dm,
		demoState->tex_earth_sm,
		demoState->tex_stone_dm,
		demoState->tex_mars_sm,
		demoState->tex_checker,
	};

	// forward pipeline shader programs
	const a3_DemoStateShaderProgram* renderProgram[pipelines_pipeline_max][pipelines_render_max] = {
		{
			demoState->prog_drawPhong_multi_mrt,
			demoState->prog_drawPhong_multi_shadow_mrt,
			demoState->prog_drawNonphoto_multi_mrt,
			demoState->prog_drawNP_multi_shadow_mrt
		},
	};

	// display shader programs
	const a3_DemoStateShaderProgram* displayProgram[pipelines_pipeline_max][pipelines_display_max] = {
		{
			demoState->prog_drawTexture,
			demoState->prog_drawTexture_outline,
		},
	};

	// framebuffers from which to read based on pipeline mode
	const a3_Framebuffer* readFBO[pipelines_pipeline_max][pipelines_pass_max] = {
		{
			demoState->fbo_shadow_d32,
			demoState->fbo_scene_c16d24s8_mrt,
			demoState->fbo_composite_c16,
		},
	};

	// tmp lighting data
	a3f32 lightSz[demoStateMaxCount_lightObject];
	a3f32 lightSzInvSq[demoStateMaxCount_lightObject];
	a3vec4 lightPos[demoStateMaxCount_lightObject];
	a3vec4 lightCol[demoStateMaxCount_lightObject];


	// pixel size and effect axis
	a3vec2 pixelSize;
	a3vec2 pixelAxis;


	// pipeline
	a3_Demo_Pipelines_PipelineName const pipeline = demoMode->pipeline;
	a3_Demo_Pipelines_RenderProgramName const render = demoMode->render;
	a3_Demo_Pipelines_DisplayProgramName const display = demoMode->display;
	a3_Demo_Pipelines_PassName pass;


	// bias matrix
	const a3mat4 bias = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
	};

	// final model matrix and full matrix stack
	a3mat4 viewMat = activeCameraObject->modelMatInv;
	a3mat4 viewProjectionMat = activeCamera->viewProjectionMat;
	a3mat4 modelViewProjectionMat = viewProjectionMat;
	a3mat4 modelViewMat = a3mat4_identity, modelMat = a3mat4_identity;
	a3mat4 modelViewProjectionBiasMat_other, viewProjectionBiasMat_other = activeShadowCaster->viewProjectionMat;


	// init
	a3real4x4ConcatR(bias.m, viewProjectionBiasMat_other.m);
	modelViewProjectionBiasMat_other = viewProjectionBiasMat_other;


	//-------------------------------------------------------------------------
	// 0) PRE-SCENE PASS: shadow pass renders scene to depth-only
	//	- activate shadow pass framebuffer
	//	- draw scene
	//		- clear depth buffer
	//		- render shapes using appropriate shaders
	//		- capture depth

	pass = pipelines_passShadow;

	// select shadow FBO
	currentWriteFBO = demoState->fbo_shadow_d32;
	a3framebufferActivate(currentWriteFBO);

	// clear
	glClear(GL_DEPTH_BUFFER_BIT);

	// draw objects inverted
	glCullFace(GL_FRONT);
	currentDemoProgram = demoState->prog_transform;
	a3shaderProgramActivate(currentDemoProgram->program);
	for (k = 0,
		currentSceneObject = demoState->planeObject, endSceneObject = demoState->teapotObject;
		currentSceneObject <= endSceneObject;
		++k, ++currentSceneObject)
		a3demo_drawModelSimple_activateModel(modelViewProjectionMat.m, activeShadowCaster->viewProjectionMat.m, currentSceneObject->modelMat.m, currentDemoProgram, drawable[k]);
	glCullFace(GL_BACK);


	//-------------------------------------------------------------------------
	// 1) SCENE PASS: render scene with desired shader
	//	- activate scene framebuffer
	//	- draw scene
	//		- clear buffers
	//		- render shapes using appropriate shaders
	//		- capture color and depth

	pass = pipelines_passScene;

	// select target framebuffer
	switch (pipeline)
	{
		// shading with MRT
	case pipelines_forward:
		// target scene framebuffer
		currentWriteFBO = demoState->fbo_scene_c16d24s8_mrt;
		a3framebufferActivate(currentWriteFBO);

		// clear now, handle skybox later
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;
	}


	// optional stencil test before drawing objects
	a3real4x4SetScale(modelMat.m, a3real_four);
	if (demoState->stencilTest)
		a3demo_drawStencilTest(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, demoState->prog_drawColorUnif, demoState->draw_sphere);


	// copy temp light data
	for (k = 0, pointLight = demoState->forwardPointLight;
		k < demoState->forwardLightCount;
		++k, ++pointLight)
	{
		lightSz[k] = pointLight->radius;
		lightSzInvSq[k] = pointLight->radiusInvSq;
		lightPos[k] = pointLight->viewPos;
		lightCol[k] = pointLight->color;
	}


	// support multiple geometry passes
	for (i = 0, j = 1; i < j; ++i)
	{
		// select forward algorithm
		switch (i)
		{
			// forward pass
		case 0: {
			// select program based on settings
			currentDemoProgram = renderProgram[pipeline][render];
			a3shaderProgramActivate(currentDemoProgram->program);

			// send shared data: 
			//	- projection matrix
			//	- light data
			//	- activate shared textures including atlases if using
			//	- shared animation data
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP, 1, activeCamera->projectionMat.mm);
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP_inv, 1, activeCamera->projectionMatInv.mm);
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
			a3shaderUniformSendDouble(a3unif_single, currentDemoProgram->uTime, 1, &demoState->renderTimer->totalTime);
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, skyblue);
			a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uLightCt, 1, &demoState->forwardLightCount);
			a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uLightSz, demoState->forwardLightCount, lightSz);
			a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uLightSzInvSq, demoState->forwardLightCount, lightSzInvSq);
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uLightPos, demoState->forwardLightCount, lightPos->v);
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uLightCol, demoState->forwardLightCount, lightCol->v);
			a3textureActivate(demoState->tex_ramp_dm, a3tex_unit04);
			a3textureActivate(demoState->tex_ramp_sm, a3tex_unit05);

			// activate shadow map and other relevant textures
			currentReadFBO = demoState->fbo_shadow_d32;
			a3framebufferBindDepthTexture(currentReadFBO, a3tex_unit06);
			a3textureActivate(demoState->tex_earth_dm, a3tex_unit07);

			// individual object requirements: 
			//	- modelviewprojection
			//	- modelview
			//	- modelview for normals
			//	- per-object animation data
			for (k = 0,
				currentSceneObject = demoState->planeObject, endSceneObject = demoState->teapotObject;
				currentSceneObject <= endSceneObject;
				++k, ++currentSceneObject)
			{
				a3textureActivate(texture_dm[k], a3tex_unit00);
				a3textureActivate(texture_sm[k], a3tex_unit01);
				a3demo_drawModelLighting_bias_other(modelViewProjectionBiasMat_other.m, modelViewProjectionMat.m, modelViewMat.m, viewProjectionBiasMat_other.m, viewProjectionMat.m, viewMat.m, currentSceneObject->modelMat.m, currentDemoProgram, drawable[k], rgba4[k + 3].v);
			}
		}	break;
			// end geometry pass
		}
	}


	// stop using stencil
	if (demoState->stencilTest)
		glDisable(GL_STENCIL_TEST);


	// draw grid aligned to world
	if (demoState->displayGrid)
		a3demo_drawModelSolidColor(modelViewProjectionMat.m, viewProjectionMat.m, demoState->gridTransform.m, demoState->prog_drawColorUnif, demoState->draw_grid, demoState->gridColor.v);


	//-------------------------------------------------------------------------
	// COMPOSITE PASS
	//	- activate composite framebuffer
	//	- composite scene layers

	pass = pipelines_passComposite;

	currentWriteFBO = demoState->fbo_composite_c16;
	a3framebufferActivate(currentWriteFBO);

	// composite skybox
	if (demoState->displaySkybox)
	{
		a3demo_drawModelTextured_invertModel(modelViewProjectionMat.m, viewProjectionMat.m, demoState->skyboxObject->modelMat.m, a3mat4_identity.m, demoState->prog_drawTexture, demoState->draw_skybox, demoState->tex_skybox_clouds);
		a3demo_enableCompositeBlending();
	}

	// use simple texturing program
	currentDemoProgram = demoState->prog_drawTexture;
	a3shaderProgramActivate(currentDemoProgram->program);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, a3mat4_identity.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);

	// draw textured quad with previous pass image on it
	// repeat as necessary to complete composite
	currentDrawable = demoState->draw_unitquad;
	a3vertexDrawableActivate(currentDrawable);

	currentReadFBO = demoState->fbo_scene_c16d24s8_mrt;
	a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, 0);
	a3vertexDrawableRenderActive();


	//-------------------------------------------------------------------------
	// PREPARE FOR POST-PROCESSING
	//	- double buffer swap (if applicable)
	//	- ensure blending is disabled
	//	- re-activate FSQ drawable IF NEEDED (i.e. changed in previous step)
	glDisable(GL_BLEND);
	currentDrawable = demoState->draw_unitquad;
	a3vertexDrawableActivate(currentDrawable);


	//-------------------------------------------------------------------------
	// DISPLAY: final pass, perform and present final composite
	//	- finally draw to back buffer
	//	- select display texture(s)
	//	- activate final pass program
	//	- draw final FSQ

	// revert to back buffer and disable depth testing
	a3framebufferDeactivateSetViewport(a3fbo_depthDisable,
		-demoState->frameBorder, -demoState->frameBorder, demoState->frameWidth, demoState->frameHeight);

	// select framebuffer to display based on mode
	currentReadFBO = readFBO[pipeline][demoMode->pass];

	// select output to display
	switch (demoMode->pass)
	{
	case pipelines_passShadow:
			a3framebufferBindDepthTexture(currentReadFBO, a3tex_unit00);
		break;
	case pipelines_passScene:
		if (currentReadFBO->color && (!currentReadFBO->depthStencil || demoMode->target_scene < pipelines_scene_fragdepth))
			a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, demoMode->target_scene);
		else
			a3framebufferBindDepthTexture(currentReadFBO, a3tex_unit00);
		break;
	case pipelines_passComposite:
		a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, demoMode->target_composite);
		break;
	}


	// final display: activate desired final program and draw FSQ
	if (currentReadFBO)
	{
		// prepare for final draw
		currentDrawable = demoState->draw_unitquad;
		a3vertexDrawableActivate(currentDrawable);

		// determine if additional passes are required
		currentDemoProgram = displayProgram[pipeline][display];
		a3shaderProgramActivate(currentDemoProgram->program);

		switch (demoMode->display)
		{
			// most basic option: simply display texture
		case pipelines_displayTexture:
			break;

			// display with outlines
			// need to activate more textures and send params (e.g. color, pixel size/axis)
		case pipelines_displayOutline:
			currentReadFBO = demoState->fbo_scene_c16d24s8_mrt;
			a3real2Set(pixelAxis.v, a3real_two, a3real_two);
			a3real2Set(pixelSize.v, a3recip((a3real)currentReadFBO->frameWidth), a3recip((a3real)currentReadFBO->frameHeight));
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, a3vec4_w.v);	// use as line color
			a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uAxis, 1, pixelAxis.v);	// use as line thickness
			a3shaderUniformSendFloat(a3unif_vec2, currentDemoProgram->uSize, 1, pixelSize.v);	// use as actual pixel size
			a3framebufferBindDepthTexture(currentReadFBO, a3tex_unit01);
			a3framebufferBindColorTexture(currentReadFBO, a3tex_unit02, pipelines_scene_normal);
			break;
		}

		// done
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, a3mat4_identity.mm);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
		a3shaderUniformSendDouble(a3unif_single, currentDemoProgram->uTime, 1, &demoState->renderTimer->totalTime);
		a3vertexDrawableRenderActive();
	}


	//-------------------------------------------------------------------------
	// OVERLAYS: done after FSQ so they appear over everything else
	//	- disable depth testing
	//	- draw overlays appropriately

	// hidden volumes
	if (demoState->displayHiddenVolumes && demoMode->pass != pipelines_passShadow)
	{
		glCullFace(GL_FRONT);

		// draw light volumes
		currentDemoProgram = demoState->prog_drawColorUnif;
		a3shaderProgramActivate(currentDemoProgram->program);
		a3vertexDrawableActivate(demoState->draw_pointlight);
		for (k = 0; k < demoState->forwardLightCount; ++k)
		{
			a3real4x4SetScale(modelMat.m, (a3real)0.0075 * demoState->forwardPointLight[k].radius);
			modelMat.v3 = demoState->forwardPointLight[k].worldPos;
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, demoState->forwardPointLight[k].color.v);
			a3demo_drawModelSimple(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, currentDemoProgram);
		}

		a3vertexDrawableActivate(demoState->draw_skybox);
		a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, orange);
		for (k = 0; k < demoStateMaxCount_cameraObject; ++k)
		{
			a3real4x4SetScale(modelMat.m, (a3real)0.0075);
			a3real4x4ConcatR(demoState->cameraObject[k].modelMat.m, modelMat.m);
			a3demo_drawModelSimple(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, currentDemoProgram);
		}

		glCullFace(GL_BACK);
	}


	// superimpose axes
	// draw coordinate axes in front of everything
	currentDemoProgram = demoState->prog_drawColorAttrib;
	a3shaderProgramActivate(currentDemoProgram->program);
	a3vertexDrawableActivate(demoState->draw_axes);

	// center of world from current viewer
	// also draw other viewer/viewer-like object in scene
	if (demoState->displayWorldAxes && demoMode->pass != pipelines_passShadow)
	{
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, viewProjectionMat.mm);
		a3vertexDrawableRenderActive();
	}

	// individual objects
	if (demoState->displayObjectAxes && demoMode->pass != pipelines_passShadow)
	{
		a3_DemoSceneObject const* axesObjects[] = {
			demoState->planeObject, demoState->sphereObject, demoState->cylinderObject, demoState->torusObject, demoState->teapotObject,
			demoState->mainLightObject, demoState->mainCameraObject,
			0
		}, ** axesObjectsItr = axesObjects;
		for (currentSceneObject = *axesObjectsItr, 
			endSceneObject = demoState->displayHiddenVolumes ? 0 : demoState->mainLightObject;
			currentSceneObject != endSceneObject;
			currentSceneObject = *(++axesObjectsItr))
			a3demo_drawModelSimple(modelViewProjectionMat.m, viewProjectionMat.m, currentSceneObject->modelMat.m, currentDemoProgram);
	}


	// pipeline
	if (demoState->displayPipeline)
	{

	}
}


//-----------------------------------------------------------------------------
