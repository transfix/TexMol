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
#if !defined(AFX_PROTEINDATA_H__2B20B1AC_C2EF_4CD8_BF1D_6820D7B91C80__INCLUDED_)
#define AFX_PROTEINDATA_H__2B20B1AC_C2EF_4CD8_BF1D_6820D7B91C80__INCLUDED_

#include <Utility/utility.h>

namespace PDBParser
{
	class ProteinData
	{
		public:
			ProteinData();
			ProteinData(const ProteinData& proteinData);
			virtual ~ProteinData();
	};
};

#endif
