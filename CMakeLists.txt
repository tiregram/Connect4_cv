cmake_minimum_required (VERSION 2.8.11)
project (Connect4_cv)


# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -Wno-long-long -pedantic -m32  -static-libgcc -static-libstdc++ ")

find_package(OpenCV REQUIRED )
#include(../cross-config.cmake)
#include("${CMAKE_SOURCE_DIR}/../opencv-3.3.1/build/OpenCVConfig.cmake")

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../opencv-3.3.1/build/"  ${CMAKE_MODULE_PATH})
#message(${CMAKE_MODULE_PATH})

file(GLOB_RECURSE C4_FILE_LIB src/*.cpp)

add_library(c4_cv SHARED ${C4_FILE_LIB})
target_include_directories(c4_cv PRIVATE "/usr/include")

include_directories("include")

if(${BUILD_IP_CAM})

  add_executable(c4_cv_ip_cam "exec/ip_cam.cpp")
  target_include_directories(c4_cv_ip_cam PUBLIC "include")


  target_link_libraries(c4_cv_ip_cam c4_cv)

  target_link_libraries(c4_cv_ip_cam  ${OpenCV_LIBS})
  message(${OpenCV_LIBS})

endif(${BUILD_IP_CAM})
