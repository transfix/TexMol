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
#ifndef CONSTRUCT_DEPTH_COLORED_VOLUMES_DIALOG_H
#define CONSTRUCT_DEPTH_COLORED_VOLUMES_DIALOG_H

#include <TexMol/Dialogs/ConstructDepthColoredVolumesDialogBase.h>

class VolumeData;
class DataManager;

class ConstructDepthColoredVolumesDialog : public ConstructDepthColoredVolumesDialogBase
{
	public:
		ConstructDepthColoredVolumesDialog(QString inputVolumeFileName, QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = {});
		virtual ~ConstructDepthColoredVolumesDialog();
		VolumeData* constructDepthColoredVolumes(DataManager* dataManager, QString volumeFileName);
		VolumeData* constructDepthColoredVolumesFromVolume(DataManager* dataManager, VolumeData* inputVolumeFile);
		bool loadFile();

	public slots:
		virtual void outputRawVFileNameSlot();

	protected:
		double getDistanceOfVoxel(int i, int j, int k, int width, int height, int depth);
		QString m_InputFileName;
		QString m_OutputRawVFileName;
};

#endif
