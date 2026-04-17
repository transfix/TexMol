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
#include <AssemblyUtils/MyMatrix.h>


MyMatrix::MyMatrix()
{
	set(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

MyMatrix::MyMatrix(
	double m00, double m01, double m02, double m03,
	double m10, double m11, double m12, double m13,
	double m20, double m21, double m22, double m23,
	double m30, double m31, double m32, double m33
)
{
	m[0][0]=m00;
	m[0][1]=m01;
	m[0][2]=m02;
	m[0][3]=m03;
	m[1][0]=m10;
	m[1][1]=m11;
	m[1][2]=m12;
	m[1][3]=m13;
	m[2][0]=m20;
	m[2][1]=m21;
	m[2][2]=m22;
	m[2][3]=m23;
	m[3][0]=m30;
	m[3][1]=m31;
	m[3][2]=m32;
	m[3][3]=m33;
}

MyMatrix::~MyMatrix()
{

}

MyMatrix::MyMatrix(const MyMatrix& copy)
{
	set(copy);
}

MyMatrix& MyMatrix::operator=(const MyMatrix& copy)
{
	return set(copy);
}


MyMatrix& MyMatrix::set(
	double m00, double m01, double m02, double m03,
	double m10, double m11, double m12, double m13,
	double m20, double m21, double m22, double m23,
	double m30, double m31, double m32, double m33
)
{
	m[0][0]=m00;
	m[0][1]=m01;
	m[0][2]=m02;
	m[0][3]=m03;
	m[1][0]=m10;
	m[1][1]=m11;
	m[1][2]=m12;
	m[1][3]=m13;
	m[2][0]=m20;
	m[2][1]=m21;
	m[2][2]=m22;
	m[2][3]=m23;
	m[3][0]=m30;
	m[3][1]=m31;
	m[3][2]=m32;
	m[3][3]=m33;
	return *this;
}

MyMatrix& MyMatrix::set(const MyMatrix& copy)
{
	if(this!=&copy)
	{
		set(
			copy.m[0][0], copy.m[0][1], copy.m[0][2], copy.m[0][3],
			copy.m[1][0], copy.m[1][1], copy.m[1][2], copy.m[1][3],
			copy.m[2][0], copy.m[2][1], copy.m[2][2], copy.m[2][3],
			copy.m[3][0], copy.m[3][1], copy.m[3][2], copy.m[3][3]
		);
	}
	return *this;
}

MyMatrix& MyMatrix::reset()
{
	set(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);
	return *this;
}


MyVector MyMatrix::operator*(MyVector& vec)
{
	return MyVector(
			   m[0][0]*vec[0]+m[0][1]*vec[1]+m[0][2]*vec[2]+m[0][3],
			   m[1][0]*vec[0]+m[1][1]*vec[1]+m[1][2]*vec[2]+m[1][3],
			   m[2][0]*vec[0]+m[2][1]*vec[1]+m[2][2]*vec[2]+m[2][3]
		   );
}

MyMatrix MyMatrix::operator*(MyMatrix& mat)
{
	MyMatrix ret;
	
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			ret.m[i][j] = 0.0;

			for(int k=0; k<4; k++)
			{
				ret.m[i][j] += m[i][k] * mat.m[k][j];
			}
		}
	}

	return ret;
}


MyMatrix MyMatrix::transpose()
{
	MyMatrix ret;
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			ret.set(j,i,m[i][j]);
	return ret;
}


void MyMatrix::print() const
{
	printf("m=[%8.3lf %8.3lf %8.3lf %8.3lf ]\n  [%8.3lf %8.3lf %8.3lf %8.3lf ]\n  [%8.3lf %8.3lf %8.3lf %8.3lf ]\n  [%8.3lf %8.3lf %8.3lf %8.3lf ]\n",
		   m[0][0], m[0][1], m[0][2], m[0][3],
		   m[1][0], m[1][1], m[1][2], m[1][3],
		   m[2][0], m[2][1], m[2][2], m[2][3],
		   m[3][0], m[3][1], m[3][2], m[3][3]);
}


MyMatrix MyMatrix::inverseAffineXform()
{
	MyMatrix B;

	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			B.set(i,j, m[j][i]);

	MyVector b(m[0][3], m[1][3], m[2][3]);

	MyVector Bb = B*b;

	MyMatrix inverse = B;
	inverse.set(0,3, -Bb[0]);
	inverse.set(1,3, -Bb[1]);
	inverse.set(2,3, -Bb[2]);

	return inverse;	
}


MyMatrix MyMatrix::alignWithZ(MyVector& vec)
{
	MyMatrix moveToXZ;
	MyMatrix moveToZ;

	double u, v, w;

	u = vec[0];
	v = vec[1];
	w = vec[2];

	double u2v2 = sqrt(u*u+v*v);
	double u2v2w2 = sqrt(u*u+v*v+w*w);

	moveToXZ.set(0,0, u/u2v2);
	moveToXZ.set(0,1, v/u2v2);
	moveToXZ.set(1,0, -v/u2v2);
	moveToXZ.set(1,1, u/u2v2);

	moveToZ.set(0,0, w/u2v2w2);
	moveToZ.set(0,2, -u2v2/u2v2w2);
	moveToZ.set(2,0, u2v2/u2v2w2);
	moveToZ.set(2,2, w/u2v2w2);

	MyMatrix combinedRotate = moveToZ*moveToXZ;

	return combinedRotate;
}



MyMatrix MyMatrix::alignZWith(MyVector& vec)
{
/*	MyMatrix xform = alignWithZ(vec);
	return xform.inverseAffineXform();*/

	MyMatrix moveToXYZ;
	MyMatrix moveToXZ;

	double u, v, w;

	u = vec[0];
	v = vec[1];
	w = vec[2];

	double u2v2 = sqrt(u*u+v*v);
	double u2v2w2 = sqrt(u*u+v*v+w*w);

	moveToXYZ.set(0,0, u/u2v2);
	moveToXYZ.set(0,1, -v/u2v2);
	moveToXYZ.set(1,0, v/u2v2);
	moveToXYZ.set(1,1, u/u2v2);

	moveToXYZ.print();

	moveToXZ.set(0,0, w/u2v2w2);
	moveToXZ.set(0,2, u2v2/u2v2w2);
	moveToXZ.set(2,0, -u2v2/u2v2w2);
	moveToXZ.set(2,2, w/u2v2w2);

	moveToXZ.print();

	MyMatrix combinedRotate = moveToXYZ*moveToXZ;

	return combinedRotate;
}



MyMatrix MyMatrix::alignWithZAndX(MyVector& PC1, MyVector& PC2)
{
	MyVector pos(0,0,0);
	return centerAndAlignWithZAndX(PC1, PC2, pos);
}



MyMatrix MyMatrix::alignZAndXWith(MyVector& PC1, MyVector& PC2)
{
	MyVector pos(0,0,0);
	return alignZAndXWithAndMove(PC1, PC2, pos);
}



MyMatrix MyMatrix::centerAndAlignWithZAndX(MyVector& PC1, MyVector& PC2, MyVector& pos)
{
	MyMatrix translateToOrigin;
	translateToOrigin.set(0,3,-pos[0]);
	translateToOrigin.set(1,3,-pos[1]);
	translateToOrigin.set(2,3,-pos[2]);

	MyMatrix moveToXZ;
	MyMatrix moveToZ;

	double u, v, w;

	u = PC1[0];
	v = PC1[1];
	w = PC1[2];

	double u2v2 = sqrt(u*u+v*v);
	double u2v2w2 = sqrt(u*u+v*v+w*w);

	moveToXZ.set(0,0, u/u2v2);
	moveToXZ.set(0,1, v/u2v2);
	moveToXZ.set(1,0, -v/u2v2);
	moveToXZ.set(1,1, u/u2v2);

	moveToZ.set(0,0, w/u2v2w2);
	moveToZ.set(0,2, -u2v2/u2v2w2);
	moveToZ.set(2,0, u2v2/u2v2w2);
	moveToZ.set(2,2, w/u2v2w2);

	MyMatrix combinedRotate = moveToZ*moveToXZ;

	// so far we have found how to translate to the origin and align the first vector to the z axis. Now we apply this to the other and then map it onto the XZ-plane

	MyMatrix temp1 = moveToXZ*translateToOrigin;
	MyMatrix temp2 = moveToZ*temp1;

	MyVector PC3 = combinedRotate*PC2;

	MyMatrix moveToXZ2;

	u = PC3[0];
	v = PC3[1];
	w = PC3[2];

	u2v2 = sqrt(u*u+v*v);

	moveToXZ2.set(0,0, u/u2v2);
	moveToXZ2.set(0,1, v/u2v2);
	moveToXZ2.set(1,0, -v/u2v2);
	moveToXZ2.set(1,1, u/u2v2);

	// finally computing the overall transformation

	return moveToXZ2*temp2;
}



MyMatrix MyMatrix::alignZAndXWithAndMove(MyVector& PC1, MyVector& PC2, MyVector& pos)
{
	MyMatrix xform = centerAndAlignWithZAndX(PC1, PC2, pos);
	return xform.inverseAffineXform();
}


MyMatrix alignZAndXWithAndMoveAfterInitialRotateAroundZ(MyVector& vec1, MyVector& vec2, MyVector& pos, double angle)
{
	return MyMatrix();
}



MyMatrix MyMatrix::rotationZ(double angle)
{
	double ca = (double)cos(angle);
	double sa = (double)sin(angle);
	return MyMatrix(ca, -sa, 0.0, 0.0,
			sa, ca, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
}
