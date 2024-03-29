PROJECT(TLEDApp)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

FIND_PACKAGE(GLUT)
FIND_PACKAGE(CUDA)

FIND_PATH( GLEW_INCLUDE_PATH GL/glew.h
  /usr/include
  /usr/local/include
  DOC "Glew include directory")
  
FIND_LIBRARY( GLEW_LIBRARY
  NAMES GLEW glew
  PATHS
  /usr/lib64
  /usr/lib
  /usr/local/lib64
  /usr/local/lib
  DOC "GLEW library")


IF(NOT CUDA_SDK_ROOT_DIR)
  MESSAGE(FATAL_ERROR "CUDA_SDK_ROOT_DIR not found, please define or set in your environment.")
ENDIF(NOT CUDA_SDK_ROOT_DIR)

INCLUDE_DIRECTORIES( 
  ${CUDA_SDK_ROOT_DIR}/C/common/inc/ 
)


##########################################################   
# TLED

FIND_PATH(TLED_INCLUDE_PATH NAMES TLEDSolver.h PATHS ~/TLEDSolver/)
FIND_LIBRARY(TLED_LIB NAMES libTLED.a PATHS ~/TLEDSolver/build)

INCLUDE_DIRECTORIES(${TLED_INCLUDE_PATH})

LINK_LIBRARIES( glut ${GLEW_LIBRARY} ${TLED_LIB} HD HDU )

CUDA_ADD_EXECUTABLE(TLEDApp main.cpp )


