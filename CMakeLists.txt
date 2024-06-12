cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(ruin)

add_executable(ruin)

if (UNIX)

    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)

    target_link_libraries(ruin PRIVATE SDL2 SDL2_ttf SDL2_image)

endif(UNIX)

if (WIN32)
    target_include_directories(ruin PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_directories(ruin PRIVATE lib)
    target_link_directories(ruin PRIVATE bin)
    target_link_libraries(ruin PRIVATE mingw32 SDL2main SDL2 SDL2_ttf SDL2_image)
endif(WIN32)

add_subdirectory(src)
