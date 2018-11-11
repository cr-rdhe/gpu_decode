cmake_minimum_required(VERSION 2.8)

project(my_project)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)

set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC ")
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_AUTOMOC on)
find_package(Protobuf REQUIRED)

include_directories(
                    ${CMAKE_SOURCE_DIR}/inc/
		    /usr/include/
		    /usr/local/cuda/include
  )

#Add external include directories
include_directories(
  /usr/local/include/
  /usr/local/ffmpeg/include/
  ${PROJECT_SOURCE_DIR}/inc/
  ${PROJECT_SOURCE_DIR}/common/inc/
  )

#Link external libraries diectories
link_directories(
  /usr/local/ffmpeg/lib/
  /usr/lib/x86_64-linux-gnu/

  ${PROJECT_SOURCE_DIR}/libs/
  )

#Link external libraries
link_libraries(
  -lavcodec
  -lavdevice
  -lavfilter
  -lavformat
  -lavutil
  -lswscale
    ${OpenCV_LIBS}
     -lprotobuf
     -lpthread
    -lX11
    -lnvcuvid
  )

#Add project source codes to project
set(SRC "" CACHE INTERNAL "")
file(GLOB SRC
  src/*.c src/*.cpp
  common/src/*.c common/src/*.cpp
  )

#Create project library
if(NOT "${SRC}" STREQUAL "")
  #Create the library
  message("[CORERAIN] Creating project library")
  add_library(${PROJECT_NAME} SHARED ${SRC})
elseif("${SRC}" STREQUAL "")
  message("[CORERAIN] No internal dependencies")
endif()

#Add sub-directories of the project
add_subdirectory(test/)
