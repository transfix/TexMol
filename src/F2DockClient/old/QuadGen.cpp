#include <iostream>
#include <q3filedialog.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include "QuadGen.h"

// Constructs a QuadGen which is a child of 'parent', with the name 'name' and widget flags set to 'f'
// The dialog will by default be modeless, unless you set 'modal' to true to construct a modal dialog.
QuadGen::QuadGen(QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl)
	: quadGen(parent, name, modal, fl)
{
	resultAvailable = false;
	jobSubmitted = false;
	paramsFound = false;
}

QuadGen::~QuadGen()
{
	// no need to delete child widgets, Qt does it all for us
}

// Public slot
void QuadGen::fineSlot()
{
	if (this->fineRB->isChecked())
	{
		quadpar->fine = true;
		this->coarseRB->setChecked(false);
	}
	else
	{
		quadpar->fine = false;
		this->coarseRB->setChecked(true);
	}
}

// Public slot
void QuadGen::coarseSlot()
{
	if (this->coarseRB->isChecked())
	{
		quadpar->fine = false;
		this->fineRB->setChecked(false);
	}
	else
	{
		quadpar->fine = true;
		this->fineRB->setChecked(true);
	}
}

// Public slot
void QuadGen::rfButtonSlot()
{
	quadpar->receptorFile = Q3FileDialog::getOpenFileName("", "Select Receptor file (*.pdb);;All files (*.*)");
/*Q3Err:C
	if (quadpar->receptorFile == 0)
*/
	if (quadpar->receptorFile.isNull())
	{
		QMessageBox::information(this, "Select Receptor File",  "Unable to find the file.\n");
	}
	else
	{
		this->RF_TB->setText(quadpar->receptorFile);
	}
}

// Public slot
void QuadGen::lfButtonSlot()
{
	quadpar->ligandFile = Q3FileDialog::getOpenFileName("", "Select Ligand File (*.pdb);;All files (*.*)");
/*Q3Err:C
	if (quadpar->ligandFile == 0)
*/
	if (quadpar->ligandFile.isNull())
	{
		QMessageBox::information(this, "Select Ligand File",  "Unable to find the file.\n");
	}
	else
	{
		this->LF_TB->setText(quadpar->ligandFile);
	}
}

// Public slot
void QuadGen::genButtonSlot()
{
	prepareParams();
	if (paramsFound)
	{
		if (submitJob())
		{
			jobSubmitted = true;
			resultAvailable = false;
			quadpar->submitted = true;
			quadpar->completed = false;
			this->close();
		}
	}
}

bool QuadGen::submitJob()
{
	client.setQuadGenValues(quadpar);
	if (client.submitQuadGenRequest())
	{
		QMessageBox::information(this, "Quadgen job Submitted",  "Quadgen job submitted.\n");
		return true;
	}
	else
	{
		QMessageBox::information(this, "Failed to submit Quad file generation job",  "Failed to submit Quad file generation job.\n");
		return false;
	}
}

void QuadGen::prepareParams()
{
	quadpar->receptorFile = this->RF_TB->text();
	quadpar->ligandFile = this->LF_TB->text();
	paramsFound = true;
}

void QuadGen::setParams(QuadParams* qp)
{
	quadpar = qp;
}
