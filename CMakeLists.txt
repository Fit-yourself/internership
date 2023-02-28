cmake_minimum_required(VERSION 3.5)
# 设置 C 标准
set(CMAKE_C_STANDARD 11)
# 项目名称
project(YueQian C)

# 目标应用程序名称
set(app_name touch)

# 添加源文件
file(GLOB SOURCES "src/show_bitmap.c" "src/systerm.c")

# 添加头文件路径
include_directories(inc)

set(lcd lcd_tools.c)
set(bitmap lcd_bitmap.c)
set(touch lcd_touch.c)
# 添加静态库和动态库
add_library(${lcd}_s STATIC lib/${lcd})
add_library(${bitmap}_s STATIC lib/${bitmap})
add_library(${touch}_s STATIC lib/${touch})
set(sources_file ${lcd}_s ${bitmap}_s ${touch}_s)

# 添加可执行文件
add_executable(${app_name} ${SOURCES})

# 链接库文件
target_link_libraries(${app_name} ${sources_file} m rt pthread) # lcd_bitmap_static lcd_tools_static 

# 设置输出路径
set_target_properties(${app_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
