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
#if !defined(AFX_POLYGONARRAY_H__9F01EA71_FDFE_47B4_A507_826834CEF66A__INCLUDED_)
#define AFX_POLYGONARRAY_H__9F01EA71_FDFE_47B4_A507_826834CEF66A__INCLUDED_

namespace OpenGLVolumeRendering
{
	class Polygon;

	class PolygonArray
	{
		public:
			PolygonArray(unsigned int sizeGuess);
			virtual ~PolygonArray();
			void clearPolygons();
			void addPolygon(const Polygon& polygon);
			Polygon* getPolygon(unsigned int i);
			unsigned int getNumPolygons();

		protected:
			void doubleArray();
			void allocateArray(unsigned int sizeGuess);
			Polygon* m_PolygonArray;
			unsigned int m_ArraySize;
			unsigned int m_NumPolygons;
	};
};

#endif
