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
#if !defined(AFX_HELIXRENDERER_H__55F98E43_5A95_4CB7_9F7D_B6ADBD4D0FA9__INCLUDED_)
#define AFX_HELIXRENDERER_H__55F98E43_5A95_4CB7_9F7D_B6ADBD4D0FA9__INCLUDED_

#include <glew/glew.h>

#ifdef CG
#include <Cg/cgGL.h>
#endif

#include <stdio.h>
#include <ImposterRenderer/Texture.h>
#include <ImposterRenderer/ExpandableBuffer.h>

namespace CCVImposterRenderer
{
	#ifdef GLSL
	class glslShader;
	#endif

	class HelixRenderer
	{
		public:
			HelixRenderer();
			virtual ~HelixRenderer();

			void clearHelics();
			inline void addHelix(float x1, float y1, float z1, float x2, float y2, float z2, float radius,
								 float red1, float green1, float blue1,
								 float red2, float green2, float blue2);
			int getNumHelics() const;
			void initRenderer(bool drawFunction);
			void renderBuffer(bool useHardwareBuffer, bool useFunctionOnSurface, double* rotations, double* translations, int numberOfTransformations, bool randomizeColors);
			static void forceMeshRendering(bool status)
			{
				m_UseGLFallback = status;
			}

		protected:
			void bindProgramAndParamsCg();
			void bindProgramAndParamsGLSL();
			void bindProgramAndParams();
			void unbindProgramCg();
			void unbindProgramGLSL();
			void unbindProgram();
			void bindMatrices();
			bool loadProgramCg();
			bool loadProgramGLSL();
			bool loadProgram();

			bool initCG(bool drawFunction);
			bool initExtensions();
			void initDefaults();
			bool m_Initialized;
			static bool m_UseGLFallback;
			// profiles
			#ifdef CG
			CGprofile m_VertexProfile;
			CGprofile m_FragmentProfile;
			CGprogram m_VertexProgram;
			CGparameter m_ModelViewITParam;
			CGparameter m_ModelViewInverseParam;
			CGparameter m_ModelViewProjParam;
			CGparameter m_ModelViewParam;
			CGparameter m_VP_ProjParam;
			CGprogram m_FragmentProgram;
			CGparameter m_MainColorParam;
			CGparameter m_NormalMapParam;
			CGparameter m_DepthMapParam;
			CGparameter m_OpacityMapParam;
			CGparameter m_SilhouetteMapParam;
			CGparameter m_SpecularWeightParam;
			CGparameter m_SilhouetteColorParam;
			CGparameter m_FP_ProjParam;
			#endif
			#ifdef GLSL
			glslShader *_shader;
			GLuint m_MainColorParam;
			GLuint m_NormalMapParam;
			GLuint m_DepthMapParam;
			GLuint m_OpacityMapParam;
			GLuint m_SilhouetteMapParam;
			GLuint m_SpecularWeightParam;
			GLuint m_SilhouetteColorParam;
			#endif
			Texture* m_TextureNormalMap;
			Texture* m_TextureDepthMap;
			Texture* m_TextureOpacityMap;
			Texture* m_TextureSilhouetteMap;
			// the buffers
			ExpandableBuffer<GLfloat> m_TextureCoord1;
			ExpandableBuffer<GLfloat> m_TextureCoord2;
			ExpandableBuffer<GLfloat> m_Position;
			ExpandableBuffer<GLfloat> m_Color;
			unsigned int m_NumHelics;
	};

	inline void HelixRenderer::addHelix(
		float x1, float y1, float z1,
		float x2, float y2, float z2,
		float radius,
		float red1, float green1, float blue1,
		float red2, float green2, float blue2)
	{
		double length = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2);
		if (length > 50)
		{
			return;
		}
		bool result = true;
		// position - stores the offsets
		result &= m_Position.add(0);
		result &= m_Position.add(-1);
		result &= m_Position.add(radius);
		result &= m_Position.add(1);

		result &= m_Position.add(0);
		result &= m_Position.add(1);
		result &= m_Position.add(radius);
		result &= m_Position.add(1);

		result &= m_Position.add(1);
		result &= m_Position.add(1);
		result &= m_Position.add(radius);
		result &= m_Position.add(1);

		result &= m_Position.add(1);
		result &= m_Position.add(-1);
		result &= m_Position.add(radius);
		result &= m_Position.add(1);
		// texturecoord - stores the 2 end points
		result &= m_TextureCoord1.add(x1);
		result &= m_TextureCoord1.add(y1);
		result &= m_TextureCoord1.add(z1);
		result &= m_TextureCoord1.add(1);

		result &= m_TextureCoord1.add(x1);
		result &= m_TextureCoord1.add(y1);
		result &= m_TextureCoord1.add(z1);
		result &= m_TextureCoord1.add(1);

		result &= m_TextureCoord1.add(x1);
		result &= m_TextureCoord1.add(y1);
		result &= m_TextureCoord1.add(z1);
		result &= m_TextureCoord1.add(1);

		result &= m_TextureCoord1.add(x1);
		result &= m_TextureCoord1.add(y1);
		result &= m_TextureCoord1.add(z1);
		result &= m_TextureCoord1.add(1);

		result &= m_TextureCoord2.add(x2);
		result &= m_TextureCoord2.add(y2);
		result &= m_TextureCoord2.add(z2);
		result &= m_TextureCoord2.add(1);
		result &= m_TextureCoord2.add(x2);
		result &= m_TextureCoord2.add(y2);
		result &= m_TextureCoord2.add(z2);
		result &= m_TextureCoord2.add(1);
		result &= m_TextureCoord2.add(x2);
		result &= m_TextureCoord2.add(y2);
		result &= m_TextureCoord2.add(z2);
		result &= m_TextureCoord2.add(1);
		result &= m_TextureCoord2.add(x2);
		result &= m_TextureCoord2.add(y2);
		result &= m_TextureCoord2.add(z2);
		result &= m_TextureCoord2.add(1);
		// color
		result &= m_Color.add(red1);
		result &= m_Color.add(green1);
		result &= m_Color.add(blue1);
		result &= m_Color.add(0.4f);
		result &= m_Color.add(red1);
		result &= m_Color.add(green1);
		result &= m_Color.add(blue1);
		result &= m_Color.add(0.0f);
		result &= m_Color.add(red2);
		result &= m_Color.add(green2);
		result &= m_Color.add(blue2);
		result &= m_Color.add(0.0f);
		result &= m_Color.add(red2);
		result &= m_Color.add(green2);
		result &= m_Color.add(blue2);
		result &= m_Color.add(0.4f);
		if (!result)
		{
			printf("Warning, could not add Stick");
		}
		m_NumHelics++;
	}
};

#endif
