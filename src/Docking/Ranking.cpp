#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// input is
//   file containing list of RMSD values
//   int : number of values in that file
// output is some sort of meaning from it
int printStats(int argc, char* argv[])
{
	if (argc != 5)
	{
		return -1;
	}
	char inpF[1024];
	strcpy(inpF, argv[2]);
	char optF[1024];
	strcpy(optF, argv[4]);
	FILE* fpIn = fopen(argv[2], "r");
	if (!fpIn)
	{
		return -1;
	}
	FILE* fpOut = fopen(argv[4], "w");
	if (!fpOut)
	{
		return -1;
	}
	int numVals = atoi(argv[3]);
	if (numVals < 1)
	{
		return -1;
	}
	// for now, compute
	// number of peaks in 2A, highest pos | number of peaks in 3A, highest pos | number of peaks .....
	// remember, opt is in descending order
	int counter[20]; // keep until 20A
	int highestPos[20];
	float* rmsd = new float[numVals];
	for (int i=0; i<numVals; i++)
	{
		fscanf(fpIn, "%f\n", &(rmsd[numVals-1-i]));
		//	float tempscore;
		//	fscanf( fpIn, "%f %f\n", &tempscore, &(rmsd[i]) );
		//      fscanf( fpIn, "%f\n", &(rmsd[i]) );
	}
	int maxRank = 10000000;
	for (int i=0; i<20; i++)
	{
		counter[i] = 0;
		highestPos[i] = maxRank;
	}
	float minRMSD = 100000;
	for (int i=0; i<numVals; i++)
	{
		if (minRMSD > rmsd[i])
		{
			minRMSD = rmsd[i];
		}
		int rmsdInt = (int)(rmsd[i]);
		if (rmsdInt < 20)
		{
			counter[rmsdInt]++;
			if (highestPos[rmsdInt] > i)
			{
				highestPos[rmsdInt] = i;
			}
		}
	}
	int numberOfGoodPeaks = 0;
	int highestRank = maxRank;
	int RMSDAllowed = 5;
	// find the highest rank, and number of ranks < 4A rmsd
	{
		for (int i=0; i<RMSDAllowed; i++)
		{
			numberOfGoodPeaks += counter[i];
			if (highestPos[i] < highestRank)
			{
				highestRank = highestPos[i];
			}
		}
	}
	for (int i=0; i<20; i++)
	{
		fprintf(fpOut, "%d --> %d %d\n", i, counter[i], highestPos[i]);
	}
	fprintf(fpOut, "\n(< %d) %d %d %f\n", RMSDAllowed, highestRank, numberOfGoodPeaks, minRMSD);
	fclose(fpIn);
	fclose(fpOut);
	return 0;
}
