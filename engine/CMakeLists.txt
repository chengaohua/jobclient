SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fopenmp -std=c++14 -O2 ")
add_library(engine STATIC mxnet/MxnetEngin.cpp engine.cpp)
