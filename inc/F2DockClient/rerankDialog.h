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
#ifndef RERANK_DIALOG_H
#define RERANK_DIALOG_H


#include <QDialog>
#include <QString>
#include <string>
#include <F2DockClient/rerankControl.h>

using namespace std;

namespace Ui
{
  class rerankUI;
}

//#include <../../src/F2DockClient/ui_rerankUI.h>


class RerankDialog : public QDialog
{
  Q_OBJECT
  public:
  RerankDialog(QWidget *parent=nullptr,Qt::WindowFlags flags={});
  virtual ~RerankDialog();

  // get methods

  // set methods
  void setController(RerankControl *rc){rerankCont = rc; updateDialog();}
  
  public slots:
	//tab1
	virtual void receptorPDBFileNameSlot();
	virtual void ligandPDBFileNameSlot();
	virtual void dockingOutputFileNameSlot();
	virtual void inputFileNameSlot();

	//tab2
	virtual void resetSlot();

	//tab3
	virtual void receptorPQRFileNameSlot();
	virtual void ligandPQRFileNameSlot();
	virtual void receptorRAWNFileNameSlot();
	virtual void ligandRAWNFileNameSlot();
	virtual void receptorQuadFileNameSlot();
	virtual void ligandQuadFileNameSlot();

	//tab4

	//common
	virtual void submitSlot();

  protected:
  Ui::rerankUI *_ui;

  private:
  RerankControl *rerankCont;

  void submit();
  bool prepareInput();
  bool prepareInputFromFile(string fileName);
  void makeConnections();
  void init();
  void updateDialog();
};

#endif
