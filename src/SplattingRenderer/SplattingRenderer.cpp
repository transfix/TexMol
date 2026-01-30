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
#include <ImposterRenderer/ImposterRenderer.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <SplattingRenderer/SplattingRenderer.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#if defined(__APPLE__)
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif

using CCVSplattingRenderer::SplattingRenderer;

SplattingRenderer::SplattingRenderer(int octreeLeafSize)
{
	if (octreeLeafSize >= 1)
	{
		m_OctreeLeafSize = octreeLeafSize;
	}
	else
	{
		m_OctreeLeafSize = 1;
	}
	m_CurrentSimpleVolumeData = 0;
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	m_ImposterRenderer = new CCVImposterRenderer::ImposterRenderer;
	m_ImposterRenderer->initRenderer();
}

SplattingRenderer::~SplattingRenderer()
{
	m_CurrentSimpleVolumeData = 0;
}

bool SplattingRenderer::initRenderer()
{
	return true;
}

bool SplattingRenderer::setOctreeLeafSize(int octreeLeafSize)
{
	if (octreeLeafSize >= 1)
	{
		m_OctreeLeafSize = octreeLeafSize;
		return true;
	}
	return false;
}

bool SplattingRenderer::buildNewOctree()
{
	if (!m_CurrentSimpleVolumeData)
	{
		return false;
	}
	return true;
}

int SplattingRenderer::getColorMapIndex(int volumeIndex, int colorMapSize, double minVal, double maxVal)
{
	if (!m_CurrentSimpleVolumeData)
	{
		return 0;
	}
	double d = m_CurrentSimpleVolumeData->getValueAt(0, volumeIndex);
	if (d > 10)
	{
		//		printf("Something seems to work?\n");
	}
	return (int)((d-minVal)/(maxVal-minVal)*((double)colorMapSize));
}

bool SplattingRenderer::uploadColorMap(double* colorMap, int size)
{
	if (!colorMap || (size<1))
	{
		return false;
	}
	return renderCube(colorMap, size);
}

bool SplattingRenderer::renderCube(double* dColorMap, int colorMapSize)
{
	if (!dColorMap || (colorMapSize<1) || !m_CurrentSimpleVolumeData)
	{
		return false;
	}
	if (!m_ImposterRenderer)
	{
		return false;
	}
	if (!m_ImposterRenderer->m_BallRenderer)
	{
		return false;
	}
	m_ImposterRenderer->clear();
	m_ImposterRenderer->initSubRenderers(false);
	float spanx = m_CurrentSimpleVolumeData->getSpanX();
	float spany = m_CurrentSimpleVolumeData->getSpanY();
	float spanz = m_CurrentSimpleVolumeData->getSpanZ();
	float origx = m_CurrentSimpleVolumeData->getMinX();
	float origy = m_CurrentSimpleVolumeData->getMinY();
	float origz = m_CurrentSimpleVolumeData->getMinZ();
	int width  = m_CurrentSimpleVolumeData->getWidth();
	int height = m_CurrentSimpleVolumeData->getHeight();
	int depth  = m_CurrentSimpleVolumeData->getDepth();
	double maxVal = 1;
	double minVal = 0;
	if (!m_CurrentSimpleVolumeData->getMinMax(0, &minVal, &maxVal))
	{
		return false;
	}
	if (maxVal == minVal)
	{
		return false;
	}
	glEnable(GL_COLOR_MATERIAL);
	glPointSize(3.0);
	glBegin(GL_POINTS);
	int index = 0;
	float z = origz;
	for (int i=0; i<depth; i++)
	{
		float y = origy;
		for (int j=0; j<height; j++)
		{
			float x = origx;
			for (int k=0; k<width; k++)
			{
				int colorMapIndex = getColorMapIndex(index, colorMapSize, minVal, maxVal);
				double r = dColorMap[colorMapIndex*4+0];
				double g = dColorMap[colorMapIndex*4+1];
				double b = dColorMap[colorMapIndex*4+2];
				double a = dColorMap[colorMapIndex*4+3];
				if (a > 0)
				{
					glColor4f((float) r, (float) g, (float) b, (float) a);
					glVertex3f(x, y, z);
					m_ImposterRenderer->m_BallRenderer->addBall(x,y,z,0.5f,
							(float) r,
							(float) g,
							(float) b);
				}
				x += spanx;
				index++;
			}
			y += spany;
		}
		z += spanz;
	}
	glEnd();
fprintf( stderr, "ball added\n");
	return true;
}

bool SplattingRenderer::render(double* dColorMap, int colorMapSize)
{
	if (!dColorMap || (colorMapSize<1) || !m_CurrentSimpleVolumeData)
	{
		return false;
	}
	if (m_CurrentSimpleVolumeData->getNumberOfVariables() < 1)
	{
		return false;
	}
	m_ImposterRenderer->renderBuffer(
		true,
		false,
		0,
		0,
		0,
		false,
		1.0
	);
	return true;
}
