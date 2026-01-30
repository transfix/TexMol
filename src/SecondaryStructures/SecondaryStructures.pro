TEMPLATE = lib
CONFIG   = qt warn_off staticlib create_prl
TARGET   = SecondaryStructures

LANGUAGE = C++


INCLUDEPATH  = ../Geometry
INCLUDEPATH += ../UsefulMath
INCLUDEPATH += ../OpenGL_Viewer
INCLUDEPATH += ../DataManager/SecondaryStructureDataManager
INCLUDEPATH += ../DataManager
INCLUDEPATH += ../Histogram


QMAKE_CXXFLAGS += $$(CPPFLAGS)
QMAKE_LFLAGS += $$(LDFLAGS)


SOURCES	 =      init.C \
		robust_cc.C \
		op.C \
		util.C \
		hfn_util.C  \
		intersect.C \
		medax.C  \
		rcocone.C    \
		skel.C     \
		u1.C \
		degen.C  \
		tcocone.C  \
		helix.C  \
		u2.C


contains( QMAKE_CXXFLAGS_RELEASE, -fno-exceptions ) {
        # have to enable exceptions
        QMAKE_CXXFLAGS_RELEASE += -fexceptions
}


dynamic_gl {
}


