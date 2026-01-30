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
#include <Qt3Support>	// by cha

#include <ColorTable2/ColorTable.h>

#include <qfile.h>
#include <q3textstream.h>
#include <TexMol/Dialogs/MainWindow.h>

#include <Blurmaps/AreaVolume.h>
#include <Blurmaps/BlurMapsDataManager.h>
#include <Blurmaps/MergeVolumes.h>
#include <Blurmaps/MolecularSurface.h>
#include <Blurmaps/SkinRegion2.h>
#include <Blurmaps/SurfaceAtomExtractor.h>
#include <Contour/contour.h>
#include <Contouring/MultiContour.h>
#include <DataManager/BallAndStickDataManager/BallAndStickDataManager.h>
#include <DataManager/SurfaceDataManager/SurfaceData.h>
#include <DataManager/VolumeDataManager/VolumeData.h>
//#include <Docking/Complex.h>
//#include <Docking/DockingMain.h>
//#include <Docking/DomainComplex.h>
//#include <Docking/RotamerFlexComplex.h>
//#include <Docking/ScoreModule.h>
//#include <F2DockAnimator/F2DockAnimator.h>
#include <GeometryFileTypes/GeometryLoader.h>
#include <GOAFileTypes/GOALoader.h>
#include <HLevelSet/HLevelSet.h>
#include <LBIE_lib/LBIE_Mesher.h>
#include <math.h>
#include <MolecularCharacteristics/MolecularCharacteristics.h>
#include <PDBParser/Atom.h>
#include <PDBParser/FlattenGOA.h>
#include <PDBParser/GOAFileIO.h>
#include <PDBParser/GroupOfAtoms.h>
#include <PDBParser/MoleculeMorph.h>
#include <Pocket/Pocket.h>
#include <SignDistanceFunction/sdfLib.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <SimpleVolumeData/SimpleVolumeDataIsocontourer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Summation/DirectToGridSummationModule.h>
#include <Summation/GaussianKernel.h>
#include <Summation/UniformOutputGrid.h>
#include <TexMol/DownloadPDB.h>
#include <TexMol/Server.h>
#include <UsefulMath/LinearAlgebra.h>
#include <UsefulMath/Matrix.h>
#include <UsefulMath/Quaternion.h>
#include <UsefulMath/Vector.h>
#include <VolumeFileTypes/VolumeLoader.h>
#include <VolMagick/VolMagick.h>
#include <MolSurfAPI/pqr.h>

#include <F2DockClient/commandLineControl.h>



//using namespace LBIE;
//using namespace HLevelSetNS;

extern float IsoValueofHLS;
extern void getContourSpectrum(unsigned char* uchar_data, int type, int* dim, int array_size, float* isoval , float* area, float* min_vol, float* max_vol, float* gradient, float* span);

Server::Server()
{
}

Server::~Server()
{
}

void Server::printUsage()
{
	printf("Usage: any one of the following, all corresponding paramters are required. Consult manual for more help\n");
	printf("\tTexMol\n");
	printf("\tTexMol [-blur <const char* pdbOrPqrFileName> <const char* volFileName> \n<int dim1> <int dim2> <int dim3> <int densityType> \n<bool writeRawV> <double blob> <unsigned int color> <const char* cmapFile> <int gap> <int radiusType> <unsigned int level> <bool optimizeRadii>]\n");
	printf("\tTexMol [-blurWithResolution <const char* pdbOrPqrFileName> <const char* volFileName> \n<int dim1> <int dim2> <int dim3> <int densityType> \n<bool writeRawV> <double resolution> <unsigned int color> <const char* cmapFile> <int gap> <int radiusType> <unsigned int level> <bool optimizeRadii>]\n");
	printf("\tWRONG , CHANGE MoleculeViz [-setcurvature <int createIsosurface> <const char* inputpdbOrPqrFileName> <int dim1> <int dim2> <int dim3> <double blob> <const char* inputRawSurfaceFileName> <const char* outputMeanRawSurfaceFileName> <const char* outputGaussianRawSurfaceFileName> <const char* outputCurvatureRawSurfaceFileName> <int numberOfGridDivisions> <double maxFunctionError> <int radiusType>]\n");
	printf("\tTexMol [-classifyPoints <const char* inputPtsFileName> <const char* outputPtsFileName> ]\n");
	printf("\tTexMol [-getSurfaceFromVolume <const char* volumeFileName> <const char* outputSurfaceFileName> <double isovalue>]\n");
	printf("\tTexMol [-getSurfaceFromPDB <const char* pdbOrPqrFileName> <const char* outputSurfaceFileName> <double isovalue> <int dim1> <int dim2> <int dim3> <bool blur> <double blobbiness> <int radiusType> <int level> <bool optimizeRadii>]\n");
	printf("\tTexMol [-evolve <const char* pdbOrPqrFileName> <const char* changesFile> <const char* pdbOrPqrFileName>]\n");
	printf("\tTexMol [-writePDB <const char* inputFileName> <const char* outputFileName> <int outputLevel>]\n");
	printf("\tTexMol [-writeGOA <const char* inputFileName> <const char* outputFileName>]\n");
	printf("\tTexMol [-depthColor <const char* inputFileName> <const char* outputFileName> <int dim1> <int dim2> <int dim3>]\n");
	printf("\tTexMol [-getHydrophobicityOnSurface <const char* inputPDBFileName> <const char* inputSurfaceFileName> <int dim1> <int dim2> <int dim3> <double blobbiness> <const char* outputValuesFileName>]\n");
	printf("\tTexMol [-createSkinRegion <const char* inputPDBFileName> <const char* inputSurfaceFileName> <int dim1> <int dim2> <int dim3> <double probeRadius> <int radiusType>]\n");
	printf("\tTexMol [-correlate <const char* hydroFile> <const char* meanCurvFile> <const char* gausCurvFile> <double probeRadius> <bool discretizeHydro> <bool discretizeCurv> <double maxDiscreteVal> <double minDiscreteVal> <int rangeToCorrelate = {0->all,<0,>0} >]\n");
	printf("\tTexMol [-getArea <const char* surfaceFileName> <outputFileName>]\n");
	printf("\tTexMol [-getVolume <const char* surfaceFileName>]\n");
	printf("\tTexMol [-getVolumeN <const char* surfaceFileName>]\n");
	printf("\tTexMol [-getPatchAreas <const char* surfaceFileName> <const char* functionValuesFileName> <double isovalue> <const char* outputFileName>]\n");
	printf("\tTexMol [-populateSAS <const char* surfaceFileName> <const char* functionValuesFileName> <double isovalue> <const char* outputFileName>]\n");
	printf("\tTexMol [-addVolumes <const char* volume1FileName> <const char* volume2FileName> <const char* volume3FileName> <double scale> <double sum>]\n");
	printf("\tTexMol [-convert <const char* inputFileName> <const char* outputFileName>]\n");
	printf("\tTexMol [-getSurfaceAtoms <const char* inputFileName> <const char* outputFileName>]\n");
	printf("\tTexMol [-getContourStats <const char* inputFileName> <const char* outputFileName>]\n");
	printf("\tTexMol [-downloadPDB <const char* pdbID> <const char* outputFileName>]\n");
	printf("\tTexMol [-getSignDistanceFunction <const char* inputFileName> <const char* outputFileName> <int size> <int flipNormals>]\n");
	printf("\tTexMol [-getMolecularCharacteristics <const char* inputGOAFileName> <const char* elecFileName> <const char* outputFileName> <int numberOfSpheres> opt: <float* active site> <double radius1> .. <double radius_n_of_spheres>]\n");
	printf("\tTexMol [-expandMolecule <const char* inputGOAFileName> <const char* outputGOAFileName>]\n");
	printf("\tTexMol [-mergeGeometry <const char* inputSurfaceFileName1> <const char* inputSurfaceFileName2> <const char* outputSurfaceFileName>]\n");
	printf("\tTexMol [-mergeVolumes <const char* inputSurfaceFileName1> <const char* inputSurfaceFileName2> <const char* outputSurfaceFileName> <optional: original and new center and normals>]\n");
	printf("\tTexMol [-writeTorsionAngles <const char* inputGOAFileName> <const char* outputTorsionAnglesFileName>]\n");
	printf("\tTexMol [-morph <const char* inputGOAFileName1> <const char* inputGOAFileName2> <const char* outputGOAFileNamePrefix> <double resolution> <int maxSteps>]\n");
	printf("\tTexMol [-getElecOnSurface <const char* inputElecVolume> <const char* inputSurfaceFileName>  <const char* outputPosSurfaceFileName>  <const char* outputNegSurfaceFileName>  <const char* outputNeuSurfaceFileName> <double negCutoff> <double posCutoff>]\n");
	printf("\tTexMol [-getCurvaturesOnSurface <const char* inputSurfaceFileName> <const char* outputFileName>]\n");
	printf("\tTexMol [-getPockets <const char* input GOA or SurfaceFileName> <float distance>]\n");
	printf("\tTexMol [-createDockingInputFiles <const char* inputGOA1> <const char* inputGOA2> <const char* outputDockingFile1> <const char* outputDockingFile2> <bool printMiscFiles>]\n");
	printf("\tTexMol [-breakUpPDBFile <const char* inputGOA> <const char* outputBasePDB>]\n");
	printf("\tTexMol [-getInterface <const char* inputGOA1> <const char* inputGOA2> <double cutOffParam> <const char* outputFile>]\n");
	printf("\tTexMol [-getInterfaceRMSD <const char* inputGOA1> <const char* inputGOA2> <const char* transformations> <int numberOfTransformations> <double cutOffParam> <const char* outputFile>]\n");
	printf("\tTexMol [-getInterfaceArea <const char* inputGOA1> <const char* inputGOA2> <const char* transformations> <int numberOfTransformations> <double cutOffParam> <const char* outputFile>]\n");
	printf("\tTexMol [-improveInterface <const char* inputGOA1> <const char* inputGOA2> <const char* unboundLigandFileName> <int numberOfTransformations> <const char* transformations> <double cutOffParam> <const char* outputFile> <bool flexDock>]\n");
	printf("\tTexMol [-printFCC <const char* inputGOA> <const char* inputDomains> <const char* filename>\n");
	printf("\tTexMol [-getConformations <const char* inputGOA> <const char* inputFCC> <int level> <const char* optbasefilename> <int recommendedSampling>\n");
	printf("\tTexMol [-computeScoringGrids <const char* inputGOA> <double ligandSize> <double gridSpacing> <const char* gridBaseFileName>\n");
	printf("\tTexMol [-docking <alpha> <interpFuncExtent> <numFreq> <gridSize> <inputFile1> <inputFile2> <rotationFile> <numRotations> <outFile> <numPositions> <performDocking> <blobbiness> <elecScale>]\n");
	printf("\tTexMol [-dockingMovie <F2DockTransformationFile> <ProteinA_PDB> <ProteinB_PDB> <OutputFilePrefix> ]\n");
	printf("\tTexMol [-geoImprove <const char* inputGeoFile> <const char* outputGeoFile>]\n");
	printf("\tTexMol [-getSurfaceUsingHLS <sonst char* psdOrPqrFileName> <const char* outputSurfaceFileName> <int dim1> <int dim2> <int dim3> <int iteration> <float weight>]\n");

	printf("\tTexMol [-dock <inputFileName (*.inp)>]\n");
	printf("\tTexMol [-rerank <inputFileName (*.inp)>]\n");
	printf("\tTexMol [-f2dgen <inputFileName (*.inp)>]\n");
	printf("\tTexMol [-quadgen <inputFileName (*.inp)>]\n");
	printf("\tTexMol [-shrinkPQR <inputPQRFileName (*.pqr)> <inputVolumeFile (*.rawiv)> <input Iteration number (int)> <input stepsize (float)> <outputPQRFile (*.pqr)>]\n");
}

bool Server::execute(int argc, char* argv[], MainWindow* mWindow)
{
	if (argc < 2)
	{
		return false;
	}
	if (strcmp(argv[1],"-help") == 0)
	{
		printUsage();
		return true;
	}
	if (strcmp(argv[1],"-h") == 0)
	{
		printUsage();
		return true;
	}
	if (strcmp(argv[1],"-blur") == 0)
	{
		return blur(argc, argv);
	}
	if (strcmp(argv[1],"-blurWithResolution") == 0)
	{
		return blurWithResolution(argc, argv);
	}
	if (strcmp(argv[1],"-setcurvature") == 0)
	{
		return setCurvature(argc, argv);
	}
	if (strcmp(argv[1],"-classifyPoints") == 0)
	{
		return classifyPoints(argc, argv);
	}
	if (strcmp(argv[1],"-getSurfaceFromVolume") == 0)
	{
		return getSurfaceFromVolume(argc, argv);
	}
	if (strcmp(argv[1],"-getSurfaceFromPDB") == 0)
	{
		return getSurfaceFromPDB(argc, argv);
	}
	if (strcmp(argv[1],"-evolve") == 0)
	{
		return evolve(argc, argv);
	}
	if (strcmp(argv[1],"-writePDB") == 0)
	{
		return writePDB(argc, argv);
	}
	if (strcmp(argv[1],"-writeGOA") == 0)
	{
		return writeGOA(argc, argv);
	}
	if (strcmp(argv[1],"-depthColor") == 0)
	{
		return depthColor(argc, argv);
	}
	if (strcmp(argv[1],"-getMaxDistanceFromPoint") == 0)
	{
		return getMaxDistanceFromPoint(argc, argv);
	}
	if (strcmp(argv[1],"-getHydrophobicityOnSurface") == 0)
	{
		return getHydrophobicityOnSurface(argc, argv);
	}
	if (strcmp(argv[1],"-createSkinRegion") == 0)
	{
		return createSkinRegion(argc, argv);
	}
	if (strcmp(argv[1],"-correlate") == 0)
	{
		return correlate(argc, argv);
	}
	if (strcmp(argv[1],"-getArea") == 0)
	{
		return getArea(argc, argv);
	}
	if (strcmp(argv[1],"-getVolume") == 0)
	{
		return getVolume(argc, argv);
	}
	if (strcmp(argv[1],"-getVolumeN") == 0)
	{
		return getVolumeN(argc, argv);
	}
	if (strcmp(argv[1],"-getPatchAreas") == 0)
	{
		return getPatchAreas(argc, argv);
	}
	if (strcmp(argv[1],"-populateSAS") == 0)
	{
		return populateSAS(argc, argv);
	}
	if (strcmp(argv[1],"-addVolumes") == 0)
	{
		return addVolumes(argc, argv);
	}
	if (strcmp(argv[1],"-convert") == 0)
	{
		return convert(argc, argv);
	}
	if (strcmp(argv[1],"-getSurfaceAtoms") == 0)
	{
		return getSurfaceAtoms(argc, argv);
	}
	if (strcmp(argv[1],"-getContourStats") == 0)
	{
		return getContourStats(argc, argv);
	}
	if (strcmp(argv[1],"-getSignDistanceFunction") == 0)
	{
		return getSignDistanceFunction(argc, argv);
	}
	if (strcmp(argv[1],"-getMolecularCharacteristics") == 0)
	{
		return getMolecularCharacteristics(argc, argv);
	}
	if (strcmp(argv[1],"-expandMolecule") == 0)
	{
		return expandMolecule(argc, argv);
	}
	if (strcmp(argv[1],"-mergeGeometry") == 0)
	{
		return mergeGeometry(argc, argv);
	}
	if (strcmp(argv[1],"-mergeVolumes") == 0)
	{
		return mergeVolumes(argc, argv);
	}
	if (strcmp(argv[1],"-writeTorsionAngles") == 0)
	{
		return writeTorsionAngles(argc, argv);
	}
	if (strcmp(argv[1],"-morph") == 0)
	{
		return morph(argc, argv);
	}
	if (strcmp(argv[1],"-getElecOnSurface") == 0)
	{
		return getElecOnSurface(argc, argv);
	}
	if (strcmp(argv[1],"-getCurvaturesOnSurface") == 0)
	{
		return getCurvaturesOnSurface(argc, argv);
	}
	if (strcmp(argv[1],"-getPockets") == 0)
	{
		return getPockets(argc, argv);
	}
	/*
	if (strcmp(argv[1],"-createDockingInputFiles") == 0)
	{
		return createDockingInputFiles(argc, argv);
	}
	*/
	if (strcmp(argv[1],"-breakUpPDBFile") == 0)
	{
		return breakUpPDBFile(argc, argv);
	}
	/*
	if (strcmp(argv[1],"-getInterface") == 0)
	{
		return getInterface(argc, argv);
	}
	*/
	/*
	if (strcmp(argv[1],"-getInterfaceRMSD") == 0)
	{
		return getInterfaceRMSD(argc, argv);
	}
	*/
	/*
	if (strcmp(argv[1],"-getInterfaceArea") == 0)
	{
		return getInterfaceArea(argc, argv);
	}
	*/
	/*
	if (strcmp(argv[1],"-improveInterface") == 0)
	{
		return improveInterface(argc, argv);
	}
	*/
	/*
	if (strcmp(argv[1],"-printFCC") == 0)
	{
		return printFCC(argc, argv);
	}
	*/
	/*
	if (strcmp(argv[1],"-getConformations") == 0)
	{
		return getConformations(argc, argv);
	}
	*/
	/*
	if (strcmp(argv[1],"-computeScoringGrids") == 0)
	{
		return computeScoringGrids(argc, argv);
	}
	*/
	if (strcmp(argv[1],"-addData") == 0)
	{
		return addNewDataSet(argc, argv, mWindow);
	}
	if (strcmp(argv[1],"-splitView") == 0)
	{
		return splitView(argc, argv, mWindow);
	}
	if (strcmp(argv[1],"-deleteData") == 0)
	{
		return deleteData(argc, argv, mWindow);
	}
	if (strcmp(argv[1],"-deletePrevData") == 0)
	{
		return deletePrevData(argc, argv, mWindow);
	}
	if (strcmp(argv[1],"-deleteAllData") == 0)
	{
		return deleteAllData(argc, argv, mWindow);
	}
	if (strcmp(argv[1],"-setVisible") == 0)
	{
		return setVisible(argc, argv, mWindow);
	}
	if (strcmp(argv[1],"-setVisiblePrev") == 0)
	{
		return setVisiblePrev(argc, argv, mWindow);
	}
	if (strcmp(argv[1],"-saveImage") == 0)
	{
		return saveImage(argc, argv, mWindow);
	}
	if (strcmp(argv[1],"-setGridVisible") == 0)
	{
		return setGridVisible(argc, argv, mWindow);
	}
	if (strcmp(argv[1],"-downloadPDB") == 0)
	{
		return downloadPDB(argc, argv, mWindow);
	}
	/*
	if (strcmp(argv[1],"-docking") == 0)
	{
		return dockingMain(argc, argv);
	}
	*/
	/*
	if (strcmp(argv[1],"-dockingMovie") == 0)
	{
		return dockingMovie(argc, argv);
	}
	*/
	if (strcmp(argv[1],"-geoImprove") == 0)
	{
		return geoImprove_LBIE(argc, argv);
	}
	if (strcmp(argv[1], "-getSurfaceUsingHLS") ==0)
	{
		return getSurfaceUsingHLS(argc, argv);
	}
	/*
	if (strcmp(argv[1], "-dock") ==0)
	{
		return dockRequest(argc, argv);
	}
	*/
	if (strcmp(argv[1], "-rerank") ==0)
	{
		return rerankRequest(argc, argv);
	}
	if (strcmp(argv[1], "-f2dgen") ==0)
	{
		return f2dGenRequest(argc, argv);
	}
	if (strcmp(argv[1], "-quadgen") ==0)
	{
		return quadGenRequest(argc, argv);
	}
	if (strcmp(argv[1], "-shrinkPQR") == 0)
	{
		return shrinkPQRintoSurface(argc,  argv);
	}

	printUsage();
	return false;
}

bool Server::depthColor(int argc, char* argv[])
{
	// Take as input a rawiv file and create a depth colored rawv.
	// It needs a color map also. We are currently using a default map
	if (argc != 7)
	{
		printUsage();
		return false;
	}
	char inputFileName[256];
	char outputFileName[256];
	int dim1, dim2, dim3;
	strcpy(inputFileName, argv[2]);
	strcpy(outputFileName, argv[3]);
	dim1 = atoi(argv[4]);
	dim2 = atoi(argv[5]);
	dim3 = atoi(argv[6]);
	int colorMapSize = 256; // new rover supports only 256  - be careful SKVINAY !
	double dColorMap[256*4];
	for (int i=0; i<256; i++)
	{
		dColorMap[i*4+0] = 1;
		dColorMap[i*4+1] = 0;
		dColorMap[i*4+2] = 0;
		dColorMap[i*4+3] = 1;
	}
	double r=1.0, g=1.0, b=1.0;
	double oldr = 1.0, oldg = 1.0, oldb = 1.0;
	for (int i=0; i<8; i++)
	{
		for (int j=0; j<32; j++)
		{
			dColorMap[(i*32 + j)*4 +0] = (r*j/32.0 + oldr*(32-j)/32);
			dColorMap[(i*32 + j)*4 +1] = (g*j/32.0 + oldg*(32-j)/32);
			dColorMap[(i*32 + j)*4 +2] = (b*j/32.0 + oldb*(32-j)/32);
			dColorMap[(i*32 + j)*4 +3] = 1;
		}
		printf("%d %f %f %f\n", i, r, g, b);
		oldr = r;
		oldg = g;
		oldb = b;
		r = rand()/((double)(RAND_MAX)+1);
		b = rand()/((double)(RAND_MAX)+1);
		g = rand()/((double)(RAND_MAX)+1);
	}
	SimpleVolumeData* simpleVolumeFile = 0;
	{
		VolumeLoader* volumeLoader = new VolumeLoader();
		simpleVolumeFile = volumeLoader->loadFile(inputFileName);
		delete volumeLoader;
		if (!simpleVolumeFile)
		{
			return false;
		}
	}
	//  create depth colored volume
	SimpleVolumeData* sData = 0;
	sData = simpleVolumeFile->createDepthColoredVolume(dColorMap, colorMapSize-1);
	if (!sData)
	{
		delete simpleVolumeFile;
		return false;
	}
	// write to file if needed
	bool ret = false;
	{
		VolumeLoader* volumeLoader = new VolumeLoader();
		ret = volumeLoader->saveFile(outputFileName, sData);
		delete volumeLoader;
	}
	delete simpleVolumeFile;
	delete sData;
	return ret;
}

bool Server::blurWithResolution(int argc, char* argv[])
{
	double resolution = atof(argv[9]);
	double blob = -log(2.0)/(resolution*resolution);
	printf("%f\n",blob);
	char** argv2;
	argv2 = new char*[argc];
	for (int i=0; i<argc; i++)
	{
		argv2[i] = new char[100];
		strcpy(argv2[i], argv[i]);
	}
	sprintf(argv2[9],"%f",blob);
	return(blur(argc,argv2));
}

bool Server::blur(int argc, char* argv[])
{
	// Create a blur map of an input pdb or pqr file. Should we also support pts files ?
	// Supports both electron density and hydrophobicity
	// Input parameters:
	//		char* input filename
	//		char* output filename
	//		int dim1, dim2, dim3
	//		double blobby factor
	//		int density type ( 0 - electron density, 1 - hydrophobicity )
	//		char* colormap
	//		int color type ( 0 - , 1 - , 2 - )
	//		int gap - not very nice to use as it destroys meaning of origin, span etc. Only use sparingly for viz
	//	Side effects:
	//		creates RAWIV or RAV5 volume
	//	Return value:
	//		bool indicates success or failure of function call
	//	usage:
	//			bool getVolume( const char* pdbOrPqrFileName, const char* volFileName,
	//			 int dim1, int dim2, int dim3, int densityType,
	//			 bool writeRawV, double blob, unsigned int color, const char* cmapFile, int gap, int radiusType, unsigned int level );
	//  radiusType == 0 -> vdv radius, == 1 -> solvent enlarged radius
	//
	// the 16 to 27 parameters can set to transform the molecule: (cx, cy, cz, nx, ny, nz) old, (cx, cy, cz, nx, ny, nz) new
	// last param is true or false, depending on whether u want to optimize radii or not
	if (argc != 16 && argc != 28)
	{
		printUsage();
		return false;
	}
	char inputFileName[256];
	char outputFileName[256];
	int dim1, dim2, dim3;
	PDBParser::GroupOfAtoms::FUNCTIONS densityType;
	bool writeRawV;
	double blob;
	PDBParser::GroupOfAtoms::GOA_TYPE colorLevel;
	char cmapFile[256];
	int gap;
	PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType;
	int level;
	strcpy(inputFileName, argv[2]);
	strcpy(outputFileName, argv[3]);
	dim1 = atoi(argv[4]);
	dim2 = atoi(argv[5]);
	dim3 = atoi(argv[6]);
	if (!PDBParser::GroupOfAtoms::intToFunctionType(&densityType, atoi(argv[7])))
	{
		return false;
	}
	if (strcmp(argv[8], "true") == 0)
	{
		writeRawV = true;
	}
	else if (strcmp(argv[8], "false") == 0)
	{
		writeRawV = false;
	}
	else
	{
		printUsage();
		return false;
	}
	blob = atof(argv[9]);
	if (!PDBParser::GroupOfAtoms::intToGOAType(&colorLevel, atoi(argv[10])))
	{
		return false;
	}
	strcpy(cmapFile, argv[11]);
	gap = atoi(argv[12]);
	if (!PDBParser::GroupOfAtoms::intToRadiusType(&radiusType, atoi(argv[13])))
	{
		return false;
	}
	level = atoi(argv[14]);
	bool optimizeRadii = false;
	if (strcmp(argv[15], "true") == 0)
	{
		optimizeRadii = true;
	}
	else if (strcmp(argv[15], "false") == 0)
	{
		optimizeRadii = false;
	}
	else
	{
		printUsage();
		return false;
	}
	SimpleVolumeData* sData = 0;
	// transform molecule if required
	if (argc == 28)
	{
		CCVOpenGLMath::Matrix transformation;
		{
			CCVOpenGLMath::Vector old_center(atof(argv[16]), atof(argv[17]), atof(argv[18]), 1);
			CCVOpenGLMath::Vector old_normal(atof(argv[19]), atof(argv[20]), atof(argv[21]), 0);
			CCVOpenGLMath::Vector new_center(atof(argv[22]), atof(argv[23]), atof(argv[24]), 1);
			CCVOpenGLMath::Vector new_normal(atof(argv[25]), atof(argv[26]), atof(argv[27]), 0);
			// shift center to new one.
			// rotate to align quaternions
			// for each mol, we need a predefined center and normal.
			// translate mol_center to input_center
			// now to rotate:
			//     cross product gives axis of rotation
			//     dot product gives the angle
			old_normal.normalize();
			new_normal.normalize();
			CCVOpenGLMath::Vector axis_of_rotation = old_normal.cross(new_normal);
			axis_of_rotation.normalize();
			double angle_of_rotation = acos(new_normal.dot(old_normal));
			float w = cos(angle_of_rotation / 2.0);
			float x = axis_of_rotation[0] * sin(angle_of_rotation / 2.0);
			float y = axis_of_rotation[1] * sin(angle_of_rotation / 2.0);
			float z = axis_of_rotation[2] * sin(angle_of_rotation / 2.0);
			CCVOpenGLMath::Quaternion quaternion(w, x, y, z);
			CCVOpenGLMath::Matrix rotation = quaternion.buildMatrix();
			CCVOpenGLMath::Vector tr_old_center = rotation*old_center;
			CCVOpenGLMath::Matrix translation = CCVOpenGLMath::Matrix::translation(new_center - tr_old_center);
			transformation = rotation.preMultiplication(translation);
			//transformation = rotation;
		}
		{
			GOALoader* goaLoader = new GOALoader();
			PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(inputFileName);
			delete goaLoader;
			if (!molecule)
			{
				return false;
			}
			if (optimizeRadii)
			{
				sData = BlurMapsDataManager::getMultiLevelBlurredVolume(molecule, outputFileName,
						dim1, dim2, dim3, densityType,
						writeRawV, blob, colorLevel, cmapFile, gap, radiusType, level, &transformation);
			}
			else
			{
				sData = BlurMapsDataManager::getVolume(molecule, outputFileName,
													   dim1, dim2, dim3, densityType,
													   writeRawV, blob, colorLevel, cmapFile, gap, radiusType, level, &transformation);
			}
			delete molecule;
		}
	}
	else
	{
		{
			GOALoader* goaLoader = new GOALoader();
			PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(inputFileName);
			delete goaLoader;
			if (!molecule)
			{
				return false;
			}
			if (optimizeRadii)
			{
				sData = BlurMapsDataManager::getVolume(molecule, outputFileName, dim1, dim2, dim3, densityType, writeRawV, blob, colorLevel, cmapFile, gap, radiusType, level, 0);
			}
			else
			{
				sData = BlurMapsDataManager::getMultiLevelBlurredVolume(molecule, outputFileName, dim1, dim2, dim3, densityType, writeRawV, blob, colorLevel, cmapFile, gap, radiusType, level, 0);
			}
			delete molecule;
		}
	}
	bool ret;
	if (sData)
	{
		ret = true;
	}
	else
	{
		ret = false;
	}
	delete sData;
	return ret;
	/*{
		GOALoader* gLoader = new GOALoader();
		PDBParser::GroupOfAtoms* molecule = gLoader->loadFile( argv[2] );
		delete gLoader;
		if( !molecule ) return false;

		int n = 0;
		molecule->getNumberOfAtomsRecursive(&n);
		if( n <= 0 ) return false;

		double blobbiness = atof(argv[9]);
		if( blobbiness >= 0 ) return false;

		double error = 1e-10;

		PDBParser::GroupOfAtoms::FUNCTIONS function = PDBParser::GroupOfAtoms::ELECTRON_DENSITY;
		if( !PDBParser::GroupOfAtoms::intToFunctionType( &function, atoi(argv[7]) ) ) return false;

		PDBParser::GroupOfAtoms::RADIUS_TYPE radius_type = PDBParser::GroupOfAtoms::VDW_RADIUS;
		if( !PDBParser::GroupOfAtoms::intToRadiusType( &radius_type, atoi(argv[13]) ) ) return false;

		int GOA_level = atoi(argv[14] );

		double min[3]; min[0] = min[1] = min[2] = 1e20;
		double max[3]; max[0] = max[1] = max[2] = 1e-20;
		double maxRadius = -1;

		double* points = new double[n*3];
		double* radii = new double[n];
		double* weights = new double[n];

		molecule->getAttributes( points, radii, min, max, weights, &maxRadius, function, GOA_level, radius_type, n );
		CCVSummationModule::Kernel* kernel = new CCVSummationModule::GaussianKernel( blobbiness, error );

		unsigned int dimensions[3];
		dimensions[0] = atoi(argv[4]); dimensions[1] = atoi(argv[5]); dimensions[2] = atoi(argv[6]);
		float* output = new float[dimensions[0]*dimensions[1]*dimensions[2]];
		float origin[3]; origin[0] = min[0]; origin[1] = min[1]; origin[2] = min[2];
		float span[3];
		span[0] = (max[0] - min[0]) / ((double)(dimensions[0]-1));
		span[1] = (max[1] - min[1]) / ((double)(dimensions[1]-1));
		span[2] = (max[2] - min[2]) / ((double)(dimensions[2]-1));

		CCVSummationModule::OutputGrid *  outputGrid = new CCVSummationModule::UniformOutputGrid( output, origin, span, dimensions );

		CCVSummationModule::SummationModule* summationModule = new CCVSummationModule::DirectToGridSummationModule( points, radii, weights, n, kernel, outputGrid );
		if( !summationModule->sum() )
		{
			if( points ) { delete []points; points = 0; }
			if( radii ) { delete []radii; radii = 0; }
			if( weights ) { delete []weights; weights = 0; }
			if( output ) { delete []output; output = 0; }
			return false;
		}
		{
			SimpleVolumeData* vol = new SimpleVolumeData(dimensions);
			vol->setDimensions( dimensions );
			vol->setNumberOfVariables(1);
			vol->setData(0, output);
			vol->setType(0, SimpleVolumeData::FLOAT);
			vol->setName(0, "TexMols blurring");
			float fmin[3], fmax[3];
			fmin[0] = min[0]; fmin[1] = min[1]; fmin[2] = min[2];
			fmax[0] = max[0]; fmax[1] = max[1]; fmax[2] = max[2];
			vol->setMinExtent(fmin);
			vol->setMaxExtent(fmax);

			VolumeLoader* volumeLoader = new VolumeLoader();
			volumeLoader->saveFile("opt.rawiv", vol );
			delete volumeLoader;

			delete vol;
		}

		// simpleVolumeData should delete the 'output' data
		if( points ) { delete []points; points = 0; }
		if( radii ) { delete []radii; radii = 0; }
		if( weights ) { delete []weights; weights = 0; }
	}
	return true;
	*/
}

bool Server::getSurfaceFromVolume(int argc, char* argv[])
{
	if (argc != 5)
	{
		printUsage();
		return false;
	}
	char volumeFileName[256];
	char surfaceFileName[256];
	double isovalue = 1;
	strcpy(volumeFileName, argv[2]);
	strcpy(surfaceFileName, argv[3]);
	isovalue = atof(argv[4]);
	// read in the volume
	SimpleVolumeData* sData = 0;
	{
		VolumeLoader* vLoader = new VolumeLoader();
		sData = vLoader->loadFile(volumeFileName);
		delete vLoader;
		if (!sData)
		{
			return false;
		}
	}
	Geometry* geometry = SimpleVolumeDataIsocontourer::getIsocontour(sData, isovalue);
	if (geometry == 0)
	{
		delete sData;
		return false;
	}
	// save the isocontour
	GeometryLoader* geometryLoader = new GeometryLoader();
	if (!geometryLoader->saveFile(surfaceFileName, geometry))
	{
		delete geometry;
		delete sData;
		return false;
	}
	delete geometryLoader;
	delete geometry;
	delete sData;
	return true;
}

bool Server::getSurfaceUsingHLS(int argc, char* argv[])
{
	if (argc != 9)
	{
		printUsage();
		return false;
	}
	SimpleVolumeData* sData = 0;
	unsigned int iter, dim[3];
	dim[0] = atoi(argv[4]);
	dim[1] = atoi(argv[5]);
	dim[2] = atoi(argv[6]);
	iter = atoi(argv[7]);
	float wei;
	wei = atof(argv[8]);
	GOALoader* goaLoader = new GOALoader();
	PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(argv[2]);
	delete goaLoader;
	HLevelSet* hls = new HLevelSet();
	sData = hls->getHigherOrderLevelSetSurface(molecule,dim, iter, wei);
	//sData = hls->getHigherOrderLevelSetSurface(molecule,dim);
	// arand hacked above...
	// arand 5-26-2010: undid the hack above since HLS code is fixed to
	//                  include those parameters...
	Geometry* geometry = 0;
	//geometry = SimpleVolumeDataIsocontourer::getIsocontour(sData, -1.4);
	geometry = SimpleVolumeDataIsocontourer::getIsocontour(sData, IsoValueofHLS);
	if (geometry == 0)
	{
		delete sData;
		return false;
	}
	delete sData;
	hls->computeGeometryNormal(geometry,  dim);
	delete hls;
	GeometryLoader* geometryLoader = new GeometryLoader();
	if (!geometryLoader->saveFile(argv[3], geometry))
	{
		delete geometry;
		return false;
	}
	delete geometryLoader;
	delete geometry;
	return true;
}



bool Server::evolve(int argc, char* argv[])
{
	if (argc != 5)
	{
		printUsage();
		return false;
	}
	char pdbOrPqrFileName[256];
	char changesFile[256];
	char outputPdbOrPqrFileName[256];
	strcpy(pdbOrPqrFileName, argv[2]);
	strcpy(changesFile, argv[3]);
	strcpy(outputPdbOrPqrFileName, argv[4]);
	BallAndStickDataManager b;
	b.evolve(pdbOrPqrFileName, changesFile, outputPdbOrPqrFileName);
	return true;
}

bool Server::setCurvature(int argc, char* argv[])
{
	if (argc < 3)
	{
		printUsage();
		return false;
	}
	char inputPQRorPDBFileName[256];
	char inputRawSurfaceFileName[256];
	char outputMeanRawSurfaceFileName[256];
	char outputGaussianRawSurfaceFileName[256];
	char outputVolumeFileName[256];
	char curvatureFileName[256];
	int dim1, dim2, dim3;
	double blob, isovalue;
	int createIsosurface;
	int numberOfGridDivisions;
	double maxFunctionError;
	PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType;
	int level;
	createIsosurface = atoi(argv[2]);
	if (createIsosurface == 0)
	{
		if (argc != 16)
		{
			printUsage();
			return false;
		}
		strcpy(inputPQRorPDBFileName, argv[3]);
		strcpy(inputRawSurfaceFileName, argv[4]);
		strcpy(outputMeanRawSurfaceFileName, argv[5]);
		strcpy(outputGaussianRawSurfaceFileName, argv[6]);
		strcpy(curvatureFileName, argv[7]);
		dim1 = atoi(argv[8]);
		dim2 = atoi(argv[9]);
		dim3 = atoi(argv[10]);
		blob = atof(argv[11]);
		numberOfGridDivisions = atoi(argv[12]);
		maxFunctionError = atof(argv[13]);
		if (!PDBParser::GroupOfAtoms::intToRadiusType(&radiusType, atoi(argv[14])))
		{
			return false;
		}
		level = atoi(argv[15]);
		return BlurMapsDataManager::getCurvaturesFromIsocontourFile(inputPQRorPDBFileName,
				dim1, dim2, dim3, blob, inputRawSurfaceFileName,
				outputMeanRawSurfaceFileName, outputGaussianRawSurfaceFileName,
				curvatureFileName,
				numberOfGridDivisions, maxFunctionError, radiusType, level);
	}
	// new with isovalue
	else if (createIsosurface == 1)
	{
		if (argc != 16)
		{
			printUsage();
			return false;
		}
		strcpy(inputPQRorPDBFileName, argv[3]);
		strcpy(outputMeanRawSurfaceFileName, argv[4]);
		strcpy(outputGaussianRawSurfaceFileName, argv[5]);
		strcpy(curvatureFileName, argv[6]);
		dim1 = atoi(argv[7]);
		dim2 = atoi(argv[8]);
		dim3 = atoi(argv[9]);
		blob = atof(argv[10]);
		isovalue = atof(argv[11]);
		numberOfGridDivisions = atoi(argv[12]);
		maxFunctionError = atof(argv[13]);
		if (!PDBParser::GroupOfAtoms::intToRadiusType(&radiusType, atoi(argv[14])))
		{
			return false;
		}
		level = atoi(argv[15]);
		SimpleVolumeData* sData = 0;
		// create volume from blurring code
		{
			GOALoader* goaLoader = new GOALoader();
			PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(inputPQRorPDBFileName);
			delete goaLoader;
			if (!molecule)
			{
				return false;
			}
			sData = BlurMapsDataManager::getVolume(molecule,
												   outputVolumeFileName,
												   dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
												   false, blob, PDBParser::GroupOfAtoms::ATOM, NULL, 0, radiusType, level);
			delete molecule;
		}
		if (!sData)
		{
			return false;
		}
		// extract an isocontour
		Geometry* geometry = SimpleVolumeDataIsocontourer::getIsocontour(sData, isovalue);
		if (geometry == 0)
		{
			return false;
		}
		//
		//  get curvatures
		return BlurMapsDataManager::getCurvatures(inputPQRorPDBFileName, dim1, dim2, dim3,
				blob, geometry, outputMeanRawSurfaceFileName,
				outputGaussianRawSurfaceFileName, curvatureFileName,
				numberOfGridDivisions, maxFunctionError, radiusType, level);
		return false;
	}
	else
	{
		printUsage();
		return false;
	}
}

bool Server::classifyPoints(int argc, char* argv[])
{
	if (argc != 3)
	{
		printUsage();
		return false;
	}
	char inputPtsFile[256];
	char outputPtsFile[256];
	strcpy(inputPtsFile, argv[2]);
	strcpy(outputPtsFile, argv[3]);
	bool ret = false;
	/*	VorocompDataManager *v = new VorocompDataManager();
		ret = v->classifyPoints( inputPtsFile, outputPtsFile );
		delete v;
	*/
	return ret;
}

bool Server::writePDB(int argc, char* argv[])
{
	if (argc != 5)
	{
		printUsage();
		return false;
	}
	int outputLevel = atoi(argv[4]);
	// create the molecule //
	GOALoader* gLoader = new GOALoader();
	PDBParser::GroupOfAtoms* molecule = gLoader->loadFile(argv[2]);
	delete gLoader;
	if (!molecule)
	{
		return false;
	}
	// write the output
	FILE* stream = fopen(argv[3], "w");
	if (stream == 0)
	{
		delete molecule;
		molecule= 0;
		return false;
	}
	bool ret = PDBParser::writeGOA2PDB(stream, molecule, outputLevel, 0);
	fclose(stream);
	// /
	delete molecule;
	molecule = 0;
	return ret;
}

bool Server::writeGOA(int argc, char* argv[])
{
	if (argc != 4)
	{
		printUsage();
		return false;
	}
	// create the molecule
	GOALoader* gLoader = new GOALoader();
	PDBParser::GroupOfAtoms* molecule = gLoader->loadFile(argv[2]);
	delete gLoader;
	if (!molecule)
	{
		return false;
	}
	// write the output
	FILE* stream = fopen(argv[3], "w");
	if (stream == 0)
	{
		delete molecule;
		molecule= 0;
		return false;
	}
	bool ret = PDBParser::writeGOAtoFile(stream, molecule);
	fclose(stream);
	// /
	delete molecule;
	molecule = 0;
	return ret;
}

bool Server::addNewDataSet(int argc, char* argv[], MainWindow* mWindow)
{
	if (!mWindow)
	{
		return false;
	}
	if (argc != 3)
	{
		return false;
	}
	char inputFileName[256];
	strcpy(inputFileName, argv[2]);
/*Q3Err:CS
	return mWindow->addNewDataSet(inputFileName);
*/
	return mWindow->addNewDataSet(QStringList(QString(inputFileName)),false);
}

bool Server::splitView(int argc, char* argv[], MainWindow* mWindow)
{
	if (!mWindow)
	{
		return false;
	}
	if (argc != 2)
	{
		return false;
	}
	mWindow->splitViewSlot();
	return true;
}

bool Server::deleteData(int argc, char* argv[], MainWindow* mWindow)
{
	if (!mWindow)
	{
		return false;
	}
	if (argc != 3)
	{
		return false;
	}
	int dataSetIndex = atoi(argv[2]);
	mWindow->deleteData(dataSetIndex);
	return true;
}

bool Server::deletePrevData(int argc, char* argv[], MainWindow* mWindow)
{
	if (!mWindow)
	{
		return false;
	}
	if (argc != 2)
	{
		return false;
	}
	return mWindow->deletePrevData();
}

bool Server::deleteAllData(int argc, char* argv[], MainWindow* mWindow)
{
	if (!mWindow)
	{
		return false;
	}
	if (argc != 2)
	{
		return false;
	}
	return mWindow->deleteAllData();
}

bool Server::setVisible(int argc, char* argv[], MainWindow* mWindow)
{
	if (!mWindow)
	{
		return false;
	}
	if (argc != 4)
	{
		return false;
	}
	bool render;
	char renderC[256];
	strcpy(renderC, argv[2]);
#ifdef _WIN32
	if (strcmpi(renderC, "true") == 0)
#else
	if (strcasecmp(renderC, "true") == 0)
#endif
		render = true;
	else
	{
		render = false;
	}
	int dataSetIndex = atoi(argv[3]);
	return mWindow->setVisible(render, dataSetIndex);
}

bool Server::setVisiblePrev(int argc, char* argv[], MainWindow* mWindow)
{
	if (!mWindow)
	{
		return false;
	}
	if (argc != 3)
	{
		return false;
	}
	bool render;
	char renderC[256];
	strcpy(renderC, argv[2]);
#ifdef _WIN32
	if (strcmpi(renderC, "true") == 0)
#else
	if (strcasecmp(renderC, "true") == 0)
#endif
		render = true;
	else
	{
		render = false;
	}
	return mWindow->setVisiblePrev(render);
}

bool Server::saveImage(int argc, char* argv[], MainWindow* mWindow)
{
	if (!mWindow)
	{
		return false;
	}
	if (argc != 4)
	{
		return false;
	}
	return mWindow->saveImage(argv[2], argv[3]);
}

bool Server::printPDBInformation(int argc, char* argv[])
{
	if (argc != 5)
	{
		return false;
	}
	BallAndStickDataManager b;
	int printType = atoi(argv[4]);
	return b.printPDBInformation(argv[2], argv[3], printType);   // input , output file names
}

bool Server::setGridVisible(int argc, char* argv[], MainWindow* mWindow)
{
	if (!mWindow)
	{
		return false;
	}
	if (argc != 3)
	{
		return false;
	}
#ifdef _WIN32
	if (strcmpi(argv[2], "true") == 0)
#else
	if (strcasecmp(argv[2], "true") == 0)
#endif
		return mWindow->setGridVisible(true);
	else
	{
		return mWindow->setGridVisible(false);
	}
}

bool Server::getMaxDistanceFromPoint(int argc, char* argv[])
{
	if (argc != 8)
	{
		return false;
	}
	double xOrigin = 0, yOrigin = 0, zOrigin = 0;
	bool appendToFile = true;
	// argv[2], argv[3] = input , output file names
	xOrigin = atof(argv[4]);
	yOrigin = atof(argv[5]);
	zOrigin = atof(argv[6]);
	BallAndStickDataManager b;
#ifdef _WIN32
	if (strcmpi(argv[7], "true") == 0)
#else
	if (strcasecmp(argv[7], "true") == 0)
#endif
		return b.getMaxDistanceFromPoint(argv[2], argv[3], xOrigin, yOrigin, zOrigin, true);
	else
	{
		return b.getMaxDistanceFromPoint(argv[2], argv[3], xOrigin, yOrigin, zOrigin, false);
	}
}

bool Server::getHydrophobicityOnSurface(int argc, char* argv[])
{
	if (argc != 11)
	{
		return false;
	}
	char inputPDBFile[256];
	char inputSurfaceFile[256];
	int dim1,dim2,dim3;
	double blobbiness;
	char outputHydrophobicityValues[256];
	PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType;
	int level;
	strcpy(inputPDBFile, argv[2]);
	strcpy(inputSurfaceFile, argv[3]);
	dim1 = atoi(argv[4]);
	dim2 = atoi(argv[5]);
	dim3 = atoi(argv[6]);
	blobbiness = atof(argv[7]);
	strcpy(outputHydrophobicityValues, argv[8]);
	if (!PDBParser::GroupOfAtoms::intToRadiusType(&radiusType, atoi(argv[9])))
	{
		return false;
	}
	level = atoi(argv[10]);
	// make the hydrophobicity volume
	SimpleVolumeData* sData = 0;
	{
		GOALoader* goaLoader = new GOALoader();
		PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(inputPDBFile);
		delete goaLoader;
		if (!molecule)
		{
			return false;
		}
		sData = BlurMapsDataManager::getVolume(molecule, "",
											   dim1, dim2, dim3, PDBParser::GroupOfAtoms::PER_ATOM_HYDROPHOBICITY,
											   false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, radiusType, level);
		delete molecule;
	}
	// read in the isocontour
	SurfaceData* surfaceData = new SurfaceData(0);
	{
	/*Q3Err:CS
		surfaceData->read(inputSurfaceFile);
	*/
		surfaceData->read(QStringList(QString(inputSurfaceFile)));
	}
	// error checking
	{
		if (!sData || !surfaceData)
		{
			delete sData;
			sData = 0;
			delete surfaceData;
			surfaceData = 0;
			return false;
		}
		if (!surfaceData->getGeometry())
		{
			delete sData;
			sData = 0;
			delete surfaceData;
			surfaceData = 0;
			return false;
		}
		if (surfaceData->getGeometry()->m_NumTriVerts < 1)
		{
			delete sData;
			sData = 0;
			delete surfaceData;
			surfaceData = 0;
			return false;
		}
	}
	// get the hydrophobicity values for each vertex
	double* funcVals = new double[surfaceData->getGeometry()->m_NumTriVerts];
	{
		if (!SimpleVolumeDataIsocontourer::getFunctionValues(sData, surfaceData->getGeometry(), funcVals))
		{
			delete funcVals;
			delete sData;
			sData = 0;
			delete surfaceData;
			surfaceData = 0;
			return false;
		}
	}
	// output the values
	{
		FILE* fp = fopen(outputHydrophobicityValues, "w");
		if (!fp)
		{
			delete funcVals;
			delete sData;
			sData = 0;
			delete surfaceData;
			surfaceData = 0;
			return false;
		}
		fprintf(fp, "%d\n", surfaceData->getGeometry()->m_NumTriVerts);
		for (int i=0; i<surfaceData->getGeometry()->m_NumTriVerts; i++)
		{
			fprintf(fp, "%lf\n", funcVals[i]);
		}
		fclose(fp);
	}
	// assign hydrophobicity values to the surface
	{
		if (surfaceData->getGeometry()->m_NumTriVerts < 1)
		{
			return false;
		}
		float r, g, b;
		delete []surfaceData->getGeometry()->m_TriVertColorsTransparent;
		surfaceData->getGeometry()->m_TriVertColorsTransparent = new float[surfaceData->getGeometry()->m_NumTriVerts*4];
		for (int i=0; i<surfaceData->getGeometry()->m_NumTriVerts; i++)
		{
			r = g = b = 1.0;
			double v = funcVals[i];
			if (v < 0)
			{
				g = 1.0 + v*5;
				b = 1.0 + v*5;
				if (v < -0.2)
				{
					g = 0.0;
					b = 0.0;
				}
			}
			if (v > 0)
			{
				r = 1.0 - v*5;
				b = 1.0 - v*5;
				if (v > 0.2)
				{
					r = 0.0;
					b = 0.0;
				}
			}
			surfaceData->getGeometry()->m_TriVertColorsTransparent[4*i+0] = r;
			surfaceData->getGeometry()->m_TriVertColorsTransparent[4*i+1] = g;
			surfaceData->getGeometry()->m_TriVertColorsTransparent[4*i+2] = b;
			surfaceData->getGeometry()->m_TriVertColorsTransparent[4*i+3] = 1.0f;
		}
		GeometryLoader* geometryLoader = new GeometryLoader();
		bool savedMeanCurvatureFile = geometryLoader->saveFile(inputSurfaceFile, "Rawnc files (*.rawnc)", surfaceData->getGeometry());
	}
	delete surfaceData;
	delete funcVals;
	delete sData;
	return true;
}

bool Server::getSurfaceFromPDB(int argc, char* argv[])
{
	if (argc != 13)
	{
		printUsage();
		return false;
	}
	char pdbFileName[256];
	char surfaceFileName[256];
	double isovalue = 1;
	int dim1, dim2, dim3;
	double blobbiness;
	PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType;
	int level;
	strcpy(pdbFileName, argv[2]);
	strcpy(surfaceFileName, argv[3]);
	isovalue = atof(argv[4]);
	dim1 = atoi(argv[5]);
	dim2 = atoi(argv[6]);
	dim3 = atoi(argv[7]);
	bool blurTheVolume = false;
	if (strcmp(argv[8], "true") == 0)
	{
		blurTheVolume = true;
	}
	else if (strcmp(argv[8], "false") == 0)
	{
		blurTheVolume = false;
	}
	blobbiness = atof(argv[9]);
	if (!PDBParser::GroupOfAtoms::intToRadiusType(&radiusType, atoi(argv[10])))
	{
		return false;
	}
	level = atoi(argv[11]);
	bool optimizeRadii = false;
	if (strcmp(argv[12], "true") == 0)
	{
		optimizeRadii = true;
	}
	else if (strcmp(argv[12], "false") == 0)
	{
		optimizeRadii = false;
	}
	Geometry* geometry = 0;
	if (blurTheVolume)
	{
		// create the volume
		SimpleVolumeData* sData = 0;
		{
			GOALoader* goaLoader = new GOALoader();
			PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(pdbFileName);
			delete goaLoader;
			if (!molecule)
			{
				return false;
			}
			if (optimizeRadii)
			{
				sData = BlurMapsDataManager::getMultiLevelBlurredVolume(molecule, "",
						dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
						false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, radiusType,
						level);
			}
			else
			{
				sData = BlurMapsDataManager::getVolume(molecule, "",
													   dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
													   false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, radiusType,
													   level);
			}
			delete molecule;
			if (!sData)
			{
				return false;
			}
		}
		// create the isosurface
		{
			geometry = SimpleVolumeDataIsocontourer::getIsocontour(sData, isovalue);
			if (geometry == 0)
			{
				delete sData;
				return false;
			}
			delete sData;
		}
	}
	else
	{
		GOALoader* goaLoader = new GOALoader();
		PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(pdbFileName);
		delete goaLoader;
		if (!molecule)
		{
			return false;
		}
		geometry = MolecularSurface::getMolecularSurface(molecule, dim1);
	}
	// save the isocontour
	{
		GeometryLoader* geometryLoader = new GeometryLoader();
		if (!geometryLoader->saveFile(surfaceFileName, geometry))
		{
			delete geometry;
			return false;
		}
		delete geometryLoader;
	}
	delete geometry;
	return true;
}

bool Server::createSkinRegion(int argc, char* argv[])
{
	if (argc != 9)
	{
		printUsage();
		return false;
	}
	char inputFileName[256];
	char outputFileName[256];
	int dim1, dim2, dim3;
	double probeRadius;
	PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType;
	int depth;
	strcpy(inputFileName, argv[2]);
	strcpy(outputFileName, argv[3]);
	dim1 = atoi(argv[4]);
	dim2 = atoi(argv[5]);
	dim3 = atoi(argv[6]);
	probeRadius = atof(argv[7]);
	if (!PDBParser::GroupOfAtoms::intToRadiusType(&radiusType, atoi(argv[8])))
	{
		return false;
	}
	depth = 2;
	SimpleVolumeData* sData;
	sData = BlurMapsDataManager::getSkinRegionVolume(inputFileName, outputFileName,
			dim1, dim2, dim3, probeRadius, radiusType, depth);
	bool ret;
	if (sData)
	{
		ret = true;
	}
	else
	{
		ret = false;
	}
	delete sData;
	return ret;
}

bool Server::correlate(int argc, char* argv[])
{
	if (argc != 10)
	{
		printUsage();
		return false;
	}
	char hydroFileName[256];
	char curvFileName[256];
	bool discretizeX;
	bool discretizeY;
	double discreteMaxVal;
	double discreteMinVal;
	int rangeToCorrelate; // 0 -> use all, < 0 -> use neg, > 0 -> use pos
	char outFileName[256];
	strcpy(hydroFileName, argv[2]);
	strcpy(curvFileName, argv[3]);
	if (strcmp(argv[4], "true") == 0)
	{
		discretizeX = true;
	}
	else
	{
		discretizeX = false;
	}
	if (strcmp(argv[5], "true") == 0)
	{
		discretizeY = true;
	}
	else
	{
		discretizeY = false;
	}
	discreteMaxVal = atof(argv[6]);
	discreteMinVal = atof(argv[7]);
	rangeToCorrelate = atoi(argv[8]);
	strcpy(outFileName, argv[9]);
	// read allocate arrays
	double* h = 0;
	double* m = 0;
	double* g = 0;
	int  n = 0;
	{
		FILE* fp1 = fopen(hydroFileName, "r");
		if (!fp1)
		{
			return false;
		}
		if (!fscanf(fp1, "%d\n", &n) == 1)
		{
			return false;
		}
		if (n < 1)
		{
			return false;
		}
		FILE* fp2 = fopen(curvFileName, "r");
		if (!fp2)
		{
			return false;
		}
		int temp;
		if (!fscanf(fp2, "%d\n", &temp) == 1)
		{
			return false;
		}
		if (temp != n)
		{
			return false;
		}
		h = new double[n];
		m = new double[n];
		g = new double[n];
		for (int i=0; i<n; i++)
		{
			if (fscanf(fp1, "%lf\n", &(h[i])) != 1)
			{
				return false;
			}
			if (fscanf(fp2, "%lf %lf\n", &(m[i]), &(g[i])) != 2)
			{
				return false;
			}
		}
		fclose(fp1);
		fclose(fp2);
	}
	// discretize if required
	if (discretizeX)
	{
		if (!CCVOpenGLMath::LinearAlgebra::discretize(h, n, discreteMaxVal, discreteMinVal))
		{
			return false;
		}
	}
	if (discretizeY)
	{
		if (!CCVOpenGLMath::LinearAlgebra::discretize(m, n, discreteMaxVal, discreteMinVal))
		{
			return false;
		}
		if (!CCVOpenGLMath::LinearAlgebra::discretize(g, n, discreteMaxVal, discreteMinVal))
		{
			return false;
		}
	}
	// correllate
	double coef_hydro_mean = 0;
	double coef_hydro_gaus = 0;
	if (!CCVOpenGLMath::LinearAlgebra::selectivelyCorrelate(m, rangeToCorrelate, h, n, &coef_hydro_mean))
	{
		return false;
	}
	if (!CCVOpenGLMath::LinearAlgebra::selectivelyCorrelate(g, rangeToCorrelate, h, n, &coef_hydro_gaus))
	{
		return false;
	}
	// print out result
	{
		FILE* fp = fopen(outFileName,"a");
		fprintf(fp, "---------------------------------------\n");
		fprintf(fp, "Correllating:\n");
		fprintf(fp, "\t%s %s\n", hydroFileName, curvFileName);
		if (discretizeX)
		{
			fprintf(fp, "\tDiscretized hydro with values %lf %lf\n", discreteMaxVal, discreteMinVal);
		}
		if (discretizeY)
		{
			fprintf(fp, "\tDiscretized curvs with values %lf %lf\n", discreteMaxVal, discreteMinVal);
		}
		if (rangeToCorrelate == 0)
		{
			fprintf(fp, "\tCorrellated the entire array\n");
		}
		if (rangeToCorrelate < 0)
		{
			fprintf(fp, "\tCorrellated the negative hydro only\n");
		}
		if (rangeToCorrelate > 0)
		{
			fprintf(fp, "\tCorrelated the positive hydro only\n");
		}
		fprintf(fp, "\tDot product <hydro.mean>,<hydro.gaus> = %lf %lf\n\n", coef_hydro_mean, coef_hydro_gaus);
		fclose(fp);
	}
	return true;
}

bool Server::getArea(int argc, char* argv[])
{
	if (argc < 4)
	{
		printf("Hello\n");
		printUsage();
		return false;
	}
	if (argc == 4)
	{
		// read in the isocontour
		SurfaceData* surfaceData = new SurfaceData(0);
		{
		/*Q3Err:CS
			if (!surfaceData->read(argv[2]))
		*/
			if (!surfaceData->read(QStringList(QString(argv[2]))))
			{
				delete surfaceData;
				surfaceData = 0;
				printf("Could not read file %s\n", argv[2]);
				return false;
			}
		}
		// /
		// calculate surface area and print it out
		{
			FILE* fp  = fopen(argv[3], "a");
			if (!fp)
			{
				delete surfaceData;
				return false;
			}
			fprintf(fp, "%lf\n", surfaceData->getArea());
			delete surfaceData;
			fclose(fp);
		}
		//
	}
	else if (argc == 5)
	{
	  /*
		Docking::DockComplex* complex = new Docking::DockComplex();
		bool deleteWater = false;
		if (!complex->init(argv[2], argv[3], deleteWater))
		{
			delete complex;
			return false;
		}
		double area = complex->getInterfaceArea(4.0);
		delete complex;
		// calculate surface area and print it out
		{
			FILE* fp  = fopen(argv[4], "a");
			if (!fp)
			{
				return false;
			}
			fprintf(fp, "%lf\n", area);
			fclose(fp);
		}
		//
	   */
	}
	return true;
}
bool Server::getVolumeN(int argc, char* argv[])
{
	if (argc != 4)
	{
		printUsage();
		return false;
	}
	// read in the isocontour
	SurfaceData* surfaceData = new SurfaceData(0);
	{
	/*Q3Err:CS
		if (!surfaceData->read(argv[2]))
	*/
		if (!surfaceData->read(QStringList(QString(argv[2]))))
		{
			delete surfaceData;
			surfaceData = 0;
			printf("Could not read file %s\n", argv[2]);
			return false;
		}
	}
	// /
	{
		FILE* fp = fopen(argv[3], "a");
		if (!fp)
		{
			delete surfaceData;
			return false;
		}
		fprintf(fp, "%lf\n", surfaceData->getVolumeN());
		delete surfaceData;
		fclose(fp);
	}
}

bool Server::getVolume(int argc, char* argv[])
{
	if (argc != 4)
	{
		printUsage();
		return false;
	}
	// read in the isocontour
	SurfaceData* surfaceData = new SurfaceData(0);
	{
	/*Q3Err:CS
		if (!surfaceData->read(argv[2]))
	*/
		if (!surfaceData->read(QStringList(QString(argv[2]))))
		{
			delete surfaceData;
			surfaceData = 0;
			printf("Could not read file %s\n", argv[2]);
			return false;
		}
	}
	// construct sdf function
	int size = 64;
	SimpleVolumeData* sData = SDFLibrary::getSDF(surfaceData->getGeometry(), size);
	if (!sData)
	{
		return false;
	}
	delete surfaceData;
	surfaceData = 0;
	// calculate volume and print it out
	{
		int dim[3];
		dim[0] = sData->getWidth();
		dim[1] = sData->getHeight();
		dim[2] = sData->getDepth();
		float span[3];
		span[0] = sData->getSpanX();
		span[1] = sData->getSpanY();
		span[2] = sData->getSpanZ();
		float orig[3];
		orig[0] = sData->getMinX();
		orig[1] = sData->getMinY();
		orig[2] = sData->getMinZ();
		float* vol = 0;
		if (!sData->getFloatData(&vol))
		{
			delete sData;
			printf("-1\n");
			return false;
		}
		delete sData;
		{
			//invert it to get correct volume
			for (int i=0; i<dim[0]*dim[1]*dim[2]; i++)
			{
				vol[i] *= -1;
			}
		}
		double area = 0;
		double volume = 0;
		double isovalue = 0;
		AreaVolume* areaVolume = new AreaVolume();
		areaVolume->getVolume(vol, isovalue, dim, orig, span, &area, &volume);
		delete areaVolume;
		if (vol)
		{
			delete []vol;
		}
		{
			FILE* fp  = fopen(argv[3], "a");
			if (!fp)
			{
				return false;
			}
			fprintf(fp, "%lf\n", volume);
			fclose(fp);
		}
	}
	return true;
}

bool Server::getPatchAreas(int argc, char* argv[])
{
	if (argc != 6)
	{
		printUsage();
		return false;
	}
	double isovalue;
	isovalue = atof(argv[4]);
	// read in the isocontour
	SurfaceData* surfaceData = new SurfaceData(0);
	{
	/*Q3Err:CS
		if (!surfaceData->read(argv[2]))
	*/
		if (!surfaceData->read(QStringList(QString(argv[2]))))
		{
			delete surfaceData;
			surfaceData = 0;
			printf("Could not read file %s\n", argv[2]);
			return false;
		}
		if (!surfaceData->getGeometry())
		{
			delete surfaceData;
			surfaceData = 0;
			printf("Could not read file %s\n", argv[2]);
			return false;
		}
	}
	// read the function values
	double* functionAtVertices = 0;
	{
		FILE* fp = fopen(argv[3], "r");
		if (!fp)
		{
			delete surfaceData;
			surfaceData = 0;
			printf("Could not read file %s\n", argv[3]);
			return false;
		}
		int numVerts = 0;
		fscanf(fp, "%d\n", &numVerts);
		if ((numVerts < 1) || (numVerts!=surfaceData->getGeometry()->m_NumTriVerts))
		{
			delete surfaceData;
			surfaceData = 0;
			printf("Number of vertices in file %s was wrong\n", argv[3]);
			return false;
		}
		functionAtVertices = new double[numVerts];
		for (int i=0; i<numVerts; i++)
		{
			if (fscanf(fp, "%lf\n", &(functionAtVertices[i])) != 1)
			{
				delete surfaceData;
				surfaceData = 0;
				delete []functionAtVertices;
				functionAtVertices = 0;
				printf("Line %d in file %s was wrong\n", i+1, argv[3]);
				return false;
			}
		}
		fclose(fp);
	}
	// calculate surface areas and print it out
	double areaBelow = 0;
	double areaAbove = 0;
	if (!surfaceData->getAreas(isovalue, functionAtVertices, &areaBelow, &areaAbove))
	{
		printf("Could not compute area of surface %s\n", argv[2]);
		delete surfaceData;
		surfaceData = 0;
		delete []functionAtVertices;
		functionAtVertices = 0;
		return false;
	}
	{
		FILE* fp = fopen(argv[5], "a");
		if (!fp)
		{
			printf("Could not open file%s\n", argv[5]);
			delete surfaceData;
			surfaceData = 0;
			delete []functionAtVertices;
			functionAtVertices = 0;
			return false;
		}
		fprintf(fp, "Surface: %s,   Isovalue: %lf,  Area below, above = %lf, %lf\n", argv[2], isovalue, areaBelow, areaAbove);
		fclose(fp);
	}
	//
	delete []functionAtVertices;
	functionAtVertices = 0;
	delete surfaceData;
	surfaceData = 0;
	return true;
}

bool Server::populateSAS(int argc, char* argv[])
{
	if (argc != 9)
	{
		printUsage();
		return false;
	}
	char inputFileName[256];
	char outputFileName[256];
	int dim1, dim2, dim3;
	double probeRadius;
	double floatingBandWidth;
	PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType;
	strcpy(inputFileName, argv[2]);
	strcpy(outputFileName, argv[3]);
	dim1 = atoi(argv[4]);
	dim2 = atoi(argv[5]);
	dim3 = atoi(argv[6]);
	probeRadius = atof(argv[7]);
	floatingBandWidth = atof(argv[8]);
	if (!PDBParser::GroupOfAtoms::intToRadiusType(&radiusType, atoi(argv[9])))
	{
		return false;
	}
	return BlurMapsDataManager::populateSAS(inputFileName, outputFileName, dim1, dim2, dim3, probeRadius, floatingBandWidth, radiusType);
}

bool Server::addVolumes(int argc, char* argv[])
{
	if (argc != 7)
	{
		printUsage();
		return false;
	}
	char inputFileName1[256];
	char inputFileName2[256];
	char outputFileName[256];
	double scale;
	double sum;
	strcpy(inputFileName1, argv[2]);
	strcpy(inputFileName2, argv[3]);
	strcpy(outputFileName, argv[4]);
	scale = atof(argv[5]);
	sum = atof(argv[6]);
	VolumeLoader* volumeLoader1 = new VolumeLoader();
	SimpleVolumeData* sData1 = volumeLoader1->loadFile(inputFileName1);
	delete volumeLoader1;
	if (!sData1)
	{
		return false;
	}
	VolumeLoader* volumeLoader2 = new VolumeLoader();
	SimpleVolumeData* sData2 = volumeLoader2->loadFile(inputFileName2);
	delete volumeLoader2;
	if (!sData2)
	{
		delete sData1;
		return false;
	}
	if (!sData1->addVolume(sData2, scale, sum))
	{
		delete sData1;
		delete sData2;
		return false;
	}
	VolumeLoader* volumeLoader = new VolumeLoader();
	bool ret = volumeLoader->saveFile(outputFileName, sData1);
	delete volumeLoader;
	delete sData1;
	delete sData2;
	return ret;
}

bool getExtension(char* filename, char* extension)
{
	int l = strlen(filename);
	int i;
	for (i=l-1; i>=0; i--)
	{
		if (filename[i] == '.')
		{
			break;
		}
	}
	if (i == -1)
	{
		return false;
	}
	if (i == l-1)
	{
		return false;
	}
	{
		int j;
		for (j=i+1; j<l; j++)
		{
			extension[j-(i+1)] = filename[j];
		}
		extension[j-(i+1)] =  '\0';
	}
	return true;
}

bool Server::convert(int argc, char* argv[])
{
	if (argc < 4)
	{
		printUsage();
		return false;
	}
	char inputFileName[256];
	char outputFileName[256];
	char inputFileNameExtension[256];
	char outputFileNameExtension[256];
	strcpy(inputFileName, argv[2]);
	strcpy(outputFileName, argv[3]);
	if (!getExtension(inputFileName, inputFileNameExtension))
	{
		return false;
	}
	if (!getExtension(outputFileName, outputFileNameExtension))
	{
		return false;
	}
	{
		VolumeLoader* vLoader = new VolumeLoader();
		if (vLoader->isValidExtension(inputFileNameExtension) && vLoader->isValidExtension(outputFileNameExtension))
		{
			bool ret = vLoader->saveFile(outputFileName, vLoader->loadFile(inputFileName));
			delete vLoader;
			return ret;
		}
		delete vLoader;
	}
	{
		GeometryLoader* gLoader = new GeometryLoader();
		if (gLoader->isValidExtension(inputFileNameExtension) && gLoader->isValidExtension(outputFileNameExtension))
		{
			if (argc == 4)
			{
				bool ret = gLoader->saveFile(outputFileName, gLoader->loadFile(inputFileName));
				delete gLoader;
				return ret;
			}
			else
			{
				Geometry* geometry = gLoader->loadFile(inputFileName);
				SurfaceData* surfaceData = new SurfaceData(0);
				surfaceData->readTransformations(argv[4]);
				surfaceData->setGeometry(geometry);
				Geometry* dup_geometry = surfaceData->createDuplicates();
				delete surfaceData;
				bool ret = gLoader->saveFile(outputFileName, dup_geometry);
				delete gLoader;
				return ret;
			}
		}
		delete gLoader;
	}
	{
		GOALoader* gLoader = new GOALoader();
		if (gLoader->isValidExtension(inputFileNameExtension) && gLoader->isValidExtension(outputFileNameExtension))
		{
			bool ret = gLoader->saveFile(outputFileName, gLoader->loadFile(inputFileName), PDBParser::ATOM_TYPE, 0);
			delete gLoader;
			return ret;
		}
		delete gLoader;
	}
	return false;
}

bool isClose(double x1, double y1, double z1, double r, double x2, double y2, double z2, double* d)
{
	double d1 = (x1-x2)*(x1-x2) +
				(y1-y2)*(y1-y2) +
				(z1-z2)*(z1-z2);
	*d = sqrt(d1);
	if (d1 < r*r)
	{
		//printf( "dist = %lf r = %lf\n", sqrt(d1), r );
		return true;
	}
	return false;
}
//Creates:
//1. A file as input for the docking code.
//2. A pdb with a corresponding ".bdy" file.
bool Server::getSurfaceAtoms(int argc, char* argv[])
{
	if (argc != 4)
	{
		printUsage();
		return false;
	}
	char inputFileName[256];
	char outputFileName[256];
	strcpy(inputFileName, argv[2]);
	strcpy(outputFileName, argv[3]);
	GOALoader* gLoader = new GOALoader();
	PDBParser::GroupOfAtoms* molecule = gLoader->loadFile(inputFileName);
	delete gLoader;
	if (!molecule)
	{
		return false;
	}
	double probeRadius = 1.4;
	PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType = PDBParser::GroupOfAtoms::VDW_RADIUS;
	bool* boundaryAtom = 0;
	int depth = 1;
	int dim1 = 64;
	int dim2 = 64;
	int dim3 = 64;
	/*SimpleVolumeData* sData = new SimpleVolumeData(dim1, dim2, dim3);
	SkinRegion2* skin = new SkinRegion2();
	if( !skin->getSkinRegion( molecule, dim1, dim2, dim3, probeRadius, radiusType, sData, depth ) )
	{
		delete sData;
		delete skin;
		delete molecule;
		return false;
	}
	delete skin;

	Geometry* geometry = 0;
	{
		float isovalue = 1.2f;
		geometry = SimpleVolumeDataIsocontourer::getIsocontour(sData, isovalue);
		if( !geometry )
		{
			delete molecule;
			delete sData;
			return false;
		}
	}
	delete sData;

	// the code below breaks up isocontour into separate components
	vector<Geometry *> components;

	geometry->separateComponents( &components );
	delete geometry;
	if( components.size() < 1 )
	{
		int i;
		for( i=0; i<components.size(); i++ )
			delete components[i];
		components.clear();
		delete molecule;
		return false;
	}

	Geometry* largestComponent = 0;
	{
		int curSize = components[0]->m_NumTris;
		largestComponent = components[0];
		int i;
		for( i=1; i<components.size(); i++ )
		{
			if( components[i]->m_NumTris > curSize )
			{
				curSize = components[i]->m_NumTris;
				largestComponent = components[i];
			}
		}
		for( i=1; i<components.size(); i++ )
		{
			if( components[i] != largestComponent )
				delete components[i];
		}
		components.clear();
	}
	*/
	Geometry* largestComponent = 0;
	{
		float blobbiness = -1.0f;
		int level = PDBParser::ATOM_TYPE ;
		PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType = PDBParser::GroupOfAtoms::VDW_RADIUS;
		SimpleVolumeData* sData = 0;
		{
			GOALoader* goaLoader = new GOALoader();
			PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(inputFileName);
			delete goaLoader;
			if (!molecule)
			{
				return false;
			}
			sData = BlurMapsDataManager::getVolume(molecule, "", 128, 128, 128, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,	false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, radiusType, level);
			delete molecule;
			if (!sData)
			{
				return false;
			}
		}
		largestComponent = SimpleVolumeDataIsocontourer::getIsocontour(sData, 1.0);
	}
	// find surface atoms of molecule as those close to the largest component
	vector<PDBParser::Atom*> atomList;
	{
		PDBParser::CollectionData* collectionData = 0;
		if (molecule->type == PDBParser::COLLECTION_TYPE)
		{
			collectionData = molecule->m_CollectionData;
		}
		PDBParser::FlattenGOA(molecule, atomList, collectionData, 0, 0, 0, radiusType, PDBParser::ATOM_TYPE, false);
	}
	boundaryAtom = new bool[atomList.size()];
	{
		for (int i=0; i<atomList.size(); i++)
		{
			boundaryAtom[i] = false;
			for (int v=0; v<largestComponent->m_NumTriVerts; v++)
			{
				double dist = 100;
				if (isClose(atomList[i]->m_Position[0],
							atomList[i]->m_Position[1],
							atomList[i]->m_Position[2],
							atomList[i]->getRadius() + 1.0,
							largestComponent->m_TriVerts[v*3+0],
							largestComponent->m_TriVerts[v*3+1],
							largestComponent->m_TriVerts[v*3+2],
							&dist
						   ))
				{
					//printf("Atom %d is %lf away from surface\n", i, dist );
					boundaryAtom[i] = true;
					break;
				}
			}
		}
	}
	delete molecule;
	delete largestComponent;
	{
		int numBdyAtoms = 0;
		// the boundary atoms file takes in surface area exposed for the given atom.
		char bdyFile[256];
		strcpy(bdyFile, inputFileName);
		strcat(bdyFile, ".bdy");
		FILE* fp = fopen(bdyFile, "w");
		char interiorfname[256];
		strcpy(interiorfname, outputFileName);
		strcat(interiorfname, "_interior.pdb");
		FILE* fpOut = fopen(interiorfname, "w");
		char skinfname[256];
		strcpy(skinfname, outputFileName);
		strcat(skinfname, "_skin.pdb");
		FILE* fpSkin = fopen(skinfname,"w");
		for (int i=0; i<atomList.size(); i++)
		{
			if (boundaryAtom[i])
			{
				fprintf(fp, "100000.0\n");
				fprintf(fpSkin, "ATOM  11111  CA  GLU A1111    %8.3f%8.3f%8.3f%6.2f                \n", atomList[i]->m_Position[0], atomList[i]->m_Position[1], atomList[i]->m_Position[2], atomList[i]->getRadius());
				numBdyAtoms++;
			}
			else
			{
				fprintf(fp, "0.0\n");
				fprintf(fpOut, "ATOM  11111  N   GLU A1111    %8.3f%8.3f%8.3f%6.2f                \n", atomList[i]->m_Position[0], atomList[i]->m_Position[1], atomList[i]->m_Position[2], atomList[i]->getRadius());
			}
		}
		fclose(fp);
		fclose(fpOut);
		fclose(fpSkin);
		{
			// write docking input file
			FILE* fp = fopen(outputFileName, "w");
			if (!fp)
			{
				return false;
			}
			fprintf(fp, "%lu\n", atomList.size());
			for (int i=0; i<atomList.size(); i++)
			{
				if (boundaryAtom[i])
				{
					continue;
				}
				PDBParser::Atom* at = atomList[i];
				fprintf(fp, "I     %12.5lf %12.5lf %12.5lf %12.5f\n", at->m_Position[0], at->m_Position[1], at->m_Position[2], at->getCharge());
			}
			for (int i=0; i<atomList.size(); i++)
			{
				if (boundaryAtom[i])
				{
					PDBParser::Atom* at = atomList[i];
					fprintf(fp, "E     %12.5lf %12.5lf %12.5lf %12.5f\n", at->m_Position[0], at->m_Position[1], at->m_Position[2], at->getCharge());
				}
			}
			fclose(fp);
		}
	}
	for (int i=0; i<atomList.size(); i++)
	{
		delete atomList[i];
	}
	atomList.clear();
	if (boundaryAtom)
	{
		delete [] boundaryAtom;
		boundaryAtom = 0;
	}
	return true;
}

/*void Server::getContourSpectrum(unsigned char* uchar_data, int type, int* dim, int array_size, float* isoval , float* area, float* min_vol, float* max_vol, float* gradient)
{
	ConDataset* the_data;
	the_data = newDatasetReg(type, CONTOUR_REG_3D, 1, 1, dim,uchar_data);

	Signature       *sig;
	sig=getSignatureFunctions(the_data, 0,0);

	for (int i=0;i<array_size;i++) {
		isoval[i]=sig[0].fx[i];
		area[i]=sig[0].fy[i];
		min_vol[i]=sig[1].fy[i];
		max_vol[i]=sig[2].fy[i];
		gradient[i]=sig[3].fy[i];
	}

	delete the_data;
	delete sig;
}*/

bool Server::getContourStats(int argc, char* argv[])
{
	if (argc != 4 && argc != 5)
	{
		printUsage();
		return false;
	}
	char inputFileName[256];
	char outputFileName[256];
	strcpy(inputFileName, argv[2]);
	strcpy(outputFileName, argv[3]);
	SimpleVolumeData* sData = 0;
	double isovalue = 1;
	if (argc == 4)
	{
		bool useBlurring = true;
		// create the volume //
		int dim1 = 128, dim2 = 128, dim3 = 128;
		if (useBlurring)
		{
			float blobbiness = -2.3f;
			int level = PDBParser::ATOM_TYPE ;
			PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType = PDBParser::GroupOfAtoms::VDW_RADIUS;
			{
				GOALoader* goaLoader = new GOALoader();
				PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(inputFileName);
				delete goaLoader;
				if (!molecule)
				{
					return false;
				}
				sData = BlurMapsDataManager::getVolume(molecule, "", dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY, false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, radiusType, level);
				delete molecule;
				if (!sData)
				{
					return false;
				}
			}
		}
		else
		{
			double probeRadius = 1.4;
			isovalue = probeRadius;
			int depth = 3;
			dim1 = 64, dim2 = 64, dim3 = 64;
			GOALoader* gLoader = new GOALoader();
			PDBParser::GroupOfAtoms* molecule = gLoader->loadFile(inputFileName);
			delete gLoader;
			if (!molecule)
			{
				return false;
			}
			sData = new SimpleVolumeData(dim1, dim2, dim3);
			SkinRegion2* skin = new SkinRegion2();
			if (!skin->getSkinRegion(molecule, dim1, dim2, dim3, probeRadius, PDBParser::GroupOfAtoms::VDW_RADIUS, sData, depth))
			{
				delete sData;
				sData = 0;
				delete skin;
				skin = 0;
				return false;
			}
			delete skin;
		}
	}
	else
	{
		VolumeLoader* vLoader = new VolumeLoader();
		sData = vLoader->loadFile(inputFileName);
		delete vLoader;
		if (!sData)
		{
			return false;
		}
		isovalue = atof(argv[4]);
	}
	// get the contour spectrum
	if (sData->getNumberOfVariables() < 0)
	{
		return false;
	}
	int type;
	void* data;
	double minVal = 0, maxVal = 1;
	if (sData->getNumberOfVariables() < 4)
	{
		// rawiv
		data = sData->getData(0);
		type = sData->getType(0);
		if (!sData ->getMinMax(0, &minVal, &maxVal))
		{
			return false;
		}
	}
	else
	{
		// rawv
		data = sData->getData(3);
		type = sData->getType(3);
		if (!sData->getMinMax(3, &minVal, &maxVal))
		{
			return false;
		}
	}
	int dim[3];
	dim[0] = sData->getWidth();
	dim[1] = sData->getHeight();
	dim[2] = sData->getDepth();
	float span[3];
	span[0] = sData->getSpanX();
	span[1] = sData->getSpanY();
	span[2] = sData->getSpanZ();
	float orig[3];
	orig[0] = sData->getMinX();
	orig[1] = sData->getMinY();
	orig[2] = sData->getMinZ();
	float* vol = 0;
	{
		vol = new float[dim[0]*dim[1]*dim[2]];
		switch (type)
		{
		case SimpleVolumeData::UCHAR:
		{
			unsigned char* ucData = (unsigned char*)data;
			for (int i=0; i<dim[0]*dim[1]*dim[2]; i++)
			{
				vol[i] = ucData[i];
			}
		}
		break;
		case SimpleVolumeData::USHORT:
		{
			unsigned short* usData = (unsigned short*)data;
			for (int i=0; i<dim[0]*dim[1]*dim[2]; i++)
			{
				vol[i] = usData[i];
			}
		}
		break;
		case SimpleVolumeData::ULONG:
		{
			unsigned long* ulData = (unsigned long*)data;
			for (int i=0; i<dim[0]*dim[1]*dim[2]; i++)
			{
				vol[i] = ulData[i];
			}
		}
		break;
		case SimpleVolumeData::FLOAT:
		{
			float* fData = (float*)data;
			for (int i=0; i<dim[0]*dim[1]*dim[2]; i++)
			{
				vol[i] = fData[i];
			}
		}
		break;
		/*case SimpleVolumeData::DOUBLE:
			{
				double* dData = (double*)data;
				for( i=0; i<dim[0]*dim[1]*dim[2]; i++ )
					vol[i] = dData[i];
			}
			break;
		*/
		default:
			return false;
		}
	}
	Geometry* geometry = SimpleVolumeDataIsocontourer::getIsocontour(sData, 1.0);
	delete sData;
	SurfaceData* surfaceData = new SurfaceData(0);
	{
		surfaceData->setGeometry(geometry);
	}
	double area = 0;
	double volume = 0;
	AreaVolume* areaVolume = new AreaVolume();
	areaVolume->getVolume(vol, isovalue, dim, orig, span, &area, &volume);
	delete areaVolume;
	FILE* fp = fopen(outputFileName, "a");
	fprintf(fp, "area, volume = %015lf %015lf\n", surfaceData->getArea(), volume);
	fclose(fp);
	delete surfaceData;
	return true;
}

bool Server::downloadPDB(int argc, char* argv[], MainWindow* mWindow)
{
	if (argc != 4)
	{
		printUsage();
		return false;
	}
	char pdbID[256];
	char outputFileName[256];
	strcpy(pdbID, argv[2]);
	strcpy(outputFileName, argv[3]);
	DownloadPDB* downloadPDB = new DownloadPDB();
	if (!downloadPDB->blockedDownload(pdbID, outputFileName))
	{
		delete downloadPDB;
		return false;
	}
	return true;
}

bool Server::getSignDistanceFunction(int argc, char* argv[])
{
	if (argc != 6)
	{
		printUsage();
		return false;
	}
	char inputFileName[256];
	char outputVolumeFileName[256];
	int size = 32;
	int flipNormals = 0;
	strcpy(inputFileName, argv[2]);
	strcpy(outputVolumeFileName, argv[3]);
	size = atoi(argv[4]);
	flipNormals = atoi(argv[5]);
	char inputFileNameExtension[256];
	if (!getExtension(inputFileName, inputFileNameExtension))
	{
		return false;
	}
	Geometry* geometry = 0;
	if (!geometry)
	{
		VolumeLoader* vLoader = new VolumeLoader();
		if (vLoader->isValidExtension(inputFileNameExtension))
		{
			// load volume, isocontour it.
			SimpleVolumeData* sData = vLoader->loadFile(inputFileName);
			if (!sData)
			{
				return false;
			}
			geometry = SimpleVolumeDataIsocontourer::getIsocontour(sData, 1.0);
		}
		delete vLoader;
	}
	if (!geometry)
	{
		GeometryLoader* gLoader = new GeometryLoader();
		if (gLoader->isValidExtension(inputFileNameExtension))
		{
			GeometryLoader* gLoader = new GeometryLoader();
			geometry = gLoader->loadFile(inputFileName);
			delete gLoader;
		}
		delete gLoader;
	}
	if (!geometry)
	{
		GOALoader* gLoader = new GOALoader();
		if (gLoader->isValidExtension(inputFileNameExtension))
		{
			// load goa. Blur it. Isocontour it.
			SimpleVolumeData* sData = 0;
			GOALoader* goaLoader = new GOALoader();
			PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(inputFileName);
			delete goaLoader;
			if (!molecule)
			{
				return false;
			}
			sData = BlurMapsDataManager::getVolume(molecule, "", 64, 64, 64, PDBParser::GroupOfAtoms::ELECTRON_DENSITY, false, -2.3, PDBParser::GroupOfAtoms::ATOM, 0, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, 0);
			delete molecule;
			if (!sData)
			{
				return false;
			}
			geometry = SimpleVolumeDataIsocontourer::getIsocontour(sData, 1.0);
			delete sData;
		}
		delete gLoader;
	}
	if (!geometry)
	{
		return false;
	}
	SimpleVolumeData* sData = SDFLibrary::getSDF(geometry, size);
	if (!sData)
	{
		return false;
	}
	VolumeLoader* vLoader = new VolumeLoader();
	bool ret = vLoader->saveFile(outputVolumeFileName, sData);
	delete vLoader;
	delete sData;
	return ret;
}

bool Server::getMolecularCharacteristics(int argc, char* argv[])
{
	if (argc < 6)
	{
		printUsage();
		return false;
	}
	char inputGOAFileName[256];
	char inputElecVolumeFileName[256];
	char outputFileName[256];
	int numberOfSpheres = 0;
	float* activeSite = 0;
	double* radii = 0;
	strcpy(inputGOAFileName, argv[2]);
	strcpy(inputElecVolumeFileName, argv[3]);
	strcpy(outputFileName, argv[4]);
	numberOfSpheres = atoi(argv[5]);
	if (numberOfSpheres > 0)
	{
		if (numberOfSpheres + 6 + 3 != argc)
		{
			return false;
		}
		activeSite = new float[3];
		radii = new double[numberOfSpheres];
		activeSite[0] = atof(argv[6]);
		activeSite[1] = atof(argv[7]);
		activeSite[2] = atof(argv[8]);
		for (int i=0; i<numberOfSpheres; i++)
		{
			radii[i] = atof(argv[9+i]);
		}
	}
	// note, molecule OR volume can be NULL !
	GOALoader* gLoader = new GOALoader();
	PDBParser::GroupOfAtoms* molecule = gLoader->loadFile(inputGOAFileName);
	delete gLoader;
	VolumeLoader* volumeLoader = new VolumeLoader();
	SimpleVolumeData* electrostatics = volumeLoader->loadFile(inputElecVolumeFileName);
	delete volumeLoader;
	if (!molecule && !electrostatics)
	{
		if (activeSite)
		{
			delete []activeSite;
			activeSite = 0;
		}
		if (radii)
		{
			delete []radii;
			radii = 0;
		}
		return false;
	}
	MolecularCharacteristics* molecularCharacteristics = new MolecularCharacteristics(
		molecule,
		electrostatics,
		numberOfSpheres,
		activeSite,
		radii,
		inputGOAFileName,
		inputElecVolumeFileName);
	if (!molecularCharacteristics->appendHeader(outputFileName))
	{
		if (activeSite)
		{
			delete []activeSite;
			activeSite = 0;
		}
		if (radii)
		{
			delete []radii;
			radii = 0;
		}
		return false;
	}
	if (!molecularCharacteristics->appendCharacteristics(outputFileName))
	{
		if (activeSite)
		{
			delete []activeSite;
			activeSite = 0;
		}
		if (radii)
		{
			delete []radii;
			radii = 0;
		}
		return false;
	}
	if (activeSite)
	{
		delete []activeSite;
		activeSite = 0;
	}
	if (radii)
	{
		delete []radii;
		radii = 0;
	}
	return true;
}

bool Server::expandMolecule(int argc, char* argv[])
{
	// read a GOA file. Apply the transformations and save into a new GOA file
	if (argc < 4)
	{
		printUsage();
		return false;
	}
	char inputGOAFileName[256];
	char outputGOAFileName[256];
	strcpy(inputGOAFileName, argv[2]);
	strcpy(outputGOAFileName, argv[3]);
	PDBParser::GroupOfAtoms* molecule = 0;
	// read file
	{
		GOALoader* gLoader = new GOALoader();
		molecule = gLoader->loadFile(inputGOAFileName);
		delete gLoader;
		if (!molecule)
		{
			return false;
		}
	}
	vector<PDBParser::Atom*> atomList;
	// "flatten" the GOA
	PDBParser::CollectionData* collectionData = 0;
	if (molecule->type == PDBParser::COLLECTION_TYPE)
	{
		collectionData = molecule->m_CollectionData;
	}
	PDBParser::FlattenGOA(molecule, atomList, collectionData, 0, 0, 0,
									PDBParser::GroupOfAtoms::VDW_RADIUS, PDBParser::GroupOfAtoms::ATOM, false);
	// write file
	{
		FILE* fp = fopen(outputGOAFileName, "w");
		if (fp == 0)
		{
			return false;
		}
		for (int i=0; i<atomList.size(); i++)
		{
			PDBParser::printAtomPDB(fp, atomList[i], 0);
		}
		fclose(fp);
	}
	{
		std::vector<PDBParser::Atom*>::iterator iter = atomList.begin(), end = atomList.end();
		for (; iter != end; ++iter)
		{
			delete *iter;
		}
		atomList.clear();
	}
	return true;
}

bool Server::mergeGeometry(int argc, char* argv[])
{
	if (argc < 5)
	{
		printUsage();
		return false;
	}
	char inputSurfaceFileName1[256];
	char inputSurfaceFileName2[256];
	char outputSurfaceFileName[256];
	strcpy(inputSurfaceFileName1, argv[2]);
	strcpy(inputSurfaceFileName2, argv[3]);
	strcpy(outputSurfaceFileName, argv[4]);
	Geometry* geometry1 = 0;
	Geometry* geometry2 = 0;
	/*{
		GeometryLoader* gLoader = new GeometryLoader();
		geometry1 = gLoader->loadFile( inputSurfaceFileName1 );
		delete gLoader;
		if( !geometry1 ) return false;
	}

	{
		GeometryLoader* gLoader = new GeometryLoader();
		geometry2 = gLoader->loadFile( inputSurfaceFileName2 );
		delete gLoader;
		if( !geometry2 )
		{
			delete geometry1;
			return false;
		}
	}

	Geometry* mergedGeometry = geometry1->merge( geometry2);
	if( !mergedGeometry )
	{
		delete geometry1;
		delete geometry2;
		return false;
	}
	*/
	Geometry* mergedGeometry = 0;
	{
		GeometryLoader* gLoader = new GeometryLoader();
		mergedGeometry = gLoader->loadFile("Z:/NMJDatasets/1C2B/1C2B_merged.rawnc");
		delete gLoader;
		if (!mergedGeometry)
		{
			return false;
		}
	}
	Geometry* tg = 0;
	{
		for (int i=2; i<=140; i++)
		{
			GeometryLoader* gLoader = new GeometryLoader();
			sprintf(inputSurfaceFileName2, "Z:/NMJDatasets/1C2B/1C2B_%d.rawnc", i);
			geometry1 = gLoader->loadFile(inputSurfaceFileName2);
			delete gLoader;
			if (!geometry1)
			{
				delete mergedGeometry;
				return false;
			}
			tg = mergedGeometry->merge(geometry1);
			delete mergedGeometry;
			mergedGeometry = 0;
			delete geometry1;
			geometry1 = 0;
			mergedGeometry = tg;
			printf("Done %d out of 140 of 1c2b\n", i);
		}
	}
	{
		for (int i=0; i<=337; i++)
		{
			GeometryLoader* gLoader = new GeometryLoader();
			sprintf(inputSurfaceFileName2, "Z:/NMJDatasets/2BG9/2BG9_%d.rawnc", i);
			geometry1 = gLoader->loadFile(inputSurfaceFileName2);
			delete gLoader;
			if (!geometry1)
			{
				delete mergedGeometry;
				return false;
			}
			tg = mergedGeometry->merge(geometry1);
			delete mergedGeometry;
			mergedGeometry = 0;
			delete geometry1;
			geometry1 = 0;
			mergedGeometry = tg;
			printf("Done %d out of 337 of 2bg9\n", i);
		}
	}
	{
		for (int i=0; i<=42; i++)
		{
			GeometryLoader* gLoader = new GeometryLoader();
			sprintf(inputSurfaceFileName2, "Z:/NMJDatasets/2BG9/2BG9_bottom_%d.rawnc", i);
			geometry1 = gLoader->loadFile(inputSurfaceFileName2);
			delete gLoader;
			if (!geometry1)
			{
				delete mergedGeometry;
				return false;
			}
			tg = mergedGeometry->merge(geometry1);
			delete mergedGeometry;
			mergedGeometry = 0;
			delete geometry1;
			geometry1 = 0;
			mergedGeometry = tg;
			printf("Done %d out of 42 of 2bg9_bottom\n", i);
		}
	}
	{
		GeometryLoader* gLoader = new GeometryLoader();
		bool ret = gLoader->saveFile(outputSurfaceFileName, mergedGeometry);
		delete gLoader;
		delete geometry1;
		delete geometry2;
		delete mergedGeometry;
		if (!ret)
		{
			return false;
		}
	}
	return true;
}

bool Server::mergeVolumes(int argc, char* argv[])
{
	if (argc != 5 && argc != 6 && argc != 11)
	{
		printUsage();
		return false;
	}
	char inputVolumeFileName1[256];
	char inputVolumeFileName2[256];
	char outputVolumeFileName[256];
	strcpy(inputVolumeFileName1, argv[2]);
	strcpy(inputVolumeFileName2, argv[3]);
	strcpy(outputVolumeFileName, argv[4]);
	bool transform = (argc == 5) ? false : true;
	SimpleVolumeData* sData1 = 0;
	SimpleVolumeData* sData2 = 0;
	{
		VolumeLoader* volumeLoader1 = new VolumeLoader();
		sData1 = volumeLoader1->loadFile(inputVolumeFileName1);
		delete volumeLoader1;
		if (!sData1)
		{
			return false;
		}
		/*// only for NMJ project!
		{
			float minExt[3];
			minExt[0] = sData1->getMinX() * 150;
			minExt[1] = sData1->getMinY() * 150;
			minExt[2] = sData1->getMinZ() * 150;
			sData1->setMinExtent( minExt );
		}
		{
			float maxExt[3];
			maxExt[0] = sData1->getMaxX() * 150;
			maxExt[1] = sData1->getMaxY() * 150;
			maxExt[2] = sData1->getMaxZ() * 150;
			sData1->setMaxExtent( maxExt );
		}
		*/
		VolumeLoader* volumeLoader2 = new VolumeLoader();
		sData2 = volumeLoader2->loadFile(inputVolumeFileName2);
		delete volumeLoader2;
		if (!sData2)
		{
			return false;
		}
	}
	FILE* fp = fopen(argv[5], "r");
	float c1x, c1y, c1z, n1x, n1y, n1z;
	float c2x, c2y, c2z, n2x, n2y, n2z;
	MergeVolumes* mVolumes = new MergeVolumes();
	int numMolecules = 0;
	fscanf(fp, "%d\n", &numMolecules);
	for (int i=0; i<numMolecules; i++)
	{
		if (!fscanf(fp, "%f %f %f %f %f %f %f %f %f %f %f %f\n",
					&c1x, &c1y, &c1z, &n1x, &n1y, &n1z,
					&c2x, &c2y, &c2z, &n2x, &n2y, &n2z))
		{
			break;
		}
		mVolumes->setTransformation(c1x, c1y, c1z, n1x, n1y, n1z,
									c2x, c2y, c2z, n2x, n2y, n2z);
		mVolumes->mergeVolumes(sData1, sData2);
	}
	delete mVolumes;
	fclose(fp);
	{
		/*// only for NMJ project!
		{
			float minExt[3];
			minExt[0] = sData1->getMinX() / 150.0;
			minExt[1] = sData1->getMinY() / 150.0;
			minExt[2] = sData1->getMinZ() / 150.0;
			sData1->setMinExtent( minExt );
		}
		{
			float maxExt[3];
			maxExt[0] = sData1->getMaxX() / 150.0;
			maxExt[1] = sData1->getMaxY() / 150.0;
			maxExt[2] = sData1->getMaxZ() / 150.0;
			sData1->setMaxExtent( maxExt );
		}
		*/
		VolumeLoader* volumeLoader = new VolumeLoader();
		volumeLoader->saveFile(outputVolumeFileName, sData1);
		delete volumeLoader;
	}
	return true;
}


bool Server::writeTorsionAngles(int argc, char* argv[])
{
	if (argc != 4)
	{
		printUsage();
		return false;
	}
	PDBParser::GroupOfAtoms* molecule = 0;
	// read file
	{
		GOALoader* gLoader = new GOALoader();
		molecule = gLoader->loadFile(argv[2]);
		delete gLoader;
		if (!molecule)
		{
			return false;
		}
	}
	FILE* fp = fopen(argv[3], "a");
	if (!fp)
	{
		delete molecule;
		return false;
	}
	bool ret = PDBParser::writeTorsionAngles(fp, molecule);
	delete molecule;
	fclose(fp);
	return ret;
}

bool Server::morph(int argc, char* argv[])
{
	if (argc != 7)
	{
		printUsage();
		return false;
	}
	PDBParser::GroupOfAtoms* molecule1 = 0;
	// read file
	{
		GOALoader* gLoader = new GOALoader();
		molecule1 = gLoader->loadFile(argv[2]);
		delete gLoader;
		if (!molecule1)
		{
			return false;
		}
	}
	PDBParser::GroupOfAtoms* molecule2 = 0;
	// read file
	{
		GOALoader* gLoader = new GOALoader();
		molecule2 = gLoader->loadFile(argv[3]);
		delete gLoader;
		if (!molecule2)
		{
			delete molecule1;
			return false;
		}
	}
	// outputFileNamePrefix = argv[4]
	double resolution = atof(argv[5]);
	int maxSteps = atoi(argv[6]);
	PDBParser::MoleculeMorph* moleculeMorpher = new PDBParser::MoleculeMorph();
	bool ret = moleculeMorpher->morph(molecule1, molecule2, argv[4], resolution, maxSteps);
	delete moleculeMorpher;
	return ret;
}

bool Server::getElecOnSurface(int argc, char* argv[])
{
	if (argc != 9)
	{
		printUsage();
		return false;
	}
	VolumeLoader* vLoader = new VolumeLoader();
	SimpleVolumeData* sData = vLoader->loadFile(argv[2]);
	delete vLoader;
	if (!sData)
	{
		return false;
	}
	double negCutoff = atof(argv[7]);
	double posCutoff = atof(argv[8]);
	GeometryLoader* gLoader = new GeometryLoader();
	Geometry* inputGeometry = gLoader->loadFile(argv[3]);
	delete gLoader;
	if (!inputGeometry)
	{
		return false;
	}
	inputGeometry->CalculateTriSmoothNormals();
	if (!inputGeometry->m_TriVertColorsTransparent)
	{
		inputGeometry->AllocateTriVertColors();
	}
	// get the function values for each vertex //
	double* funcVals = new double[inputGeometry->m_NumTriVerts];
	{
		if (!SimpleVolumeDataIsocontourer::getFunctionValues(sData, inputGeometry, funcVals))
		{
			delete []funcVals;
			delete sData;
			delete inputGeometry;
			return false;
		}
	}
	delete sData;
	sData = 0;
	//
	// write 3 surfaces depending on the cutoffs //
	Geometry* posGeometry = 0;
	Geometry* neuGeometry = 0;
	Geometry* negGeometry = 0;
	// first get the number of triangles in each
	int numPosTri = 0;
	int numNeuTri = 0;
	int numNegTri = 0;
	{
		for (int i=0; i<inputGeometry->m_NumTris; i++)
		{
			double f1 = funcVals[inputGeometry->m_Tris[i*3+0]];
			double f2 = funcVals[inputGeometry->m_Tris[i*3+1]];
			double f3 = funcVals[inputGeometry->m_Tris[i*3+2]];
			if (f1+f2+f3 > 3*posCutoff)
			{
				numPosTri++;
			}
			else if (f1+f2+f3 < 3*negCutoff)
			{
				numNegTri++;
			}
			else
			{
				numNeuTri++;
			}
		}
	}
	printf("Number of tris obtained were %d %d %d\n", numPosTri, numNeuTri, numNegTri);
	if (!numPosTri && !numNeuTri && !numNegTri)
	{
		delete []funcVals;
		delete inputGeometry;
		return false;
	}
	if (numPosTri)
	{
		posGeometry = new Geometry;
	}
	if (numNeuTri)
	{
		neuGeometry = new Geometry;
	}
	if (numNegTri)
	{
		negGeometry = new Geometry;
	}
	if (numPosTri)
	{
		posGeometry->AllocateTris(inputGeometry->m_NumTriVerts, numPosTri);
	}
	if (numNeuTri)
	{
		neuGeometry->AllocateTris(inputGeometry->m_NumTriVerts, numNeuTri);
	}
	if (numNegTri)
	{
		negGeometry->AllocateTris(inputGeometry->m_NumTriVerts, numNegTri);
	}
	printf("Allocated all three geometry\n");
	// copy all vertices and relevant triangles to the geometries
	{
		for (int i=0; i<inputGeometry->m_NumTriVerts*3; i++)
		{
			if (numPosTri)
			{
				posGeometry->m_TriVerts[i] = inputGeometry->m_TriVerts[i];
			}
			if (numNeuTri)
			{
				neuGeometry->m_TriVerts[i] = inputGeometry->m_TriVerts[i];
			}
			if (numNegTri)
			{
				negGeometry->m_TriVerts[i] = inputGeometry->m_TriVerts[i];
			}
		}
		for (int i=0; i<inputGeometry->m_NumTriVerts*3; i++)
		{
			if (numPosTri)
			{
				posGeometry->m_TriVertNormals[i] = inputGeometry->m_TriVertNormals[i];
			}
			if (numNeuTri)
			{
				neuGeometry->m_TriVertNormals[i] = inputGeometry->m_TriVertNormals[i];
			}
			if (numNegTri)
			{
				negGeometry->m_TriVertNormals[i] = inputGeometry->m_TriVertNormals[i];
			}
		}
		for (int i=0; i<inputGeometry->m_NumTriVerts; i++)
		{
			float r, g, b;
			float f = funcVals[i];
			if (f < negCutoff)
			{
				r = 1.0;
				g = b = 0;
			}
			else if (f < posCutoff)
			{
				r = 0.0;
				g = 0.5;
				b = 1.0;
			}
			else if (f < 0)
			{
				r = 1.0;
				g = b = 1.0f - f/negCutoff;
			}
			else if (f > 0)
			{
				b = 1.0;
				r = f/posCutoff;
				g = 0.5 + f / (2.0f*posCutoff);
			}
			inputGeometry->m_TriVertColorsTransparent[i*4+0] = r;
			inputGeometry->m_TriVertColorsTransparent[i*4+1] = g;
			inputGeometry->m_TriVertColorsTransparent[i*4+2] = b;
			inputGeometry->m_TriVertColorsTransparent[i*4+3] = 1.0;
		}
		printf("Done with vertices\n");
		int curPosTriIndex = 0;
		int curNeuTriIndex = 0;
		int curNegTriIndex = 0;
		for (int i=0; i<inputGeometry->m_NumTris; i++)
		{
			double f1 = funcVals[inputGeometry->m_Tris[i*3+0]];
			double f2 = funcVals[inputGeometry->m_Tris[i*3+1]];
			double f3 = funcVals[inputGeometry->m_Tris[i*3+2]];
			//printf("Doing with triangle %d with indices [%d %d %d]\n", i, inputGeometry->m_Tris[i*3+0],inputGeometry->m_Tris[i*3+1],inputGeometry->m_Tris[i*3+2] );
			if (f1+f2+f3 > 3*posCutoff)
			{
				posGeometry->m_Tris[3*curPosTriIndex+0] = inputGeometry->m_Tris[3*i+0];
				posGeometry->m_Tris[3*curPosTriIndex+1] = inputGeometry->m_Tris[3*i+1];
				posGeometry->m_Tris[3*curPosTriIndex+2] = inputGeometry->m_Tris[3*i+2];
				curPosTriIndex++;
			}
			else if (f1+f2+f3 < 3*negCutoff)
			{
				negGeometry->m_Tris[3*curNegTriIndex+0] = inputGeometry->m_Tris[3*i+0];
				negGeometry->m_Tris[3*curNegTriIndex+1] = inputGeometry->m_Tris[3*i+1];
				negGeometry->m_Tris[3*curNegTriIndex+2] = inputGeometry->m_Tris[3*i+2];
				curNegTriIndex++;
			}
			else
			{
				neuGeometry->m_Tris[3*curNeuTriIndex+0] = inputGeometry->m_Tris[3*i+0];
				neuGeometry->m_Tris[3*curNeuTriIndex+1] = inputGeometry->m_Tris[3*i+1];
				neuGeometry->m_Tris[3*curNeuTriIndex+2] = inputGeometry->m_Tris[3*i+2];
				curNeuTriIndex++;
			}
		}
	}
	printf("Trying to save\n");
	// save them all
	GeometryLoader* geometryLoader = new GeometryLoader();
	if (numPosTri)
	{
		geometryLoader->saveFile(argv[4], posGeometry);
	}
	if (numNeuTri)
	{
		geometryLoader->saveFile(argv[5], neuGeometry);
	}
	if (numNegTri)
	{
		geometryLoader->saveFile(argv[6], negGeometry);
	}
	geometryLoader->saveFile(argv[3], inputGeometry);
	delete geometryLoader;
	if (numPosTri)
	{
		delete posGeometry;
	}
	if (numNeuTri)
	{
		delete neuGeometry;
	}
	if (numNegTri)
	{
		delete negGeometry;
	}
	delete inputGeometry;
	inputGeometry = 0;
	return true;
}

bool Server::getCurvaturesOnSurface(int argc, char* argv[])
{
	if (argc != 4)
	{
		printUsage();
		return false;
	}
	GeometryLoader* gLoader = new GeometryLoader();
	Geometry* inputGeometry = gLoader->loadFile(argv[2]);
	delete gLoader;
	if (!inputGeometry)
	{
		return false;
	}
	inputGeometry->computeDerivatives();
	bool ret = inputGeometry->printDerivatives(argv[3]);
	delete inputGeometry;
	return ret;
}

bool Server::getPockets(int argc, char* argv[])
{
	if (argc != 4)
	{
		printUsage();
		return false;
	}
	PDBParser::GroupOfAtoms* molecule = 0;
	// read file
	{
		GOALoader* gLoader = new GOALoader();
		molecule = gLoader->loadFile(argv[2]);
		delete gLoader;
		if (!molecule)
		{
			return false;
		}
	}
	Pocket* pocket = new Pocket();
	Geometry* pocketGeometry = 0;
	int size = 65;
	//bool ret = pocket->constructPockets(molecule, argv[2], atof(argv[3]), &pocketGeometry, size);
	pocket->constructPockets(molecule, argv[2], atof(argv[3]), &pocketGeometry, size);
	delete pocket;
	delete molecule;
	if (!pocketGeometry)
	{
		return false;
	}
	delete pocketGeometry;
	return true;
}
/*
bool Server::createDockingInputFiles(int argc, char* argv[])
{
	Docking::DockComplex* complex = new Docking::DockComplex();
	bool deleteWater = false;
	if (!complex->init(argv[2], argv[3], deleteWater))
	{
		delete complex;
		return false;
	}
	bool printMiscFiles = (strcmp(argv[6], "true") == 0) ? true:false;
	bool ret = complex->createDockingInputFiles(argv[4], argv[5], printMiscFiles);
	delete complex;
	return ret;
}
*/
bool Server::breakUpPDBFile(int argc, char* argv[])
{
	if (argc != 4)
	{
		printUsage();
		return false;
	}
	FILE* fp = fopen(argv[2], "r");
	if (!fp)
	{
		return false;
	}
	int count = 0;
	char optFileName[1024];
	FILE* optFp = 0;
	while (true)
	{
		char line[1024];
		if (!fgets(line, 1023, fp))
		{
			break;
		}
		if (!optFp)
		{
			sprintf(optFileName, "%s_%d.pdb", argv[3], count);
			optFp = fopen(optFileName, "w");
			if (!optFp)
			{
				return false;
			}
		}
		fputs(line, optFp);
		if (strlen(line) >= 3)
		{
			if (line[0] == 'E' && line[1] == 'N' && line[2] == 'D')
			{
				count++;
				fclose(optFp);
				optFp = 0;
			}
		}
	}
	if (optFp)
	{
		fclose(optFp);
	}
	fclose(fp);
	return true;
}


/*
// takes in 2 molecules and a cutoff. It determines all the interface atoms and prints it out
bool Server::getInterface(int argc, char* argv[])
{
	if (argc != 6)
	{
		return false;
	}
	Docking::DockComplex* complex = new Docking::DockComplex();
	bool deleteWater = false;
	if (!complex->init(argv[2], argv[3], deleteWater))
	{
		delete complex;
		return false;
	}
	double cutoff = atof(argv[4]);
	bool ret = complex->getInterface(cutoff, argv[5]);
	delete complex;
	return ret;
}
*/


 /*
// takes in 2 molecules, a transformation file, number of transformations, a cut off parameter to decide which atoms
// at the interface and an output file of RMSD values
bool Server::getInterfaceRMSD(int argc, char* argv[])
{
	if (argc != 8 && argc != 9)
	{
		return false;
	}
	if (argc == 8)
	{
		Docking::DockComplex* complex = new Docking::DockComplex();
		bool deleteWater = false;
		if (!complex->init(argv[2], argv[3], deleteWater))
		{
			delete complex;
			return false;
		}
		int numberOfTransformations = atoi(argv[5]);
		double cutoff = atof(argv[6]);
		bool ret = complex->getInterfaceRMSD(argv[4], numberOfTransformations, cutoff, argv[7]);
		delete complex;
		return ret;
	}
	else
	{
		Docking::DockComplex* complex = new Docking::DockComplex();
		bool deleteWater = false;
		if (!complex->init(argv[2], argv[3], deleteWater))
		{
			delete complex;
			return false;
		}
		int numberOfTransformations = atoi(argv[5]);
		double cutoff = atof(argv[6]);
		bool ret = complex->getInterfaceRMSD(argv[4], numberOfTransformations, cutoff, argv[7], argv[8]);
		delete complex;
		return ret;
	}
}

 */

  /*

// takes in 2 molecules, a transformation file, number of transformations, a cut off parameter to decide which surface triangles are
// at the interface and an output file of interface area values
bool Server::getInterfaceArea(int argc, char* argv[])
{
	if (argc != 8)
	{
		return false;
	}
	Docking::DockComplex* complex = new Docking::DockComplex();
	bool deleteWater = false;
	if (!complex->init(argv[2], argv[3], deleteWater))
	{
		delete complex;
		return false;
	}
	int numberOfTransformations = atoi(argv[5]);
	double cutoff = atof(argv[6]);
	bool ret = complex->getInterfaceArea(argv[4], numberOfTransformations, cutoff, argv[7]);
	delete complex;
	return ret;
}

  */


   /*
// takes in the two bound proteins, one unbound, num of xforms, a distance criteria,
// a set of possible xforms file, and an output file and whether to look at rotamers or not
bool Server::improveInterface(int argc, char* argv[])
{
	if (argc != 10 && argc != 11)
	{
		return false;
	}
	if (argc == 10)
	{
		Docking::DockComplex* complex = new Docking::DockComplex();
		bool deleteWater = false;
		if (!complex->init(argv[2], argv[3], deleteWater))
		{
			delete complex;
			return false;
		}
		bool flexDock = false;
		if (strcmp(argv[9],"true") == 0)
		{
			delete complex;    // wierd, flex dock needs one more parameter
			return false;
		}
		int numberOfTransformations = atoi(argv[5]);
		double cutoff = atof(argv[6]);
		bool ret = complex->improveInterface(argv[4], numberOfTransformations, cutoff, argv[7], argv[8], flexDock);
		delete complex;
		return ret;
	}
	else
	{
		Docking::RotamerFlexComplex* complex = new Docking::RotamerFlexComplex();
		bool deleteWater = false;
		if (!complex->init(argv[2], argv[3], deleteWater))
		{
			delete complex;
			return false;
		}
		bool flexDock = true;
		if (strcmp(argv[9],"false") == 0)
		{
			delete complex;    // wierd
			return false;
		}
		int numberOfTransformations = atoi(argv[5]);
		double cutoff = atof(argv[6]);
		int numNewConformations = atoi(argv[10]);
		bool ret = complex->improveInterface(argv[4], numberOfTransformations, cutoff, argv[7], argv[8], flexDock, numNewConformations);
		delete complex;
		return ret;
	}
	return false;
}
   */

    /*
bool Server::printFCC(int argc, char* argv[])
{
	//if( argc < 6 ) return false;
	PDBParser::GroupOfAtoms* molecule = 0;
	// read file
	{
		GOALoader* gLoader = new GOALoader();
		molecule = gLoader->loadFile(argv[2]);
		delete gLoader;
		if (!molecule)
		{
			return false;
		}
	}
	// read first level domains
	Docking::DomainComplex* domainComplex = new Docking::DomainComplex(molecule);
	printf("Server readDomain TEST\n");
	domainComplex->readDomainFinderOpt(argv[3], 0);
	// read any remaining levels domains
	{
		for (int i=6; i<argc; i++)
		{
			domainComplex->readDomainFinderOpt(argv[i], i-4);
		}
	}
	// classify all segments at all levels
	if (!domainComplex->classifyFlexibleSegments())
	{
		delete domainComplex;
		return false;
	}
	if (!domainComplex->computeFlexibility())
	{
		delete domainComplex;
		return false;
	}
	bool ret = true;
	if (!domainComplex->printFCC(argv[4]))
	{
		ret = false;
	}
	if (!domainComplex->printColorMap(argv[5], 0))
	{
		ret = false;
	}
	delete domainComplex;
	delete molecule;
	return ret;
}
*/

     /*
bool Server::getConformations(int argc, char* argv[])
{
	//if( argc < 7 ) return false;
	PDBParser::GroupOfAtoms* molecule = 0;
	// read file
	{
		GOALoader* gLoader = new GOALoader();
		molecule = gLoader->loadFile(argv[2]);
		delete gLoader;
		if (!molecule)
		{
			return false;
		}
	}
	Docking::DomainComplex* domainComplex = new Docking::DomainComplex(molecule);
	bool ret = domainComplex->getConformations(argv[3], atoi(argv[4]), argv[5], atoi(argv[6]));
	delete domainComplex;
	delete molecule;
	return ret;
}
*/

      /*
bool Server::computeScoringGrids(int argc, char* argv[])
{
	if (argc < 6)
	{
		return false;
	}
	PDBParser::GroupOfAtoms* molecule = 0;
	// read file
	{
		GOALoader* gLoader = new GOALoader();
		molecule = gLoader->loadFile(argv[2]);
		delete gLoader;
		if (!molecule)
		{
			return false;
		}
	}
	double ligandSize = atof(argv[3]);
	double gridSpacing = atof(argv[4]);
	Docking::ScoreModule* scoreModule = new Docking::ScoreModule;
	bool ret = scoreModule->computeScoringGrids(molecule, ligandSize, gridSpacing, argv[5]);
	delete scoreModule;
	return ret;
}
*/

       /*
bool Server::dockingMovie(int argc, char* argv[])
{
	if (argc != 6)
	{
		return false;
	}
	F2DockAnimator dockingMovie(argv[2], argv[3], argv[4], argv[5]);
	dockingMovie.buildFiles();
}
*/
bool Server::geoImprove_LBIE(int argc, char* argv[])
{
	//	LBIE::LBIE_Mesher* lbie_mesher = new LBIE::LBIE_Mesher(argv[2],argv[3]);
	LBIE::LBIE_Mesher mesher;
	mesher.loadMesh(argv[2]);
	mesher.geoImprove();
	mesher.saveMesh(argv[3]);
}


bool Server::dockRequest(int argc, char* argv[])
{
	CommandLineControl *clc = new CommandLineControl();

	return clc->handleDockingRequest(string(argv[2]));
}

bool Server::rerankRequest(int argc, char* argv[])
{
//	cout<<"Server does not support rerank at the moment"<<endl;	

	CommandLineControl *clc = new CommandLineControl();

	return clc->handleRerankingRequest(string(argv[2]));
}

bool Server::f2dGenRequest(int argc, char* argv[])
{
	CommandLineControl *clc = new CommandLineControl();

	return clc->handleF2dGenRequest(string(argv[2]));
}

bool Server::quadGenRequest(int argc, char* argv[])
{
	CommandLineControl *clc = new CommandLineControl();

	return clc->handleQuadGenRequest(string(argv[2]));
}


bool Server::shrinkPQRintoSurface(int argc, char* argv[])
{

	if (argc != 7)
	{
		printUsage();
		return false;
	}

	char inputPQRFileName[256];
	char inputVolumeFileName[256];

	char outputPQRFileName[256];
	strcpy(inputPQRFileName, argv[2]);
	FILE* fpqr = fopen(inputPQRFileName, "r");

	strcpy(outputPQRFileName, argv[6]);
	FILE* fsavepqr = fopen(outputPQRFileName, "w");

	MOLECULE::RADIUS_TYPE radiusType;
	radiusType = MOLECULE::PQR_RADIUS;


	MOLECULE::GroupOfAtoms* molecule = ParsePQR(fpqr, radiusType );
	fclose(fpqr);

	VolMagick::Volume v;
	VolMagick::readVolumeFile(v, string(argv[3]));

	vector<VolMagick::Volume> gradient;
	VolMagick::calcGradient(gradient, v);

	float dx = atof(argv[5]);

	float d_hls[3];
	float abs_grad;
	double p[3];

	for(int i = 0; i< molecule->numOfAtoms; i++)
	{
		p[0] = molecule->m_Atoms[i]->position[0];
		p[1] = molecule->m_Atoms[i]->position[1];
		p[2] = molecule->m_Atoms[i]->position[2];

		for(int iteration=0; iteration < atoi(argv[4]); iteration ++)
		{

			d_hls[0] = gradient[0].interpolate(p[0], p[1], p[2]);
			d_hls[1] = gradient[1].interpolate(p[0], p[1], p[2]);
			d_hls[2] = gradient[2].interpolate(p[0], p[1], p[2]);
			
			abs_grad = d_hls[0]*d_hls[0]+d_hls[1]*d_hls[1]+d_hls[2]*d_hls[2];
			if(abs_grad >= 0.0001) {
				d_hls[0] /= sqrtf(abs_grad);
				d_hls[1] /= sqrtf(abs_grad);
				d_hls[2] /= sqrtf(abs_grad);
			}
			p[0] -= dx*d_hls[0];
			p[1] -= dx*d_hls[1];
			p[2] -= dx*d_hls[2];
		}

		molecule->m_Atoms[i]->setPosition(p);

//		cout<<molecule->m_Atoms[i]->radius << endl;
			
	}

	savePQR(molecule, fsavepqr);
	fclose(fsavepqr);
	return 1;
}
