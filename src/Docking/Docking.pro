
TEMPLATE = lib
CONFIG   = qt warn_off staticlib create_prl
TARGET   = Docking


INCLUDEPATH  = ../GeometryRenderer
INCLUDEPATH += ../UsefulMath
INCLUDEPATH += ../PDBParser
INCLUDEPATH += ../Blurmaps
INCLUDEPATH += ../DataManager
INCLUDEPATH += ../IsocontourRenderer
INCLUDEPATH += ../contour
INCLUDEPATH += ../OpenGL_Viewer
INCLUDEPATH += ../GOAFileTypes
INCLUDEPATH += ../Geometry
INCLUDEPATH += ../SignDistanceFunction
INCLUDEPATH += ../VolumeFileTypes
INCLUDEPATH += ../SmoothingFunction
INCLUDEPATH += ../


SOURCES  = ApbsFormatResidue.cpp
SOURCES += Compare.cpp
SOURCES += Complex.cpp
SOURCES += Connector.cpp
SOURCES += DockingMain.cpp
SOURCES += DockingManager.cpp
SOURCES += DomainComplex.cpp
SOURCES += DomainConnector.cpp
SOURCES += Domain.cpp
SOURCES += erfs.cpp
SOURCES += fastfft.cpp
SOURCES += fullfft.cpp
SOURCES += Flexibility.cpp
SOURCES += FlexibleLoop.cpp
SOURCES += GaussianFFT.cpp
SOURCES += Interface.cpp
SOURCES += IO.cpp
SOURCES += PrintTransformations.cpp
SOURCES += Ranking.cpp
SOURCES += RotamerFlexComplex.cpp
SOURCES += ScoreModule.cpp
SOURCES += Segment.cpp
SOURCES += AuxBH.cpp
SOURCES += TopValues.cpp
SOURCES += ValuePosition3D.cpp
                                

HEADERS  = ApbsFormatResidue.h
HEADERS += Complex.h
HEADERS += Compare.h
HEADERS += Connector.h
HEADERS += DockingMain.h
HEADERS += DockingManager.h
HEADERS += Domain.h
HEADERS += erfs.h
HEADERS += fastfft.h
HEADERS += fullfft.h
HEADERS += DomainComplex.h
HEADERS += DomainConnector.h
HEADERS += DunbrackLibrary.h
HEADERS += Flexibility.h
HEADERS += FlexibleLoop.h
HEADERS += GaussianFFT.h
HEADERS += Interface.h
HEADERS += IO.h
HEADERS += PrintTransformations.h
HEADERS += Ranking.h
HEADERS += RotamerFlexComplex.h
HEADERS += ScoreModule.h
HEADERS += Segment.h
HEADERS += AuxBH.h
HEADERS += TopValues.h
HEADERS += ValuePosition3D.h
                

