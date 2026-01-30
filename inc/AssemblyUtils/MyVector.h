/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of TexMol.

  TexMol is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.


  TexMol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with TexMol; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef MY_VECTOR_H
#define MY_VECTOR_H


class MyVector
{
	public:
		MyVector(double x, double y, double z);
		MyVector();
		virtual ~MyVector();
		MyVector(const MyVector& copy);
		MyVector& operator=(const MyVector& copy);
		MyVector& set(double x, double y, double z);
		double dot(const MyVector& vec) const;
		MyVector cross(const MyVector& vec) const;
		MyVector& normalize();
		double& operator[](unsigned int i);
		void print() const;

	protected:
		double v[3];
};

#endif
