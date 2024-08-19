cmake_minimum_required(VERSION 3.12)
project(smaster4s-inis
    VERSION 0.1.0.0)
add_library(${PROJECT_NAME} STATIC src/smaster4s-inis.c)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}_test test/test.c)

target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})
