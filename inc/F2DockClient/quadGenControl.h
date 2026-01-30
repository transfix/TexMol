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
#ifndef QUADGEN_CONTROL
#define QUADGEN_CONTROL

#include <F2DockClient/quadGenParams.h>
#include <F2DockClient/basicParams.h>


class QuadGenControl
{
	private:
	QuadGenParams *quadPar;
	BasicParams *basPar;

	public:
	QuadGenControl();
	~QuadGenControl();
	
	void init();

	bool handleJob(bool r);

	//set Methods
	void setQuadGenParams(QuadGenParams *fp) {quadPar = fp;}
	void setBasicParams(BasicParams *bp) {basPar = bp;}

	bool setReceptorPDBName(string fileName);
	bool setLigandPDBName(string fileName);
	bool setResolution(int index);
	bool setJobTitle(string name);

	bool setParamsFromFile(string fileName);
};

#endif
