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
#if !defined(AFX_BALLRENDERER_H__18AC0DE6_49B9_4036_9221_63B5DF373C4B__INCLUDED_)
#define AFX_BALLRENDERER_H__18AC0DE6_49B9_4036_9221_63B5DF373C4B__INCLUDED_

#include <glew/glew.h>

#ifdef CG
#include <Cg/cgGL.h>
#endif

#include <stdio.h>
#include <ImposterRenderer/ExpandableBuffer.h>

namespace CCVImposterRenderer
{
	class PlainSphereRenderer;
	class FunctionSphereRenderer;

	class BallRenderer
	{
		public:
			BallRenderer();
			virtual ~BallRenderer();
			void clearBalls();
			inline void addBall(float x, float y, float z, float radius, float red, float green, float blue);
			int getNumBalls() const;
			void initRenderer(bool drawFunction);
			void renderBuffer(bool useHardwareBuffer, bool useFunctionOnSurface, double* rotations, double* translations, int numberOfTransformations, bool randomizeColors, float avgradius);
			bool addFunction(unsigned char* data, double minx, double miny, double minz, double maxx, double maxy, double maxz, int width, int height, int depth);
			static void forceMeshRendering(bool status)
			{
				m_UseGLFallback = status;
			}

		protected:
			bool initCG();
			bool initExtensions();
			void initDefaults();
			void prepareBuffers();
			void allocateBufferObjects(unsigned int num);
			void deallocateBufferObjects();
			void renderOnce();
			void initAO();
			void prepareAOvolume(float delta, double* rotations, double* translations, int numberOfTransformations);
			// the buffers
			ExpandableBuffer<GLfloat> m_TextureCoord;
			ExpandableBuffer<GLfloat> m_Position;
			ExpandableBuffer<GLfloat> m_Color;
			bool m_Dirty;
			unsigned int m_NumBalls;
			unsigned int m_NumBufferObjectsAllocated;
			GLuint* m_BufferObjects;
			int m_CGRequirements;
			int m_OpenGLRequirements;
			int m_Texture3dPresent;
			bool m_Initialized;
			static bool m_UseGLFallback;
			int m_ProgramsLoaded;
			PlainSphereRenderer* m_PlainSphereRenderer;
			FunctionSphereRenderer* m_FunctionSphereRenderer;
			int _rendResX, _rendResY;
			// ambient occlusion
			GLuint AOtex;
			float m_AOdelta;
			float m_bmin[3];
			int m_gn[3];
	};

	inline void BallRenderer::addBall(float x, float y, float z, float radius, float red, float green, float blue)
	{
		m_Dirty = true;
		bool result = true;
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
		result &= m_Color.add(radius);
		result &= m_Color.add(red);
		result &= m_Color.add(green);
		result &= m_Color.add(blue);
		result &= m_Color.add(radius);
		result &= m_Color.add(red);
		result &= m_Color.add(green);
		result &= m_Color.add(blue);
		result &= m_Color.add(radius);
		result &= m_Color.add(red);
		result &= m_Color.add(green);
		result &= m_Color.add(blue);
		result &= m_Color.add(radius);
		if (!result)
		{
			printf("Warning, could not add ball");
		}
		m_NumBalls++;
	}

};

#endif
