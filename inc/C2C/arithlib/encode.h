/*
  Copyright 2000-2003 The University of Texas at Austin

	Authors: Xiaoyu Zhang 2000-2002 <xiaoyu@ices.utexas.edu>
					 John Wiggins 2003 <prok@cs.utexas.edu>
	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of iotree.

  iotree is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  iotree is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef C2C_ENCODE_H
#define C2C_ENCODE_H
#ifdef WIN32
#include <C2C/arithlib/unitypes.h>
#else
#include <unistd.h>
#endif
#include <C2C/arithlib/arith.h>
#include <C2C/arithlib/bitbuffer.h>

/** isoval is provided as central of array values. it
    can be set as 0
*/
/// encode a float array of length nv
BitBuffer* encode_vals(float* vals, int nv, float isoval = 0);

/// encode a unsigned short array of length nv
BitBuffer* encode_vals(u_short* vals, int nv, float isoval = 0);

/// encode a unsigned char array of length nv
BitBuffer* encode_vals(u_char* vals, int nv, float isoval = 0);

void en_second_diff(BitBuffer*, float* vals, int nv);

/// decode a float array of length nv
bool  decode_vals(BitBuffer*, float* vals, int nv, float isoval = 0);

/// decode a unsigned short array of length nv
bool  decode_vals(BitBuffer*, u_short* vals, int nv, float isoval = 0);

/// decode a unsigned char array of length nv
bool  decode_vals(BitBuffer*, u_char* vals, int nv, float isoval = 0);


#endif

