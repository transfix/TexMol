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
// NURBSData.h: interface for the NURBSData class.

#ifndef CCV_TEXMOL_DATA_MANAGER_NURBS_DATA_H
#define CCV_TEXMOL_DATA_MANAGER_NURBS_DATA_H

#include <qstring.h>
#include <vector>

#include <DataManager/AbstractData.h>
#include <UsefulMath/Ray.h>
#include <Nurbs/NURBS.h>

class DataManager;
class NURBSRenderingProperties;

using std::vector;

class NURBSData  : public AbstractData
{
		Q_OBJECT

	public:
		NURBSData(DataManager* dataManager);
		virtual ~NURBSData();

		static bool supportsFileNames(QStringList fileNames);

		virtual QWidget* getPropertiesWidget();
		virtual void setVisibilityInPropertiesWidget();
		virtual QString getSelection(CCVOpenGLMath::Ray targetVector);
		bool read(QStringList fileNames);
		bool save(const char* filename);
		virtual bool getMinMax(float* min, float* max);
		bool parseAnimationCommand(QStringList commands, int curCommand);

		void render();
		void renderOnce();

		NURBSRenderingProperties* m_NURBSRenderingProperties;

		vector<CCV_NURBS::NURBS*> m_NURBS;

	protected:
		void deleteData();
		vector<double> m_min;
		vector<double> m_max;
};

#endif
