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
// GeometryRenderer.h: interface for the GeometryRenderer class.

#if !defined(AFX_GEOMETRYRENDERER_H__B7BCA210_FD1B_474D_85CC_C9F9F78E946F__INCLUDED_)
#define AFX_GEOMETRYRENDERER_H__B7BCA210_FD1B_474D_85CC_C9F9F78E946F__INCLUDED_

#include <OpenGL_Viewer/Plane.h>
#include <UsefulMath/Vector.h>
#include <UsefulMath/Matrix.h>
#include <vector>

class Geometry;
class GeometryScene;
class DepthPeelingRenderer;

namespace CCVTexMolGeometryRenderer
{

	class BumpMapRenderer;

	class GeometryRenderer
	{
		public:
			GeometryRenderer();
			virtual ~GeometryRenderer();

			// WARNING: dont declare specific values as I am using the SIZE variable
			enum RenderingType {SMOOTH, FLAT, SIZE, INVALID};

			// if you render transparent geometry, it will be merged instead
			bool render(GeometryScene* geometryScene, float* surfaceColors, const char* renderingTypeName,
						Plane plane, bool useClippingPlane, float surfaceAlpha = 1.0f);
			bool render(Geometry* geometry, float* surfaceColors, const char* renderingTypeName,
						Plane plane, bool useClippingPlane, float surfaceAlpha = 1.0f);
			bool render(Geometry* geometry);

			int getNumberOfRenderingTypes();
			char* getRenderingTypeName(int index);
			void setClipPlanes(double aspectX, double aspectY, double aspectZ) const;
			void disableClipPlanes() const;

			void drawGeometry(Geometry* geometry, float* surfaceColors, const char* renderingTypeName,
							  Plane plane, bool useClippingPlane, float surfaceAlpha);
			void drawPoints(Geometry* geometry);
			void drawLines(Geometry* geometry);

			void drawTris(Geometry* geometry, float* surfaceColors, float surfaceAlpha);
			//void drawTrisInternal(Geometry* geometry, float* surfaceColors, float surfaceAlpha, bool updateAlpha = true);
			void drawTrisInternal(Geometry* geometry, float* surfaceColors, float surfaceAlpha, bool updateAlpha = true, bool enableColorBuffer = true);
			void drawTrisWireframe(Geometry* geometry);

			void drawFlatTris(Geometry* geometry);
			void drawQuads(Geometry* geometry);
			void drawFlatQuads(Geometry* geometry);

			RenderingType getRenderingType(const char* renderingTypeName);
			char** m_RenderingTypes;

			bool initBumpMapRendering();
			void useBumpMapping(bool use);

			static void resetTransparentGeometry();
			static void renderTransparentGeometry();
		protected:
			BumpMapRenderer* m_BumpMapRenderer;
			void bumpMapTriangles(Geometry* geometry, float* surfaceColors);
			bool m_UseBumpMapping;
		private:
			//static Geometry* m_TransparentGeometry;
			static std::vector<Geometry*> m_TransparentGeometry;
			static std::vector<CCVOpenGLMath::Matrix*> m_TransparentGeoMatrices;
	};
};

#endif // !defined(AFX_GEOMETRYRENDERER_H__B7BCA210_FD1B_474D_85CC_C9F9F78E946F__INCLUDED_)
