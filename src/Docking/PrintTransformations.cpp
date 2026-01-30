#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <UsefulMath/Matrix.h>

using CCVOpenGLMath::Matrix;

// takes in the results file, a rotations file, number of rotations considered and an opt file
int printTransformations(int argc, char* argv[])
{
	if ((argc != 6) && (argc != 7))
	{
		return -1;
	}
	FILE* fpResults = fopen(argv[2], "r");
	FILE* fpRotations = fopen(argv[3], "r");
	FILE* fpOutput = fopen(argv[5], "w");
	if (!fpResults || !fpRotations || !fpOutput)
	{
		return -1;
	}
	int numberOfRotations = atoi(argv[4]);
	if (numberOfRotations < 1)
	{
		return -1;
	}
	int numFile2 = 1;
	if (argc == 7)
	{
		numFile2 = atoi(argv[ 6 ]);
	}
	float translateA[3];
	float* translateB = new float[ numFile2 * 3 ];
	float* scale = new float[ numFile2 ];
	int gridSize;
	int numberOfPositions;
	float* trans;
	int* confIndex;
	int* rotIndex;
	float* rotations = new float[ numberOfRotations * 9 ];
	// read in all rotations
	{
		for (int i=0; i<numberOfRotations; i++)
		{
			if (fscanf(fpRotations, "%f %f %f %f %f %f %f %f %f\n",
					   &(rotations[i*9+0]),
					   &(rotations[i*9+1]),
					   &(rotations[i*9+2]),
					   &(rotations[i*9+3]),
					   &(rotations[i*9+4]),
					   &(rotations[i*9+5]),
					   &(rotations[i*9+6]),
					   &(rotations[i*9+7]),
					   &(rotations[i*9+8])) != 9)
			{
				return -1;
			}
		}
	}
	// read in the results
	{
		fscanf(fpResults, "%f %f %f\n", &(translateA[0]), &(translateA[1]), &(translateA[2]));
		for (int i = 0; i < numFile2; i++)
		{
			fscanf(fpResults, "%f %f %f\n", &(translateB[i * 3 + 0]), &(translateB[i * 3 + 1]), &(translateB[i * 3 + 2]));
			fscanf(fpResults, "%f\n", &(scale[ i ]));
		}
		fscanf(fpResults, "%d\n", &(gridSize));
		fscanf(fpResults, "%d\n", &(numberOfPositions));
		trans = new float[numberOfPositions*3];
		if (argc == 7)
		{
			confIndex = new int[ numberOfPositions ];
		}
		rotIndex = new int[numberOfPositions];
		for (int i=0; i<numberOfPositions; i++)
		{
			float dummyFloat;
			if (argc == 7)
			{
				if (fscanf(fpResults, "%f %f %f %f %d %d\n", &dummyFloat, &(trans[i*3+0]), &(trans[i*3+1]), &(trans[i*3+2]), &(rotIndex[i]), &(confIndex[i])) != 6)
				{
					return -1;
				}
			}
			else
			{
				if (fscanf(fpResults, "%f %f %f %f %d\n", &dummyFloat, &(trans[i*3+0]), &(trans[i*3+1]), &(trans[i*3+2]), &(rotIndex[i])) != 5)
				{
					return -1;
				}
			}
		}
	}
	// compute the transformations and print them out
	{
		for (int i=0; i<numberOfPositions; i++)
		{
			int c = 0;
			if (argc == 7)
			{
				c = confIndex[ i ];
			}
			Matrix transformation = Matrix::translation(translateB[ 3 * c + 0 ], translateB[ 3 * c + 1 ], translateB[ 3 * c + 2 ]);
			// get current matrix
			int r = rotIndex[i] - 1;
			if (r < 0)
				transformation = transformation.preMultiplication(Matrix(1, 0, 0, 0,
								 0, 1, 0, 0,
								 0, 0, 1, 0,
								 0, 0, 0, 1));
			else
				transformation = transformation.preMultiplication(Matrix(rotations[r*9+0], rotations[r*9+1], rotations[r*9+2], 0,
								 rotations[r*9+3], rotations[r*9+4], rotations[r*9+5], 0,
								 rotations[r*9+6], rotations[r*9+7], rotations[r*9+8], 0,
								 0,                0,                0,                1));
			transformation = transformation.preMultiplication(Matrix::translation(-translateB[ 3 * c + 0 ], -translateB[ 3 * c + 1 ], -translateB[ 3 * c + 2 ]));
			// now get the translation in real space
			// 1. get the real pos
			double realx = (translateA[0]-translateB[ 3 * c + 0 ])*scale[ c ]*gridSize;
			double realy = (translateA[1]-translateB[ 3 * c + 1 ])*scale[ c ]*gridSize;
			double realz = (translateA[2]-translateB[ 3 * c + 2 ])*scale[ c ]*gridSize;
			// 2. get the current trans
			double transx = trans[i*3+0];
			double transy = trans[i*3+1];
			double transz = trans[i*3+2];
			// 3. normalize it if needed
			if (transx > gridSize/2)
			{
				transx = transx-gridSize;
			}
			if (transy > gridSize/2)
			{
				transy = transy-gridSize;
			}
			if (transz > gridSize/2)
			{
				transz = transz-gridSize;
			}
			// 4. now get real trans
			double rTransx = (transx - realx) / (gridSize*scale[ c ]);
			double rTransy = (transy - realy) / (gridSize*scale[ c ]);
			double rTransz = (transz - realz) / (gridSize*scale[ c ]);
			// find final transformation by adding the translation
			transformation = transformation.preMultiplication(Matrix::translation(rTransx, rTransy, rTransz));
			// I think I need to print transpose of matrix. OpenGL problems.
			const float* m = transformation.getMatrix();
			if (argc == 7)
			{
				fprintf(fpOutput, "%d ", c);
			}
			fprintf(fpOutput, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
					m[0], m[4], m[8],  m[12],
					m[1], m[5], m[9],  m[13],
					m[2], m[6], m[10], m[14],
					m[3], m[7], m[11], m[15]);
		}
	}
	delete []translateB;
	delete []scale;
	delete []trans;
	delete []rotIndex;
	if (argc == 7)
	{
		delete []confIndex;
	}
	delete []rotations;
	fclose(fpResults);
	fclose(fpRotations);
	fclose(fpOutput);
	return 0;
}
