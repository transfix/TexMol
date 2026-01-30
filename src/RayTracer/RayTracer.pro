TEMPLATE = lib
CONFIG   = qt warn_off staticlib create_prl
TARGET   = RayTracer


INCLUDEPATH  = ../ColorManager
INCLUDEPATH += ../OpenGL_Viewer
INCLUDEPATH += ../UsefulMath


SOURCES =  \
		Buffer.cpp \
		Intersector.cpp \
		RayTracer.cpp \
		Sampler.cpp \
		SimpleSampler.cpp


HEADERS =  \
		Buffer.h \
		Intersector.h \
		RayTracer.h \
		Sampler.h \
		SimpleSampler.h


