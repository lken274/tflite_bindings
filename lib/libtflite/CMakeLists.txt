#file(GLOB TFFILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")
add_library(tflitelib INTERFACE)
#set_target_properties(tflitelib PROPERTIES LINKER_LANGUAGE C)
target_include_directories(tflitelib INTERFACE ./include ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(tflitelib INTERFACE "libs\\windows_x86_64\\libtensorflowlite_c.dll")