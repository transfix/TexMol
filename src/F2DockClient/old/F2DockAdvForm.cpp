#include "F2DockClient/F2DockAdvForm.h"
#include <qcheckbox.h>
#include <qcombobox.h>
#include <q3filedialog.h>
#include <q3groupbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qradiobutton.h>

// Constructs a F2DockAdvForm which is a child of 'parent', with the name 'name' and widget flags set to 'f'
// The dialog will by default be modeless, unless you set 'modal' to true to construct a modal dialog.
F2DockAdvForm::F2DockAdvForm(QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl)
	: F2DockAdv(parent, name, modal, fl)
{
}

F2DockAdvForm::~F2DockAdvForm()
{
	// no need to delete child widgets, Qt does it all for us
}

// Public slot
void F2DockAdvForm::saveSlot()
{
	prepareParams();
	if (checkValidity())
	{
		this->close();
	}
}

// Public slot
void F2DockAdvForm::defaultSlot()
{
	resetToDefault();
}

void F2DockAdvForm::newFunction()
{
}

void F2DockAdvForm::setAdvParams(AdvancedParams* adv)
{
	advpar = adv;
}

void F2DockAdvForm::setBound(bool b)
{
	bound = b;
}

void F2DockAdvForm::prepareParams()
{
	advpar->singleLayerSkin = false;
	advpar->curvatureWeightedReceptorSkin = false;
	advpar->curvatureWeightedLigandSkin = false;
	advpar->narrowBandFFT = false;
	advpar->sparseFFT = false;
	advpar->randomRotate = false;
	advpar->rotateVolume = false;
	advpar->VDWFilter = false;
	advpar->surfaceVDW = false;
	advpar->clashFilter = false;
	advpar->miscFilter = false;
	if (this->SLS_CB->isOn())
	{
		advpar->singleLayerSkin = true;
	}
	if (this->CWR_CB->isOn())
	{
		advpar->curvatureWeightedReceptorSkin = true;
	}
	if (this->CWL_CB->isOn())
	{
		advpar->curvatureWeightedLigandSkin = true;
	}
	if (this->NBF_CB->isOn())
	{
		advpar->narrowBandFFT = true;
	}
	if (this->SF_CB->isOn())
	{
		advpar->sparseFFT = true;
	}
	if (this->RRI_CB->isOn())
	{
		advpar->randomRotate = true;
	}
	if (this->RV_CB->isOn())
	{
		advpar->rotateVolume = true;
	}
	if (this->VDW_CB->isOn())
	{
		advpar->VDWFilter = true;
	}
	if (this->SVDW_CB->isOn())
	{
		advpar->surfaceVDW = true;
	}
	if (this->CF_CB->isOn())
	{
		advpar->clashFilter = true;
	}
	if (this->MSC_CB->isOn())
	{
		advpar->miscFilter = true;
	}
	advpar->pseudoAtomRadius = PR_TB->text().toDouble();
	advpar->curvatureWeightingRadius = CWR_TB->text().toDouble();
	advpar->peaksPerRotation = NPR_TB->text().toInt();
	advpar->hBondWeight = HBW_TB->text().toDouble();
	advpar->hBondCutoff = HBC_TB->text().toDouble();
	advpar->VDWCutoff = VDWC_TB->text().toDouble();
	advpar->VDWGridSize = VDWG_TB->text().toDouble();
	advpar->clashTolerance = CT_TB->text().toDouble();
	advpar->clashWeight = CW_TB->text().toDouble();
}

bool F2DockAdvForm::checkValidity()
{
	return true;
}

void F2DockAdvForm::resetToDefault()
{
	if (bound)
	{
		this->SLS_CB->setChecked(false);
		this->PR_TB->setText("1.1");
		this->CWR_CB->setChecked(true);
		this->CWL_CB->setChecked(false);
		this->CWR_TB->setText("4.5");
		this->NBF_CB->setChecked(true);
		this->SF_CB->setChecked(true);
		this->RRI_CB->setChecked(false);
		this->RV_CB->setChecked(true);
		this->NPR_TB->setText("1");
		this->HBW_TB->setText("0");
		this->HBC_TB->setText("2.0");
		this->VDW_CB->setChecked(false);
		this->VDWC_TB->setText("100000");
		this->VDWG_TB->setText("512");
		this->SVDW_CB->setChecked(false);
		this->CF_CB->setChecked(false);
		this->CT_TB->setText("6");
		this->CW_TB->setText("0");
		this->MSC_CB->setChecked(true);
	}
	else
	{
		this->SLS_CB->setChecked(false);
		this->PR_TB->setText("1.1");
		this->CWR_CB->setChecked(true);
		this->CWL_CB->setChecked(false);
		this->CWR_TB->setText("4.5");
		this->NBF_CB->setChecked(true);
		this->SF_CB->setChecked(true);
		this->RRI_CB->setChecked(false);
		this->RV_CB->setChecked(true);
		this->NPR_TB->setText("1");
		this->HBW_TB->setText("0");
		this->HBC_TB->setText("2.0");
		this->VDW_CB->setChecked(false);
		this->VDWC_TB->setText("100000");
		this->VDWG_TB->setText("512");
		this->SVDW_CB->setChecked(false);
		this->CF_CB->setChecked(false);
		this->CT_TB->setText("6");
		this->CW_TB->setText("0");
		this->MSC_CB->setChecked(true);
	}
}
