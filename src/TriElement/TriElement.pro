TEMPLATE = lib
CONFIG   = qt warn_off staticlib create_prl
TARGET   = ASpline 


SOURCES  = aspline.cpp
SOURCES += bezier.cpp
SOURCES += geometryParser.cpp
SOURCES += quadrature.cpp
SOURCES += linearpatch.cpp
SOURCES += trielement.cpp
SOURCES += PotGeometryParser.cpp


HEADERS  = aspline.h
HEADERS += geometry.h
HEADERS += geometryParser.h
HEADERS += quadrature.h
HEADERS += linearpatch.h
HEADERS += trielement.h
HEADERS += PotGeometryParser.h

