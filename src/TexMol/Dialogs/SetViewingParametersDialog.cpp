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
#include <TexMol/Dialogs/SetViewingParametersDialog.h>
#include <UsefulMath/Vector.h>
#include <qlineedit.h>

SetViewingParametersDialog::SetViewingParametersDialog(CCVOpenGLMath::Quaternion orientation, CCVOpenGLMath::Vector translation, float windowSize, QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl)
	: SetViewingParametersDialogBase(parent, name, modal, fl)
{
	translationXLineEdit->setText(QString::number(translation[0]));
	translationYLineEdit->setText(QString::number(translation[1]));
	translationZLineEdit->setText(QString::number(translation[2]));
	translationWLineEdit->setText(QString::number(translation[3]));
	orientationXLineEdit->setText(QString::number(orientation[0]));
	orientationYLineEdit->setText(QString::number(orientation[1]));
	orientationZLineEdit->setText(QString::number(orientation[2]));
	orientationWLineEdit->setText(QString::number(orientation[3]));
	windowSizeLineEdit->setText(QString::number(windowSize));
}

SetViewingParametersDialog::~SetViewingParametersDialog()
{
}

bool SetViewingParametersDialog::convertToDouble(QString str, double* val)
{
	bool ok = false;
	*val = str.toDouble(&ok);

	if(!ok)
	{
		return false;
	}

	return true;
}

bool SetViewingParametersDialog::getParams(double* translationParams, double* rotationParams, double* windowSize)
{
	if(!translationParams || !rotationParams || !windowSize)
	{
		return false;
	}

	if(!convertToDouble(translationXLineEdit->text(), &(translationParams[0])))
	{
		return false;
	}

	if(!convertToDouble(translationYLineEdit->text(), &(translationParams[1])))
	{
		return false;
	}

	if(!convertToDouble(translationZLineEdit->text(), &(translationParams[2])))
	{
		return false;
	}

	if(!convertToDouble(translationWLineEdit->text(), &(translationParams[3])))
	{
		return false;
	}

	if(!convertToDouble(orientationXLineEdit->text(), &(rotationParams[0])))
	{
		return false;
	}

	if(!convertToDouble(orientationYLineEdit->text(), &(rotationParams[1])))
	{
		return false;
	}

	if(!convertToDouble(orientationZLineEdit->text(), &(rotationParams[2])))
	{
		return false;
	}

	if(!convertToDouble(orientationWLineEdit->text(), &(rotationParams[3])))
	{
		return false;
	}

	if(!convertToDouble(windowSizeLineEdit->text(), windowSize))
	{
		return false;
	}

	return true;
}
