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
#ifndef TRANSFORMATIONS_DIALOG_H
#define TRANSFORMATIONS_DIALOG_H

#include <qstringlist.h>
#include <UsefulMath/Matrix.h>
#include <QDialog>
// Qt3 base removed — stub
typedef QDialog TransformationsDialogBase;

class TransformationsDialog : public TransformationsDialogBase
{
	public:
		TransformationsDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
		~TransformationsDialog();
		CCVOpenGLMath::Matrix getTransformations();
		float getScale();

	public slots:
		virtual void addRotationSlot();
		virtual void addTranslationSlot();
		virtual void addScaleSlot();
		virtual void addGeneralRotationSlot();
		virtual void addGeneralXFormSlot();

	protected:
		CCVOpenGLMath::Matrix m_Transformation;
		float m_Scale;
};

#endif
