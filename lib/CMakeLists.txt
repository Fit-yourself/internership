aux_source_directory(. LIB)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} LIB_SOURCES)
add_library(MyLibrary STATIC ${LIB_SOURCES})

# 链接库
# target_link_libraries(${app_name} beep_static lcd_bitmap_static lcd_tools_static lcd_touch_static led_static)
