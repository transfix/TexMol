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
#if !defined(AFX_IMPOSTERRENDERER_H__01E3AE03_D9AD_4A30_A364_BA19EF5ADEFE__INCLUDED_)
#define AFX_IMPOSTERRENDERER_H__01E3AE03_D9AD_4A30_A364_BA19EF5ADEFE__INCLUDED_

class VolumeData;

#ifdef _WIN32
#  include <TexMol/compat.h>
#endif
#include <GL/glew.h>

#include <ImposterRenderer/Texture.h>
#include <ImposterRenderer/BallRenderer.h>
#include <ImposterRenderer/GaussianRenderer.h>
#include <ImposterRenderer/StickRenderer.h>
#include <ImposterRenderer/HelixRenderer.h>
#include <ImposterRenderer/HollowCylinderRenderer.h>
#include <ImposterRenderer/sphereDrawing.h>
#ifdef CG
#include <Cg/cgGL.h>
#endif

namespace CCVImposterRenderer
{
	class ImposterRenderer
	{
		public:
			ImposterRenderer();
			virtual ~ImposterRenderer();
			bool initRenderer();
			void renderBuffer(bool useHardwareBuffer, bool useFunctionOnSurface, double* rotations, double* translations, int numberOfTransformations, bool randomizeColors, float avgradius);
			void initSubRenderers(bool drawFunction);
			bool addFunction(unsigned char* data, double minx, double miny, double minz, double maxx, double maxy, double maxz, int width, int height, int depth);
			void drawFunctionOnSurfaces(bool drawFunction);
			void clear();
			BallRenderer* m_BallRenderer;
			GaussianRenderer* m_GaussianRenderer;
			StickRenderer* m_StickRenderer;
			HelixRenderer* m_HelixRenderer;
			HollowCylinderRenderer* m_HollowCylinderRenderer;
			/* force using OpenGL meshes instead of the CG impostors */
			static void forceMeshRendering(bool status);

		protected:
			bool initialized;
			// general cg
			#ifdef CG
			CGcontext m_ShaderContext;
			CGprofile m_VertexProfile;
			CGprofile m_FragmentProfile;
			#endif
	};
};

#endif
