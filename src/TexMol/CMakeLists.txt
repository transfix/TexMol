CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

SetupQT()

find_package(OpenGL)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

# arand note: WITH_CG defined is outdated... 
#             in the future use WITH_NFFT
IF (HAVE_NFFT)
ADD_DEFINITIONS(-DWITH_CG)
ADD_DEFINITIONS(-DWITH_NFFT)
ELSE (HAVE_NFFT)
ADD_DEFINITIONS(-UWITH_CG)
ADD_DEFINITIONS(-UWITH_NFFT)
ENDIF (HAVE_NFFT)


IF (HAVE_CGAL)
  ADD_DEFINITIONS(-DWITH_CGAL)
ELSE (HAVE_CGAL)
  ADD_DEFINITIONS(-UWITH_CGAL)
ENDIF (HAVE_CGAL)

IF (HAVE_PETSC)
  ADD_DEFINITIONS(-DWITH_PETSC)
  INCLUDE_DIRECTORIES(../../inc/KIFMM3D)
  INCLUDE_DIRECTORIES(../../inc/KIFMM3D/common)
  INCLUDE_DIRECTORIES(../../inc/KIFMM3D/fmm3d)
  INCLUDE_DIRECTORIES($ENV{PETSC_DIR}/$ENV{PETSC_ARCH}/include)
  INCLUDE_DIRECTORIES($ENV{PETSC_DIR}/include)
  INCLUDE_DIRECTORIES($ENV{PETSC_DIR}/include/mpiuni)
  LINK_DIRECTORIES($ENV{PETSC_DIR}/$ENV{PETSC_ARCH}/lib)
ELSE (HAVE_PETSC)
  ADD_DEFINITIONS(-UWITH_PETSC)
ENDIF (HAVE_PETSC)


# for HDF5
find_package(HDF5 COMPONENTS C CXX)

# this was suggested for building on the Mac by
# a random google search...
#INCLUDE(FindOpenGL)

FILE(GLOB SOURCE_FILES 
  ./*.cpp
  ./Dialogs/*.cpp
)
FILE(GLOB INCLUDE_FILES 
  ../../inc/TexMol/*.[Hh]
  ../../inc/TexMol/Dialogs/*.[Hh]
)
SET(MOC_HEADERS
  ../../inc/TexMol/Dialogs/ConstructCurvaturesDialog.h
  ../../inc/TexMol/Dialogs/AboutDialog.h
  ../../inc/TexMol/Dialogs/ConstructVolumesDialog.h
  ../../inc/TexMol/Dialogs/MainWindow.h
  ../../inc/TexMol/Dialogs/RenderAreaWidgetBase.h
  ../../inc/TexMol/DownloadPDB.h
  ../../inc/TexMol/Dialogs/TransformationsDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/SurfaceDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/SurfaceAreaAndVolumeDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/SliceDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/SetViewingParametersDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/ScriptsDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/MovieImageFileSaveDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/MouseHandlerDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/LightsDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/FileSaveDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/DownloadPDBDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/ConstructVolumesDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/ConstructDepthColoredVolumesDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/ConstructCurvaturesDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/AboutDialogBase.Qt3.h
  ../../inc/TexMol/Dialogs/PoissonBoltzmannDialog.h
  ../../inc/TexMol/Dialogs/GeneralizedBornDialog.h
  ../../inc/TexMol/Dialogs/GeneralizedBornForceDialog.h
  ../../inc/TexMol/Dialogs/SaveTiledImageDialog.h

)
#FILE(GLOB QT_UI_FORMS 
#  ./*.ui
#  ./Dialogs/*.ui
#)

FILE(GLOB UI3_FORMS ./*.Qt3.ui ./Dialogs/*.Qt3.ui)
FILE(GLOB UI4_FORMS ./*.Qt4.ui ./Dialogs/*.Qt4.ui)

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

INCLUDE_DIRECTORIES(
  ../../inc
  ${CMAKE_BINARY_DIR}/src/TexMol  #for generated headers
  ../../inc/ColorTable2  #for constructDepthColoredVolumesDialogBase.ui
  ${CMAKE_BINARY_DIR}/src/F2DockClient
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


set(MACOSX_BUNDLE_ICON_FILE ${TexMol_SOURCE_DIR}/dat/TexMol.icns)

ADD_EXECUTABLE (TexMol MACOSX_BUNDLE
  ${SOURCE_FILES}
  ${INCLUDE_FILES}
  ${MOC_SOURCES}
  ${MOC_HEADERS}
  ${UI_H}
  ${UI_CPP}
  ${UI_FILES}
) 


SET(TEXMOL_LINK_LIBS
	ComputeNormals
	Decimation
	ASpline
        OptimizerLib	
	F2DockClient
	XmlRPC
#	F2DockAnimator
#	Docking
	DataManager 
	SimpleVolumeData
#	SimpleCuration
	#AdaptiveIsocontouring
	Blurmaps
	ByteOrder
	C2C
	arithlib
	libdjvu
	Contour
	ColorSurfaceByFunction
	Contouring
	Curvatures
	GeometryRenderer
	Geometry
	GeometryFileTypes
	HLevelSet
	LBIE_lib
	ARM
	Optimizer
	MolecularCharacteristics
	Pocket
	SignDistanceFunction_v2
	SmoothingFunction
	SphericalPatchIntersections
	Summation
	Utility
	UsefulMath
	VolumeFileTypes
	Nurbs
	vis-lib
	fast-hydro
	fast-resCont
	DPG
	Contour
	OpenGL_Viewer
	ImposterRenderer
	VolumeLibrary
	SplattingRenderer
	Histogram
	ObjectRenderer
	#AdaptiveIsocontouring
	Curvatures
	SignDistanceFunction
	RayTracer
	PatchComplex
	GOAFileTypes
	PDBParser
	ColorTable2
	ColorManager
	pthread
)




IF(ENABLE_MULTIRESOL_SURFACE)
SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS}
	MultiresolutionSurface
)
ENDIF(ENABLE_MULTIRESOL_SURFACE)

IF (LAPACK_FOUND)
   message("Link Lapack for TexMol: ${LAPACK_LIBRARIES} ${BLAS_LIBRARIES}")

SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS}
   ${BLAS_LIBRARIES}
   ${LAPACK_LIBRARIES}
)
ELSE(LAPACK_FOUND)
   message(STATUS "Lapack was not found")

ENDIF (LAPACK_FOUND)

IF (ENABLE_POCKET_TUNNEL)
SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS}
   PocketTunnel
)
ENDIF (ENABLE_POCKET_TUNNEL)
IF (ENABLE_SECONDARY_STRUCTURE)
SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS}
   SecondaryStructures
)
ENDIF (ENABLE_SECONDARY_STRUCTURE)

if(BUILD_INTERFACESTATS)
SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS} 
  InterfaceStats
)
endif(BUILD_INTERFACESTATS)

if(BUILD_MOLSURF_API)
SET(TEXMOL_LINK_LIBS MolSurfAPI
  ${TEXMOL_LINK_LIBS} 
)
endif(BUILD_MOLSURF_API)

IF (HAVE_NFFT)
SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS} 
  CoarseGrain
  GBEnergy
  levmar
)
ENDIF (HAVE_NFFT)

IF (HAVE_PETSC)
SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS} 
  PB2
  TriaElement
  KIFMM3D
  KIFMM3DCommon
)
ENDIF (HAVE_PETSC)

IF (HAVE_NFFT)
SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS} 
FastSummation ${NFFT_LIB} fftw3)
ENDIF (HAVE_NFFT)

IF (HAVE_FFTW)
SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS} 	
  ${FTWD_LIB}
  ${FFTWD_THREADS_LIB}
)
ENDIF (HAVE_FFTW)

SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS} 	
  MolEnergyUtil
  ${QT_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CG_LIBRARY}
  ${CG_GL_LIBRARY}
#  GL
#  GLU
  glew
#  ${OPENGL_LIBRARIES}
)


if(HDF5_FOUND)
SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS}
  ${HDF5_LIBRARIES}
)
ENDif(HDF5_FOUND)


IF (HAVE_PETSC)
SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS} 
  petsc
  mpich
)

IF (APPLE)
SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS} 
# I needed these on the mac... set this up so it is a swtich...
  f2cblas
  f2clapack
  pmpich
  ${X11_LIBRARIES}
)
ENDIF (APPLE)

ENDIF (HAVE_PETSC)


SET(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS} 
  gfortran
)


SetupBoost(TexMol)

# for secondary structure includes
ADD_DEFINITIONS(-DHUGE=1000000)

find_package(CGAL)
if(CGAL_FOUND)
  include(${CGAL_USE_FILE})
  # need the following flags in case CGAL has some special compiler needs for this compiler
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CGAL_CXX_FLAGS_INIT}")
  set(TEXMOL_LINK_LIBS ${TEXMOL_LINK_LIBS} ${CGAL_LIBRARIES})
endif(CGAL_FOUND)

if(CG_FOUND)
  include_directories(
    ${CG_INCLUDE_PATH}
  )
endif(CG_FOUND)

TARGET_LINK_LIBRARIES(TexMol ${TEXMOL_LINK_LIBS})


#ADD_EXECUTABLE(TexMolServer Server.cpp  ${SOURCE_FILES}
#  ${INCLUDE_FILES})
#TARGET_LINK_LIBRARIES(TexMolServer ${TEXMOL_LINK_LIBS})


# new cpack stuff for clean install...


if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.4)
  if(APPLE)
    set_target_properties(TexMol PROPERTIES
      OUTPUT_NAME ${CMAKE_BUNDLE_NAME})
  endif(APPLE)
  set(CMAKE_INSTALL_DESTINATION_ARGS 
    BUNDLE DESTINATION "${CMAKE_BUNDLE_LOCATION}")
endif(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 2.4)

install(TARGETS TexMol
        RUNTIME DESTINATION bin ${CMAKE_INSTALL_DESTINATION_ARGS}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# update after writing this script
if(UNIX AND NOT APPLE)
  install(PROGRAMS "${TexMol_SOURCE_DIR}/bin/runTexMol" DESTINATION bin)
endif(UNIX AND NOT APPLE)

# install rules for including 3rd party libs such as Qt
# if a system Qt is used (e.g. installed in /usr/lib/), it will not be included in the installation
set(fixup_exe "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/TexMol${CMAKE_EXECUTABLE_SUFFIX}")
if(APPLE)
  set(fixup_exe "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/MacOS/${CMAKE_BUNDLE_NAME}")
endif(APPLE)

install(CODE "
  include(\"${TexMol_SOURCE_DIR}/CMake/BundleUtilities.cmake\")
  fixup_bundle(\"${fixup_exe}\" \"\" \"/usr/local/lib;${QT_LIBRARY_DIR};${QT_BINARY_DIR};${Boost_LIBRARY_DIRS};${BUNDLE_LIB_SEARCH_DIRS};${CG_LIBRARY_DIR};${LAPACK_LIBRARY_DIR};${LAPACK_BIN_DIR}\")
")

#CHA: trying to add required libraries (dlls) for windows installer
#     as it doesn't do it automatically as in linux build.

if( WIN32 )
   message(STATUS "Start set up dll installs" )
   install(PROGRAMS "C:\\MinGW\\bin\\libquadmath-0.dll" DESTINATION bin)
   install(PROGRAMS "C:\\MinGW\\bin\\libstdc++-6.dll" DESTINATION bin)

   #install Boost dlls manually - add all
   if( Boost_FOUND )
   file( GLOB BOOST_DLLS ${Boost_LIBRARIES}/*.dll )
   if( BOOST_DLLS )
   foreach( Boost_dll_file ${BOOST_DLLS} )
      #list( APPEND TexMol_install_libs ${Boost_dll_file} )
   endforeach( Boost_dll_file ${BOOST_DLLS} )
   endif( BOOST_DLLS )
   endif( Boost_FOUND )

   install(PROGRAMS "${Boost_LIBRARIES}/libboost_thread-gcc46-mt-1_41.dll" DESTINATION bin)

   #install Qt dlls manually
   if ( QT4_FOUND )
    #message( STATUS "Qt4 found: ${QT_BINARY_DIR}" )
    file( GLOB QT_DLLS ${QT_BINARY_DIR}/*.dll )
    if( QT_DLLS )
    foreach( Qt_dll_file ${QT_DLLS} )
       #message(STATUS "Qt dlls: ${Qt_dll_file}")
       #list( APPEND TexMol_install_libs ${Qt_dll_file} )
    endforeach( Qt_dll_file ${QT_DLLs} )
    endif( QT_DLLS )
   endif (QT4_FOUND )

   #install Cg dlls manually
   if( CG_FOUND )
   file( GLOB CG_DLLS ${CG_LIBRARY_DIR}/*.dll )
   if( CG_DLLS )
   foreach( Cg_dll_file ${CG_DLLS} )
      #list( APPEND TexMol_install_libs ${Cg_dll_file} )
   endforeach( Cg_dll_file ${CG_DLLS} )
   endif( CG_DLLS )
   endif( CG_FOUND )

   if( TexMol_install_libs )
   # remove duplicates 
      list( REMOVE_DUPLICATES TexMol_install_libs )
      install( FILES ${TexMol_install_libs} DESTINATION bin COMPONENT ThirdParty )
   endif( TexMol_install_libs )

endif( WIN32 )

