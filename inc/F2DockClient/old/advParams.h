#ifndef ADVANCED_PARAMS
#define ADVANCED_PARAMS

#include <qvariant.h>

typedef struct
{
	bool singleLayerSkin;
	double pseudoAtomRadius;
	bool curvatureWeightedReceptorSkin;
	bool curvatureWeightedLigandSkin;
	double curvatureWeightingRadius;
	bool narrowBandFFT;
	bool sparseFFT;
	bool randomRotate;
	bool rotateVolume;
	int peaksPerRotation;
	double hBondWeight;
	double hBondCutoff;
	bool VDWFilter;
	bool surfaceVDW;
	double VDWCutoff;
	int VDWGridSize;
	bool clashFilter;
	double clashTolerance;
	double clashWeight;
	bool miscFilter;
} AdvancedParams;

#endif
