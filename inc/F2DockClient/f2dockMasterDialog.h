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
#ifndef F2DOCK_MASTER_DIALOG_H
#define F2DOCK_MASTER_DIALOG_H


#include <QDialog>
#include <QString>
#include <string>

using namespace std;

namespace Ui
{
  class f2dockMasterUI;
}

//#include <../../src/F2DockClient/ui_f2dockMasterUI.h>
#include <F2DockClient/f2dockMasterControl.h>
#include <F2DockClient/nagger.h>
#include <F2DockClient/resultDialog.h>
#include <F2DockClient/resultControl.h>

class F2DockMasterDialog : public QDialog
{
  Q_OBJECT
  public:
  F2DockMasterDialog(QWidget *parent=0,Qt::WFlags flags=0);
  virtual ~F2DockMasterDialog();

  // get methods
  string getReceptor(void);
  string getLigand(void);
  ResultDialog * getResultDialog(){return resultDial;}

  // set methods
  void setController(F2dockMasterControl *mc) {masterCont = mc;}
  void setResultDialog(ResultDialog *rd) {resultDial = rd;}
  
  public slots:
  virtual void createButtonSlot();
  virtual void loadButtonSlot();
  virtual void saveButtonSlot();
  virtual void closeButtonSlot();

  //virtual void detailsButtonSlot();
  virtual void resultButtonSlot();

  virtual void dockButtonSlot();
  virtual void rerankButtonSlot();
  virtual void f2dGenButtonSlot();
  virtual void quadGenButtonSlot();
  virtual void jobStatusChangedSlot(QString id, int status);
  virtual void resultSelectedSlot(int);
  virtual void vizSettingsChangedSlot();

//  virtual void jobSelectedSlot();
  signals:
        void resultSelectedFromUI(int);
	void resultUILaunched();
	void vizSettingsChanged();

  protected:
  Ui::f2dockMasterUI *_ui;

  private:
  F2dockMasterControl *masterCont;
  NaggerThread *nag; 
  ResultDialog *resultDial;
  ResultControl *resultCont;

  void startNag();
  void stopNag();
  void addJobToNag(string id, int status);  
  void removeJobFromNag(string id);

  void makeConnections();
  void init();
  void populateTable();
};

#endif
