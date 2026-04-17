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
#ifdef CG
	#ifdef AO
	#include <ImposterRenderer/cgGLFragmentShaderAO.h>
	#else
	#include <ImposterRenderer/cgGLFragmentShader.h>
	#endif
#include <ImposterRenderer/cgGLVertexShader.h>
#include <ImposterRenderer/GlobalCGContext.h>
#endif
#ifdef GLSL
#include <ImposterRenderer/glslShader.h>
	#ifdef AO
	#include <ImposterRenderer/FragmentShaderAO.glsl>
	#else
	#include <ImposterRenderer/FragmentShader.glsl>
	#endif
#include <ImposterRenderer/VertexShader.glsl>
#endif

#include <ImposterRenderer/PlainSphereRenderer.h>
#include <ImposterRenderer/Texture.h>
#include <stdio.h>
#include <iostream>

using namespace std;

using CCVImposterRenderer::PlainSphereRenderer;

PlainSphereRenderer::PlainSphereRenderer()
{
	#ifdef CG
	m_VertexProgram = 0;
	m_FragmentProgram = 0;
	#endif
	#ifdef GLSL
	_shader = NULL;
	#endif
}

PlainSphereRenderer::~PlainSphereRenderer()
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

bool PlainSphereRenderer::bindProgramAndParamsCg()
{
	#ifdef CG
	// Set up for ball rendering
	cgGLEnableProfile(m_VertexProfile);
	cgGLBindProgram(m_VertexProgram);
	cgGLEnableProfile(m_FragmentProfile);
	cgGLBindProgram(m_FragmentProgram);
	cgGLEnableTextureParameter(m_LightMapParam);
	cgGLEnableTextureParameter(m_DepthMapParam);
	cgGLEnableTextureParameter(m_NormalMapParam);
	cgGLEnableTextureParameter(m_AOMapParam);
	cgGLSetParameter4f(m_MainColorParam, 1.0, 0.0, 0.0, 1.0); // red
	cgGLSetParameter1f(m_AmbientParam, 0.05); // red
	cgGLSetParameter2f(m_AODeltaParam, AOdelta, 1.f/AOdelta);
	cgGLSetParameter3f(m_BBoxMinParam, AOvolMin[0], AOvolMin[1], AOvolMin[2]);
	cgGLSetParameter3f(m_AOResolParam, 1.f/(float)AOvolResol[0], 1.f/(float)AOvolResol[1], 1.f/(float)AOvolResol[2]);
	#endif
	return true;
}

bool PlainSphereRenderer::bindProgramAndParamsGLSL()
{
	#ifdef GLSL
	_shader->bind();

	glEnable( GL_TEXTURE_2D );	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureLightMap->getTextureID());
	glUniform1i(m_LightMapParam, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_TextureDepthMap->getTextureID());
	glUniform1i(m_DepthMapParam, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_TextureNormalMap->getTextureID());
	glUniform1i(m_NormalMapParam, 2);

	glEnable( GL_TEXTURE_3D );	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_3D, AOtexID());
	glUniform1i(m_AOMapParam, 3);

	glUniform4f(m_MainColorParam, 1.0, 0.0, 0.0, 1.0); // red
	glUniform1f(m_AmbientParam, 0.05);
	glUniform2f(m_AODeltaParam, AOdelta, 1.f/AOdelta);
	glUniform3f(m_BBoxMinParam, AOvolMin[0], AOvolMin[1], AOvolMin[2]);
	glUniform3f(m_AOResolParam, 1.f/(float)AOvolResol[0], 1.f/(float)AOvolResol[1], 1.f/(float)AOvolResol[2]);
	#endif
	return true;
}

bool PlainSphereRenderer::bindProgramAndParams()
{
	#ifdef CG
	bindProgramAndParamsCg();
	#endif
	#ifdef GLSL
	bindProgramAndParamsGLSL();
	#endif
	return true;
}

void PlainSphereRenderer::bindMatrices()
{
	#ifdef CG
	cgGLSetStateMatrixParameter(m_ModelViewParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(m_ModelViewProjParam, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(m_VP_ProjParam, CG_GL_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(m_ModelViewInverseParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE);
	cgGLSetStateMatrixParameter(m_ModelViewTParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_TRANSPOSE);
	cgGLSetStateMatrixParameter(m_FP_ProjParam, CG_GL_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	#endif
}

bool PlainSphereRenderer::unbindProgramAndParamsCg()
{
	#ifdef CG
	cgGLDisableTextureParameter(m_LightMapParam);
	cgGLDisableTextureParameter(m_DepthMapParam);
	cgGLDisableTextureParameter(m_NormalMapParam);
	cgGLDisableTextureParameter(m_AOMapParam);
	cgGLDisableProfile(m_VertexProfile);
	cgGLDisableProfile(m_FragmentProfile);
	#endif
	return true;
}

bool PlainSphereRenderer::unbindProgramAndParamsGLSL()
{
	#ifdef GLSL
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_TEXTURE_3D );

	_shader->unbind();
	#endif
	return true;
}

bool PlainSphereRenderer::unbindProgramAndParams()
{
	#ifdef CG
	unbindProgramAndParamsCg();
	#endif
	#ifdef GLSL
	unbindProgramAndParamsGLSL();
	#endif
	return true;
}

bool PlainSphereRenderer::initCG()
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
		std::cout << "PlainSphereRender: Using ARB profile for Cg" << std::endl;
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

bool PlainSphereRenderer::loadProgram()
{
	#ifdef CG
	return loadProgramCg();
	#endif
	#ifdef GLSL
	return loadProgramGLSL();
	#endif
}

bool PlainSphereRenderer::loadProgramCg()
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
	CGcontext context = GlobalCGContext::getCGContext();
	// Create Vertex Program for Spheres
	m_VertexProgram = cgCreateProgram(context, CG_SOURCE,cgGLVertexShader, m_VertexProfile, 0, 0);
	// Load Vertex Program
	cgGLLoadProgram(m_VertexProgram);
	printf("PlainSphereRender: VP loaded\n");
	// get parameter handles
	m_PositionParam = cgGetNamedParameter(m_VertexProgram, "IN.center");
	m_ColorParam = cgGetNamedParameter(m_VertexProgram, "IN.color");
	m_ModelViewParam = cgGetNamedParameter(m_VertexProgram, "ModelView");
	m_ModelViewInverseParam = cgGetNamedParameter(m_VertexProgram, "ModelViewInverse");
	m_ModelViewProjParam = cgGetNamedParameter(m_VertexProgram, "ModelViewProj");
	m_VP_ProjParam = cgGetNamedParameter(m_VertexProgram, "Proj");
	if (!m_ModelViewParam || !m_ModelViewProjParam || !m_VP_ProjParam || !m_ColorParam || !m_PositionParam)
	{
		return false;
	}
	// Create Fragment Program
	m_FragmentProgram = cgCreateProgram(context, CG_SOURCE,cgGLFragmentShader, m_FragmentProfile, 0, 0);
	// Load Fragment Program
	cgGLLoadProgram(m_FragmentProgram);
	printf("PlainSphereRender: FP loaded\n");
	// get parameter handles
	m_MainColorParam  = cgGetNamedParameter(m_FragmentProgram, "maincolor");
	m_AmbientParam   = cgGetNamedParameter(m_FragmentProgram, "ambient");
	m_LightMapParam   = cgGetNamedParameter(m_FragmentProgram, "lightmap");
	m_DepthMapParam   = cgGetNamedParameter(m_FragmentProgram, "depthmap");
	m_NormalMapParam  = cgGetNamedParameter(m_FragmentProgram, "normalmap");
	m_AOMapParam      = cgGetNamedParameter(m_FragmentProgram, "AOmap");
	m_AODeltaParam    = cgGetNamedParameter(m_FragmentProgram, "AOdelta");
	m_BBoxMinParam    = cgGetNamedParameter(m_FragmentProgram, "bboxmin");
	m_AOResolParam    = cgGetNamedParameter(m_FragmentProgram, "rAOresol");
	m_ModelViewTParam = cgGetNamedParameter(m_FragmentProgram, "MVT");
	m_FP_ProjParam    = cgGetNamedParameter(m_FragmentProgram, "PT");
	// prepare the textures
	m_TextureLightMap = new Texture();
	m_TextureLightMap->calculateSphereOpacityAndLightMap(512);
	m_TextureDepthMap = new Texture();
	m_TextureDepthMap->calculateDepthMap(512,512);
	m_TextureNormalMap = new Texture();
	m_TextureNormalMap->calculateNormalMap(512,512);
	cgGLSetTextureParameter(m_LightMapParam, m_TextureLightMap->getTextureID());
	cgGLSetTextureParameter(m_DepthMapParam, m_TextureDepthMap->getTextureID());
	cgGLSetTextureParameter(m_NormalMapParam, m_TextureNormalMap->getTextureID());
	cgGLSetTextureParameter(m_AOMapParam, AOtexID() );
	#endif
	return true;
}

bool PlainSphereRenderer::loadProgramGLSL()
{
	#ifdef GLSL
	// create shader object
        _shader        = new glslShader();
	_shader->initFromString( glslVertexShader, glslFragmentShader );

	m_MainColorParam  = glGetUniformLocation(_shader->id(), "maincolor");
	m_AmbientParam   = glGetUniformLocation(_shader->id(), "ambient");
	m_LightMapParam   = glGetUniformLocation(_shader->id(), "lightmap");
	m_DepthMapParam   = glGetUniformLocation(_shader->id(), "depthmap");
	m_NormalMapParam  = glGetUniformLocation(_shader->id(), "normalmap");
	m_AOMapParam      = glGetUniformLocation(_shader->id(), "AOmap");
	m_AODeltaParam    = glGetUniformLocation(_shader->id(), "AOdelta");
	m_BBoxMinParam    = glGetUniformLocation(_shader->id(), "bboxmin");
	m_AOResolParam    = glGetUniformLocation(_shader->id(), "rAOresol");

	// prepare the textures
	m_TextureLightMap = new Texture();
	m_TextureLightMap->calculateSphereOpacityAndLightMap(512);
	m_TextureDepthMap = new Texture();
	m_TextureDepthMap->calculateDepthMap(512,512);
	m_TextureNormalMap = new Texture();
	m_TextureNormalMap->calculateNormalMap(512,512);
	#endif
	return true;
}

void PlainSphereRenderer::setAOparams(float delta, float min[3], int gresol[3])
{
	AOdelta = delta;
	AOvolMin[0] = min[0];
	AOvolMin[1] = min[1];
	AOvolMin[2] = min[2];
	AOvolResol[0] = gresol[0];
	AOvolResol[1] = gresol[1];
	AOvolResol[2] = gresol[2];
}
