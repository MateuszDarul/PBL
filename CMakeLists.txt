cmake_minimum_required(VERSION 3.12)

set(project_name "GameEngine")

project(${project_name} VERSION 0.1)

set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

include(thirdparty/thirdparty.cmake)

add_subdirectory(game)