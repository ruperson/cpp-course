cmake_minimum_required(VERSION 3.5)
add_definitions(-DHW3)
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -std=c++17 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -DDEBUG")
  set(CMAKE_CXX_FLAGS_COVERAGE "--coverage")
elseif (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /permissive- /D_HAS_TR1_NAMESPACE /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING ")
endif()
set(LINK_FLAGS "-pthread ${LINK_FLAGS}")

include_directories(.)
add_subdirectory(gtest)
include_directories(${CMAKE_SOURCE_DIR}/gtest)

add_executable(big_integer_testing
               big_integer_testing.cpp
               big_integer.h
               big_integer.cpp
			   vector/vector.h
			   vector/vector.cpp
			   )

target_link_libraries(big_integer_testing gtest)