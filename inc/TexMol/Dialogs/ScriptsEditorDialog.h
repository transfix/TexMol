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
//Added by qt3to4:
#include <QKeyEvent>
#ifdef CVC_USE_QSA
#ifndef SCRIPT_EDITOR_DIALOG_H
#define SCRIPT_EDITOR_DIALOG_H

class QSEditor;
class QSProject;

#include "qdialog.h"

class ScriptEditor : public QDialog
{
	public:
		ScriptEditor(QSProject* proj, QWidget* parent = 0, const char* name = 0);
		virtual ~ScriptEditor();
		void setCode(const QString& c);
		QString code() const;

	protected:
		void keyPressEvent(QKeyEvent* e);

	private:
		QSEditor* ed;
};

#endif
#endif
