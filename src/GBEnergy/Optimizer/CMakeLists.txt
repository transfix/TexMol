cmake_minimum_required(VERSION 3.16)

FILE(GLOB SOURCE_FILES *.cpp)
FILE(GLOB INCLUDE_FILES ../../../inc/Optimizer/*.[Hh])

SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})

INCLUDE_DIRECTORIES(
  #/h2/wzhao/Software/nfft-3.0.2/include/nfft
  ../../../inc
  
)

ADD_LIBRARY (Optimizer STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

