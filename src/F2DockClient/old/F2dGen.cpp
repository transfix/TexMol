#include "F2DockClient/F2dGen.h"
#include <iostream>
#include <q3filedialog.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpushbutton.h>

// Constructs a F2dGen which is a child of 'parent', with the name 'name' and widget flags set to 'f'
// The dialog will by default be modeless, unless you set 'modal' to true to construct a modal dialog.
F2dGen::F2dGen(QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl)
	: f2dGen(parent, name, modal, fl)
{
	resultAvailable = false;
	jobSubmitted = false;
	paramsFound = false;
}

F2dGen::~F2dGen()
{
}

void F2dGen::rfButtonSlot()
{
	f2dpar->receptorFile = Q3FileDialog::getOpenFileName("", "Select Receptor file (*.pdb);;All files (*.*)");
/*Q3Err:C
	if (f2dpar->receptorFile == 0)
*/
	if (f2dpar->receptorFile.isNull())
	{
		QMessageBox::information(this, "Select Receptor File",  "Unable to find the file.\n");
	}
	else
	{
		this->RF_TB->setText(f2dpar->receptorFile);
	}
}

void F2dGen::lfButtonSlot()
{
	f2dpar->ligandFile = Q3FileDialog::getOpenFileName("", "Select Ligand File (*.pdb);;All files (*.*)");
/*Q3Err:C
	if (f2dpar->ligandFile == 0)
*/
	if (f2dpar->ligandFile.isNull())
	{
		QMessageBox::information(this, "Select Ligand File",  "Unable to find the file.\n");
	}
	else
	{
		this->LF_TB->setText(f2dpar->ligandFile);
	}
}

void F2dGen::genButtonSlot()
{
	prepareParams();
	if (paramsFound)
	{
		if (submitJob())
		{
			jobSubmitted = true;
			resultAvailable = false;
			f2dpar->submitted  = true;
			f2dpar->completed  = false;
			this->close();
		}
	}
}

bool F2dGen::submitJob()
{
	client.setF2dGenValues(f2dpar);
	if (client.submitF2dGenRequest())
	{
		QMessageBox::information(this, "f2d Generation job Submitted",  "f2d Generation job submitted.\n");
		return true;
	}
	else
	{
		QMessageBox::information(this, "Failed to submit f2d Generation job",  "Failed to submit f2d Generation job.\n");
		return false;
	}
}

void F2dGen::prepareParams()
{
	f2dpar->receptorFile = this->RF_TB->text();
	f2dpar->ligandFile = this->LF_TB->text();
	paramsFound = true;
}

void F2dGen::setParams(F2dParams* fp)
{
	f2dpar = fp;
}
