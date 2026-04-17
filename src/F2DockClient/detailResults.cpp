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
#include <F2DockClient/detailResults.h>

using namespace std;

DetailResults::DetailResults(bool ido)
{
	_isDockingOutput = ido;
	
	init();
}	

DetailResults::DetailResults()
{
	_isDockingOutput = true;
	
	init();
}

DetailResults::~DetailResults()
{

}	


bool DetailResults::init()
{
	rank = -1;
	newRank = -1;
	score = 0.0;
	newScore = 0.0;

	shape = 0.0;
	ssr = 0.0;
	ccr = 0.0;
	scr = 0.0;

	elec = 0.0;
	hbond = 0.0;
	hydro = 0.0;
	scc = 0.0;

	vdwScore = 0.0;
	clashes = 0.0;
	pgsolScore = 0.0;
	pgsolhScore = 0.0;
	deldispeScore = 0.0;

	delGpol = 0.0;;
	areaProp = 0.0;;

	conf = -1;
	rmsd = 0.0;;
	return true;
}	


CCVOpenGLMath::Matrix DetailResults::getTransformationMatrix()
{
	CCVOpenGLMath::Matrix mtx;

	mtx.set(0,0, mat[0]);
	mtx.set(0,1, mat[1]);
	mtx.set(0,2, mat[2]);
	mtx.set(0,3, mat[3]);
	mtx.set(1,0, mat[4]);
	mtx.set(1,1, mat[5]);
	mtx.set(1,2, mat[6]);
	mtx.set(1,3, mat[7]);
	mtx.set(2,0, mat[8]);
	mtx.set(2,1, mat[9]);
	mtx.set(2,2, mat[10]);
	mtx.set(2,3, mat[11]);
	mtx.set(3,0, 0);
	mtx.set(3,1, 0);
	mtx.set(3,2, 0);
	mtx.set(3,3, 1);

	return mtx;
}

