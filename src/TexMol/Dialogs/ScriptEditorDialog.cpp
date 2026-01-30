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
#ifdef CVC_USE_QSA

#include "qlayout.h"
#include "qpushbutton.h"
#include "qseditor.h"
#include "qsproject.h"
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <QKeyEvent>
#include <Q3HBoxLayout>
#include <TexMol/ScriptEditorDialog.h>

ScriptEditor::ScriptEditor(QSProject* proj, QWidget* parent, const char* name)
	: QDialog(parent, name)
{
	QString scName = QString("script%1.qs").arg(proj->scripts().count());
	ed = proj->createEditor(proj->createScript(scName), this);
	QPushButton* ok = new QPushButton("&OK", this);
	QPushButton* cancel = new QPushButton("&Cancel", this);
	connect(ok, SIGNAL(clicked()), SLOT(accept()));
	connect(cancel, SIGNAL(clicked()), SLOT(reject()));
	Q3VBoxLayout* vbox = new Q3VBoxLayout(this, 5);
	vbox->addWidget(ed);
	Q3HBoxLayout* hbox = new Q3HBoxLayout(vbox);
	hbox->addWidget(ok);
	hbox->addStretch(-1);
	hbox->addWidget(cancel);
	resize(400, 400);
}

ScriptEditor::~ScriptEditor()
{
}

void ScriptEditor::setCode(const QString& c)
{
	ed->setText(c);
}

QString ScriptEditor::code() const
{
	return ed->text();
}

void ScriptEditor::keyPressEvent(QKeyEvent* e)
{
	if(e->key() == Qt::Key_Escape)
	{
		// let the editor handle the ESC key
		e->ignore();
		return;
	}

	QDialog::keyPressEvent(e);
}

#endif //CVC_USE_QSA
