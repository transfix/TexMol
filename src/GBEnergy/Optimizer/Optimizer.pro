
TEMPLATE = lib
CONFIG   = qt warn_off staticlib create_prl
TARGET   = Optimizer


#############################################
#   ENVIRONMENT
#############################################

LEVMAR = /h2/wzhao/Software/levmar-2.2
TEXMOL = ../..


#############################################
#   INCLUDES
#############################################

INCLUDEPATH  = $${LEVMAR}

INCLUDEPATH += $${TEXMOL}/PDBParser
INCLUDEPATH += $${TEXMOL}/UsefulMath


#############################################
#   LIBRARIES
#############################################

LIBS  = $${LEVMAR}/liblevmar.a

LIBS += $${TEXMOL}/PDBParser/libPDBParser.a
LIBS += $${TEXMOL}/UsefulMath/libUsefulMath.a


#############################################
#   HEADERS
#############################################

HEADERS  = optimizer.h
HEADERS += optimizationFunction.h
HEADERS += geometry.h
HEADERS += surface.h


#############################################
#   SOURCES
#############################################

SOURCES  = optimizer.cpp
SOURCES += optimizationFunction.cpp
SOURCES += surface.cpp


