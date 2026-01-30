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
#include <DataManager/ObjectBoundingBoxOctree.h>
#include <OpenGL_Viewer/ViewFrustrum.h>

ObjectBoundingBoxOctree::ObjectBoundingBoxOctree(double* min, double* max, int numLevels)
{
	m_Node1 = 0;
	m_Node2 = 0;
	m_Node3 = 0;
	m_Node4 = 0;
	m_Node5 = 0;
	m_Node6 = 0;
	m_Node7 = 0;
	m_Node8 = 0;
	m_Min[0] = min[0];
	m_Min[1] = min[1];
	m_Min[2] = min[2];
	m_Max[0] = max[0];
	m_Max[1] = max[1];
	m_Max[2] = max[2];
	m_MaxLevels = numLevels;
}

ObjectBoundingBoxOctree::~ObjectBoundingBoxOctree()
{
	delete m_Node1;
	m_Node1 = 0;
	delete m_Node2;
	m_Node2 = 0;
	delete m_Node3;
	m_Node3 = 0;
	delete m_Node4;
	m_Node4 = 0;
	delete m_Node5;
	m_Node5 = 0;
	delete m_Node6;
	m_Node6 = 0;
	delete m_Node7;
	m_Node7 = 0;
	delete m_Node8;
	m_Node8 = 0;
	m_Object.clear();
}

bool intersects(double* mid, double size, double* min, double* max)
{
	double dmin = 0;
	for (int i = 0; i < 3; i++)
	{
		if (mid[i] < min[i])
		{
			dmin += (mid[i] - min[i])*(mid[i] - min[i]);
		}
		else if (mid[i] > max[i])
		{
			dmin += (mid[i] - max[i])*(mid[i] - max[i]);
		}
	}
	if (dmin <= size*size)
	{
		return true;
	}
	return false;
}

bool intersects(double* obj_min, double* obj_max, double* min, double* max)
{
	if (obj_min[0] > max[0])
	{
		return false;
	}
	if (obj_max[0] < min[0])
	{
		return false;
	}
	if (obj_min[1] > max[1])
	{
		return false;
	}
	if (obj_max[1] < min[1])
	{
		return false;
	}
	if (obj_min[2] > max[2])
	{
		return false;
	}
	if (obj_max[2] < min[2])
	{
		return false;
	}
	return true;
}

void ObjectBoundingBoxOctree::findObjects(bool* visible, double projectedAreaLimit, int octreeLevel, ViewFrustrum* viewFrustrum)
{
	// if we are in orthographic mode, just do view frustrum culling. Ensure leaf cube is large enough just once.
	// in perspective mode, also check for area of each cube
	if (viewFrustrum->intersects(m_Min, m_Max) == ViewFrustrum::OUTSIDE)
	{
		return;
	}
	if (octreeLevel == m_MaxLevels)
	{
		for (int i=0; i<m_Object.size(); i++)
		{
			visible[m_Object[i]] = true;
		}
	}
	else
	{
		if (m_Node1)
		{
			m_Node1->findObjects(visible, projectedAreaLimit, octreeLevel+1, viewFrustrum);
		}
		if (m_Node2)
		{
			m_Node2->findObjects(visible, projectedAreaLimit, octreeLevel+1, viewFrustrum);
		}
		if (m_Node3)
		{
			m_Node3->findObjects(visible, projectedAreaLimit, octreeLevel+1, viewFrustrum);
		}
		if (m_Node4)
		{
			m_Node4->findObjects(visible, projectedAreaLimit, octreeLevel+1, viewFrustrum);
		}
		if (m_Node5)
		{
			m_Node5->findObjects(visible, projectedAreaLimit, octreeLevel+1, viewFrustrum);
		}
		if (m_Node6)
		{
			m_Node6->findObjects(visible, projectedAreaLimit, octreeLevel+1, viewFrustrum);
		}
		if (m_Node7)
		{
			m_Node7->findObjects(visible, projectedAreaLimit, octreeLevel+1, viewFrustrum);
		}
		if (m_Node8)
		{
			m_Node8->findObjects(visible, projectedAreaLimit, octreeLevel+1, viewFrustrum);
		}
	}
}

bool ObjectBoundingBoxOctree::addObject(int id, double* center, double radius, int octreeLevel)
{
	if (octreeLevel == m_MaxLevels)
	{
		m_Object.push_back(id);
		return true;
	}
	else
	{
		// add to children.
		double m_Mid[3];
		m_Mid[0] = (m_Max[0] + m_Min[0])/2.0;
		m_Mid[1] = (m_Max[1] + m_Min[1])/2.0;
		m_Mid[2] = (m_Max[2] + m_Min[2])/2.0;
		double min[3], max[3];
		bool intersected[8];
		min[0] = m_Min[0];
		min[1] = m_Min[1];
		min[2] = m_Min[2];
		max[0] = m_Mid[0];
		max[1] = m_Mid[1];
		max[2] = m_Mid[2];
		if (intersects(center, radius, min, max))
		{
			if (!m_Node1)
			{
				m_Node1 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[0] = m_Node1->addObject(id, center, radius, octreeLevel+1);
		}
		min[0] = m_Mid[0];
		min[1] = m_Min[1];
		min[2] = m_Min[2];
		max[0] = m_Max[0];
		max[1] = m_Mid[1];
		max[2] = m_Mid[2];
		if (intersects(center, radius, min, max))
		{
			if (!m_Node2)
			{
				m_Node2 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[1] = m_Node2->addObject(id, center, radius, octreeLevel+1);
		}
		min[0] = m_Min[0];
		min[1] = m_Mid[1];
		min[2] = m_Min[2];
		max[0] = m_Mid[0];
		max[1] = m_Max[1];
		max[2] = m_Mid[2];
		if (intersects(center, radius, min, max))
		{
			if (!m_Node3)
			{
				m_Node3 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[2] = m_Node3->addObject(id, center, radius, octreeLevel+1);
		}
		min[0] = m_Mid[0];
		min[1] = m_Mid[1];
		min[2] = m_Min[2];
		max[0] = m_Max[0];
		max[1] = m_Max[1];
		max[2] = m_Mid[2];
		if (intersects(center, radius, min, max))
		{
			if (!m_Node4)
			{
				m_Node4 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[3] = m_Node4->addObject(id, center, radius, octreeLevel+1);
		}
		min[0] = m_Min[0];
		min[1] = m_Min[1];
		min[2] = m_Mid[2];
		max[0] = m_Mid[0];
		max[1] = m_Mid[1];
		max[2] = m_Max[2];
		if (intersects(center, radius, min, max))
		{
			if (!m_Node5)
			{
				m_Node5 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[4] = m_Node5->addObject(id, center, radius, octreeLevel+1);
		}
		min[0] = m_Mid[0];
		min[1] = m_Min[1];
		min[2] = m_Mid[2];
		max[0] = m_Max[0];
		max[1] = m_Mid[1];
		max[2] = m_Max[2];
		if (intersects(center, radius, min, max))
		{
			if (!m_Node6)
			{
				m_Node6 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[5] = m_Node6->addObject(id, center, radius, octreeLevel+1);
		}
		min[0] = m_Min[0];
		min[1] = m_Mid[1];
		min[2] = m_Mid[2];
		max[0] = m_Mid[0];
		max[1] = m_Max[1];
		max[2] = m_Max[2];
		if (intersects(center, radius, min, max))
		{
			if (!m_Node7)
			{
				m_Node7 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[6] = m_Node7->addObject(id, center, radius, octreeLevel+1);
		}
		min[0] = m_Mid[0];
		min[1] = m_Mid[1];
		min[2] = m_Mid[2];
		max[0] = m_Max[0];
		max[1] = m_Max[1];
		max[2] = m_Max[2];
		if (intersects(center, radius, min, max))
		{
			if (!m_Node8)
			{
				m_Node8 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[7] = m_Node8->addObject(id, center, radius, octreeLevel+1);
		}
		if (intersected[0] || intersected[1] || intersected[2] || intersected[3] ||
				intersected[4] || intersected[5] || intersected[6] || intersected[7])
		{
			return true;
		}
	}
	return false;
}

bool ObjectBoundingBoxOctree::addObject(int id, double* obj_min, double* obj_max, int octreeLevel)
{
	if (octreeLevel == m_MaxLevels)
	{
		m_Object.push_back(id);
		return true;
	}
	else
	{
		// add to children.
		double m_Mid[3];
		m_Mid[0] = (m_Max[0] + m_Min[0])/2.0;
		m_Mid[1] = (m_Max[1] + m_Min[1])/2.0;
		m_Mid[2] = (m_Max[2] + m_Min[2])/2.0;
		double min[3], max[3];
		bool intersected[8];
		min[0] = m_Min[0];
		min[1] = m_Min[1];
		min[2] = m_Min[2];
		max[0] = m_Mid[0];
		max[1] = m_Mid[1];
		max[2] = m_Mid[2];
		if (intersects(obj_min, obj_max, min, max))
		{
			if (!m_Node1)
			{
				m_Node1 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[0] = m_Node1->addObject(id, obj_min, obj_max, octreeLevel+1);
		}
		min[0] = m_Mid[0];
		min[1] = m_Min[1];
		min[2] = m_Min[2];
		max[0] = m_Max[0];
		max[1] = m_Mid[1];
		max[2] = m_Mid[2];
		if (intersects(obj_min, obj_max, min, max))
		{
			if (!m_Node2)
			{
				m_Node2 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[1] = m_Node2->addObject(id, obj_min, obj_max, octreeLevel+1);
		}
		min[0] = m_Min[0];
		min[1] = m_Mid[1];
		min[2] = m_Min[2];
		max[0] = m_Mid[0];
		max[1] = m_Max[1];
		max[2] = m_Mid[2];
		if (intersects(obj_min, obj_max, min, max))
		{
			if (!m_Node3)
			{
				m_Node3 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[2] = m_Node3->addObject(id, obj_min, obj_max, octreeLevel+1);
		}
		min[0] = m_Mid[0];
		min[1] = m_Mid[1];
		min[2] = m_Min[2];
		max[0] = m_Max[0];
		max[1] = m_Max[1];
		max[2] = m_Mid[2];
		if (intersects(obj_min, obj_max, min, max))
		{
			if (!m_Node4)
			{
				m_Node4 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[3] = m_Node4->addObject(id, obj_min, obj_max, octreeLevel+1);
		}
		min[0] = m_Min[0];
		min[1] = m_Min[1];
		min[2] = m_Mid[2];
		max[0] = m_Mid[0];
		max[1] = m_Mid[1];
		max[2] = m_Max[2];
		if (intersects(obj_min, obj_max, min, max))
		{
			if (!m_Node5)
			{
				m_Node5 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[4] = m_Node5->addObject(id, obj_min, obj_max, octreeLevel+1);
		}
		min[0] = m_Mid[0];
		min[1] = m_Min[1];
		min[2] = m_Mid[2];
		max[0] = m_Max[0];
		max[1] = m_Mid[1];
		max[2] = m_Max[2];
		if (intersects(obj_min, obj_max, min, max))
		{
			if (!m_Node6)
			{
				m_Node6 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[5] = m_Node6->addObject(id, obj_min, obj_max, octreeLevel+1);
		}
		min[0] = m_Min[0];
		min[1] = m_Mid[1];
		min[2] = m_Mid[2];
		max[0] = m_Mid[0];
		max[1] = m_Max[1];
		max[2] = m_Max[2];
		if (intersects(obj_min, obj_max, min, max))
		{
			if (!m_Node7)
			{
				m_Node7 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[6] = m_Node7->addObject(id, obj_min, obj_max, octreeLevel+1);
		}
		min[0] = m_Mid[0];
		min[1] = m_Mid[1];
		min[2] = m_Mid[2];
		max[0] = m_Max[0];
		max[1] = m_Max[1];
		max[2] = m_Max[2];
		if (intersects(obj_min, obj_max, min, max))
		{
			if (!m_Node8)
			{
				m_Node8 = new ObjectBoundingBoxOctree(min, max, m_MaxLevels);
			}
			intersected[7] = m_Node8->addObject(id, obj_min, obj_max, octreeLevel+1);
		}
		if (intersected[0] || intersected[1] || intersected[2] || intersected[3] ||
				intersected[4] || intersected[5] || intersected[6] || intersected[7])
		{
			return true;
		}
	}
	return false;
}
