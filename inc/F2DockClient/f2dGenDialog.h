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
#ifndef F2DGEN_DIALOG_H
#define F2DGEN_DIALOG_H


#include <QDialog>
#include <QString>
#include <string>
#include <F2DockClient/f2dGenControl.h>

using namespace std;

namespace Ui
{
  class f2dGenUI;
}

//#include <../../src/F2DockClient/ui_f2dGenUI.h>


class F2dGenDialog : public QDialog
{
  Q_OBJECT
  public:
  F2dGenDialog(QWidget *parent=0,Qt::WFlags flags=0);
  virtual ~F2dGenDialog();

  // get methods

  // set methods

  void setController(F2dGenControl *fc){f2dCont = fc;}
  
  public slots:
  virtual void receptorPDBFileNameSlot();
  virtual void ligandPDBFileNameSlot();
  virtual void submitSlot();

  virtual void saveF2dSlot(bool);
  virtual void savePQRSlot(bool);

  protected:
  Ui::f2dGenUI *_ui;

  private:
  F2dGenControl *f2dCont;
  bool pqr;
  void makeConnections();
  void init();
};

#endif
