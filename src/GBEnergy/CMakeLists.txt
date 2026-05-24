cmake_minimum_required(VERSION 3.16)

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ../../inc/GBEnergy/*.[Hh])


# use these in libCG
#ADD_SUBDIRECTORY (CoarseGrain)
#ADD_SUBDIRECTORY (Optimizer)

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

INCLUDE_DIRECTORIES(
#  /h2/wzhao/Software
  ../../inc
)

ADD_LIBRARY (GBEnergy STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

SetupNFFT(GBEnergy)

