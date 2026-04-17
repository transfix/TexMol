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
#include <F2DockClient/summaryResults.h>

using namespace std;

SummaryResults::SummaryResults()
{
	_isDockingOutput = false;
	_isRerankingOutput = false;

	init();
}


SummaryResults::~SummaryResults()
{
	detres.clear();
	rerankdetres.clear();
}


bool SummaryResults::init()
{
//	cout<<"inside init"<<endl;

	for(int i=0;i<6;i++)
	{
		hitsInRange[i] = 0;
		hitsInRangeAfterRerank[i] = 0;
	}

	numGoodPeaks[0] = 0;
	numGoodPeaks[1] = 0;

	minRMSD[0] = -1;
	minRMSD[1] = -1;

	minRankedPeak[0] = -1;
	minRankedPeak[1] = -1;

	minRankedPeakRMSD[0] = -1;
	minRankedPeakRMSD[1] = -1;

	minRankedPeakScore[0] = 0;
	minRankedPeakScore[1] = 0;

	indexMinRMSD[0] = -1;
	indexMinRMSD[1] = -1;

	scoreMinRMSD[0] = 0;
	scoreMinRMSD[1] = 0;

	_dockingOutputProcessed = false;
	_rerankingOutputProcessed = false;

//	cout<<"clearing vectors"<<endl;

	detres.clear();
	rerankdetres.clear();

	numStats = 0;

//	cout<<"init done"<<endl;
	return true;
}

bool SummaryResults::readOutputFromFile()
{
//	cout<<"inside read output from file"<<endl;

	if(_isDockingOutput) _dockingOutputProcessed = readDockingOutputFromFile();

	if(_isRerankingOutput) _rerankingOutputProcessed = readRerankingOutputFromFile();

//	cout<<"reading complete"<<endl;

	if(_dockingOutputProcessed || _rerankingOutputProcessed) 
	{
		flipOutputs();
		mineOutputs();
	}

//	cout<<"mining complete"<<endl;

	return mergeOutputs();
}


bool SummaryResults::readDockingOutputFromFile()
{
	double rmsdGood = 5.0;
	int index;

//	cout<<"inside read docking output"<<endl;

	FILE* ifp = fopen(dockingOutputFileName.c_str(), "rt");
	if (ifp == NULL)
	{
		printError("Failed to open F2Dock output file !");
	}

	char s[ 2000 ], t[ 2000 ];
	int rankLoc = 1, scoreLoc = 0, shapeLoc = 0;			// columns for each detailed result item
	int ssrLoc = 0, ccrLoc = 0, scrLoc = 0;
	int elecLoc = 0, hbondLoc = 0, hydrophobicityLoc = 0, smplcompLoc = 0, vdwLoc = 0, clashesLoc = 0, pgsolLoc = 0, pgsolhLoc = 0;
	int matLoc = 0, confLoc = 0, rmsdLoc = 0;
	bool gotNumSol = false;
	bool gotPeak = false;

	while (fgets(s, 1999, ifp) != NULL)
	{
		if (skipInitial(s, (char *)"# OUTPUT FORMAT:", t))
		{
			int m;
			if (sscanf(t, "%d", &m) != 1)
			{
				printError("Error reading F2Dock output file !");
				return false;
			}
			m = 1;
			while (fgets(s, 1999, ifp) != NULL)
			{
				if ((m == 1) && !skipInitial(s, (char *)"#	 COLNAME rank int", t))
				{
					printError("First column of F2Dock output must be the rank ( int )!");
					return false;
				}
				if (skipInitial(s, (char *)"#	 COLNAME score ", t))
				{
					scoreLoc = m;
				}
				else if (skipInitial(s, (char *)"#	 COLNAME shape ", t))
				{
					shapeLoc = m;
				}
				else if (skipInitial(s, (char *)"#	 COLNAME ssr ", t))
				{
					ssrLoc = m;
				}
				else if (skipInitial(s, (char *)"#	 COLNAME ccr ", t))
				{
					ccrLoc = m;
				}
				else if (skipInitial(s, (char *)"#	 COLNAME scr ", t))
				{
					scrLoc = m;
				}
				else if (skipInitial(s, (char *)"#	 COLNAME elec ", t))
				{
					elecLoc = m;
				}
				else if (skipInitial(s,(char *) "#	 COLNAME hbond ", t))
				{
					hbondLoc = m;
				}
				else if (skipInitial(s, (char *)"#	 COLNAME hydrophobicity ", t))
				{
					hydrophobicityLoc = m;
				}
				else if (skipInitial(s,(char *) "#	 COLNAME smplcomp ", t))
				{
					smplcompLoc = m;
				}
				else if (skipInitial(s,(char *) "#	 COLNAME vdw ", t))
				{
					vdwLoc = m;
				}
				else if (skipInitial(s,(char *) "#	 COLNAME clashes ", t))
				{
					clashesLoc = m;
				}
				else if (skipInitial(s,(char *) "#	 COLNAME pgsol ", t))
				{
					pgsolLoc = m;
				}
				else if (skipInitial(s, (char *)"#	 COLNAME pgsolh ", t))
				{
					pgsolhLoc = m;
				}
				else if (skipInitial(s, (char *)"#	 COLNAME mat1 ", t))
				{
					matLoc = m;
				}
				else if (skipInitial(s, (char *)"#	 COLNAME conf ", t))
				{
					confLoc = m;
				}
				else if (skipInitial(s, (char *)"#	 COLNAME rmsd ", t))
				{
					rmsdLoc = m;
				}
				m++;
				if ((rankLoc > 0) && (shapeLoc > 0) && (ssrLoc > 0) && (ccrLoc > 0) && (scrLoc > 0) && (elecLoc > 0) && (hbondLoc > 0) && (smplcompLoc > 0) && (hydrophobicityLoc > 0) && (vdwLoc > 0) && (clashesLoc > 0) && (matLoc > 0) && (pgsolLoc>0) && (pgsolhLoc>0) && (confLoc > 0) && (rmsdLoc > 0))
				{
					break;
				}
			}
			cout<< rankLoc <<endl;
			cout<< elecLoc <<endl;
			cout<< matLoc <<endl;
			cout<< rmsdLoc <<endl;
		}
		else if (skipInitial(s, (char *) "# START PEAKS", t))
		{
			cout<<"Reading results"<<endl;
			if (!((rankLoc > 0) && (shapeLoc > 0) && (ssrLoc > 0) && (ccrLoc > 0) && (scrLoc > 0) && (elecLoc > 0) && (hbondLoc > 0) && (smplcompLoc > 0) && (hydrophobicityLoc > 0) && (vdwLoc > 0) && (clashesLoc > 0) && (matLoc > 0) && (confLoc > 0) && (rmsdLoc > 0)))
			{
				printError("Missing format data in F2Dock output file !");
			}
			if ((matLoc <= rmsdLoc) && (matLoc + 12 > rmsdLoc))
			{
				printError("Invalid format data in F2Dock output file !");
			}
			while (fgets(s, 1999, ifp) != NULL)
			{
//				cout<<s<<endl;

				if (skipInitial(s, (char *)"# END PEAKS", t))
				{
					break;
				}
				if (!gotNumSol)
				{
					double d;
					if (!getDoublesInRange(s, 1, 1, &d))
					{
						printError("Error reading F2Dock output file !");
					}
					numSol = (int) floor(d + 0.5);
					index = numSol;

					gotNumSol = true;
					//cout<<numSol<<endl;
				}

				double trans[ 12 ], temp;

				DetailResults* dr = new DetailResults(_isDockingOutput);
				dr->setRank(index);

				index--;

				if (index<0)
				{
					break;
				}
				if (!getDoublesInRange(s, scoreLoc, scoreLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setScore(temp);
				}
				if (!getDoublesInRange(s, shapeLoc, shapeLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setShape(temp);
				}
				if (!getDoublesInRange(s, ssrLoc, ssrLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setSsr(temp);
				}
				if (!getDoublesInRange(s, ccrLoc, ccrLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setCcr(temp);
				}
				if (!getDoublesInRange(s, scrLoc, scrLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setScr(temp);
				}
				if (!getDoublesInRange(s, elecLoc, elecLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setElec(temp);
				}
				if (!getDoublesInRange(s, hbondLoc, hbondLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setHbond(temp);
				}
				if (!getDoublesInRange(s, hydrophobicityLoc, hydrophobicityLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setHydro(temp);
				}
				if (!getDoublesInRange(s, smplcompLoc, smplcompLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setScc(temp);
				}
				if (!getDoublesInRange(s, vdwLoc, vdwLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setVdw(temp);
				}
				if (!getDoublesInRange(s, clashesLoc, clashesLoc, &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setClashes((int)floor(temp+0.5));
				}
				if (!getDoublesInRange(s, pgsolLoc, pgsolLoc, &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setPgsolScore(temp);
				}
				if (!getDoublesInRange(s, pgsolhLoc, pgsolhLoc, &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setPgsolhScore(temp);
				}
				if (!getDoublesInRange(s, matLoc, matLoc + 11, trans))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					dr->setMat(trans);
					
					/*for (int tempi = 0; tempi<12; tempi++)
					{
						dr->mat[tempi] = trans[tempi];
					}*/
				}
				if (!getDoublesInRange(s, confLoc, confLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setConf((int)floor(temp+0.5));
				}
				if (!getDoublesInRange(s, rmsdLoc, rmsdLoc,  &temp))
				{
					printError("Error reading F2Dock output file !");
				}
				else
				{
					
					dr->setRmsd(temp);
				}
				
				detres.push_back(dr);
			}
		}
		else if (skipInitial(s, (char *)"# total time = ", t))
		{
			double d;
			int i = 0;
			i = getDouble(t, i, &d);    // time in seconds
			time[0] = d;
		}
	}
	fclose(ifp);

	numSol = detres.size();
	cout<<detres.size()<<endl;

	return true;
}


bool SummaryResults::readRerankingOutputFromFile()
{
	double rmsdGood = 5.0;
	int index;

//	cout<<"inside read reranking output"<<endl;

	FILE* ifp = fopen(rerankingOutputFileName.c_str(), "rt");
	if (ifp == NULL)
	{
		printError((char*) "Failed to open GBRerank output file");
		return false;
	}

	char s[ 2000 ], t[ 2000 ];
	int newrankLoc = 1, newscoreLoc = 0, delgpolLoc = 0;			// columns for each detailed result item
	int areapropLoc = 0, rankLoc = 0, scoreLoc = 0;
	int matLoc = 0, rmsdLoc = 0;
	bool gotNumSol = false;
	bool gotPeak = false;

	int rerankingOutputCount = 0;

	while (fgets(s, 1999, ifp) != NULL)
	{
		if (skipInitial(s, (char*) "# OUTPUT FORMAT:", t))
		{
			int m;
			if (sscanf(t, "%d", &m) != 1)
			{
				printError((char*) "Error reading GBRerank output file ( %s )!", rerankingOutputFileName.c_str());
				fclose(ifp);
				return false;
			}
			m = 1;
			while (fgets(s, 1999, ifp) != NULL)
			{
				if ((m == 1) && !skipInitial(s, (char*)"#	 COLNAME new_rank int", t))
				{
					printError((char*) "First column of GBRerank output must be the rank ( int )!");
					fclose(ifp);
					return false;
				}
				if (skipInitial(s, (char*)"#	 COLNAME new_score ", t))
				{
					newscoreLoc = m;
				}
				else if (skipInitial(s,(char*) "#	 COLNAME delgpol ", t))
				{
					delgpolLoc = m;
				}
				else if (skipInitial(s,(char*) "#	 COLNAME areaprop ", t))
				{
					areapropLoc = m;
				}
				else if (skipInitial(s,(char*) "#	 COLNAME rank ", t))
				{
					rankLoc = m;
				}
				else if (skipInitial(s,(char*) "#	 COLNAME score ", t))
				{
					scoreLoc = m;
				}
				else if (skipInitial(s,(char*) "#	 COLNAME mat1 ", t))
				{
					matLoc = m;
				}
				else if (skipInitial(s,(char*) "#	 COLNAME rmsd ", t))
				{
					rmsdLoc = m;
				}
				m++;
				if ((newrankLoc > 0) && (newscoreLoc > 0) && (delgpolLoc > 0) && (areapropLoc > 0)
						&& (rankLoc > 0) && (scoreLoc > 0) && (matLoc > 0) && (rmsdLoc > 0))
				{
					break;
				}
			}
		}
		else if (skipInitial(s,(char*) "# START PEAKS", t))
		{
			if (!((newrankLoc > 0) && (newscoreLoc > 0) && (delgpolLoc > 0) && (areapropLoc > 0)
						&& (rankLoc > 0) && (scoreLoc > 0) && (matLoc > 0) && (rmsdLoc > 0)))
			{
				printError((char*) "Missing format data in GBRerank output file ( %s )!", rerankingOutputFileName.c_str());
				fclose(ifp);
				return false;
			}
			if ((matLoc <= rmsdLoc) && (matLoc + 12 > rmsdLoc))
			{
				printError((char*) "Invalid format data in GBRerank output file ( %s )!", rerankingOutputFileName.c_str());
				fclose(ifp);
				return false;
			}
			while (fgets(s, 1999, ifp) != NULL)
			{
				if (skipInitial(s,(char*) "# END PEAKS", t))
				{
					break;
				}
				if (!gotNumSol)
				{
					double d;
					if (!getDoublesInRange(s, 1, 1, &d))
					{
						printError((char*) "Error reading GBRerank output file ( %s )!", rerankingOutputFileName.c_str());
						fclose(ifp);
						return false;
					}
					rerankNumSol = (int) floor(d + 0.5);
					index = numSol;
					gotNumSol = true;
					//cout<<numSol<<endl;
				}
				double trans[ 12 ], temp;


				DetailResults* dr = new DetailResults(_isDockingOutput);
				dr->setNewRank(index);
				index--;

				if (index<0)
				{
					break;
				}
				if (!getDoublesInRange(s, matLoc, matLoc + 11, trans))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rerankingOutputFileName.c_str());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->setMat(trans);
					/*for (int tempi = 0; tempi<12; tempi++)
					{
						dr->mat[tempi] = trans[tempi];
					}*/
				}
				if (!getDoublesInRange(s, newscoreLoc, newscoreLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rerankingOutputFileName.c_str());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->setNewScore(temp);
				}
				if (!getDoublesInRange(s, delgpolLoc, delgpolLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rerankingOutputFileName.c_str());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->setDelGpol(temp);
				}
				if (!getDoublesInRange(s, areapropLoc, areapropLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rerankingOutputFileName.c_str());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->setAreaProp(temp);
				}
				if (!getDoublesInRange(s, rankLoc, rankLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rerankingOutputFileName.c_str());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->setRank((int)floor(temp+0.5));
				}
				if (!getDoublesInRange(s, scoreLoc, scoreLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rerankingOutputFileName.c_str());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->setScore(temp);
				}
				if (!getDoublesInRange(s, rmsdLoc, rmsdLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rerankingOutputFileName.c_str());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->setRmsd(temp);
				}

				rerankingOutputCount++;

				rerankdetres.push_back(dr);

				// end of detailed results setting
			}
		}
		else if (skipInitial(s, (char*)"# total time = ", t))
		{
			double d;
			int i = 0;
			i = getDouble(t, i, &d);    // time in seconds
			time[1] = d;
		}
	}
	fclose(ifp);
	
	return true;
}



bool SummaryResults::mergeOutputs()
{
//	cout<<"inside merge output"<<endl;

	int dockingOutputCount;
	int rerankingOutputCount;

	if(!_dockingOutputProcessed && !_rerankingOutputProcessed) 
		return false;

	if(_dockingOutputProcessed && !_rerankingOutputProcessed) 	// no need to merge
		return true;

	if(!_dockingOutputProcessed && _rerankingOutputProcessed) 	// no need to merge. but need to move rerankdetres to detres
	{
		rerankingOutputCount = rerankdetres.size();

		detres.clear();

		for(int i=0; i<rerankingOutputCount; i++)
			detres.push_back(rerankdetres[i]);

		return true;
	}

	else								// we are going to sort the reranked part of the docking output using the new ranks
	{								// and update the corresponding values - {new rank, new score, delGpol and areaProp}
		rerankingOutputCount = rerankdetres.size();
		dockingOutputCount = detres.size();

		int index;

		vector<DetailResults*> temp;

		for(int i=0; i<rerankingOutputCount; i++)
		{
			index = rerankdetres[i]->getRank() - 1;	//ranks start at 1. now, result at index should move to i

			detres[index]->setNewRank(rerankdetres[i]->getNewRank());	//updating the values
			detres[index]->setNewScore(rerankdetres[i]->getNewScore());
			detres[index]->setDelGpol(rerankdetres[i]->getDelGpol());
			detres[index]->setAreaProp(rerankdetres[i]->getAreaProp());
	
			temp.push_back(detres[index]);
		}

		for(int i=0; i<rerankingOutputCount; i++)		//reassigning according to reranked ranks
		{
			detres[i] = temp[i];
		}

		return true;
	}

	return true;
}


bool SummaryResults::flipOutputs()
{
//	cout<<"inside flip output"<<endl;

	int dockingOutputCount, rerankingOutputCount, halfway;

	if(_dockingOutputProcessed)
	{
		dockingOutputCount = detres.size();
//		cout<<dockingOutputCount<<endl;
		halfway = dockingOutputCount/2;

		for(int i = 0; i<halfway; i++)
		{
			DetailResults* temp = detres[i];
			detres[i] = detres[dockingOutputCount-1-i]; 
			detres[dockingOutputCount-1-i] = temp;
		}
	}

	if(_rerankingOutputProcessed)
	{
		rerankingOutputCount = rerankdetres.size();
	
		halfway = rerankingOutputCount/2;

		for(int i = 0; i<halfway; i++)
		{
			DetailResults* temp = rerankdetres[i];
			rerankdetres[i] = rerankdetres[rerankingOutputCount-1-i]; 
			rerankdetres[rerankingOutputCount-1-i] = temp;
		}
	}

	return true;
}


bool SummaryResults::mineOutputs()
{
//	cout<<"inside mine output"<<endl;

	int dockingOutputCount, rerankingOutputCount;
	double rmsdGood = 5.0;
	bool gotPeak;

	if(!_rmsdPresent) return true;

	if(!_dockingOutputProcessed && !_rerankingOutputProcessed) 
		return false;	

	if(_rerankingOutputProcessed)
	{
		rerankingOutputCount = rerankdetres.size();

		gotPeak = false;

		for(int i=0; i<rerankingOutputCount; i++)
		{
			if (rerankdetres[i]->getRmsd() < rmsdGood)
			{
				int newrank = rerankdetres[i]->getNewRank();

				//counting hits

				if (newrank <= 1)
				{
					hitsInRangeAfterRerank[0]++;
				}
				if (newrank <= 10)
				{
					hitsInRangeAfterRerank[1]++;
				}
				if (newrank <= 100)
				{
					hitsInRangeAfterRerank[2]++;
				}
				if (newrank <= 1000)
				{
					hitsInRangeAfterRerank[3]++;
				}
				if (newrank <= 10000)
				{
					hitsInRangeAfterRerank[4]++;
				}
				if (newrank <= 100000)
				{
					hitsInRangeAfterRerank[5]++;
				}
			
				//fixing best ranked peak

				if(minRankedPeak[1] == -1)
					minRankedPeak[1] = newrank-1;

				//fixing best rmsd
			
				if (rerankdetres[i]->getRmsd() < minRMSD[1] || minRMSD[1] == -1.0)
				{
					indexMinRMSD[1] = newrank;
					minRMSD[1] = rerankdetres[i]->getRmsd();
				}
	
				gotPeak = true;
				numGoodPeaks[1]++;
			}
		}
		if(gotPeak)
		{
			minRankedPeakRMSD[1] = rerankdetres[minRankedPeak[1]]->getRmsd();
			minRankedPeakScore[1] = rerankdetres[minRankedPeak[1]]->getNewScore();

			scoreMinRMSD[1] = rerankdetres[indexMinRMSD[1]]->getNewScore();	
		}
	}


	if(_dockingOutputProcessed)
	{
		dockingOutputCount = detres.size();

		gotPeak = false;

//		cout<<"summarizing docking hits"<<endl;
//		cout<<dockingOutputCount<<endl;
		for(int i=0; i<dockingOutputCount; i++)
		{

			if (detres[i]->getRmsd() < rmsdGood)
			{
				int rank = detres[i]->getRank();

//				cout<<"hit "<<i<<" "<<rank<<endl;
				//counting hits

				if (rank <= 1)
				{
					hitsInRange[0]++;
				}
				if (rank <= 10)
				{
					hitsInRange[1]++;
				}
				if (rank <= 100)
				{
					hitsInRange[2]++;
				}
				if (rank <= 1000)
				{
					hitsInRange[3]++;
				}
				if (rank <= 10000)
				{
					hitsInRange[4]++;
				}
				if (rank <= 100000)
				{
					hitsInRange[5]++;
				}
			
				//fixing best ranked peak

				if(minRankedPeak[0] == -1)
					minRankedPeak[0] = rank-1;

				//fixing best rmsd
			
				if (detres[i]->getRmsd() < minRMSD[0] || minRMSD[0] == -1.0)
				{
					indexMinRMSD[0] = rank;
					minRMSD[0] = detres[i]->getRmsd();
				}
	
				gotPeak = true;
				numGoodPeaks[0]++;
			}		
		}
/*		cout<<numGoodPeaks[0]<<endl;
		for(int i=0; i<6; i++)
		{
			cout<<hitsInRange[i]<<endl;
		}*/
		if(gotPeak)
		{
			minRankedPeakRMSD[0] = detres[minRankedPeak[0]]->getRmsd();
			minRankedPeakScore[0] = detres[minRankedPeak[0]]->getScore();

			scoreMinRMSD[0] = detres[indexMinRMSD[0]]->getScore();	
		}
	}
	return true;
}



bool SummaryResults::readDockingOutput(string outputFileName, bool rmsd)
{
	dockingOutputFileName = outputFileName;
	init();
	_isDockingOutput = true;
	_rmsdPresent = rmsd;

	if(!readOutputFromFile()) return false;

	return true;
}


bool SummaryResults::readRerankingOutput(string outputFileName, bool rmsd)
{
	rerankingOutputFileName = outputFileName;
	init();
	_isRerankingOutput = true;
	_rmsdPresent = true;

	if(!readOutputFromFile()) return false;

	return true;
}

bool SummaryResults::readOutput(string outputFileName1, string outputFileName2, bool rmsd)
{
//	cout<<"inside read output"<<endl;

	cout<<outputFileName1.c_str();
	cout<<outputFileName2.c_str();

	dockingOutputFileName = outputFileName1;
	rerankingOutputFileName = outputFileName2;
	init();
	_isDockingOutput = true;	
	_isRerankingOutput = true;
	_rmsdPresent = true;

//	cout<<"performed init"<<endl;

	if(!readOutputFromFile()) return false;

//	cout<<"read complete"<<endl;

	return true;
}


DetailResults* SummaryResults::getDetailResult(int i)
{
//	cout<<detres.size()<<endl;

	if(i<detres.size())
	{
		return detres[i];
	}
	else
	{
		return NULL;
	}
}


int SummaryResults::computeInterfaceStats(string pqrFileName1, string pqrFileName2, string quadFileName1, string quadFileName2, int num)
{
#ifdef WITH_INTERFACE_STATS
	vector<Matrix> transformations;

	int numResult = detres.size();
	if (num>numResult) num = numResult;

	for(int i=0; i<num; i++)
		transformations.push_back(detres[i]->getTransformationMatrix());

	compStats = new ComputeInterfaceStats( pqrFileName1, pqrFileName2, quadFileName1, quadFileName2, transformations );
	
	for(int i=0; i<num; i++)
		detres[i]->setInterfaceStats(compStats->getInterfaceStats(i));

	numStats = num;
	return num;
#else
	return 0;
#endif

}
