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
// Qt3Support removed

#include <q3textedit.h>
//#include <q3filedialog.h>
#include <QFileDialog>
#include <TexMol/Dialogs/MainWindow.h>
#include <TexMol/Dialogs/ScriptsDialog.h>
#include <TexMol/ArgumentList.h>


ScriptsDialog::ScriptsDialog(QWidget* parent)
{
	m_Parent = parent;
}

ScriptsDialog::~ScriptsDialog()
{
}

void ScriptsDialog::executeCommandsSequentially(ArgumentList* argumentList)
{
	if(argumentList == 0)
	{
		return;
	}

	int i;

	for(i=0; i<argumentList->argcs.size(); i++)
	{
		((MainWindow*)(m_Parent))->executeCommand(argumentList->argcs.at(i), argumentList->argvs.at(i));
	}
}

void ScriptsDialog::executeSlot()
{
	ArgumentList* argumentList = 0;
	QString commandList = m_CommandsTextEdit->selectedText();
	int cListLen =commandList.length();
	int first = 0;
	int second = commandList.find('\n');

	if(second == -1)
	{
		second = cListLen;
	}

	while(1)
	{
		QString command = commandList.mid(first, second-first);

		if(!(command.length() == 0) && !((command.length() == 1) && (command.at(0) == '\n')))
		{
			printf("Got a command : %s\n", command.toLatin1().constData());

			if(argumentList == 0)
			{
				argumentList = new ArgumentList();
			}

			argumentList->addNewCommand((command.simplifyWhiteSpace()));
		}

		if(second >= cListLen-1)
		{
			break;
		}

		first = second+1;
		second = commandList.find('\n', second+1);

		if(second == -1)
		{
			second = cListLen;
		}
	}

	executeCommandsSequentially(argumentList);
	delete 	argumentList;
}

void ScriptsDialog::loadSlot()
{
  //QString fileName = Q3FileDialog::getOpenFileName("", "(*.*)", this, "open file dialog", "Load file", 0);
	QString fileName = QFileDialog::getOpenFileName("", "(*.*)", this, "open file dialog", "Load file", 0);
	FILE* fp = fopen(fileName.toLatin1().constData(), "r");

	if(fp == 0)
	{
		return;
	}

	QString commandList;

	while(1)
	{
		char command[10000];
		int ret = fscanf(fp, "%s\n", command);

		if(ret ==0 || ret == EOF)
		{
			break;
		}

		commandList += QString(command);
	}

	fclose(fp);
	m_CommandsTextEdit->setText(commandList);
}

void ScriptsDialog::saveSlot()
{
  //QString fileName = Q3FileDialog::getSaveFileName(
  //						   "",
  //					   "(*.*)",
  //					   this,
  //					   "save file dialog",
  //					   "Save as",
  //					   0
  //				   );
  QString fileName = QFileDialog::getSaveFileName(
						   "",
						   "(*.*)",
						   this,
						   "save file dialog",
						   "Save as",
						   0
						   );

	FILE* fp = fopen(fileName.toLatin1().constData(), "w");

	if(fp == 0)
	{
		return;
	}

	QString commandList = m_CommandsTextEdit->selectedText();
	int cListLen =commandList.length();
	int first = 0;
	int second = commandList.find('\n');

	if(second == -1)
	{
		second = cListLen;
	}

	while(1)
	{
		QString command = commandList.mid(first, second-first);

		if(!(command.length() == 0))
		{
			fprintf(fp, "%s\n", command.toLatin1().constData());
		}
		else
		{
			fprintf(fp, "\n");
		}

		if(second >= cListLen-1)
		{
			break;
		}

		first = second+1;
		second = commandList.find('\n', second+1);

		if(second == -1)
		{
			second = cListLen;
		}
	}

	fclose(fp);
}
