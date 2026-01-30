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
#include <OpenGL_Viewer/View.h>
#include <RayTracer/Intersector.h>
#include <RayTracer/RayTracer.h>
#include <RayTracer/Sampler.h>
#include <RayTracer/SimpleSampler.h>
#include <UsefulMath/Ray.h>

using namespace CCVRayTracer;

RayTracer::RayTracer(View* view, LightSet* lightSet, DataManager* dataManager)
{
	m_View = view;
	m_LightSet = lightSet;
	m_DataManager = dataManager;
	m_Sampler = new SimpleSampler(m_View);
}

RayTracer::~RayTracer()
{
	delete m_Sampler;
}

float* RayTracer::trace()
{
	m_Sampler->reset();
	while (true)
	{
		CCVOpenGLMath::Ray* ray = m_Sampler->nextRay();
		if (!ray)
		{
			break;
		}
		ColorManager::Color color = trace(ray);
		delete ray;
		m_Sampler->addSample(color);
	}
	return m_Sampler->getImage();
}

ColorManager::Color RayTracer::trace(CCVOpenGLMath::Ray* ray)
{
	ColorManager::Color color;
	// assume one triangle, from [0,0,0] [10,0,0] [0,10,0]
	if (Intersector::intersectTriangle(CCVOpenGLMath::Vector(0,0,0,0),
									   CCVOpenGLMath::Vector(10,0,0,0),
									   CCVOpenGLMath::Vector(0,10,0,0),
									   *ray))
	{
		return ColorManager::Color(1,0,0,1);
	}
	return ColorManager::Color(0,0,0,1);
}
