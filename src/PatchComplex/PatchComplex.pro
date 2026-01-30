
TEMPLATE = lib
CONFIG   = qt warn_off staticlib create_prl
TARGET   = PatchComplex


INCLUDEPATH  = ../PDBParser
INCLUDEPATH += ../UsefulMath


SOURCES  = ConcavePatch.cpp
SOURCES += PatchComplex.cpp 


HEADERS  = ConcavePatch.h
HEADERS += PatchComplex.h


