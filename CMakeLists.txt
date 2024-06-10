cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

# set some cmake paramaters
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# create a project
project(ruin)

# create a new executable
add_executable(ruin)

# linking the libraries and binary files, also setting include path
target_include_directories(ruin PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_directories(ruin PRIVATE lib)
target_link_directories(ruin PRIVATE bin)
target_link_libraries(ruin PRIVATE mingw32 SDL2main SDL2 SDL2_ttf SDL2_image)

# adding all the source files
add_subdirectory(src)