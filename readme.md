
# 插件编译

g++ -fPIC -shared custom.cpp -o libplugin.so $(pkg-config --cflags --libs opencv)


# MXNET 编译

## 下载mxnet
```shell
git clone  https://github.com/apache/incubator-mxnet mxnet
cd mxnet
git checkout -b v1.6.x origin/v1.6.x
git submodule update --init
```

## 修改 CMakeLists.txt

```gitexclude
+++ b/CMakeLists.txt
@@ -20,7 +20,7 @@ include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Utils.cmake)
 
 include(CMakeDependentOption)
 #Some things have order. This must be put in front alone
-option(USE_CUDA "Build with CUDA support"   ON)
+option(USE_CUDA "Build with CUDA support"   OFF)
 set(MXNET_CUDA_ARCH "Auto" CACHE STRING "Target NVIDIA GPU achitecture.
 Format: Auto | Common | All | LIST(ARCH_AND_PTX ...)
 - \"Auto\" detects local machine GPU compute arch at runtime.
@@ -52,7 +52,7 @@ option(USE_JEMALLOC "Build with Jemalloc support"   ON)
 option(USE_DIST_KVSTORE "Build with DIST_KVSTORE support" OFF)
 option(USE_PLUGINS_WARPCTC "Use WARPCTC Plugins" OFF)
 option(USE_PLUGIN_CAFFE "Use Caffe Plugin" OFF)
-option(USE_CPP_PACKAGE "Build C++ Package" OFF)
+option(USE_CPP_PACKAGE "Build C++ Package" ON)
 option(USE_MXNET_LIB_NAMING "Use MXNet library naming conventions." ON)
 option(USE_GPROF "Compile with gprof (profiling) flag" OFF)
 option(USE_CXX14_IF_AVAILABLE "Build with C++14 if the compiler supports it" OFF)

```

## 安装第三方依赖, 需要提前安装opencv

```shell
sudo apt-get update
sudo apt-get install -y build-essential git ninja-build ccache libopenblas-dev  cmake

```

## 编译

```shell
mkdir build 
cd build
cmake ..
```

## 目标文件位置如下
```asm
mxnet/include/mxnet
mxnet/3rdparty/dmlc-core/include/dmlc

mxnet/build/libmxnet.so
mxnet/build/libmxnet.a
mxnet/build/3rdparty/dmlc-core/libdmlc.a
```


## 参考
*https://mxnet.apache.org/versions/master/get_started/build_from_source.html#installing-mxnet's-recommended-dependencies*

*https://docs.opencv.org/3.4.14/d7/d9f/tutorial_linux_install.html*