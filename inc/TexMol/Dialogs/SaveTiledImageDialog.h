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
#ifndef _SAVE_TILED_IMAGE_DIALOG_H_
#define _SAVE_TILED_IMAGE_DIALOG_H_


#include <QDialog>
#include <QString>


namespace Ui
{
  class SaveTiledImageDialogBase;
}

#include "ui_SaveTiledImageDialogBase.h"


class SaveTiledImageDialog : public QDialog
{
  Q_OBJECT
    public:
  SaveTiledImageDialog(QWidget *parent=nullptr,Qt::WindowFlags flags={});
  
  virtual ~SaveTiledImageDialog();
  void getParameters(QString* fileName, QString* format, int *resol, int *ntiles, bool *saveAll, bool *mergeToSingleImage);

  public slots:

  void baseImageFileSlot();
  void saveAllSlot();
 
 protected:
  Ui::SaveTiledImageDialogBase *_ui;

};

#endif
