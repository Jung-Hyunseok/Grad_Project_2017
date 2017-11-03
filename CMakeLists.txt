cmake_minimum_required (VERSION 2.8)

set(CMAKE_MODULE_PATH "/usr/loca/lib/cmake/${CMAKE_MODULE_PATH})
find_package(raspicam REQUIRED)
find_package(OpenCV)

add_excutable (main test.cc)
target_link_libraries (main ${raspicam_CV_LIBS})
