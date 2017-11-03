cmake_minimum_required (VERSION 2.8)

set(CMAKE_MODULE_PATH "/usr/loca/lib/cmake/${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake/)
find_package(raspicam REQUIRED)
find_package(OpenCV)

add_executable (main test.cc)
target_link_libraries (main ${raspicam_CV_LIBS})
