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
#ifndef CCV_BIT_VECTOR_H
#define CCV_BIT_VECTOR_H

#include <vector>

namespace CCV
{
	class BitVector
	{
		public:
			BitVector();
			virtual ~BitVector();
			BitVector(const BitVector& copy);
			BitVector& operator=(const BitVector& copy);
			void push_back(bool value);
			int size();
			void empty();
			bool operator[](unsigned int i);
			const bool operator[](unsigned int i) const;
			bool set(unsigned int i, bool value);
			int m_NumberOfElements;
			std::vector<unsigned char> m_BitStream;

		protected:
			unsigned char m_Table[8];
	};
};

#endif
