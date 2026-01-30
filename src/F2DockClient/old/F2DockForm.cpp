#include <F2DockClient/F2DockAdvForm.h>
#include <F2DockClient/F2DockForm.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <q3filedialog.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <Utility/utility.h>

// Constructs a F2DockForm which is a child of 'parent', with the name 'name' and widget flags set to 'f'
// The dialog will by default be modeless, unless you set 'modal' to TRUE to construct a modal dialog.
F2DockForm::F2DockForm(QWidget* parent,  const char* name, bool modal, Qt::WFlags fl)
	: F2Dock(parent, name, modal, fl), my_parent(parent)
{
	bound = false;
	advParamsSet = false;
	receptorFileFound = false;
	ligandFileFound = false;
	rmsdFileFound = false;
	outputFileFound = false;
	inputFileFound = false;
	QString rots[6] = {"04", "06", "08", "10", "12", "20" };
	this->rotationCombo->insertItem("04",0);
	this->rotationCombo->insertItem("06",1);
	this->rotationCombo->insertItem("08",2);
	this->rotationCombo->insertItem("10",3);
	this->rotationCombo->insertItem("12",4);
	this->rotationCombo->insertItem("20",5);
	resultAvailable = false;
	jobSubmitted = false;
}

F2DockForm::~F2DockForm()
{
}

void F2DockForm::selectReceptorFileSlot()
{
	baspar->receptorFile = Q3FileDialog::getOpenFileName("", "Select Receptor file (*.f2d);;All files (*.*)");
/*Q3Err:C
	if (baspar->receptorFile == 0)
*/
	if (baspar->receptorFile.isNull())
	{
		QMessageBox::information(this, "Select Receptor File",  "Unable to find the file.\n");
	}
	else
	{
		this->RF_TB->setText(baspar->receptorFile);
	}
}

void F2DockForm::selectReceptorPDBSlot()
{
	baspar->receptorPDB = Q3FileDialog::getOpenFileName("", "Select Receptor PDB (*.pdb);;All files (*.*)");
/*Q3Err:C
	if (baspar->receptorPDB == 0)
*/
	if (baspar->receptorPDB.isNull())
	{
		QMessageBox::information(this, "Select Receptor PDB", "Unable to find the file.\n");
	}
	else
	{
		this->RPDB_TB->setText(baspar->receptorPDB);
	}
}

void F2DockForm::selectLigandFileSlot()
{
	baspar->ligandFile = Q3FileDialog::getOpenFileName("", "Select Ligand File (*.f2d);;All files (*.*)");
/*Q3Err:C
	if (baspar->ligandFile == 0)
*/
	if (baspar->ligandFile.isNull())
	{
		QMessageBox::information(this, "Select Ligand File",  "Unable to find the file.\n");
	}
	else
	{
		this->LF_TB->setText(baspar->ligandFile);
	}
}

void F2DockForm::selectRMSDFileSlot()
{
	baspar->rmsdFile = Q3FileDialog::getOpenFileName("", "Select RMSD file (*.rmsd);;All files (*.*)");
/*Q3Err:C
	if (baspar->rmsdFile == 0)
*/
	if (baspar->rmsdFile.isNull())
	{
		QMessageBox::information(this, "Select RMSD File",  "Unable to find the file.\n");
	}
	else
	{
		this->RMSDF_TB->setText(baspar->rmsdFile);
	}
}

void F2DockForm::selectOutputFileSlot()
{
	baspar->outputFile = Q3FileDialog::getSaveFileName("", "Select output file (*.out);;All files (*.*)");
/*Q3Err:C
	if (baspar->outputFile == 0)
*/
	if (baspar->outputFile.isNull())
	{
		QMessageBox::information(this, "Select output File",  "Unable to create the file.\n");
	}
	else
	{
		this->OF_TB->setText(baspar->outputFile);
	}
}

void F2DockForm::advencedOptionsSlot()
{
	F2DockAdvForm dialog(this);
	dialog.setAdvParams(&advpar);
	dialog.setBound(bound);
	dialog.resetToDefault();
	advParamsSet = false;
	if (dialog.exec() == QDialog::Accepted)
	{
		advParamsSet = true;
		return;
	}
	advParamsSet = true;
}

void F2DockForm::defaultSlot()
{
	setDefaults();
}

void F2DockForm::dockSlot()
{
	prepareParams();
	if (checkValidity())
	{
		if (prepareInputFile())
		{
			if (dock())
			{
				jobSubmitted = true;
				resultAvailable = false;
				baspar->submitted = true;
				baspar->completed = false;
				this->close();
			}
		}
	}
}

void F2DockForm::uuSlot()
{
	if (this->uuDockRB->isChecked())
	{
		bound = false;
		this->bbDockRB->setChecked(false);
	}
	else
	{
		bound = true;
		this->bbDockRB->setChecked(true);
	}
}

void F2DockForm::bbSlot()
{
	if (this->bbDockRB->isChecked())
	{
		bound = true;
		this->uuDockRB->setChecked(false);
	}
	else
	{
		bound = false;
		this->uuDockRB->setChecked(true);
	}
}

void F2DockForm::setDefaults()
{
	baspar->hostName = "cvcweb.ices.utexas.edu";
	baspar->port = 9091;
	if (bound)
	{
		this->gridSpaceTB->setText("1.2");
		this->numSolutionsTB->setText("2000");
		this->ssWeightTB->setText("0.7");
		this->scWeightTB->setText("-0.2");
		this->ccWeightTB->setText("17");
		this->elecWeightTB->setText("0.86");
		this->rotationCombo->setCurrentItem(1);
		baspar->gridSpacing = 1.2;
		baspar->numSol = 2000;
		baspar->ssWeight = 0.7;
		baspar->scWeight = 17;
		baspar->ccWeight = -0.2;
		baspar->elecWeight = 0.86;
		baspar->rotSeparation = 6;
		baspar->bound = false;
		advpar.singleLayerSkin = false;
		advpar.curvatureWeightedReceptorSkin = true;
		advpar.curvatureWeightedLigandSkin = false;
		advpar.narrowBandFFT = true;
		advpar.sparseFFT = true;
		advpar.randomRotate = false;
		advpar.rotateVolume = true;
		advpar.VDWFilter = false;
		advpar.surfaceVDW = false;
		advpar.clashFilter = false;
		advpar.miscFilter = true;
		advpar.pseudoAtomRadius = 1.1;
		advpar.curvatureWeightingRadius = 4.5;
		advpar.peaksPerRotation = 1;
		advpar.hBondWeight = 0;
		advpar.hBondCutoff = 2.0;
		advpar.VDWCutoff = 100000;
		advpar.VDWGridSize = 512;
		advpar.clashTolerance = 6;
		advpar.clashWeight = 0;
	}
	else
	{
		this->gridSpaceTB->setText("1.2");
		this->numSolutionsTB->setText("2000");
		this->ssWeightTB->setText("0.7");
		this->scWeightTB->setText("-0.2");
		this->ccWeightTB->setText("17");
		this->elecWeightTB->setText("0.86");
		this->rotationCombo->setCurrentItem(1);
		baspar->gridSpacing = 1.2;
		baspar->numSol = 2000;
		baspar->ssWeight = 0.7;
		baspar->scWeight = 17;
		baspar->ccWeight = -0.2;
		baspar->elecWeight = 0.86;
		baspar->rotSeparation = 6;
		baspar->bound = false;
		advpar.singleLayerSkin = false;
		advpar.curvatureWeightedReceptorSkin = true;
		advpar.curvatureWeightedLigandSkin = false;
		advpar.narrowBandFFT = true;
		advpar.sparseFFT = true;
		advpar.randomRotate = false;
		advpar.rotateVolume = true;
		advpar.VDWFilter = false;
		advpar.surfaceVDW = false;
		advpar.clashFilter = false;
		advpar.miscFilter = true;
		advpar.pseudoAtomRadius = 1.1;
		advpar.curvatureWeightingRadius = 4.5;
		advpar.peaksPerRotation = 1;
		advpar.hBondWeight = 0;
		advpar.hBondCutoff = 2.0;
		advpar.VDWCutoff = 100000;
		advpar.VDWGridSize = 512;
		advpar.clashTolerance = 6;
		advpar.clashWeight = 0;
	}
	advParamsSet = true;
}

bool F2DockForm::checkValidity()
{
	receptorFP = fopen(baspar->receptorFile.ascii(), "rb");
	if (receptorFP == NULL)
	{
		QMessageBox::information(this, "Select Receptor File",  "Unable to find the file.\n");
		receptorFileFound = false;
		return false;
	}
	else
	{
		receptorFileFound = true;
	}
	ligandFP = fopen(baspar->ligandFile.ascii(), "rb");
	if (ligandFP == NULL)
	{
		QMessageBox::information(this, "Select Ligand File",  "Unable to find the file.\n");
		ligandFileFound = false;
		return false;
	}
	else
	{
		ligandFileFound = true;
	}
	rmsdFP = fopen(baspar->rmsdFile.ascii(), "rb");
	if (rmsdFP == NULL)
	{
		QMessageBox::information(this, "Select RMSD File",  "Unable to find the file.\n");
		rmsdFileFound = false;
		return false;
	}
	else
	{
		rmsdFileFound = true;
	}
	outFP = fopen(baspar->outputFile.ascii(), "wb");
	if (outFP == NULL)
	{
		QMessageBox::information(this, "Select Output File",  "Unable to find the file.\n");
		outputFileFound = false;
		return false;
	}
	else
	{
		outputFileFound = true;
	}
	return true;
}

bool F2DockForm::dock()
{
	cout<<"submitting request"<<endl;
	client.setValues(baspar);
	if (client.submitRequest())
	{
		QMessageBox::information(this, "Docking job Submitted",  "Docking job submitted.\n");
		return true;
	}
	else
	{
		QMessageBox::information(this, "Failed to submit Docking job",  "Failed to submit Docking job.\n");
		return false;
	}
}

void F2DockForm::closeFiles()
{
	if (receptorFP)
	{
		fclose(receptorFP);
	}
	if (ligandFP)
	{
		fclose(ligandFP);
	}
	if (rmsdFP)
	{
		fclose(rmsdFP);
	}
	if (outFP)
	{
		fclose(outFP);
	}
	if (inpFP)
	{
		fclose(inpFP);
	}
}

bool F2DockForm::prepareInputFile()
{
	QString after(".inp");
	QString before(".out");
	QString inpFile = baspar->outputFile;
	inpFile.replace(before, after, true);
	baspar->inputFile = inpFile;
	inpFP = fopen(inpFile.ascii(), "wb");
	if (inpFP == NULL)
	{
		QMessageBox::information(this, "",  "Unable to create input file.\n");
		inputFileFound = false;
		return false;
	}
	else
	{
		inputFileFound = true;
	}
	if (receptorFileFound && ligandFileFound && rmsdFileFound && outputFileFound && inputFileFound && advParamsSet)
	{
		if (bound)
		{
			fprintf(inpFP, "gridSpacing %lf\n", baspar->gridSpacing);
			fprintf(inpFP, "singleLayerLigandSkin %s\n", advpar.singleLayerSkin?"true":"false");
			fprintf(inpFP, "pseudoAtomRadius %lf\n", advpar.pseudoAtomRadius);
			fprintf(inpFP, "effGridFile fftw-rank.txt\n");
			fprintf(inpFP, "numSolutions %d\n", baspar->numSol);
			fprintf(inpFP, "useSparseFFT %s\n", advpar.sparseFFT?"true":"false");
			fprintf(inpFP, "narrowBand %s\n", advpar.narrowBandFFT?"true":"false");
			fprintf(inpFP, "skinSkinWeight %lf\n", baspar->ssWeight);
			fprintf(inpFP, "coreCoreWeight %lf\n", baspar->ccWeight);
			fprintf(inpFP, "skinCoreWeight %lf\n", baspar->scWeight);
			fprintf(inpFP, "elecWeight %lf\n", baspar->elecWeight);
			fprintf(inpFP, "hbondWeight %lf\n", advpar.hBondWeight);
			fprintf(inpFP, "hbondDistanceCutoff %lf\n", advpar.hBondCutoff);
			fprintf(inpFP, "curvatureWeightedStaticMol %s\n", advpar.curvatureWeightedReceptorSkin?"true":"false");
			fprintf(inpFP, "curvatureWeightedMovingMol %s\n", advpar.curvatureWeightedLigandSkin?"true":"false");
			fprintf(inpFP, "curvatureWeightingRadius %lf\n", advpar.curvatureWeightingRadius);
			fprintf(inpFP, "spreadReceptorSkin %s\n", "false");
			fprintf(inpFP, "randomRotate %s\n", advpar.randomRotate?"true":"false");
			fprintf(inpFP, "dockVolume %s\n", "false");
			fprintf(inpFP, "rotateVolume %s\n", advpar.rotateVolume?"true":"false");
			fprintf(inpFP, "smoothSkin %s\n", "false");
			fprintf(inpFP, "rotFile deg%02d.matrix\n", baspar->rotSeparation);
			fprintf(inpFP, "scoreScaleUpFactor %d\n", 10000);
			fprintf(inpFP, "vdWGridSize %d\n", advpar.VDWGridSize);
			fprintf(inpFP, "surfaceBasedVdW %s\n", advpar.surfaceVDW?"true":"false");
			fprintf(inpFP, "applyVdWFilter %s\n", advpar.VDWFilter?"true":"false");
			fprintf(inpFP, "vdWCutoff %lf\n", advpar.VDWCutoff);
			fprintf(inpFP, "vdWWellWidth %lf\n", 0.0);
			fprintf(inpFP, "applyClashFilter %s\n", advpar.clashFilter?"true":"false");
			fprintf(inpFP, "eqmDistFrac %lf\n", 0.5);
			fprintf(inpFP, "clashTolerance %lf\n", advpar.clashTolerance);
			fprintf(inpFP, "clashWeight %lf\n", advpar.clashWeight);
			fprintf(inpFP, "applyMiscFilter %s\n", advpar.miscFilter?"true":"false");
			fprintf(inpFP, "compQuadVdW %s\n", "false");
			fprintf(inpFP, "clusterTransRad %lf\n", 1.0);
			fprintf(inpFP, "peaksPerRotation %d\n", advpar.peaksPerRotation);
			fprintf(inpFP, "filterDepth %d\n", 20);
			fprintf(inpFP, "elecRadiusInGrids %lf\n", 2.9);
			fprintf(inpFP, "numThreads %d\n", 4);
			fprintf(inpFP, "breakDownScores false\n");
		}
		else
		{
			fprintf(inpFP, "gridSpacing %lf\n", baspar->gridSpacing);
			fprintf(inpFP, "singleLayerLigandSkin %s\n", advpar.singleLayerSkin?"true":"false");
			fprintf(inpFP, "pseudoAtomRadius %lf\n", advpar.pseudoAtomRadius);
			fprintf(inpFP, "effGridFile fftw-rank.txt\n");
			fprintf(inpFP, "numSolutions %d\n", baspar->numSol);
			fprintf(inpFP, "useSparseFFT %s\n", advpar.sparseFFT?"true":"false");
			fprintf(inpFP, "narrowBand %s\n", advpar.narrowBandFFT?"true":"false");
			fprintf(inpFP, "skinSkinWeight %lf\n", baspar->ssWeight);
			fprintf(inpFP, "coreCoreWeight %lf\n", baspar->ccWeight);
			fprintf(inpFP, "skinCoreWeight %lf\n", baspar->scWeight);
			fprintf(inpFP, "elecWeight %lf\n", baspar->elecWeight);
			fprintf(inpFP, "hbondWeight %lf\n", advpar.hBondWeight);
			fprintf(inpFP, "hbondDistanceCutoff %lf\n", advpar.hBondCutoff);
			fprintf(inpFP, "curvatureWeightedStaticMol %s\n", advpar.curvatureWeightedReceptorSkin?"true":"false");
			fprintf(inpFP, "curvatureWeightedMovingMol %s\n", advpar.curvatureWeightedLigandSkin?"true":"false");
			fprintf(inpFP, "curvatureWeightingRadius %lf\n", advpar.curvatureWeightingRadius);
			fprintf(inpFP, "spreadReceptorSkin %s\n", "false");
			fprintf(inpFP, "randomRotate %s\n", advpar.randomRotate?"true":"false");
			fprintf(inpFP, "dockVolume %s\n", "false");
			fprintf(inpFP, "rotateVolume %s\n", advpar.rotateVolume?"true":"false");
			fprintf(inpFP, "smoothSkin %s\n", "false");
			fprintf(inpFP, "rotFile deg%02d.matrix\n", baspar->rotSeparation);
			fprintf(inpFP, "scoreScaleUpFactor %d\n", 10000);
			fprintf(inpFP, "vdWGridSize %d\n", advpar.VDWGridSize);
			fprintf(inpFP, "surfaceBasedVdW %s\n", advpar.surfaceVDW?"true":"false");
			fprintf(inpFP, "applyVdWFilter %s\n", advpar.VDWFilter?"true":"false");
			fprintf(inpFP, "vdWCutoff %lf\n", advpar.VDWCutoff);
			fprintf(inpFP, "vdWWellWidth %lf\n", 0.0);
			fprintf(inpFP, "applyClashFilter %s\n", advpar.clashFilter?"true":"false");
			fprintf(inpFP, "eqmDistFrac %lf\n", 0.5);
			fprintf(inpFP, "clashTolerance %lf\n", advpar.clashTolerance);
			fprintf(inpFP, "clashWeight %lf\n", advpar.clashWeight);
			fprintf(inpFP, "applyMiscFilter %s\n", advpar.miscFilter?"true":"false");
			fprintf(inpFP, "compQuadVdW %s\n", "false");
			fprintf(inpFP, "clusterTransRad %lf\n", 1.0);
			fprintf(inpFP, "peaksPerRotation %d\n", advpar.peaksPerRotation);
			fprintf(inpFP, "filterDepth %d\n", 20);
			fprintf(inpFP, "elecRadiusInGrids %lf\n", 2.9);
			fprintf(inpFP, "numThreads %d\n", 4);
			fprintf(inpFP, "breakDownScores false\n");
		}
		fclose(inpFP);
		return true;
	}
	else
	{
		QMessageBox::information(this, "",  "Please provide all inputs.\n");
		return false;
	}
	return false;
}

void F2DockForm::prepareParams()
{
	baspar->receptorFile = this->RF_TB->text();
	baspar->ligandFile = this->LF_TB->text();
	baspar->rmsdFile = this->RMSDF_TB->text();
	baspar->outputFile = this->OF_TB->text();
	baspar->ssWeight = this->ssWeightTB->text().toDouble();
	baspar->scWeight = this->scWeightTB->text().toDouble();
	baspar->ccWeight = this->ccWeightTB->text().toDouble();
	baspar->elecWeight = this->elecWeightTB->text().toDouble();
	baspar->numSol = this->numSolutionsTB->text().toInt();
	baspar->rotSeparation = this->rotationCombo->text(this->rotationCombo->currentItem()).toInt();
	baspar->gridSpacing = this->gridSpaceTB->text().toDouble();
	baspar->bound = bound;
}

void F2DockForm::setParams(BasicParams* bp)
{
	baspar = bp;
	setDefaults();
}
