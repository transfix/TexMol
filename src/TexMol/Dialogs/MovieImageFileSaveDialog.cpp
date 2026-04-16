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
#include <TexMol/Dialogs/MovieImageFileSaveDialog.h>
#include <qcheckbox.h>
//#include <q3filedialog.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>
//Added by qt3to4:
#include <Q3StrList>
#include <QImageWriter>
#include <QFileDialog>

MovieImageFileSaveDialog::MovieImageFileSaveDialog(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
	: MovieImageFileSaveDialogBase(parent, name, modal, fl)
{
/*Q3Err:CS
	Q3StrList imgList = QImageIO::outputFormats();
	m_ImageFileFormatsComboBox->insertStrList(imgList);
*/
	QList<QByteArray> imgList = QImageWriter::supportedImageFormats();
	if(!imgList.isEmpty()) {
		QStringList imgListQS;
		const int imgList_cnt = imgList.size();
		for(int i=0; i<imgList_cnt; i++)
		{
			imgListQS.push_back(QString(imgList.back()));
			imgList.pop_back();
		}
		m_ImageFileFormatsComboBox->insertStringList(imgListQS, -1);
	}
}

MovieImageFileSaveDialog::~MovieImageFileSaveDialog()
{
}

void MovieImageFileSaveDialog::getSelectedFileNames(QString* animationFileName, QString* imageFileName, QString* formatSelected)
{
	*animationFileName = m_AnimationFileNameLineEdit1->text();
	*imageFileName = m_BaseImageFileNameLineEdit->text();
	*formatSelected = m_ImageFileFormatsComboBox->currentText();
}

void MovieImageFileSaveDialog::baseImageFileSlot()
{
  //QString baseImageFileName = Q3FileDialog::getSaveFileName("","(*.*)",this,"save file dialog","Save as",0);
  QString baseImageFileName = QFileDialog::getSaveFileName("","(*.*)",this,"save file dialog","Save as",0);
  m_BaseImageFileNameLineEdit->setText(baseImageFileName);
}

void MovieImageFileSaveDialog::animationFileSlot()
{
  //QString animationFileName = Q3FileDialog::getSaveFileName("","(*.*)",this,"save file dialog","Save as",0);
  //QString animationFileName = QFileDialog::getSaveFileName("","(*.*)",this,"save file dialog","Save as",0);
  QString animationFileName = QFileDialog::getOpenFileName("","(*.*)",this,"open file dialog","Open",0);
  m_AnimationFileNameLineEdit1->setText(animationFileName);
}
