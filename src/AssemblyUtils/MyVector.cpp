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

#include <cstdio>
#include <cmath>
#include <AssemblyUtils/MyVector.h>


using namespace std;

MyVector::MyVector(double x, double y, double z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

MyVector::MyVector()
{
	v[0] = 0.0;
	v[1] = 0.0;
	v[2] = 0.0;
}

MyVector::~MyVector()
{

}

MyVector::MyVector(const MyVector& copy)
{
	v[0] = copy.v[0];
	v[1] = copy.v[1];
	v[2] = copy.v[2];
}

MyVector& MyVector::operator=(const MyVector& copy)
{
	v[0] = copy.v[0];
	v[1] = copy.v[1];
	v[2] = copy.v[2];
	return *this;
}

MyVector& MyVector::set(double x, double y, double z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
	return *this;
}

double MyVector::dot(const MyVector& vec) const
{
	return v[0]*vec.v[0] + v[1]*vec.v[1] + v[2]*vec.v[2];
}

MyVector MyVector::cross(const MyVector& vec) const
{
	return MyVector(
			   v[1]*vec.v[2] - v[2]*vec.v[1],
			   v[2]*vec.v[0] - v[0]*vec.v[2],
			   v[0]*vec.v[1] - v[1]*vec.v[0]
		   );
}

MyVector& MyVector::normalize()
{
	double length = (double)sqrt((double)(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]));
	return set(v[0]/length,v[1]/length,v[2]/length);	
}

double& MyVector::operator[](unsigned int i)
{
	return v[i];
}

void MyVector::print() const
{
	printf("[%lf %lf %lf]\n", v[0], v[1], v[2]);
}
