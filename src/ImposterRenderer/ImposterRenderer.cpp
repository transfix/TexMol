/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of TexMol.

  TexMol is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  TexMol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <ImposterRenderer/ImposterRenderer.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>

using CCVImposterRenderer::ImposterRenderer;
using CCVImposterRenderer::BallRenderer;
using CCVImposterRenderer::GaussianRenderer;
using CCVImposterRenderer::StickRenderer;
using CCVImposterRenderer::HelixRenderer;
using CCVImposterRenderer::HollowCylinderRenderer;

ImposterRenderer::ImposterRenderer()
{
	m_BallRenderer = new BallRenderer();
	m_GaussianRenderer = new GaussianRenderer();
	m_StickRenderer = new StickRenderer();
	m_HelixRenderer = new HelixRenderer();
	m_HollowCylinderRenderer = new HollowCylinderRenderer();
	initialized = false;
	#ifdef CG
	m_ShaderContext = 0;
	#endif
}

ImposterRenderer::~ImposterRenderer()
{
	#ifdef CG		
	if (m_ShaderContext)
	{
		cgDestroyContext(m_ShaderContext);
	}
	#endif
	delete m_HollowCylinderRenderer;
	m_HollowCylinderRenderer = 0;
	delete m_HelixRenderer;
	m_HelixRenderer = 0;
	delete m_StickRenderer;
	m_StickRenderer = 0;
	delete m_GaussianRenderer;
	m_GaussianRenderer = 0;
	delete m_BallRenderer;
	m_BallRenderer = 0;
}

void cgErrorCallback(void)
{
	#ifdef CG
	CGerror LastError = cgGetError();
	#endif
}

void ImposterRenderer::initSubRenderers(bool drawFunction)
{
	m_StickRenderer->initRenderer(drawFunction);
	m_HelixRenderer->initRenderer(drawFunction);
	m_HollowCylinderRenderer->initRenderer(drawFunction);
	m_BallRenderer->initRenderer(drawFunction);
	m_GaussianRenderer->initRenderer(drawFunction);
}

bool ImposterRenderer::initRenderer()
{
	glewInit();

	// dont need to return false, change SKVINAY
	#ifdef CG
	cgSetErrorCallback(cgErrorCallback);
	#endif
	if(!glewIsSupported("GL_VERSION_1_2") ||
	   !glewIsSupported("GL_EXT_secondary_color") ||
	   !glewIsSupported("GL_ARB_multitexture") ||
	   !glewIsSupported("GL_ARB_vertex_buffer_object"))
	{
	   fprintf( stderr, "ImposterRenderer: init extensions fail!\n");
	   return false;
	}
       
	#ifdef GLSL
	if(!glewIsSupported("GL_VERSION_2_0") ||
	   !glewIsSupported("GL_ARB_vertex_shader") ||
	   !glewIsSupported("GL_ARB_fragment_shader") ||
	   !glewIsSupported("GL_ARB_shader_objects"))
	{
		printf("ImposterRenderer: init extensions fail for GLSL\n");
		return false;
	}
	else
		printf("ImposterRenderer: Using GLSL!\n");

	#endif
 	
	initialized = true;
	return true;
}

void ImposterRenderer::renderBuffer(bool useHardwareBuffer, bool useFunctionOnSurface, double* rotations, double* translations, int numberOfTransformations, bool randomizeColors, float avgradius)
{
	if( !initialized ) return;

	m_BallRenderer->renderBuffer(useHardwareBuffer, useFunctionOnSurface, rotations, translations, numberOfTransformations, randomizeColors, avgradius);
	m_StickRenderer->renderBuffer(useHardwareBuffer, useFunctionOnSurface, rotations, translations, numberOfTransformations, randomizeColors);
	m_HelixRenderer->renderBuffer(useHardwareBuffer, useFunctionOnSurface, rotations, translations, numberOfTransformations, randomizeColors);
	m_HollowCylinderRenderer->renderBuffer(useHardwareBuffer, useFunctionOnSurface, rotations, translations, numberOfTransformations, randomizeColors);
	m_GaussianRenderer->renderBuffer(rotations, translations, numberOfTransformations);
}

bool ImposterRenderer::addFunction(unsigned char* data, double minx, double miny, double minz, double maxx, double maxy, double maxz, int width, int height, int depth)
{
	return m_BallRenderer->addFunction(data, minx, miny, minz, maxx, maxy, maxz, width, height, depth);
}

void ImposterRenderer::clear()
{
	m_BallRenderer->clearBalls();
	m_GaussianRenderer->clearGaussians();
	m_StickRenderer->clearSticks();
	m_HelixRenderer->clearHelics();
	m_HollowCylinderRenderer->clear();
}

void ImposterRenderer::forceMeshRendering(bool status)
{
	BallRenderer::forceMeshRendering(status);
	GaussianRenderer::forceMeshRendering(status);
	StickRenderer::forceMeshRendering(status);
	HelixRenderer::forceMeshRendering(status);
	HollowCylinderRenderer::forceMeshRendering(status);
}
