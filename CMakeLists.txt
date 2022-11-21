cmake_minimum_required(VERSION 3.0)
project(EdgeDetect)
# OpenCV
set(OpenCV_DIR  ../opencv345/build/x64/vc15/lib)
find_package(OpenCV  REQUIRED)
message("Opencv version:${OpenCV_VERSION}")
include_directories(${OpenCV_INCLUDE_DIRS}) 
link_directories(${Opencv_Lib_Dir})


SET(SRC main.cpp edgeDetectRect.cpp edgeDetectRect.h validMatTool.cpp validMatTool.h)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} edgeDetectRect validMatTool)