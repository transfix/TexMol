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
#ifndef EXPANDABLEBUFFER_H
#define EXPANDABLEBUFFER_H

template <class T>
class ExpandableBuffer
{
	public:
		ExpandableBuffer(unsigned int size = 1024);
		~ExpandableBuffer();
		bool allocateBuffer(unsigned int size);
		T get(int index) const;
		bool add(T object);
		void clearBuffer();
		bool tighten();
		unsigned int getNumObjects() const;
		T* getBuffer();

	protected:
		bool forceAllocate(unsigned int size);
		bool doubleBuffer();
		T* m_Buffer;
		unsigned int m_BufferSize;
		unsigned int m_NumberOfObjects;
};

template <class T>
ExpandableBuffer <T>::ExpandableBuffer(unsigned int size)
{
	m_BufferSize = 0;
	m_NumberOfObjects = 0;
	m_Buffer = 0;
	allocateBuffer(size);
}

template <class T>
ExpandableBuffer <T>::~ExpandableBuffer()
{
	delete [] m_Buffer;
	m_Buffer = 0;
}

template <class T>
bool ExpandableBuffer <T>::allocateBuffer(unsigned int size)
{
	if (size<=m_BufferSize)
	{
		clearBuffer();
		return true;
	}
	else
	{
		return forceAllocate(size);
	}
}

template <class T>
T ExpandableBuffer <T>::get(int index) const
{
	if (index < 0)
	{
		return NULL;
	}
	if (m_NumberOfObjects <= (unsigned int)index)
	{
		return NULL;
	}
	return m_Buffer[index];
}

template <class T>
bool ExpandableBuffer <T>::add(T object)
{
	if (m_NumberOfObjects==m_BufferSize)
	{
		if (!doubleBuffer())
		{
			return false;
		}
	}
	m_Buffer[m_NumberOfObjects] = object;
	m_NumberOfObjects++;
	return true;
}

template <class T>
void ExpandableBuffer <T>::clearBuffer()
{
	m_NumberOfObjects = 0;
}

template <class T>
bool ExpandableBuffer <T>::tighten()
{
	T* temp = new T[m_NumberOfObjects];
	if (!temp)
	{
		return false;
	}
	unsigned int c;
	for (c=0; c<m_NumberOfObjects; c++)
	{
		temp[c] = m_Buffer[c];
	}
	delete [] m_Buffer;
	m_Buffer = temp;
	m_BufferSize=m_NumberOfObjects;
}

template <class T>
unsigned int ExpandableBuffer <T>::getNumObjects() const
{
	return m_NumberOfObjects;
}

template <class T>
T* ExpandableBuffer <T>::getBuffer()
{
	return m_Buffer;
}

template <class T>
bool ExpandableBuffer <T>::forceAllocate(unsigned int size)
{
	delete [] m_Buffer;
	m_Buffer = new T[size];
	if (!m_Buffer)
	{
		m_BufferSize = 0;
		m_NumberOfObjects = 0;
		return false;
	}
	else
	{
		m_BufferSize = size;
		m_NumberOfObjects = 0;
		return true;
	}
}

template <class T>
bool ExpandableBuffer <T>::doubleBuffer()
{
	T* temp = new T[(m_BufferSize*3)/2];
	if (!temp)
	{
		return false;
	}
	for (int c=0; c<m_NumberOfObjects; c++)
	{
		temp[c] = m_Buffer[c];
	}
	delete [] m_Buffer;
	m_Buffer = temp;
	m_BufferSize=(m_BufferSize*3)/2;
	return true;
}

#endif
