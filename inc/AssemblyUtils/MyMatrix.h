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

#ifndef MY_MATRIX_H
#define MY_MATRIX_H

#include <AssemblyUtils/MyVector.h>

class MyMatrix
{
	public:
		MyMatrix();
		MyMatrix(
			double m00, double m01, double m02, double m03,
			double m10, double m11, double m12, double m13,
			double m20, double m21, double m22, double m23,
			double m30, double m31, double m32, double m33
		);
		virtual ~MyMatrix();
		MyMatrix(const MyMatrix& copy);
		MyMatrix& operator=(const MyMatrix& copy);
		void print() const;
		MyMatrix& set(
			double m00, double m01, double m02, double m03,
			double m10, double m11, double m12, double m13,
			double m20, double m21, double m22, double m23,
			double m30, double m31, double m32, double m33
		);
		MyMatrix& set(const MyMatrix& copy);
		MyMatrix& reset();
		inline double get(int row, int column) const;
		inline void set(int row, int column, double value);
		MyVector operator*( MyVector& vec) ;
		MyMatrix operator*( MyMatrix& mat) ;
		MyMatrix transpose();

		MyMatrix inverseAffineXform();	// rotation+translation (no scale)

		static MyMatrix alignWithZ(MyVector& vec);	// computes the xform for aligning the given vector to the positive Z axis
		static MyMatrix alignZWith(MyVector& vec);	// computes the xform for aligning the positive Z axis to the given vector

		static MyMatrix alignWithZAndX(MyVector& vec1, MyVector& vec2);

		static MyMatrix alignZAndXWith(MyVector& vec1, MyVector& vec2);

		static MyMatrix centerAndAlignWithZAndX(MyVector& vec1, MyVector& vec2, MyVector& pos);

		static MyMatrix alignZAndXWithAndMove(MyVector& vec1, MyVector& vec2, MyVector& pos);


		static MyMatrix rotationZ(double angle);
	
	protected:
		double m[4][4];
};

inline double MyMatrix::get(int row, int column) const
{
	return m[row][column];
}

inline void MyMatrix::set(int row, int column, double value)
{
        m[row][column] = value;
}

#endif
