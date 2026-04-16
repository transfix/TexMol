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
#ifndef RESULT_DIALOG_H
#define RESULT_DIALOG_H


#include <QDialog>
#include <QString>
#include <string>
#include <F2DockClient/resultControl.h>

using namespace std;

namespace Ui
{
  class resultUI;
}

//#include <../../src/F2DockClient/ui_resultUI.h>


class ResultDialog : public QDialog
{
  Q_OBJECT
  public:
  ResultDialog(QWidget *parent=nullptr,Qt::WindowFlags flags={});
  virtual ~ResultDialog();

  // get methods
  double * getMat(int index);
  bool isSurface(){return resCont->isSurface();}
  bool isPotential(){return resCont->isPotential();}
  int getPotentialType(){return resCont->getPotentialType();}
  bool isMultipleLigand(){return resCont->isMultipleLigand();}

  // set methods
  bool setController(ResultControl *rc);

  signals:
        void resultSelected(int);
	void vizSettingsChanged();
  
  public slots:
	//tab1
	virtual void clearSlot();
	virtual void saveSlot();
//	virtual void doneSlot();

	virtual void saveConformationSlot();

	virtual void uofbRBSlot(bool);
	virtual void surfRBSlot(bool);
	virtual void potRBSlot(bool);

	virtual void coulRBSlot(bool);
	virtual void gbRBSlot(bool);
	virtual void pbRBSlot(bool);

	virtual void resultSelectedSlot();
	


  protected:
  Ui::resultUI *_ui;

  private:
  ResultControl *resCont;
  
  int numRow, numCol;
  bool statsAvailable;

  bool rerankedMode;
  bool rmsdMode;

  void makeConnections();
  void init();
  void updateDialog();


  void setResultTableHeader();
  void setDefaultButtonStates();

  void updateResultTable();
  void updateResultRow(int i);
  void updateIndivResults(int i);
  void updateSummaryResults();
  void updateJobDetails();
};

#endif



