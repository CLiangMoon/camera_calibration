cmake_minimum_required( VERSION 3.10 )

project( CAMERA )

set( CMAKE_BUILD_TYPE "Release" )

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(camera_parameter2 CameraCalibration.cpp)

target_link_libraries(camera_parameter2 ${OpenCV_LIBS})