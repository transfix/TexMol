#include <qapplication.h>
#include <qlineedit.h>
//Added by qt3to4:
#include <QCustomEvent>
#include <QEvent>

#include <F2DockResult.h>

#include <Utility/utility.h>


//  Constructs a F2DockResult which is a child of 'parent', with the name 'name' and widget flags set to 'f'
// The dialog will by default be modeless, unless you set 'modal' to true to construct a modal dialog.
F2DockResult::F2DockResult(QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl)
	: f2DockResult(parent, name, modal, fl), my_parent(parent)
{
}

F2DockResult::~F2DockResult()
{
	// no need to delete child widgets, Qt does it all for us
}

class ViewRowEventResult : public QCustomEvent
{
	public:
		ViewRowEventResult(const QString& m, double* t, const bool s) : QCustomEvent(QEvent::User+103), msg(m), mat(t), surf(s) {}
		QString message() const
		{
			return msg;
		}
		double* matrix()
		{
			return mat;
		}
		bool surface() const
		{
			return surf;
		}
	private:
		QString msg;
		double* mat;
		bool surf;
};

// Public slot
void F2DockResult::detailsSlot()
{
	detresDialog = new F2DockResultDetails(this);
	detresDialog->setParams(&detres, baspar);
	detresThread = new DetailResultThread(this);
	detresThread->setForm(detresDialog);
	detresThread->start();
	//send the receptor pdb to mainwindow here
	//ligand pdbs will be sent by F2DockResultDetails
	QApplication::postEvent(my_parent, new ViewRowEventResult(baspar->receptorPDB, 0, false));
}

/*Q3Err:I
F2DockResult::DetailResultThread::DetailResultThread(F2DockResult* par,  unsigned int stackSize)  : QThread(stackSize), parent(par)
*/
F2DockResult::DetailResultThread::DetailResultThread(F2DockResult* par,  unsigned int stackSize)  : QThread(this), parent(par)
{
}

void F2DockResult::DetailResultThread::setForm(F2DockResultDetails* f)
{
	fdm = f;
}

void F2DockResult::DetailResultThread::run()
{
	cout<<"Launching Detailed results"<<endl;
	//	fdm->exec();
	fdm->show();
}

// Public slot
void F2DockResult::doneSlot()
{
	this->close();
}

void F2DockResult::updateValues()
{
	if (baspar->bound)
	{
		DM_TB->setText("Bound-Bound Docking");
	}
	else
	{
		DM_TB->setText("Unbound-Unbound Docking");
	}
	RC_TB->setText(baspar->receptorFile);
	LG_TB->setText(baspar->ligandFile);
	RM_TB->setText(baspar->rmsdFile);
	OU_TB->setText(baspar->outputFile);
	char ss[20], sc[20], cc[20], ee[20], rs[20], gs[20];
	sprintf(ss, "%0.2lf", baspar->ssWeight);
	sprintf(sc, "%0.2lf", baspar->scWeight);
	sprintf(cc, "%0.2lf", baspar->ccWeight);
	sprintf(ee, "%0.2lf", baspar->elecWeight);
	sprintf(rs, "%2d", baspar->rotSeparation);
	sprintf(gs, "%0.2lf", baspar->gridSpacing);
	SS_TB->setText(ss);
	SC_TB->setText(sc);
	CC_TB->setText(cc);
	EE_TB->setText(ee);
	RS_TB->setText(rs);
	GS_TB->setText(gs);
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
	totPeak_TB->setText(totPeak);
	rangePeak_TB_1->setText(rangePeak[0]);
	rangePeak_TB_2->setText(rangePeak[1]);
	rangePeak_TB_3->setText(rangePeak[2]);
	rangePeak_TB_4->setText(rangePeak[3]);
	rangePeak_TB_5->setText(rangePeak[4]);
	rank_TB->setText(rank[0]);
	rmsd_TB->setText(rmsd[0]);
	ts_TB->setText(score[0]);
	rank_TB_2->setText(rank[1]);
	rmsd_TB_2->setText(rmsd[1]);
	ts_TB_2->setText(score[1]);
	time_TB->setText(time);
}

void F2DockResult::setParams(BasicParams* bp)
{
	baspar = bp;
	if (getValues())
	{
		updateValues();
	}
}

bool F2DockResult::getValues()
{
	double rmsdGood = 5.0;
	int index;
	FILE* ifp = fopen(baspar->outputFile.ascii(), "rt");
	if (ifp == NULL)
	{
		error("Failed to open F2Dock output file !");
	}
	char s[ 2000 ], t[ 2000 ];
	int rankLoc = 1, scoreLoc = 0, shapeLoc = 0;			// columns for each detailed result item
	int ssrLoc = 0, ccrLoc = 0, scrLoc = 0;
	int elecLoc = 0, hbondLoc = 0, vdwLoc = 0, clashesLoc = 0;
	int matLoc = 0, confLoc = 0, rmsdLoc = 0;
	bool gotNumSol = false;
	bool gotPeak = false;
	while (fgets(s, 1999, ifp) != NULL)
	{
		if (skipInitial(s, "# OUTPUT FORMAT:", t))
		{
			int m;
			if (sscanf(t, "%d", &m) != 1)
			{
				error("Error reading F2Dock output file !");
				return false;
			}
			m = 1;
			while (fgets(s, 1999, ifp) != NULL)
			{
				if ((m == 1) && !skipInitial(s, "#	 COLNAME rank int", t))
				{
					error("First column of F2Dock output must be the rank ( int )!");
					return false;
				}
				if (skipInitial(s, "#	 COLNAME score ", t))
				{
					scoreLoc = m;
				}
				else if (skipInitial(s, "#	 COLNAME shape ", t))
				{
					shapeLoc = m;
				}
				else if (skipInitial(s, "#	 COLNAME ssr ", t))
				{
					ssrLoc = m;
				}
				else if (skipInitial(s, "#	 COLNAME ccr ", t))
				{
					ccrLoc = m;
				}
				else if (skipInitial(s, "#	 COLNAME scr ", t))
				{
					scrLoc = m;
				}
				else if (skipInitial(s, "#	 COLNAME elec ", t))
				{
					elecLoc = m;
				}
				else if (skipInitial(s, "#	 COLNAME hbond ", t))
				{
					hbondLoc = m;
				}
				else if (skipInitial(s, "#	 COLNAME vdw ", t))
				{
					vdwLoc = m;
				}
				else if (skipInitial(s, "#	 COLNAME clashes ", t))
				{
					clashesLoc = m;
				}
				else if (skipInitial(s, "#	 COLNAME mat1 ", t))
				{
					matLoc = m;
				}
				else if (skipInitial(s, "#	 COLNAME conf ", t))
				{
					confLoc = m;
				}
				else if (skipInitial(s, "#	 COLNAME rmsd ", t))
				{
					rmsdLoc = m;
				}
				m++;
				if ((rankLoc > 0) && (shapeLoc > 0) && (ssrLoc > 0) && (ccrLoc > 0) && (scrLoc > 0) && (elecLoc > 0) && (hbondLoc > 0) && (vdwLoc > 0) && (clashesLoc > 0) && (matLoc > 0) && (confLoc > 0) && (rmsdLoc > 0))
				{
					break;
				}
			}
			// cout<< rankLoc <<endl;
			// cout<< elecLoc <<endl;
			// cout<< matLoc <<endl;
			// cout<< rmsdLoc <<endl;
		}
		else if (skipInitial(s, "# START PEAKS", t))
		{
			if (!((rankLoc > 0) && (shapeLoc > 0) && (ssrLoc > 0) && (ccrLoc > 0) && (scrLoc > 0) && (elecLoc > 0) && (hbondLoc > 0) && (vdwLoc > 0) && (clashesLoc > 0) && (matLoc > 0) && (confLoc > 0) && (rmsdLoc > 0)))
			{
				error("Missing format data in F2Dock output file !");
			}
			if ((matLoc <= rmsdLoc) && (matLoc + 12 > rmsdLoc))
			{
				error("Invalid format data in F2Dock output file !");
			}
			while (fgets(s, 1999, ifp) != NULL)
			{
				if (skipInitial(s, "# END PEAKS", t))
				{
					break;
				}
				if (!gotNumSol)
				{
					double d;
					if (!getDoublesInRange(s, 1, 1, &d))
					{
						error("Error reading F2Dock output file !");
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
				DetailResults* dr = new DetailResults;
				dr->rank = index;
				index--;
				if (index<0)
				{
					break;
				}
				if (!getDoublesInRange(s, matLoc, matLoc + 11, trans))
				{
					error("Error reading F2Dock output file !");
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
					error("Error reading F2Dock output file !");
				}
				else
				{
					dr->score = temp;
				}
				if (!getDoublesInRange(s, shapeLoc, shapeLoc,  &temp))
				{
					error("Error reading F2Dock output file !");
				}
				else
				{
					dr->shape = temp;
				}
				if (!getDoublesInRange(s, ssrLoc, ssrLoc,  &temp))
				{
					error("Error reading F2Dock output file !");
				}
				else
				{
					dr->ssr = temp;
				}
				if (!getDoublesInRange(s, ccrLoc, ccrLoc,  &temp))
				{
					error("Error reading F2Dock output file !");
				}
				else
				{
					dr->ccr = temp;
				}
				if (!getDoublesInRange(s, scrLoc, scrLoc,  &temp))
				{
					error("Error reading F2Dock output file !");
				}
				else
				{
					dr->scr = temp;
				}
				if (!getDoublesInRange(s, elecLoc, elecLoc,  &temp))
				{
					error("Error reading F2Dock output file !");
				}
				else
				{
					dr->elec = temp;
				}
				if (!getDoublesInRange(s, hbondLoc, hbondLoc,  &temp))
				{
					error("Error reading F2Dock output file !");
				}
				else
				{
					dr->hbond = temp;
				}
				if (!getDoublesInRange(s, vdwLoc, vdwLoc,  &temp))
				{
					error("Error reading F2Dock output file !");
				}
				else
				{
					dr->vdw = temp;
				}
				if (!getDoublesInRange(s, clashesLoc, clashesLoc, &temp))
				{
					error("Error reading F2Dock output file !");
				}
				else
				{
					dr->clashes = (int)floor(temp+0.5);
				}
				if (!getDoublesInRange(s, confLoc, confLoc,  &temp))
				{
					error("Error reading F2Dock output file !");
				}
				else
				{
					dr->conf = (int)floor(temp+0.5);
				}
				if (!getDoublesInRange(s, rmsdLoc, rmsdLoc,  &temp))
				{
					error("Error reading F2Dock output file !");
				}
				else
				{
					dr->rmsd = temp;
				}
				if (dr->rmsd < rmsdGood)
				{
					//cout<<dr->rmsd<<endl;
					if (dr->rank <= 1)
					{
						sumres.hitsInRange[0]++;
					}
					if (dr->rank <= 10)
					{
						sumres.hitsInRange[1]++;
					}
					if (dr->rank <= 100)
					{
						sumres.hitsInRange[2]++;
					}
					if (dr->rank <= 1000)
					{
						sumres.hitsInRange[3]++;
					}
					if (dr->rank <= 10000)
					{
						sumres.hitsInRange[4]++;
					}
					if (dr->rank <= 100000)
					{
						sumres.hitsInRange[5]++;
					}
					sumres.numGoodPeaks++;
					sumres.minRankedPeak = dr->rank;
					if (dr->rmsd < sumres.minRMSD || sumres.minRMSD == -1)
					{
						sumres.indexMinRMSD = dr->rank;
						sumres.minRMSD = dr->rmsd;
					}
					gotPeak = true;
				}
				detres.push_back(dr);
			}
		}
		else if (skipInitial(s, "# total time = ", t))
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
		sumres.minRankedPeakRMSD = detres[sumres.numSol - sumres.minRankedPeak]->rmsd;
		sumres.minRankedPeakScore = detres[sumres.numSol - sumres.minRankedPeak]->score;
		sumres.scoreMinRMSD = detres[sumres.numSol - sumres.indexMinRMSD]->score;
	}
	return true;
}

/*class ViewRowEventResult : public QCustomEvent
{
	public:
		ViewRowEventResult(const QString& m) : QCustomEvent(QEvent::User+103), msg(m) {}
		QString message() const
		{
			return msg;
		}
	private:
		QString msg;
};*/

void F2DockResult::customEvent(QCustomEvent* e)
{
	int index;
	if (e->type() == QEvent::User+102)
	{
		QApplication::postEvent(my_parent, new ViewRowEventResult(static_cast<ViewRowEventResult*>(e)->message(), static_cast<ViewRowEventResult*>(e)->matrix(), static_cast<ViewRowEventResult*>(e)->surface()));
	}
}
