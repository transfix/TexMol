
TEMPLATE = lib
CONFIG   = qt warn_off staticlib opengl create_prl
TARGET   = ImposterRenderer


INCLUDEPATH  = ../OpenGL_Viewer
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
macx-g++ {
	INCLUDEPATH += /usr/X11R6/include
}


SOURCES  = BallRenderer.cpp
SOURCES += FunctionSphereRenderer.cpp
SOURCES += GaussianRenderer.cpp
SOURCES += GlobalCGContext.cpp
SOURCES += HelixRenderer.cpp
SOURCES += HollowCylinderRenderer.cpp
SOURCES += ImposterRenderer.cpp
SOURCES += MeshCylinderRenderer.cpp
SOURCES += MeshSphereRenderer.cpp
SOURCES += MeshHelixRenderer.cpp
SOURCES += PlainSphereRenderer.cpp
SOURCES += sphereDrawing.cpp
SOURCES += StickRenderer.cpp
SOURCES += Texture.cpp


HEADERS  = BallRenderer.h
HEADERS += ExpandableBuffer.h
HEADERS += FunctionSphereRenderer.h
HEADERS += GaussianRenderer.h
HEADERS += GlobalCGContext.h
HEADERS += HelixRenderer.h
HEADERS += HollowCylinderRenderer.h
HEADERS += ImposterRenderer.h
HEADERS += MeshCylinderRenderer.h
HEADERS += MeshHelixRenderer.h
HEADERS += PlainSphereRenderer.h
HEADERS += sphereDrawing.h
HEADERS += StickRenderer.h
HEADERS += Texture.h
HEADERS += cgGLCylFragmentShader.h
HEADERS += cgGLCylVertexShader.h
HEADERS += cgGLFragmentFunctionShader.h
HEADERS += cgGLFragmentShader.h
HEADERS += cgGLHelixFragmentShader.h
HEADERS += cgGLHelixVertexShader.h
HEADERS += cgGLHollowCylinderFragmentShader.h
HEADERS += cgGLHollowCylinderVertexShader.h
HEADERS += cgGLVertexFunctionShader.h
HEADERS += cgGLVertexShader.h


