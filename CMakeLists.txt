cmake_minimum_required(VERSION 2.8)
project(trilateration)

## To activate c++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

## Enable all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

## External libs
find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

## Settings
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

SET(SRCS_TR_LIB
	src/trilateration.cpp
)

## Build
add_executable(main src/main.cpp ${SRCS_TR_LIB})
add_executable(main_pr src/main_pr.cpp ${SRCS_TR_LIB})

## Link
target_link_libraries(main ${CERES_LIBRARIES})
target_link_libraries(main_pr ${CERES_LIBRARIES})











