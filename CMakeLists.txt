cmake_minimum_required(VERSION 3.10)
project(EniRenderer)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(EniRenderer main.cpp tgaimage.cpp tgaimage.h math.hpp model.cpp model.h Texture.cpp Texture.hpp math.cpp EniShader.h GouraudShader.cpp EniShader.cpp)
target_link_libraries(EniRenderer ${OpenCV_LIBRARIES})