
TEMPLATE = lib
CONFIG   = qt warn_off opengl staticlib create_prl
TARGET   = ObjectRenderer


INCLUDEPATH  = ../OpenGL_Viewer


macx-g++ {
	INCLUDEPATH += /usr/X11R6/include
}
win32-g++ {
	INCLUDEPATH += ../include
}


SOURCES  = ObjectRenderer.cpp
SOURCES += OpenGLShader.cpp
SOURCES += OpenGLShadingLanguageRenderer.cpp
SOURCES += Texture.cpp
SOURCES += TransformationParameters.cpp
SOURCES += Sphere/FunctionImposterSphereRenderer.cpp
SOURCES += Sphere/ImposterSphereRenderer.cpp
SOURCES += Sphere/OpenGLSphereRenderer.cpp
SOURCES += Sphere/PlainImposterSphereRenderer.cpp
SOURCES += Sphere/SphereRenderer.cpp
SOURCES += Sphere/SphereRendererBase.cpp


HEADERS  = ObjectRenderer.h
HEADERS += OpenGLShader.cpp
HEADERS += OpenGLShadingLanguageRenderer.h
HEADERS += Texture.h
HEADERS += TransformationParameters.h
HEADERS += Sphere/FunctionImposterSphereRenderer.h
HEADERS += Sphere/ImposterSphereRenderer.h
HEADERS += Sphere/OpenGLSphereRenderer.h
HEADERS += Sphere/PlainImposterSphereRenderer.h
HEADERS += Sphere/SphereRenderer.h
HEADERS += Sphere/SphereRendererBase.h


