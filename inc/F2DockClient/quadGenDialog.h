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
#ifndef QUADGEN_DIALOG_H
#define QUADGEN_DIALOG_H



#include <F2DockClient/quadGenControl.h>
#include <QDialog>
#include <QString>
#include <string>
using namespace std;

namespace Ui
{
  class quadGenUI;
}

//#include <../../src/F2DockClient/ui_quadGenUI.h>


class QuadGenDialog : public QDialog
{
  Q_OBJECT
  public:
  QuadGenDialog(QWidget *parent=0,Qt::WFlags flags=0);
  virtual ~QuadGenDialog();

  // get methods

  // set methods
  void setController(QuadGenControl *qc){quadCont = qc;}
  
  public slots:
  virtual void receptorPDBFileNameSlot();
  virtual void ligandPDBFileNameSlot();
  virtual void submitSlot();

  virtual void saveRAWNSlot(bool);
  virtual void saveQuadSlot(bool);

  protected:
  Ui::quadGenUI *_ui;

  private:
  QuadGenControl *quadCont;
  bool rawn;
  void makeConnections();
  void init();
};

#endif
