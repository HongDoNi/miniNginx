# CMake 教程

# cmake中有很多预设参数：https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html
cmake_minimum_required(VERSION 3.12.0)
project(miniNginx-https VERSION 0.1.0 LANGUAGES C CXX)

# 用于生成 compile_commands.json文件
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

set(CMAKE_CXX_STANDARD 11)

# 获取某路径下所有的源文件
# 方式1：aux_source_directory 按照源文件后缀匹配，后缀在project的languages中设置（该种文件搜索方式无法迭代搜索
aux_source_directory(${PROJECT_SOURCE_DIR}/app APP_SRC1)
# 方式2：file(GLOB/GLOB_RECURSE 变量名 要搜索的文件路径和文件类型) 可以迭代搜搜
file(GLOB APP_SRC2 ${PROJECT_SOURCE_DIR}/app/*.cpp)
file(GLOB_RECURSE ALL_SRC ${PROJECT_SOURCE_DIR}/*.cpp) # 该语句会有问题：会把cmake中的cpp文件一起匹配出来，即build中用于cmake的cpp文件

file(GLOB_RECURSE SRC1 ${PROJECT_SOURCE_DIR}/app/*.cpp)
file(GLOB_RECURSE SRC2 ${PROJECT_SOURCE_DIR}/misc/*.cpp)
file(GLOB_RECURSE SRC3 ${PROJECT_SOURCE_DIR}/net/*.cpp)
file(GLOB_RECURSE SRC4 ${PROJECT_SOURCE_DIR}/proc/*.cpp)
file(GLOB_RECURSE SRC5 ${PROJECT_SOURCE_DIR}/signal/*.cpp)

set(SRC_FILES ${SRC1} ${SRC2} ${SRC3} ${SRC4} ${SRC5})

include_directories(${PROJECT_SOURCE_DIR}/_include)


include(CTest)
enable_testing()

add_executable(miniNginx ${SRC_FILES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# 打印信息 ：message([<mode>] "message text" ...), mode有好几种STATUS为例：
# message(STATUS "Looking for someheader.h - found")
# 下面这个例子也说明了如何在cmake中access环境变量
# message(STATUS "$ENV{PATH}") 

# message(STATUS, "${SRC_FILES}")

# CMAKE_SOURCE_DIR vs PROJECT_SOURCE_DIR
# CMAKE_SOURCE_DIR does indeed refer to the folder where the top-level CMakeLists.txt is defined. 
# However, PROJECT_SOURCE_DIR refers to the folder of the CMakeLists.txt containing the most recent project() command.

# CMAKE_SOURCE_DIR vs CMAKE_CURRENT_SOURCE_DIR
# CMAKE_SOURCE_DIR is where cmake was originally invoked
# CMAKE_CURRENT_SOURCE_DIR is where cmake is currently working



