// Auto-generated stubs for Qt3 dialogs not yet ported (Phase 6 TODO)
// These provide link-time symbols so the executable can build.

#include <QDialog>
#include <QWidget>
#include <QMessageBox>

// AboutDialog
#include <TexMol/Dialogs/AboutDialog.h>
AboutDialog::AboutDialog(QWidget* p, const char*, bool m, Qt::WindowFlags f) : AboutDialogBase(p, f) { setModal(m); }
AboutDialog::~AboutDialog() {}

// ConstructCurvaturesDialog
#include <TexMol/Dialogs/ConstructCurvaturesDialog.h>
ConstructCurvaturesDialog::ConstructCurvaturesDialog(QWidget* p, const char*, bool m, Qt::WindowFlags f) : ConstructCurvaturesDialogBase(p, f) { setModal(m); }
ConstructCurvaturesDialog::~ConstructCurvaturesDialog() {}
bool ConstructCurvaturesDialog::constructCurvature() { return false; }

// ConstructDepthColoredVolumesDialog
#include <TexMol/Dialogs/ConstructDepthColoredVolumesDialog.h>
ConstructDepthColoredVolumesDialog::ConstructDepthColoredVolumesDialog(QString, QWidget* p, const char*, bool m, Qt::WindowFlags f) : ConstructDepthColoredVolumesDialogBase(p, f) { setModal(m); }
ConstructDepthColoredVolumesDialog::~ConstructDepthColoredVolumesDialog() {}
VolumeData* ConstructDepthColoredVolumesDialog::constructDepthColoredVolumesFromVolume(DataManager*, VolumeData*) { return nullptr; }
bool ConstructDepthColoredVolumesDialog::loadFile() { return false; }

// ConstructVolumesDialog
#include <TexMol/Dialogs/ConstructVolumesDialog.h>
ConstructVolumes::ConstructVolumes(QWidget* p, const char*, bool m, Qt::WindowFlags f) : ConstructVolumesDialogBase(p, f) { setModal(m); }
ConstructVolumes::~ConstructVolumes() {}
bool ConstructVolumes::constructVolume(DataManager*, PDBParser::GroupOfAtoms*, VolumeData*) { return false; }

// FileSaveDialog
#include <TexMol/Dialogs/FileSaveDialog.h>
FileSaveDialog::FileSaveDialog(QWidget* p, const char*, bool m, Qt::WindowFlags f) : FileSaveDialogBase(p, f) { setModal(m); }
FileSaveDialog::~FileSaveDialog() {}

// LightsDialog
#include <TexMol/Dialogs/LightsDialog.h>
LightsDialog::LightsDialog(int, LightSet*, QWidget* p, const char*, bool m, Qt::WindowFlags f) : LightsDialogBase(p, f) { setModal(m); }
LightsDialog::~LightsDialog() {}
bool LightsDialog::saveLightInformation(int, LightSet*) { return false; }

// MouseHandlerDialog
#include <TexMol/Dialogs/MouseHandlerDialog.h>
MouseHandlerDialog::MouseHandlerDialog(QWidget* p, const char*, bool m, Qt::WindowFlags f) : MouseHandlerDialogBase(p, f) { setModal(m); }
MouseHandlerDialog::~MouseHandlerDialog() {}

// MovieImageFileSaveDialog
#include <TexMol/Dialogs/MovieImageFileSaveDialog.h>
MovieImageFileSaveDialog::MovieImageFileSaveDialog(QWidget* p, const char*, bool m, Qt::WindowFlags f) : MovieImageFileSaveDialogBase(p, f) { setModal(m); }
MovieImageFileSaveDialog::~MovieImageFileSaveDialog() {}

// ScriptsDialog
#include <TexMol/Dialogs/ScriptsDialog.h>
ScriptsDialog::ScriptsDialog(QWidget* p) : ScriptsDialogBase(p) {}
ScriptsDialog::~ScriptsDialog() {}

// SetViewingParametersDialog
#include <TexMol/Dialogs/SetViewingParametersDialog.h>
SetViewingParametersDialog::SetViewingParametersDialog(CCVOpenGLMath::Quaternion, CCVOpenGLMath::Vector, float, QWidget* p, const char*, bool m, Qt::WindowFlags f) : SetViewingParametersDialogBase(p, f) { setModal(m); }
SetViewingParametersDialog::~SetViewingParametersDialog() {}
bool SetViewingParametersDialog::getParams(double*, double*, double*) { return false; }

// SliceDialog
#include <TexMol/Dialogs/SliceDialog.h>
SliceDialog::SliceDialog(PDBParser::GroupOfAtoms*, QWidget* p, const char*, bool m, Qt::WindowFlags f) : SliceDialogBase(p, f) { setModal(m); }
SliceDialog::~SliceDialog() {}
bool SliceDialog::printParameterization() { return false; }

// SurfaceAreaAndVolumeDialog
#include <TexMol/Dialogs/SurfaceAreaAndVolumeDialog.h>
SurfaceAreaAndVolumeDialog::SurfaceAreaAndVolumeDialog(double, double, QWidget* p, const char*, bool m, Qt::WindowFlags f) : SurfaceAreaAndVolumeDialogBase(p, f) { setModal(m); }
SurfaceAreaAndVolumeDialog::~SurfaceAreaAndVolumeDialog() {}

// SurfaceDialog
#include <TexMol/Dialogs/SurfaceDialog.h>
SurfaceDialog::SurfaceDialog(QWidget* p, const char*, bool m, Qt::WindowFlags f) : SurfaceDialogBase(p, f) { setModal(m); }
SurfaceDialog::~SurfaceDialog() {}
Geometry* SurfaceDialog::getSurface(PDBParser::GroupOfAtoms*) { return nullptr; }

// TransformationsDialog
#include <TexMol/Dialogs/TransformationsDialog.h>
TransformationsDialog::TransformationsDialog(QWidget* p, const char*, bool m, Qt::WindowFlags f) : TransformationsDialogBase(p, f) { setModal(m); }
TransformationsDialog::~TransformationsDialog() {}

// Additional method stubs needed at link time

bool FileSaveDialog::getSaveAll() { return false; }
void FileSaveDialog::getSelectedFileName(QString*, QString*) {}

CCVOpenGLMath::Matrix TransformationsDialog::getTransformations() { return CCVOpenGLMath::Matrix(); }
float TransformationsDialog::getScale() { return 1.0f; }

// DownloadPDB stub
#include <TexMol/DownloadPDB.h>
DownloadPDB::DownloadPDB() {}

// MouseHandler stub (from MouseHandlerHelper)
#include <TexMol/MouseHandlerHelper.h>
MouseHandler::MouseHandler(QWidget*) {}
MouseHandler::~MouseHandler() {}
void MouseHandler::updateUserPreferences() {}
MouseHandler::USER_SELECTED_TRANSFORMATION MouseHandler::getUserSelectedTransformation(QMouseEvent*, bool, bool, bool) { return NO_TRANSFORM; }

void MovieImageFileSaveDialog::getSelectedFileNames(QString*, QString*, QString*) {}
DownloadPDB::~DownloadPDB() {}
bool DownloadPDB::blockedDownload(const char*, const char*) { return false; }
void DownloadPDB::finishedop(bool) {}

// Additional dialog slot stubs needed for AUTOMOC
void ConstructDepthColoredVolumesDialog::outputRawVFileNameSlot() {}
void FileSaveDialog::fileNameSlot() {}
void LightsDialog::ambientColorSlot() {}
void LightsDialog::diffuseColorSlot() {}
void LightsDialog::specularColorSlot() {}
void MovieImageFileSaveDialog::animationFileSlot() {}
void MovieImageFileSaveDialog::baseImageFileSlot() {}
void ScriptsDialog::executeSlot() {}
void ScriptsDialog::loadSlot() {}
void ScriptsDialog::saveSlot() {}
void SurfaceDialog::setupSlot(const QString&) {}
void TransformationsDialog::addGeneralRotationSlot() {}
void TransformationsDialog::addGeneralXFormSlot() {}
void TransformationsDialog::addRotationSlot() {}
void TransformationsDialog::addScaleSlot() {}
void TransformationsDialog::addTranslationSlot() {}

// Include moc output for dialog classes with Q_OBJECT
// Use path prefix so AUTOMOC can find the corresponding headers
#include "TexMol/Dialogs/moc_AboutDialog.cpp"
#include "TexMol/Dialogs/moc_ConstructCurvaturesDialog.cpp"
#include "TexMol/Dialogs/moc_ConstructVolumesDialog.cpp"
#include "TexMol/moc_DownloadPDB.cpp"

// More dialog slot stubs from vtable requirements
void ConstructVolumes::colorMapFileSlot() {}
void ConstructVolumes::constructRawVSlot(bool) {}
void ConstructVolumes::functionTypeChangedSlot(int) {}
void ConstructCurvaturesDialog::inputPDBFileSlot() {}
void ConstructCurvaturesDialog::inputIsocontourFileSlot() {}
void ConstructCurvaturesDialog::outputMeanCurvatureFileSlot() {}
void ConstructCurvaturesDialog::outputGaussianCurvatureFileSlot() {}
void ConstructCurvaturesDialog::isovalueRadioButtonSelectedSlot(bool) {}
void ConstructCurvaturesDialog::isocontourRadioButtonSelectedSlot(bool) {}
