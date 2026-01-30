CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

SetupQt()

IF (HAVE_CGAL)
ADD_DEFINITIONS(-DWITH_CGAL)
ELSE (HAVE_CGAL)
ADD_DEFINITIONS(-UWITH_CGAL)
ENDIF (HAVE_CGAL)

IF (ENABLE_SECONDARY_STRUCTURE)
ADD_DEFINITIONS(-DWITH_SECSTRUCT)
ELSE (ENABLE_SECONDARY_STRUCTURE)
ADD_DEFINITIONS(-UWITH_SECSTRUCT)
ENDIF (ENABLE_SECONDARY_STRUCTURE)


# for secondary structure includes
ADD_DEFINITIONS(-DHUGE=1000000)

FILE(GLOB SOURCE_FILES 
  ./*.cpp
  ./SurfaceDataManager/*.cpp
  ./BallAndStickDataManager/*.cpp
  ./SecondaryStructureDataManager/*.cpp
  ./NURBSDataManager/*.cpp
  ./VolumeDataManager/*.cpp
)
FILE(GLOB UI3_FORMS 
  ./*.Qt3.ui
  ./SurfaceDataManager/*.Qt3.ui
  ./BallAndStickDataManager/*.Qt3.ui
  ./SecondaryStructureDataManager/*.Qt3.ui
  ./NURBSDataManager/*.Qt3.ui
  ./VolumeDataManager/*.Qt3.ui
)
FILE(GLOB UI4_FORMS 
  ./*.Qt4.ui
  ./SurfaceDataManager/*.Qt4.ui
  ./BallAndStickDataManager/*.Qt4.ui
  ./SecondaryStructureDataManager/*.Qt4.ui
  ./NURBSDataManager/*.Qt4.ui
  ./VolumeDataManager/*.Qt4.ui
)


FILE(GLOB INCLUDE_FILES 
  ../../inc/DataManager/*.[Hh]
  ../../inc/DataManager/BallAndStickDataManager/*.[Hh]
  ../../inc/DataManager/SecondaryStructureDataManager/*.[Hh]
  ../../inc/DataManager/NURBSDataManager/*.[Hh]
  ../../inc/DataManager/SurfaceDataManager/*.[Hh]
  ../../inc/DataManager/VolumeDataManager/*.[Hh]
)

SET(MOC_HEADERS
  ../../inc/DataManager/AbstractData.h
  ../../inc/DataManager/Rover3DWidget.h
  ../../inc/DataManager/BallAndStickDataManager/BallAndStickData.h
  ../../inc/DataManager/BallAndStickDataManager/ColorMapInterface.h
  ../../inc/DataManager/SecondaryStructureDataManager/SecondaryStructureData.h
  ../../inc/DataManager/NURBSDataManager/NURBSData.h
  ../../inc/DataManager/SurfaceDataManager/SurfaceData.h
  ../../inc/DataManager/VolumeDataManager/VolumeData.h
  ../../inc/DataManager/BallAndStickDataManager/ColorLegendBase.Qt3.h
  ../../inc/DataManager/BallAndStickDataManager/ColorMapInterfaceBase.Qt3.h
  ../../inc/DataManager/BallAndStickDataManager/AnglesDialog.Qt3.h
)

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("UI Files" FILES ${QT_UI_FORMS})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
  ${CMAKE_BINARY_DIR}/src/DataManager  #for generated headers
  ../../inc/Histogram  #for SecondaryStructureRenderingPropertiesBase.ui
  ../../inc/ColorTable  #for VolumeRenderingPropertiesBase.ui
)

if(QT3_FOUND)
  SET(UI_FORMS ${UI3_FORMS})
  SOURCE_GROUP("UI Files" FILES ${UI_FORMS})

  QT_WRAP_CPP(${PROJECT_NAME} MOC_SOURCES ${MOC_HEADERS})
  QT_WRAP_UI(${PROJECT_NAME} UI_H UI_CPP ${UI_FORMS})

  SET(UI_FILES ${UI_H} ${UI_CPP})
elseif(QT4_FOUND)
  SET(UI_FORMS ${UI4_FORMS})
  SOURCE_GROUP("UI Files" FILES ${UI_FORMS})
  
  QT4_WRAP_CPP(MOC_SOURCES ${MOC_HEADERS})
  QT4_WRAP_UI(UI_FILES ${UI_FORMS})  
endif(QT3_FOUND)

#QT_WRAP_UI(DataManager UI_H UI_CPP ${QT_UI_FORMS})
#QT_WRAP_CPP(DataManager MOC_SOURCES ${MOC_HEADERS})

set(LIBS ${QT_LIBRARIES})

if(CG_FOUND)
  include_directories(
    ${CG_INCLUDE_PATH}
  )
  set(LIBS ${LIBS} ${CG_LIBRARY} ${CG_GL_LIBRARY})
endif(CG_FOUND)

find_package(CGAL)
if(CGAL_FOUND)
  include(${CGAL_USE_FILE})
  # need the following flags in case CGAL has some special compiler needs for this compiler
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CGAL_CXX_FLAGS_INIT}")
  set(LIBS ${LIBS} ${CGAL_LIBRARIES})
endif(CGAL_FOUND)

ADD_LIBRARY(DataManager STATIC 
	${SOURCE_FILES}
	${INCLUDE_FILES}
	${UI_H}
	${UI_CPP}
	${MOC_SOURCES}
	${UI_FILES}
)

target_link_libraries(DataManager ${LIBS})

SetupBoost(DataManager)
