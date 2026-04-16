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
// BumpMapRenderer.cpp: implementation of the BumpMapRenderer class.

#include <GL/glew.h>
#include <GeometryRenderer/BumpMapping.h>
#include <GeometryRenderer/BumpMapRenderer.h>
#include <stdio.h>

using CCVTexMolGeometryRenderer::BumpMapRenderer;

extern void cgErrorCallback(void);

BumpMapRenderer::BumpMapRenderer()
{
	m_Initialized = false;
}

BumpMapRenderer::~BumpMapRenderer()
{
	#ifdef CG
	cgDestroyProgram(m_FragmentProgram);
	cgDestroyContext(m_Context);
	#endif
}

bool BumpMapRenderer::initialize()
{
	#ifdef CG
	cgSetErrorCallback(cgErrorCallback);
	m_Context = cgCreateContext();
	m_FragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(m_FragmentProfile);
	m_FragmentProgram = cgCreateProgram(m_Context, CG_SOURCE, cgGLBumpMappingFragmentShader,
										m_FragmentProfile, 0, 0);
	cgGLLoadProgram(m_FragmentProgram);
	cgGLBindProgram(m_FragmentProgram);
	cgGLEnableProfile(m_FragmentProfile);
	#endif

	// Extension availability is handled by GLEW (initialized in RenderAreaWidgetBase)
	m_Initialized = true;
	return true;
}

bool BumpMapRenderer::render(Geometry* geometry, float* surfaceColors)
{
	if (!m_Initialized || !geometry)
	{
		return false;
	}
	/*	glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT);
	    glDisable(GL_LIGHTING);

	    // The first texture unit contains the detail texture
	    glActiveTextureARB(GL_TEXTURE0_ARB);
	    glEnable(GL_TEXTURE_2D);
	    glBindTexture(GL_TEXTURE_2D, [detail texture handle]);

	    // The second texture unit contains the normalmap texture
	    glActiveTextureARB(GL_TEXTURE1_ARB);
	    glEnable(GL_TEXTURE_2D);
	    glBindTexture(GL_TEXTURE_2D, [normalmap texture handle]);

	    // Set the (fixed) ambient color value
	    CGparameter ambientColorParameter = cgGetNamedParameter(program, "ambientColor");
	    cgGLSetParameter3f(ambientColorParameter, [ambientr], [ambientg], [ambientb]);

	    for every vertex in the triangle {
	        // Bind the light vector to COLOR0 and interpolate
	        // it across the edge
	        glColor3f([lightx], [lighty], [lightz]);

	        // Bind the texture coordinates to TEXTURE0 and
	        // interpolate them across the edge
	        glMultiTexCoord2fARB(GL_TEXTURE0_ARB,
	            [texturex], [texturey]);

	        // Bind the normalmap coordinates to TEXTURE1 and
	        // interpolate them across the edge
	        glMultiTexCoord2fARB(GL_TEXTURE1_ARB,
	            [texturex], [texturey]);

	        // Specify the vertex coordinates
	        glVertex3fv([vertexx], [vertexy], [vertexz]);
	     }
		glPopAttrib();
	*/
	return false;
}
