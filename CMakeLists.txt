# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.15)

project ("RAMFSln")

#set(CMAKE_CXX_STANDARD 14)
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Eigen3
get_filename_component(EIGEN3_INCLUDE_DIRS ./lib/eigen-3.3.9 ABSOLUTE)
include_directories(${EIGEN3_INCLUDE_DIRS})
message("Eigen3 at ${EIGEN3_INCLUDE_DIRS}")


# TODO: 如有需要，请添加测试并安装目标。


# configure MKL for compiling torchlib
find_package(MKL CONFIG)
message("*${MKL_DIR}")
set (myMKL_Dir ${MKL_DIR}/../../${MKL_ARCH}/)
#E:/Intel/oneAPI/mkl/2021.3.0/lib/intel64/
foreach (templib ${MKL_LIBRARIES})
    list(APPEND myMKL_LIBRARIES ${myMKL_Dir}/${templib}.lib)
endforeach()
if (MSVC)
add_link_options("/LIBPATH:${myMKL_Dir}")
endif()

#set (MKL_LIBRARIES ${myMKL_LIBRARIES})
#foreach (templib ${MKL_LIBRARIES})
#    message("* ${templib}")
#endforeach()

#openmp
find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
set(CMAKE_SHARE_LINKER_FLAGS "${CMAKE_SHARE_LINKER_FLAGS} ${OpenMP_SHARE_LINKER_FLAGS}") 


# torchlib
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(TorchPath /home/qjr/mydata/torchlib-cpu-linux/libtorch/share/cmake/Torch)

    elseif (${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(TorchPath /home/qjr/mydata/torchlib-cpu-linux/libtorch/share/cmake/Torch)
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(TorchPath D:/Homework/Y4-1/2021-09-07-RAMF_cmake/libtorch-release/libtorch/share/cmake/Torch)

    elseif (${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(TorchPath D:/Homework/Y4-1/2021-09-07-RAMF_cmake/libtorch-release/libtorch/share/cmake/Torch)

    elseif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(TorchPath D:/Homework/Y4-1/2021-09-07-RAMF_cmake/libtorch-debug/libtorch/share/cmake/Torch)
    endif()
endif()
get_filename_component(TorchPath ${TorchPath} REALPATH)
message("TorchPath: ${TorchPath}")

find_package(Torch REQUIRED
PATHS ${TorchPath}
NO_DEFAULT_PATH
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

message("TORCH_INCLUDE_DIRECTORIES: ${TORCH_INCLUDE_DIRECTORIES}")
include_directories(${TorchPath}/../../../include) 
include_directories(${TorchPath}/../../../include/torch/csrc/api/include) 
# 包含子项目。
add_subdirectory ("RAMF")
