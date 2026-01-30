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
// This class will be used to parse and execute command line arguments
// It will help in running moleculeViz in batch mode.
// It should also help explain on how to use the different libraries separately.

/* to dos
// be able to use the data set index in all commands!
	#1. add data
	#2. delete data
	#2.b delete prev data
	#3. delete all
	#4. set visible ( bool, index )
	#4.b set visible prev (bool )
	5. blur - viruses, threshold, depth color (take in color table), internal pdb and external
	6. add isocontour to volume
	7. save image
*/

#ifndef CCV_TEXMOL_SERVER_H
#define CCV_TEXMOL_SERVER_H

class MainWindow;

class Server
{
	public:
		Server();
		virtual ~Server();
		virtual bool execute(int argc, char* argv[], MainWindow* mWindow);

	protected:
		bool blur(int argc, char* argv[]);
		bool blurWithResolution(int argc, char* argv[]);
		bool createSkinRegion(int argc, char* argv[]);
		bool setCurvature(int argc, char* argv[]);
		void printUsage();
		bool outGridPositions(int argc, char* argv[]);
		bool classifyPoints(int argc, char* argv[]);
		bool growOut(int argc, char* argv[]);
		bool getSurfaceFromVolume(int argc, char* argv[]);
		bool getSurfaceFromPDB(int argc, char* argv[]);
		bool correlate(int argc, char* argv[]);
		bool getArea(int argc, char* argv[]);
		bool getVolume(int argc, char* argv[]);
		bool getVolumeN(int argc, char* argv[]);
		bool getPatchAreas(int argc, char* argv[]);
		bool populateSAS(int argc, char* argv[]);
		bool getSurfaceAtoms(int argc, char* argv[]);
		bool addVolumes(int argc, char* argv[]);
		bool convert(int argc, char* argv[]);
		bool getContourStats(int argc, char* argv[]);
		bool evolve(int argc, char* argv[]);
		bool writePDB(int argc, char* argv[]);
		bool writeGOA(int argc, char* argv[]);
		bool depthColor(int argc, char* argv[]);
		bool printPDBInformation(int argc, char* argv[]);
		bool getMaxDistanceFromPoint(int argc, char* argv[]);
		bool getHydrophobicityOnSurface(int argc, char* argv[]);
		bool getSignDistanceFunction(int argc, char* argv[]);
		bool getMolecularCharacteristics(int argc, char* argv[]);
		//albert
		bool geoImprove_LBIE(int argc, char* argv[]);
		bool LBIE_Meshing(int argc, char* argv[]);
		bool expandMolecule(int argc, char* argv[]);
		bool mergeGeometry(int argc, char* argv[]);
		bool mergeVolumes(int argc, char* argv[]);
		bool writeTorsionAngles(int argc, char* argv[]);
		bool morph(int argc, char* argv[]);
		bool getElecOnSurface(int argc, char* argv[]);
		bool getCurvaturesOnSurface(int argc, char* argv[]);
		bool getPockets(int argc, char* argv[]);
		bool createDockingInputFiles(int argc, char* argv[]);
		bool breakUpPDBFile(int argc, char* argv[]);
		bool getInterface(int argc, char* argv[]);
		bool getInterfaceRMSD(int argc, char* argv[]);
		bool getInterfaceArea(int argc, char* argv[]);
		bool improveInterface(int argc, char* argv[]);
		bool printFCC(int argc, char* argv[]);
		bool getConformations(int argc, char* argv[]);
		bool computeScoringGrids(int argc, char* argv[]);
		bool dockingMovie(int argc, char* argv[]);
		bool addNewDataSet(int argc, char* argv[], MainWindow* mWindow);
		bool splitView(int argc, char* argv[], MainWindow* mWindow);
		bool deleteData(int argc, char* argv[], MainWindow* mWindow);
		bool deletePrevData(int argc, char* argv[], MainWindow* mWindow);
		bool deleteAllData(int argc, char* argv[], MainWindow* mWindow);
		bool setVisible(int argc, char* argv[], MainWindow* mWindow);
		bool setVisiblePrev(int argc, char* argv[], MainWindow* mWindow);
		bool saveImage(int argc, char* argv[], MainWindow* mWindow);
		bool setGridVisible(int argc, char* argv[], MainWindow* mWindow);
		bool downloadPDB(int argc, char* argv[], MainWindow* mWindow);
		bool getSurfaceUsingHLS(int argc, char* argv[]);
		bool dockRequest(int argc, char* argv[]);
		bool rerankRequest(int argc, char* argv[]);
		bool f2dGenRequest(int argc, char* argv[]);
		bool quadGenRequest(int argc, char* argv[]);
		bool shrinkPQRintoSurface(int argc, char* argv[]);
};

#endif
