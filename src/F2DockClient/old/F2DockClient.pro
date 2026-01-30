TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= create_prl warn_off release staticlib exceptions rtti

INCLUDEPATH	+= ../XmlRPC

HEADERS	+= F2DockForm.h \
	F2DockAdvForm.h \
	F2DockClient.h \
	basicParams.h \
	F2DockResult.h \
	F2DockResultDetailsForm.h \
	detailResults.h \
	summaryResults.h \
	utils.h \
	F2dGen.h \
	f2dgenParams.h \
	rerankBasicParams.h \
	GBRerankForm.h \
	GBRerankResult.h \
	GBRerankResultDetailsForm.h \
	rerankDetailResults.h \
	F2DockMaster.h

SOURCES	+= F2DockForm.cpp \
	F2DockAdvForm.cpp \
	F2DockClient.cpp \
	F2DockResult.cpp \
	F2DockResultDetailsForm.cpp \
	F2dGen.cpp \
	utils.cpp \
	GBRerankForm.cpp \
	GBRerankResult.cpp \
	GBRerankResultDetailsForm.cpp \
	F2DockMaster.cpp \
	QuadGen.cpp

FORMS	= f2dock.ui \
	f2dockadv.ui \
	f2dockresult.ui \
	f2dockresultdetails.ui \
	f2dgen.ui \
	gbrerank.ui \
	gbrerankresult.ui \
	gbrerankresultdetails.ui \
	f2dockmaster.ui \
	quadgen.ui

TARGET += F2DockClient 


QMAKE_CXXFLAGS += $$(CPPFLAGS)
QMAKE_LFLAGS += $$(LDFLAGS)





