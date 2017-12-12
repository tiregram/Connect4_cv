cmake_minimum_required (VERSION 2.8.11)
project (Connect4_cv)

set(CMAKE_CXX_STANDARD 11)
add_definitions("-D_GLIBCXX_USE_CXX11_ABI=0")

find_package(OpenCV REQUIRED )


include_directories("include")

file(GLOB_RECURSE C4_FILE_LIB src/*.cpp)
include_directories(${OpenCV_INCLUDE_DIRS})
add_library(c4_cv SHARED ${C4_FILE_LIB})
target_link_libraries(c4_cv ${OpenCV_LIBS})

if(${BUILD_IP_CAM})
  add_executable(c4_cv_ip_cam "exec/ip_cam.cpp")
  target_include_directories(c4_cv_ip_cam PUBLIC "include")

  target_link_libraries(c4_cv_ip_cam c4_cv)

  target_link_libraries(c4_cv_ip_cam  ${OpenCV_LIBS})
  message(${OpenCV_LIBS})

endif(${BUILD_IP_CAM})
