
TEMPLATE = lib
CONFIG   = qt warn_off staticlib opengl create_prl
TARGET   = GeometryRenderer


INCLUDEPATH  = ../Geometry
INCLUDEPATH += ../OpenGL_Viewer
INCLUDEPATH += ../UsefulMath

linux-g++ {
	unix:INCLUDEPATH += ../include/Cg.linux
}
linux-g++-64 {
	unix:INCLUDEPATH += ../include/Cg.linux
}
win32-g++ {
	INCLUDEPATH += $(CG_INC_PATH)
	INCLUDEPATH += ../include
}
macx-g++ {
	unix:INCLUDEPATH += ../include/Cg.MacOSX
}


SOURCES  = BumpMapRenderer.cpp
SOURCES += GeometryRenderer.cpp
SOURCES += TransparentVolumeRenderer.cpp 
# SOURCES += DepthPeelingRenderer.c


HEADERS  = BumpMapping.h
HEADERS += BumpMapRenderer.h
HEADERS += GeometryRenderer.h
HEADERS += TransparentVolumeRenderer.h 
# HEADERS += DepthPeelingRenderer


