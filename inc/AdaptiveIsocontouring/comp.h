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
// This is the header for 4 different source files:
//   comp.cpp, process.cpp, utility.cpp, decimation.cpp

#ifndef CCV_COMP_H_
#define CCV_COMP_H_

#include <Utility/utility.h>
#include <AdaptiveIsocontouring/Cell.h>
#include <Contour/CellQueue.h>

const float ft_PI = 4 * 3 / PI;

typedef struct _vtx
{
	float x;
	float y;
	float z;
} Vtx;

// bishoulder.cpp bishoulder computation
void GetBishoulder(float* val , Vtx& bishoulder , float isovalue) ;
void Norm2Read_Bishoulder2(Vtx normalized_bishoulder,float* bishoulder, float xyz[3], float cell_size[3]);
bool is_ambiguous(float* val, float iso_val);
void copyVect(const float* in, float* out, int size);
void copyVect(const int* in, int* out, int size);
void copyVect(const double* in, double* out, int size);

#endif
