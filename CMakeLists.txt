cmake_minimum_required(VERSION 3.15)
project(foncMo3d)

set(CMAKE_CXX_STANDARD 14)

add_executable(foncMo3d main.cpp lib/tgaimage.h lib/tgaimage.cpp model/Model.cpp model/Model.h model/Vectors.h model/Vectors.cpp render/our_gl.h render/our_gl.cpp)