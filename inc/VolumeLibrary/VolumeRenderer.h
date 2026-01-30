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
#if !defined(AFX_VOLUMERENDERER_H__ADD2CDB1_7261_4BA2_8E20_865DD924B4D0__INCLUDED_)
#define AFX_VOLUMERENDERER_H__ADD2CDB1_7261_4BA2_8E20_865DD924B4D0__INCLUDED_

#ifdef _WIN32
#include <windows.h>
#endif

#if defined(__APPLE__)
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif

#include <Geometry/Geometry.h>

namespace OpenGLVolumeRendering
{
	class Renderer;
}

class VolumeRenderer
{
	public:
		VolumeRenderer();
		VolumeRenderer(const VolumeRenderer& copy);
		VolumeRenderer& operator=(const VolumeRenderer& copy);
		virtual ~VolumeRenderer();
		bool initRenderer();
		bool setAspectRatio(double ratioX, double ratioY, double ratioZ);
		bool setTextureSubCube(double minX, double minY, double minZ, double maxX, double maxY, double maxZ);
		bool setQuality(double quality);
		double getQuality() const;
		bool setNearPlane(double nearPlane);
		double getNearPlane();
		bool isShadedRenderingAvailable() const;
		bool enableShadedRendering();
		bool disableShadedRendering();
		bool uploadColorMappedData(const GLubyte* data, int width, int height, int depth);
		bool uploadColorMappedDataWithBorder(const GLubyte* data, int width, int height, int depth);
		bool testColorMappedData(int width, int height, int depth);
		bool testColorMappedDataWithBorder(int width, int height, int depth);
		bool uploadRGBAData(const GLubyte* data, int width, int height, int depth);
		bool uploadGradients(const GLubyte* data, int width, int height, int depth);
		bool calculateGradientsFromDensities(const GLubyte* data, int width, int height, int depth);
		bool uploadColorMap(const GLubyte* colorMap);
		int getNumberOfPlanesRendered() const;
		bool renderVolume();
		Geometry* getGeometry();
		void prepareToRenderGeometry();

	private:
		OpenGLVolumeRendering::Renderer* m_PrivateRenderer;
};

#endif
