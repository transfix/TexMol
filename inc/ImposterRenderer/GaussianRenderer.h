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
#if !defined(AFX_GAUSSIANRENDERER_H__B61DC823_49F9_4912_BE8E_6E4AABE7F0CB__INCLUDED_)
#define AFX_GAUSSIANRENDERER_H__B61DC823_49F9_4912_BE8E_6E4AABE7F0CB__INCLUDED_

#ifdef _WIN32
#  include <TexMol/compat.h>
#endif
#include <GL/glew.h>

#ifdef CG
#include <Cg/cgGL.h>
#endif

#include <stdio.h>
#include <ImposterRenderer/ExpandableBuffer.h>
#include <math.h>

namespace CCVImposterRenderer
{
	class PlainSphereRenderer;
	class FunctionSphereRenderer;
	class Texture;

	class GaussianRenderer
	{
		public:
			GaussianRenderer();
			virtual ~GaussianRenderer();
			void clearGaussians();
			inline void addGaussian(float x, float y, float z, float radius, float red, float green, float blue);
			int getNumGaussians() const;
			void initRenderer(bool drawFunction);
			void setApproxSort( int nslices ) {
				nSlices( nslices );

				if( _posSort ) delete[] _posSort;
				_posSort = new ExpandableBuffer<GLfloat>[ nslices ];
				if( _colSort ) delete[] _colSort;
				_colSort = new ExpandableBuffer<GLfloat>[ nslices ];
				if( _sortBufSize ) delete[] _sortBufSize;
				_sortBufSize = new int[ nslices ];
				m_approxSort = true;
			}

			void approxSort( float viewplane[4], float mind, float maxd )
			{
				viewPlane(viewplane);
				minDistFromView( mind );
				maxDistFromView( maxd );
				sliceDelta( (maxd - mind)/(float)_nslices );

				for(int i = 0; i < _nslices; i++ ) {
					_sortBufSize[i] = 0;
					_posSort[i].clearBuffer();
					_colSort[i].clearBuffer();
				}
				float *pos = m_Position.getBuffer();
				float *col = m_Color.getBuffer();
				for(int i = 0; i < m_NumGaussians; i++ )
				{
					float x = pos[i*3];
					float y = pos[i*3+1];
					float z = pos[i*3+2];
					float r = col[i*4];
					float g = col[i*4+1];
					float b = col[i*4+2];
					float rad = col[i*4+3];
					int id = getSlice( x, y, z );
					if( id < 0 ) continue;
					_posSort[id].add(x);
					_posSort[id].add(y);
					_posSort[id].add(z);
					_posSort[id].add(0);
					_posSort[id].add(x);
					_posSort[id].add(y);
					_posSort[id].add(z);
					_posSort[id].add(1);
					_posSort[id].add(x);
					_posSort[id].add(y);
					_posSort[id].add(z);
					_posSort[id].add(2);
					_posSort[id].add(x);
					_posSort[id].add(y);
					_posSort[id].add(z);
					_posSort[id].add(3);
					_colSort[id].add(r);
					_colSort[id].add(g);
					_colSort[id].add(b);
					_colSort[id].add(rad);
					_colSort[id].add(r);
					_colSort[id].add(g);
					_colSort[id].add(b);
					_colSort[id].add(rad);
					_colSort[id].add(r);
					_colSort[id].add(g);
					_colSort[id].add(b);
					_colSort[id].add(rad);
					_colSort[id].add(r);
					_colSort[id].add(g);
					_colSort[id].add(b);
					_colSort[id].add(rad);
					_sortBufSize[id]++;
				}
			}

			void renderBuffer(double* rotations, double* translations, int numberOfTransformations);
			static void forceMeshRendering(bool status)
			{
				m_UseGLFallback = status;
			}

			void Blobbyness( float val ) { m_Blobbyness = val; }
			float Blobbyness( void ) { return m_Blobbyness; }

		protected:
			bool initCG();
			bool initExtensions();
			bool loadProgram();
			void prepareBuffers();
			bool bindProgramAndParams();
			bool unbindProgramAndParams();
			void bindMatrices();
			void allocateBufferObjects(unsigned int num);
			void deallocateBufferObjects();
			void renderOnce();

			void viewPlane( float plane[4] ) { _viewPlane[0] = plane[0]; _viewPlane[1] = plane[1]; _viewPlane[2] = plane[2]; _viewPlane[3] = plane[3]; }
			const float *viewPlane( void ) { return _viewPlane; }
			float viewPlaneDist( float x, float y, float z ) { return _viewPlane[0]*x + _viewPlane[2]*y + _viewPlane[2]*z + _viewPlane[3]; }
			void nSlices( int val ) { _nslices = val; }
			int nSlices( void ) { return _nslices; }
			void sliceDelta( float val ) { _sliceDelta = val; }
			float sliceDelta( void ) { return _sliceDelta; }
			void minDistFromView( float val ) { _minDistFromView = val; }
			float minDistFromView( void ) { return _minDistFromView; }
			void maxDistFromView( float val ) { _maxDistFromView = val; }
			float maxDistFromView( void ) { return _maxDistFromView; }
			inline int getSlice( float x, float y, float z )
			{
				float dist = viewPlaneDist( x, y, z );
				if( dist < _minDistFromView || dist > _maxDistFromView ) {
					fprintf( stderr, "error dist: %f\n", dist );
					return -1;
				}
				else
					return (int)((dist - _minDistFromView)/_sliceDelta);
			}
			// gaussian blur param.
			float m_Blobbyness;

			// approx sort info
			float _viewPlane[4];
			int _nslices;
			float _sliceDelta;
			float _minDistFromView;
			float _maxDistFromView;

			bool m_Initialized;
			static bool m_UseGLFallback;
			// the buffers
			bool m_approxSort;
			int *_sortBufSize;
			ExpandableBuffer<GLfloat> *_tcoordSort;
			ExpandableBuffer<GLfloat> *_posSort;
			ExpandableBuffer<GLfloat> *_colSort;

			ExpandableBuffer<GLfloat> m_TextureCoord;
			ExpandableBuffer<GLfloat> m_Position;
			ExpandableBuffer<GLfloat> m_Color;
			bool m_Dirty;
			unsigned int m_NumGaussians;
			unsigned int m_NumBufferObjectsAllocated;
			GLuint* m_BufferObjects;
			int m_CGRequirements;
			int m_OpenGLRequirements;
			int m_ProgramsLoaded;
			#ifdef CG
			CGprogram m_VertexProgram;
			CGprogram m_FragmentProgram;
			CGprofile m_VertexProfile;
			CGprofile m_FragmentProfile;
			CGparameter m_PositionParam;
			CGparameter m_ColorParam;
			CGparameter m_ModelViewITParam;
			CGparameter m_ModelViewInverseParam;
			CGparameter m_ModelViewProjParam;
			CGparameter m_ProjParam;
			CGparameter m_MainColorParam;
			CGparameter m_LightMapParam;
			#endif
			Texture* m_TextureLightMap;
	};

	inline void GaussianRenderer::addGaussian(float x, float y, float z, float radius, float red, float green, float blue)
	{
		m_Dirty = true;
		bool result = true;
		const double eps = 10.0e-3f;
		const double smoothScale = 2.0;
		float electronDenRad = smoothScale*sqrt(1.0 + log(eps) / m_Blobbyness);
		if( m_approxSort )
		{
			result &= m_Position.add(x);
			result &= m_Position.add(y);
			result &= m_Position.add(z);

			result &= m_Color.add(red);
			result &= m_Color.add(green);
			result &= m_Color.add(blue);
			result &= m_Color.add(electronDenRad); //0.1f);
		}
		else {
			// position
			result &= m_Position.add(x);
			result &= m_Position.add(y);
			result &= m_Position.add(z);
			result &= m_Position.add(0);
			result &= m_Position.add(x);
			result &= m_Position.add(y);
			result &= m_Position.add(z);
			result &= m_Position.add(1);
			result &= m_Position.add(x);
			result &= m_Position.add(y);
			result &= m_Position.add(z);
			result &= m_Position.add(2);
			result &= m_Position.add(x);
			result &= m_Position.add(y);
			result &= m_Position.add(z);
			result &= m_Position.add(3);
			// color
			result &= m_Color.add(red);
			result &= m_Color.add(green);
			result &= m_Color.add(blue);
			result &= m_Color.add(electronDenRad); //0.1f);
			result &= m_Color.add(red);
			result &= m_Color.add(green);
			result &= m_Color.add(blue);
			result &= m_Color.add(electronDenRad); //0.6f);
			result &= m_Color.add(red);
			result &= m_Color.add(green);
			result &= m_Color.add(blue);
			result &= m_Color.add(electronDenRad); //0.1f);
			result &= m_Color.add(red);
			result &= m_Color.add(green);
			result &= m_Color.add(blue);
			result &= m_Color.add(electronDenRad); //0.1f);
		}
		if (!result)
		{
			printf("Warning, could not add gaussian");
		}
		m_NumGaussians++;
	}

};

#endif
