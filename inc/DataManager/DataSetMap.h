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
#if !defined(AFX_DATASETMAP_H__A8DD967A_48F5_4665_AD10_2C368A508801__INCLUDED_)
#define AFX_DATASETMAP_H__A8DD967A_48F5_4665_AD10_2C368A508801__INCLUDED_

const int GOA = 0;
const int SCALAR_VOLUME = 1;
const int VECTOR_VOLUME = 2;
const int SURFACE = 3;
const int PTS = 4;
const int NURB = 5;
const int SECONDARYSTRUCTURE = 6;

// Quite a useless class. It is used to interface with the arrays containing the data
// to the list box in the UI. It is quite an ugly way of doing it. Maybe change later.
class DataSetMap
{
	public:
		DataSetMap(int index, int dataType);
		virtual ~DataSetMap();
		virtual void setIndex(int index);
		virtual void setDataType(int dataType);
		virtual int getIndex();
		virtual int getDataType();

	protected:
		int m_Index;
		int m_DataType;
};

#endif
