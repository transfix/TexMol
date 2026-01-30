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
#ifndef POISSON_BOLTZMANN_DIALOG_H
#define POISSON_BOLTZMANN_DIALOG_H


#include <QDialog>
#include <QString>


namespace Ui
{
  class PoissonBoltzmannDialogBase;
}

#include "ui_PoissonBoltzmannDialogBase.h"


class PoissonBoltzmannDialog : public QDialog
{
  Q_OBJECT
    public:
  PoissonBoltzmannDialog(QWidget *parent=0,Qt::WFlags flags=0);
  
  virtual ~PoissonBoltzmannDialog();
  
  public slots:
  
  virtual void pbPqrFileNameSlot();
  virtual void pbRawnFileNameSlot();
  virtual void pbOutputPrefixSlot();

  QString getPqrFileName();
  QString getRawnFileName();
  QString getOutputPrefix();
  double getTemp();
  double getIonConc();


 protected:
  Ui::PoissonBoltzmannDialogBase *_ui;

};


/*
class PoissonBoltzmann : public PoissonBoltzmannBase
{
        public:
                PoissonBoltzmann(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0);
                virtual ~PoissonBoltzmann();
                virtual void getSelectedFileNames(QString* pqrFileName, QString* rawnFileName, QString* outputFilePrefix);
                virtual void getData();

        public slots:
                virtual void pbPQRFileSlot();
                virtual void pbRawnFileSlot();
                virtual void pbOutputPrefixSlot();

 private:
		double temp;
		double ionConc;
};
*/

#endif
