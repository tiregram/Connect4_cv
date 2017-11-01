cmake_minimum_required (VERSION 2.8.11)
project (Connect4_rule)

if(${FOR_NAO})
  include(cmake_4_nao.txt)
endif(${FOR_NAO})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

if( NOT ${FOR_NAO})
  set(CMAKE_CXX_STANDARD 11)

endif( NOT ${FOR_NAO})

find_package( OpenCV REQUIRED )



file(GLOB_RECURSE C4_FILE_LIB src/*.cpp)

#add_executable(C4cv ${C4_FILE_LIB})



add_library(c4_cv_S SHARED ${C4_FILE_LIB})
add_library(c4_cv STATIC ${C4_FILE_LIB})

target_include_directories(c4_cv_S PUBLIC "include")
target_include_directories(c4_cv PUBLIC "include")

if( NOT ${FOR_NAO})
  set(CMAKE_CXX_STANDARD 11)
  find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

  # UT
  file(GLOB_RECURSE C4_CV_FILE_UT test/*.cpp)
  add_executable(c4_cv_ut ${C4_CV_FILE_UT})

  target_link_libraries(c4_cv_ut
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

  target_link_libraries(c4_cv_ut c4_cv)

  target_link_libraries(c4_cv_ut ${OpenCV_LIBS})

endif( NOT ${FOR_NAO})


if(${BUILD_CONFIG})
  add_executable(c4_cv_config "exec/config.cpp")
  target_include_directories(c4_cv_config PUBLIC "include")

  target_link_libraries(c4_cv_config c4_cv)
  target_link_libraries(c4_cv_config ${OpenCV_LIBS})

endif(${BUILD_CONFIG})

if(${BUILD_IP_CAM})
  add_executable(c4_cv_ip_cam "exec/ip_cam.cpp")
  target_include_directories(c4_cv_ip_cam PUBLIC "include")

  target_link_libraries(c4_cv_ip_cam c4_cv)
  target_link_libraries(c4_cv_ip_cam ${OpenCV_LIBS})

endif(${BUILD_IP_CAM})

