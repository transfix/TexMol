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
#include <ImposterRenderer/FunctionSphereRenderer.h>

#ifdef CG
#include <ImposterRenderer/cgGLFragmentFunctionShader.h>
#include <ImposterRenderer/cgGLVertexFunctionShader.h>
#include <ImposterRenderer/GlobalCGContext.h>
#endif
#ifdef GLSL
#include <ImposterRenderer/FragmentFunctionShader.glsl>
#include <ImposterRenderer/VertexFunctionShader.glsl>
#include <ImposterRenderer/glslShader.h>
#endif

#include <ImposterRenderer/Texture.h>
#include <iostream>

using CCVImposterRenderer::FunctionSphereRenderer;

FunctionSphereRenderer::FunctionSphereRenderer()
{
	#ifdef CG
	m_VertexProgram = 0;
	m_FragmentProgram = 0;
	#endif
	#ifdef GLSL
	_shader = 0;
	#endif
	m_Data = 0;
	m_minx = 0.0;
	m_miny = 0.0;
	m_minz = 0.0;
	m_maxx = 1.0;
	m_maxy = 1.0;
	m_maxz = 1.0;
	m_Width = 10;
	m_Height = 10;
	m_Depth = 10;
}

FunctionSphereRenderer::~FunctionSphereRenderer()
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
	_shader = 0;
	#endif
	delete [] m_Data;
	m_Data = 0;
}

void FunctionSphereRenderer::bindProgramAndParamsCg()
{
	#ifdef CG
	// Set up for ball rendering
	cgGLEnableProfile(m_VertexProfile);
	cgGLBindProgram(m_VertexProgram);
	cgGLEnableProfile(m_FragmentProfile);
	cgGLBindProgram(m_FragmentProgram);
	cgGLEnableTextureParameter(m_LightMapParam);
	cgGLEnableTextureParameter(m_DepthMapParam);
	cgGLEnableTextureParameter(m_FunctionMapParam);
	cgGLSetParameter4f(m_MainColorParam, 1.0, 0.0, 0.0, 1.0); // red
	cgGLSetParameter3f(m_MinExtentParam, (float)m_minx, (float)m_miny, (float)m_minz);
	cgGLSetParameter3f(m_MaxExtentParam, (float)m_maxx, (float)m_maxy, (float)m_maxz);
	#endif
}

void FunctionSphereRenderer::bindProgramAndParamsGLSL()
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

	glEnable( GL_TEXTURE_3D );	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, m_TextureFunctionMap->getTextureID());
	glUniform1i(m_FunctionMapParam, 2);

	glUniform4f(m_MainColorParam, 1.0, 0.0, 0.0, 1.0); // red
	glUniform3f(m_MinExtentParam, (float)m_minx, (float)m_miny, (float)m_minz);
	glUniform3f(m_MaxExtentParam, (float)m_maxx, (float)m_maxy, (float)m_maxz);
	#endif	
}

void FunctionSphereRenderer::bindProgramAndParams()
{
	#ifdef CG
	bindProgramAndParamsCg();
	#endif
	#ifdef GLSL
	bindProgramAndParamsGLSL();
	#endif
}

void FunctionSphereRenderer::bindMatrices()
{
	#ifdef CG
	cgGLSetStateMatrixParameter(m_ModelViewParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(m_ModelViewProjParam, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(m_VP_ProjParam, CG_GL_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(m_ModelViewInverseParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE);
	cgGLSetStateMatrixParameter(m_ModelViewProjInverseParam, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_INVERSE);
	cgGLSetStateMatrixParameter(m_FP_ProjParam, CG_GL_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	#endif
}

void FunctionSphereRenderer::unbindProgramAndParamsCg()
{
	#ifdef CG
	cgGLDisableTextureParameter(m_FunctionMapParam);
	cgGLDisableTextureParameter(m_LightMapParam);
	cgGLDisableTextureParameter(m_DepthMapParam);
	cgGLDisableProfile(m_VertexProfile);
	cgGLDisableProfile(m_FragmentProfile);
	#endif
}

void FunctionSphereRenderer::unbindProgramAndParamsGLSL()
{
	#ifdef GLSL
	glDisable(GL_TEXTURE_2D);

	_shader->unbind();
	#endif	
}

void FunctionSphereRenderer::unbindProgramAndParams()
{
	#ifdef CG
	unbindProgramAndParamsCg();
	#endif
	#ifdef GLSL
	unbindProgramAndParamsGLSL();
	#endif
}

bool FunctionSphereRenderer::initCG()
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
                std::cout << "FunctionSphereRender: Using ARB profile for Cg" << std::endl;
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

bool FunctionSphereRenderer::loadProgramCg()
{
	#ifdef CG
	if (!m_Data)
	{
		return false;    // this shouldnt crash the rendering, ensure that
	}
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
	m_VertexProgram = cgCreateProgram(context, CG_SOURCE,cgGLVertexFunctionShader, m_VertexProfile, 0, 0);
	// Load Vertex Program
	cgGLLoadProgram(m_VertexProgram);
	// get parameter handles
	m_PositionParam         = cgGetNamedParameter(m_VertexProgram, "IN.center");
	m_ColorParam            = cgGetNamedParameter(m_VertexProgram, "IN.color");
	m_ModelViewParam        = cgGetNamedParameter(m_VertexProgram, "ModelView");
	m_ModelViewInverseParam = cgGetNamedParameter(m_VertexProgram, "ModelViewInverse");
	m_ModelViewProjParam    = cgGetNamedParameter(m_VertexProgram, "ModelViewProj");
	m_VP_ProjParam          = cgGetNamedParameter(m_VertexProgram, "Proj");
	if (!m_ModelViewParam || !m_ModelViewProjParam || !m_VP_ProjParam || !m_ColorParam || !m_PositionParam)
	{
		std::cout << "FunctionSphere: parameter handle fail" << std::endl;
		return false;
	}
	std::cout << "FunctionSphere: VP load" << std::endl;

	// Create Fragment Program
	m_FragmentProgram = cgCreateProgram(context, CG_SOURCE,cgGLFragmentFunctionShader, m_FragmentProfile, 0, 0);
	// Load Fragment Program
	cgGLLoadProgram(m_FragmentProgram);
	// get parameter handles
	m_MainColorParam            = cgGetNamedParameter(m_FragmentProgram, "maincolor");
	m_LightMapParam             = cgGetNamedParameter(m_FragmentProgram, "lightmap");
	m_DepthMapParam             = cgGetNamedParameter(m_FragmentProgram, "depthmap");
	m_FunctionMapParam          = cgGetNamedParameter(m_FragmentProgram, "functionmap");
	m_ModelViewProjInverseParam = cgGetNamedParameter(m_FragmentProgram, "ModelViewProjInverse");
	m_MinExtentParam            = cgGetNamedParameter(m_FragmentProgram, "minExtent");
	m_MaxExtentParam            = cgGetNamedParameter(m_FragmentProgram, "maxExtent");
	m_FP_ProjParam              = cgGetNamedParameter(m_FragmentProgram, "PT");
	// prepare the textures
	m_TextureLightMap = new Texture();
	m_TextureLightMap->calculateSphereOpacityAndLightMap(512);
	m_TextureDepthMap = new Texture();
	m_TextureDepthMap->calculateDepthMap(512,512);
	m_TextureFunctionMap = new Texture();
	if (!m_TextureFunctionMap->loadFunction(m_Data, m_Width, m_Height, m_Depth))
	{
		return false;
	}
	cgGLSetTextureParameter(m_LightMapParam, m_TextureLightMap->getTextureID());
	cgGLSetTextureParameter(m_DepthMapParam, m_TextureDepthMap->getTextureID());
	cgGLSetTextureParameter(m_FunctionMapParam, m_TextureFunctionMap->getTextureID());
	std::cout << "FunctionSphere: FP load" << std::endl;
	#endif
	return true;
}

bool FunctionSphereRenderer::loadProgramGLSL()
{
	#ifdef GLSL
	// create shader object
        _shader        = new glslShader();
	_shader->initFromString( glslVertexFunctionShader, glslFragmentFunctionShader );

	m_MainColorParam     = glGetUniformLocation(_shader->id(), "maincolor");
	m_LightMapParam      = glGetUniformLocation(_shader->id(), "lightmap");
	m_DepthMapParam      = glGetUniformLocation(_shader->id(), "depthmap");
	m_FunctionMapParam   = glGetUniformLocation(_shader->id(), "functionmap");
	m_MinExtentParam     = glGetUniformLocation(_shader->id(), "minExtent");
	m_MaxExtentParam     = glGetUniformLocation(_shader->id(), "maxExtent");

	// prepare the textures
	m_TextureLightMap = new Texture();
	m_TextureLightMap->calculateSphereOpacityAndLightMap(512);
	m_TextureDepthMap = new Texture();
	m_TextureDepthMap->calculateDepthMap(512,512);
	m_TextureFunctionMap = new Texture();
	if (!m_TextureFunctionMap->loadFunction(m_Data, m_Width, m_Height, m_Depth))
	{
		return false;
	}
	#endif
}

bool FunctionSphereRenderer::loadProgram()
{
	#ifdef CG
	return loadProgramCg();
	#endif
	#ifdef GLSL
	return loadProgramGLSL();
	#endif
}

bool FunctionSphereRenderer::addFunction(unsigned char* data, double minx, double miny, double minz, double maxx, double maxy, double maxz, int width, int height, int depth)
{
	delete [] m_Data; // how to delete from CG ?
	m_Data = data;
	m_minx = minx;
	m_miny = miny;
	m_minz = minz;
	m_maxx = maxx;
	m_maxy = maxy;
	m_maxz = maxz;
	m_Width = width;
	m_Height = height;
	m_Depth = depth;
	return true;
}

// loads cubemap without making CG calls
bool FunctionSphereRenderer::fallbackLoadProgram()
{
	m_TextureFunctionMap = new Texture();
	return m_TextureFunctionMap->loadFunction(m_Data, m_Width, m_Height, m_Depth);
}
