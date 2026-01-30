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
#include <ImposterRenderer/BallRenderer.h>
#include <ImposterRenderer/FunctionSphereRenderer.h>
#include <ImposterRenderer/GlobalCGContext.h>
#include <ImposterRenderer/MeshSphereRenderer.h>
#include <ImposterRenderer/PlainSphereRenderer.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using CCVImposterRenderer::BallRenderer;
using CCVImposterRenderer::PlainSphereRenderer;
using CCVImposterRenderer::FunctionSphereRenderer;

static const int BatchSize = 10000;
static const int TRI_STATE_WORKS = 0;
static const int TRI_STATE_DOESNT_WORK= 1;
static const int TRI_STATE_UNKNOWN = 2;
bool BallRenderer::m_UseGLFallback = false;

BallRenderer::BallRenderer()
{
	initDefaults();
}

void BallRenderer::initDefaults()
{
	m_Initialized = false;
	m_UseGLFallback = false;
	m_NumBalls = 0;
	m_Dirty = true;
	m_NumBufferObjectsAllocated = 0;
	m_BufferObjects = 0;
	m_CGRequirements = TRI_STATE_UNKNOWN;
	m_OpenGLRequirements = TRI_STATE_UNKNOWN;
	m_ProgramsLoaded = TRI_STATE_UNKNOWN;
	m_Texture3dPresent = TRI_STATE_UNKNOWN;
	m_PlainSphereRenderer = new PlainSphereRenderer();
	m_FunctionSphereRenderer = 0;

	_rendResX = 0;
	_rendResY = 0;

	m_AOdelta = 24.76;
}

BallRenderer::~BallRenderer()
{
	deallocateBufferObjects();
	delete m_PlainSphereRenderer;
	m_PlainSphereRenderer = 0;
	delete m_FunctionSphereRenderer;
	m_FunctionSphereRenderer = 0;
}

void BallRenderer::initRenderer(bool drawFunction)
{
	// without extensions we cannot operate at all
	if (!initExtensions())
	{
		m_Initialized = false;
		fprintf(stderr, "BallRender: ERROR: OpenGL extensions unavailable, update your graphics drivers.\n");
		return;
	}

	#ifdef AO
	// generate 3D texture for Ambient Occlusion
	initAO();
	#endif

	// if FP30 isn't present we will use a different branch of every render method
	if (!initCG())
	{
		fprintf(stderr, "BallRender: WARNING: VP20/VP40/FP30/FP40 not found, falling back on GL Rendering.\n");
		m_UseGLFallback = true;
	}

	m_Initialized = true;
}

bool BallRenderer::initCG()
{
	if (m_CGRequirements != TRI_STATE_UNKNOWN)
	{
		return true;
	}
	// drastic, change
	if (!m_PlainSphereRenderer)
	{
		m_CGRequirements = TRI_STATE_DOESNT_WORK;
		return false;
	}

	#ifdef CG
	if (!m_PlainSphereRenderer->initCG())
	{
		m_CGRequirements = TRI_STATE_DOESNT_WORK;
		return false;
	}
	#endif

	#ifdef AO
	m_PlainSphereRenderer->AOtexID( AOtex );
	#endif
	bool ret = m_PlainSphereRenderer->loadProgram();

	if (ret)
	{
		m_CGRequirements = TRI_STATE_WORKS;
	}
	else
	{
		m_CGRequirements = TRI_STATE_DOESNT_WORK;
		fprintf( stderr, "BallRender: ERROR: load program\n");
	}
	return ret;
}

bool BallRenderer::initExtensions()
{
	if(!glewIsSupported("GL_VERSION_1_2") ||
	   !glewIsSupported("GL_EXT_secondary_color") ||
	   !glewIsSupported("GL_ARB_multitexture") ||
	   !glewIsSupported("GL_ARB_vertex_buffer_object"))
	{
		printf("BallRender: init base extensions fail\n");
		m_OpenGLRequirements = TRI_STATE_DOESNT_WORK;
		return false;
	}

	#ifdef CGARB
	if(!glewIsSupported("GL_ARB_vertex_shader") ||
	   !glewIsSupported("GL_ARB_fragment_shader"))
	{
		printf("BallRenderer: init ARB extensions fail for Cg\n");
		m_OpenGLRequirements = TRI_STATE_DOESNT_WORK;
		return false;
	}
	#endif

	#ifdef GLSL
	if(!glewIsSupported("GL_VERSION_2_0") ||
	   !glewIsSupported("GL_ARB_vertex_shader") ||
	   !glewIsSupported("GL_ARB_fragment_shader") ||
	   !glewIsSupported("GL_ARB_shader_objects"))
	{
		printf("BallRenderer: init extensions fail for GLSL\n");
		m_OpenGLRequirements = TRI_STATE_DOESNT_WORK;
		return false;
	}
	#endif

	m_OpenGLRequirements = TRI_STATE_WORKS;

/*
	#ifdef AO
	// check 3D texture
	if(!glewIsSupported("GL_EXT_texture3D"))
	  {
		printf("BallRenderer: init GL_EXT_texture3D extensions fail for ambient occlusion\n");
		m_Texture3dPresent = TRI_STATE_DOESNT_WORK;
	    	return false;
	  }
	#endif
*/
	m_Texture3dPresent = TRI_STATE_WORKS;

	return true;
}

void BallRenderer::clearBalls()
{
	m_TextureCoord.clearBuffer();
	m_Color.clearBuffer();
	m_Position.clearBuffer();
	m_NumBalls = 0;
}

int BallRenderer::getNumBalls() const
{
	return m_NumBalls;
}

void BallRenderer::prepareBuffers()
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
	unsigned int remaining = m_NumBalls;
	unsigned int batch = BatchSize;
	unsigned int offset = 0;
	unsigned int currentBuffer = 0;
	allocateBufferObjects((m_NumBalls/BatchSize+1)*2);
	while (remaining>0)
	{
		unsigned int thisRender = (remaining<batch?remaining:batch);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_BufferObjects[currentBuffer]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, thisRender*4*4*sizeof(float), m_Position.getBuffer()+offset, GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_BufferObjects[currentBuffer+1]);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, thisRender*4*4*sizeof(float), m_Color.getBuffer()+offset, GL_STATIC_DRAW_ARB);
		// decrement remaining
		remaining-=thisRender;
		// increment offset
		offset+=thisRender*4*4;
		currentBuffer+=2;
	}
	m_Dirty = false;
}

static inline unsigned int upToPowerOfTwo(unsigned int value)
{
  unsigned int c = 0;
  unsigned int v = value;

  // round down to nearest power of two 
  while (v>1) {
    v = v>>1;
    c++;
  }

  // if that isn't exactly the original value 
  if ((v<<c)!=value) { 
    // return the next power of two 
    return (v<<(c+1));
  }
  else {
    // return this power of two 
    return (v<<c);
  }
}

#define SWAP_32(a)							\
  {									\
    unsigned char tmp[4];						\
    unsigned char *ch;							\
    ch = (unsigned char *)(a);						\
    tmp[0] = ch[0]; tmp[1] = ch[1]; tmp[2] = ch[2]; tmp[3] = ch[3];	\
    ch[0] = tmp[3]; ch[1] = tmp[2]; ch[2] = tmp[1]; ch[3] = tmp[0];	\
  }

static inline int big_endian()
{
	long one=1;
	return !(*((char *)(&one)));
}

void BallRenderer::prepareAOvolume(float delta, double* rotations, double* translations, int numberOfTransformations)
{
	// compute bounding box
	m_bmin[0] = 1000000.f;
	m_bmin[1] = 1000000.f;
	m_bmin[2] = 1000000.f;
	float bmax[] = { -1000000.f, -1000000.f, -1000000.f };
	const float *pos = m_Position.getBuffer();
	const float *col = m_Color.getBuffer();
	float maxRad = 0.f;
	int tnumBalls = 0;

	if (rotations && translations && (numberOfTransformations>0))
	{
	tnumBalls += m_NumBalls;
	for(int i = 0; i < m_NumBalls; i++ ) {
		float px = pos[i*16  ];
		float py = pos[i*16+1];
		float pz = pos[i*16+2];
		if( maxRad < col[i*16+3] ) maxRad = col[i*16+3];
		for(int j = 0; j < numberOfTransformations; j++ ) {
			float x = rotations[j*9  ] * px + rotations[j*9+1] * py + rotations[j*9+2] * pz + translations[ j*3  ];
			float y = rotations[j*9+3] * px + rotations[j*9+4] * py + rotations[j*9+5] * pz + translations[ j*3+1];
			float z = rotations[j*9+6] * px + rotations[j*9+7] * py + rotations[j*9+8] * pz + translations[ j*3+2];

			if( m_bmin[0] > x ) m_bmin[0] = x;
			if( m_bmin[1] > y ) m_bmin[1] = y;
			if( m_bmin[2] > z ) m_bmin[2] = z;
			if( bmax[0] < x ) bmax[0] = x;
			if( bmax[1] < y ) bmax[1] = y;
			if( bmax[2] < z ) bmax[2] = z;
		}
	}
	}
	else
	{
	for(int i = 0; i < m_NumBalls; i++ ) {
		float x = pos[i*16  ];
		float y = pos[i*16+1];
		float z = pos[i*16+2];
		if( maxRad < col[i*16+3] ) maxRad = col[i*16+3];
		if( m_bmin[0] > x ) m_bmin[0] = x;
		if( m_bmin[1] > y ) m_bmin[1] = y;
		if( m_bmin[2] > z ) m_bmin[2] = z;
		if( bmax[0] < x ) bmax[0] = x;
		if( bmax[1] < y ) bmax[1] = y;
		if( bmax[2] < z ) bmax[2] = z;
	}
	}
	printf( "total atoms: %d\n", tnumBalls );
	printf( "maxRad: %f\n", maxRad );
	float expand = maxRad + delta;
	m_bmin[0] -= expand;
	m_bmin[1] -= expand;
	m_bmin[2] -= expand;
	bmax[0] += expand;
	bmax[1] += expand;
	bmax[2] += expand;
	// allocate volume
	m_gn[0] = (int)((bmax[0] - m_bmin[0]) / delta) + 1;
	m_gn[1] = (int)((bmax[1] - m_bmin[1]) / delta) + 1;
	m_gn[2] = (int)((bmax[2] - m_bmin[2]) / delta) + 1;

	m_gn[0] = upToPowerOfTwo(m_gn[0]);
	m_gn[1] = upToPowerOfTwo(m_gn[1]);
	m_gn[2] = upToPowerOfTwo(m_gn[2]);

	printf( "grid resol: %d %d %d\n", m_gn[0], m_gn[1], m_gn[2]);
	bmax[0] = m_bmin[0] + m_gn[0] * delta;
	bmax[1] = m_bmin[1] + m_gn[1] * delta;
	bmax[2] = m_bmin[2] + m_gn[2] * delta;

	GLfloat *AOvolume = new GLfloat[ m_gn[0]*m_gn[1]*m_gn[2]*3 ];
	memset( AOvolume, 0, sizeof(GLfloat)*m_gn[0]*m_gn[1]*m_gn[2]*3 );

	const float coef = (1.f/0.74f)*(4.f/3.f)*M_PI/(delta*delta*delta);
	if (rotations && translations && (numberOfTransformations>0))
	{
	for(int i = 0; i < m_NumBalls; i++ ) {
		float px = pos[i*16  ];
		float py = pos[i*16+1];
		float pz = pos[i*16+2];
		float rad = col[i*16+3];
		for(int j = 0; j < numberOfTransformations; j++ ) {
			float x = rotations[j*9  ] * px + rotations[j*9+1] * py + rotations[j*9+2] * pz + translations[ j*3  ];
			float y = rotations[j*9+3] * px + rotations[j*9+4] * py + rotations[j*9+5] * pz + translations[ j*3+1];
			float z = rotations[j*9+6] * px + rotations[j*9+7] * py + rotations[j*9+8] * pz + translations[ j*3+2];

			int xi = (int)( (x - m_bmin[0]) / delta);
			int yi = (int)( (y - m_bmin[1]) / delta);
			int zi = (int)( (z - m_bmin[2]) / delta);
			int idx = zi * m_gn[0] * m_gn[1] + yi * m_gn[0] + xi;
			AOvolume[idx*3] += coef * rad * rad * rad;
		}
	}
	}
	else
	{
	for(int i = 0; i < m_NumBalls; i++ ) {
		float x = pos[i*16  ];
		float y = pos[i*16+1];
		float z = pos[i*16+2];
		float rad = col[i*16+3];
		int xi = (int)( (x - m_bmin[0]) / delta);
		int yi = (int)( (y - m_bmin[1]) / delta);
		int zi = (int)( (z - m_bmin[2]) / delta);
		int idx = zi * m_gn[0] * m_gn[1] + yi * m_gn[0] + xi;
		AOvolume[idx*3] += coef * rad * rad * rad;
	}
	}
/*
	// write rawiv
	typedef struct _rawiv_header
	{
		float min[3];
		float max[3];
		unsigned int numVerts;
		unsigned int numCells;
		unsigned int dim[3];
		float origin[3];
		float span[3];
	}RawIVHeader;
	RawIVHeader header;

	header.min[0] = m_bmin[0];
	header.min[1] = m_bmin[1];
	header.min[2] = m_bmin[2];
	header.max[0] = bmax[0];
	header.max[1] = bmax[1];
	header.max[2] = bmax[2];
	header.numVerts = m_gn[0] * m_gn[1] * m_gn[2];
	header.numCells = (m_gn[0]-1) * (m_gn[1]-1) * (m_gn[2]-1);
 	header.dim[0] = m_gn[0];
 	header.dim[1] = m_gn[1];
 	header.dim[2] = m_gn[2];
 	header.origin[0] = m_bmin[0];
 	header.origin[1] = m_bmin[1];
 	header.origin[2] = m_bmin[2];
	header.span[0] = delta;
	header.span[1] = delta;
	header.span[2] = delta;
	if(!big_endian())
	{
	  for(int i=0; i<3; i++) SWAP_32(&(header.min[i]));
	  for(int i=0; i<3; i++) SWAP_32(&(header.max[i]));
	  SWAP_32(&(header.numVerts));
	  SWAP_32(&(header.numCells));
	  for(int i=0; i<3; i++) SWAP_32(&(header.dim[i]));
	  for(int i=0; i<3; i++) SWAP_32(&(header.origin[i]));
	  for(int i=0; i<3; i++) SWAP_32(&(header.span[i]));
	}
	FILE *fp = fopen( "AO.rawiv", "wb");
	fwrite(&header,sizeof(RawIVHeader),1,fp);
	for(int i = 0; i < m_gn[0]*m_gn[1]*m_gn[2]; i++ ) {
		float val = AOvolume[i*3];
		if(!big_endian())
			SWAP_32(&val);
		fwrite( &val, sizeof(float), 1, fp );
	}
	fclose(fp);
*/
/*
	float max = 0;
	for(int i = 0; i < m_gn[0]*m_gn[1]*m_gn[2]; i++ )
		if( AOvolume[ i*3 ] > max ) max = AOvolume[ i ];

	max = 1.f / max;
	for(int i = 0; i < m_gn[0]*m_gn[1]*m_gn[2]; i++ )
		AOvolume[ i*3 ] *= max;
*/
/*
	for(int i = 0; i < m_gn[0]*m_gn[1]*m_gn[2]; i++ ) {
		AOvolume[ i*4   ] = 1.f;
		AOvolume[ i*4 +1] = 0.f;
		AOvolume[ i*4 +2] = 0.f;
		AOvolume[ i*4 +3] = 0.f;
	}
*/
	glBindTexture(GL_TEXTURE_3D, AOtex);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, m_gn[0], m_gn[1], m_gn[2], 0,
				 GL_RGB, GL_FLOAT, AOvolume);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	delete[] AOvolume;
	AOvolume = 0;
}

void BallRenderer::initAO()
{
	glGenTextures(1, &AOtex);
}

void BallRenderer::renderOnce()
{
	// MESH RENDERING
	if (m_UseGLFallback)
	{
		MeshSphereRenderer::DrawSphereBuffer(m_Position, m_Color);
	}
	// CG RENDERING
	else
	{
		// try rendering in batches
		unsigned int remaining = m_NumBalls;
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

void BallRenderer::renderBuffer(bool useHardwareBuffer, bool useFunctionOnSurface, double* rotations, double* translations, int numberOfTransformations, bool randomizeColors, float avgradius)
{
	// MESH RENDERER
	if (m_UseGLFallback)
	{
		if (!m_Initialized)
		{
			return;
		}
		if (m_NumBalls==0)
		{
			return;
		}
		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_LIGHTING);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.5);
		glEnable(GL_COLOR_SUM_ARB);
		glSecondaryColor3fEXT(0.,0.,1.);
		srand(255); // needed to call rand to set colors for SS, CHAIN and PROTEINS
		////// perform transformations if needed and repeat the rendering
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
				if (useFunctionOnSurface)
				{
					m_FunctionSphereRenderer->bindMatrices();
				}
				else
				{
					m_PlainSphereRenderer->bindMatrices();
				}
				if (randomizeColors)
				{
					float blue = (float)(rand() / (RAND_MAX + 1.0f));
					float red = (float)(rand() / (RAND_MAX + 1.0f));
					float green = (float)(rand() / (RAND_MAX + 1.0f));
					glColor4f(red, blue, green, avgradius);
				}
				renderOnce();
				glPopMatrix();
			}
		}
		else
		{
			if (useFunctionOnSurface)
			{
				m_FunctionSphereRenderer->bindMatrices();
			}
			else
			{
				m_PlainSphereRenderer->bindMatrices();
			}
			renderOnce();
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);
		glDisable(GL_COLOR_SUM_ARB);
		glPopAttrib();
	}
	// CG RENDERER
	else
	{
		if (useHardwareBuffer)
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
		}
		// bail if not initialized
		// WRONG SKVINAY, use glusphere or something like that
		if (!m_Initialized)
		{
			return;
		}
		if (m_NumBalls==0)
		{
			return;
		}
		if (m_Dirty)
		{
			prepareBuffers();
			#ifdef AO
			prepareAOvolume(m_AOdelta, rotations, translations, numberOfTransformations);
			if (!m_PlainSphereRenderer)
			{
				return;
			}
			m_PlainSphereRenderer->setAOparams(m_AOdelta, m_bmin, m_gn);
			#endif
			
		}
		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);
		if (useFunctionOnSurface)
		{
			if (!m_FunctionSphereRenderer)
			{
				return;
			}
			m_FunctionSphereRenderer->bindProgramAndParams();
		}
		else
		{
			if (!m_PlainSphereRenderer)
			{
				return;
			}
			m_PlainSphereRenderer->bindProgramAndParams();
		}
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.5);
		glEnable(GL_COLOR_SUM_ARB);
		glSecondaryColor3fEXT(0.,0.,1.);
		glEnableClientState(GL_VERTEX_ARRAY);
		if (! randomizeColors)
		{
			glEnableClientState(GL_COLOR_ARRAY);
		}
		srand(255); // needed to call rand to set colors for SS, CHAIN and PROTEINS
		////// perform transformations if needed and repeat the rendering
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
				if (useFunctionOnSurface)
				{
					m_FunctionSphereRenderer->bindMatrices();
				}
				else
				{
					m_PlainSphereRenderer->bindMatrices();
				}
				if (randomizeColors)
				{
					float blue = (float)(rand() / (RAND_MAX + 1.0f));
					float red = (float)(rand() / (RAND_MAX + 1.0f));
					float green = (float)(rand() / (RAND_MAX + 1.0f));
					glColor4f(red, blue, green, avgradius);
				}
				renderOnce();
				glPopMatrix();
			}
		}
		else
		{
			if (useFunctionOnSurface)
			{
				m_FunctionSphereRenderer->bindMatrices();
			}
			else
			{
				m_PlainSphereRenderer->bindMatrices();
			}
			renderOnce();
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		if (! randomizeColors)
		{
			glDisableClientState(GL_COLOR_ARRAY);
		}
		if (useFunctionOnSurface)
		{
			if (!m_FunctionSphereRenderer)
			{
				return;
			}
			m_FunctionSphereRenderer->unbindProgramAndParams();
		}
		else
		{
			if (!m_PlainSphereRenderer)
			{
				return;
			}
			m_PlainSphereRenderer->unbindProgramAndParams();
		}
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
		glDisable(GL_BLEND);
		glDisable(GL_COLOR_SUM_ARB);
		glDisable(GL_DEPTH_TEST);
		glPopAttrib();
	}
}

void BallRenderer::allocateBufferObjects(unsigned int num)
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

void BallRenderer::deallocateBufferObjects()
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

bool BallRenderer::addFunction(unsigned char* data, double minx, double miny, double minz, double maxx, double maxy, double maxz, int width, int height, int depth)
{
	if (m_UseGLFallback)
	{
		delete m_FunctionSphereRenderer;
		m_FunctionSphereRenderer = new FunctionSphereRenderer();
		if (!m_FunctionSphereRenderer->initCG())
		{
			return false;
		}
		if (!m_FunctionSphereRenderer->addFunction(data, minx, miny, minz, maxx, maxy, maxz, width, height, depth))
		{
			return false;
		}
		return m_FunctionSphereRenderer->fallbackLoadProgram();
	}
	else
	{
		delete m_FunctionSphereRenderer;
		m_FunctionSphereRenderer = new FunctionSphereRenderer();
		if (!m_FunctionSphereRenderer->initCG())
		{
			return false;
		}
		if (!m_FunctionSphereRenderer->addFunction(data, minx, miny, minz, maxx, maxy, maxz, width, height, depth))
		{
			return false;
		}
		return m_FunctionSphereRenderer->loadProgram();
	}
}
