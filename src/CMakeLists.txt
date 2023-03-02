aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_SOURCES)

# 目标应用程序名称
set(app_name MyApp)

add_executable(${app_name}  ${SRC_SOURCES})
# 链接库文件
# target_link_libraries(${app_name} ${sources_file} m rt pthread) # lcd_bitmap_static lcd_tools_static 
target_link_libraries(${app_name} MyLibrary m rt pthread)

# 将可执行文件输出到根目录的 bin 文件夹下
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# 设置输出路径
# set_target_properties(${app_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${${CMAKE_SOURCE_DIR}}/bin)
# set_target_properties(${app_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)


# aux_source_directory(. SOUCES)
# file(GLOB SOURCES "src/show_bitmap.c" "src/systerm.c")
# add_library(APP STATIC ${SOUCES})


# # 链接库文件
# # target_link_libraries(${app_name} ${sources_file} m rt pthread) # lcd_bitmap_static lcd_tools_static 
# target_link_libraries(${app_name} LCD Touch LED BEEP m rt pthread)

# # 设置输出路径
