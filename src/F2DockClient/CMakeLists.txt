CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

SetupQT()

FILE(GLOB SOURCE_FILES 
  ./*.cpp
)

if(BUILD_INTERFACESTATS)
  ADD_DEFINITIONS(-DWITH_INTERFACE_STATS)
endif(BUILD_INTERFACESTATS)


FILE(GLOB UI3_FORMS ./*.Qt3.ui)
FILE(GLOB UI4_FORMS ./*.Qt4.ui)


FILE(GLOB INCLUDE_FILES 
  ../../inc/F2DockClient/*.[Hh]
)

SET(MOC_HEADERS
#  ../../inc/F2DockClient/F2dGen.h
  ../../inc/F2DockClient/f2dGenDialog.h
  ../../inc/F2DockClient/quadGenDialog.h
  ../../inc/F2DockClient/rerankDialog.h
  ../../inc/F2DockClient/dockDialog.h
  ../../inc/F2DockClient/nagger.h
  ../../inc/F2DockClient/resultDialog.h
  ../../inc/F2DockClient/f2dockMasterDialog.h
#  ../../inc/F2DockClient/F2DockAdvForm.h
#  ../../inc/F2DockClient/F2DockForm.h
#  ../../inc/F2DockClient/F2DockResultDetailsForm.h
#  ../../inc/F2DockClient/F2DockResult.h
#  ../../inc/F2DockClient/GBRerankForm.h
#  ../../inc/F2DockClient/GBRerankResult.h
#  ../../inc/F2DockClient/GBRerankResultDetailsForm.h
#  ../../inc/F2DockClient/QuadGen.h
#  ../../inc/F2DockClient/F2DockMaster.h
#  ../../inc/F2DockClient/f2dgenQt3.h
#  ../../inc/F2DockClient/f2dockadv.Qt3.h
#  ../../inc/F2DockClient/f2dockmasterQt3.h
#  ../../inc/F2DockClient/f2dock.Qt3.h
#  ../../inc/F2DockClient/f2dockresultdetails.Qt3.h
#  ../../inc/F2DockClient/f2dockresultQt3.h
#  ../../inc/F2DockClient/gbrerank.Qt3.h
#  ../../inc/F2DockClient/gbrerankresultdetails.Qt3.h
#  ../../inc/F2DockClient/gbrerankresultQt3.h
#  ../../inc/F2DockClient/quadgenQt3.h
)


SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("UI Files" FILES ${QT_UI_FORMS})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
  ${CMAKE_BINARY_DIR}/src/F2DockClient
  ../../inc/F2DockClient
)

if(QT3_FOUND)
  SET(UI_FORMS ${UI3_FORMS})
  SOURCE_GROUP("UI Files" FILES ${UI_FORMS})

  QT_WRAP_CPP(${PROJECT_NAME} MOC_SOURCES ${MOC_HEADERS})
#  QT_WRAP_UI(${PROJECT_NAME} UI_H UI_CPP ${UI_FORMS})

  SET(UI_FILES ${UI_H} ${UI_CPP})
elseif(QT4_FOUND)
  SET(UI_FORMS ${UI4_FORMS})
  SOURCE_GROUP("UI Files" FILES ${UI_FORMS})
  
  QT4_WRAP_CPP(MOC_SOURCES ${MOC_HEADERS})
  QT4_WRAP_UI(UI_FILES ${UI_FORMS})  
endif(QT3_FOUND)


#QT_WRAP_UI(F2DockClient UI_H UI_CPP ${QT_UI_FORMS})
#QT_WRAP_CPP(F2DockClient MOC_SOURCES ${MOC_HEADERS})

ADD_LIBRARY(F2DockClient STATIC 
	${SOURCE_FILES}
	${INCLUDE_FILES}
	${UI_H}
	${UI_CPP}
	${MOC_SOURCES}
	${UI_FILES}
) 


SET(FDOCK_LINK_LIBS F2DockClient XmlRPC)

ADD_EXECUTABLE (SubmitDockingJob SubmitDockingJob.cpp)
TARGET_LINK_LIBRARIES(SubmitDockingJob ${FDOCK_LINK_LIBS})

ADD_EXECUTABLE (CheckStatus CheckStatus.cpp)
TARGET_LINK_LIBRARIES(CheckStatus ${FDOCK_LINK_LIBS})

ADD_EXECUTABLE (GetResults GetResults.cpp)
TARGET_LINK_LIBRARIES(GetResults ${FDOCK_LINK_LIBS})
