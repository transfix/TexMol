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
#include <ColorTable2/ColorTable.h>
//#include <DataManager/VolumeDataManager/VolumeRenderingProperties.h>

#include <DataManager/VolumeDataManager/SplattingRendererExtn.h>
#include <DataManager/VolumeDataManager/VolumeData.h>

using namespace CCVSplattingRenderer;
SplattingRendererExtn::SplattingRendererExtn(int octreeLeafSize) : SplattingRenderer(octreeLeafSize)
{
}

SplattingRendererExtn::~SplattingRendererExtn()
{
}

bool SplattingRendererExtn::initRenderer()
{
	return true;
}

bool SplattingRendererExtn::checkAndInitNewVolumeData(VolumeData* volumeData)
{
	if(!volumeData)
	{
		return false;
	}

	if(!volumeData->m_SimpleVolumeData)
	{
		return false;
	}

	if(volumeData->m_SimpleVolumeData == m_CurrentSimpleVolumeData)
	{
		return true;
	}

	m_CurrentSimpleVolumeData = volumeData->m_SimpleVolumeData;

	if(!buildNewOctree())
	{
		return false;
	}

	return true;
}

bool SplattingRendererExtn::renderVolume(VolumeData* volumeData)
{
	if(!volumeData)
	{
		return false;
	}

	if(!volumeData->m_VolumeRenderingProperties)
	{
		return false;
	}

	if(!volumeData->m_VolumeRenderingProperties->m_ColorTable)
	{
		return false;
	}

	// Get the color table values
	int colorMapSize = 256; // new rover supports only 256  - be careful SKVINAY !
	double dColorMap[256*4];


	// arand: FIXME
	// need to reimplement this in the VolRover version of ColorTable2
	//volumeData->m_VolumeRenderingProperties->m_ColorTable->getTableDouble(dColorMap, colorMapSize);

	// Done getting values
	if(!checkAndInitNewVolumeData(volumeData))
	{
		return false;
	}

	render(dColorMap, colorMapSize);
	return true;
}
