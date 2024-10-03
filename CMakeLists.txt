cmake_minimum_required( VERSION 3.10 )

project( CAMERA )

set( CMAKE_BUILD_TYPE "Release" )
#set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

# add_executable(camera_parameter CameraCalibration.cpp)

# target_link_libraries(camera_parameter ${OpenCV_LIBS})

add_executable(camera_parameter2 camera_parameter.cpp)

target_link_libraries(camera_parameter2 ${OpenCV_LIBS})