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
#if !defined(AFX_DATAMANAGER_H__D330C23B_B629_4F2A_B174_FB33A71BB443__INCLUDED_)
#define AFX_DATAMANAGER_H__D330C23B_B629_4F2A_B174_FB33A71BB443__INCLUDED_

#include <TexMol/compat.h>

#include <DataManager/DataSetMap.h>
#include <UsefulMath/Ray.h>
#include <DataManager/BoundingBox.h>
#include <OpenGL_Viewer/View.h>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QList>
//Added by qt3to4:
#include <QMouseEvent>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

class QListWidget;
class VolumeRendererExtn;
class SplattingRendererExtn;
class GLImposter;
class GeometryRenderer;
class QSProject;
class RendererSet;
class Animator;
class AbstractData;
class VolumeData;
class SurfaceData;
class SecondaryStructureData;
class BallAndStickData;
class NURBSData;

const int NUMBER_OF_DATA_TYPES = 4;

static QString strDataTypes[26] =
{
	"Molecules", "7", "pdb","pqr","goa","pts", "xyz", "xyzr", "pdb1",
	"Surface geometry", "6", "raw","rawn","rawc","rawnc","c2c","obj",
	"Volumes", "4", "rawiv","rawv","dx","mrc",
	"NURBS", "1", "nurb"
};

class DataManager
{
	public:
	DataManager(QSProject* qSProject, QWidget* parentWidget, QListWidget* dataSetsListBox,
					bool renderGlobalBoundingBox, bool renderDataBoundingBox, RendererSet* rendererSet);
		virtual ~DataManager();
		virtual void init();
		QString* getDataTypes();
		int getNumberOfDataTypes();
		virtual bool addNewDataSet(QStringList fileNames, bool instantVisibility = true);
		virtual void deleteDataSet();
		virtual bool save(int selectedIndex, const char* filename);
		virtual int getNumberOfDataSets();
		virtual QString getDataSetName(int index);
		virtual QWidget* getPropertiesWidget();
		virtual void setCurrentRendererId(int currentRendererId);
		virtual bool render(int rendererId);
		bool uploadColorMap(unsigned char* colorMap, int size);
		bool uploadColorMap(double* colorMap, int size);
		virtual int getCurrentRenderer();
		virtual void setVisibilityInPropertiesWidget();
		virtual void setSelectedIndex(int selectedIndex);
		virtual void setDataSetIndex(int dataSetIndex);
		QWidget* m_ParentPropertiesWidget;
		bool checkVolumeRendererAllocation();
		bool addData(BallAndStickData* ballAndStickData, QString fileName);
		bool addData(VolumeData* volumeData, QString fileName);
		bool addData(SurfaceData* surfaceData, QString fileName);
		bool addData(SecondaryStructureData* secondaryStructureData, QString fileName);
		bool addData(NURBSData* nurbsData, QString fileName);
		void deleteIsocontourBarFromVolumes(int surfaceIndex);
		bool deleteIsocontour(int index); // dont delete the bar
		bool setVisible(bool render);
		bool setVisible(int index, bool render);
		void updateUIWithNewData();
		int getCurrentDataSetType();
		int getCurrentDataSetIndex();
		void pan(CCVOpenGLMath::Vector v);
		void rotate(CCVOpenGLMath::Matrix m);
		void scale(float scale);
		virtual bool mousePress3DEvent(View*, QMouseEvent* e);
		virtual bool mouseRelease3DEvent(View*, QMouseEvent* e);
		virtual bool mouseDoubleClick3DEvent(View*, QMouseEvent* e);
		virtual bool mouseMove3DEvent(View*, QMouseEvent* e);
		void debugPrint();
		AbstractData* getDataFromUniqueID(int uniqueID);
		BallAndStickData* getBallAndStickData(int id);
		VolumeData* getScalarVolumeData(int id);
		VolumeData* getScalarVolumeData(); // returns last scalar volume data
		VolumeData* getVectorVolumeData(int id);
		SurfaceData* getSurfaceData(int id);
		NURBSData* getNURBSData(int id);
		SecondaryStructureData* getSecondaryStructureData(int id);
		int getDataSetIndex(int index);
		AbstractData* getSelectedData(int selectedDataIndex);
		QString getSelection(CCVOpenGLMath::Ray targetVector);
		bool parseAnimationCommand(QStringList commands, int curCommand);
		CCVOpenGLMath::Matrix getTransformation(int selectedIndex);
		bool setTransformation(int selectedIndex, CCVOpenGLMath::Matrix m);
		bool addTransformation(int selectedIndex, CCVOpenGLMath::Matrix m, float scale);
		QSProject* getQSProject();
		int m_DataSetsAdded; // we do not decrease when deleting, best to remove this variable
		int m_NextUniqueAvailableId;
		RendererSet* m_RendererSet;
		int m_RenderingMode;

		void Test_Call_VolumeRender(void);

	public slots:
		void functionChangedSlot();
		bool update();
		void globalBoundingBoxSlot(bool);
		void dataBoundingBoxSlot(bool);

	protected:
		GLImposter* m_GLImposter;
		VolumeRendererExtn* m_VolumeRendererExtn;
		SplattingRendererExtn* m_SplattingRendererExtn;
		QList<DataSetMap*> m_DataSetMapList;
		int m_CurrentDataType;
		int m_CurrentDataSet;
		QList<BallAndStickData*> m_BallAndStickDataArray;
		QList<VolumeData*> m_ScalarVolumeArray;
		QList<VolumeData*> m_VectorVolumeArray;
		QList<SurfaceData*> m_SurfaceArray;
		QList<SecondaryStructureData*> m_SecondaryStructureArray;
		QList<NURBSData*> m_NURBSArray;
		bool m_Initialized;
		int m_CurrentRendererId;
		QListWidget* m_DataSetsListBox;
		int m_SelectedIndex;
		BoundingBox m_GlobalBoundingBox;
		bool m_RenderGlobalBoundingBox;
		bool m_RenderDataBoundingBox;
		QSProject* m_QSProject;
};

#endif
