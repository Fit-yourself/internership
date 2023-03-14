aux_source_directory(. LIB)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} LIB_SOURCES)
add_library(MyLibrary STATIC ${LIB_SOURCES})
