
TEMPLATE = lib
CONFIG   = qt warn_off staticlib opengl create_prl
TARGET   = SplattingRenderer

INCLUDEPATH  = ../Blurmaps
INCLUDEPATH += ../ImposterRenderer
INCLUDEPATH += ../OpenGL_Viewer


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
macx-g++ {
	INCLUDEPATH += /usr/X11R6/include
}


SOURCES =  \
		SplattingRenderer.cpp


HEADERS =  \
		SplattingRenderer.h


