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
#if !defined(AFX_BOUNDINGBOX_H__0D59E12F_63D8_42D5_9903_C120BEB15316__INCLUDED_)
#define AFX_BOUNDINGBOX_H__0D59E12F_63D8_42D5_9903_C120BEB15316__INCLUDED_

class BoundingBox
{
	public:
		BoundingBox();
		virtual ~BoundingBox();
		bool render();
		void reset();
		bool setMinMax(float* min, float* max);
		bool expandMinMax(float* min, float* max);
		bool getMinMax(float* min, float* max) const;

	protected:
		bool m_Initialized;
		float m_Min[3];
		float m_Max[3];
};

#endif
