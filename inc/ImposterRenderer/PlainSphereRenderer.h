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
#if !defined(AFX_PLAINSPHERERENDERER_H__EADD46E7_06F6_4BEF_B26C_21081B2E64CC__INCLUDED_)
#define AFX_PLAINSPHERERENDERER_H__EADD46E7_06F6_4BEF_B26C_21081B2E64CC__INCLUDED_

#ifdef CG
#include <Cg/cgGL.h>
#endif

#ifdef GLSL
class glslShader;
#endif

namespace CCVImposterRenderer
{
	class Texture;
	class PlainSphereRenderer
	{
		public:
			PlainSphereRenderer();
			virtual ~PlainSphereRenderer();
			bool bindProgramAndParams();
			bool unbindProgramAndParams();
			bool initCG();
			bool loadProgram();
			void bindMatrices();
			void setAOparams(float delta, float min[3], int gresol[3]);
			void AOtexID( GLuint val ) { _AOtexID = val; }
			GLuint AOtexID( void ) { return _AOtexID; }

		protected:
			bool bindProgramAndParamsCg();
			bool bindProgramAndParamsGLSL();
			bool unbindProgramAndParamsCg();
			bool unbindProgramAndParamsGLSL();
			bool loadProgramCg();
			bool loadProgramGLSL();
			#ifdef CG
			CGprofile m_VertexProfile;
			CGprofile m_FragmentProfile;
			CGprogram m_VertexProgram;
			CGprogram m_FragmentProgram;
			CGparameter m_PositionParam;
			CGparameter m_ColorParam;
			CGparameter m_AmbientParam;
			CGparameter m_ModelViewParam;
			CGparameter m_ModelViewInverseParam;
			CGparameter m_ModelViewProjParam;
			CGparameter m_ModelViewTParam;
			CGparameter m_VP_ProjParam;
			CGparameter m_FP_ProjParam;
			CGparameter m_MainColorParam;
			CGparameter m_LightMapParam;
			CGparameter m_DepthMapParam;
			CGparameter m_NormalMapParam;
			CGparameter m_AOMapParam;
			CGparameter m_AODeltaParam;
			CGparameter m_BBoxMinParam;
			CGparameter m_AOResolParam;
			#endif
			#ifdef GLSL
			glslShader *_shader;
			GLuint m_AmbientParam;
			GLuint m_MainColorParam;
			GLuint m_LightMapParam;
			GLuint m_DepthMapParam;
			GLuint m_NormalMapParam;
			GLuint m_AOMapParam;
			GLuint m_AODeltaParam;
			GLuint m_BBoxMinParam;
			GLuint m_AOResolParam;
			#endif
			Texture* m_TextureLightMap;
			Texture* m_TextureDepthMap;
			Texture* m_TextureNormalMap;
			GLuint _AOtexID;
			float AOdelta;
			float AOvolMin[3];
			int AOvolResol[3];
	};
};

#endif
