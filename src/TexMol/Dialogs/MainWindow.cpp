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
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <qtextstream.h>

//
// arand, 6-7-11
// 
//   The order of the headers in this file is a mess...
//   and sometimes it is important to get the code to build.
//

// Qt3Support removed

#include <ImposterRenderer/ImposterRenderer.h>
#include <TexMol/Dialogs/MainWindow.h>

#include <Blurmaps/BlurMapsDataManager.h>

#include <DataManager/BallAndStickDataManager/BallAndStickData.h>

#ifdef WITH_CGAL
#include <DataManager/SecondaryStructureDataManager/SecondaryStructureData.h>
#endif

#include <DataManager/SurfaceDataManager/SurfaceData.h>
#include <DataManager/VolumeDataManager/VolumeData.h>


#include <Blurmaps/MolecularSurface.h>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include <DataManager/DataManager.h>

// Qt3 base removed
//#include <F2DockAnimator/F2DockAnimator.h>
#include <GeometryFileTypes/GeometryLoader.h>
#include <GeometryFileTypes/RawcFile.h>
#include <GeometryFileTypes/RawncFile.h>
#include <GOAFileTypes/GOALoader.h>
#include <HLevelSet/HLevelSet.h>
#include <PatchComplex/PatchComplex.h>
#include <Pocket/Pocket.h>


//Added by qt3to4:
#include <QBoxLayout>
#include <QCloseEvent>
#include <QKeyEvent>
// QCustomEvent removed — using QEvent
#include <QMenu>
#include <QFrame>
#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QColorDialog>

#include <QtConcurrent/QtConcurrent>




#include <SimpleVolumeData/SimpleVolumeData.h>
#include <SimpleVolumeData/SimpleVolumeDataIsocontourer.h>
#include <TexMol/Animator.h>
#include <TexMol/CallEvent.h>
#include <TexMol/Dialogs/ConstructCurvaturesDialog.h>
#include <TexMol/Dialogs/ConstructDepthColoredVolumesDialog.h>
#include <TexMol/Dialogs/ConstructVolumesDialog.h>
#include <TexMol/Dialogs/FileSaveDialog.h>
#include <TexMol/Dialogs/LightsDialog.h>
#include <TexMol/Dialogs/MovieImageFileSaveDialog.h>
#include <TexMol/Dialogs/GeneralizedBornDialog.h>
#include <TexMol/Dialogs/GeneralizedBornForceDialog.h>
#include <TexMol/Dialogs/PoissonBoltzmannDialog.h>
#include <TexMol/Dialogs/ScriptsDialog.h>
#include <TexMol/Dialogs/SetViewingParametersDialog.h>
#include <TexMol/Dialogs/SliceDialog.h>
#include <TexMol/Dialogs/SurfaceDialog.h>
#include <TexMol/Dialogs/SurfaceAreaAndVolumeDialog.h>
#include <TexMol/Dialogs/TransformationsDialog.h>
#include <TexMol/Documentation.h>
#include <TexMol/LightSet.h>
#include <TexMol/MouseHandlerHelper.h>
#include <TexMol/RendererSet.h>
#include <TexMol/Server.h>
#include <TexMol/Settings.h>
#include <Interface/clashFilter/clashFilter.h>
#include <Interface/pseudo-Gsol/pseudoGsol.h>
#include <TexMol/Dialogs/SaveTiledImageDialog.h>

#ifdef MSA
#include <MolSurfAPI/molsurf.h>
#endif

#ifdef WITH_PETSC
#include <PB2/PBLinear.h>
#endif

#ifdef WITH_NFFT
#include <GBEnergy/GenBornAPI.h>
#endif

#include <PDBParser/GroupOfAtoms.h>

// Include ordering dependencies
#include <PDBParser/GOAFileIO.h>

#ifdef CVC_USE_QSA
#include <qsproject.h>
#include <qsinterpreter.h>
#include <qsutilfactory.h>
#endif

#include <TexMol/Dialogs/AboutDialog.h>

#ifdef POCKET_TUNNEL
#include <PocketTunnel/pocket_tunnel.h>
#include <ui_PocketTunnelDialogBase.h>
#endif

#ifdef SS
#include <SecondaryStructures/skel.h>
#endif

#include <ui_MainWindowBase.h>


#ifndef TEXMOL_VERSION
// arand: lets call the Qt4 version with latest updates to be 
//        version 2.0...
#define TEXMOL_VERSION 2.0
#endif

extern float IsoValueofHLS;
extern float weight;
extern int iterationNum;
SimpleVolumeData* SimVolData;


//Craig: long lines are annoying, here is a function to pop up your error boxes!
void errorDialog(string bodyText)
{
	QMessageBox::warning((QWidget*)0, QString("Error"), QString(bodyText.c_str()), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
}


//Craig: will overload for volumes, molecules, and surfaces
//Returns two things by reference, success by value
bool MainWindow::getSelected(BallAndStickData* & ballAndStickData, PDBParser::GroupOfAtoms* & molecule)
{
	int dataSetType = m_DataManager->getCurrentDataSetType();
	int dataSetIndex = m_DataManager->getCurrentDataSetIndex();
	if(dataSetType == -1 || dataSetIndex == -1 || dataSetType != GOA)
	{
		errorDialog("A molecule was not selected");
		return false;
	}

	ballAndStickData = m_DataManager->getBallAndStickData(dataSetIndex);
	if(ballAndStickData == 0)
	{
		errorDialog("The molecule is empty");
		return false;
	}

	molecule = ballAndStickData->getData();

	if(molecule == 0)
	{
		errorDialog("A molecule was not selected");
		return false;
	}

	return true;
}

bool MainWindow::getSelected(SurfaceData* & surfaceData, Geometry* & surface)
{
	int dataSetType = m_DataManager->getCurrentDataSetType();
	int dataSetIndex = m_DataManager->getCurrentDataSetIndex();
	if(dataSetType == -1 || dataSetIndex == -1 || dataSetType != SURFACE)
	{
		errorDialog("A surface was not selected");
		return false;
	}


	surfaceData = m_DataManager->getSurfaceData(dataSetIndex);

	if(surfaceData == 0)
	{
		errorDialog("No surface loaded");
		return false;
	}

	surface = surfaceData->getGeometry();
	if(surface == 0)
	{
		errorDialog("No surface loaded");
		return false;
	}
	return true;
}


MainWindow::MainWindow(QWidget* parent, const char* name, Qt::WindowFlags f)
	: QMainWindow(parent, f), _ui(NULL)
{
	resize(824, 840);
	setMinimumSize(QSize(824, 0));
	
	//set mainwindow ui files
	_centralWidget = new QWidget;
  	setCentralWidget(_centralWidget);
  	_ui = new Ui::MainWindowBase;
  	_ui->setupUi(_centralWidget);
	//******************************

	initializeMainWindowMenubar();

	initializeVariables();
	initializeMainWindowStructures();

	initializeFileMenu();
	initializeViewMenu();
	initializeAnimationMenu();
	initializeOptionsMenu();
	initializeUtilitiesMenu();
	initializeHelpMenu();
}

MainWindow::~MainWindow()
{
	if(m_MouseHandler) delete m_MouseHandler;
	m_MouseHandler = 0;
	if(m_Server) delete m_Server;
	if(m_ScriptsDialog) delete m_ScriptsDialog;
	m_ScriptsDialog = 0;
	if(m_Documentation) delete m_Documentation;
	m_Documentation = 0;
	if(m_QBoxLayout) delete m_QBoxLayout;
	m_QBoxLayout = 0;
	if(m_RendererSet) delete m_RendererSet;
	m_RendererSet = 0;
	if(m_DataManager) delete m_DataManager;
	m_DataManager = 0;
	if(m_Settings) delete m_Settings;
	m_Settings = 0;
	if(m_LightSet) delete m_LightSet;
	m_LightSet = 0;
#ifdef CVC_USE_QSA
	if(m_QSProject) delete m_QSProject;
	m_QSProject = 0;
#endif // CVC_USE_QSA
}

void MainWindow::initializeMainWindowMenubar()
{
    menuFile = menuBar()->addMenu(tr("&File"));
    m_Action_RecentFileListMenu = new QMenu(menuFile);
   
    menuView = menuBar()->addMenu(tr("&View"));
   

    menuAnimation = menuBar()->addMenu(tr("&Animation"));
    menuOptions = menuBar()->addMenu(tr("&Options"));
    m_Menu_LightingPopup = menuBar()->addMenu(tr("&Lighting"));
    menuUtilities = menuBar()->addMenu(tr("&Utilities"));
    
    menuHelp = menuBar()->addMenu(tr("&Help"));
    menuHelp->setTearOffEnabled(true);

    menuBar()->addAction(menuFile->menuAction());
    menuBar()->addAction(menuView->menuAction());
    menuBar()->addAction(menuAnimation->menuAction());
    menuBar()->addAction(menuOptions->menuAction());
    menuBar()->addAction(m_Menu_LightingPopup->menuAction());
    menuBar()->addAction(menuUtilities->menuAction());
    menuBar()->addAction(menuHelp->menuAction());

    // Create Actions
    m_Action_FileOpen = new QAction(this);
    m_Action_FileOpen->setText("Open");

    m_Action_FileOpenMulti = new QAction(this);
    m_Action_FileOpenMulti->setText("Open Multi");

    m_Action_DownloadPDB = new QAction(this);
    m_Action_DownloadPDB->setText("DownloadPDB");

    m_Action_SaveImage = new QAction(this);
    m_Action_SaveImage->setText("Save Render Image");
    m_Action_SaveTiledImage = new QAction(this);
    m_Action_SaveTiledImage->setText("Save Tiled Image");

    m_Action_RecentFileListDummy = new QAction(this);
    m_Action_RecentFileListDummy->setText("No File");
    m_Action_RecentFileListDummy->setEnabled(false);
    
    m_Action_RecentFileListMenu = menuFile->addMenu(tr("&Recent Files"));
    m_Action_RecentFileListMenu->addAction(m_Action_RecentFileListDummy);

    m_Action_Exit = new QAction(this);
    m_Action_Exit->setText("Exit");

    menuFile->addAction(m_Action_FileOpen);
    menuFile->addAction(m_Action_FileOpenMulti);
    menuFile->addAction(m_Action_DownloadPDB);
    menuFile->addAction(m_Action_SaveImage);
    menuFile->addAction(m_Action_SaveTiledImage);
    menuFile->addAction(m_Action_RecentFileListMenu->menuAction());
    menuFile->addSeparator();
    menuFile->addAction(m_Action_Exit);
    
    m_Action_OrthographicView = new QAction(this);
    m_Action_OrthographicView->setText("Orthographic");
    m_Action_OrthographicView->setCheckable(true);
    m_Action_PerspectiveView = new QAction(this);
    m_Action_PerspectiveView->setText("Perspective");
    m_Action_PerspectiveView->setCheckable(true);
    m_Action_PerspectiveView->setChecked(true);
    m_Action_SetViewParameters = new QAction(this);

    menuProjection = menuView->addMenu(tr("&Projection"));
    menuProjection->addAction(m_Action_OrthographicView);
    menuProjection->addAction(m_Action_PerspectiveView);

    m_Action_SetViewParameters->setText("Set View Parameters");
    //m_Action_SplitView = new QAction(this);
    //m_Action_SplitView->setText("Split View");
    //m_Action_SyncView = new QAction(this);
    //m_Action_SyncView->setText("Sync View");
    //m_Action_SyncView->setCheckable(true);
    m_Action_FullScreen = new QAction(this);
    m_Action_FullScreen->setText("Full Screen (Esc for exit)");
    m_Action_Stereo = new QAction(this);
    m_Action_Stereo->setText("Stereo");
    m_Action_Stereo->setCheckable(true);
    m_Action_ForceMeshRender = new QAction(this);
    m_Action_ForceMeshRender->setText("Force Mesh Render");
    m_Action_ForceMeshRender->setCheckable(true);
    
    menuView->addAction(menuProjection->menuAction());
    menuView->addAction(m_Action_SetViewParameters);
    //menuView->addSeparator();
    //menuView->addAction(m_Action_SplitView);
    //menuView->addAction(m_Action_SyncView);
    menuView->addSeparator();
    menuView->addAction(m_Action_FullScreen);
    menuView->addAction(m_Action_Stereo);
    menuView->addAction(m_Action_ForceMeshRender);

    m_Action_StartRecording = new QAction(this);
    m_Action_StartRecording->setText("Start Recording");
    m_Action_StartPerFrameRecording = new QAction(this);
    m_Action_StartPerFrameRecording->setText("Start Per Frame Recording");
    m_Action_StartPerFrameRecording->setCheckable(true);
    m_Action_StartPerFrameRecording->setChecked(false);
    m_Action_RecordCurrentFrame = new QAction(this);
    m_Action_RecordCurrentFrame->setText("Record Current Frame");
    m_Action_StopRecording = new QAction(this);
    m_Action_StopRecording->setText("Stop Recording");
    m_Action_PlaybackAnimation = new QAction(this);
    m_Action_PlaybackAnimation->setText("Playback Animation");
    m_Action_RecordAnimation = new QAction(this);
    m_Action_RecordAnimation->setText("Record Animation");
    m_Action_RecordStereoAnimation = new QAction(this);
    m_Action_RecordStereoAnimation->setText("Record Stereo Animation");
    //------------------------------------------------------------
    m_Action_RecordStereoAnimationX30deg2sec = new QAction(this);
    m_Action_RecordStereoAnimationX30deg2sec->setText("30deg. over 4sec.");
    m_Action_RecordStereoAnimationX30deg4sec = new QAction(this);
    m_Action_RecordStereoAnimationX30deg4sec->setText("30deg. over 4sec.");
    m_Action_RecordStereoAnimationX30deg8sec = new QAction(this);
    m_Action_RecordStereoAnimationX30deg8sec->setText("30deg. over 8sec.");
    m_Action_RecordStereoAnimationX60deg4sec = new QAction(this);
    m_Action_RecordStereoAnimationX60deg4sec->setText("60deg. over 4sec.");
    m_Action_RecordStereoAnimationX60deg8sec = new QAction(this);
    m_Action_RecordStereoAnimationX60deg8sec->setText("60deg. over 8sec.");
    m_Action_RecordStereoAnimationX60deg16sec = new QAction(this);
    m_Action_RecordStereoAnimationX60deg16sec->setText("60deg. over 16sec.");
    m_Action_RecordStereoAnimationX90deg6sec = new QAction(this);
    m_Action_RecordStereoAnimationX90deg6sec->setText("90deg. over 6sec.");
    m_Action_RecordStereoAnimationX90deg12sec = new QAction(this);
    m_Action_RecordStereoAnimationX90deg12sec->setText("90deg. over 12sec.");
    m_Action_RecordStereoAnimationX90deg24sec = new QAction(this);
    m_Action_RecordStereoAnimationX90deg24sec->setText("90deg. over 24sec.");

    m_Action_RecordStereoAnimationY30deg2sec = new QAction(this);
    m_Action_RecordStereoAnimationY30deg2sec->setText("30deg. over 4sec.");
    m_Action_RecordStereoAnimationY30deg4sec = new QAction(this);
    m_Action_RecordStereoAnimationY30deg4sec->setText("30deg. over 4sec.");
    m_Action_RecordStereoAnimationY30deg8sec = new QAction(this);
    m_Action_RecordStereoAnimationY30deg8sec->setText("30deg. over 8sec.");
    m_Action_RecordStereoAnimationY60deg4sec = new QAction(this);
    m_Action_RecordStereoAnimationY60deg4sec->setText("60deg. over 4sec.");
    m_Action_RecordStereoAnimationY60deg8sec = new QAction(this);
    m_Action_RecordStereoAnimationY60deg8sec->setText("60deg. over 8sec.");
    m_Action_RecordStereoAnimationY60deg16sec = new QAction(this);
    m_Action_RecordStereoAnimationY60deg16sec->setText("60deg. over 16sec.");
    m_Action_RecordStereoAnimationY90deg6sec = new QAction(this);
    m_Action_RecordStereoAnimationY90deg6sec->setText("90deg. over 6sec.");
    m_Action_RecordStereoAnimationY90deg12sec = new QAction(this);
    m_Action_RecordStereoAnimationY90deg12sec->setText("90deg. over 12sec.");
    m_Action_RecordStereoAnimationY90deg24sec = new QAction(this);
    m_Action_RecordStereoAnimationY90deg24sec->setText("90deg. over 24sec.");

    m_Action_RecordStereoAnimationZ30deg2sec = new QAction(this);
    m_Action_RecordStereoAnimationZ30deg2sec->setText("30deg. over 4sec.");
    m_Action_RecordStereoAnimationZ30deg4sec = new QAction(this);
    m_Action_RecordStereoAnimationZ30deg4sec->setText("30deg. over 4sec.");
    m_Action_RecordStereoAnimationZ30deg8sec = new QAction(this);
    m_Action_RecordStereoAnimationZ30deg8sec->setText("30deg. over 8sec.");
    m_Action_RecordStereoAnimationZ60deg4sec = new QAction(this);
    m_Action_RecordStereoAnimationZ60deg4sec->setText("60deg. over 4sec.");
    m_Action_RecordStereoAnimationZ60deg8sec = new QAction(this);
    m_Action_RecordStereoAnimationZ60deg8sec->setText("60deg. over 8sec.");
    m_Action_RecordStereoAnimationZ60deg16sec = new QAction(this);
    m_Action_RecordStereoAnimationZ60deg16sec->setText("60deg. over 16sec.");
    m_Action_RecordStereoAnimationZ90deg6sec = new QAction(this);
    m_Action_RecordStereoAnimationZ90deg6sec->setText("90deg. over 6sec.");
    m_Action_RecordStereoAnimationZ90deg12sec = new QAction(this);
    m_Action_RecordStereoAnimationZ90deg12sec->setText("90deg. over 12sec.");
    m_Action_RecordStereoAnimationZ90deg24sec = new QAction(this);
    m_Action_RecordStereoAnimationZ90deg24sec->setText("90deg. over 24sec.");

    //------------------------------------------------------------
    //m_Action_PlaybackMovie = new QAction(this);
    //m_Action_PlaybackMovie->setText("Playback Movie");
    //m_Action_RayTrace = new QAction(this);
    //m_Action_RayTrace->setText("Ray Trace");
    //m_Action_RayTrace->setCheckable(true);
    m_Action_BackgroundColor = new QAction(this);
    m_Action_BackgroundColor->setText("Background Color");
    m_Action_DisplayGrid = new QAction(this);
    m_Action_DisplayGrid->setText("Display Grid");
    m_Action_DisplayGrid->setCheckable(true);
    m_Action_DisplayGrid->setChecked(true);
    m_Action_MouseKeyboardFunc = new QAction(this);
    m_Action_MouseKeyboardFunc->setText("Mouse-Keyboard Function");
    m_Action_TransformObject = new QAction(this);
    m_Action_TransformObject->setText("Transform Object");
    m_Action_TransformObject->setEnabled(false);
    m_Action_SelectObjects = new QAction(this);
    m_Action_SelectObjects->setText("Select Objects");
    m_Action_SelectObjects->setCheckable(true);
    //m_Action_GlobalBoundingBox = new QAction(this);
    //m_Action_GlobalBoundingBox->setText("Global Bounding Box");
    //m_Action_GlobalBoundingBox->setCheckable(true);
    //m_Action_GlobalBoundingBox->setChecked(false);
    //m_Action_DataBoundingBox = new QAction(this);
    //m_Action_DataBoundingBox->setText("Data Bounding Box");
    //m_Action_DataBoundingBox->setCheckable(true);
    m_Action_ShowViewInformation = new QAction(this);
    m_Action_ShowViewInformation->setText("Show View Information");
    m_Action_ShowViewInformation->setCheckable(true);
    m_Action_ShowViewInformation->setChecked(true);
    m_Action_ShowMousePosition = new QAction(this);
    m_Action_ShowMousePosition->setText("Show Mouse Position");
    m_Action_ShowMousePosition->setCheckable(true);
    m_Action_ShowMousePosition->setChecked(true);

    m_Action_Script = new QAction(this);
    m_Action_Script->setText("Script");
    m_Action_ConstructSurface = new QAction(this);
    m_Action_ConstructSurface->setText("Construct Surface");
    //m_Action_ConstructNURBSSurface = new QAction(this);
    //m_Action_ConstructNURBSSurface->setText("Construct NURBS Surface");
    m_Action_SurfaceAreaAndVolume = new QAction(this);
    m_Action_SurfaceAreaAndVolume->setText("Surface Area and Volume");
    m_Action_ConstructVolume = new QAction(this);
    m_Action_ConstructVolume->setText("Construct Volume");
    m_Action_ConstructDepthColoredVolume = new QAction(this);
    m_Action_ConstructDepthColoredVolume->setText("Construct Depth-Colored Volume");
    m_Action_ConstructPockets = new QAction(this);
    m_Action_ConstructPockets->setText("Construct Pockets");
    m_Action_ConstructPocketTunnelStableManifold = new QAction(this);
    m_Action_ConstructPocketTunnelStableManifold->setText("Construct Pocket-Tunnel by Stable Manifold");
    m_Action_ConstructHLSPockets = new QAction(this);
    m_Action_ConstructHLSPockets->setText("Construct HLS Pockets");
    m_Action_GetCurvature = new QAction(this);
    m_Action_GetCurvature->setText("Get Curvatures");
    //m_Action_FormMatch = new QAction(this);
    //m_Action_FormMatch->setText("Form a (MACT) Match");
    m_Action_ComputeEnergyGB = new QAction(this);
    m_Action_ComputeEnergyGB->setText("Compute Energy (GB)");
    m_Action_ComputeForceFieldGB = new QAction(this);
    m_Action_ComputeForceFieldGB->setText("Compute Force Field (GB)");
    m_Action_ComputeEnergyPotentialPB = new QAction(this);
    m_Action_ComputeEnergyPotentialPB->setText("Compute Energy+Potential (PB)");
    //m_Action_Get2DSlice = new QAction(this);
    //m_Action_Get2DSlice->setText("Get2DSlice");
    m_Action_ElucidateSecondaryStructure = new QAction(this);
    m_Action_ElucidateSecondaryStructure->setText("Elucidate Secondary Structures");
    m_Action_F2Dock = new QAction(this);
    m_Action_F2Dock->setText("F2Dock");

    m_Action_Contents = new QAction(this);
    m_Action_Contents->setText("Contents");
    m_Action_Index = new QAction(this);
    m_Action_Index->setText("Index");
    m_Action_Acknowledgements = new QAction(this);
    m_Action_Acknowledgements->setText("Acknowledgements");

    menuAnimation->addAction(m_Action_StartRecording);
    menuAnimation->addAction(m_Action_StartPerFrameRecording);
    menuAnimation->addAction(m_Action_RecordCurrentFrame);
    menuAnimation->addAction(m_Action_StopRecording);
    menuAnimation->addAction(m_Action_PlaybackAnimation);
    menuAnimation->addSeparator();
    menuAnimation->addAction(m_Action_RecordAnimation);
    menuAnimation->addAction(m_Action_RecordStereoAnimation);
    //------------------------------------------------------------
    menuPresetStereoRecord = menuAnimation->addMenu(tr("&Record Stereo Preset"));
    menuPresetStereoRecordX = menuPresetStereoRecord->addMenu(tr("&X-Roll"));
    menuPresetStereoRecordY = menuPresetStereoRecord->addMenu(tr("&Y-Roll"));
    menuPresetStereoRecordZ = menuPresetStereoRecord->addMenu(tr("&Z-Roll"));

    menuPresetStereoRecordX->addAction(m_Action_RecordStereoAnimationX30deg2sec);
    menuPresetStereoRecordX->addAction(m_Action_RecordStereoAnimationX30deg4sec);
    menuPresetStereoRecordX->addAction(m_Action_RecordStereoAnimationX30deg8sec);
    menuPresetStereoRecordX->addSeparator();
    menuPresetStereoRecordX->addAction(m_Action_RecordStereoAnimationX60deg4sec);
    menuPresetStereoRecordX->addAction(m_Action_RecordStereoAnimationX60deg8sec);
    menuPresetStereoRecordX->addAction(m_Action_RecordStereoAnimationX60deg16sec);
    menuPresetStereoRecordX->addSeparator();
    menuPresetStereoRecordX->addAction(m_Action_RecordStereoAnimationX90deg6sec);
    menuPresetStereoRecordX->addAction(m_Action_RecordStereoAnimationX90deg12sec);
    menuPresetStereoRecordX->addAction(m_Action_RecordStereoAnimationX90deg24sec);

    menuPresetStereoRecordY->addAction(m_Action_RecordStereoAnimationY30deg2sec);
    menuPresetStereoRecordY->addAction(m_Action_RecordStereoAnimationY30deg4sec);
    menuPresetStereoRecordY->addAction(m_Action_RecordStereoAnimationY30deg8sec);
    menuPresetStereoRecordY->addSeparator();
    menuPresetStereoRecordY->addAction(m_Action_RecordStereoAnimationY60deg4sec);
    menuPresetStereoRecordY->addAction(m_Action_RecordStereoAnimationY60deg8sec);
    menuPresetStereoRecordY->addAction(m_Action_RecordStereoAnimationY60deg16sec);
    menuPresetStereoRecordY->addSeparator();
    menuPresetStereoRecordY->addAction(m_Action_RecordStereoAnimationY90deg6sec);
    menuPresetStereoRecordY->addAction(m_Action_RecordStereoAnimationY90deg12sec);
    menuPresetStereoRecordY->addAction(m_Action_RecordStereoAnimationY90deg24sec);

    menuPresetStereoRecordZ->addAction(m_Action_RecordStereoAnimationZ30deg2sec);
    menuPresetStereoRecordZ->addAction(m_Action_RecordStereoAnimationZ30deg4sec);
    menuPresetStereoRecordZ->addAction(m_Action_RecordStereoAnimationZ30deg8sec);
    menuPresetStereoRecordZ->addSeparator();
    menuPresetStereoRecordZ->addAction(m_Action_RecordStereoAnimationZ60deg4sec);
    menuPresetStereoRecordZ->addAction(m_Action_RecordStereoAnimationZ60deg8sec);
    menuPresetStereoRecordZ->addAction(m_Action_RecordStereoAnimationZ60deg16sec);
    menuPresetStereoRecordZ->addSeparator();
    menuPresetStereoRecordZ->addAction(m_Action_RecordStereoAnimationZ90deg6sec);
    menuPresetStereoRecordZ->addAction(m_Action_RecordStereoAnimationZ90deg12sec);
    menuPresetStereoRecordZ->addAction(m_Action_RecordStereoAnimationZ90deg24sec);

    //------------------------------------------------------------
    //menuAnimation->addSeparator();
    //menuAnimation->addAction(m_Action_PlaybackMovie);
    //menuOptions->addAction(m_Action_RayTrace);
    menuOptions->addAction(m_Action_BackgroundColor);
    menuOptions->addAction(m_Action_DisplayGrid);
    menuOptions->addSeparator();
    menuOptions->addAction(m_Action_MouseKeyboardFunc);
    menuOptions->addAction(m_Action_TransformObject);
    menuOptions->addAction(m_Action_SelectObjects);
    //menuOptions->addSeparator();
    //menuOptions->addAction(m_Action_GlobalBoundingBox);
    //menuOptions->addAction(m_Action_DataBoundingBox);
    menuOptions->addSeparator();
    menuOptions->addAction(m_Action_ShowViewInformation);
    menuOptions->addAction(m_Action_ShowMousePosition);
    menuUtilities->addAction(m_Action_Script);
    menuUtilities->addSeparator();
    menuUtilities->addAction(m_Action_ConstructSurface);
    //menuUtilities->addAction(m_Action_ConstructNURBSSurface);
    menuUtilities->addAction(m_Action_SurfaceAreaAndVolume);
    menuUtilities->addSeparator();
    menuUtilities->addAction(m_Action_ConstructVolume);
    menuUtilities->addAction(m_Action_ConstructDepthColoredVolume);
    menuUtilities->addSeparator();
    menuUtilities->addAction(m_Action_ConstructPockets);
    menuUtilities->addAction(m_Action_ConstructPocketTunnelStableManifold);
    menuUtilities->addAction(m_Action_ConstructHLSPockets);
    menuUtilities->addSeparator();
    menuUtilities->addAction(m_Action_GetCurvature);
    //menuUtilities->addAction(m_Action_FormMatch);
    menuUtilities->addAction(m_Action_ComputeEnergyGB);
    menuUtilities->addAction(m_Action_ComputeForceFieldGB);
    menuUtilities->addAction(m_Action_ComputeEnergyPotentialPB);
    menuUtilities->addSeparator();
    //menuUtilities->addAction(m_Action_Get2DSlice);
    menuUtilities->addAction(m_Action_ElucidateSecondaryStructure);
    menuUtilities->addAction(m_Action_F2Dock);
    menuHelp->addAction(m_Action_Contents);
    menuHelp->addAction(m_Action_Index);
    menuHelp->addSeparator();
    menuHelp->addAction(m_Action_Acknowledgements);
}

void MainWindow::initializeVariables()
{
	m_Settings	= 0;
	m_DataManager	= 0;
	m_RendererSet	= 0;
	m_QBoxLayout	= 0;
	m_Documentation = 0;
	m_FullScreen	= false;
	m_SyncView	= false;
	
	//setWindowTitle("TexMol 1.04 Alpha");
	setWindowTitle("TexMol 2.0a");

	m_F2DockReceptorIndex		= -1;
	m_F2DockLigandIndex		= -1;
	m_F2DockRendererId		= -1;
	m_F2DockReceptorSurfaceIndex	= -1;
	m_F2DockLigandSurfaceIndex	= -1;

	receptorName.assign(" ");
	ligandName.assign(" ");
	numSelectedResults = 0;
	transformations.clear();
	result_indices.clear();
	surfaceGenerated = false;
	PBGenerated = false;
	GBGenerated = false;
	CoulGenerated = false;
}

void MainWindow::populateStatusBar()
{
	// Add widgets to status bar to display:
	// 1. View information
	// 2. Temp messages
	QStatusBar* mainStatusBar = statusBar();
	m_ViewInformationLabel = new QLabel(mainStatusBar);
	m_ViewInformationLabel->setText("0,0,0");
	mainStatusBar->addPermanentWidget(m_ViewInformationLabel);
	m_MouseInformationLabel = new QLabel(mainStatusBar);
	m_MouseInformationLabel->setText("X, Y");
	mainStatusBar->addPermanentWidget(m_MouseInformationLabel);
	
	//3. Selection
	m_SelectionInformationLabel = new QLabel(mainStatusBar);
	m_SelectionInformationLabel->setText("[No Selection]");
	mainStatusBar->addPermanentWidget(m_SelectionInformationLabel);

	this->setStatusBar(mainStatusBar);
}

void MainWindow::initializeMainWindowStructures()
{
	m_Settings = new Settings();
 	setPreviousSavedEnvironment(); // from m_Settings

	populateStatusBar();

#ifdef CVC_USE_QSA
	m_QSProject = new QSProject(this,"TexMolScriptProject");
	m_QSProject->interpreter()->addObjectFactory(new QSUtilFactory);
#endif // CVC_USE_QSA
	m_MouseHandler = new MouseHandler((QWidget*)this);
	// SKVINAY change the boolean values to actually read from menu items, or set the menu items here.
	// otherwise this is not possibly consistant.
	m_LightSet = new LightSet();

	// allocate render set
	m_RendererSet = new RendererSet(true, m_ViewInformationLabel, m_MouseInformationLabel, m_SelectionInformationLabel, false);
	// allocate data manager
	m_DataManager = new DataManager(m_QSProject, _ui->m_PropertiesGroupBox, _ui->m_DataSetsListBox, false, false, m_RendererSet);

	_ui->m_SplitViewer->setFrameShape( QFrame::StyledPanel );

	m_RendererSet->addNewRenderer( this, _ui->m_SplitViewer, m_DataManager, m_Action_SyncView, true, m_MouseHandler, m_LightSet);

	// create a light menu and enable first one
	createLightsMenu();
	enableFirstLight();

	// set Propertiy Window
	m_QBoxLayout = new QGridLayout(_ui->m_PropertiesGroupBox);
	m_PropertyWidget = 0;
	// connect signal of right mouse button being clicked to some slot to bring up a popup menu.
	_ui->m_DataSetsListBox->clear();
// Qt3 signal removed: 	connect(_ui->m_DataSetsListBox, SIGNAL(rightButtonClicked(Q3ListBoxItem*, const QPoint&)), this, SLOT(dataSetSelectedRightMouse(Q3ListBoxItem*, const QPoint&)));
	connect(_ui->m_DataSetsListBox, SIGNAL(currentRowChanged(int)), this, SLOT(newDataSetSelectedSlot(int)));

	m_Documentation = new Documentation();
	m_ScriptsDialog = new ScriptsDialog(this);
// Qt3 removed: 	http = new Q3Http();
	http_file = 0;
// Qt3 removed: 	connect(http, SIGNAL(done(bool)), this, SLOT(finishedop(bool)));
// Qt3 removed: 	connect(http, SIGNAL(dataReadProgress(int, int)), this, SLOT(dataTransferProgressop(int, int)));

	m_Server = new Server();
	{
		GLboolean stereoMode[1];
		stereoMode[0] = GL_FALSE;
		glGetBooleanv(GL_STEREO, stereoMode);

		if(stereoMode[0] == GL_TRUE)
		{
			printf("Stereo works\n");
		}
	}
	m_RendererSet->setViewType(PERSPECTIVE_VIEW, m_SyncView); // default to perspective projection
}

void MainWindow::initializeFileMenu()
{
	connect(m_Action_FileOpen, SIGNAL(triggered()), SLOT(fileOpenSlot()));
	connect(m_Action_FileOpenMulti, SIGNAL(triggered()), SLOT(fileOpenMultiSlot()));
	connect(m_Action_DownloadPDB, SIGNAL(triggered()), SLOT(downloadPDBSlot()));
	connect(m_Action_SaveImage, SIGNAL(triggered()), SLOT(saveImageSlot()));
	connect(m_Action_SaveTiledImage, SIGNAL(triggered()), SLOT(saveTiledImageSlot()));
	connect(m_Action_Exit, SIGNAL(triggered()), SLOT(close()));
}

void MainWindow::initializeViewMenu()
{
	connect(m_Action_PerspectiveView, SIGNAL(triggered()), SLOT(perspectiveViewSlot()));
	connect(m_Action_OrthographicView, SIGNAL(triggered()), SLOT(orthographicViewSlot()));
	connect(m_Action_SetViewParameters, SIGNAL(triggered()), SLOT(setViewingParametersSlot()));
	//connect(m_Action_SplitView, SIGNAL(triggered()), SLOT(splitViewSlot()));
	//connect(m_Action_SyncView, SIGNAL(triggered()), SLOT(syncViewSlot()));
	connect(m_Action_FullScreen, SIGNAL(triggered()), SLOT(fullscreenSlot()));
	connect(m_Action_Stereo, SIGNAL(toggled(bool)), SLOT(stereoSlot(bool)));
	connect(m_Action_ForceMeshRender, SIGNAL(toggled(bool)), SLOT(forceMeshRenderingSlot(bool)));
}

void MainWindow::initializeAnimationMenu()
{
	connect(m_Action_StartRecording, SIGNAL(triggered()), SLOT(startRecordSlot()));
	connect(m_Action_StartPerFrameRecording, SIGNAL(triggered()), SLOT(startPerFrameRecordSlot()));
	connect(m_Action_RecordCurrentFrame, SIGNAL(triggered()), SLOT(recordCurrentFrameSlot()));
	connect(m_Action_StopRecording, SIGNAL(triggered()), SLOT(stopRecordSlot()));
	connect(m_Action_PlaybackAnimation, SIGNAL(triggered()), SLOT(playbackAnimationSlot()));
	connect(m_Action_RecordAnimation, SIGNAL(triggered()), SLOT(recordAnimationSlot()));
	connect(m_Action_RecordStereoAnimation, SIGNAL(triggered()), SLOT(recordStereoAnimationSlot()));

	connect(m_Action_RecordStereoAnimationX30deg2sec, SIGNAL(triggered()), SLOT(recordStereoPresetX30deg2secSlot()));
	connect(m_Action_RecordStereoAnimationX30deg4sec, SIGNAL(triggered()), SLOT(recordStereoPresetX30deg4secSlot()));
	connect(m_Action_RecordStereoAnimationX30deg8sec, SIGNAL(triggered()), SLOT(recordStereoPresetX30deg8secSlot()));
	connect(m_Action_RecordStereoAnimationX60deg4sec, SIGNAL(triggered()), SLOT(recordStereoPresetX60deg4secSlot()));
	connect(m_Action_RecordStereoAnimationX60deg8sec, SIGNAL(triggered()), SLOT(recordStereoPresetX60deg8secSlot()));
	connect(m_Action_RecordStereoAnimationX60deg16sec, SIGNAL(triggered()), SLOT(recordStereoPresetX60deg16secSlot()));
	connect(m_Action_RecordStereoAnimationX90deg6sec, SIGNAL(triggered()), SLOT(recordStereoPresetX90deg6secSlot()));
	connect(m_Action_RecordStereoAnimationX90deg12sec, SIGNAL(triggered()), SLOT(recordStereoPresetX90deg12secSlot()));
	connect(m_Action_RecordStereoAnimationX90deg24sec, SIGNAL(triggered()), SLOT(recordStereoPresetX90deg24secSlot()));
	connect(m_Action_RecordStereoAnimationY30deg2sec, SIGNAL(triggered()), SLOT(recordStereoPresetY30deg2secSlot()));
	connect(m_Action_RecordStereoAnimationY30deg4sec, SIGNAL(triggered()), SLOT(recordStereoPresetY30deg4secSlot()));
	connect(m_Action_RecordStereoAnimationY30deg8sec, SIGNAL(triggered()), SLOT(recordStereoPresetY30deg8secSlot()));
	connect(m_Action_RecordStereoAnimationY60deg4sec, SIGNAL(triggered()), SLOT(recordStereoPresetY60deg4secSlot()));
	connect(m_Action_RecordStereoAnimationY60deg8sec, SIGNAL(triggered()), SLOT(recordStereoPresetY60deg8secSlot()));
	connect(m_Action_RecordStereoAnimationY60deg16sec, SIGNAL(triggered()), SLOT(recordStereoPresetY60deg16secSlot()));
	connect(m_Action_RecordStereoAnimationY90deg6sec, SIGNAL(triggered()), SLOT(recordStereoPresetY90deg6secSlot()));
	connect(m_Action_RecordStereoAnimationY90deg12sec, SIGNAL(triggered()), SLOT(recordStereoPresetY90deg12secSlot()));
	connect(m_Action_RecordStereoAnimationY90deg24sec, SIGNAL(triggered()), SLOT(recordStereoPresetY90deg24secSlot()));
	connect(m_Action_RecordStereoAnimationZ30deg2sec, SIGNAL(triggered()), SLOT(recordStereoPresetZ30deg2secSlot()));
	connect(m_Action_RecordStereoAnimationZ30deg4sec, SIGNAL(triggered()), SLOT(recordStereoPresetZ30deg4secSlot()));
	connect(m_Action_RecordStereoAnimationZ30deg8sec, SIGNAL(triggered()), SLOT(recordStereoPresetZ30deg8secSlot()));
	connect(m_Action_RecordStereoAnimationZ60deg4sec, SIGNAL(triggered()), SLOT(recordStereoPresetZ60deg4secSlot()));
	connect(m_Action_RecordStereoAnimationZ60deg8sec, SIGNAL(triggered()), SLOT(recordStereoPresetZ60deg8secSlot()));
	connect(m_Action_RecordStereoAnimationZ60deg16sec, SIGNAL(triggered()), SLOT(recordStereoPresetZ60deg16secSlot()));
	connect(m_Action_RecordStereoAnimationZ90deg6sec, SIGNAL(triggered()), SLOT(recordStereoPresetZ90deg6secSlot()));
	connect(m_Action_RecordStereoAnimationZ90deg12sec, SIGNAL(triggered()), SLOT(recordStereoPresetZ90deg12secSlot()));
	connect(m_Action_RecordStereoAnimationZ90deg24sec, SIGNAL(triggered()), SLOT(recordStereoPresetZ90deg24secSlot()));
	//connect(m_Action_PlaybackMovie, SIGNAL(triggered()), SLOT(playbackMovieSlot()));
}

void MainWindow::initializeOptionsMenu()
{
        //connect(m_Action_RayTrace, SIGNAL(toggled(bool)), SLOT(renderTypeSlot(bool)));
	connect(m_Action_BackgroundColor, SIGNAL(triggered()), SLOT(backgroundColorSlot()));
	connect(m_Action_DisplayGrid, SIGNAL(toggled(bool)), SLOT(showGridOptionsSlot(bool)));
	connect(m_Action_MouseKeyboardFunc, SIGNAL(triggered()), SLOT(userInteractionSlot()));
	connect(m_Action_TransformObject, SIGNAL(triggered()), SLOT(transformObjectSlot()));
	connect(m_Action_SelectObjects, SIGNAL(toggled(bool)), SLOT(selectObjectsSlot(bool)));
	//connect(m_Action_GlobalBoundingBox, SIGNAL(toggled(bool)), SLOT(globalBoundingBoxSlot(bool)));
	//connect(m_Action_DataBoundingBox, SIGNAL(toggled(bool)), SLOT(dataBoundingBoxSlot(bool)));
	connect(m_Action_ShowViewInformation, SIGNAL(toggled(bool)), SLOT(showViewInformationSlot(bool)));
	connect(m_Action_ShowMousePosition, SIGNAL(toggled(bool)), SLOT(showMousePositionSlot(bool)));
}

void MainWindow::initializeUtilitiesMenu()
{
	connect(m_Action_Script, SIGNAL(triggered()), SLOT(scriptSlot()));
	connect(m_Action_ConstructSurface, SIGNAL(triggered()), SLOT(surfaceDialogSlot()));
	//connect(m_Action_ConstructNURBSSurface, SIGNAL(triggered()), SLOT(constructNurbsSurfaceSlot()));
	connect(m_Action_SurfaceAreaAndVolume, SIGNAL(triggered()), SLOT(surfaceAreaAndVolumeDialogSlot()));
	connect(m_Action_ConstructVolume, SIGNAL(triggered()), SLOT(constructVolumeUtilitiesSlot()));
	connect(m_Action_ConstructDepthColoredVolume, SIGNAL(triggered()), SLOT(constructDepthColoredVolumeSlot()));
	connect(m_Action_ConstructPockets, SIGNAL(triggered()), SLOT(constructPocketsSlot()));
	connect(m_Action_ConstructPocketTunnelStableManifold, SIGNAL(triggered()), SLOT(computePocketTunnelSlot()));
	connect(m_Action_ConstructHLSPockets, SIGNAL(triggered()), SLOT(constructHLSPocketsSlot()));
	connect(m_Action_GetCurvature, SIGNAL(triggered()), SLOT(constructCurvaturesUtilitySlot()));
	//connect(m_Action_FormMatch, SIGNAL(triggered()), SLOT(matchSlot()));
	connect(m_Action_ComputeEnergyGB, SIGNAL(triggered()), SLOT(GB_energy_Slot()));	
	connect(m_Action_ComputeForceFieldGB, SIGNAL(triggered()), SLOT(GB_forceField_Slot()));
	connect(m_Action_ComputeEnergyPotentialPB, SIGNAL(triggered()), SLOT(PB_energy_Slot()));
	//connect(m_Action_Get2DSlice, SIGNAL(triggered()), SLOT(sliceParameterizationSlot()));
	connect(m_Action_ElucidateSecondaryStructure, SIGNAL(triggered()), SLOT(elucidateSecondaryStructuresSlot()));
	connect(m_Action_F2Dock, SIGNAL(triggered()), SLOT(dockingSlot()));
}

void MainWindow::initializeHelpMenu()
{
	connect(m_Action_Contents, SIGNAL(triggered()), SLOT(contentsSlot()));
	connect(m_Action_Index, SIGNAL(triggered()), SLOT(indexHelpSlot()));
	connect(m_Action_Acknowledgements, SIGNAL(triggered()), SLOT(acknowledgementSlot()));
}

//*****************************************
//	Slot functions for File Menu
//*****************************************
QString getFilter(QString* dataTypes, int numberOfDataTypes);

bool MainWindow::fileOpenSlot()
{
	bool addedNewDataSet = true;
	QStringList fileNames;
	fileNames = QFileDialog::getOpenFileNames(this, "Open File", "", getFilter(m_DataManager->getDataTypes(), m_DataManager->getNumberOfDataTypes()));

	if(fileNames.count() > 0) {
		addedNewDataSet = addNewDataSet(fileNames);
	}

	if(addedNewDataSet) {
		return true;
	}

	errorDialog("Error adding new data.");
	return false;
}

bool MainWindow::fileOpenMultiSlot()
{
	bool addedNewDataSet = true;
	QStringList fileNames;
	fileNames = QFileDialog::getOpenFileNames(this, "Open File", "", getFilter(m_DataManager->getDataTypes(), m_DataManager->getNumberOfDataTypes()));
				

	if(fileNames.count() > 0)
	{
		addedNewDataSet = addNewDataSet(fileNames, true);
	}

	if(addedNewDataSet)
	{
		return true;
	}

	errorDialog("Error adding new data.");
	return false;
}

void MainWindow::downloadPDBSlot()
{
	// Disabled — requires DownloadPDBDialogBase and Q3Http (Qt3 networking)
	QMessageBox::information(this, "PDB Download", "PDB download is not yet available in this version.");
}

void MainWindow::saveImageSlot()
{
	FileSaveDialog* fsDialog = new FileSaveDialog(this);

	if(fsDialog->exec() == QDialog::Accepted)
	{
		bool saveAll = false;
		saveAll = fsDialog->getSaveAll();
		QString formatSelected;
		QString imageFileName;
		fsDialog->getSelectedFileName(&imageFileName, &formatSelected);

		if(!imageFileName.isNull())
		{
			m_RendererSet->saveImages(imageFileName,formatSelected, saveAll);
		}
	}

	delete fsDialog;
	fsDialog = 0;
}

void MainWindow::saveTiledImageSlot()
{
	SaveTiledImageDialog *stDialog = new SaveTiledImageDialog(this);

	if(stDialog->exec() == QDialog::Accepted)
	{
		bool saveAll = false;
                bool mergeToSingleImage = false;
		QString formatSelected;
		QString imageFileName;
		int tileResol, nTiles;
		stDialog->getParameters(&imageFileName, &formatSelected, &tileResol, &nTiles, &saveAll, &mergeToSingleImage);

		if(!imageFileName.isNull())
		{
			m_RendererSet->saveTiledImages(imageFileName,formatSelected, tileResol, nTiles, saveAll, mergeToSingleImage);
		}
	}

	delete stDialog;
}

//*****************************************
//	Slot functions for View Menu
//*****************************************
void MainWindow::perspectiveViewSlot()
{
	m_Action_OrthographicView->setChecked(false);
	m_RendererSet->setViewType(PERSPECTIVE_VIEW, m_SyncView);
}

void MainWindow::orthographicViewSlot()
{
	m_Action_PerspectiveView->setChecked(false);
	m_RendererSet->setViewType(ORTHOGRAPHIC_VIEW, m_SyncView);
}

// Obtain the translation (4), rotation (4) and zoom (1)
// values from the user using a dialog and set it to current renderers
void MainWindow::setViewingParametersSlot()
{
	if(!m_RendererSet)
	{
		return;
	}

	CCVOpenGLMath::Quaternion orientation;
	CCVOpenGLMath::Vector translation;
	float windowSize;
	m_RendererSet->getViewingParameters(&orientation, &translation, &windowSize);
	SetViewingParametersDialog* vpd = new SetViewingParametersDialog(orientation, translation, windowSize, this);

	if(vpd->exec() == QDialog::Accepted)
	{
		double translationParams[4];
		double rotationParams[4];
		double windowSize;

		if(!vpd->getParams(translationParams, rotationParams, &windowSize))
		{
			errorDialog("Error setting viewing parameters");
		}
		else
		{
			setViewingParameters(translationParams, rotationParams, windowSize);
		}
	}

	delete vpd;
	vpd = 0;
}

void MainWindow::syncViewSlot()
{
	m_SyncView = ! m_SyncView;
	m_RendererSet->syncView(m_SyncView);
	//if( m_SyncView ) m_RendererSet->update();
	m_RendererSet->update();
}

void MainWindow::splitViewSlot()
{
	m_RendererSet->splitView(0, this, _ui->m_SplitViewer, m_DataManager, m_SyncView, m_MouseHandler, m_LightSet);
	m_RendererSet->syncView(m_SyncView);
	//if( m_SyncView ) m_RendererSet->update();
	m_RendererSet->update();
}

void MainWindow::fullscreenSlot()
{
	// make opengl window the main screen, call make full screen.
	// Let this work only if here is only one renderer. Else we should
	// disable that option.
	if(!m_FullScreen)
	{
		if(m_RendererSet->getNumberOfRenderers() != 1)
		{
			return;
		}

		m_FullScreen = true;
		m_RendererSet->makeFullScreen(m_Action_FullScreen, (QWidget*)_ui->m_SplitViewer );
	}
	else
	{
		m_FullScreen = false;
		m_RendererSet->makeFullScreen(m_Action_FullScreen, (QWidget*)_ui->m_SplitViewer);
		showNormal();
	}

//	/* other lame way to do the same thing in a worse fashion. Maybe safer due to
//	   the flags.
//	if( ! m_FullScreen )
//	{
//		setChildrenVisible( false );
//		setRendererFullScreen( true );
//		m_FullScreen = true;
//	}
//	else
//	{
//		// uncheck menu item
//		setChildrenVisible( true );
//		setRendererFullScreen( false );
//		m_FullScreen = false;
//	}
}

void MainWindow::stereoSlot(bool useStereoMode)
{
	if(useStereoMode)
	{
		// check if stereo is supported in the constructor itself. So here assume it works.
		//if( !m_FullScreen ) fullscreenSlot();
	}

	m_RendererSet->makeStereo(useStereoMode);
}

void MainWindow::forceMeshRenderingSlot(bool status)
{
	CCVImposterRenderer::ImposterRenderer::forceMeshRendering(status);
}

//*********************************************
//	Slot functions for Animation Menu
//*********************************************

void MainWindow::startRecordSlot()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if(!fileName.isNull())
	{
		m_DataManager->m_RenderingMode = Animator::FLY_THROUGH_RECORDING_MODE;
		m_RendererSet->startRecordingViews(fileName.toLatin1().constData());
		// also update state of GUI components
		// enable stop, disable start, allow others to remain ?
		// should we also save loading, deleting data sets ?
	}
}

void MainWindow::startPerFrameRecordSlot()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if(!fileName.isNull()) {
		m_RendererSet->startRecordingViews(fileName.toLatin1().constData());
		m_Action_StartPerFrameRecording->setChecked(true);
	}
	else
		m_Action_StartPerFrameRecording->setChecked(false);
}

void MainWindow::recordCurrentFrameSlot()
{
	if( m_Action_StartPerFrameRecording->isChecked() ) {
		m_DataManager->m_RenderingMode = Animator::PER_FRAME_RECORDING_MODE;
		m_RendererSet->recordCurrentView();
		m_DataManager->m_RenderingMode = Animator::NORMAL_RENDERING_MODE;
	}
	else
		fprintf( stderr, "You need to check [Start Per Frame Recording] first\n");
}

void MainWindow::stopRecordSlot()
{
	m_DataManager->m_RenderingMode = Animator::NORMAL_RENDERING_MODE;
	m_RendererSet->stopRecordingViews();
	m_Action_StartPerFrameRecording->setChecked(false);
}

void MainWindow::playbackAnimationSlot()
{
	QString fileName = QFileDialog::getOpenFileName(this);
	if(!fileName.isNull())
	{
		m_RendererSet->playbackAnimation(fileName.toLatin1().constData());
	}
}

void MainWindow::recordAnimationSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);

	if(mifsDialog->exec() == QDialog::Accepted)
	{
		QString animationFileName;
		QString imageFileName;
		QString formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);

		if(!imageFileName.isNull())
		{
			if(m_DataManager->m_RenderingMode == Animator::FLY_THROUGH_RECORDING_MODE)
			{
				return;
			}

			m_DataManager->m_RenderingMode = Animator::ANIMATION_IMAGES_RECORDING_MODE;
			m_RendererSet->recordAnimation(animationFileName.toLatin1().constData(), imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData());
			m_DataManager->m_RenderingMode = Animator::NORMAL_RENDERING_MODE;
		}
	}

	delete mifsDialog;
	mifsDialog = 0;
}

void MainWindow::recordStereoAnimationSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);

	if(mifsDialog->exec() == QDialog::Accepted)
	{
		QString animationFileName;
		QString imageFileName;
		QString formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);

		if(!imageFileName.isNull())
		{
			if(m_DataManager->m_RenderingMode == Animator::FLY_THROUGH_RECORDING_MODE)
			{
				return;
			}

			m_DataManager->m_RenderingMode = Animator::ANIMATION_STEREO_IMAGES_RECORDING_MODE;
			m_RendererSet->recordAnimation(animationFileName.toLatin1().constData(), imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData());
			m_DataManager->m_RenderingMode = Animator::NORMAL_RENDERING_MODE;
		}
	}

	delete mifsDialog;
	mifsDialog = 0;
}

//----------------------------------------------------------------------------------------------------------------------------------
void MainWindow::recordStereoPresetX30deg2secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 30, 1, 0, 0, 2);
	}
}

void MainWindow::recordStereoPresetX30deg4secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 30, 1, 0, 0, 4);
	}
}

void MainWindow::recordStereoPresetX30deg8secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 30, 1, 0, 0, 8);
	}
}

void MainWindow::recordStereoPresetX60deg4secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 60, 1, 0, 0, 4);
	}
}

void MainWindow::recordStereoPresetX60deg8secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 60, 1, 0, 0, 8);
	}
}

void MainWindow::recordStereoPresetX60deg16secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 60, 1, 0, 0, 16);
	}
}

void MainWindow::recordStereoPresetX90deg6secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 90, 1, 0, 0, 6);
	}
}

void MainWindow::recordStereoPresetX90deg12secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 90, 1, 0, 0, 12);
	}
}

void MainWindow::recordStereoPresetX90deg24secSlot()
{	
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 90, 1, 0, 0, 24);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------
void MainWindow::recordStereoPresetY30deg2secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 30, 0, 1, 0, 2);
	}
}

void MainWindow::recordStereoPresetY30deg4secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 30, 0, 1, 0, 4);
	}
}

void MainWindow::recordStereoPresetY30deg8secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 30, 0, 1, 0, 8);
	}
}

void MainWindow::recordStereoPresetY60deg4secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 60, 0, 1, 0, 4);
	}
}

void MainWindow::recordStereoPresetY60deg8secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 60, 0, 1, 0, 8);
	}
}

void MainWindow::recordStereoPresetY60deg16secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 60, 0, 1, 0, 16);
	}
}

void MainWindow::recordStereoPresetY90deg6secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 90, 0, 1, 0, 6);
	}
}

void MainWindow::recordStereoPresetY90deg12secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 90, 0, 1, 0, 12);
	}
}

void MainWindow::recordStereoPresetY90deg24secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 90, 0, 1, 0, 24);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------
void MainWindow::recordStereoPresetZ30deg2secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 30, 0, 0, 1, 2);
	}
}

void MainWindow::recordStereoPresetZ30deg4secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 30, 0, 0, 1, 4);
	}
}

void MainWindow::recordStereoPresetZ30deg8secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 30, 0, 0, 1, 8);
	}
}

void MainWindow::recordStereoPresetZ60deg4secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 60, 0, 0, 1, 4);
	}
}

void MainWindow::recordStereoPresetZ60deg8secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 60, 0, 0, 1, 8);
	}
}

void MainWindow::recordStereoPresetZ60deg16secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 60, 0, 0, 1, 16);
	}
}

void MainWindow::recordStereoPresetZ90deg6secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 90, 0, 0, 1, 6);
	}
}

void MainWindow::recordStereoPresetZ90deg12secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 90, 0, 0, 1, 12);
	}
}

void MainWindow::recordStereoPresetZ90deg24secSlot()
{
	MovieImageFileSaveDialog* mifsDialog = new MovieImageFileSaveDialog(this);
	if(mifsDialog->exec() == QDialog::Accepted) {
		QString animationFileName, imageFileName, formatSelected;
		mifsDialog->getSelectedFileNames(&animationFileName, &imageFileName, &formatSelected);
		if(!imageFileName.isNull())
			m_RendererSet->recordAxisRotatedAnimation(imageFileName.toLatin1().constData(), formatSelected.toLatin1().constData(), 90, 0, 0, 1, 24);
	}
}
//----------------------------------------------------------------------------------------------------------------------------------



void MainWindow::playbackMovieSlot()
{
	errorDialog("Not yet implemented");
	//m_RendererSet->playbackMovie( animationDirName, imageBaseFileName, numberOfImages, fps );
}

//*********************************************
//	Slot functions for Options Menu
//*********************************************

void MainWindow::renderTypeSlot(bool useRayTracer)
{
	m_RendererSet->renderType(useRayTracer);
}

void MainWindow::backgroundColorSlot()
{
	QColor prevColor;
	m_RendererSet->getBackgroundColor(&prevColor);
	QColor newColor = QColorDialog::getColor(prevColor, this);

	if(newColor.isValid())
	{
		m_RendererSet->setBackgroundColor(newColor);
	}
}

void MainWindow::showGridOptionsSlot(bool displayGrid)
{
	m_RendererSet->showGrids(displayGrid);
}

void MainWindow::userInteractionSlot()
{
	if(!m_MouseHandler)
	{
		m_MouseHandler = new MouseHandler((QWidget*)this);
	}

	m_MouseHandler->updateUserPreferences();
}

//  This function only chnages the position of a  selected object.
void MainWindow::transformObjectSlot()
{
	if(m_DataManager->getNumberOfDataSets() < 1)
	{
		return;
	}

	int selectedIndex = _ui->m_DataSetsListBox->currentRow();

	if(selectedIndex == -1)
	{
		return;
	}

	TransformationsDialog* td = new TransformationsDialog(this);

	if(td->exec() == QDialog::Accepted)
	{
		m_DataManager->addTransformation(selectedIndex, td->getTransformations(), td->getScale());
	}

	delete td;
	td = 0;
}

void MainWindow::selectObjectsSlot(bool select)
{
	m_RendererSet->selectObjectsSlot(select);
}

void MainWindow::globalBoundingBoxSlot(bool render)
{
	m_DataManager->globalBoundingBoxSlot(render);
}

void MainWindow::dataBoundingBoxSlot(bool render)
{
	m_DataManager->dataBoundingBoxSlot(render);
}

void MainWindow::showViewInformationSlot(bool show)
{
	if(!m_ViewInformationLabel)
	{
		return;    // strange!
	}

	QStatusBar* mainStatusBar = statusBar();

	if(!mainStatusBar)
	{
		return;    // strange also!
	}

	if(show)
	{
		mainStatusBar->addPermanentWidget(m_ViewInformationLabel);
		m_ViewInformationLabel->show();
	}
	else
	{
		mainStatusBar->removeWidget(m_ViewInformationLabel);
		m_ViewInformationLabel->hide();
	}
}

void MainWindow::showMousePositionSlot(bool show)
{
	if(!m_MouseInformationLabel)
	{
		return;    // strange!
	}

	QStatusBar* mainStatusBar = statusBar();

	if(!mainStatusBar)
	{
		return;    // strange also!
	}

	if(show)
	{
		mainStatusBar->addPermanentWidget(m_MouseInformationLabel);
		m_MouseInformationLabel->show();
	}
	else
	{
		mainStatusBar->removeWidget(m_MouseInformationLabel);
		m_MouseInformationLabel->hide();
	}
}

//*********************************************
//	Slot functions for Utilities Menu
//*********************************************

void MainWindow::scriptSlot()
{
	m_ScriptsDialog->show();
}


void MainWindow::surfaceDialogSlot()
{
  // Disabled — SurfaceDialog UI widgets not yet ported from Qt3
  QMessageBox::information(this, "Surface Generation", "Surface dialog is not yet available in this version.");
}
  
void MainWindow::displayResultingSurface() {  
  int foundFinished = 0;
  while (foundFinished >= 0) {
    foundFinished = -1;

    for (int i=0; i<surfaceJobs.size(); i++) {
      if (surfaceJobs[i].future->isFinished()) {
	foundFinished = i;
	break;
      }
    }

    if (foundFinished >= 0) {
      // display this mesh and delete form the list
      SurfaceData* surfaceData = new SurfaceData(m_DataManager);
      Geometry * geo = surfaceJobs[foundFinished].future->result();
      surfaceData->setGeometry(geo);
      m_DataManager->addData(surfaceData, surfaceJobs[foundFinished].name + ".raw");

      // FIXME: autorender the surface

      updateUIWithNewData();

      surfaceJobs.erase(surfaceJobs.begin() + foundFinished);

      QMessageBox::information(0, "Molecular Surface Generation", "Job Completed", QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

    }
  }
  
}


void MainWindow::constructNurbsSurfaceSlot()
{
	BallAndStickData* ballAndStickData;
	PDBParser::GroupOfAtoms* molecule;
	if(!getSelected(ballAndStickData,molecule)) return; // Get the selected surface

	SurfacePatchComplex::PatchComplex* patchComplex = new SurfacePatchComplex::PatchComplex();
	patchComplex->setMolecule(molecule);
	patchComplex->createComplex();
	delete patchComplex;
}

void MainWindow::surfaceAreaAndVolumeDialogSlot()
{
	SurfaceData* surfacedata;
	Geometry* surface;
	if(!getSelected(surfacedata,surface)) return; // Get the selected surface

	SurfaceAreaAndVolumeDialog* dialog = new SurfaceAreaAndVolumeDialog(surface->getArea(),surface->getVolume());;
	dialog->show();
}

void MainWindow::constructVolumeUtilitiesSlot()
{
	BallAndStickData* ballAndStickData;
	PDBParser::GroupOfAtoms* molecule;
	if(!getSelected(ballAndStickData,molecule)) return; // Get the selected surface
	ConstructVolumes* cv = new ConstructVolumes(this);
	if(cv->exec() == QDialog::Accepted)
	{
		VolumeData* vData = new VolumeData(m_DataManager);

		if(!cv->constructVolume(m_DataManager, molecule, vData))
		{
			errorDialog("Function returned an error");
			delete vData;
			vData = 0;
		}
		else
		{
			// add to data manager, assume that filename and name are set !
			m_DataManager->addData(vData, vData->getFileName());
			updateUIWithNewData();
		}
	}
	delete cv;
	cv = 0;
}

void MainWindow::constructDepthColoredVolumeSlot()
{
	int dataSetType = m_DataManager->getCurrentDataSetType();
	int dataSetIndex = m_DataManager->getCurrentDataSetIndex();

	if(dataSetType == -1 || dataSetIndex == -1)
	{
		errorDialog("Volume not selected");
		return;
	}

	if(dataSetType != SCALAR_VOLUME && dataSetType != VECTOR_VOLUME)
	{
		errorDialog("Volume not selected");
		return;
	}

	VolumeData* volumeData;

	if(dataSetType == SCALAR_VOLUME)
	{
		volumeData = m_DataManager->getScalarVolumeData(dataSetIndex);
	}
	else
	{
		volumeData = m_DataManager->getVectorVolumeData(dataSetIndex);
	}

	if(volumeData == 0)
	{
		errorDialog("Volume empty");
		return;
	}

	ConstructDepthColoredVolumesDialog* cDialog = new ConstructDepthColoredVolumesDialog(volumeData->getFileName(), this);

	if(cDialog->exec() == QDialog::Accepted)
	{
		VolumeData* newvolumeData = cDialog->constructDepthColoredVolumesFromVolume(m_DataManager, volumeData);

		if(!newvolumeData)
		{
			errorDialog("Function returned an error");
		}
		else
		{
			if(cDialog->loadFile())
			{
				// add to data manager, assume that filename and name are set !
				m_DataManager->addData(newvolumeData, newvolumeData->getFileName());
				updateUIWithNewData();
			}
			else
			{
				delete newvolumeData;
			}
		}
	}

	delete cDialog;
	cDialog = 0;
}

void MainWindow::constructPocketsSlot()
{
	BallAndStickData* ballAndStickData;
	PDBParser::GroupOfAtoms* molecule;
	if(!getSelected(ballAndStickData,molecule)) return; // Get the selected surface

	Pocket* pocket = new Pocket();
	Geometry* pocketGeometry = 0;
	int size = 129;
	SimpleVolumeData* sData = BlurMapsDataManager::getVolume(molecule, "", size, size, size, PDBParser::GroupOfAtoms::ELECTRON_DENSITY,
		false, -2.3, PDBParser::GroupOfAtoms::ATOM, NULL, 0, PDBParser::GroupOfAtoms::VDW_RADIUS, PDBParser::GroupOfAtoms::ATOM);
	VolumeData* vData = new VolumeData(m_DataManager);
	vData->setSimpleData(sData);
	Geometry* molecularSurface = SimpleVolumeDataIsocontourer::getIsocontour(sData, 1.0);
	float distance = 40; //god knows what this is
	char filename[1024];
	strcpy(filename, ballAndStickData->getFileName().toLatin1().constData());

	if(strlen(filename) < 1)
	{
		strcpy(filename, "temp.pdb");
	}

	//bool ret = pocket->constructPockets( molecularSurface, filename, distance, &pocketGeometry, size );
	//bool ret = pocket->constructPockets(molecule, filename, distance, &pocketGeometry, size);
	pocket->constructPockets(molecule, filename, distance, &pocketGeometry, size);
	delete pocket;

	if(!pocketGeometry)
	{
		return;
	}

	SurfaceData* surfaceData = new SurfaceData(m_DataManager);
	surfaceData->setGeometry(pocketGeometry);
	m_DataManager->addData(surfaceData, ballAndStickData->getFileName() + ".raw");
	updateUIWithNewData();
	m_DataManager->addData(vData, ballAndStickData->getFileName() + ".rawiv");
	updateUIWithNewData();
}

// Given a surface, compute the Pocket-Tunnel-Void etc.
// Given a volume, select an isosurface and compute the same.
// Given a PDB, blur it, select an isosurface and compute the same.
void MainWindow::computePocketTunnelSlot()
{

#ifdef POCKET_TUNNEL
	using namespace PocketTunnel; // arand removed...
	BallAndStickData* ballAndStickData;
	PDBParser::GroupOfAtoms* molecule;
	int dataSetType = m_DataManager->getCurrentDataSetType();
	int dataSetIndex = m_DataManager->getCurrentDataSetIndex();

	if(dataSetType == GOA)
	{
	    if(!getSelected(ballAndStickData,molecule)) return; // Get the selected surface

		// dialog
		Ui::PocketTunnelDialogBase curation_dialog;
		QDialog dialog;
		curation_dialog.setupUi( &dialog );

		if( dialog.exec() == QDialog::Accepted )
		{
			int opc = curation_dialog.numPocket->text().toInt();
			int otc = curation_dialog.numTunnel->text().toInt();

			// blurred volume.
			int size = 128;
			SimpleVolumeData* blurred_density_map = BlurMapsDataManager::getVolume(molecule, "", size, size, size,
				PDBParser::GroupOfAtoms::ELECTRON_DENSITY,false, -0.5, PDBParser::GroupOfAtoms::ATOM, NULL, 0,
				PDBParser::GroupOfAtoms::VDW_RADIUS,PDBParser::GroupOfAtoms::ATOM);
			//calculate correct isovalue
			double volmin, volmax;
			blurred_density_map->getMinMax(0,&volmin,&volmax);
			double mapped_isoval = 0.25*(volmax-volmin);
			// surface from blurred volume.
			Geometry* molsurf = SimpleVolumeDataIsocontourer::getIsocontour(blurred_density_map,mapped_isoval); //mapped_isoval=1.0
			// container for output pocket-tunnel geometry.
			Geometry* PTV = new Geometry(); // Geometry of pocket-tunnel-void.
			Geometry* curated_surf = new Geometry(); // Geometry after curation.
			// pocket-tunnel computation and curation.
			cerr << endl << "PTV computation from " << (ballAndStickData->getFileName()).toStdString() << endl;
			pocket_tunnel_fromsurf(molsurf, &PTV, &curated_surf, opc, otc);
			cerr << endl << "done." << endl << endl;
			if(!PTV)
			{
				cerr << "Pocket-Tunnel-Void Geometry is NULL." << endl;
				return;
			}

			if(!curated_surf)
			{
				cerr << "Curated Geometry is NULL." << endl;
				return;
			}

			// rendering of computed pocket-tunnel, curated surface and the blurred volume.
			SurfaceData* surfacePTVData = new SurfaceData(m_DataManager);
			surfacePTVData->setGeometry(PTV);
			m_DataManager->addData(surfacePTVData, ballAndStickData->getFileName() + ".PTV.rawc");
			updateUIWithNewData();
			SurfaceData* surfaceCuratedData = new SurfaceData(m_DataManager);
			surfaceCuratedData->setGeometry(curated_surf);
			m_DataManager->addData(surfaceCuratedData, ballAndStickData->getFileName() + ".curated.raw");
			updateUIWithNewData();
			VolumeData* vData = new VolumeData(m_DataManager);
			vData->setSimpleData(blurred_density_map);
			m_DataManager->addData(vData, ballAndStickData->getFileName() + ".rawiv");
			updateUIWithNewData();
		}
		// end dialog
	}
	else if(dataSetType == SCALAR_VOLUME)
	{
		// Not yet implemented.
		cerr << "SCALAR_VOLUME is not yet implemented." << endl;
	}
	else if(dataSetType == SURFACE)
	{
		// dialog
		Ui::PocketTunnelDialogBase curation_dialog;
		QDialog dialog;
		curation_dialog.setupUi( &dialog );
		if( dialog.exec() == QDialog::Accepted )
		{
			int opc = curation_dialog.numPocket->text().toInt();
			int otc = curation_dialog.numTunnel->text().toInt();
			// input surface
			SurfaceData* surfaceInputData = m_DataManager->getSurfaceData(dataSetIndex);
			Geometry* molsurf = surfaceInputData->getGeometry();
			// container for output pocket-tunnel geometry.
			Geometry* PTV = new Geometry();
			Geometry* curated_surf = new Geometry(); // Geometry after curation.
			cerr << endl << "PTV computation from "<< (surfaceInputData->getFileName()).toStdString() << endl;
			// computation of pocket-tunnel.
			pocket_tunnel_fromsurf(molsurf, &PTV, &curated_surf, opc, otc);
			cerr << endl << "done." << endl << endl;

			if(!PTV)
			{
				cerr << "Pocket-Tunnel-Void Geometry is NULL." << endl;
				return;
			}

			if(!curated_surf)
			{
				cerr << "Curated Geometry is NULL." << endl;
				return;
			}

			// rendering
			SurfaceData* surfacePTVData = new SurfaceData(m_DataManager);
			surfacePTVData->setGeometry(PTV);
			m_DataManager->addData(surfacePTVData, surfaceInputData->getFileName() + ".PTV.rawc");
			updateUIWithNewData();
			SurfaceData* surfaceCuratedData = new SurfaceData(m_DataManager);
			surfaceCuratedData->setGeometry(curated_surf);
			m_DataManager->addData(surfaceCuratedData, surfaceInputData->getFileName() + ".curated.raw");
			updateUIWithNewData();
		}
	}
	else
	{
		// The execution should not come here.
		return;
	}

	return;

#else
	// no CGAL... popup a message
	errorDialog("Pocket-Tunnel was not enabled in this build.");

#endif
}

void MainWindow::constructHLSPocketsSlot()
{
	int dataSetType = m_DataManager->getCurrentDataSetType();
	int dataSetIndex = m_DataManager->getCurrentDataSetIndex();

	if(dataSetType == -1 || dataSetIndex == -1)
	{
		errorDialog("GroupOfAtoms not selected");
		return;
	}

	if(dataSetType != GOA)
	{
		errorDialog("GroupOfAtoms not selected");
		return;
	}

	GOALoader* goaLoader = new GOALoader();
	QString pqr_filename = m_DataManager->getDataSetName(dataSetIndex);
	PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(pqr_filename.toStdString(), true);
	//water will be deleted.

	Pocket* pocket = new Pocket();

	Geometry* pocketGeometry = 0;
	int size = 256;
	float distance = 60 ; // 40; //god knows what this is

	HLevelSet* hls = new HLevelSet();

	unsigned int dim[3];
	dim[0]=dim[1]=dim[2] = size;
	
	SimpleVolumeData* sData = hls->getHigherOrderLevelSetSurface(molecule, dim, 2, 0.00001);
	//SimVolData;
	int sDataSize = sData->getWidth()*sData->getHeight()*sData->getDepth();

	for(int i = 0; i < sDataSize; i++)
	{
		double f = sData->getValueAt(0, i);
		sData->setValueAt(0, i, 1-f);
	}

	VolumeData* vData = new VolumeData(m_DataManager);
	char filename[] = "test";
	AbstractData* inputData= 0;
	Geometry* inputGeometry = 0;
	QString geometryName;
	printf("IsoValueofHLS = %f\n", IsoValueofHLS);
	Geometry* molecularSurface = SimpleVolumeDataIsocontourer::getIsocontour(sData, 2-IsoValueofHLS);//1-IsoValueofHLS);
	//bool ret = pocket->constructPockets(molecularSurface, filename, distance, &pocketGeometry, size);
	pocket->constructPockets(molecularSurface, filename, distance, &pocketGeometry, size);
	delete pocket;
	if(!pocketGeometry)
	{
		return;
	}
	vData->setSimpleData(sData);
	SurfaceData* surfaceData = new SurfaceData(m_DataManager);
	surfaceData->setGeometry(pocketGeometry);


	m_DataManager->addData(surfaceData, "test_pks.raw"); 
	updateUIWithNewData();
	m_DataManager->addData(vData, "test_pks.rawiv"); 
	updateUIWithNewData();

}

void MainWindow::constructCurvaturesUtilitySlot()
{
	ConstructCurvaturesDialog* ccd = new ConstructCurvaturesDialog(this);

	if(ccd->exec() == QDialog::Accepted)
	{
		if(ccd->constructCurvature() == false)
		{
			errorDialog("Function returned an error while constructing curvatures");
		}
	}

	delete ccd;
	ccd = 0;
}

void MainWindow::matchSlot()
{
	errorDialog("Feature not yet supported");
//	/* (void)system("./match/match"); 
}

QString get_env_var(std::string const& key);

void MainWindow::GB_energy_Slot()
{
#ifdef WITH_NFFT
  GeneralizedBornDialog gbd;
  if (gbd.exec() == QDialog::Accepted) {       

    // get the parameters
    QString pqrFileQS = gbd.getPqrFileName();
    QByteArray pqrFileQBA = pqrFileQS.toLatin1();
    char * pqrFile = pqrFileQBA.data();
    
    QString rawnFileQS = gbd.getRawnFileName();
    QByteArray rawnFileQBA = rawnFileQS.toLatin1();
    char *rawnFile = rawnFileQBA.data();
    
    QString outputPrefixQS = gbd.getOutputPrefix();
    QByteArray outputPrefixQBA = outputPrefixQS.toLatin1();
    char *outputPrefix = outputPrefixQBA.data();
    
    double epsilon = gbd.getEpsilon();
    double threshold = gbd.getThreshold();

    int surfType = gbd.getSurfaceType();    
   
    // first compute the area because 
    
    double myArea = area2(rawnFile);

    double energy = GBEnergy(pqrFile, rawnFile, outputPrefix,
			     myArea, surfType, epsilon, threshold);


    char tmpstr[256];
    sprintf(tmpstr, "Energy computed: %g kcal/mol", energy);

    QMessageBox::information(0, "GB Energy", tmpstr, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
    
  }

#else
  errorDialog("NFFT not enabled in this build.");
#endif

}

void MainWindow::GB_forceField_Slot()
{


#ifdef WITH_NFFT
  GeneralizedBornDialog gbd;
  if (gbd.exec() == QDialog::Accepted) {       

    // get the parameters
    QString pqrFileQS = gbd.getPqrFileName();
    QByteArray pqrFileQBA = pqrFileQS.toLatin1();
    char * pqrFile = pqrFileQBA.data();
    
    QString rawnFileQS = gbd.getRawnFileName();
    QByteArray rawnFileQBA = rawnFileQS.toLatin1();
    char *rawnFile = rawnFileQBA.data();
    
    QString outputPrefixQS = gbd.getOutputPrefix();
    QByteArray outputPrefixQBA = outputPrefixQS.toLatin1();
    char *outputPrefix = outputPrefixQBA.data();
    
    double epsilon = gbd.getEpsilon();
    double threshold = gbd.getThreshold();

    int surfType = gbd.getSurfaceType();    
   
    // first compute the area because 
    
    double myArea = area2(rawnFile);

    GBForce(pqrFile, rawnFile, outputPrefix,
	    myArea, surfType, epsilon, threshold);
    
    char tmpstr[256];
    sprintf(tmpstr, "%s.rawnc", outputPrefix);

    if(!m_DataManager->addNewDataSet(QStringList(tmpstr), -1)) {
      printf("error displaying results");
      return;
    }
    updateUIWithNewData();
    
  }

#else
  errorDialog("NFFT not enabled in this build.");
#endif


}

void MainWindow::PB_energy_Slot()
{

#ifdef WITH_PETSC

  PoissonBoltzmannDialog pbd;
  if (pbd.exec() == QDialog::Accepted) {


    QString pqrFileQS = pbd.getPqrFileName();
    QByteArray pqrFileQBA = pqrFileQS.toLatin1();
    char * pqrFile = pqrFileQBA.data();

    QString rawnFileQS = pbd.getRawnFileName();
    QByteArray rawnFileQBA = rawnFileQS.toLatin1();
    char *rawnFile = rawnFileQBA.data();

    QString outputPrefixQS = pbd.getOutputPrefix();
    QByteArray outputPrefixQBA = outputPrefixQS.toLatin1();
    char *outputPrefix = outputPrefixQBA.data();

    double temp = pbd.getTemp();
    double ionConc = pbd.getIonConc();

    char ** argv2 = new char*[4];    
    argv2[0] = new char[100];
    strcpy(argv2[0], "-ksp_rtol");
    argv2[1] = new char[100];
    //strcpy(argv2[1],  "1.0e-4"); // much faster... but not as accurate
    strcpy(argv2[1],  "1.0e-5");
    //strcpy(argv2[1],  "1.0e-6"); // more accurate...
    
    argv2[2] = new char[100];
    strcpy(argv2[2], "-ksp_max_it");
    argv2[3] = new char[100];
    strcpy(argv2[3], "110");
    
    int argc2 = 4;
    PetscInitialize(&argc2,&argv2,0,0);


    char potFile[256];
    sprintf(potFile, "%s.potential", outputPrefix);
   
    PBSolver PB;
    PBOptions PBOpt;
    PB.setOptions(&PBOpt,2.0,78.0,temp,ionConc,128,pqrFile,rawnFile,potFile,outputPrefix,NULL,NULL,NULL);


    PB.computePotentialCommandLine(&PBOpt);

    
    char tmpstr[256];
    sprintf(tmpstr, "%s.rawnc", outputPrefix);

    if(!m_DataManager->addNewDataSet(QStringList(tmpstr), -1)) {
      printf("error displaying results");
      return;
    }
    updateUIWithNewData();


    PB.computeEnergy(&PBOpt);


    sprintf(tmpstr, "Energy computed: %g kcal/mol", PB.totalEnergy);


    QMessageBox::information(0, "PB Energy", tmpstr, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);

  }
#else
  errorDialog("PETSc not enabled in this build.");
#endif

}

void MainWindow::sliceParameterizationSlot()
{
	BallAndStickData* ballAndStickData;
	PDBParser::GroupOfAtoms* molecule;
	if(!getSelected(ballAndStickData,molecule)) return; // Get the selected surface

	SliceDialog* sd = new SliceDialog(molecule, this);

	if(sd->exec() == QDialog::Accepted)
	{
		sd->printParameterization();
	}

	delete sd;
	sd = 0;
}

void MainWindow::elucidateSecondaryStructuresSlot()
{

#ifdef SS 
	// load the geometry
	AbstractData* inputData= 0;
	Geometry* inputGeometry = 0;
	QString geometryName;

	if(m_DataManager->getCurrentDataSetType() == SCALAR_VOLUME)
	{
		// here is the volume to analyze
		VolumeData* currentScalarVolume = m_DataManager->getScalarVolumeData(m_DataManager->getCurrentDataSetIndex());
		inputData = currentScalarVolume;
		// get the most-recently-touched isosurface
		inputGeometry = currentScalarVolume->getGeometry();
		geometryName = currentScalarVolume->getFileName();

		if(inputGeometry == 0)
		{
			errorDialog("Scalar volume has no isosurface");
			return;
		}
	}
	else if(m_DataManager->getCurrentDataSetType() == SURFACE)
	{
		// use the currently-visible surface geometry
		SurfaceData* surfacedata = m_DataManager->getSurfaceData(m_DataManager->getCurrentDataSetIndex());
		inputData = surfacedata;
		geometryName = surfacedata->getFileName();
		inputGeometry = surfacedata->getGeometry();
	}
	else
	{
		errorDialog("Scalar volume or surface not currently selected");
		return;
	}

	// geometry holds the geometry to analyze
	assert(inputGeometry != 0);
	SecondaryStructureData* ssData = new SecondaryStructureData(m_DataManager, inputGeometry);
	// ensure that the transfomations on the input are applied to the output
	// (e.g.: input surface was translated-- output surface should also be translated)
	ssData->setTransformation(inputData->getTransformation());

	QFileInfo geometryfilename(geometryName);

    char* ssfilename;  
	ssfilename = (char *) ((geometryfilename.baseName()+ QString("_SS.rawc")).toStdString().c_str());
 
	m_DataManager->addData(ssData, ssfilename); 
	// now the skeleton is added in a separate file
	SurfaceData* skeleton = new SurfaceData(m_DataManager);
	skeleton->setGeometry(ssData->getSkeletonGeometry());

	char* skeletonfilename;

	skeletonfilename =  (char *) ((geometryfilename.baseName()+ QString("_skel.rawc")).toStdString().c_str());

	m_DataManager->addData(skeleton, skeletonfilename);
	// make the secondary structures visible immediately (100% alpha is default)

	ssData->renderEnabledSlot(true);
	ssData->setVisibilityInPropertiesWidget();
	
	updateUIWithNewData();
#else
	errorDialog("Secondary structure elucidation was not enabled in this build.");
#endif

}



//*********************************************
//	Slot functions for Help Menu
//*********************************************
void MainWindow::contentsSlot()
{
}

void MainWindow::indexHelpSlot()
{
	m_Documentation->showHelpIndex();
}

void MainWindow::acknowledgementSlot()
{

  /* // old version...
	QMessageBox::information(this,"Acknowledgements",
				 "This software was written by Vinay K Siddavanahalli under Dr C Bajaj at\n"
				 "the Computational Visualization Center, The University of Texas at Austin\n"
				 "\n"
				 "The algorithms and softwares were partly developed by the following students:\n"
				 "\n"
				 "Anthony Thane: Main visualization and contouring algorithms\n"
				 "Bong Soo Sohn: Contour trees\n"
				 "John Wiggins: Color maps, MRC IO\n"
				 "Julio Castrillon: Fast summation ideas\n"
				 "Peter Djeu: PDB parser\n"
				 "\n"
				 "The fancy ascii headers were made using a Figlet server.\n"
				 );
  */
  QMessageBox::information(this, "", "TexMol was developed at the Computational Visualization Center, The University of Texas at Austin under the direction of Dr. Chandrajit Bajaj.\n" 
			   "\n"
			   "TexMol contributors include:\n"
			   "Deukhyun Cha\n"
			   "Albert Chen\n"
			   "Rezaul Chowdhury\n"
			   "Peter Djeu\n"
			   "Samrat Goswami\n"
			   "Jasmin Horozovic\n"
			   "Bongjune Kwon\n"
			   "Alex Lee\n"
			   "Craig Pemberton\n"
			   "Alex Rand\n"
			   "Muhibur Rasheed\n"
			   "Vinay Siddavanahalli\n"
			   "Matt Strange\n"
			   "Anthony Thane\n"
			   "John Wiggins\n"
			   "Zhang Qin\n"
			   "Xiaoyu Zhang\n"
			   "Wenqi Zhao\n"
);


}

//*************************
//	Update UI
//*************************

void MainWindow::updateUIWithNewData()
{
	updateListBox();
	m_RendererSet->update();
//	newDataSetSelectedSlot( m_DataManager->getNumberOfDataSets()-1 );
}

void MainWindow::updateListBox()
{
	// reset the listbox
	_ui->m_DataSetsListBox->clear();	// maybe keep track of which is currently highlighted also
	int i, numberOfDataSets;
	numberOfDataSets= m_DataManager->getNumberOfDataSets();

	for(i=0; i<numberOfDataSets; i++)
	{
		string dName = (m_DataManager->getDataSetName(i)).toStdString();
		int dIndex = m_DataManager->getDataSetIndex(i);
	
		//Craig: we only want the file name, not the path
		_ui->m_DataSetsListBox->addItem( QString((dName.substr(dName.find_last_of("/")+1)).c_str())) ;
	}

	if(numberOfDataSets < 1)
	{
		m_Action_TransformObject->setEnabled(false);
	}
	else
	{
		m_Action_TransformObject->setEnabled(true);
	}
}

//*********************************************
//	Basic Setting and Utility functions
//*********************************************

QString getFilter(QString* dataTypes, int numberOfDataTypes)
{
	QString m_Filter = "";
	int c = 0;
	for(int i=0; i<numberOfDataTypes; i++)
	{
		m_Filter.append(dataTypes[c]);
		c++;
		int numTypes = dataTypes[c].toInt();
		c++;
		m_Filter.append(" (");
		for(int j=0; j<numTypes; j++)
		{
			m_Filter.append("*.");
			m_Filter.append(dataTypes[c]);
			c++;

			if(j != numTypes-1)
			{
				m_Filter.append(" ");
			}
		}

		m_Filter.append(");;");
	}
	m_Filter.append("All files (*.*)");
	return m_Filter;
}

bool MainWindow::addNewDataSet(QStringList fileNames, bool separate)
{
	int nextID = m_DataManager->m_NextUniqueAvailableId;
	bool setView = m_DataManager->getNumberOfDataSets() == 0;

	if(separate)
	{
		for(int i = 0; i < fileNames.count(); i++)
		{
			QStringList singleFile = QStringList(fileNames[i]);
			if(!m_DataManager->addNewDataSet(singleFile))
			{
				return false;
			}
		}
	}
	else
	{
		if(!m_DataManager->addNewDataSet(fileNames))
		{
			return false;
		}
	}

	//this code will center on the loaded union of balls or surface data
	//if nothing else is already loaded into the data manager
	if(BallAndStickData::supportsFileNames(fileNames) && setView)
	{
		BallAndStickData* data = m_DataManager->getBallAndStickData(nextID);
		if(data)
		{
			PDBParser::GroupOfAtoms* atoms = data->getData();
			if(atoms)
			{
				double rx, ry, rz, sum;
				sum = (double)atoms->getMassCenter(&rx, &ry, &rz);
				rx = rx / sum;
				ry = ry / sum;
				rz = rz / sum;
				double trans[4] = {rx, ry, rz, 1.0};
				double rot[4] = {1.0, 0.0, 0.0, 0.0};
				atoms->createSingleBoundingSphere(rx, ry, rz);
				double width = atoms->m_MultiresAtoms[0]->getRadius() * 2;
				m_RendererSet->setViewingParameters(trans, rot, width);
			}
		}
	}

	if(SurfaceData::supportsFileNames(fileNames) && setView)
	{
		SurfaceData* data = m_DataManager->getSurfaceData(nextID);
		if(data)
		{
			Geometry* geometry = data->getGeometry();
			if(geometry)
			{
				geometry->CalculateExtents();
				float* center = geometry->m_Center;
				double trans[4] = {center[0], center[1], center[2], 1.0};
				double rot[4] = {1.0, 0.0, 0.0, 0.0};
				float* min = geometry->m_Min;
				float* max = geometry->m_Max;
				double width = sqrt((max[0]-min[0])*(max[0]-min[0])
							+ (max[1]-min[1])*(max[1]-min[1])
							+ (max[2]-min[2])*(max[2]-min[2])) * 2;
				m_RendererSet->setViewingParameters(trans, rot, width);
			}
		}
	}

	updateUIWithNewData();
	m_Settings->pushRecentFile(fileNames[0], (QMenu*)m_Action_RecentFileListMenu, this);
	return true;
}

void MainWindow::setPreviousSavedEnvironment()
{
	m_Settings->loadPreviousFiles((QMenu*)m_Action_RecentFileListMenu, this);
}

//  Set the viewing parameters to current renderer
bool MainWindow::setViewingParameters(double* translationParams, double* rotationParams, double windowSize)
{
	if(!translationParams || !rotationParams)
	{
		return false;
	}

	if(!m_RendererSet)
	{
		return false;
	}

	return m_RendererSet->setViewingParameters(translationParams, rotationParams, windowSize);
}

//  Set the viewing parameters to the selected renderer
bool MainWindow::setViewingParameters(int rendererId, double* translationParams, double* rotationParams, double windowSize)
{
	if(!translationParams || !rotationParams)
	{
		return false;
	}

	if(!m_RendererSet)
	{
		return false;
	}

	return m_RendererSet->setViewingParameters(rendererId, translationParams, rotationParams, windowSize);
}
void MainWindow::createLightsMenu()
{
	// read the number of lights and create a new menu for each. Turn all of them off.
	int numberOfLights = 0;
	glGetIntegerv(GL_MAX_LIGHTS, (GLint *) &numberOfLights);
	int i;

	for(i=0; i<numberOfLights; i++)
	{
		char displayName[256];
		sprintf(displayName, "Light %d - off", i);
		QAction* action = m_Menu_LightingPopup->addAction(displayName);
		action->setData(i);
		connect(action, &QAction::triggered, this, [this, i]() { editLight(i); });
		m_LightActions.append(action);
	}

	m_LightSet->init(numberOfLights);
}

bool MainWindow::enableFirstLight()
{
	{
		int numberOfLights = 0;
		glGetIntegerv(GL_MAX_LIGHTS, (GLint *) &numberOfLights);

		if(numberOfLights <1)
		{
			return false;
		}
	}

	if(!m_RendererSet)
	{
		return false;
	}

	if(!m_RendererSet->enableFirstLight())
	{
		return false;
	}

	m_LightSet->m_Lights[0].enabled = true;
	// Qt6: changeItem not available on QMenu — light menu needs rework
	// m_Menu_LightingPopup->changeItem(0, "Light 0 - on");
	if (!m_LightActions.isEmpty()) {
		m_LightActions[0]->setText("Light 0 - on");
	}
	return true;
}

// Qt3 signal removed — dataSetSelectedRightMouse no longer connected
// Keeping stub for ABI compatibility
void MainWindow::dataSetSelectedRightMouse(Q3ListBoxItem* /*qListBoxItem*/, const QPoint& /*qPoint*/)
{
}

#if 0 // Original Qt3 implementation — needs porting
void MainWindow::dataSetSelectedRightMouse_original(const QPoint& qPoint)
{
	int selectedIndex = _ui->m_DataSetsListBox->currentRow();
	if(selectedIndex == -1) return;

	POPUPSELECTION popUpSelection = showPopup(qPoint);

	switch(popUpSelection)
	{
		case SAVE_DATASET:
			{
			  QString fileName = QFileDialog::getSaveFileName(this, "Save file dialog", "../DataSet",
									  getFilter(m_DataManager->getDataTypes(), 
										    m_DataManager->getNumberOfDataTypes()));

				if(fileName.length())
				{
					m_DataManager->save(selectedIndex,fileName);
				}
			}
			break;
		case DELETE_DATASET:
			{
				deleteAndUpdateUI(selectedIndex);
			}
			break;
		default:
			break;
	}
}
#endif

void MainWindow::newDataSetSelectedSlot(int selectedIndex)
{
	// get the properties widget from datamanager and set it
	if(m_PropertyWidget)
	{
		m_PropertyWidget->hide();
		m_QBoxLayout->removeWidget(m_PropertyWidget);
	}

	m_DataManager->setSelectedIndex(selectedIndex);
	m_PropertyWidget = (QWidget*)(m_DataManager->getPropertiesWidget());

	if(m_PropertyWidget == 0)
	{
		return;
	}

	m_QBoxLayout->addWidget(m_PropertyWidget);
	m_PropertyWidget->show();
}

QString get_env_var(std::string const& key)
{
	char* val;
	val = getenv(key.c_str());
	QString retval = "";

	if(val != NULL)
	{
		retval = val;
	}

	return retval;
}

void MainWindow::deleteAndUpdateUI(int selectedIndex)
{
	if(selectedIndex == -1)
	{
		return;
	}

	m_DataManager->setSelectedIndex(selectedIndex);
	m_DataManager->deleteDataSet();
	updateListBox();
	m_RendererSet->update();

	if(m_DataManager->getNumberOfDataSets() == 0)
	{
		if(m_PropertyWidget)
		{
			m_PropertyWidget->hide();
			m_QBoxLayout->removeWidget(m_PropertyWidget);
		}
	}
}

bool MainWindow::deleteData(int dataSetIndex)
{
	int numberOfDataSets= m_DataManager->getNumberOfDataSets();
	int selectedIndex = -1;
	int i;

	// this is done in the same order as in UpdateListBox
	for(i=0; i<numberOfDataSets; i++)
	{
		if(dataSetIndex == m_DataManager->getDataSetIndex(i))
		{
			selectedIndex = i;
		}
	}

	if(selectedIndex == -1)
	{
		return false;
	}

	deleteAndUpdateUI(selectedIndex);
	return true;
}

void MainWindow::aboutHelpSlot()
{
	AboutDialog* about = new AboutDialog(this);
	about->exec();
	delete about;
	about = 0;
}

void MainWindow::redraw()
{
	m_RendererSet->update();
}

void MainWindow::setChildrenVisible(bool mode)
{
	if(mode)
	{
		_ui->m_Frame_MoleculeBrowser->show();
		_ui->m_PropertiesGroupBox->show();
		menuBar()->show();
	}
	else
	{
		_ui->m_Frame_MoleculeBrowser->hide();
		_ui->m_PropertiesGroupBox->hide();
		menuBar()->hide();
	}
}

void MainWindow::setRendererFullScreen(bool mode)
{
	if(mode)
	{
		showFullScreen();
	}
	else
	{
		showNormal();
	}
}

// Craig:	ctrl+key works... kind of.
//		we have two errors right now:
//		1) ctrl release isn't caught once a dialog steals keyboard focus, so you have to type ctrl again to restore normal function later on
//		2) you have to tab until keyboard focus returns otherwise the alphanumeric and left/right keys are handled elsewhere
void MainWindow::keyPressEvent(QKeyEvent* event)
{
	bool ctrl = m_RendererSet->m_ControlPressed;
	int currentItem = _ui->m_DataSetsListBox->currentRow();
	switch(event->key())
	{
		case Qt::Key_Shift:
			m_RendererSet->setShiftPressed(true);
			break;
		case Qt::Key_Control:
			m_RendererSet->setControlPressed(true);
			break;
		case Qt::Key_F:
			fullscreenSlot();
			break;
		case Qt::Key_M:
			//buildMovie();
			break;
		case Qt::Key_O:
			if(ctrl) fileOpenSlot();
			break;
		case Qt::Key_P:
			//debugPrint();
			break;
		case Qt::Key_S:
			if(ctrl && currentItem != -1)
			{
				QString fileName = QFileDialog::getSaveFileName(this, "Choose a filename to save under", "",
					getFilter(m_DataManager->getDataTypes(), m_DataManager->getNumberOfDataTypes()));
				m_DataManager->save(currentItem,fileName.toLatin1().constData());
			} 
			break;
		case Qt::Key_W:
			if(ctrl && currentItem != -1) deleteAndUpdateUI(currentItem);
			break;
		default:
			event->ignore();
			break;
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
	switch(event->key())
	{
		case Qt::Key_Shift:
			m_RendererSet->setShiftPressed(false);
			break;
		case Qt::Key_Control:
			m_RendererSet->setControlPressed(false);
			break;
		default:
			event->ignore();
			break;
	}
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if(!m_Settings)
	{
		event->accept();
		return;
	}

	if(m_Settings->userSettingsChanged())
	{
		int saveChanges = QMessageBox::information(this, "MoleculeViz",
						  "Save user settings ?",
						  "Yes", "No", "Cancel"
												  );

		if(saveChanges == 0)
		{
			m_Settings->saveUserSettings();
			m_Settings->save();
			event->accept();
		}
		else if(saveChanges == 1)
		{
			m_Settings->save();
			event->accept();
		}
		else
		{
			event->ignore();
		}
	}
	else
	{
		m_Settings->save();
		event->accept();
	}
}

bool MainWindow::executeCommand(int argc, QStringList argv)
{
	int i;
	printf("New command\n");

	for(i=0; i<argc; i++)
	{
		QString s = argv[i];
		printf("\t<%s>\n", s.toLatin1().constData());
	}

	Server server;
	char** ctype_argv = new char*[argc];

	for(i=0; i<argc; i++)
	{
		ctype_argv[i] = new char[argv[i].length() + 1];
		strcpy(ctype_argv[i], argv[i].toLatin1().constData());
	}

	bool ret = server.execute(argc, ctype_argv, this);

	for(i=0; i<argc; i++)
	{
		delete ctype_argv[i];
	}

	delete ctype_argv;
	return ret;
}

bool MainWindow::setVisible(bool render, int dataSetIndex)
{
	int numberOfDataSets= m_DataManager->getNumberOfDataSets();
	int i;

	// this is done in the same order as in UpdateListBox
	for(i=0; i<numberOfDataSets; i++)
	{
		if(dataSetIndex == m_DataManager->getDataSetIndex(i))
		{
			m_DataManager->setSelectedIndex(i);
			m_DataManager->setVisible(render);
			m_DataManager->setVisibilityInPropertiesWidget();
			return true;
		}
	}

	return false;
}

bool MainWindow::setVisiblePrev(bool render)
{
	int listIndex = m_DataManager->getNumberOfDataSets() - 1;
	int dataSetIndex = m_DataManager->getDataSetIndex(listIndex);

	if(dataSetIndex == -1)
	{
		return false;
	}

	int numberOfDataSets= m_DataManager->getNumberOfDataSets();
	int i;

	for(i=0; i<numberOfDataSets; i++)
	{
		if(dataSetIndex == m_DataManager->getDataSetIndex(i))
		{
			m_DataManager->setSelectedIndex(i);
			m_DataManager->setVisible(render);
			m_DataManager->setVisibilityInPropertiesWidget();
			return true;
		}
	}

	return false;
}

bool MainWindow::setGridVisible(bool visibility)
{
	showGridOptionsSlot(visibility);
	return true;
}

bool MainWindow::deleteAllData()
{
	int num = m_DataManager->getNumberOfDataSets();
	int i;

	for(i=0; i<num; i++)
	{
		deleteAndUpdateUI(0);
	}

	return true;
}

bool MainWindow::deletePrevData()
{
	int index = m_DataManager->getNumberOfDataSets() - 1;
	deleteAndUpdateUI(index);
	return true;
}

bool MainWindow::saveImage(const char* fileName, const char* fileFormat)
{
	if(!fileName || !fileFormat)
	{
		return false;
	}

	bool saveAll = false;
	m_RendererSet->saveImages(fileName,fileFormat, saveAll);
	return true;
}

void MainWindow::finishedop(bool error)
{
	// Disabled — Q3Http removed
	Q_UNUSED(error);
}

void MainWindow::dataTransferProgressop(int bytesDone, int bytesTotal)
{
}

void MainWindow::editLight(int light) // now light is same as id in the pop up menu
{
	LightsDialog* ld = new LightsDialog(light, m_LightSet, this);

	if(ld->exec() == QDialog::Accepted)
	{
		ld->saveLightInformation(light, m_LightSet);
		// update the menu
		int openGLLightNumber = GL_LIGHT0 + light;
		char popupMenuString[256];

		if(glIsEnabled(openGLLightNumber) == GL_TRUE)
		{
			sprintf(popupMenuString, "Light %d - on", light);
			if (light < m_LightActions.size()) m_LightActions[light]->setText(popupMenuString);
		}
		else
		{
			sprintf(popupMenuString, "Light %d - off", light);
			if (light < m_LightActions.size()) m_LightActions[light]->setText(popupMenuString);
		}
	}

	delete ld;
	ld = 0;
}

void MainWindow::readPrevioslyOpenedFile(int previousFileIndexInMenu)
{
	QString fileName = m_Settings->getFileName(previousFileIndexInMenu);

	if(fileName.length())
	{
		QStringList fileNames;
		fileNames.append(fileName);

		if(!addNewDataSet(fileNames))
		{
			errorDialog("Function returned an error while adding new data");
		}
	}
}

MainWindow::POPUPSELECTION MainWindow::showPopup(QPoint point)
{
	QMenu popup;
	QAction* saveAction = popup.addAction("Save");
	QAction* deleteAction = popup.addAction("Delete");
	QAction* selected = popup.exec(point);
	if (selected == saveAction) return SAVE_DATASET;
	if (selected == deleteAction) return DELETE_DATASET;
	return SAVE_DATASET; // default
}

QString MainWindow::GB_write_surface_file(QString in_filename)
{
	// Compute the surface
	GOALoader* goaLoader = new GOALoader();
	PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(in_filename.toStdString(), true);
	delete goaLoader;

	if(!molecule)
	{
		errorDialog("There was a problem with the pqr file");
		return in_filename;
	}

	Geometry* geometry = 0;
	geometry = MolecularSurface::getMolecularSurface(molecule, 128);
	// Write the surface file
	GeometryLoader* geometryLoader = new GeometryLoader();
	QString surface_filename = in_filename;
	surface_filename += "_TEMPORARY_surface.rawnc";
	if(!geometryLoader->saveFile(surface_filename.toStdString(), geometry))
	{
		delete geometry;
		errorDialog("There was a problem saving the surface file.");
		return in_filename;
	}

	delete geometryLoader;
	return surface_filename;
}

QString MainWindow::GB_write_area_file(QString in_filename)
{
	QString area_filename = in_filename;
	area_filename += ".area";
	// read in the isocontour
	SurfaceData* surfaceData = new SurfaceData(0);
	if(!surfaceData->read(QStringList(in_filename)))
	{
		delete surfaceData;
		surfaceData = 0;
		errorDialog("There was a problem with the surface file.");
		return in_filename;
	}

	// calculate area and write file
	FILE* fp  = fopen(area_filename.toLatin1().constData(), "a");

	if(!fp)
	{
		delete surfaceData;
		return in_filename;
	}

	fprintf(fp, "%lf\n", surfaceData->getArea());
	delete surfaceData;
	fclose(fp);
	return area_filename;
}

void MainWindow::debugPrint()
{
	m_DataManager->debugPrint();
	CCVOpenGLMath::Quaternion orientation;
	CCVOpenGLMath::Vector translation;
	float windowSize;
	m_RendererSet->getCurrentRenderer()->getViewingParameters(&orientation, &translation, &windowSize);
	std::cerr << "T: " << translation[0] << ", " << translation[1] << ", " << translation[2] << ", " << translation[3] << "\n";
	std::cerr << "O: " << orientation[0] << ", " << orientation[1] << ", " << orientation[2] << ", " << orientation[3] << "\n";
	std::cerr << windowSize << "\n";
}

void MainWindow::buildMovie()
{
  //F2DockAnimator::buildMovie(m_DataManager, m_RendererSet);
	updateUIWithNewData();
}

class ViewRowEventMain : public QEvent
{
	public:
		ViewRowEventMain(const QString& m, double* t, const bool s) : QEvent(static_cast<QEvent::Type>(QEvent::User+105)), msg(m), mat(t), surf(s) {}
		QString message() const
		{
			return msg;
		}
		double* matrix()
		{
			return mat;
		}
		bool surface() const
		{
			return surf;
		}
	private:
		QString msg;
		double* mat;
		bool surf;
};

void MainWindow::customEvent(QEvent* event)
{
	if(event->type() == CallEvent::TypeId)
	{
		CallEvent* ce = static_cast<CallEvent*>(event);

		if(ce->funcName() == "deleteAndUpdateUI")
		{
			deleteAndUpdateUI(std::any_cast<int>(ce->args()[0]));
		}
		else if(ce->funcName() == "redraw")
		{
			redraw();
		}
		else if(ce->funcName() == "updateUIWithNewData")
		{
			updateUIWithNewData();
		}
	}
	else if(event->type() == QEvent::User+104)
	{
		QString fileName = static_cast<ViewRowEventMain*>(event)->message();
		double* m = static_cast<ViewRowEventMain*>(event)->matrix();
		bool surface = static_cast<ViewRowEventMain*>(event)->surface();

		if(fileName == "delete")
		{
			deleteData(m_F2DockReceptorIndex);
			m_F2DockReceptorIndex = -1;
			deleteData(m_F2DockLigandIndex);
			m_F2DockLigandIndex = -1;
			deleteData(m_F2DockReceptorSurfaceIndex);
			m_F2DockReceptorSurfaceIndex = -1;
			deleteData(m_F2DockLigandSurfaceIndex);
			m_F2DockLigandSurfaceIndex = -1;
			return;
		}

		if(surface && m_F2DockReceptorIndex != -1 && m_F2DockReceptorSurfaceIndex == -1)
		{
			printf("generating receptor surface...\n");
			#ifdef MSA
			m_F2DockReceptorSurfaceIndex = m_DataManager->m_NextUniqueAvailableId;
			m_DataManager->setDataSetIndex(m_F2DockReceptorIndex);
			BallAndStickData* ballAndStickData;
			PDBParser::GroupOfAtoms* molecule;
			if(!getSelected(ballAndStickData,molecule)) return; // Get the selected surface

			Geometry* geo = surfaceUsingAdaptiveGrid(molecule, 128);

			// Update TexMol with the new data
			SurfaceData* surfaceData = new SurfaceData(m_DataManager);
			surfaceData->setGeometry(geo);
			m_DataManager->addData(surfaceData, ballAndStickData->getFileName() + ".raw");
			updateUIWithNewData();

			printf("...done\n");

			//set color
			Geometry* surfGeo = m_DataManager->getSurfaceData(m_F2DockReceptorSurfaceIndex)->getGeometry();
			surfGeo->SetDiffusedColor(0.0, 0.0, 0.3);
			surfGeo->SetAmbientColor(0.0, 0.0, 0.3);
			surfGeo->SetSpecularColor(0.0, 0.0, 0.3);
			surfGeo->ClearTriVertColors();
			#else
			fprintf( stderr, "MolSurf API was not enabled in this build\n");
			#endif
		}
		if(surface && m_F2DockLigandIndex != -1 && m_F2DockLigandSurfaceIndex == -1)
		{
			printf("generating ligand surface...\n");
			#ifdef MSA
			m_F2DockLigandSurfaceIndex = m_DataManager->m_NextUniqueAvailableId;
			m_DataManager->setDataSetIndex(m_F2DockLigandIndex);
			BallAndStickData* ballAndStickData;
			PDBParser::GroupOfAtoms* molecule;
			if(!getSelected(ballAndStickData,molecule)) return; // Get the selected surface

			Geometry* geo = surfaceUsingAdaptiveGrid(molecule, 128);

			// Update TexMol with the new data
			SurfaceData* surfaceData = new SurfaceData(m_DataManager);
			surfaceData->setGeometry(geo);
			surfaceData->setTransformation(m_DataManager->getBallAndStickData(m_F2DockLigandIndex)->getTransformation());
			m_DataManager->addData(surfaceData, ballAndStickData->getFileName() + ".raw");
			updateUIWithNewData();

			printf("...done\n");

			//make the ligand transparent
			m_DataManager->getSurfaceData(m_F2DockLigandSurfaceIndex)->setAlpha(0.5);

			//set color
			Geometry* surfGeo = m_DataManager->getSurfaceData(m_F2DockLigandSurfaceIndex)->getGeometry();
			surfGeo->SetDiffusedColor(0.5, 0.0, 0.0);
			surfGeo->SetAmbientColor(0.5, 0.0, 0.0);
			surfGeo->SetSpecularColor(0.5, 0.0, 0.0);
			surfGeo->ClearTriVertColors();
			#else
			fprintf( stderr, "MolSurf API was not enabled in this build\n");
			#endif
		}

		//set visibility
		if(m_F2DockRendererId != -1)
		{
			if(surface)
			{
				m_DataManager->setCurrentRendererId(m_F2DockRendererId);
				m_DataManager->setDataSetIndex(m_F2DockReceptorIndex);
				m_DataManager->setVisible(false);
				m_DataManager->setDataSetIndex(m_F2DockLigandIndex);
				m_DataManager->setVisible(false);
				m_DataManager->setDataSetIndex(m_F2DockReceptorSurfaceIndex);
				m_DataManager->setVisible(true);
				m_DataManager->setDataSetIndex(m_F2DockLigandSurfaceIndex);
				m_DataManager->setVisible(true);

				m_DataManager->setCurrentRendererId(m_F2DockRendererId + 1);
				m_DataManager->setDataSetIndex(m_F2DockReceptorIndex);
				m_DataManager->setVisible(false);
				m_DataManager->setDataSetIndex(m_F2DockReceptorSurfaceIndex);
				m_DataManager->setVisible(true);

				m_DataManager->setCurrentRendererId(m_F2DockRendererId + 2);
				m_DataManager->setDataSetIndex(m_F2DockLigandIndex);
				m_DataManager->setVisible(false);
				m_DataManager->setDataSetIndex(m_F2DockLigandSurfaceIndex);
				m_DataManager->setVisible(true);
			}
			else
			{
				m_DataManager->setCurrentRendererId(m_F2DockRendererId);
				m_DataManager->setDataSetIndex(m_F2DockReceptorSurfaceIndex);
				m_DataManager->setVisible(false);
				m_DataManager->setDataSetIndex(m_F2DockLigandSurfaceIndex);
				m_DataManager->setVisible(false);
				m_DataManager->setDataSetIndex(m_F2DockReceptorIndex);
				m_DataManager->setVisible(true);
				m_DataManager->setDataSetIndex(m_F2DockLigandIndex);
				m_DataManager->setVisible(true);

				m_DataManager->setCurrentRendererId(m_F2DockRendererId + 1);
				m_DataManager->setDataSetIndex(m_F2DockReceptorSurfaceIndex);
				m_DataManager->setVisible(false);
				m_DataManager->setDataSetIndex(m_F2DockReceptorIndex);
				m_DataManager->setVisible(true);

				m_DataManager->setCurrentRendererId(m_F2DockRendererId + 2);
				m_DataManager->setDataSetIndex(m_F2DockLigandSurfaceIndex);
				m_DataManager->setVisible(false);
				m_DataManager->setDataSetIndex(m_F2DockLigandIndex);
				m_DataManager->setVisible(true);
			}
		}

		if(fileName == "surface button changed")
		{
			return;
		}

		if(m_F2DockReceptorIndex == -1)
		{
			m_F2DockReceptorIndex = m_DataManager->m_NextUniqueAvailableId;

			bool addedNewDataSet = true;
			QStringList fileNames = QStringList(fileName);

			if(fileNames.count() > 0)
			{
				addedNewDataSet = addNewDataSet(fileNames);
			}

			if(addedNewDataSet)
			{
				m_F2DockRendererId = m_DataManager->getCurrentRenderer();
				//add 2 more views and render the receptor in the last view
				//(making the assumption that renderer id's correspond to the number of views,
				// which should be okay since there doesn't seem to be a way to decrease the number of views)
				if(m_DataManager->m_RendererSet->getNumberOfRenderers() <= m_F2DockRendererId + 1)
				{
					splitViewSlot();
				}
				if(m_DataManager->m_RendererSet->getNumberOfRenderers() <= m_F2DockRendererId + 2)
				{
					splitViewSlot();
				}
				m_DataManager->setCurrentRendererId(m_F2DockRendererId + 1);
				m_DataManager->setDataSetIndex(m_F2DockReceptorIndex);
				//m_DataManager->setVisibilityInPropertiesWidget();

				double rx, ry, rz, sum;
				sum = (double)m_DataManager->getBallAndStickData(m_F2DockReceptorIndex)->getData()->getMassCenter(&rx, &ry, &rz);
				rx = rx / sum;
				ry = ry / sum;
				rz = rz / sum;
				CCVOpenGLMath::Vector transRVec = CCVOpenGLMath::Vector(rx, ry, rz, 1.0);
				double transR[4] = {transRVec[0], transRVec[1], transRVec[2], transRVec[3]};
				double rotR[4] = {1.0, 0.0, 0.0, 0.0};
				m_RendererSet->setViewingParameters(m_F2DockRendererId + 1, transR, rotR, 200.0);

				m_DataManager->setVisible(true);
				return;
			}

			m_F2DockReceptorIndex = -1;
			errorDialog("Error adding new data.");
		}
		else {
			CCVOpenGLMath::Matrix matrix = CCVOpenGLMath::Matrix(m[0], m[1], m[2], m[3],
										m[4], m[5], m[6], m[7],
										m[8], m[9], m[10], m[11],
										0.0, 0.0, 0.0, 1.0);

			if(m_F2DockLigandIndex != -1 && fileName == "matrix changed")
			{
				m_DataManager->getBallAndStickData(m_F2DockLigandIndex)->setTransformation(matrix);
				if(m_DataManager->getSurfaceData(m_F2DockLigandSurfaceIndex))
				{
					m_DataManager->getSurfaceData(m_F2DockLigandSurfaceIndex)->setTransformation(matrix);
				
				}

				//set the viewing parameters
				double lx, ly, lz, sum;
				sum = (double)m_DataManager->getBallAndStickData(m_F2DockLigandIndex)->getData()->getMassCenter(&lx, &ly, &lz);
				lx = lx / sum;
				ly = ly / sum;
				lz = lz / sum;
				CCVOpenGLMath::Vector transLVec = CCVOpenGLMath::Vector(lx, ly, lz, 1.0);
				transLVec = matrix * transLVec;
				double transL[4] = {transLVec[0], transLVec[1], transLVec[2], transLVec[3]};
				double rotL[4] = {1.0, 0.0, 0.0, 0.0};
				m_RendererSet->setViewingParameters(m_F2DockRendererId + 2, transL, rotL, 200.0);

				m_DataManager->update();
				return;
			}

			m_DataManager->setCurrentRendererId(m_F2DockRendererId);

			m_F2DockLigandIndex = m_DataManager->m_NextUniqueAvailableId;

			bool addedNewDataSet = true;
			QStringList fileNames = *(new QStringList(fileName));

			if(fileNames.count() > 0)
			{
				addedNewDataSet = addNewDataSet(fileNames);
			}

			if(addedNewDataSet)
			{
				if(surface)
				{
					printf("generating ligand surface...\n");
					#ifdef MSA
					m_F2DockLigandSurfaceIndex = m_DataManager->m_NextUniqueAvailableId;
					m_DataManager->setDataSetIndex(m_F2DockLigandIndex);
					BallAndStickData* ballAndStickData;
					PDBParser::GroupOfAtoms* molecule;
					if(!getSelected(ballAndStickData,molecule)) return; // Get the selected surface

					Geometry* geo = surfaceUsingAdaptiveGrid(molecule, 128);

					// Update TexMol with the new data
					SurfaceData* surfaceData = new SurfaceData(m_DataManager);
					surfaceData->setGeometry(geo);
					m_DataManager->addData(surfaceData, ballAndStickData->getFileName() + ".raw");
					updateUIWithNewData();

					printf("...done\n");

					//make the surface data visible in the first view
					m_DataManager->setCurrentRendererId(m_F2DockRendererId);
					m_DataManager->setDataSetIndex(m_F2DockLigandIndex);
					m_DataManager->setVisible(false);
					m_DataManager->setDataSetIndex(m_F2DockLigandSurfaceIndex);
					m_DataManager->setVisible(true);
					//make the ligand transparent
					m_DataManager->getSurfaceData(m_F2DockLigandSurfaceIndex)->setAlpha(0.5);
					//set colors
					Geometry* surfGeo = m_DataManager->getSurfaceData(m_F2DockLigandSurfaceIndex)->getGeometry();
					surfGeo->SetDiffusedColor(0.5, 0.0, 0.0);
					surfGeo->SetAmbientColor(0.5, 0.0, 0.0);
					surfGeo->SetSpecularColor(0.5, 0.0, 0.0);
					surfGeo->ClearTriVertColors();
					#else
					fprintf( stderr, "MolSurf API was not enabled in this build\n");
					#endif
				}

				//set the transformation
				m_DataManager->getBallAndStickData(m_F2DockLigandIndex)->setTransformation(matrix);
				if(m_DataManager->getSurfaceData(m_F2DockLigandSurfaceIndex))
				{
					m_DataManager->getSurfaceData(m_F2DockLigandSurfaceIndex)->setTransformation(matrix);
				}

				//now make the ligand visible in the final view
				m_DataManager->setCurrentRendererId(m_F2DockRendererId + 2);
				if(surface)
				{
					m_DataManager->setDataSetIndex(m_F2DockLigandIndex);
					m_DataManager->setVisible(false);
					m_DataManager->setDataSetIndex(m_F2DockLigandSurfaceIndex);
					m_DataManager->setVisible(true);
					//calculate interface using pseudo-Gsol
					
				}
				else
				{
					m_DataManager->setDataSetIndex(m_F2DockLigandSurfaceIndex);
					m_DataManager->setVisible(false);
					m_DataManager->setDataSetIndex(m_F2DockLigandIndex);
					m_DataManager->setVisible(true);
					//calculate interface using clashFilter
					
				}

				//set the viewing parameters
				double lx, ly, lz, sum;
				sum = (double)m_DataManager->getBallAndStickData(m_F2DockLigandIndex)->getData()->getMassCenter(&lx, &ly, &lz);
				lx = lx / sum;
				ly = ly / sum;
				lz = lz / sum;
				CCVOpenGLMath::Vector transLVec = CCVOpenGLMath::Vector(lx, ly, lz, 1.0);
				transLVec = matrix * transLVec;
				double transL[4] = {transLVec[0], transLVec[1], transLVec[2], transLVec[3]};
				double rotL[4] = {1.0, 0.0, 0.0, 0.0};
				m_RendererSet->setViewingParameters(m_F2DockRendererId + 2, transL, rotL, 200.0);

				return;
			}

			m_F2DockLigandIndex = -1;
			errorDialog("Error adding new data.");
		}
	}
}


void MainWindow::updateViewingOptions(moleculeVisData *mv)
{
	mv->moleculeUofBData->setTransformation(mv->trans);
	mv->moleculeSurfaceData->setTransformation(mv->trans);

	if(mv->renderType == 0)
	{
		if(mv->goaAvailable)
			m_DataManager->setVisible(mv->goaDataManagerIndex, true);					
		if(mv->surfaceAvailable)
			m_DataManager->setVisible(mv->surfaceDataManagerIndex, false);				
	}
	else if(mv->renderType == 1)
	{
		if(mv->goaAvailable)
			m_DataManager->setVisible(mv->goaDataManagerIndex, false);					
		if(mv->surfaceAvailable)
			m_DataManager->setVisible(mv->surfaceDataManagerIndex, true);	
	}
	if(mv->renderType == 2)
	{
		/* write code to generate and add potential data */ 
	}
}


//F2Dock launcher
void MainWindow::dockingSlot()
{
	masterDial = new F2DockMasterDialog(this);
//	f2dThread = new F2DockThread(this);
//	f2dThread->setMaster(masterDial);
	connect( masterDial, SIGNAL( resultSelectedFromUI(int) ), this, SLOT( resultSelectedInDockingSlot(int) ) );
	connect( masterDial, SIGNAL( resultUILaunched() ), this, SLOT( resultUILaunchedInDockingSlot() ) );
	connect( masterDial, SIGNAL( vizSettingsChanged() ), this, SLOT( vizSettingsChangedSlot() ) );

//	f2dThread->start();
	masterDial->show();
}


void MainWindow::resultUILaunchedInDockingSlot()
{
	receptorName = masterDial->getReceptor();
	ligandName = masterDial->getLigand();

	receptorData = new moleculeVisData();
	ligandData = new moleculeVisData();


	//read ligand pdb and create a BallAndStickDataManager for this

	GOALoader* gLoader = new GOALoader();
	ligandData->moleculeGOA = gLoader->loadFile(ligandName.c_str());
	delete gLoader;
	if (!ligandData->moleculeGOA)
	{
		cout<<"Could not parse PDB"<<endl;
		return;
	}
	ligandData->moleculeUofBData = new BallAndStickData(m_DataManager);
	QStringList temp;
	temp.append(ligandName.c_str());
	ligandData->moleculeUofBData->read(temp);
//	ligandData->moleculeUofBData->m_GroupOfAtoms.push_back(ligandData->moleculeGOA);
	cout<<"Read ligand pdb"<<endl;

	//generate ligand surface and create SurfaceDataManager
	cout<<"Ligand surface generation:"<<endl;
	#ifdef MSA
	ligandData->moleculeSurface = surfaceUsingAdaptiveGrid(ligandData->moleculeGOA, 128);
	ligandData->moleculeSurfaceData = new SurfaceData(m_DataManager);
	ligandData->moleculeSurfaceData->setGeometry(ligandData->moleculeSurface);
	cout<<"Ligand surface generated"<<endl;
	#else
	cout<<"MolSurf API was not enabled in this build\n" << endl;
	#endif


	//read receptor pdb and create a BallAndStickDataManager for this

	gLoader = new GOALoader();
	receptorData->moleculeGOA = gLoader->loadFile(receptorName.c_str());
	delete gLoader;
	if (!receptorData->moleculeGOA)
	{
		cout<<"Could not parse PDB"<<endl;
		return;
	}
	receptorData->moleculeUofBData = new BallAndStickData(m_DataManager);
	QStringList temp2;
	temp2.append(receptorName.c_str());
	receptorData->moleculeUofBData->read(temp2);
//	receptorData->moleculeUofBData->m_GroupOfAtoms.push_back(receptorData->moleculeGOA);
	cout<<"Read receptor pdb"<<endl;

	//generate receptor surface and create SurfaceDataManager
	cout<<"Receptor surface generation:"<<endl;
	#ifdef MSA
	receptorData->moleculeSurface = surfaceUsingAdaptiveGrid(receptorData->moleculeGOA, 128);
	receptorData->moleculeSurfaceData = new SurfaceData(m_DataManager);
	receptorData->moleculeSurfaceData->setGeometry(receptorData->moleculeSurface);
	cout<<"Receptor surface generated"<<endl;
	#else
	cout<<"MolSurf API was not enabled in this build\n" << endl;
	#endif

	
	// adding the dataset to the Datamanager
	
	if(m_DataManager->addData(ligandData->moleculeUofBData, QString(ligandName.c_str())))
	{
		ligandData->goaDataManagerIndex = m_DataManager->m_NextUniqueAvailableId - 1;
		ligandData->goaAvailable = true;
		m_DataManager->setVisible(false);
	}

	if(m_DataManager->addData(receptorData->moleculeUofBData, QString(receptorName.c_str())))
	{
		receptorData->goaDataManagerIndex = m_DataManager->m_NextUniqueAvailableId - 1;
		receptorData->goaAvailable = true;
		m_DataManager->setVisible(true);
	}

	if(m_DataManager->addData(ligandData->moleculeSurfaceData, QString(string(ligandName + ".raw").c_str())))
	{
		ligandData->surfaceDataManagerIndex = m_DataManager->m_NextUniqueAvailableId - 1;
		ligandData->surfaceAvailable = true;
		m_DataManager->setVisible(false);
	}

	if(m_DataManager->addData(receptorData->moleculeSurfaceData, QString(string(receptorName + ".raw").c_str())))
	{
		receptorData->surfaceDataManagerIndex = m_DataManager->m_NextUniqueAvailableId - 1;
		receptorData->surfaceAvailable = true;
		m_DataManager->setVisible(false);
	}

	//setting up the viz settings

	ResultDialog *resultDial  = masterDial->getResultDialog();
	bool isSurface = resultDial->isSurface();
	bool isPotential = resultDial->isPotential();
	int potType = resultDial->getPotentialType();
	bool multipleLigand = resultDial->isMultipleLigand();

	if(isPotential)
	{
		ligandData->renderType = 2;
		ligandData->potentialType = potType;

		receptorData->renderType = 2;
		receptorData->potentialType = potType;
	}
	else if(isSurface)
	{
		ligandData->renderType = 1;
		receptorData->renderType = 1;
	}
	else
	{
		ligandData->renderType = 0;
		receptorData->renderType = 0;
	}

	m_DataManager->updateUIWithNewData();
}


void MainWindow::vizSettingsChangedSlot()
{
	//reading in visualization settings
	ResultDialog *resultDial  = masterDial->getResultDialog();
	bool isSurface = resultDial->isSurface();
	bool isPotential = resultDial->isPotential();
	int potType = resultDial->getPotentialType();
	bool multipleLigand = resultDial->isMultipleLigand();

	if(isPotential)
	{
		ligandData->renderType = 2;
		ligandData->potentialType = potType;

		receptorData->renderType = 2;
		receptorData->potentialType = potType;
	}
	else if(isSurface)
	{
		ligandData->renderType = 1;
		receptorData->renderType = 1;
	}
	else
	{
		ligandData->renderType = 0;
		receptorData->renderType = 0;
	}

	updateViewingOptions(ligandData);
	updateViewingOptions(receptorData);

	m_DataManager->updateUIWithNewData();
}


void MainWindow::resultSelectedInDockingSlot(int index)
{
	ResultDialog *resultDial  = masterDial->getResultDialog();
	double* t = resultDial->getMat(index);

	ligandData->trans = CCVOpenGLMath::Matrix( t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8], t[9], t[10], t[11], 0.0, 0.0, 0.0, 1.0 );
	updateViewingOptions(ligandData);
	updateViewingOptions(receptorData);

	m_DataManager->updateUIWithNewData();
}


/*
void MainWindow::resultSelectedInDockingSlot(int index)
{
	string receptorPDBName = masterDial->getReceptor();
	string ligandPDBName = masterDial->getLigand();

	if(receptorPDBName.compare(receptorName) || ligandPDBName.compare(ligandName) )		// new set of pdbs. refreshing all internal params. reading pdbs
	{
		numSelectedResults = 0;
		transformations.clear();
		result_indices.clear();
		surfaceGenerated = false;
		PBGenerated = false;
		GBGenerated = false;
		CoulGenerated = false;
		receptorRendered = false;

		receptorName = receptorPDBName;
		ligandName = ligandPDBName;

		GOALoader* gLoader = new GOALoader();
		ligandMolecule = gLoader->loadFile(ligandName.c_str());
		delete gLoader;

		if (!ligandMolecule)
		{
			cout<<"Could not parse PDB"<<endl;
			return;
		}
		cout<<"Read ligand pdb"<<endl;


		gLoader = new GOALoader();
		receptorMolecule = gLoader->loadFile(receptorName.c_str());
		delete gLoader;

		if (!receptorMolecule)
		{
			cout<<"Could not parse PDB"<<endl;
			return;
		}
		cout<<"Read receptor pdb"<<endl;

		ligandUofBData = new BallAndStickData(m_DataManager);
		receptorUofBData = new BallAndStickData(m_DataManager);
	}


	//reading in visualization settings
	ResultDialog *resultDial  = masterDial->getResultDialog();
	bool isSurface = resultDial->isSurface();
	bool isPotential = resultDial->isPotential();
	int potType = resultDial->getPotentialType();
	bool multipleLigand = resultDial->isMultipleLigand();


	//reading in the conformation
	double * trans = resultDial->getMat(index);


	//Adding newly selected conformations
	transformations.push_back(trans);	
	result_indices.push_back(index);
	numSelectedResults ++;


	//generating the surface is neccessary
	if(isSurface && !surfaceGenerated)
	{
		//generate ligand surface

		ligandSurface = surfaceUsingAdaptiveGrid(ligandMolecule, 128);

		ligandSurfaceData = new SurfaceData(m_DataManager);
		ligandSurfaceData->setGeometry(ligandSurface);

		cout<<"Ligand surface generated"<<endl;


		//generate receptor surface

		receptorSurface = surfaceUsingAdaptiveGrid(receptorMolecule, 128);

		receptorSurfaceData = new SurfaceData(m_DataManager);
		receptorSurfaceData->setGeometry(receptorSurface);

		cout<<"Receptor surface generated"<<endl;

		surfaceGenerated = true;
		receptorRendered = false;
	}

	//generating potential on the surface is neccessary
	if(isPotential && !(PBGenerated || GBGenerated || CoulGenerated))
	{

	}

	
	//Display the receptor. if it is not already displayed

	if(!receptorRendered)
	{
		cout<<"trying to render receptor"<<endl;

		if(!isSurface && !isPotential)
		{
			cout<<"UofB"<<endl;

			QStringList temp;
			QString receptor(receptorName.c_str());
			temp.append(receptor);

			if(!m_DataManager->addNewDataSet(temp)) 
			{
				printf("error displaying results");
		      		return;
		    	}
		}
		else
		{

		}

		updateUIWithNewData();

		receptorRendered = true;
	}


	if(!isSurface && !isPotential)
	{
		cout<<"trying to render ligand"<<endl;

		//transform the ligand

		FILE *ifp = fopen(ligandPDBName.c_str(), "rt");

		if (ifp == NULL)
		{
			fprintf(stderr, "\n\nError: Failed to open file ( %s )!\n\n", ligandPDBName.c_str());
			return;
		}

		string newLigandPDB;
		newLigandPDB.assign(ligandPDBName, 0, ligandPDBName.length() - 4);
		char conf[20];
		sprintf(conf,"%0d", index);
		string newLigandPDBName = newLigandPDB;
		newLigandPDBName += conf;
		newLigandPDBName += ".pdb";

		FILE* ofp = fopen(newLigandPDBName.c_str(), "wt");

		if (ofp == NULL)
		{
			fprintf(stderr, "\n\nError: Failed to create transformed PDB file ( %s )!\n\n", newLigandPDBName.c_str());
			return;
			fclose(ifp);
		}

		fseek(ifp, 0, SEEK_SET);
		char line[ 500 ];
		int l = 0;

		while (fgets(line, 300, ifp) != NULL)
		{
			l++;
			if (strncmp(line, "ATOM", 4) && strncmp(line, "HETATM", 6))
			{
				fprintf(ofp, "%s", line);
				continue;
			}
			double x, y, z;
			if (sscanf(line + 30, "%lf %lf %lf", &x, &y, &z) != 3)
			{
				fprintf(stderr, "\n\nError: Failed to read coordinates from line %d of input PDB file!\n\n", l);
				return;
			}
			double nx = trans[  0 ] * x + trans[  1 ] * y + trans[  2 ] * z + trans[  3 ],
				   ny = trans[  4 ] * x + trans[  5 ] * y + trans[  6 ] * z + trans[  7 ],
				   nz = trans[  8 ] * x + trans[  9 ] * y + trans[ 10 ] * z + trans[ 11 ];
			line[ 30 ] = 0;
			fprintf(ofp, "%s%8.3lf%8.3lf%8.3lf%s", line, nx, ny, nz, line + 54);
		}

		fclose(ofp);
		fclose(ifp);

		QStringList temp;
		QString ligand(newLigandPDBName.c_str());
		temp.append(ligand);

		if(!m_DataManager->addNewDataSet(temp)) 
		{
			double *m = transformations[numSelectedResults-1];

//			CCVOpenGLMath::Matrix matrix = CCVOpenGLMath::Matrix(m[0], m[1], m[2], m[3],
//										m[4], m[5], m[6], m[7],
//										m[8], m[9], m[10], m[11],
//										0.0, 0.0, 0.0, 1.0);
//
//			m_DataManager->setTransformation( m_DataManager->getNumberOfDataSets()-1, matrix);
			printf("error displaying results");
	      		return;
	    	}
		updateUIWithNewData();
	}

	else if(isSurface)
	{

	}

	else
	{

	}
}
*/

/*
MainWindow::F2DockThread::F2DockThread(MainWindow* par,  unsigned int stackSize)
	: QThread(par)
{
	fdm = new F2DockMasterDialog(par);
	connect( fdm, SIGNAL( resultSelectedFromUI(double *) ), this, SLOT( resultSelectedSlot(double *) ) );
}

void MainWindow::F2DockThread::resultSelectedSlot(double* mat)
{
	cout<<"Recieved signal "<<mat[0]<<endl;
	emit resultSelectedFromMaster(mat);
}

void MainWindow::F2DockThread::run()
{
	cout<<"Launching F2DockMaster"<<endl;
	fdm->show();
}*/


