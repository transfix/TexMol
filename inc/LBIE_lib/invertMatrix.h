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
typedef float MATRIX3[9];
typedef float MATRIX4[16];
typedef float VFLOAT;
void m4_submat(MATRIX4 mr, MATRIX3 mb, int i, int j);
VFLOAT m4_det(MATRIX4 mr);
int m4_inverse(MATRIX4 mr, MATRIX4 ma);
VFLOAT m3_det(MATRIX3 mat);
