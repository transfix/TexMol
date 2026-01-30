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
#ifndef BASIC_PARAMS
#define BASIC_PARAMS

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

enum { 	INIT,
	SUBMITTED, 
	RUNNING, 
	COMPLETED, 
	AVAILABLE
};

enum { 	DOCKING, 
	RERANKING, 
	F2DGEN, 
	QUADGEN
};


enum {  RECEPTOR_PQR, 
	LIGAND_PQR,
	RECEPTOR_F2D, 
	LIGAND_F2D,
	RECEPTOR_RAWN, 
	LIGAND_RAWN,
	RECEPTOR_QUAD, 
	LIGAND_QUAD,
	DOCKING_OUT,
	RERANKING_OUT
};


class BasicParams
{
	private:

	int jobId;
	int jobType;
	int status;
	string jobTitle;

	bool isReceptorPDBAvailable;
	string receptorPDBName;

	bool isReceptorPQRAvailable;
	string receptorPQRName;

	bool isReceptorF2dAvailable;
	string receptorF2dName;

	bool isReceptorRAWNAvailable;
	string receptorRAWNName;

	bool isReceptorQuadAvailable;
	string receptorQuadName;

	bool isLigandPDBAvailable;
	string ligandPDBName;

	bool isLigandPQRAvailable;
	string ligandPQRName;

	bool isLigandF2dAvailable;
	string ligandF2dName;

	bool isLigandQuadAvailable;
	string ligandRAWNName;

	bool isLigandRAWNAvailable;
	string ligandQuadName;

	bool isRMSDAvailable;	
	string rmsdFileName;

	bool isDockingInputAvailable;
	string dockingInputFileName;

	bool isRerankingInputAvailable;
	string rerankingInputFileName;

	bool isF2dGenInputAvailable;
	string f2dGenInputFileName;

	bool isQuadGenInputAvailable;
	string quadGenInputFileName;

	bool isDockingOutputAvailable;
	string dockingOutputFileName;

	bool isRerankingOutputAvailable;
	string rerankingOutputFileName;

	bool performRerank;

	bool storeIntermediateFiles;

	bool storeReceptorPQR;
	bool storeLigandPQR;
	bool storeReceptorF2d;
	bool storeLigandF2d;
	bool storeReceptorRAWN;
	bool storeLigandRAWN;
	bool storeReceptorQuad;
	bool storeLigandQuad;

	void createIOFileNames(string name);

	public:
	BasicParams();
	~BasicParams();

	bool save(string paramFile);
	bool load(string paramFile);		
	bool init();

	//set methods

	void setJobId(int id) {jobId = id;}
	bool setJobTitle(string name);
	bool setJobStatus(int st);
	bool setJobType(int t);

	bool setReceptorPDBName(string fileName, int mode = 0);
	bool setLigandPDBName(string fileName, int mode = 0);

	bool setReceptorPQRName(string fileName, int mode = 0);
	bool setLigandPQRName(string fileName, int mode = 0);

	bool setReceptorF2dName(string fileName, int mode = 0);
	bool setLigandF2dName(string fileName, int mode = 0);

	bool setReceptorRAWNName(string fileName, int mode = 0);
	bool setLigandRAWNName(string fileName, int mode = 0);

	bool setReceptorQuadName(string fileName, int mode = 0);
	bool setLigandQuadName(string fileName, int mode = 0);

	bool setRMSDName(string fileName, int mode = 0);

	bool setDockingInputName(string fileName, int mode = 0);
	bool setRerankingInputName(string fileName, int mode = 0);		
	bool setF2dGenInputName(string fileName, int mode = 0);
	bool setQuadGenInputName(string fileName, int mode = 0);		

	bool setDockingOutputName(string fileName, int mode = 0);
	bool setRerankingOutputName(string fileName, int mode = 0);

	//used if intermediate files are retrieved from the server and saved
	void setReceptorPQRAvailable(bool b) {isReceptorPQRAvailable = b;}
	void setLigandPQRAvailable(bool b) {isLigandPQRAvailable = b;}
	void setReceptorF2dAvailable(bool b) {isReceptorF2dAvailable = b;}
	void setLigandF2dAvailable(bool b) {isLigandF2dAvailable = b;}
	void setReceptorRAWNAvailable(bool b) {isReceptorRAWNAvailable = b;}
	void setLigandRAWNAvailable(bool b) {isLigandRAWNAvailable = b;}
	void setReceptorQuadAvailable(bool b) {isReceptorQuadAvailable = b;}
	void setLigandQuadAvailable(bool b) {isLigandQuadAvailable = b;}
	//used if output files are retrieved from the server and saved
	void setDockingOutputAvailable(bool b) {isDockingOutputAvailable = b;}
	void setRerankingOutputAvailable(bool b) {isRerankingOutputAvailable = b;}
	//used after input files are prepared
	void setDockingInputAvailable(bool b) {isDockingInputAvailable = b;}
	void setRerankingInputAvailable(bool b) {isRerankingInputAvailable = b;}
	void setF2dGenInputAvailable(bool b) {isF2dGenInputAvailable = b;}
	void setQuadGenInputAvailable(bool b) {isQuadGenInputAvailable = b;}

	
	void setStoreReceptorPQR(bool b) {storeReceptorPQR = b; if(b) storeIntermediateFiles = true;}
	void setStoreLigandPQR(bool b) {storeLigandPQR = b; if(b) storeIntermediateFiles = true;}
	void setStoreReceptorF2d(bool b) {storeReceptorF2d = b; if(b) storeIntermediateFiles = true;}
	void setStoreLigandF2d(bool b) {storeLigandF2d = b; if(b) storeIntermediateFiles = true;}
	void setStoreReceptorRAWN(bool b) {storeReceptorRAWN = b; if(b) storeIntermediateFiles = true;}
	void setStoreLigandRAWN(bool b) {storeLigandRAWN = b; if(b) storeIntermediateFiles = true;}
	void setStoreReceptorQuad(bool b) {storeReceptorQuad = b; if(b) storeIntermediateFiles = true;}
	void setStoreLigandQuad(bool b) {storeLigandQuad = b; if(b) storeIntermediateFiles = true;}

	bool setPerformRerank(bool b) {performRerank = b; return true;}
	

	//get methods
	string getJobId();
	string getJobTitle();
	int getJobTypeInt();
	string getJobType();
	string getJobStatus();
	int getJobIdInt(){return jobId;}
	int getJobStatusInt(){return status;}

	bool isRunning() {if(status<COMPLETED && status>INIT) return true; return false;}
	bool doRetrieve() {if(status==COMPLETED) return true; return false;}

	bool getReceptorPDBAvailable() {return isReceptorPDBAvailable;}
	string getReceptorPDBName() {return receptorPDBName;}

	bool getReceptorPQRAvailable() {return isReceptorPQRAvailable;}
	string getReceptorPQRName() {return receptorPQRName;}

	bool getReceptorF2dAvailable() {return isReceptorF2dAvailable;}
	string getReceptorF2dName() {return receptorF2dName;}

	bool getReceptorRAWNAvailable() {return isReceptorRAWNAvailable;}
	string getReceptorRAWNName() {return receptorRAWNName;}

	bool getReceptorQuadAvailable() {return isReceptorQuadAvailable;}
	string getReceptorQuadName() {return receptorQuadName;}

	bool getLigandPDBAvailable() {return isLigandPDBAvailable;}
	string getLigandPDBName() {return ligandPDBName;}

	bool getLigandPQRAvailable() {return isLigandPQRAvailable;}
	string getLigandPQRName() {return ligandPQRName;}

	bool getLigandF2dAvailable() {return isLigandF2dAvailable;}
	string getLigandF2dName() {return ligandF2dName;}

	bool getLigandQuadAvailable() {return isLigandQuadAvailable;}
	string getLigandRAWNName() {return ligandRAWNName;}

	bool getLigandRAWNAvailable() {return isLigandRAWNAvailable;}
	string getLigandQuadName() {return ligandQuadName;}

	bool getRMSDAvailable() {return isRMSDAvailable;}	
	string getRmsdFileName() {return rmsdFileName;}

	bool getDockingInputAvailable() {return isDockingInputAvailable;}
	string getDockingInputFileName() {return dockingInputFileName;}

	bool getRerankingInputAvailable() {return isRerankingInputAvailable;}
	string getRerankingInputFileName() {return rerankingInputFileName;}

	bool getF2dGenInputAvailable() {return isF2dGenInputAvailable;}
	string getF2dGenInputFileName() {return f2dGenInputFileName;}

	bool getQuadGenInputAvailable() {return isQuadGenInputAvailable;}
	string getQuadGenInputFileName() {return quadGenInputFileName;}

	bool getDockingOutputAvailable() {return isDockingOutputAvailable;}
	string getDockingOutputFileName() {return dockingOutputFileName;}

	bool getRerankingOutputAvailable() {return isRerankingOutputAvailable;}
	string getRerankingOutputFileName() {return rerankingOutputFileName;}

	bool getPerformRerank() {return performRerank;}

	bool getStoreIntermediateFiles() {return storeIntermediateFiles;}

	bool getStoreReceptorPQR() {return storeReceptorPQR;}
	bool getStoreLigandPQR() {return storeLigandPQR;}
	bool getStoreReceptorF2d() {return storeReceptorF2d;}
	bool getStoreLigandF2d() {return storeLigandF2d;}
	bool getStoreReceptorRAWN() {return storeReceptorRAWN;}
	bool getStoreLigandRAWN() {return storeLigandRAWN;}
	bool getStoreReceptorQuad() {return storeReceptorQuad;}
	bool getStoreLigandQuad() {return storeLigandQuad;}


	string getReceptor(void){if(isReceptorPDBAvailable) return receptorPDBName; else if(isReceptorPQRAvailable) return receptorPQRName; else return NULL;}
	string getLigand(void){if(isLigandPDBAvailable) return ligandPDBName; else if(isLigandPQRAvailable) return ligandPQRName; else return NULL;}
};


#endif
