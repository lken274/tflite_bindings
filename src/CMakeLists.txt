set(SOURCE main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC . ${CMAKE_CURRENT_SOURCE_DIR})

set(TENSORFLOW_SOURCE_DIR "C:\\msys64\\home\\Logan\\tensorflow_src" CACHE PATH "Directory that contains the TensorFlow project" )

add_subdirectory(
  "${TENSORFLOW_SOURCE_DIR}\\tensorflow\\lite"
  "${CMAKE_CURRENT_BINARY_DIR}\\tensorflow-lite"
  EXCLUDE_FROM_ALL
)

target_link_libraries(${PROJECT_NAME} tensorflow-lite cimglib)