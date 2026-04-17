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
#include <TexMol/BitVector.h>

using namespace CCV;

BitVector::BitVector()
{
	m_NumberOfElements = 0;
	// set a bit to 1 to OR with
	// if you AND, you can check if a bit is set
	m_Table[0] = 1;
	m_Table[1] = 2;
	m_Table[2] = 4;
	m_Table[3] = 8;
	m_Table[4] = 16;
	m_Table[5] = 32;
	m_Table[6] = 64;
	m_Table[7] = 128;
}

BitVector::~BitVector()
{
}

BitVector::BitVector(const BitVector& copy)
{
	if (this!=&copy)
	{
		m_NumberOfElements = copy.m_NumberOfElements;
		m_BitStream = copy.m_BitStream;
	}
}

BitVector& BitVector::operator=(const BitVector& copy)
{
	if (this!=&copy)
	{
		m_NumberOfElements = copy.m_NumberOfElements;
		m_BitStream = copy.m_BitStream;
	}
	return *this;
}

void BitVector::push_back(bool value)
{
	int size = m_BitStream.size();
	int n =  size*8 - m_NumberOfElements;	// n should be between 0 to 7
	// add element if required
	if (n == 0)
	{
		m_BitStream.push_back((char)0);
	}
	m_NumberOfElements++;
	if (!value)
	{
		return; // done !
	}
	// need to OR last char with correct 00...1..00
	unsigned char lastCharacter = m_BitStream.at(size-1);
	lastCharacter = lastCharacter | m_Table[n];
	m_BitStream[size-1] = lastCharacter;
}

int BitVector::size()
{
	return m_NumberOfElements;
}

void BitVector::empty()
{
	m_BitStream.clear();
	m_NumberOfElements = 0;
}

bool BitVector::operator[](unsigned int i)
{
	if (i >= m_NumberOfElements)
	{
		return false;    // maybe someday throw exception
	}
	return m_BitStream[i/8] & m_Table[i%8];
}

const bool BitVector::operator[](unsigned int i) const
{
	if (i >= m_NumberOfElements)
	{
		return false;    // maybe someday throw exception
	}
	return m_BitStream[i/8] & m_Table[i%8];
}

bool BitVector::set(unsigned int i, bool value)
{
	if (i >= m_NumberOfElements)
	{
		return false;    // maybe someday throw exception
	}
	int characterCount = i/8;
	int bitCount = i%8;
	if (value)
	{
		unsigned char character = m_BitStream[characterCount] | (m_Table[bitCount]);
		m_BitStream[characterCount] = character;
	}
	else
	{
		unsigned char character = m_BitStream[characterCount] & (~(m_Table[bitCount]));
		m_BitStream[characterCount] = character;
	}
	return value;
}
