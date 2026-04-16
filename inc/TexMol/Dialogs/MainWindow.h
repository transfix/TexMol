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
/*
	The QT GUI application starts with the execution of this main window widget.
	All the different functionalities present in the UI should be found in the
	documentation on how to use the user interface.
*/
#include <TexMol/Dialogs/RenderAreaWidget.h>
#include <F2DockClient/f2dockMasterDialog.h>
#include <F2DockClient/resultDialog.h>

//#include <q3ptrlist.h>
//#include <qthread.h>

//#include <Qt3Support>

//Added by qt3to4:

//#include <QCustomEvent>
//#include <QCloseEvent>
//#include <Q3BoxLayout>
//#include <QLabel>
//#include <QKeyEvent>
#include <PDBParser/GOAFileIO.h>
#include <DataManager/BallAndStickDataManager/BallAndStickData.h>
#include <DataManager/SurfaceDataManager/SurfaceData.h>

#include <QMainWindow>

namespace Ui
{
  class MainWindowBase;
}


template <class type> class QFuture;
template <class type> class QFutureWatcher;

class DataManager;
class Q3NetworkOperation;
class Q3BoxLayout;
class QKeyEvent;
class QCloseEvent;
class RendererSet;
class Documentation;
class Settings;
class Server;
class ScriptsDialog;
class QLabel;
class Q3Http;
class QFile;
class MouseHandler;
class LightSet;
class QSProject;
class Q3ListBoxItem;
class SurfaceBackgroundJob;
class QGridLayout;

class moleculeVisData
{
	public:
		int resultIndex;	// docking result index

		/* Visualization properties */
		int renderType;		// 0 = UofB, 1 = surface, 2 = potential
		int potentialType;	// 0 = GB, 1 = PB, 2 = Interface propensity, 3 = residue contact
		bool colorByInterface;
		bool isVisiable;
		bool istransparent;


		/* data for rendering using datamanager*/
	
		PDBParser::GroupOfAtoms* moleculeGOA;
		BallAndStickData* moleculeUofBData;
		int goaDataManagerIndex;
		bool goaAvailable;

		Geometry* moleculeSurface;
		SurfaceData* moleculeSurfaceData;
		int surfaceDataManagerIndex;
		bool surfaceAvailable;

		Geometry* moleculePotentialSurface;
		SurfaceData* moleculePotentialSurfaceData;
		int potentialDataManagerIndex;
		bool poteintialAvailable;

		CCVOpenGLMath::Matrix trans;

		moleculeVisData()
		{
			resultIndex = -1;

			goaAvailable = false;
			surfaceAvailable = false;
			poteintialAvailable = false;

			goaDataManagerIndex = -1;
			surfaceDataManagerIndex = -1;
			potentialDataManagerIndex = -1;

			renderType = 0;		
			potentialType = 0;	
			colorByInterface = false;
			isVisiable = false;
			istransparent = false;

			trans = CCVOpenGLMath::Matrix( 1.0, 0.0, 0.0, 0.0 , 0.0, 1.0, 0.0, 0.0 , 0.0, 0.0, 1.0, 0.0 , 0.0, 0.0, 0.0, 1.0);
		}

		~moleculeVisData()
		{
			if(moleculeGOA) delete moleculeGOA;
			if(moleculeUofBData) delete moleculeUofBData;
			if(moleculeSurface) delete moleculeSurface;
			if(moleculeSurfaceData) delete moleculeSurfaceData;
			if(moleculePotentialSurface) delete moleculePotentialSurface;
			if(moleculePotentialSurfaceData) delete moleculePotentialSurfaceData;
		}
};

class MainWindow : public QMainWindow
{
		Q_OBJECT

/*		class F2DockThread : public QThread
		{
			public:
				F2DockThread(MainWindow* parent, unsigned int stackSize = 0);
				virtual void run();
			
			public slots:
				virtual void resultSelectedSlot(double *mat);

			signals:
				void resultSelectedFromMaster(double *mat);

			private:
				MainWindow* parent;
				F2DockMasterDialog* fdm;
		} *fdt;
		F2DockThread* f2dThread;*/
		F2DockMasterDialog *masterDial;

	public:
		MainWindow(QWidget* parent = nullptr, const char* name = 0, Qt::WindowFlags f = Qt::Window);
		virtual ~MainWindow();

	public slots:
		virtual bool addNewDataSet(QStringList fileNames, bool separate = false);
		virtual bool deleteAllData();
		virtual bool deleteData(int dataSetIndex);
		virtual bool deletePrevData();
		virtual bool executeCommand(int argc, QStringList argv);
		virtual bool fileOpenSlot();
		virtual bool fileOpenMultiSlot();
		virtual bool saveImage(const char* fileName, const char* fileFormat);
		virtual bool setGridVisible(bool visibility);
		virtual bool setViewingParameters(double* translationParams, double* rotationParams, double windowSize);
		virtual bool setViewingParameters(int rendererId, double* translationParams, double* rotationParams, double windowSize);
		virtual bool setVisible(bool render, int dataSetIndex);
		virtual bool setVisiblePrev(bool render);
		virtual QString GB_write_area_file(QString in_filename);
		virtual QString GB_write_surface_file(QString in_filename);
		virtual void aboutHelpSlot();
		virtual void acknowledgementSlot();
		virtual void backgroundColorSlot();
		virtual void computePocketTunnelSlot();
		virtual void constructCurvaturesUtilitySlot();
		virtual void constructDepthColoredVolumeSlot();
		virtual void constructHLSPocketsSlot();
		virtual void constructNurbsSurfaceSlot();
		virtual void constructPocketsSlot();
		virtual void constructVolumeUtilitiesSlot();
		virtual void dataBoundingBoxSlot(bool);
		virtual void dataSetSelectedRightMouse(Q3ListBoxItem* qListBoxItem, const QPoint& qPoint);
		virtual void dataTransferProgressop(int bytesDone, int bytesTotal);
		virtual void deleteAndUpdateUI(int selectedIndex);
		virtual void dockingSlot();
		virtual void downloadPDBSlot();
		virtual void editLight(int light);
		virtual void elucidateSecondaryStructuresSlot();
		virtual void finishedop(bool b);
		virtual void forceMeshRenderingSlot(bool status);
		virtual void fullscreenSlot();
		virtual void GB_energy_Slot();
		virtual void GB_forceField_Slot();
		virtual void globalBoundingBoxSlot(bool);
		virtual void indexHelpSlot();
		virtual void matchSlot();
		virtual void newDataSetSelectedSlot(int);
		virtual void orthographicViewSlot();
		virtual void PB_energy_Slot();
		virtual void perspectiveViewSlot();
		virtual void playbackAnimationSlot();
		virtual void playbackMovieSlot();
		virtual void readPrevioslyOpenedFile(int previousFileIndexInMenu);
		virtual void recordAnimationSlot();
		virtual void recordStereoAnimationSlot();
		virtual void recordStereoPresetX30deg2secSlot();
		virtual void recordStereoPresetX30deg4secSlot();
		virtual void recordStereoPresetX30deg8secSlot();
		virtual void recordStereoPresetX60deg4secSlot();
		virtual void recordStereoPresetX60deg8secSlot();
		virtual void recordStereoPresetX60deg16secSlot();
		virtual void recordStereoPresetX90deg6secSlot();
		virtual void recordStereoPresetX90deg12secSlot();
		virtual void recordStereoPresetX90deg24secSlot();
		virtual void recordStereoPresetY30deg2secSlot();
		virtual void recordStereoPresetY30deg4secSlot();
		virtual void recordStereoPresetY30deg8secSlot();
		virtual void recordStereoPresetY60deg4secSlot();
		virtual void recordStereoPresetY60deg8secSlot();
		virtual void recordStereoPresetY60deg16secSlot();
		virtual void recordStereoPresetY90deg6secSlot();
		virtual void recordStereoPresetY90deg12secSlot();
		virtual void recordStereoPresetY90deg24secSlot();
		virtual void recordStereoPresetZ30deg2secSlot();
		virtual void recordStereoPresetZ30deg4secSlot();
		virtual void recordStereoPresetZ30deg8secSlot();
		virtual void recordStereoPresetZ60deg4secSlot();
		virtual void recordStereoPresetZ60deg8secSlot();
		virtual void recordStereoPresetZ60deg16secSlot();
		virtual void recordStereoPresetZ90deg6secSlot();
		virtual void recordStereoPresetZ90deg12secSlot();
		virtual void recordStereoPresetZ90deg24secSlot();
		virtual void redraw();
		virtual void renderTypeSlot(bool);
		virtual void saveImageSlot();
		virtual void saveTiledImageSlot();
		virtual void scriptSlot();
		virtual void selectObjectsSlot(bool);
		virtual void setViewingParametersSlot();
		virtual void showGridOptionsSlot(bool);
		virtual void showMousePositionSlot(bool);
		virtual void showViewInformationSlot(bool);
		virtual void sliceParameterizationSlot();
		virtual void splitViewSlot();
		virtual void startRecordSlot();
		virtual void startPerFrameRecordSlot();
		virtual void recordCurrentFrameSlot();
		virtual void stereoSlot(bool);
		virtual void stopRecordSlot();
		virtual void surfaceDialogSlot();
		virtual void syncViewSlot();
		virtual void transformObjectSlot();
		virtual void updateUIWithNewData();
		virtual void userInteractionSlot();
		virtual void surfaceAreaAndVolumeDialogSlot();
		virtual void contentsSlot();
		virtual void resultSelectedInDockingSlot(int);
		virtual void resultUILaunchedInDockingSlot();
		virtual void vizSettingsChangedSlot();
		virtual void displayResultingSurface();

	protected:
		bool getSelected(BallAndStickData* & ballAndStickData, PDBParser::GroupOfAtoms* & molecule);
		bool getSelected(SurfaceData* & surfaceData, Geometry* & surface);
		//bool getSelected(BallAndStickData* ballAndStickData, Volume* volume);

		void setPreviousSavedEnvironment();
		void populateStatusBar();
		
		void initializeMainWindowMenubar();
		void initializeVariables();
		void initializeMainWindowStructures();
		
		// initialize Signal-Slot connections
		void initializeFileMenu();
		void initializeViewMenu();
		void initializeAnimationMenu();
		void initializeOptionsMenu();
		void initializeUtilitiesMenu();
		void initializeHelpMenu();

		// UI functions
		void updateListBox();
		void createLightsMenu();
		bool enableFirstLight();

		void setChildrenVisible(bool mode);
		void setRendererFullScreen(bool mode);
		void keyPressEvent(QKeyEvent* event);
		void keyReleaseEvent(QKeyEvent* event);
		void closeEvent(QCloseEvent* event);
		void debugPrint();
		void buildMovie();
		void updateViewingOptions(moleculeVisData *mv);

		enum POPUPSELECTION {SAVE_DATASET,DELETE_DATASET};
		POPUPSELECTION showPopup(QPoint point);
		void customEvent(QEvent* event);
		// mainwindow ui
		QWidget* _centralWidget;
		Ui::MainWindowBase *_ui;
		
		//*****************************
		//	Menu Actions
		//*****************************
		QAction *m_Action_FileOpen;
		QAction *m_Action_FileOpenMulti;
		QAction *m_Action_DownloadPDB;
		QAction *m_Action_SaveImage;
		QAction *m_Action_SaveTiledImage;
    		QAction *m_Action_Exit;
    		QAction *m_Action_RecentFileListDummy;
    		QAction *m_Action_OrthographicView;
    		QAction *m_Action_PerspectiveView;
  		QAction *m_Action_SetViewParameters;
    		QAction *m_Action_SplitView;
    		QAction *m_Action_SyncView;
    		QAction *m_Action_FullScreen;
    		QAction *m_Action_Stereo;
    		QAction *m_Action_ForceMeshRender;
    		QAction *m_Action_TransformObject;
    		QAction *m_Action_StartRecording;
    		QAction *m_Action_StartPerFrameRecording;
    		QAction *m_Action_RecordCurrentFrame;
    		QAction *m_Action_StopRecording;
    		QAction *m_Action_PlaybackAnimation;
    		QAction *m_Action_RecordAnimation;
    		QAction *m_Action_RecordStereoAnimation;
    		QAction *m_Action_RecordStereoAnimationX30deg2sec;
    		QAction *m_Action_RecordStereoAnimationX30deg4sec;
    		QAction *m_Action_RecordStereoAnimationX30deg8sec;
    		QAction *m_Action_RecordStereoAnimationX60deg4sec;
    		QAction *m_Action_RecordStereoAnimationX60deg8sec;
    		QAction *m_Action_RecordStereoAnimationX60deg16sec;
    		QAction *m_Action_RecordStereoAnimationX90deg6sec;
    		QAction *m_Action_RecordStereoAnimationX90deg12sec;
    		QAction *m_Action_RecordStereoAnimationX90deg24sec;
    		QAction *m_Action_RecordStereoAnimationY30deg2sec;
    		QAction *m_Action_RecordStereoAnimationY30deg4sec;
    		QAction *m_Action_RecordStereoAnimationY30deg8sec;
    		QAction *m_Action_RecordStereoAnimationY60deg4sec;
    		QAction *m_Action_RecordStereoAnimationY60deg8sec;
    		QAction *m_Action_RecordStereoAnimationY60deg16sec;
    		QAction *m_Action_RecordStereoAnimationY90deg6sec;
    		QAction *m_Action_RecordStereoAnimationY90deg12sec;
    		QAction *m_Action_RecordStereoAnimationY90deg24sec;
    		QAction *m_Action_RecordStereoAnimationZ30deg2sec;
    		QAction *m_Action_RecordStereoAnimationZ30deg4sec;
    		QAction *m_Action_RecordStereoAnimationZ30deg8sec;
    		QAction *m_Action_RecordStereoAnimationZ60deg4sec;
    		QAction *m_Action_RecordStereoAnimationZ60deg8sec;
    		QAction *m_Action_RecordStereoAnimationZ60deg16sec;
    		QAction *m_Action_RecordStereoAnimationZ90deg6sec;
    		QAction *m_Action_RecordStereoAnimationZ90deg12sec;
    		QAction *m_Action_RecordStereoAnimationZ90deg24sec;
    		QAction *m_Action_PlaybackMovie;
    		QAction *m_Action_RayTrace;
    		QAction *m_Action_BackgroundColor;
    		QAction *m_Action_DisplayGrid;
   		QAction *m_Action_MouseKeyboardFunc;
    		QAction *m_Action_SelectObjects;
    		QAction *m_Action_GlobalBoundingBox;
    		QAction *m_Action_DataBoundingBox;
   		QAction *m_Action_ShowViewInformation;
	 	QAction *m_Action_ShowMousePosition;
    		QAction *m_Action_Script;
		QAction *m_Action_ConstructSurface;
    		QAction *m_Action_ConstructNURBSSurface;
    		QAction *m_Action_SurfaceAreaAndVolume;
   		QAction *m_Action_ConstructVolume;
    		QAction *m_Action_ConstructDepthColoredVolume;
    		QAction *m_Action_ConstructPockets;
    		QAction *m_Action_ConstructPocketTunnelStableManifold;
    		QAction *m_Action_ConstructHLSPockets;
    		QAction *m_Action_GetCurvature;
    		QAction *m_Action_FormMatch;
    		QAction *m_Action_ComputeEnergyGB;
    		QAction *m_Action_ComputeForceFieldGB;
    		QAction *m_Action_ComputeEnergyPotentialPB;
    		QAction *m_Action_Get2DSlice;
    		QAction *m_Action_ElucidateSecondaryStructure;
    		QAction *m_Action_F2Dock;
    		QAction *m_Action_Contents;
    		QAction *m_Action_Index;
    		QAction *m_Action_Acknowledgements;

		QMenu *menuFile;
    		QMenu *m_Action_RecentFileListMenu;
    		QMenu *menuView;
    		QMenu *menuProjection;
    		QMenu *menuPresetStereoRecord;
		QMenu *menuPresetStereoRecordX;
		QMenu *menuPresetStereoRecordY;
		QMenu *menuPresetStereoRecordZ;
    		QMenu *menuAnimation;
    		QMenu *menuOptions;
    		QMenu *m_Menu_LightingPopup;
		QList<QAction*> m_LightActions;
    		QMenu *menuUtilities;
    		QMenu *menuHelp;
		//*****************************
		
		Q3Http* http;
		QFile* http_file;
//		Q3BoxLayout* m_QBoxLayout;
		QGridLayout* m_QBoxLayout;
		DataManager* m_DataManager;
		QWidget* m_PropertyWidget;
		RendererSet* m_RendererSet;
		Documentation* m_Documentation;
		Settings* m_Settings;
		ScriptsDialog* m_ScriptsDialog;
		Server* m_Server;
		QLabel* m_ViewInformationLabel;
		QLabel* m_MouseInformationLabel;
		QLabel* m_SelectionInformationLabel;
		bool m_FullScreen;
		bool m_SyncView;
		MouseHandler* m_MouseHandler;
		LightSet* m_LightSet;
		QSProject* m_QSProject;

		vector<double *> transformations;
		vector<int> result_indices;
		int numSelectedResults;

		string receptorName;
		string ligandName;

		moleculeVisData* receptorData;
		moleculeVisData* ligandData;

		PDBParser::GroupOfAtoms* ligandMolecule;
		PDBParser::GroupOfAtoms* receptorMolecule;
		Geometry* ligandSurface;
		Geometry* receptorSurface;
		SurfaceData *ligandSurfaceData;
		SurfaceData *receptorSurfaceData;
		BallAndStickData *ligandUofBData;
		BallAndStickData *receptorUofBData;
		bool receptorRendered;
		bool surfaceGenerated;
		bool PBGenerated;
		bool GBGenerated;
		bool CoulGenerated;
		QStringList dockingReceptorList;
		QStringList dockingOutputList;


		int m_F2DockReceptorIndex;
		int m_F2DockLigandIndex;
		int m_F2DockRendererId;
		int m_F2DockReceptorSurfaceIndex;
		int m_F2DockLigandSurfaceIndex;
		QString m_ReceptorXYZ;
		QString m_LigandXYZ;
		QString m_ReceptorRAW;
		QString m_LigandRAW;

		// FUTURE: make these vectors to
		//         manage multiple requests.
		QFuture<Geometry *> * surfaceFuture;
		QFutureWatcher<Geometry *> * surfaceWatcher;
		QString surfaceFutureName;


		vector<SurfaceBackgroundJob> surfaceJobs;


};

