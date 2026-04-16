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
#if !defined(AFX_FUNCTIONSPHERERENDERER_H__6F47A833_5065_4885_A0C3_B44CF7530782__INCLUDED_)
#define AFX_FUNCTIONSPHERERENDERER_H__6F47A833_5065_4885_A0C3_B44CF7530782__INCLUDED_

#include <GL/glew.h>
#ifdef CG
#include <Cg/cgGL.h>
#endif

namespace CCVImposterRenderer
{
	#ifdef GLSL
	class glslShader;
	#endif
	class Texture;

	class FunctionSphereRenderer
	{
		public:
			FunctionSphereRenderer();
			virtual ~FunctionSphereRenderer();
			void bindProgramAndParams();
			void unbindProgramAndParams();
			bool initCG();
			bool loadProgram();
			bool fallbackLoadProgram();
			bool addFunction(unsigned char* data, double minx, double miny, double minz, double maxx, double maxy, double maxz, int width, int height, int depth);
			void bindMatrices();

		protected:
			void bindProgramAndParamsCg();
			void bindProgramAndParamsGLSL();
			void unbindProgramAndParamsCg();
			void unbindProgramAndParamsGLSL();
			bool loadProgramCg();
			bool loadProgramGLSL();
			#ifdef CG
			CGprofile m_VertexProfile;
			CGprofile m_FragmentProfile;
			CGprogram m_VertexProgram;
			CGprogram m_FragmentProgram;
			CGparameter m_PositionParam;
			CGparameter m_ColorParam;
			CGparameter m_ModelViewParam;
			CGparameter m_ModelViewInverseParam;
			CGparameter m_ModelViewProjParam;
			CGparameter m_VP_ProjParam;
			CGparameter m_ModelViewProjInverseParam;
			CGparameter m_MainColorParam;
			CGparameter m_MinExtentParam;
			CGparameter m_MaxExtentParam;
			CGparameter m_LightMapParam;
			CGparameter m_DepthMapParam;
			CGparameter m_FunctionMapParam;	
			CGparameter m_FP_ProjParam;
			#endif
			#ifdef GLSL
			glslShader *_shader;
			GLuint m_MainColorParam;
			GLuint m_MinExtentParam;
			GLuint m_MaxExtentParam;
			GLuint m_LightMapParam;
			GLuint m_DepthMapParam;
			GLuint m_FunctionMapParam;	
			#endif
			Texture* m_TextureLightMap;
			Texture* m_TextureDepthMap;
			Texture* m_TextureFunctionMap;
			unsigned char* m_Data;
			double m_minx;
			double m_miny;
			double m_minz;
			double m_maxx;
			double m_maxy;
			double m_maxz;
			int m_Width;
			int m_Height;
			int m_Depth;
	};
};

#endif
