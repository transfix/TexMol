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
#ifndef CCV_TEXMOL_RAY_TRACER_H
#define CCV_TEXMOL_RAY_TRACER_H

class View;
class LightSet;
class DataManager;

namespace CCVOpenGLMath
{
	class Ray;
};

#include <ColorManager/Color.h>

namespace CCVRayTracer
{
	class Buffer;
	class Sampler;

	class RayTracer
	{
		public:
			RayTracer(View* view, LightSet* lightSet, DataManager* dataManager);
			virtual ~RayTracer();
			float* trace();

		protected:
			ColorManager::Color trace(CCVOpenGLMath::Ray* ray);
			View* m_View;
			LightSet* m_LightSet;
			DataManager* m_DataManager;
			Sampler* m_Sampler;
	};
};

#endif
