get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)

string(REPLACE " " "_" ProjectId ${ProjectId})

project(${ProjectId} C CXX)

set (CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 2.8)
find_package( OpenCV REQUIRED )
include_directories($(OpenCV_INCLUDE_DIRS))
 
set( CPP_SOURCES  
        calib_intrinsic.cpp
        )
 
add_executable(${PROJECT_NAME} ${CPP_SOURCES}  )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} popt)
