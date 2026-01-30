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
#include <ImposterRenderer/cgGLGaussFragmentShader.h>
#include <ImposterRenderer/cgGLGaussVertexShader.h>
#include <ImposterRenderer/GlobalCGContext.h>
#endif
#include <ImposterRenderer/GaussianRenderer.h>
#include <ImposterRenderer/MeshSphereRenderer.h>
#include <ImposterRenderer/Texture.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>

using CCVImposterRenderer::GaussianRenderer;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static const int BatchSize = 10000;
static const int TRI_STATE_WORKS = 0;
static const int TRI_STATE_DOESNT_WORK= 1;
static const int TRI_STATE_UNKNOWN = 2;
bool GaussianRenderer::m_UseGLFallback = false;

GaussianRenderer::GaussianRenderer()
{
	m_Initialized = false;
	m_NumGaussians = 0;
	m_Dirty = true;
	m_NumBufferObjectsAllocated = 0;
	m_BufferObjects = 0;
	m_CGRequirements = TRI_STATE_UNKNOWN;
	m_OpenGLRequirements = TRI_STATE_UNKNOWN;
	m_ProgramsLoaded = TRI_STATE_UNKNOWN;
	#ifdef CG
	m_VertexProgram = 0;
	m_FragmentProgram = 0;
	#endif

	m_approxSort = false;
	_tcoordSort = NULL;
	_posSort = NULL;
	_colSort = NULL;
	_sortBufSize = NULL;

        m_Blobbyness = -2.3;
}

GaussianRenderer::~GaussianRenderer()
{
	deallocateBufferObjects();
}

void GaussianRenderer::initRenderer(bool drawFunction)
{
	// without extensions we cannot operate at all
	if (!initExtensions())
	{
		m_Initialized = false;
		fprintf(stderr, "Gaussian: ERROR: OpenGL extensions unavailable, update your graphics drivers.\n");
		return;
	}
	// if FP30 isn't present we will use a different branch of every render method
	if (!initCG())
	{
		fprintf(stderr, "Gaussian: WARNING: FP30/FP40 not found, falling back on GL Rendering.\n");
		m_CGRequirements = TRI_STATE_DOESNT_WORK;
		m_UseGLFallback = true;
	}
	if (!loadProgram())
	{
		fprintf(stderr, "Gaussian: load program error, falling back on GL Rendering.\n");
		m_CGRequirements = TRI_STATE_DOESNT_WORK;
		m_UseGLFallback = true;
	}

	m_CGRequirements = TRI_STATE_WORKS;
	// prepare framebuffer object
	m_Initialized = true;
}

bool GaussianRenderer::initCG()
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
		std::cout << "GaussianRender: Using ARB profile for Cg" << std::endl;
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

bool GaussianRenderer::loadProgram()
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
	m_VertexProgram = cgCreateProgram(context, CG_SOURCE,cgGLGaussVertexShader, m_VertexProfile, 0, 0);
	// Load Vertex Program
	cgGLLoadProgram(m_VertexProgram);
	// get parameter handles
	m_PositionParam = cgGetNamedParameter(m_VertexProgram, "IN.center");
	m_ColorParam = cgGetNamedParameter(m_VertexProgram, "IN.color");
	m_ModelViewITParam = cgGetNamedParameter(m_VertexProgram, "ModelViewIT");
	m_ModelViewInverseParam = cgGetNamedParameter(m_VertexProgram, "ModelViewInverse");
	m_ModelViewProjParam = cgGetNamedParameter(m_VertexProgram, "ModelViewProj");
	m_ProjParam = cgGetNamedParameter(m_VertexProgram, "Proj");
	if (!m_ModelViewITParam || !m_ModelViewProjParam || !m_ProjParam || !m_ColorParam || !m_PositionParam)
	{
		return false;
	}
	// Create Fragment Program
	m_FragmentProgram = cgCreateProgram(context, CG_SOURCE,cgGLGaussFragmentShader, m_FragmentProfile, 0, 0);
	// Load Fragment Program
	cgGLLoadProgram(m_FragmentProgram);
	// get parameter handles
	m_MainColorParam = cgGetNamedParameter(m_FragmentProgram, "maincolor");
	m_LightMapParam = cgGetNamedParameter(m_FragmentProgram, "blurmap");
	// prepare the textures
	m_TextureLightMap = new Texture();
	m_TextureLightMap->calculateElectronDensityBlurMap(512, Blobbyness(), 1.0);
	cgGLSetTextureParameter(m_LightMapParam, m_TextureLightMap->getTextureID());
	#endif
	return true;
}

bool GaussianRenderer::initExtensions()
{
	if (!glewIsSupported("GL_VERSION_2_0") ) {
		m_OpenGLRequirements = TRI_STATE_DOESNT_WORK;
		fprintf( stderr, "GL version is two low\n");
		return false;
	}
	if (!glewIsSupported("GL_EXT_secondary_color") ||
	    !glewIsSupported("GL_ARB_multitexture") ||
	    !glewIsSupported("GL_ARB_vertex_buffer_object") ||
	    !glewIsSupported("GL_EXT_framebuffer_object"))
	{
		m_OpenGLRequirements = TRI_STATE_DOESNT_WORK;
		return false;
	}
	m_OpenGLRequirements = TRI_STATE_WORKS;
	return true;
}

void GaussianRenderer::deallocateBufferObjects()
{
	if (m_OpenGLRequirements == TRI_STATE_UNKNOWN)
	{
		if (!initExtensions())
		{
			return;
		}
	}
	if (m_OpenGLRequirements == TRI_STATE_DOESNT_WORK)
	{
		return;
	}
	// OK, buffers are present on this card.
	if (m_BufferObjects)
	{
		glDeleteBuffersARB(m_NumBufferObjectsAllocated, m_BufferObjects);
	}
	m_NumBufferObjectsAllocated = 0;
	delete [] m_BufferObjects;
	m_BufferObjects = 0;
}

void GaussianRenderer::clearGaussians()
{
	m_TextureCoord.clearBuffer();
	m_Color.clearBuffer();
	m_Position.clearBuffer();
	m_NumGaussians = 0;

	if( _tcoordSort ) { for(int i = 0; i < nSlices(); i++ ) _tcoordSort[i].clearBuffer(); delete[] _tcoordSort; }
	if( _posSort ) { for(int i = 0; i < nSlices(); i++ ) _posSort[i].clearBuffer(); delete[] _posSort; }
	if( _colSort ) { for(int i = 0; i < nSlices(); i++ ) _colSort[i].clearBuffer(); delete[] _colSort; }
	if( _sortBufSize ) delete[] _sortBufSize;
}

int GaussianRenderer::getNumGaussians() const
{
	return m_NumGaussians;
}

void GaussianRenderer::prepareBuffers()
{
	if (m_OpenGLRequirements == TRI_STATE_UNKNOWN)
	{
		if (!initExtensions())
		{
			return;
		}
	}
	if (m_OpenGLRequirements == TRI_STATE_DOESNT_WORK)
	{
		return;
	}
	// OK, buffers are present on this card.
	unsigned int remaining = m_NumGaussians;
	unsigned int batch = BatchSize;
	unsigned int offset = 0;
	unsigned int currentBuffer = 0;
	allocateBufferObjects((m_NumGaussians/BatchSize+1)*2);
	float *positions = NULL;
	float *colors = NULL;
	if( m_approxSort )
	{
		positions = new float[ m_NumGaussians * 4 * 4 ];
		colors = new float[ m_NumGaussians * 4 * 4 ];
		int offset = 0;
		for(int i = 0; i < _nslices; i++ ) {
			if( _sortBufSize[i] > 0 ) {
				int bufsize = _sortBufSize[i]*4*4;
				memcpy( positions + offset, _posSort[i].getBuffer(), bufsize*sizeof(float) );
				memcpy( colors + offset, _colSort[i].getBuffer(), bufsize*sizeof(float) );
				offset += bufsize;
			}
		}
	}
	else
	{
		positions = m_Position.getBuffer();
		colors = m_Color.getBuffer();
	}

	while (remaining>0)
	{
		unsigned int thisRender = (remaining<batch?remaining:batch);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_BufferObjects[currentBuffer]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, thisRender*4*4*sizeof(float), positions+offset, GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_BufferObjects[currentBuffer+1]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, thisRender*4*4*sizeof(float), colors+offset, GL_STATIC_DRAW_ARB);
		// decrement remaining
		remaining-=thisRender;
		// increment offset
		offset+=thisRender*4*4;
		currentBuffer+=2;
	}
	m_Dirty = false;
}

void GaussianRenderer::renderOnce()
{
	if (m_UseGLFallback)
	{
		MeshSphereRenderer::DrawSphereBuffer(m_Position, m_Color);
	}
	else
	{
		// try rendering in batches
		unsigned int remaining = m_NumGaussians;
		unsigned int batch = BatchSize;
		unsigned int offset = 0;
		unsigned int currentBuffer = 0;
		while (remaining>0)
		{
			unsigned int thisRender = (remaining<batch?remaining:batch);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_BufferObjects[currentBuffer]);
			glVertexPointer(4, GL_FLOAT, 0, BUFFER_OFFSET(0));
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_BufferObjects[currentBuffer+1]);
			glColorPointer(4, GL_FLOAT, 0, BUFFER_OFFSET(0));
			glDrawArrays(GL_QUADS, 0, thisRender*4);
			// decrement remaining
			remaining-=thisRender;
			// increment offset
			offset+=thisRender*4*4*sizeof(GLfloat);
			currentBuffer+=2;
		}
	}
}

void GaussianRenderer::renderBuffer(double* rotations, double* translations, int numberOfTransformations)
{
	if (m_UseGLFallback)
	{
		if (!m_Initialized)
		{
			return;
		}
		if (m_NumGaussians==0)
		{
			return;
		}
		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_COLOR_SUM_ARB);
		bindMatrices();
		renderOnce();
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_COLOR_SUM_ARB);
		glPopAttrib();
	}
	else
	{
		if (m_OpenGLRequirements == TRI_STATE_UNKNOWN)
		{
			if (!initExtensions())
			{
				return;
			}
		}
		if (m_OpenGLRequirements == TRI_STATE_DOESNT_WORK)
		{
			return;
		}
		// bail if not initialized
		// WRONG SKVINAY, use glusphere or something like that
		if (!m_Initialized)
		{
			return;
		}
		if (m_NumGaussians==0)
		{
			return;
		}
		if (m_Dirty)
		{
			prepareBuffers();
		}
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

				glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);
				glDisable(GL_CULL_FACE);
				bindProgramAndParams();
				glDisable(GL_LIGHTING);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_DEPTH_TEST);
				glDisable(GL_COLOR_SUM_ARB);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glSecondaryColor3fEXT(0.,0.,1.);
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				bindMatrices();
				renderOnce();
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				unbindProgramAndParams();
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
				glDisable(GL_BLEND);
				glDisable(GL_COLOR_SUM_ARB);
				glPopAttrib();

				glPopMatrix();
			}
		}
	}
}

void GaussianRenderer::allocateBufferObjects(unsigned int num)
{
	if (m_OpenGLRequirements == TRI_STATE_UNKNOWN)
	{
		if (!initExtensions())
		{
			return;
		}
	}
	if (m_OpenGLRequirements == TRI_STATE_DOESNT_WORK)
	{
		return;
	}
	// OK, buffers are present on this card.
	deallocateBufferObjects();
	m_BufferObjects = new GLuint[num];
	m_NumBufferObjectsAllocated = num;
	glGenBuffersARB(num, m_BufferObjects);
}

bool GaussianRenderer::bindProgramAndParams()
{
	#ifdef CG
	// Set up for ball rendering
	cgGLEnableProfile(m_VertexProfile);
	cgGLBindProgram(m_VertexProgram);
	cgGLEnableProfile(m_FragmentProfile);
	cgGLBindProgram(m_FragmentProgram);
	cgGLEnableTextureParameter(m_LightMapParam);
	cgGLSetParameter4f(m_MainColorParam, 1.0, 0.0, 0.0, 1.0); // red
	#endif
	return true;
}

bool GaussianRenderer::unbindProgramAndParams()
{
	#ifdef CG
	cgGLDisableTextureParameter(m_LightMapParam);
	cgGLDisableProfile(m_VertexProfile);
	cgGLDisableProfile(m_FragmentProfile);
	#endif
	return true;
}

void GaussianRenderer::bindMatrices()
{
	#ifdef CG
	cgGLSetStateMatrixParameter(m_ModelViewITParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE_TRANSPOSE);
	cgGLSetStateMatrixParameter(m_ModelViewProjParam, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(m_ProjParam, CG_GL_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
	cgGLSetStateMatrixParameter(m_ModelViewInverseParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE);
	#endif
}
