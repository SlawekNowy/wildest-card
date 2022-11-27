cmake_minimum_required(VERSION 3.8)

project(wildest_card)

set(LIB_SOURCES wildcard.c wildcard.h)

add_library(wildest_card STATIC ${LIB_SOURCES})
target_include_directories(wildest_card PUBLIC ${CMAKE_CURRENT_LIST_DIR})
