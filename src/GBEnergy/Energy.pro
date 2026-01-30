
TEMPLATE = lib
CONFIG   = qt warn_off staticlib create_prl
TARGET   = Energy


#############################################
#   ENVIRONMENT
#############################################

NFFT   = /h2/wzhao/Software/nfft-3.0.2
LEVMAR = /h2/wzhao/Software/levmar-2.2
TEXMOL = ../..


#############################################
#   INCLUDES
#############################################

INCLUDEPATH  = ../CoarseGrain
INCLUDEPATH += ../FastSummation
INCLUDEPATH += ../Optimizer

INCLUDEPATH += $${LEVMAR}

INCLUDEPATH += $${NFFT}/include/nfft

INCLUDEPATH += $${TEXMOL}/PDBParser
INCLUDEPATH += $${TEXMOL}/UsefulMath


#############################################
#   LIBRARIES
#############################################

LIBS  = ../CoarseGrain/libCoarseGrain.a
LIBS += ../FastSummation/libFastSummation.a
LIBS += ../Optimizer/libOptimizer.a

LIBS += -L$${NFFT}/lib
LIBS += -lnfft3
LIBS += $${NFFT}/applications/fastsum/.libs/libfastsum.a
LIBS += $${NFFT}/applications/fastsum/.libs/libkernels.a

LIBS += $${LEVMAR}/liblevmar.a

LIBS += $${TEXMOL}/PDBParser/libPDBParser.a
LIBS += $${TEXMOL}/UsefulMath/libUsefulMath.a


#############################################
#   HEADERS
#############################################

HEADERS  = energy.h
HEADERS += atom.h
HEADERS += groupOfAtoms.h
HEADERS += PQRParser.h
HEADERS += geometry.h
HEADERS += surface.h
HEADERS += geometryParser.h
HEADERS += polEnergy.h
HEADERS += polForce.h
HEADERS += PBunit.h 


#############################################
#   SOURCES
#############################################

SOURCES  = energy.cpp
SOURCES += atom.cpp
SOURCES += groupOfAtoms.cpp
SOURCES += PQRParser.cpp
SOURCES += surface.cpp
SOURCES += geometryParser.cpp
SOURCES += bezier.cpp
SOURCES += polEnergy.cpp
SOURCES += polForce.cpp


