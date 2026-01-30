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
#ifndef F2DGEN_CONTROL
#define F2DGEN_CONTROL


#include <F2DockClient/f2dGenParams.h>
#include <F2DockClient/basicParams.h>


class F2dGenControl
{
	private:
	F2dGenParams *f2dPar;
	BasicParams *basPar;

	public:
	F2dGenControl();
	~F2dGenControl();
	
	void init();
	
	bool handleJob(bool p);

	void setF2dGenParams(F2dGenParams *fp) {f2dPar = fp;}
	void setBasicParams(BasicParams *bp) {basPar = bp;}

	bool setReceptorPDBName(string fileName);
	bool setLigandPDBName(string fileName);
	bool setResolution(int index);
	bool setJobTitle(string name);

	bool setParamsFromFile(string fileName);
};

#endif
