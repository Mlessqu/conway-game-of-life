cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_C_COMPILER clang)
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)
add_executable(game_of_life src/main.cpp
        include/gaia.h
        src/CoreLoop.cpp
        src/GameGrid.cpp
        src/GameGrid.h++)
file(GLOB_RECURSE MAIN_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
)
target_include_directories(game_of_life PUBLIC
        "${CMAKE_SOURCE_DIR}/src")
target_compile_features(game_of_life PRIVATE cxx_std_20)
target_link_libraries(game_of_life PRIVATE SFML::Graphics SFML::Audio)



set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT game_of_life)