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
#include <F2DockClient/f2dockMasterDialog.h>

#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

#include "ui_f2dockMasterUI.h"

using namespace std;

F2DockMasterDialog::F2DockMasterDialog(QWidget *parent,Qt::WFlags flags) 
  : QDialog(parent, flags) {

  _ui = new Ui::f2dockMasterUI;
  _ui->setupUi(this);

  init();
  startNag();
  makeConnections();
}


F2DockMasterDialog::~F2DockMasterDialog()
{
	delete _ui;
	stopNag();
}


void F2DockMasterDialog::startNag()
{
	nag = new NaggerThread(this);
}

void F2DockMasterDialog::stopNag()
{
	nag->quit();
}

void F2DockMasterDialog::addJobToNag(string id, int status)
{
	nag->addJob(id, status);
}

void F2DockMasterDialog::removeJobFromNag(string id)
{
	nag->removeJob(id);
}


void F2DockMasterDialog::init()
{
	QStringList labels;
	labels.append("Job Id");
	labels.append("Job Type");
	labels.append("Job Status");
	labels.append("Job Title");

	_ui->jobTableWidget->setHorizontalHeaderLabels(labels);
	_ui->jobTableWidget->setRowCount(10);

//	_ui->rerankButton->setEnabled(false);

	masterCont = new F2dockMasterControl();
	resultDial = new ResultDialog(this);
}


void F2DockMasterDialog::makeConnections()
{
//	connect( _ui->jobTableWidget, SIGNAL( itemSelectionChanged () ), this, SLOT( jobSelectedSlot() ) );

	connect( _ui->createButton, SIGNAL( clicked() ), this, SLOT( createButtonSlot() ) );
	connect( _ui->loadButton, SIGNAL( clicked() ), this, SLOT( loadButtonSlot() ) );
	connect( _ui->saveButton, SIGNAL( clicked() ), this, SLOT( saveButtonSlot() ) );
	connect( _ui->closeButton, SIGNAL( clicked() ), this, SLOT( closeButtonSlot() ) );

	connect( _ui->resultButton, SIGNAL( clicked() ), this, SLOT( resultButtonSlot() ) );
//	connect( _ui->detailsButton, SIGNAL( clicked() ), this, SLOT( detailsButtonSlot() ) );

	connect( _ui->dockButton, SIGNAL( clicked() ), this, SLOT( dockButtonSlot() ) );
	connect( _ui->rerankButton, SIGNAL( clicked() ), this, SLOT( rerankButtonSlot() ) );
	connect( _ui->f2dGenButton, SIGNAL( clicked() ), this, SLOT( f2dGenButtonSlot() ) );
	connect( _ui->quadGenButton, SIGNAL( clicked() ), this, SLOT( quadGenButtonSlot() ) );

	connect( nag, SIGNAL( jobStatusChanged(QString, int) ), this, SLOT( jobStatusChangedSlot(QString, int) ) );
	connect( resultDial, SIGNAL( resultSelected(int) ), this, SLOT( resultSelectedSlot(int) ) );
	connect( resultDial, SIGNAL( vizSettingsChanged() ), this, SLOT( vizSettingsChangedSlot() ) );
}


void F2DockMasterDialog::populateTable()
{
	int numberOfJobs = masterCont->getNumberOfJobs();

	nag->clearJobs();

	int rows = _ui->jobTableWidget->rowCount();
	int columns = _ui->jobTableWidget->columnCount();

	for(int i = 0; i<rows ; i++ )
	{
		for(int j = 0; j<columns ; j++ )
		{
			_ui->jobTableWidget->setItem(i, j, new QTableWidgetItem( "" ));
		}		
	}
	
	for(int i = 0; i<numberOfJobs ; i++ )
	{
		_ui->jobTableWidget->setItem(i, 0, new QTableWidgetItem( masterCont->getJobId(i).c_str() ));
		_ui->jobTableWidget->setItem(i, 1, new QTableWidgetItem( masterCont->getJobType(i).c_str() ));
		_ui->jobTableWidget->setItem(i, 2, new QTableWidgetItem( masterCont->getJobStatus(i).c_str() ));
		_ui->jobTableWidget->setItem(i, 3, new QTableWidgetItem( masterCont->getJobTitle(i).c_str() ));

		if(masterCont->isRunning(i)) nag->addJob(masterCont->getJobId(i).c_str(), masterCont->getJobStatusInt(i));
	}
}


void F2DockMasterDialog::resultSelectedSlot(int index)
{
	emit resultSelectedFromUI(index);
}

void F2DockMasterDialog::vizSettingsChangedSlot()
{
	emit vizSettingsChanged();
}


void F2DockMasterDialog::createButtonSlot()
{
	if(masterCont->getNumberOfJobs()>=10)
	{
		QMessageBox::information(this, "Cannot load more than 10 jobs at a time",  "Cannot load more than 10 jobs at a time");
		return;
	}

	QString inputFileName = QFileDialog::getOpenFileName(this, "Select a docking input file (.inp)", QString::null, "inp (*.inp)");

	if(!inputFileName.isNull()) 
	{
		QString outputFileName = QFileDialog::getOpenFileName(this, "Select the output file (.out)", QString::null, "txt (*.txt)");

		if(!outputFileName.isNull())
		{
			if(masterCont->createJob(inputFileName.toStdString(), outputFileName.toStdString()))
			{
				QMessageBox::information(this, "Job Created from output file",  "Job Created from output file");
				populateTable();
			}
			else
			{
				QMessageBox::information(this, "Failed to create job from output file", "Failed to create job from output file");
			}
		}
	}
}


void F2DockMasterDialog::loadButtonSlot()
{
	if(masterCont->getNumberOfJobs()>=10)
	{
		QMessageBox::information(this, "Cannot load more than 10 jobs at a time",  "Cannot load more than 10 jobs at a time");
		return;
	}

	if(masterCont->loadJob( (QFileDialog::getOpenFileName(this, "Load job from file", QString::null, "JOB (*.job)")).toStdString() ))
	{
		QMessageBox::information(this, "Job Loaded",  "Job Loaded");
		populateTable();
	}
	else
	{
		QMessageBox::information(this, "Failed to load job", "Failed to load job");
	}
}

void F2DockMasterDialog::saveButtonSlot()
{
	if(masterCont->saveJob(_ui->jobTableWidget->currentRow() ))
	{
		QMessageBox::information(this, "Job Saved",  "Job Saved");
		populateTable();
	}
	else
	{
		QMessageBox::information(this, "Failed to save job", "Failed to save job");
	}
}

void F2DockMasterDialog::closeButtonSlot()
{
	if(masterCont->closeJob( _ui->jobTableWidget->currentRow() ))
	{
		QMessageBox::information(this, "Job Closed",  "Job Closed");
		populateTable();
	}
	else
	{
		QMessageBox::information(this, "Failed to close job", "Failed to close job");
	}	
}

void F2DockMasterDialog::resultButtonSlot()
{
	resultCont = masterCont->getResultControl(_ui->jobTableWidget->currentRow());

	cout<<"here 4 "<<endl;

	if(!resultDial->setController(resultCont))
	{
		QMessageBox::information(this, "Failed to show result", "Failed to show result");
	}
	else
	{
		resultDial->show();
		emit resultUILaunched();
	}
/*
	if(masterCont->showResults( _ui->jobTableWidget->currentRow() ))
	{
		
	}
	else
	{
		//QMessageBox::information(this, "Results not available", "Results not available");
	}*/	
}
/*
void F2DockMasterDialog::detailsButtonSlot()
{
	if(masterCont->showDetails( _ui->jobTableWidget->currentRow() ))
	{
		
	}
	else
	{
		QMessageBox::information(this, "Details not available", "Details not available");
	}	
}*/

void F2DockMasterDialog::jobStatusChangedSlot(QString id, int status)
{
	if(masterCont->updateJobStatus( id.toStdString(), status))
	{
		populateTable();
	}
}



void F2DockMasterDialog::dockButtonSlot()
{
	if(masterCont->getNumberOfJobs()>=10)
	{
		QMessageBox::information(this, "Cannot load more than 10 jobs at a time",  "Cannot load more than 10 jobs at a time");
		return;
	}

	if(masterCont->handleDockingRequest())
	{
		QMessageBox::information(this, "Docking Job Submitted", "Docking Job Submitted");
		populateTable();		
	}
	else
	{
		QMessageBox::information(this, "Failed to submit docking job", "Failed to submit docking job");
	}	
}

void F2DockMasterDialog::rerankButtonSlot()
{
	if(masterCont->getNumberOfJobs()>=10)
	{
		QMessageBox::information(this, "Cannot load more than 10 jobs at a time",  "Cannot load more than 10 jobs at a time");
		return;
	}

	if(masterCont->handleRerankingRequest())
	{
		QMessageBox::information(this, "Reranking Job Submitted", "Reranking Job Submitted");
		populateTable();		
	}
	else
	{
		QMessageBox::information(this, "Failed to submit reranking job", "Failed to submit reranking job");
	}	
}

void F2DockMasterDialog::f2dGenButtonSlot()
{
	if(masterCont->getNumberOfJobs()>=10)
	{
		QMessageBox::information(this, "Cannot load more than 10 jobs at a time",  "Cannot load more than 10 jobs at a time");
		return;
	}

	if(masterCont->handleF2dGenRequest() )
	{
		QMessageBox::information(this, "F2dGen Job Submitted", "F2dGen Job Submitted");
		populateTable();		
	}
	else
	{
		QMessageBox::information(this, "Failed to submit F2dGen job", "Failed to submit F2dGen job");
	}	
}

void F2DockMasterDialog::quadGenButtonSlot()
{
	if(masterCont->getNumberOfJobs()>=10)
	{
		QMessageBox::information(this, "Cannot load more than 10 jobs at a time",  "Cannot load more than 10 jobs at a time");
		return;
	}

	if(masterCont->handleQuadGenRequest() )
	{
		QMessageBox::information(this, "QuadGen Job Submitted", "QuadGen Job Submitted");
		populateTable();		
	}
	else
	{
		QMessageBox::information(this, "Failed to submit QuadGen job", "Failed to submit QuadGen job");
	}	
}


string F2DockMasterDialog::getReceptor(void)
{
	return masterCont->getReceptor(_ui->jobTableWidget->currentRow());
}


string F2DockMasterDialog::getLigand(void)
{
	return masterCont->getLigand(_ui->jobTableWidget->currentRow());
}




