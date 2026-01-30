#include "F2DockClient/GBRerankForm.h"
#include <iostream>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <q3filedialog.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>

// Constructs a GBRerankForm which is a child of 'parent', with the name 'name' and widget flags set to 'f'
// The dialog will by default be modeless, unless you set 'modal' to TRUE to construct a modal dialog.
GBRerankForm::GBRerankForm(QWidget* parent,  const char* name, bool modal, Qt::WFlags fl)
	: GBRerank(parent, name, modal, fl)
{
	receptorPDBFileFound = false;
	ligandPDBFileFound = false;
	receptorQUADFileFound = false;
	ligandQUADFileFound = false;
	f2dockOutputFileFound = false;
	gbRerankInputFileFound = false;
	gbRerankOutputFileFound = false;
	resultAvailable = false;
	jobSubmitted = false;
}

GBRerankForm::~GBRerankForm()
{
	// no need to delete child widgets, Qt does it all for us
}

// Public slot
void GBRerankForm::selectReceptorPQRFileSlot()
{
	rbaspar->receptorPDB = Q3FileDialog::getOpenFileName("", "Select Receptor PDB file (*.pdb);;All files (*.*)");
/*Q3Err:C
	if (rbaspar->receptorPDB == 0)
*/
	if (rbaspar->receptorPDB.isNull())
	{
		QMessageBox::information(this, "Select Receptor PDB File",  "Unable to find the file.\n");
	}
	else
	{
		this->receptorPQR_TB->setText(rbaspar->receptorPDB);
	}
}

// Public slot
void GBRerankForm::selectLigandPQRFileSlot()
{
	rbaspar->ligandPDB = Q3FileDialog::getOpenFileName("", "Select Ligand PDBfile (*.pdb);;All files (*.*)");
/*Q3Err:C
	if (rbaspar->ligandPDB == 0)
*/
	if (rbaspar->ligandPDB.isNull())
	{
		QMessageBox::information(this, "Select Ligand PDB File",  "Unable to find the file.\n");
	}
	else
	{
		this->ligandPQR_TB->setText(rbaspar->ligandPDB);
	}
}

// Public slot
void GBRerankForm::selectReceptorQUADFileSlot()
{
	rbaspar->receptorQUAD = Q3FileDialog::getOpenFileName("", "Select Receptor QUAD file (*.quad);;All files (*.*)");
/*Q3Err:C
	if (rbaspar->receptorQUAD == 0)
*/
	if (rbaspar->receptorQUAD.isNull())
	{
		QMessageBox::information(this, "Select Receptor QUAD File",  "Unable to find the file.\n");
	}
	else
	{
		this->receptorQUAD_TB->setText(rbaspar->receptorQUAD);
	}
}

// Public slot
void GBRerankForm::selectLigandQUADFileSlot()
{
	rbaspar->ligandQUAD = Q3FileDialog::getOpenFileName("", "Select Ligand QUAD file (*.quad);;All files (*.*)");
/*Q3Err:C
	if (rbaspar->ligandQUAD == 0)
*/
	if (rbaspar->ligandQUAD.isNull())
	{
		QMessageBox::information(this, "Select Ligand QUAD File",  "Unable to find the file.\n");
	}
	else
	{
		this->ligandQUAD_TB->setText(rbaspar->ligandQUAD);
	}
}

// Public slot
void GBRerankForm::selectF2dockOutputFileSlot()
{
	rbaspar->f2dockOutputFile = Q3FileDialog::getOpenFileName("", "Select F2Dock output file (*.out);;All files (*.*)");
/*Q3Err:C
	if (rbaspar->f2dockOutputFile == 0)
*/
	if (rbaspar->f2dockOutputFile.isNull())
	{
		QMessageBox::information(this, "Select F2Dock output File",  "Unable to find the file.\n");
	}
	else
	{
		this->f2dockOutput_TB->setText(rbaspar->f2dockOutputFile);
	}
}

// Public slot
void GBRerankForm::selectGbrerankOutputFileSlot()
{
	rbaspar->rerankOutputFile = Q3FileDialog::getSaveFileName("", "Select output file (*.out);;All files (*.*)");
/*Q3Err:C
	if (rbaspar->rerankOutputFile == 0)
*/
	if (rbaspar->rerankOutputFile.isNull())
	{
		QMessageBox::information(this, "Select output File",  "Unable to create the file.\n");
	}
	else
	{
		this->gbrerankOutput_TB->setText(rbaspar->rerankOutputFile);
	}
}

// Public slot
void GBRerankForm::resetSlot()
{
	setDefaults();
}

// Public slot
void GBRerankForm::rerankSlot()
{
	prepareParams();
	if (checkValidity())
	{
		if (prepareInputFile())
		{
			if (rerank())
			{
				jobSubmitted = true;
				resultAvailable = false;
				rbaspar->submitted = true;
				rbaspar->completed = false;
				this->close();
			}
		}
	}
}

void GBRerankForm::setDefaults()
{
	//rbaspar->hostName = "optic.ices.utexas.edu";
	//rbaspar->port = 8091;
	this->gpol_TB->setText("-1");
	this->gnonpol_TB->setText("0.005");
	this->epsilonBR_TB->setText("0.5");
	this->epsilonGpol_TB->setText("0.7");
	this->f2dockScore_TB->setText("0");
	this->numSol_TB->setText("2000");
	rbaspar->gpolWeight = -1;
	rbaspar->gnonpolWeight = 0.005;
	rbaspar->f2dockWeight = 0.5;
	rbaspar->epsilonBR = 0.7;
	rbaspar->epsilonGpol = 0;
	rbaspar->numSol = 2000;
}

bool GBRerankForm::checkValidity()
{
	receptorPDBFP = fopen(rbaspar->receptorPDB.ascii(), "rb");
	if (receptorPDBFP == NULL)
	{
		QMessageBox::information(this, "Select Receptor PDB File",  "Unable to find the file.\n");
		receptorPDBFileFound = false;
		return false;
	}
	else
	{
		receptorPDBFileFound = true;
	}
	ligandPDBFP = fopen(rbaspar->ligandPDB.ascii(), "rb");
	if (ligandPDBFP == NULL)
	{
		QMessageBox::information(this, "Select Ligand PDB File",  "Unable to find the file.\n");
		ligandPDBFileFound = false;
		return false;
	}
	else
	{
		ligandPDBFileFound = true;
	}
	receptorQUADFP = fopen(rbaspar->receptorQUAD.ascii(), "rb");
	if (receptorQUADFP == NULL)
	{
		QMessageBox::information(this, "Select Receptor QUAD File",  "Unable to find the file.\n");
		receptorQUADFileFound = false;
		return false;
	}
	else
	{
		receptorQUADFileFound = true;
	}
	ligandQUADFP = fopen(rbaspar->ligandQUAD.ascii(), "rb");
	if (ligandQUADFP == NULL)
	{
		QMessageBox::information(this, "Select Ligand QUAD File",  "Unable to find the file.\n");
		ligandQUADFileFound = false;
		return false;
	}
	else
	{
		ligandQUADFileFound = true;
	}
	f2dOutFP = fopen(rbaspar->f2dockOutputFile.ascii(), "rb");
	if (f2dOutFP == NULL)
	{
		QMessageBox::information(this, "Select F2Dock Output File",  "Unable to find the file.\n");
		f2dockOutputFileFound = false;
		return false;
	}
	else
	{
		f2dockOutputFileFound = true;
	}
	gbOutFP = fopen(rbaspar->rerankOutputFile.ascii(), "wb");
	if (gbOutFP == NULL)
	{
		QMessageBox::information(this, "Select Output File",  "Unable to find the file.\n");
		gbRerankOutputFileFound = false;
		return false;
	}
	else
	{
		gbRerankOutputFileFound = true;
	}
	return true;
}

bool GBRerankForm::rerank()
{
	client.setGBRerankValues(rbaspar);
	if (client.submitGBRerankRequest())
	{
		QMessageBox::information(this, "Reranking job Submitted",  "Reranking job submitted.\n");
		return true;
	}
	else
	{
		QMessageBox::information(this, "Failed to submit reranking job",  "Failed to submit reranking job.\n");
		return false;
	}
}

void GBRerankForm::closeFiles()
{
	if (receptorPDBFP)
	{
		fclose(receptorPDBFP);
	}
	if (ligandPDBFP)
	{
		fclose(ligandPDBFP);
	}
	if (receptorQUADFP)
	{
		fclose(receptorQUADFP);
	}
	if (ligandQUADFP)
	{
		fclose(ligandQUADFP);
	}
	if (f2dOutFP)
	{
		fclose(f2dOutFP);
	}
	if (gbOutFP)
	{
		fclose(gbOutFP);
	}
}

bool GBRerankForm::prepareInputFile()
{
	QString after(".inp");
	QString before(".out");
	//int size = rbaspar->outputFile.size()-3;
	QString inpFile = rbaspar->rerankOutputFile;
	inpFile.replace(before, after, true);
	rbaspar->rerankInputFile = inpFile;
	gbInpFP = fopen(inpFile.ascii(), "wb");
	if (gbInpFP == NULL)
	{
		QMessageBox::information(this, "",  "Unable to create input file.\n");
		gbRerankInputFileFound = false;
		return false;
	}
	else
	{
		gbRerankInputFileFound = true;
	}
	if (receptorPDBFileFound && ligandPDBFileFound && receptorQUADFileFound && ligandQUADFileFound && f2dockOutputFileFound && gbRerankInputFileFound && gbRerankOutputFileFound)
	{
		//fprintf(gbInpFP, "staticMoleculePQR %s\n", rbaspar->receptorPQR.ascii());
		//fprintf(gbInpFP, "movingMoleculePQR %s\n", rbaspar->ligandPQR.ascii());
		//fprintf(gbInpFP, "staticMoleculeQUAD %s\n", rbaspar->receptorQUAD.ascii());
		//fprintf(gbInpFP, "movingMoleculeQUAD %s\n", rbaspar->ligandQUAD.ascii());
		//fprintf(gbInpFP, "F2DockOutputFile %s\n", rbaspar->f2dockOutputFile.ascii());
		//fprintf(gbInpFP, "rerankedOutputFile %s\n", rbaspar->rerankOutputFile.ascii());
		fprintf(gbInpFP, "numSol %d\n", rbaspar->numSol);
		fprintf(gbInpFP, "spectrum 1:100-1:200\n");
		fprintf(gbInpFP, "GpolWeight %lf\n", rbaspar->gpolWeight);
		fprintf(gbInpFP, "GnonpolWeight %lf\n", rbaspar->gnonpolWeight);
		fprintf(gbInpFP, "F2DockScoreWeight %lf\n", rbaspar->f2dockWeight);
		fprintf(gbInpFP, "distanceCutoff 1.5\n");
		fprintf(gbInpFP, "numThreadsBR 4\n");
		fprintf(gbInpFP, "numThreadsGpol 4\n");
		fprintf(gbInpFP, "epsilonBR %lf\n", rbaspar->epsilonBR);
		fprintf(gbInpFP, "epsilonGpol %lf\n", rbaspar->epsilonGpol);
		fprintf(gbInpFP, "useApproxMath false\n");
		fclose(gbInpFP);
		return true;
	}
	else
	{
		QMessageBox::information(this, "",  "Please provide all inputs.\n");
		return false;
	}
	return false;
}

void GBRerankForm::prepareParams()
{
	rbaspar->receptorPDB = this->receptorPQR_TB->text();
	rbaspar->ligandPDB = this->ligandPQR_TB->text();
	rbaspar->receptorQUAD = this->receptorQUAD_TB->text();
	rbaspar->ligandQUAD = this->ligandQUAD_TB->text();
	rbaspar->f2dockOutputFile = this->f2dockOutput_TB->text();
	rbaspar->rerankOutputFile = this->gbrerankOutput_TB->text();
	rbaspar->gpolWeight = this->gpol_TB->text().toDouble();
	rbaspar->gnonpolWeight = this->gnonpol_TB->text().toDouble();
	rbaspar->f2dockWeight = this->f2dockScore_TB->text().toDouble();
	rbaspar->epsilonBR = this->epsilonBR_TB->text().toDouble();
	rbaspar->epsilonGpol = this->epsilonGpol_TB->text().toDouble();
	rbaspar->numSol = this->numSol_TB->text().toInt();
}

void GBRerankForm::setParams(RerankBasicParams* rp)
{
	rbaspar = rp;
	setDefaults();
}
