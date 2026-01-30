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
#include <stdlib.h>
#include <math.h>
#include <qvalidator.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qcolor.h>
#include <qcolordialog.h>
#include <TexMol/Dialogs/LightsDialog.h>
#include <TexMol/LightSet.h>
#ifdef _WIN32
#include <windows.h>
#endif
#if defined(__APPLE__)
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif

LightsDialog::LightsDialog(int light, LightSet* lightSet, QWidget* parent,  const char* name, bool modal, Qt::WFlags fl)
	: LightsDialogBase(parent, name, modal, fl)
{
	m_XPositionLineEdit->setValidator(new QDoubleValidator(this));
	m_YPositionLineEdit->setValidator(new QDoubleValidator(this));
	m_ZPositionLineEdit->setValidator(new QDoubleValidator(this));
	m_WPositionLineEdit->setValidator(new QDoubleValidator(this));
	m_RedAmbientLineEdit->setValidator(new QDoubleValidator(this));
	m_GreenAmbientLineEdit->setValidator(new QDoubleValidator(this));
	m_BlueAmbientLineEdit->setValidator(new QDoubleValidator(this));
	m_AlphaAmbientLineEdit->setValidator(new QDoubleValidator(this));
	m_RedDiffuseLineEdit->setValidator(new QDoubleValidator(this));
	m_GreenDiffuseLineEdit->setValidator(new QDoubleValidator(this));
	m_BlueDiffuseLineEdit->setValidator(new QDoubleValidator(this));
	m_AlphaDiffuseLineEdit->setValidator(new QDoubleValidator(this));
	m_RedSpecularLineEdit->setValidator(new QDoubleValidator(this));
	m_GreenSpecularLineEdit->setValidator(new QDoubleValidator(this));
	m_BlueSpecularLineEdit->setValidator(new QDoubleValidator(this));
	m_AlphaSpecularLineEdit->setValidator(new QDoubleValidator(this));
	m_XSpotLightDirectionLineEdit->setValidator(new QDoubleValidator(this));
	m_YSpotLightDirectionLineEdit->setValidator(new QDoubleValidator(this));
	m_ZSpotLightDirectionLineEdit->setValidator(new QDoubleValidator(this));
	m_SpotLightExponentLineEdit->setValidator(new QDoubleValidator(this));
	m_SpotLightCutOffLineEdit->setValidator(new QDoubleValidator(this));
	m_ConstantAttenuationLineEdit->setValidator(new QDoubleValidator(this));
	m_LinearAttenuationLineEdit->setValidator(new QDoubleValidator(this));
	m_QuadraticAttenuationLineEdit->setValidator(new QDoubleValidator(this));
	// fill up the values from OpenGL's state.
	//MSDN: It is always the case that GL_LIGHTi = GL_LIGHT0 + i.
	int openGLLightNumber = GL_LIGHT0 + light;
	Light curlight = lightSet->m_Lights[light];

	// enabled ?
	if(curlight.enabled)
	{
		m_EnableCheckBox->setChecked(true);
	}
	else
	{
		m_EnableCheckBox->setChecked(false);
	}

	// position
	float* position = curlight.position;
	m_XPositionLineEdit->setText(QString::number(position[0]));
	m_YPositionLineEdit->setText(QString::number(position[1]));
	m_ZPositionLineEdit->setText(QString::number(position[2]));
	m_WPositionLineEdit->setText(QString::number(position[3]));
	// colors
	// 1. Ambient
	float* ambient = curlight.ambient;
	m_RedAmbientLineEdit->setText(QString::number(ambient[0]));
	m_GreenAmbientLineEdit->setText(QString::number(ambient[1]));
	m_BlueAmbientLineEdit->setText(QString::number(ambient[2]));
	m_AlphaAmbientLineEdit->setText(QString::number(ambient[3]));
	// 2. Diffuse
	float* diffuse = curlight.diffuse;
	m_RedDiffuseLineEdit->setText(QString::number(diffuse[0]));
	m_GreenDiffuseLineEdit->setText(QString::number(diffuse[1]));
	m_BlueDiffuseLineEdit->setText(QString::number(diffuse[2]));
	m_AlphaDiffuseLineEdit->setText(QString::number(diffuse[3]));
	// 3. Specular
	float* specular = curlight.specular;
	m_RedSpecularLineEdit->setText(QString::number(specular[0]));
	m_GreenSpecularLineEdit->setText(QString::number(specular[1]));
	m_BlueSpecularLineEdit->setText(QString::number(specular[2]));
	m_AlphaSpecularLineEdit->setText(QString::number(specular[3]));
	// spotlight params
	float* spotDirection = curlight.spotDirection;
	m_XSpotLightDirectionLineEdit->setText(QString::number(spotDirection[0]));
	m_YSpotLightDirectionLineEdit->setText(QString::number(spotDirection[1]));
	m_ZSpotLightDirectionLineEdit->setText(QString::number(spotDirection[2]));
	float spotExponent = curlight.spotExponent;
	m_SpotLightExponentLineEdit->setText(QString::number(spotExponent));
	float spotCutoff = curlight.spotCutoff;
	m_SpotLightCutOffLineEdit->setText(QString::number(spotCutoff));
	// attenuation
	// 1. constant
	float constantAttenuation = curlight.constantAttenuation;
	m_ConstantAttenuationLineEdit->setText(QString::number(constantAttenuation));
	// 2. linear
	float linearAttenuation = curlight.linearAttenuation;
	m_LinearAttenuationLineEdit->setText(QString::number(linearAttenuation));
	// 3. quadratic
	float quadraticAttenuation = curlight.quadraticAttenuation;
	m_QuadraticAttenuationLineEdit->setText(QString::number(quadraticAttenuation));
}

LightsDialog::~LightsDialog()
{
}

bool LightsDialog::saveLightInformation(int light, LightSet* lightSet)
{
	int openGLLightNumber = GL_LIGHT0 + light;
	Light* curlight = &(lightSet->m_Lights[light]);
	// enabled ?
	curlight->enabled = m_EnableCheckBox->isChecked();

	if(curlight->enabled)
	{
		glEnable(openGLLightNumber);
	}
	else
	{
		glDisable(openGLLightNumber);
	}

	// position
	curlight->position[0] = m_XPositionLineEdit->text().toFloat();
	curlight->position[1] = m_YPositionLineEdit->text().toFloat();
	curlight->position[2] = m_ZPositionLineEdit->text().toFloat();
	curlight->position[3] = m_WPositionLineEdit->text().toFloat();
	glLightfv(openGLLightNumber, GL_POSITION, curlight->position);
	// colors
	// 1. Ambient
	curlight->ambient[0] = m_RedAmbientLineEdit->text().toFloat();
	curlight->ambient[1] = m_GreenAmbientLineEdit->text().toFloat();
	curlight->ambient[2] = m_BlueAmbientLineEdit->text().toFloat();
	curlight->ambient[3] = m_AlphaAmbientLineEdit->text().toFloat();
	glLightfv(openGLLightNumber, GL_AMBIENT, curlight->ambient);
	// 2. Diffuse
	curlight->diffuse[0] = m_RedDiffuseLineEdit->text().toFloat();
	curlight->diffuse[1] = m_GreenDiffuseLineEdit->text().toFloat();
	curlight->diffuse[2] = m_BlueDiffuseLineEdit->text().toFloat();
	curlight->diffuse[3] = m_AlphaDiffuseLineEdit->text().toFloat();
	glLightfv(openGLLightNumber, GL_DIFFUSE, curlight->diffuse);
	// 3. Specular
	curlight->specular[0] = m_RedSpecularLineEdit->text().toFloat();
	curlight->specular[1] = m_GreenSpecularLineEdit->text().toFloat();
	curlight->specular[2] = m_BlueSpecularLineEdit->text().toFloat();
	curlight->specular[3] = m_AlphaSpecularLineEdit->text().toFloat();
	glLightfv(openGLLightNumber, GL_SPECULAR, curlight->specular);
	// spotlight params
	curlight->spotDirection[0] = m_XSpotLightDirectionLineEdit->text().toFloat();
	curlight->spotDirection[1] = m_YSpotLightDirectionLineEdit->text().toFloat();
	curlight->spotDirection[2] = m_ZSpotLightDirectionLineEdit->text().toFloat();
	glLightfv(openGLLightNumber, GL_SPOT_DIRECTION, curlight->spotDirection);
	curlight->spotExponent = m_SpotLightExponentLineEdit->text().toFloat();
	glLightfv(openGLLightNumber, GL_SPOT_EXPONENT, &(curlight->spotExponent));
	curlight->spotCutoff = m_SpotLightCutOffLineEdit->text().toFloat();
	glLightfv(openGLLightNumber, GL_SPOT_CUTOFF, &(curlight->spotCutoff));
	// attenuation
	// 1. constant
	curlight->constantAttenuation = m_ConstantAttenuationLineEdit->text().toFloat();
	glLightfv(openGLLightNumber, GL_CONSTANT_ATTENUATION, &(curlight->constantAttenuation));
	// 2. linear
	curlight->linearAttenuation = m_LinearAttenuationLineEdit->text().toFloat();
	glLightfv(openGLLightNumber, GL_LINEAR_ATTENUATION, &(curlight->linearAttenuation));
	// 3. quadratic
	curlight->quadraticAttenuation = m_QuadraticAttenuationLineEdit->text().toFloat();
	glLightfv(openGLLightNumber, GL_QUADRATIC_ATTENUATION, &(curlight->quadraticAttenuation));
	return true;
}

void LightsDialog::ambientColorSlot()
{
	QColor prevColor;
	prevColor.setRgb(
		((int)(m_RedAmbientLineEdit->text()).toFloat()*256),
		((int)(m_GreenAmbientLineEdit->text()).toFloat()*256),
		((int)(m_BlueAmbientLineEdit->text()).toFloat()*256));
	QColor newColor = QColorDialog::getColor(prevColor, this);

	if(newColor.isValid())
	{
		m_RedAmbientLineEdit->setText(QString::number(newColor.red()/256.0));
		m_GreenAmbientLineEdit->setText(QString::number(newColor.green()/256.0));
		m_BlueAmbientLineEdit->setText(QString::number(newColor.blue()/256.0));
	}
}

void LightsDialog::diffuseColorSlot()
{
	QColor prevColor;
	prevColor.setRgb(
		((int)(m_RedDiffuseLineEdit->text()).toFloat()*256),
		((int)(m_GreenDiffuseLineEdit->text()).toFloat()*256),
		((int)(m_BlueDiffuseLineEdit->text()).toFloat()*256));
	QColor newColor = QColorDialog::getColor(prevColor, this);

	if(newColor.isValid())
	{
		m_RedDiffuseLineEdit->setText(QString::number(newColor.red()/256.0));
		m_GreenDiffuseLineEdit->setText(QString::number(newColor.green()/256.0));
		m_BlueDiffuseLineEdit->setText(QString::number(newColor.blue()/256.0));
	}
}

void LightsDialog::specularColorSlot()
{
	QColor prevColor;
	prevColor.setRgb(
		((int)(m_RedSpecularLineEdit->text()).toFloat()*256),
		((int)(m_GreenSpecularLineEdit->text()).toFloat()*256),
		((int)(m_BlueSpecularLineEdit->text()).toFloat()*256));
	QColor newColor = QColorDialog::getColor(prevColor, this);

	if(newColor.isValid())
	{
		m_RedSpecularLineEdit->setText(QString::number(newColor.red()/256.0));
		m_GreenSpecularLineEdit->setText(QString::number(newColor.green()/256.0));
		m_BlueSpecularLineEdit->setText(QString::number(newColor.blue()/256.0));
	}
}
