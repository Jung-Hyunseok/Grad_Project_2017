cmake_minimum_required (VERSION 2.8)
project(test)

set(CMAKE_MODULE_PATH "/usr/loca/lib/cmake/${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake/)

find_package(raspicam REQUIRED)
find_package(OpenCV)
find_library(wiringPi_LIB wiringPi)

set(SRC_FILES test.cc tracking.cc tracking.h)

add_executable (test ${SRC_FILES})
target_link_libraries (test ${raspicam_CV_LIBS})
target_link_libraries (test ${wiringPi_LIB})
