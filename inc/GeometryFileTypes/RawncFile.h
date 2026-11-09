/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of MolSurf.

  MolSurf is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.


  MolSurf is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#if !defined(AFX_RAWNCFILE_H__2755A9D8_A22A_496D_BF36_C7CF6FE93F6D__INCLUDED_)
#define AFX_RAWNCFILE_H__2755A9D8_A22A_496D_BF36_C7CF6FE93F6D__INCLUDED_

#include <GeometryFileTypes/GeometryFileType.h>

class RawncFile : public GeometryFileType
{
	public:
		virtual ~RawncFile();
		virtual Geometry* loadFile(const string& fileName);
		virtual bool checkType(const string& fileName);
		virtual bool saveFile(const Geometry* geometry, const string& fileName);
		virtual string extension()
		{
			return "rawnc";
		};
		virtual string filter()
		{
			return "Rawnc files (*.rawnc)";
		};
		static RawncFile ms_RawncFileRepresentative;
		static GeometryFileType* getRepresentative();
		bool saveMolCenters(const Geometry* geometry);
	protected:
		RawncFile();
};

#endif
