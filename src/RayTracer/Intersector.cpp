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
#include <RayTracer/Intersector.h>

using CCVOpenGLMath::Vector;
using CCVOpenGLMath::Ray;

using namespace CCVRayTracer;

static const double eps = 0.00001;

Intersector::Intersector()
{
}

Intersector::~Intersector()
{
}

bool Intersector::intersectSphere()
{
	return false;
}

bool Intersector::intersectTriangle(CCVOpenGLMath::Vector p1, CCVOpenGLMath::Vector p2, CCVOpenGLMath::Vector p3, CCVOpenGLMath::Ray ray)
{
	Vector edge1 = p2-p1;
	Vector edge2 = p3-p1;
	Vector h = ray.m_Dir.cross(edge2);
	double a = edge1.dot(h);
	if (a > -1*eps && a < eps)
	{
		return false;
	}
	double f = 1.0/a;
	Vector s = ray.m_Origin-p1;
	double u = f * (s.dot(h));
	if (u < 0.0 || u > 1.0)
	{
		return false;
	}
	Vector q = s.cross(edge1);
	double v = f * (ray.m_Dir.dot(q));
	if (v < 0.0 || u + v > 1.0)
	{
		return false;
	}
	double t = f * (edge2.dot(q));
	if (t > 0)
	{
		return true;
	}
	return false;
}

bool Intersector::intersectBoundingBox()
{
	return false;
}

bool Intersector::intersectPlane(CCVOpenGLMath::Vector point, CCVOpenGLMath::Vector normal, CCVOpenGLMath::Ray ray, double* t, CCVOpenGLMath::Vector* intersectionPoint)
{
	return true;
}
