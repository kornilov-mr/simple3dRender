cmake_minimum_required(VERSION 3.20)
project(magnetic)

set(CMAKE_CXX_STANDARD 20)

add_executable(magnetic 3DRender.cpp vecfun.h)
target_link_libraries(magnetic -lOpenGL32 -lfreeGLUT)
