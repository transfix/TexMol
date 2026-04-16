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
#include <TexMol/Dialogs/FileSaveDialog.h>
#include <qcheckbox.h>
#include <qimage.h>
//#include <q3filedialog.h>
#include <QFileDialog>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcombobox.h>
//Added by qt3to4:
#include <Q3StrList>
#include <QImageWriter>

FileSaveDialog::FileSaveDialog(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
	: FileSaveDialogBase(parent, name, modal, fl)
{
/*Q3Err:CS
	Q3StrList imgList = QImageWriter::supportedImageFormats();
	m_FileFormatComboBox->insertStrList(imgList);
*/
	QList<QByteArray> imgList = QImageWriter::supportedImageFormats();
	if(!imgList.isEmpty()) {
		QStringList imgList2;
		// just copy to QStringList
		const int imgList_cnt = imgList.size();
		for(int i=0; i<imgList_cnt; i++) {
			imgList2.push_back(QString(imgList.back()));
			imgList.pop_back();
		}
		m_FileFormatComboBox->insertStringList(imgList2, -1);
	}
}

FileSaveDialog::~FileSaveDialog()
{
}

bool FileSaveDialog::getSaveAll()
{
	return m_SaveAllCheckBox->isChecked();
}

void FileSaveDialog::getSelectedFileName(QString* imageFileName, QString* formatSelected)
{
	*imageFileName = m_FileNameLineEdit->text();
	*formatSelected = m_FileFormatComboBox->currentText();
}

void FileSaveDialog::fileNameSlot()
{
  //QString imageFileName = Q3FileDialog::getSaveFileName("", "(*.*)", this, "save file dialog", "Save as", 0);
	QString imageFileName = QFileDialog::getSaveFileName("", "(*.*)", this, "save file dialog", "Save as", 0);
	m_FileNameLineEdit->setText(imageFileName);
}
