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
#include <math.h>
#include <VolumeLibrary/ClipCube.h>
#include <VolumeLibrary/FragmentProgramARBImpl.h>
#include <VolumeLibrary/FragmentProgramImpl.h>
#include <VolumeLibrary/Paletted2DImpl.h>
#include <VolumeLibrary/PalettedImpl.h>
#include <VolumeLibrary/Polygon.h>
#include <VolumeLibrary/Renderer.h>
#include <VolumeLibrary/SGIColorTableImpl.h>
#include <VolumeLibrary/SimpleRGBA2DImpl.h>
#include <VolumeLibrary/SimpleRGBAImpl.h>

using namespace OpenGLVolumeRendering;

Renderer::Renderer()
{
	m_RGBA = 0;
	m_Colormapped = 0;
	initFlags();
}

Renderer::~Renderer()
{
	delete m_Colormapped;
	delete m_RGBA;
}

bool Renderer::initFlags()
{
	m_UseColormapped = false;
	m_DataLoaded = false;
	return true;
}

bool Renderer::initRenderer()
{
	bool colormapped = initColormappedRenderer();
	bool rgba = initRGBARenderer();
	if (colormapped &&
			rgba)
	{
		m_DataLoaded = false;
		return true;
	}
	else
	{
		return false;
	}
}

bool Renderer::setTextureSubCube(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
{
	if (m_Colormapped)
	{
		m_Colormapped->setTextureSubCube(minX, minY, minZ, maxX, maxY, maxZ);
	}
	if (m_RGBA)
	{
		m_RGBA->setTextureSubCube(minX, minY, minZ, maxX, maxY, maxZ);
	}
	return true;
}

bool Renderer::setQuality(double quality)
{
	if (m_Colormapped)
	{
		m_Colormapped->setQuality(quality);
	}
	if (m_RGBA)
	{
		m_RGBA->setQuality(quality);
	}
	return true;
}

double Renderer::getQuality() const
{
	if (m_UseColormapped && m_Colormapped)
	{
		return m_Colormapped->getQuality();
	}
	else if (m_RGBA)
	{
		return m_RGBA->getQuality();
	}
	else
	{
		return 0;
	}
}

bool Renderer::setNearPlane(double nearPlane)
{
	if (m_Colormapped)
	{
		m_Colormapped->setNearPlane(nearPlane);
	}
	if (m_RGBA)
	{
		m_RGBA->setNearPlane(nearPlane);
	}
	return true;
}

double Renderer::getNearPlane()
{
	if (m_Colormapped)
	{
		return m_Colormapped->getNearPlane();
	}
	else if (m_RGBA)
	{
		return m_RGBA->getNearPlane();
	}
	else
	{
		return 1.0;
	}
}

bool Renderer::setAspectRatio(double ratioX, double ratioY, double ratioZ)
{
	if (m_Colormapped)
	{
		m_Colormapped->setAspectRatio(ratioX, ratioY, ratioZ);
	}
	if (m_RGBA)
	{
		m_RGBA->setAspectRatio(ratioX, ratioY, ratioZ);
	}
	return true;
}


bool Renderer::isShadedRenderingAvailable() const
{
	return false;
}

bool Renderer::enableShadedRendering()
{
	return false;
}

bool Renderer::disableShadedRendering()
{
	return true;
}


bool Renderer::uploadColorMappedData(const GLubyte* data, int width, int height, int depth)
{

	if (m_Colormapped && m_Colormapped->uploadColormappedData(data, width, height, depth))
	{
		m_UseColormapped = true;
		m_DataLoaded = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool Renderer::uploadColorMappedDataWithBorder(const GLubyte* data, int width, int height, int depth)
{
	return false;
}

bool Renderer::testColorMappedData(int width, int height, int depth)
{
	return m_Colormapped->testColormappedData(width, height, depth);
}

bool Renderer::testColorMappedDataWithBorder(int width, int height, int depth)
{
	return false;
}

bool Renderer::uploadRGBAData(const GLubyte* data, int width, int height, int depth)
{
	if (m_RGBA && m_RGBA->uploadRGBAData(data, width, height, depth))
	{
		m_UseColormapped = false;
		m_DataLoaded = true;
		return true;
	}
	else
	{
		return false;
	}
}

bool Renderer::uploadGradients(const GLubyte* data, int width, int height, int depth)
{
	return false;
}

bool Renderer::calculateGradientsFromDensities(const GLubyte* data, int width, int height, int depth)
{
	return false;
}

bool Renderer::uploadColorMap(const GLubyte* colorMap)
{
	if (m_Colormapped && m_Colormapped->uploadColorMap(colorMap)) {
		return true;
	}
	else
		return false;
}

int Renderer::getNumberOfPlanesRendered() const
{
	if (m_UseColormapped && m_Colormapped)
	{
		return m_Colormapped->getNumberOfPlanesRendered();
	}
	else if (m_RGBA)
	{
		return m_RGBA->getNumberOfPlanesRendered();
	}
	else
	{
		return 0;
	}
}

bool Renderer::renderVolume()
{
	if (m_UseColormapped)
	{
		return (m_Colormapped?m_Colormapped->renderVolume():false);
	}
	else
	{
		return (m_RGBA?m_RGBA->renderVolume():false);
	}
}

bool Renderer::initColormappedRenderer()
{
	// first we try the paletted version which we know works on
	// Nvidia
	m_Colormapped = new PalettedImpl;
	if (m_Colormapped->initRenderer())
	{
		return true;
	}
	// failed
	delete m_Colormapped;
	m_Colormapped = 0;
	// looks like NVIDIA might not support the paletted texture
	// extension any more, this is the alternative
	m_Colormapped = new FragmentProgramImpl;
	if (m_Colormapped->initRenderer())
	{
		return true;
	}
	// failed
	delete m_Colormapped;
	m_Colormapped = 0;
	// This should work on ATI's and NVidia cards
	m_Colormapped = new FragmentProgramARBImpl;
	if (m_Colormapped->initRenderer())
	{
		return true;
	}
	// failed
	delete m_Colormapped;
	m_Colormapped = 0;
	// try a 2d paletted version
	m_Colormapped = new Paletted2DImpl;
	if (m_Colormapped->initRenderer())
	{
		return true;
	}
	// failed
	delete m_Colormapped;
	m_Colormapped = 0;
	// next we try the sgi version
	m_Colormapped = new SGIColorTableImpl;
	if (m_Colormapped->initRenderer())
	{
		return true;
	}
	// failed, out of options
	delete m_Colormapped;
	m_Colormapped = 0;
	return false;
}

bool Renderer::initRGBARenderer()
{
	// this should work on most platforms
	m_RGBA = new SimpleRGBAImpl;
	if (m_RGBA->initRenderer())
	{
		return true;
	}
	// failed
	delete m_RGBA;
	m_RGBA = 0;
	m_RGBA = new SimpleRGBA2DImpl;
	if (m_RGBA->initRenderer())
	{
		return true;
	}
	// failed
	delete m_RGBA;
	m_RGBA = 0;
	return false;
}

Geometry* Renderer::getGeometry()
{
	if (m_UseColormapped)
	{
		return m_Colormapped->getGeometry();
	}
	else
	{
		return m_RGBA->getGeometry();
	}
}

void Renderer::prepareToRenderGeometry()
{
	if (m_UseColormapped)
	{
		m_Colormapped->prepareToRenderGeometry();
	}
	else
	{
		m_RGBA->prepareToRenderGeometry();
	}
}
