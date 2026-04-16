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
#ifndef DOCK_DIALOG_H
#define DOCK_DIALOG_H


#include <QDialog>
#include <QString>
#include <string>
#include <F2DockClient/dockControl.h>

using namespace std;

namespace Ui
{
  class dockUI;
}

//#include <../../src/F2DockClient/ui_dockUI.h>


class DockDialog : public QDialog
{
  Q_OBJECT
  public:
  DockDialog(QWidget *parent=nullptr,Qt::WindowFlags flags={});
  virtual ~DockDialog();

  // get methods

  // set methods
  void setController(DockControl *rc){dockCont = rc; updateDialog();}
  
  public slots:
	//tab1
	virtual void receptorPDBFileNameSlot();
	virtual void ligandPDBFileNameSlot();
	virtual void rmsdFileNameSlot();
	virtual void complexTypeSlot(int);
	virtual void unboundSlot();
	virtual void rerankSlot();

	//tab2
	virtual void resetSlot();		
	virtual void inputFileNameSlot();

	//tab3
	virtual void receptorPQRFileNameSlot();
	virtual void ligandPQRFileNameSlot();
	virtual void receptorF2dFileNameSlot();
	virtual void ligandF2dFileNameSlot();
	virtual void receptorRAWNFileNameSlot();
	virtual void ligandRAWNFileNameSlot();
	virtual void receptorQuadFileNameSlot();
	virtual void ligandQuadFileNameSlot();	
	//tab4

	//common
	virtual void submitSlot();

  protected:
  Ui::dockUI *_ui;

  private:
  DockControl *dockCont;

  void submit();
  bool prepareInput();
  bool prepareInputFromFile(string fileName);
  void makeConnections();
  void init();
  void updateDialog();
};

#endif
