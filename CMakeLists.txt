cmake_minimum_required(VERSION 3.5)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
# 项目名称
project(YueQian C)

# 添加头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

#添加编译子目录
add_subdirectory(lib)
add_subdirectory(src)


