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
#ifndef CCV_TEXMOL_RAY_TRACER_SIMPLE_SAMPLER_H
#define CCV_TEXMOL_RAY_TRACER_SIMPLE_SAMPLER_H

#include <RayTracer/Sampler.h>
#include <UsefulMath/Vector.h>

namespace CCVRayTracer
{

	class SimpleSampler : public Sampler
	{
		public:
			SimpleSampler(View* view);
			virtual ~SimpleSampler();
			virtual void reset();
			virtual CCVOpenGLMath::Ray* nextRay();
			virtual void addSample(ColorManager::Color color);

		protected:
			int m_XPos;
			int m_YPos;
			CCVOpenGLMath::Vector m_Origin;
			CCVOpenGLMath::Vector m_LookAt;
			CCVOpenGLMath::Vector m_UImage;
			CCVOpenGLMath::Vector m_VImage;
			double m_RealWidth;
			double m_RealHeight;
	};
};

#endif
