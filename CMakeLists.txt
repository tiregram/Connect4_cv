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
add_executable(C4cv ${C4_FILE_LIB})

# add_library(c4rule_S SHARED ${C4_FILE_LIB})
# add_library(c4rule STATIC ${C4_FILE_LIB})

target_link_libraries( C4cv ${OpenCV_LIBS} )

if( NOT ${FOR_NAO})
  set(CMAKE_CXX_STANDARD 11)
  find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

  # UT
  file(GLOB_RECURSE C4_FILE_UT test/*.cpp)
  add_executable(c4rule_ut ${C4_FILE_UT})

  target_link_libraries(c4rule_ut
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
  target_link_libraries(c4rule_ut c4rule)

endif( NOT ${FOR_NAO})
