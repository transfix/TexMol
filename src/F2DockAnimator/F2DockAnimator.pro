
TEMPLATE = lib
CONFIG   = qt warn_off staticlib create_prl
TARGET   = F2DockAnimator


LANGUAGE = C++


INCLUDEPATH  = ../Docking
INCLUDEPATH += ../PDBParser
INCLUDEPATH += ../UsefulMath
INCLUDEPATH += ../GOAFileTypes


INCLUDEPATH  = ../contour
INCLUDEPATH += ../ColorManager
INCLUDEPATH += ../C2C
INCLUDEPATH += ../Curvatures
INCLUDEPATH += ../ByteOrder
INCLUDEPATH += ../GeometryRenderer
INCLUDEPATH += ../Geometry
INCLUDEPATH += ../GeometryFileTypes
INCLUDEPATH += ../GOAFileTypes
INCLUDEPATH += ../Contouring
INCLUDEPATH += ../Blurmaps
INCLUDEPATH += ../MultiresolutionSurface
INCLUDEPATH += ../PDBParser
INCLUDEPATH += ../UsefulMath
INCLUDEPATH += ../ColorTable
INCLUDEPATH += ../VolumeLibrary
INCLUDEPATH += ../IsocontourRenderer
INCLUDEPATH += ../ImposterRenderer
INCLUDEPATH += ../ObjectRenderer
INCLUDEPATH += ../OpenGL_Viewer
INCLUDEPATH += ../SplattingRenderer
INCLUDEPATH += ../VolumeFileTypes
INCLUDEPATH += ../Nurbs
INCLUDEPATH += ../
INCLUDEPATH += ../SecondaryStructures/
INCLUDEPATH += ../Histogram/


SOURCES	 = F2DockAnimator.cpp
		

