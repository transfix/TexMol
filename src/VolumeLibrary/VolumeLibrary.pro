
TEMPLATE = lib
CONFIG   = warn_off staticlib opengl create_prl
TARGET   = VolumeLibrary


INCLUDEPATH  = ../OpenGL_Viewer
INCLUDEPATH += ../UsefulMath
INCLUDEPATH += ../Geometry


win32-g++ {
	INCLUDEPATH += ../include
}


SOURCES =  \
		ClipCube.cpp \
		Extent.cpp \
		FragmentProgramARBImpl.cpp \
		FragmentProgramImpl.cpp \
		Paletted2DImpl.cpp \
		PalettedImpl.cpp \
		Polygon.cpp \
		PolygonArray.cpp \
		Renderer.cpp \
		RendererBase.cpp \
		RGBABase.cpp \
		SGIColorTableImpl.cpp \
		SimpleRGBA2DImpl.cpp \
		SimpleRGBAImpl.cpp \
		UnshadedBase.cpp \
		VolumeRenderer.cpp \
		VolumeRendererFactory.cpp


HEADERS =  \
		ClipCube.h \
		Extent.h \
		FragmentProgramARBImpl.h \
		FragmentProgramImpl.h \
		LookupTables.h \
		Paletted2DImpl.h \
		PalettedImpl.h \
		Polygon.h \
		PolygonArray.h \
		Renderer.h \
		RendererBase.h \
		RGBABase.h \
		SGIColorTableImpl.h \
		SimpleRGBA2DImpl.h \
		SimpleRGBAImpl.h \
		UnshadedBase.h \
		VolumeRenderer.h \
		VolumeRendererFactory.h


