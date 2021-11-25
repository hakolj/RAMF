# CMakeList.txt: RAMF 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.15)


# 将源代码添加到此项目的可执行文件。
option(CHECK_INCLUDE_DIR "switch to show include directories for debugging cmakelists" ON)

add_executable (RAMF source/RAMF_main.cpp)
#add_executable (RAMF RAMF.cpp RAMF.h)


include_directories(source/include)
include_directories(source/)




if (CHECK_INCLUDE_DIR)
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()
endif()


## start compiling source


add_library(PCH STATIC source/include/pch.cpp)
add_library(PTEST STATIC source/performanceTest.cpp)

aux_source_directory(source/other src_other) 
add_library(Other STATIC ${src_other})

add_library(SManager STATIC source/SimuManager.cpp)

aux_source_directory(source/MachineLearning src_ML) 
add_library(ML STATIC ${src_ML})
#add_library(ML STATIC "source/Machine Learning/QLearning.cpp" "source/Machine Learning/DDQL.cpp" "source/Machine Learning/Memory.cpp")

aux_source_directory(source/Actor src_Actor) 
add_library(Actor STATIC ${src_Actor})

aux_source_directory(source/Agent src_Agent) 
add_library(Agent STATIC ${src_Agent})

aux_source_directory(source/Sensor src_Sensor) 
add_library(Sensor STATIC ${src_Sensor})

aux_source_directory(source/Environment src_Environment) 
add_library(Env STATIC ${src_Environment})

aux_source_directory(source/Task src_Task) 
add_library(Task STATIC ${src_Task})


target_link_libraries(PCH ${TORCH_LIBRARIES})
target_link_libraries(Other PCH)
target_link_libraries(Agent Other)
target_link_libraries(Sensor Other)
target_link_libraries(Actor Other)
target_link_libraries(Env Other)
target_link_libraries(Task Other)
target_link_libraries(SManager Other)
target_link_libraries(ML PCH)
target_link_libraries(PTEST Agent Sensor Actor Env Task SManager ML)

target_link_libraries(RAMF PTEST)

#find_package(MKL REQUIRED
#PATHS E:/Intel/oneAPI/mkl/2021.3.0/lib/cmake/mkl/)


if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET RAMF
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TORCH_DLLS}
        $<TARGET_FILE_DIR:RAMF>)

 #   add_custom_command(TARGET RAMF
 #       POST_BUILD
  #      COMMAND ${CMAKE_COMMAND} -E copy_if_different
 #       E:/Intel/oneAPI/mkl/2021.3.0/lib/intel64/mkl_intel_ilp64.lib
  #      E:/Intel/oneAPI/mkl/2021.3.0/lib/intel64/mkl_core.lib
  #      E:/Intel/oneAPI/mkl/2021.3.0/lib/intel64/mkl_intel_thread.lib
  #      $<TARGET_FILE_DIR:RAMF>/..)
endif (MSVC)