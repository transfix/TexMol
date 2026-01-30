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
#ifndef CCV_TEXMOL_RAY_TRACER_INTERSECTOR_H
#define CCV_TEXMOL_RAY_TRACER_INTERSECTOR_H

#include <UsefulMath/Vector.h>
#include <UsefulMath/Ray.h>

namespace CCVRayTracer
{
	class Intersector
	{
		public:
			Intersector();
			virtual ~Intersector();
			static bool intersectSphere();
			static bool intersectTriangle(CCVOpenGLMath::Vector p1, CCVOpenGLMath::Vector p2, CCVOpenGLMath::Vector p3, CCVOpenGLMath::Ray r);
			static bool intersectBoundingBox();
			static bool intersectPlane(CCVOpenGLMath::Vector point,
									   CCVOpenGLMath::Vector normal,
									   CCVOpenGLMath::Ray ray,
									   double* t,
									   CCVOpenGLMath::Vector* intersectionPoint);
	};
}

#endif
