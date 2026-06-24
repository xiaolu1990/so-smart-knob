file(GLOB_RECURSE SOURCES 
    ui.c 
    ui_helpers.c
    "${CMAKE_CURRENT_SOURCE_DIR}/screens/*.c" 
    "${CMAKE_CURRENT_SOURCE_DIR}/components/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/images/*.c")

add_library(ui ${SOURCES})
