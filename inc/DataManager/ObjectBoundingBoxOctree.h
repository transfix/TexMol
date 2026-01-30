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
#ifndef CCV_TEXMOL_DATAMANAGER_OBJECT_BOUNDING_BOX_OCTREE_H
#define CCV_TEXMOL_DATAMANAGER_OBJECT_BOUNDING_BOX_OCTREE_H

#include <vector>

class ViewFrustrum;

class ObjectBoundingBoxOctree
{
	public:
		ObjectBoundingBoxOctree(double* min, double* max, int octreeLevel);
		virtual ~ObjectBoundingBoxOctree();
		bool addObject(int id, double* mid, double size, int octreeLevel);
		bool addObject(int id, double* obj_min, double* obj_max, int octreeLevel);
		void findObjects(bool* visible, double projectedAreaLimit, int octreeLevel, ViewFrustrum* viewFrustrum);

	protected:
		std::vector<int> m_Object;
		double m_Min[3];
		double m_Max[3];
		int m_MaxLevels;
		ObjectBoundingBoxOctree* m_Node1;
		ObjectBoundingBoxOctree* m_Node2;
		ObjectBoundingBoxOctree* m_Node3;
		ObjectBoundingBoxOctree* m_Node4;
		ObjectBoundingBoxOctree* m_Node5;
		ObjectBoundingBoxOctree* m_Node6;
		ObjectBoundingBoxOctree* m_Node7;
		ObjectBoundingBoxOctree* m_Node8;
};

#endif
