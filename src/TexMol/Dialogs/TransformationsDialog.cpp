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
// ERROR: SKVINAY I think  it should be pre multiply everywhere
#include <TexMol/Dialogs/TransformationsDialog.h>
#include <qvalidator.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qmessagebox.h>
#include <q3listbox.h>
#include <qpushbutton.h>

TransformationsDialog::TransformationsDialog(QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl)
	: TransformationsDialogBase(parent, name, modal, fl)
{
	m_CurrentTransformationsListBox->clear();
	m_AngleLineEdit->setValidator(new QDoubleValidator(this));
	m_XTranslationLineEdit->setValidator(new QDoubleValidator(this));
	m_YTranslationLineEdit->setValidator(new QDoubleValidator(this));
	m_ZTranslationLineEdit->setValidator(new QDoubleValidator(this));
	m_ScaleLineEdit->setValidator(new QDoubleValidator(this));
	m_RotationAxisComboBox->insertItem("x");
	m_RotationAxisComboBox->insertItem("y");
	m_RotationAxisComboBox->insertItem("z");
	m_R00LineEdit->setValidator(new QDoubleValidator(this));
	m_R01LineEdit->setValidator(new QDoubleValidator(this));
	m_R02LineEdit->setValidator(new QDoubleValidator(this));
	m_R10LineEdit->setValidator(new QDoubleValidator(this));
	m_R11LineEdit->setValidator(new QDoubleValidator(this));
	m_R12LineEdit->setValidator(new QDoubleValidator(this));
	m_R20LineEdit->setValidator(new QDoubleValidator(this));
	m_R21LineEdit->setValidator(new QDoubleValidator(this));
	m_R22LineEdit->setValidator(new QDoubleValidator(this));
	m_X00LineEdit->setValidator(new QDoubleValidator(this));
	m_X01LineEdit->setValidator(new QDoubleValidator(this));
	m_X02LineEdit->setValidator(new QDoubleValidator(this));
	m_X03LineEdit->setValidator(new QDoubleValidator(this));
	m_X10LineEdit->setValidator(new QDoubleValidator(this));
	m_X11LineEdit->setValidator(new QDoubleValidator(this));
	m_X12LineEdit->setValidator(new QDoubleValidator(this));
	m_X13LineEdit->setValidator(new QDoubleValidator(this));
	m_X20LineEdit->setValidator(new QDoubleValidator(this));
	m_X21LineEdit->setValidator(new QDoubleValidator(this));
	m_X22LineEdit->setValidator(new QDoubleValidator(this));
	m_X23LineEdit->setValidator(new QDoubleValidator(this));
	m_X30LineEdit->setValidator(new QDoubleValidator(this));
	m_X31LineEdit->setValidator(new QDoubleValidator(this));
	m_X32LineEdit->setValidator(new QDoubleValidator(this));
	m_X33LineEdit->setValidator(new QDoubleValidator(this));
	m_Transformation.reset();
	m_Scale = 1.0;
}

TransformationsDialog::~TransformationsDialog()
{
}

// If the rotation is valid, add it to the conbo box
void TransformationsDialog::addRotationSlot()
{
	if(m_AngleLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Incomplete information", "Enter a value to rotate with in degrees");
		return;
	}

	// here we assume that there is a number in the line edit since we used validators.
	// seems overkill to recheck.
	m_CurrentTransformationsListBox->insertItem("R " +
			m_RotationAxisComboBox->currentText() +
			" " +
			m_AngleLineEdit->text());
	float angle = m_AngleLineEdit->text().toFloat();

	if(m_RotationAxisComboBox->currentText() == "x")
	{
		m_Transformation.postMultiplication(CCVOpenGLMath::Matrix::rotationX(angle));
	}

	if(m_RotationAxisComboBox->currentText() == "y")
	{
		m_Transformation.postMultiplication(CCVOpenGLMath::Matrix::rotationY(angle));
	}

	if(m_RotationAxisComboBox->currentText() == "z")
	{
		m_Transformation.postMultiplication(CCVOpenGLMath::Matrix::rotationZ(angle));
	}
}

// Remove the selected transformation, if any.
// If the translation is valid, add it to the conbo box
void TransformationsDialog::addTranslationSlot()
{
	if(m_XTranslationLineEdit->text().isEmpty() ||
			m_YTranslationLineEdit->text().isEmpty() ||
			m_ZTranslationLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Incomplete information", "Enter a complete translation set");
		return;
	}

	// here we assume that there is a number in the line edit since we used validators.
	// seems overkill to recheck.
	m_CurrentTransformationsListBox->insertItem("T " +
			m_XTranslationLineEdit->text() +
			" " +
			m_YTranslationLineEdit->text() +
			" " +
			m_ZTranslationLineEdit->text());
	float x = m_XTranslationLineEdit->text().toFloat();
	float y = m_YTranslationLineEdit->text().toFloat();
	float z = m_ZTranslationLineEdit->text().toFloat();
	m_Transformation.postMultiplication(CCVOpenGLMath::Matrix::translation(x, y, z));
}

// If the scale is valid, add it to the conbo box
void TransformationsDialog::addScaleSlot()
{
	if(m_ScaleLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Incomplete information", "Enter a scale value");
		return;
	}

	// here we assume that there is a number in the line edit since we used validators.
	// seems overkill to recheck.
	m_CurrentTransformationsListBox->insertItem("S " +
			m_ScaleLineEdit->text());
	float scale = m_ScaleLineEdit->text().toFloat();
	m_Transformation.postMultiplication(CCVOpenGLMath::Matrix::scale(scale, scale, scale));
	m_Scale *= scale;
}

void TransformationsDialog::addGeneralRotationSlot()
{
	if(m_R00LineEdit->text().isEmpty() ||
			m_R01LineEdit->text().isEmpty() ||
			m_R02LineEdit->text().isEmpty() ||
			m_R10LineEdit->text().isEmpty() ||
			m_R11LineEdit->text().isEmpty() ||
			m_R12LineEdit->text().isEmpty() ||
			m_R20LineEdit->text().isEmpty() ||
			m_R21LineEdit->text().isEmpty() ||
			m_R22LineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Incomplete information", "Enter a scale value");
		return;
	}

	m_CurrentTransformationsListBox->insertItem("G " +
			m_R00LineEdit->text() + " " +
			m_R01LineEdit->text() + " " +
			m_R02LineEdit->text() + " " +
			m_R10LineEdit->text() + " " +
			m_R11LineEdit->text() + " " +
			m_R12LineEdit->text() + " " +
			m_R20LineEdit->text() + " " +
			m_R21LineEdit->text() + " " +
			m_R22LineEdit->text());
	float r00 = m_R00LineEdit->text().toFloat();
	float r01 = m_R01LineEdit->text().toFloat();
	float r02 = m_R02LineEdit->text().toFloat();
	float r10 = m_R10LineEdit->text().toFloat();
	float r11 = m_R11LineEdit->text().toFloat();
	float r12 = m_R12LineEdit->text().toFloat();
	float r20 = m_R20LineEdit->text().toFloat();
	float r21 = m_R21LineEdit->text().toFloat();
	float r22 = m_R22LineEdit->text().toFloat();
	m_Transformation.postMultiplication(CCVOpenGLMath::Matrix(r00, r01, r02, 0, r10, r11, r12, 0, r20, r21, r22, 0, 0,   0,   0,   1));
}

CCVOpenGLMath::Matrix TransformationsDialog::getTransformations()
{
	return m_Transformation;
}

float TransformationsDialog::getScale()
{
	return m_Scale;
}

void TransformationsDialog::addGeneralXFormSlot()
{
	if(m_X00LineEdit->text().isEmpty() ||
			m_X01LineEdit->text().isEmpty() ||
			m_X02LineEdit->text().isEmpty() ||
			m_X03LineEdit->text().isEmpty() ||
			m_X10LineEdit->text().isEmpty() ||
			m_X11LineEdit->text().isEmpty() ||
			m_X12LineEdit->text().isEmpty() ||
			m_X13LineEdit->text().isEmpty() ||
			m_X20LineEdit->text().isEmpty() ||
			m_X21LineEdit->text().isEmpty() ||
			m_X22LineEdit->text().isEmpty() ||
			m_X23LineEdit->text().isEmpty() ||
			m_X30LineEdit->text().isEmpty() ||
			m_X31LineEdit->text().isEmpty() ||
			m_X32LineEdit->text().isEmpty() ||
			m_X33LineEdit->text().isEmpty())
	{
		QMessageBox::warning(this, "Incomplete information", "Enter a scale value");
		return;
	}

	m_CurrentTransformationsListBox->insertItem("X " +
			m_X00LineEdit->text() + " " +
			m_X01LineEdit->text() + " " +
			m_X02LineEdit->text() + " " +
			m_X03LineEdit->text() + " " +
			m_X10LineEdit->text() + " " +
			m_X11LineEdit->text() + " " +
			m_X12LineEdit->text() + " " +
			m_X13LineEdit->text() + " " +
			m_X20LineEdit->text() + " " +
			m_X21LineEdit->text() + " " +
			m_X22LineEdit->text() + " " +
			m_X23LineEdit->text() + " " +
			m_X30LineEdit->text() + " " +
			m_X31LineEdit->text() + " " +
			m_X32LineEdit->text() + " " +
			m_X33LineEdit->text());
	float t00 = m_X00LineEdit->text().toFloat();
	float t01 = m_X01LineEdit->text().toFloat();
	float t02 = m_X02LineEdit->text().toFloat();
	float t03 = m_X03LineEdit->text().toFloat();
	float t10 = m_X10LineEdit->text().toFloat();
	float t11 = m_X11LineEdit->text().toFloat();
	float t12 = m_X12LineEdit->text().toFloat();
	float t13 = m_X13LineEdit->text().toFloat();
	float t20 = m_X20LineEdit->text().toFloat();
	float t21 = m_X21LineEdit->text().toFloat();
	float t22 = m_X22LineEdit->text().toFloat();
	float t23 = m_X23LineEdit->text().toFloat();
	float t30 = m_X30LineEdit->text().toFloat();
	float t31 = m_X31LineEdit->text().toFloat();
	float t32 = m_X32LineEdit->text().toFloat();
	float t33 = m_X33LineEdit->text().toFloat();
	m_Transformation.postMultiplication(CCVOpenGLMath::Matrix(t00, t01, t02, t03, t10, t11, t12, t13, t20, t21, t22, t23, t30, t31, t32, t33));
}
