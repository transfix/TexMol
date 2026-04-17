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
#include <ImposterRenderer/StickRenderer.h>
#include <ImposterRenderer/MeshCylinderRenderer.h>

#ifdef CG
#include <ImposterRenderer/cgGLCylFragmentShader.h>
#include <ImposterRenderer/cgGLCylVertexShader.h>
#include <ImposterRenderer/GlobalCGContext.h>
#endif
#ifdef GLSL
#include <ImposterRenderer/CylFragmentShader.glsl>
#include <ImposterRenderer/CylVertexShader.glsl>
#include <ImposterRenderer/glslShader.h>
#endif


#include <iostream>

using namespace std;

using CCVImposterRenderer::StickRenderer;

bool StickRenderer::m_UseGLFallback = false;

StickRenderer::StickRenderer()
{
	initDefaults();
}

void StickRenderer::initDefaults()
{
	m_Initialized = false;
	m_NumSticks = 0;

	#ifdef CG
	m_VertexProgram = 0;
	m_FragmentProgram = 0;
	#endif
	#ifdef GLSL
	_shader = NULL;
	#endif
}

StickRenderer::~StickRenderer()
{
	#ifdef CG
	if (m_FragmentProgram)
	{
		cgDestroyProgram(m_FragmentProgram);
	}
	if (m_VertexProgram)
	{
		cgDestroyProgram(m_VertexProgram);
	}
	#endif
	#ifdef GLSL
	delete _shader;
	#endif
}

void StickRenderer::clearSticks()
{
	m_TextureCoord1.clearBuffer();
	m_TextureCoord2.clearBuffer();
	m_Color.clearBuffer();
	m_Position.clearBuffer();
	m_NumSticks = 0;
}

int StickRenderer::getNumSticks() const
{
	return m_NumSticks;
}

void StickRenderer::initRenderer(bool drawFunction)
{
	// without extensions we cannot operate at all
	if (!initExtensions())
	{
		m_Initialized = false;
		fprintf(stderr, "StickRender: ERROR: OpenGL extensions unavailable, update your graphics drivers.\n");
		return;
	}
	#ifdef CG
	// if FP30 isn't present we will use a different branch of every render method
	if (!initCG())
	{
		fprintf(stderr, "StickRender: WARNING: FP30/FP40 not found, falling back on GL Rendering.\n");
		m_UseGLFallback = true;
	}
	#endif
	if (!loadProgram())
	{
		fprintf(stderr, "StickRender: load program error, falling back on GL Rendering.\n");
		m_UseGLFallback = true;
	}

	m_Initialized = true;
}

bool StickRenderer::bindProgramAndParamsCg()
{
	#ifdef CG
	cgGLEnableProfile(m_VertexProfile);
	cgGLBindProgram(m_VertexProgram);
	cgGLEnableProfile(m_FragmentProfile);
	cgGLBindProgram(m_FragmentProgram);
	cgGLEnableTextureParameter(m_NormalMapParam);
	cgGLEnableTextureParameter(m_DepthMapParam);
	cgGLSetParameter4f(m_MainColorParam, 0.7f, 0.7f, 0.7f, 1.0f); // gray
	cgGLSetParameter1f(m_SpecularWeightParam, 0.487f);
	#endif
	return true;
}

bool StickRenderer::bindProgramAndParamsGLSL()
{
	#ifdef GLSL
	_shader->bind();

	glEnable( GL_TEXTURE_1D );	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, m_TextureNormalMap->getTextureID());
	glUniform1i(m_NormalMapParam, 0);

	glEnable( GL_TEXTURE_2D );	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_TextureDepthMap->getTextureID());
	glUniform1i(m_DepthMapParam, 1);

	glUniform4f(m_MainColorParam, 0.7, 0.7, 0.7, 1.0); // gray
	glUniform1f(m_SpecularWeightParam, 0.487f);
	#endif
	return true;
}

bool StickRenderer::bindProgramAndParams()
{
	#ifdef CG
	return bindProgramAndParamsCg();
	#endif
	#ifdef GLSL
	return bindProgramAndParamsGLSL();
	#endif
}

bool StickRenderer::unbindProgramAndParamsCg()
{
	#ifdef CG
	cgGLDisableTextureParameter(m_DepthMapParam);
	cgGLDisableTextureParameter(m_NormalMapParam);
	cgGLDisableProfile(m_VertexProfile);
	cgGLDisableProfile(m_FragmentProfile);
	#endif
	return true;
}

bool StickRenderer::unbindProgramAndParamsGLSL()
{
	#ifdef GLSL
	glDisable( GL_TEXTURE_1D );
	glDisable( GL_TEXTURE_2D );

	_shader->unbind();
	#endif
	return true;
}

bool StickRenderer::unbindProgramAndParams()
{
	#ifdef CG
	return unbindProgramAndParamsCg();
	#endif
	#ifdef GLSL
	return unbindProgramAndParamsGLSL();
	#endif
}

void StickRenderer::bindMatrices()
{
	#ifdef CG
	cgGLSetStateMatrixParameter(m_ModelViewITParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE_TRANSPOSE);
	cgGLSetStateMatrixParameter(m_ModelViewProjParam, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(m_ModelViewParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(m_VP_ProjParam, CG_GL_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(m_ModelViewInverseParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE);
	cgGLSetStateMatrixParameter(m_FP_ProjParam, CG_GL_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	#endif
}

void StickRenderer::renderBuffer(bool useHardwareBuffer, bool useFunctionOnSurface, double* rotations, double* translations, int numberOfTransformations, bool randomizeColors)
{
	// MESH RENDERING
	if (m_UseGLFallback)
	{
		// bail if not initialized
		if (!m_Initialized)
		{
			return;
		}
		using namespace std;
		if (m_NumSticks==0)
		{		 
			// we are done
			return;
		}

		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);
		// Set up for Stick rendering
		glDisable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.5);
		glEnable(GL_COLOR_SUM_ARB);
		glSecondaryColor3fEXT(0.,0.,1.);
		// set up multitextures
		// perform transformations if needed and repeat the rendering
		if (rotations && translations && (numberOfTransformations>0))
		{
			int i;
			for (int i=0; i<numberOfTransformations; i++)
			{
				GLfloat trans[16];
				trans[0]  = (GLfloat)rotations[i*9+0];
				trans[1]  = (GLfloat)rotations[i*9+3];
				trans[2]  = (GLfloat)rotations[i*9+6];
				trans[3]  = (GLfloat)0;
				trans[4]  = (GLfloat)rotations[i*9+1];
				trans[5]  = (GLfloat)rotations[i*9+4];
				trans[6]  = (GLfloat)rotations[i*9+7];
				trans[7]  = (GLfloat)0;
				trans[8]  = (GLfloat)rotations[i*9+2];
				trans[9]  = (GLfloat)rotations[i*9+5];
				trans[10] = (GLfloat)rotations[i*9+8];
				trans[11] = (GLfloat)0;
				trans[12]  = (GLfloat)translations[i*3+0];
				trans[13]  = (GLfloat)translations[i*3+1];
				trans[14] = (GLfloat)translations[i*3+2];
				trans[15] = (GLfloat)1;
				// perform the rotation needed
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glMultMatrixf(trans);
				MeshCylinderRenderer::DrawCylinderBuffer(m_Position, m_Color, m_TextureCoord1, m_TextureCoord2);
				glPopMatrix();
			}
		}
		else
		{
			MeshCylinderRenderer::DrawCylinderBuffer(m_Position, m_Color, m_TextureCoord1, m_TextureCoord2);
		}
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_COLOR_SUM_ARB);
		glPopAttrib();
	}
	// CG RENDERING
	else
	{

		// bail if not initialized
		if (!m_Initialized)
		{
			return;
		}
		if (m_NumSticks==0)
		{
			// we are done
			return;
		}
		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);
		bindProgramAndParams();

		// Set up for Stick rendering
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.5);
		glEnable(GL_COLOR_SUM_ARB);
		glSecondaryColor3fEXT(0.,0.,1.);
		// set up multitextures
		glClientActiveTextureARB(GL_TEXTURE0_ARB);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(4, GL_FLOAT, 0, m_TextureCoord1.getBuffer());
		glClientActiveTextureARB(GL_TEXTURE1_ARB);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(4, GL_FLOAT, 0, m_TextureCoord2.getBuffer());
		// draw
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		// perform transformations if needed and repeat the rendering
		if (rotations && translations && (numberOfTransformations>0))
		{
			for (int i=0; i<numberOfTransformations; i++)
			{
				GLfloat trans[16];
				trans[0]  = (GLfloat)rotations[i*9+0];
				trans[1]  = (GLfloat)rotations[i*9+3];
				trans[2]  = (GLfloat)rotations[i*9+6];
				trans[3]  = (GLfloat)0;
				trans[4]  = (GLfloat)rotations[i*9+1];
				trans[5]  = (GLfloat)rotations[i*9+4];
				trans[6]  = (GLfloat)rotations[i*9+7];
				trans[7]  = (GLfloat)0;
				trans[8]  = (GLfloat)rotations[i*9+2];
				trans[9]  = (GLfloat)rotations[i*9+5];
				trans[10] = (GLfloat)rotations[i*9+8];
				trans[11] = (GLfloat)0;
				trans[12]  = (GLfloat)translations[i*3+0];
				trans[13]  = (GLfloat)translations[i*3+1];
				trans[14] = (GLfloat)translations[i*3+2];
				trans[15] = (GLfloat)1;

				// perform the rotation needed
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glMultMatrixf(trans);
				bindMatrices();	
				glVertexPointer(4, GL_FLOAT, 0, m_Position.getBuffer());
				glColorPointer(4, GL_FLOAT, 0, m_Color.getBuffer());
				glDrawArrays(GL_QUADS, 0, m_NumSticks*4);
				glPopMatrix();
			}
		}
		else
		{
			bindMatrices();
			glVertexPointer(4, GL_FLOAT, 0, m_Position.getBuffer());
			glColorPointer(4, GL_FLOAT, 0, m_Color.getBuffer());
			glDrawArrays(GL_QUADS, 0, m_NumSticks*4);
		}
		// disable states
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glClientActiveTextureARB(GL_TEXTURE1_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glClientActiveTextureARB(GL_TEXTURE0_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
		unbindProgramAndParams();
	
		glDisable(GL_BLEND);
		glDisable(GL_COLOR_SUM_ARB);
		
		glPopAttrib();
	}
}

bool StickRenderer::initCG()
{
	#ifdef CG
		#ifdef CGARB
		if (cgGLIsProfileSupported(CG_PROFILE_ARBVP1))
			m_VertexProfile = CG_PROFILE_ARBVP1;
		else
			return false;

		if (cgGLIsProfileSupported(CG_PROFILE_ARBFP1))
			m_FragmentProfile = CG_PROFILE_ARBFP1;
		else
			return false;
		std::cout << "StickRender: Using ARB profile for Cg" << std::endl;
		#else
		if (cgGLIsProfileSupported(CG_PROFILE_VP40))
			m_VertexProfile = CG_PROFILE_VP40;
		else  if (cgGLIsProfileSupported(CG_PROFILE_VP20))
			m_VertexProfile = CG_PROFILE_VP20;
		else
			return false;
	
		if (cgGLIsProfileSupported(CG_PROFILE_FP40))
			m_FragmentProfile = CG_PROFILE_FP40;
		else if (cgGLIsProfileSupported(CG_PROFILE_FP30))
			m_FragmentProfile = CG_PROFILE_FP30;
		else
			return false;
		#endif
	#endif
	return true;
}

bool StickRenderer::loadProgram()
{
	#ifdef CG
	return loadProgramCg();
	#endif
	#ifdef GLSL
	return loadProgramGLSL();
	#endif
}

bool StickRenderer::loadProgramCg()
{
	#ifdef CG
  	CGcontext context = GlobalCGContext::getCGContext();
	// Create Vertex Program for Cylinder
	m_VertexProgram = cgCreateProgram(context, CG_SOURCE,cgGLCylVertexShader,
									  m_VertexProfile, NULL, NULL);
	// Load Vertex Program for Cylinder
	cgGLLoadProgram(m_VertexProgram);
	m_ModelViewITParam = cgGetNamedParameter(m_VertexProgram, "ModelViewIT");
	m_ModelViewInverseParam = cgGetNamedParameter(m_VertexProgram, "ModelViewInverse");
	m_ModelViewProjParam = cgGetNamedParameter(m_VertexProgram, "ModelViewProj");
	m_ModelViewParam = cgGetNamedParameter(m_VertexProgram, "ModelView");
	m_VP_ProjParam = cgGetNamedParameter(m_VertexProgram, "Proj");
	if (!m_ModelViewITParam || !m_ModelViewInverseParam || !m_ModelViewProjParam || !m_ModelViewParam || !m_VP_ProjParam)
	{
		return false;
	}
	std::cout << "StickRender: VP load" << std::endl;
	// Create Fragment Program
	m_FragmentProgram = cgCreateProgram(context, CG_SOURCE,cgGLCylFragmentShader,
										m_FragmentProfile, NULL, NULL);
	// Load Fragment Program
	cgGLLoadProgram(m_FragmentProgram);
	m_MainColorParam      = cgGetNamedParameter(m_FragmentProgram, "maincolor");
	m_NormalMapParam      = cgGetNamedParameter(m_FragmentProgram, "normalmap");
	m_DepthMapParam       = cgGetNamedParameter(m_FragmentProgram, "depthmap");
	m_SpecularWeightParam = cgGetNamedParameter(m_FragmentProgram, "specweight");
	m_FP_ProjParam        = cgGetNamedParameter(m_FragmentProgram, "PT");
	m_TextureNormalMap = new Texture();
	m_TextureNormalMap->calculateCylinderNormalMap(512);
	m_TextureDepthMap = new Texture();
	m_TextureDepthMap->calculateCylinderDepthMap(512);
	cgGLSetTextureParameter(m_NormalMapParam, m_TextureNormalMap->getTextureID());
	cgGLSetTextureParameter(m_DepthMapParam, m_TextureDepthMap->getTextureID());
	std::cout << "StickRender: FP load" << std::endl;
	#endif
	return true;
}

bool StickRenderer::loadProgramGLSL()
{
	#ifdef GLSL
	// create shader object
        _shader        = new glslShader();
	_shader->initFromString( glslCylVertexShader, glslCylFragmentShader );

	m_MainColorParam       = glGetUniformLocation(_shader->id(), "maincolor");
	m_NormalMapParam       = glGetUniformLocation(_shader->id(), "normalmap");
	m_DepthMapParam        = glGetUniformLocation(_shader->id(), "depthmap");
	m_SpecularWeightParam  = glGetUniformLocation(_shader->id(), "specweight");

	m_TextureNormalMap = new Texture();
	m_TextureNormalMap->calculateCylinderNormalMap(512);
	m_TextureDepthMap = new Texture();
	m_TextureDepthMap->calculateCylinderDepthMap(512);
	#endif
	return true;
}

bool StickRenderer::initExtensions()
{
	return true;
}
