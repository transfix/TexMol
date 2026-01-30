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
// NURBSData.cpp: implementation of the NURBSData class.

#include "ui_NURBSRenderingPropertiesBase.h"

#include <DataManager/NURBSDataManager/NURBSData.h>
#include <Nurbs/NURBSLoader.h>
#include <DataManager/NURBSDataManager/NURBSRenderingProperties.h>
#include <DataManager/DataManager.h>
#include <qcheckbox.h>
#include <qslider.h>

using namespace CCV_NURBS;

NURBSData::NURBSData(DataManager* dataManager)
	: AbstractData(dataManager)
{
	if(dataManager)
	{
		m_DataManager = dataManager;
		m_NURBSRenderingProperties = new NURBSRenderingProperties(dataManager->m_ParentPropertiesWidget);
		m_NURBSRenderingProperties->hide();
		connect((const class QObject*)(m_NURBSRenderingProperties->_ui->m_RenderCheckBox), SIGNAL(toggled(bool)), this, SLOT(renderEnabledSlot(bool)));
	}
}

NURBSData::~NURBSData()
{
	deleteData();
}

void NURBSData::deleteData()
{
	{
		std::vector<NURBS*>::iterator iter = m_NURBS.begin(), end = m_NURBS.end();

		for(; iter != end; ++iter)
		{
			delete *iter;
		}

		m_NURBS.clear();
	}
}

bool NURBSData::supportsFileNames(QStringList fileNames)
{
	if(fileNames.size() < 1)
	{
		return false;
	}

	int i;
	bool ret = true;

	for(i=0; i<fileNames.size(); i++)
	{
		ret = ret && (fileNames[i].endsWith("nurb"));
	}

	return ret;
}

QWidget* NURBSData::getPropertiesWidget()
{
	return m_NURBSRenderingProperties;
}

void NURBSData::setVisibilityInPropertiesWidget()
{
	if(m_NURBSRenderingProperties)
	{
		m_NURBSRenderingProperties->_ui->m_RenderCheckBox->setChecked(renderEnabled[m_DataManager->getCurrentRenderer()]);
	}
}

bool NURBSData::read(QStringList fileNames)
{
	deleteData();
	int numModels = fileNames.size();

	if(numModels < 1)
	{
		return false;
	}

	int i;

	for(i=0; i<numModels; i++)
	{
		const char* fileName = (fileNames[i]).latin1();

		if(!fileName)
		{
			m_NURBS.push_back(0);
			continue;
		}

		// Load nurb if filename ends with nurb
		if(strstr(fileName,"nurb") || strstr(fileName, "NURB"))
		{
			NURBSLoader* nurbsLoader = new NURBSLoader();
			NURBS* nurbs = nurbsLoader->load(fileName);
			delete nurbsLoader;

			if(!nurbs)
			{
				m_NURBS.push_back(0);
			}
			else
			{
				m_NURBS.push_back(nurbs);
			}
		}

		// Done loading
	}

	if(m_NURBSRenderingProperties)
	{
		if(m_NURBS.size() > 1)
		{
			m_NURBSRenderingProperties->_ui->m_FrameSlider->setMinValue(0);
			m_NURBSRenderingProperties->_ui->m_FrameSlider->setMaxValue(m_NURBS.size()-1);
			m_NURBSRenderingProperties->_ui->m_FrameSlider->setTickInterval(1);
			m_NURBSRenderingProperties->_ui->m_FrameSlider->setEnabled(true);
		}
		else
		{
			m_NURBSRenderingProperties->_ui->m_FrameSlider->setMinValue(0);
			m_NURBSRenderingProperties->_ui->m_FrameSlider->setMaxValue(0);
			m_NURBSRenderingProperties->_ui->m_FrameSlider->setTickInterval(1);
			m_NURBSRenderingProperties->_ui->m_FrameSlider->setEnabled(false);
		}
	}

	m_Frame = 0;
	bool found = false;

	for(i=0; i<m_NURBS.size(); i++)
	{
		if(m_NURBS[i])
		{
			found = true;
		}
	}

	if(found)
	{
		return true;
	}

	deleteData();
	return false;
}

bool NURBSData::getMinMax(float* min, float* max)
{
	if(m_NURBS.size() <= m_Frame)
	{
		return false;
	}

	if(!m_NURBS[m_Frame])
	{
		return false;
	}

	if(m_min.size() < (m_Frame+1)*3)
	{
		return false;
	}

	if(m_max.size() < (m_Frame+1)*3)
	{
		return false;
	}

	min[0] = m_min[m_Frame*3+0];
	min[1] = m_min[m_Frame*3+1];
	min[2] = m_min[m_Frame*3+2];
	max[0] = m_max[m_Frame*3+0];
	max[1] = m_max[m_Frame*3+1];
	max[2] = m_max[m_Frame*3+2];
	return true;
}

bool NURBSData::parseAnimationCommand(QStringList commands, int curCommand)
{
	return false;
}

bool NURBSData::save(const char* filename)
{
	if(!filename)
	{
		return false;
	}

	if(m_NURBS.size() <= m_Frame)
	{
		return false;
	}

	NURBS* curNurbs = m_NURBS[m_Frame];

	if(!curNurbs)
	{
		return false;
	}

	// save it.
	return true;
}

QString NURBSData::getSelection(CCVOpenGLMath::Ray targetVector)
{
	return 0;
}

void NURBSData::renderOnce()
{
	if(m_NURBS.size() <= m_Frame)
	{
		return;
	}

	if(!m_NURBS[m_Frame])
	{
		return;
	}

	if(m_NURBS[m_Frame])
	{
		m_NURBS[m_Frame]->render();
	}
}

void NURBSData::render()
{
	if(m_NURBS.size() <= m_Frame)
	{
		return;
	}

	if(!m_NURBS[m_Frame])
	{
		return;
	}

	if(m_ReplicationMatrices.size() > 0)
	{
		int i;

		for(i=0; i<m_ReplicationMatrices.size(); i++)
		{
			CCVOpenGLMath::Matrix* transformation = m_ReplicationMatrices[i];

			if(!transformation)
			{
				continue;
			}

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMultMatrixf(transformation->getMatrix());
			//if( distanceSqFromObject() < cutoffSq ) // need to make this a multires call
			renderOnce();
			glPopMatrix();
		}
	}
	else
	{
		renderOnce();
	}
}
