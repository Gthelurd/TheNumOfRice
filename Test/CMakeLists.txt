cmake_minimum_required(VERSION 2.8)
project( Test )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( countrice test.cpp )
target_link_libraries( countrice ${OpenCV_LIBS} )
