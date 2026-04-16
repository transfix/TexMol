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
#ifndef SURFACE_DIALOG_H
#define SURFACE_DIALOG_H

#include <QFutureWatcher>

#include <QDialog>
// Qt3 base removed — stub
typedef QDialog SurfaceDialogBase;
#include <Geometry/Geometry.h>
#include <PDBParser/GroupOfAtoms.h>



struct SurfaceBackgroundJob {
  QFuture<Geometry *> * future;
  QFutureWatcher<Geometry *> * watcher;
  QString name;
};

struct GetSurfaceData {
  QString algorithm;
  int size;
  int isovalue;
  double blobbiness;
  bool radiusType;
  int level;
  int iterations;
  double weight;
  bool optimizeRadii;
  PDBParser::GroupOfAtoms* molecule;
};

Geometry* getSurfaceExtern(GetSurfaceData d);
			   //QString algorithm, int size, int isovalue,double blobbiness,
			   //bool radiusType, int level, int iterations, double weight, 
			   //bool optimizeRadii, PDBParser::GroupOfAtoms* molecule);


class SurfaceDialog : public SurfaceDialogBase
{
	public:
		SurfaceDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
		virtual ~SurfaceDialog();
		Geometry* getSurface(PDBParser::GroupOfAtoms* molecule);

	public slots:
		virtual void setupSlot(const QString& algorithm);

	protected:
		QWidget * m_Parent;
};

#endif
