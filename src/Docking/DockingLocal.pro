
TEMPLATE = app
CONFIG   = qt warn_off x11 opengl link_prl
TARGET   = Docking


LIBS  = SmoothingFunction/libSmoothingFunction.a
LIBS += UsefulMath/libUsefulMath.a
LIBS += /usr/lib/libpthread.so
LIBS += -lqassistantclient


contains( DEFINES, CVC_USE_QSA )
{
	load( qsa )
}


INCLUDEPATH  = ./UsefulMath
INCLUDEPATH += ./SmoothingFunction


SOURCES +=  \
SOURCES += Compare.cpp
SOURCES += erfs.cpp
SOURCES += fastfft.cpp
SOURCES += fullfft.cpp
SOURCES += GaussianFFT.cpp
SOURCES += Geometry.cpp
SOURCES += Inverse.cpp
SOURCES += IO.cpp
SOURCES += main.cpp
SOURCES += PrintTransformations.cpp
SOURCES += AuxBH.cpp
SOURCES += TopValues.cpp
SOURCES += ValuePosition3D.cpp


HEADERS  = Compare.h
HEADERS += erfs.h
HEADERS += fastfft.h
HEADERS += fullfft.h
HEADERS += GaussianFFT.h
HEADERS += Geometry.h
HEADERS += Inverse.h
HEADERS += IO.h
HEADERS += PrintTransformations.h
HEADERS += AuxBH.h
HEADERS += TopValues.h
HEADERS += ValuePosition3D.h


