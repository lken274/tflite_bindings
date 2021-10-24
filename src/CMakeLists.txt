set(SOURCE main.cpp)

# Create our library target
add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME} PUBLIC ${SOURCE})

#add_executable(${PROJECT_NAME} ${SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC . ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(${PROJECT_NAME}
PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION}
)
# Let's set compiler-specific flags
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    # G++
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    # MSVC
    target_compile_options(${PROJECT_NAME} PRIVATE /EHsc /MTd /W2 /c)
    # Set the DLLEXPORT variable to export symbols
    target_compile_definitions(${PROJECT_NAME} PRIVATE WIN_EXPORT)
endif()

set(TENSORFLOW_SOURCE_DIR "C:\\msys64\\home\\Logan\\tensorflow_src" CACHE PATH "Directory that contains the TensorFlow project" )

add_subdirectory(
  "${TENSORFLOW_SOURCE_DIR}\\tensorflow\\lite"
  "${CMAKE_CURRENT_BINARY_DIR}\\tensorflow-lite"
  EXCLUDE_FROM_ALL
)

target_link_libraries(${PROJECT_NAME} tensorflow-lite cimglib)