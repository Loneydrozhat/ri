# Recuperação de Informação - Trabalho Prático 1
# author: Danilo Ferreira e Silva
# mail: danilofes@gmail.com

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

project(TP1)

set (CMAKE_C_FLAGS "-Wall -Werror")
set (CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11 -g")
# set (CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11 -pg -fno-omit-frame-pointer")

enable_testing()

add_subdirectory(lib/ricp)
add_subdirectory(src)

add_subdirectory(lib/gmock-1.6.0)
add_subdirectory(test)
