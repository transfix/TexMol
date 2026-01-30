
TEMPLATE = lib
CONFIG   = qt warn_off staticlib opengl create_prl
TARGET   = OpenGL_Viewer


## need this on Sun machines.
# DEFINES += GLX_get_proc_address


INCLUDEPATH  = ../Geometry
INCLUDEPATH += ../GeometryRenderer
INCLUDEPATH += ../UsefulMath


win32-g++ {
	INCLUDEPATH += ../include
}


SOURCES  = Extents.cpp
SOURCES += Grid.cpp
SOURCES += MyExtensions.cpp
SOURCES += OpenGL_Viewer.cpp
SOURCES += OrthographicView.cpp
SOURCES += PerspectiveView.cpp
SOURCES += Plane.cpp
SOURCES += View.cpp
SOURCES += ViewFrustrum.cpp


HEADERS  = ExtensionPointers.h
HEADERS += Extents.h
HEADERS += Grid.h
HEADERS += OpenGL_Viewer.h
HEADERS += OrthographicView.h
HEADERS += PerspectiveView.h
HEADERS += Plane.h
HEADERS += StaticExtensionPointers.h
HEADERS += View.h
HEADERS += ViewFrustrum.h


