cmake_minimum_required(VERSION 3.5)
project(ttyX_Getter)

add_library(${PROJECT_NAME} SHARED ttyX_Getter.cpp)
target_link_libraries(${PROJECT_NAME} udev)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
