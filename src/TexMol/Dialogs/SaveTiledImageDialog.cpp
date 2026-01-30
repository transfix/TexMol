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
#include <TexMol/Dialogs/SaveTiledImageDialog.h>
#include <QImageWriter>
#include <QFileDialog>

#include "ui_SaveTiledImageDialogBase.h"

SaveTiledImageDialog::SaveTiledImageDialog(QWidget *parent,Qt::WFlags flags) 
  : QDialog(parent, flags) {

  _ui = new Ui::SaveTiledImageDialogBase;
  _ui->setupUi(this);
 
  QList<QByteArray> imgList = QImageWriter::supportedImageFormats();
  if(!imgList.isEmpty()) {
    QStringList imgListQS;
    const int imgList_cnt = imgList.size();
    for(int i=0; i<imgList_cnt; i++)
    {
       imgListQS.push_back(QString(imgList.back()));
       imgList.pop_back();
    }
    _ui->m_fileFormat->addItems(imgListQS);
  }
  
  connect((const class QObject*)(_ui->m_searchFile), SIGNAL(clicked(bool)), this, SLOT(baseImageFileSlot()));
  connect((const class QObject*)(_ui->m_saveAll), SIGNAL(stateChanged()), this, SLOT(saveAllSlot()));
  connect(_ui->m_Ok, SIGNAL(clicked(bool)), this, SLOT(accept()));
  connect(_ui->m_Cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));

}


SaveTiledImageDialog::~SaveTiledImageDialog()
{ delete _ui;}

void SaveTiledImageDialog::baseImageFileSlot()
{
  QString baseImageFileName = QFileDialog::getSaveFileName("","(*.*)",this,"save file dialog","Save as",0);
  _ui->m_fileName->setText(baseImageFileName);
}

void SaveTiledImageDialog::saveAllSlot()
{
  if( _ui->m_saveAll->isChecked() )
    _ui->m_saveAll->setChecked( true );
  else
    _ui->m_saveAll->setChecked( false );
}

void SaveTiledImageDialog::getParameters(QString* fileName, QString* format, int *resol, int *ntiles, bool *saveAll, bool *mergeToSingleImage)
{
	*fileName = _ui->m_fileName->text();
	*format = _ui->m_fileFormat->currentText();
	*resol = (_ui->m_pixelResol->text()).toInt();
	*ntiles = (_ui->m_nTiles->text()).toInt();
	*saveAll = _ui->m_saveAll->isChecked();
        *mergeToSingleImage = _ui->m_mergeToSingleImage->isChecked();
}
