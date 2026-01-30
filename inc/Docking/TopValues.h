#ifndef CCV_TOP_VALUES_H
#define CCV_TOP_VALUES_H

#include <stdio.h>
#include "fftw3.h"
#include <Docking/AuxBH.h>

class ValuePosition3D;

class TopValues
{
	public:
		TopValues(int numberOfPositions, int gridSize);
		virtual ~TopValues();
		bool updateTopValues(double value, double x, double y, double z, int rotIndex, int fineRotIndex);
		bool updateTopValues(double value, double x, double y, double z, int rotIndex, int fineRotIndex, int confIndex);
		bool updateTopPositions(fftw_complex* grid, int rotIndex, int fineRotIndex);
		bool updateTopPositions(fftw_complex* grid, int rotIndex, int fineRotIndex, int confIndex);
		bool updateTopPositions(ValuePosition3D* valuePosition3D, int numberOfPositions);
		bool print(int standardGridSize, double functionScaleFactor);
		bool print(int standardGridSize, double functionScaleFactor, bool printConfId);
		bool print(FILE* fp, int standardGridSize, double functionScaleFactor);
		bool print(FILE* fp, int standardGridSize, double functionScaleFactor, bool printConfId);
		int getCurrentNumberOfPositions();
		int getGridSize();
		int getCurMin();
		void extractMin(double* value, double* x, double* y, double* z, int* rotationIndex, int* fineRotationIndex, int* conformationIndex);

	protected:
		inline bool updateList(double value, double x, double y, double z, int rotIndex, int fineRotIndex);
		inline bool updateList(double value, double x, double y, double z, int rotIndex, int fineRotIndex, int confIndex);

	private:
		int m_NumberOfPositions; // number of top positions to capture
		int m_CurrentNumberOfPositions; // number of top positions captured
		int m_GridSize; // The size of the input grid.
		int m_CurMin;
		ValuePosition3D* m_ValuePosition3D; // top positions in unsorted order
		AuxBufferHeap* m_Heap; // an auxiliary buffer heap on valuePositions
		inline void deleteMin();
		inline void replaceMin(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex, int conformationIndex);
		inline void replaceMin(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex);
		inline void insert(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex, int conformationIndex);
		inline void insert(double value, double x, double y, double z, int rotationIndex, int fineRotationIndex);
		inline void insert(int valuePosIndex);
};

#endif
