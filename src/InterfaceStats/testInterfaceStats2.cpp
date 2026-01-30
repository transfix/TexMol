/*
  Copyright 2011 The University of Texas at Austin

	Authors: Muhibur Rasheed <muhibur@ices.utexas.edu>
	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of MolSurf.

  MolSurf is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.


  MolSurf is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with MolSurf; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <InterfaceStats/ComputeInterfaceStats.h>
#include <UsefulMath/Matrix.h>

int main(int argc, char** argv)
{
	if(argc<2)
	{
		printf("Usage: testInterfaceStats2 <mol1.pqr> <mol2.pqr> \n\n (output will be printed to standard out. use redirection if you prefer)\n\n");
		return -1;
	}

	return 0;
}
