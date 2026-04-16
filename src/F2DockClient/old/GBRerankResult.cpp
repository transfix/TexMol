#include <Utility/utility.h>
#include <GBRerankResult.h>
#include <qlineedit.h>

// Constructs a GBRerankResult which is a child of 'parent', with the name 'name' and widget flags set to 'f'
// The dialog will by default be modeless, unless you set 'modal' to  true to construct a modal dialog.
GBRerankResult::GBRerankResult(QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl)
	: gbRerankResult(parent, name, modal, fl)
{
}

GBRerankResult::~GBRerankResult()
{
	// no need to delete child widgets, Qt does it all for us
}

// Public slot
void GBRerankResult::detailedsSlot()
{
	detresDialog = new GBRerankResultDetails(this);
	detresDialog->setParams(&detres, rbaspar);
	detresThread = new GBDetailResultThread(this);
	detresThread->setForm(detresDialog);
	detresThread->start();
}

/*Q3Err:CS
GBRerankResult::GBDetailResultThread::GBDetailResultThread(GBRerankResult* par,  unsigned int stackSize)  : QThread(stackSize), parent(par)
*/
GBRerankResult::GBDetailResultThread::GBDetailResultThread(GBRerankResult* par,  unsigned int stackSize)  : QThread(this), parent(par)
{
}

void GBRerankResult::GBDetailResultThread::setForm(GBRerankResultDetails* f)
{
	fdm = f;
}

void GBRerankResult::GBDetailResultThread::run()
{
	cout<<"Launching Detailed results"<<endl;
	//	fdm->exec();
	fdm->show();
}

// Public slot
void GBRerankResult::doneSlot()
{
	this->close();
}

void GBRerankResult::updateValues()
{
	receptorPQR_TB->setText(rbaspar->receptorPDB);
	ligandPQR_TB->setText(rbaspar->ligandPDB);
	receptorQUAD_TB->setText(rbaspar->receptorQUAD);
	ligandQUAD_TB->setText(rbaspar->ligandQUAD);
	f2dockOutput_TB->setText(rbaspar->f2dockOutputFile);
	gbRerankOutput_TB->setText(rbaspar->rerankOutputFile);
	char gpw[20], gnpw[20], f2dw[20], ebr[20], egp[20];
	//char ns[20];
	sprintf(gpw, "%0.2lf", rbaspar->gpolWeight);
	sprintf(gnpw, "%0.2lf", rbaspar->gnonpolWeight);
	sprintf(f2dw, "%0.2lf", rbaspar->f2dockWeight);
	sprintf(ebr, "%0.2lf", rbaspar->epsilonBR);
	sprintf(egp, "%2lf", rbaspar->epsilonGpol);
	//sprintf(ns, "%0.2lf", rbaspar->gridSpacing);
	gpol_TB->setText(gpw);
	gnonpol_TB->setText(gnpw);
	f2dockScore_TB->setText(f2dw);
	epsilonBR_TB->setText(ebr);
	epsilonGpol_TB->setText(egp);
	//numSol_TB->setText(ns);
	char totPeak[20], rangePeak[5][20], rank[2][20], rmsd[2][20], score[2][20], shape[2][20], time[20];
	sprintf(totPeak, "%d", sumres.numGoodPeaks);
	for (int i=0; i<5; i++)
	{
		sprintf(rangePeak[i], "%d", sumres.hitsInRange[i]);
	}
	sprintf(rank[0], "%d", sumres.minRankedPeak);
	sprintf(rmsd[0], "%0.4lf", sumres.minRankedPeakRMSD);
	sprintf(score[0], "%0.4lf", sumres.minRankedPeakScore);
	sprintf(rank[1], "%d", sumres.indexMinRMSD);
	sprintf(rmsd[1], "%0.4lf", sumres.minRMSD);
	sprintf(score[1], "%0.4lf", sumres.scoreMinRMSD);
	sprintf(time, "%0.7lf", sumres.time);
	totalPeaks_TB->setText(totPeak);
	range1Peaks_TB->setText(rangePeak[0]);
	range2Peaks_TB->setText(rangePeak[1]);
	range3Peaks_TB->setText(rangePeak[2]);
	range4Peaks_TB->setText(rangePeak[3]);
	range5Peaks_TB->setText(rangePeak[4]);
	rank1_TB->setText(rank[0]);
	rmsd1_TB->setText(rmsd[0]);
	score1_TB->setText(score[0]);
	rank2_TB->setText(rank[1]);
	rmsd2_TB->setText(rmsd[1]);
	score2_TB->setText(score[1]);
	time_TB->setText(time);
}

void GBRerankResult::setParams(RerankBasicParams* rbp)
{
	rbaspar = rbp;
	if (getValues())
	{
		updateValues();
	}
}

bool GBRerankResult:: getValues()
{
	double rmsdGood = 5.0;
	int index;
	FILE* ifp = fopen(rbaspar->rerankOutputFile.ascii(), "rt");
	if (ifp == NULL)
	{
		printError((char*) "Failed to open GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
		return false;
	}
	char s[ 2000 ], t[ 2000 ];
	int newrankLoc = 1, newscoreLoc = 0, delgpolLoc = 0;			// columns for each detailed result item
	int areapropLoc = 0, rankLoc = 0, scoreLoc = 0;
	int elecLoc = 0, hbondLoc = 0, vdwLoc = 0;
	int matLoc = 0, confLoc = 0, rmsdLoc = 0;
	bool gotNumSol = false;
	bool gotPeak = false;
	while (fgets(s, 1999, ifp) != NULL)
	{
		if (skipInitial(s, (char*) "# OUTPUT FORMAT:", t))
		{
			int m;
			if (sscanf(t, "%d", &m) != 1)
			{
				printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
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
				else if (skipInitial(s,(char*) "#	 COLNAME elec ", t))
				{
					elecLoc = m;
				}
				else if (skipInitial(s,(char*) "#	 COLNAME hbond ", t))
				{
					hbondLoc = m;
				}
				else if (skipInitial(s,(char*) "#	 COLNAME vdw ", t))
				{
					vdwLoc = m;
				}
				else if (skipInitial(s,(char*) "#	 COLNAME mat1 ", t))
				{
					matLoc = m;
				}
				else if (skipInitial(s,(char*) "#	 COLNAME conf ", t))
				{
					confLoc = m;
				}
				else if (skipInitial(s,(char*) "#	 COLNAME rmsd ", t))
				{
					rmsdLoc = m;
				}
				m++;
				if ((newrankLoc > 0) && (newscoreLoc > 0) && (delgpolLoc > 0) && (areapropLoc > 0)
						&& (rankLoc > 0) && (scoreLoc > 0) && (elecLoc > 0) && (hbondLoc > 0)
						&& (vdwLoc > 0) && (matLoc > 0) && (confLoc > 0) && (rmsdLoc > 0))
				{
					break;
				}
			}
		}
		else if (skipInitial(s,(char*) "# START PEAKS", t))
		{
			if (!((newrankLoc > 0) && (newscoreLoc > 0) && (delgpolLoc > 0) && (areapropLoc > 0)
					&& (rankLoc > 0) && (scoreLoc > 0) && (elecLoc > 0) && (hbondLoc > 0)
					&& (vdwLoc > 0) && (matLoc > 0) && (confLoc > 0) && (rmsdLoc > 0)))
			{
				printError((char*) "Missing format data in GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
				fclose(ifp);
				return false;
			}
			if ((matLoc <= rmsdLoc) && (matLoc + 12 > rmsdLoc))
			{
				printError((char*) "Invalid format data in GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
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
						printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
						fclose(ifp);
						return false;
					}
					sumres.numSol = (int) floor(d + 0.5);
					index = sumres.numSol;
					for (int j = 0; j < 6; j++)
					{
						sumres.hitsInRange[ j ] = 0;
					}
					sumres.numGoodPeaks = 0;
					sumres.minRankedPeak = 0;
					sumres.indexMinRMSD = 0;
					sumres.minRMSD = -1;
					gotNumSol = true;
					//cout<<sumres.numSol<<endl;
				}
				double trans[ 12 ], temp;
				// do detailed results later!!
				RerankDetailResults* dr = new RerankDetailResults;
				dr->newRank = index;
				index--;
				if (index<0)
				{
					break;
				}
				if (!getDoublesInRange(s, matLoc, matLoc + 11, trans))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
					fclose(ifp);
					return false;
				}
				else
				{
					for (int tempi = 0; tempi<12; tempi++)
					{
						dr->mat[tempi] = trans[tempi];
					}
				}
				if (!getDoublesInRange(s, scoreLoc, scoreLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->newScore = temp;
				}
				if (!getDoublesInRange(s, delgpolLoc, delgpolLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->delGpol = temp;
				}
				if (!getDoublesInRange(s, areapropLoc, areapropLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->areaProp = temp;
				}
				if (!getDoublesInRange(s, rankLoc, rankLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->rank = (int)floor(temp+0.5);
				}
				if (!getDoublesInRange(s, scoreLoc, scoreLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->score = temp;
				}
				if (!getDoublesInRange(s, elecLoc, elecLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->elec = temp;
				}
				if (!getDoublesInRange(s, hbondLoc, hbondLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->hbond = temp;
				}
				if (!getDoublesInRange(s, vdwLoc, vdwLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->vdw = temp;
				}
				if (!getDoublesInRange(s, confLoc, confLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->conf = (int)floor(temp+0.5);
				}
				if (!getDoublesInRange(s, rmsdLoc, rmsdLoc,  &temp))
				{
					printError((char*) "Error reading GBRerank output file ( %s )!", rbaspar->rerankOutputFile.ascii());
					fclose(ifp);
					return false;
				}
				else
				{
					dr->rmsd = temp;
				}
				if (dr->rmsd < rmsdGood)
				{
					if (dr->newRank <= 1)
					{
						sumres.hitsInRange[0]++;
					}
					if (dr->newRank <= 10)
					{
						sumres.hitsInRange[1]++;
					}
					if (dr->newRank <= 100)
					{
						sumres.hitsInRange[2]++;
					}
					if (dr->newRank <= 1000)
					{
						sumres.hitsInRange[3]++;
					}
					if (dr->newRank <= 10000)
					{
						sumres.hitsInRange[4]++;
					}
					if (dr->newRank <= 100000)
					{
						sumres.hitsInRange[5]++;
					}
					sumres.numGoodPeaks++;
					sumres.minRankedPeak = dr->newRank;
					if (dr->rmsd < sumres.minRMSD || sumres.minRMSD == -1)
					{
						sumres.indexMinRMSD = dr->newRank;
						sumres.minRMSD = dr->rmsd;
					}
					gotPeak = true;
				}
				detres.push_back(dr);
				// end of detailed results setting
			}
		}
		else if (skipInitial(s, (char*)"# total time = ", t))
		{
			double d;
			int i = 0;
			i = getDouble(t, i, &d);    // time in seconds
			sumres.time = d;
		}
	}
	fclose(ifp);
	if (gotPeak)
	{
		// need detailed results to get these values!!
		sumres.minRankedPeakRMSD = detres[sumres.numSol - sumres.minRankedPeak]->rmsd;
		sumres.minRankedPeakScore = detres[sumres.numSol - sumres.minRankedPeak]->newScore;
		sumres.scoreMinRMSD = detres[sumres.numSol - sumres.indexMinRMSD]->newScore;
	}
	return true;
}
