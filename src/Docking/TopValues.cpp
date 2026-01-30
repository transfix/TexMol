#include <Docking/TopValues.h>
#include <Docking/ValuePosition3D.h>
#include <math.h>
#include <stdio.h>

// The constructor tells us how many of the top positions needs to be searched for.
TopValues::TopValues(int numberOfPositions, int gridSize)
{
	m_NumberOfPositions = numberOfPositions;
	m_CurrentNumberOfPositions = 0;
	m_GridSize = gridSize;
	m_ValuePosition3D = new ValuePosition3D[ numberOfPositions ];
	m_Heap = new AuxBufferHeap(numberOfPositions + 10000);
	int i;
	m_Heap->Find_Min(i, m_CurMin);
}

TopValues::~TopValues()
{
	delete [ ] m_ValuePosition3D;
	m_ValuePosition3D = 0;
	delete m_Heap;
	m_Heap = 0;
}

int TopValues::getCurrentNumberOfPositions()
{
	return m_CurrentNumberOfPositions;
}

int TopValues::getGridSize()
{
	return m_GridSize;
}

int TopValues::getCurMin()
{
	return m_CurMin;
}

inline void TopValues::deleteMin()
{
	if (m_CurrentNumberOfPositions > 0)
	{
		int i, v;
		m_Heap->Delete_Min(i, v);
		m_Heap->Find_Min(i, m_CurMin);
		m_CurrentNumberOfPositions--;
	}
}

void TopValues::extractMin(double* value, double* x, double* y, double* z, int* rotationIndex, int* fineRotationIndex, int* conformationIndex)
{
	if (m_CurrentNumberOfPositions > 0)
	{
		int i, v;
		m_Heap->Delete_Min(i, v);
		*value = m_ValuePosition3D[ i ].m_Value;
		*x = m_ValuePosition3D[ i ].m_Translation[ 0 ];
		*y = m_ValuePosition3D[ i ].m_Translation[ 1 ];
		*z = m_ValuePosition3D[ i ].m_Translation[ 2 ];
		*rotationIndex = m_ValuePosition3D[ i ].m_RotationIndex;
		*fineRotationIndex = m_ValuePosition3D[ i ].m_FineRotationIndex;
		*conformationIndex = m_ValuePosition3D[ i ].m_ConformationIndex;
		m_CurrentNumberOfPositions--;
	}
	else
	{
		*value = -1000000000;
	}
}

inline void TopValues::replaceMin(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex, int conformationIndex)
{
	if (m_CurrentNumberOfPositions > 0)
	{
		int i, v;
		m_Heap->Delete_Min(i, v);
		m_ValuePosition3D[ i ].m_Value = value;
		m_ValuePosition3D[ i ].m_Translation[ 0 ] = x;
		m_ValuePosition3D[ i ].m_Translation[ 1 ] = y;
		m_ValuePosition3D[ i ].m_Translation[ 2 ] = z;
		m_ValuePosition3D[ i ].m_RotationIndex = rotationIndex;
		m_ValuePosition3D[ i ].m_FineRotationIndex = fineRotationIndex;
		m_ValuePosition3D[ i ].m_ConformationIndex = conformationIndex;
		m_Heap->Insert(i, (int) value);
		m_Heap->Find_Min(i, m_CurMin);
	}
}

inline void TopValues::replaceMin(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex)
{
	replaceMin(value, x, y, z, rotationIndex, fineRotationIndex, 0);
}

inline void TopValues::insert(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex, int conformationIndex)
{
	if (m_CurrentNumberOfPositions < m_NumberOfPositions)
	{
		int i = m_CurrentNumberOfPositions++;
		m_ValuePosition3D[ i ].m_Value = value;
		m_ValuePosition3D[ i ].m_Translation[ 0 ] = x;
		m_ValuePosition3D[ i ].m_Translation[ 1 ] = y;
		m_ValuePosition3D[ i ].m_Translation[ 2 ] = z;
		m_ValuePosition3D[ i ].m_RotationIndex = rotationIndex;
		m_ValuePosition3D[ i ].m_FineRotationIndex = fineRotationIndex;
		m_ValuePosition3D[ i ].m_ConformationIndex = conformationIndex;
		m_Heap->Insert(i, (int) value);
	}
}

inline void TopValues::insert(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex)
{
	insert(value, x, y, z, rotationIndex, fineRotationIndex, 0);
}

inline void TopValues::insert(int valuePosIndex)
{
	if (m_CurrentNumberOfPositions < m_NumberOfPositions)
	{
		int v = (int) m_ValuePosition3D[ valuePosIndex ].m_Value;
		m_Heap->Insert(valuePosIndex, v);
		m_CurrentNumberOfPositions++;
	}
}

inline bool TopValues::updateList(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex, int conformationIndex)
{
	if (m_CurrentNumberOfPositions < m_NumberOfPositions)
	{
		insert(value, x, y, z, rotationIndex, fineRotationIndex, conformationIndex);
		if (value < m_CurMin)
		{
			m_CurMin = (int) value;
		}
		return true;
	}
	else
	{
		if (value > m_CurMin)
		{
			replaceMin(value, x, y, z, rotationIndex, fineRotationIndex, conformationIndex);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool TopValues::updateTopValues(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex, int conformationIndex)
{
	return updateList(value, x, y, z, rotationIndex, fineRotationIndex, conformationIndex);
}

inline bool TopValues::updateList(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex)
{
	return updateList(value, x, y, z, rotationIndex, fineRotationIndex, 0);
}

bool TopValues::updateTopValues(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex)
{
	return updateList(value, x, y, z, rotationIndex, fineRotationIndex);
}

// Output the values in a nicely formatted manner.
bool TopValues::print(FILE* fp, int standardGridSize, double functionScaleFactor, bool printConfId)
{
	if (!fp)
	{
		return false;
	}
	if (!m_ValuePosition3D || !m_Heap || (m_NumberOfPositions < 1) || (functionScaleFactor == 0))
	{
		return false;
	}
	double gridFactor = (double) standardGridSize / (double) m_GridSize;
	int k = m_CurrentNumberOfPositions, j;
	double value, x, y, z;
	int rotationIndex, fineRotationIndex, conformationIndex;
	int v, curMin = m_CurMin;
	for (int i = 0; i < k; i++)
	{
		m_Heap->Delete_Min(j, v);
		value =  m_ValuePosition3D[ j ].m_Value / functionScaleFactor;
		x = m_ValuePosition3D[ j ].m_Translation[ 0 ];
		y = m_ValuePosition3D[ j ].m_Translation[ 1 ];
		z = m_ValuePosition3D[ j ].m_Translation[ 2 ];
		rotationIndex =  m_ValuePosition3D[ j ].m_RotationIndex;
		fineRotationIndex =  m_ValuePosition3D[ j ].m_FineRotationIndex;
		if (printConfId)
		{
			conformationIndex =  m_ValuePosition3D[ j ].m_ConformationIndex;
			if (fprintf(fp, "%f %f %f %f %d %d %d\n", value, x*gridFactor, y*gridFactor, z*gridFactor, rotationIndex, fineRotationIndex, conformationIndex) < 0)
			{
				return false;
			}
		}
		else
		{
			if (fprintf(fp, "%f %f %f %f %d %d\n", value, x*gridFactor, y*gridFactor, z*gridFactor, rotationIndex, fineRotationIndex) < 0)
			{
				return false;
			}
		}
	}
	for (int i = 0; i < k; i++)
	{
		insert(i);
	}
	m_CurMin = curMin;
	return true;
}

bool TopValues::print(FILE* fp, int standardGridSize, double functionScaleFactor)
{
	return print(fp, standardGridSize, functionScaleFactor, false);
}

bool TopValues::print(int standardGridSize, double functionScaleFactor, bool printConfId)
{
	if (!m_ValuePosition3D || !m_Heap || (m_NumberOfPositions < 1) || (functionScaleFactor == 0))
	{
		return false;
	}
	printf("Top values and positions in descending order are are follows\n");
	return print(stdout, standardGridSize, functionScaleFactor, printConfId);
}

bool TopValues::print(int standardGridSize, double functionScaleFactor)
{
	if (!m_ValuePosition3D || !m_Heap || (m_NumberOfPositions < 1) || (functionScaleFactor == 0))
	{
		return false;
	}
	printf("Top values and positions in descending order are are follows\n");
	return print(stdout, standardGridSize, functionScaleFactor, false);
}


// Given a grid of complex values,
// update the current top positions with the top values from the grid if any are larger.
// Store both value and position
// Ensure that the same grid size is given as input each time
bool TopValues::updateTopPositions(fftw_complex* grid, int rotationIndex, int fineRotationIndex, int conformationIndex)
{
	if (!grid)
	{
		return false;
	}
	int c = 0;
	for (int i = 0; i < m_GridSize; i++)
		for (int j = 0; j < m_GridSize; j++)
			for (int k = 0; k < m_GridSize; k++)
			{
				if (grid[ c ][ 0 ] > 0)
				{
					updateList(grid[ c ][ 0 ], k, j, i, rotationIndex, fineRotationIndex, conformationIndex);
				}
				c++;
			}
	return true;
}

bool TopValues::updateTopPositions(fftw_complex* grid, int rotationIndex, int fineRotationIndex)
{
	return updateTopPositions(grid, rotationIndex, fineRotationIndex, 0);
}

bool TopValues::updateTopPositions(ValuePosition3D* valuePosition3D, int numberOfPositions)
{
	if (!valuePosition3D || (numberOfPositions < 1))
	{
		return false;
	}
	for (int i = 0; i < numberOfPositions; i++)
		updateList(valuePosition3D[ i ].m_Value,
				   valuePosition3D[ i ].m_Translation[ 0 ],
				   valuePosition3D[ i ].m_Translation[ 1 ],
				   valuePosition3D[ i ].m_Translation[ 2 ],
				   valuePosition3D[ i ].m_RotationIndex,
				   valuePosition3D[ i ].m_FineRotationIndex);
	return true;
}
