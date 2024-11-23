cmake_minimum_required(VERSION 3.8)

project(wildest_card)

set(LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/include/wildcard.h")

add_library(wildest_card INTERFACE)
target_sources(wildest_card INTERFACE ${LIB_SOURCES})
target_include_directories(wildest_card INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
