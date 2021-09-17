cmake_minimum_required(VERSION 3.16)
project(test)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fopenmp -std=c++14 -O2 ")
include(cmake/common.cmake)

# Proto file
get_filename_component(hw_proto "proto/jobmanager.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/jobmanager.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/jobmanager.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/jobmanager.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/jobmanager.grpc.pb.h")
add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
add_subdirectory(engine)

find_package(PkgConfig)
pkg_search_module(OPENCV REQUIRED opencv)
include_directories(${OPENCV_INCLUDE_DIRS})
link_directories(${OPENCV_LIBRARY_DIRS})

#
#add_library(plugin SHARED custom.cpp)
#target_link_libraries(plugin ${OPENCV_LIBRARIES} )


add_executable(jobClient main.cpp
        hander/handerManager.cpp
        grpc/device_manager.cpp
        grpc/job_manager.cpp
        task/taskManager.cpp
        dataset/dataset_mgr.cpp
        process/workProcess.cpp
        process/taskProcess.cpp
        process/masterProcess.cpp
        ${hw_grpc_srcs}
        ${hw_grpc_hdrs}
        ${hw_proto_srcs}
        ${hw_proto_hdrs})

target_link_libraries(jobClient  ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

