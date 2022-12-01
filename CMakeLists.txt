cmake_minimum_required(VERSION 3.23)
project(DB_ANON)

set(CMAKE_CXX_STANDARD 14)

add_executable(DB_ANON db_anon.cpp)
#SET(GCC_COVERAGE_COMPILE_FLAGS "-O3 -march=native -flto")
#SET(GCC_COVERAGE_LINK_FLAGS    "-lg")
add_compile_options(-O3 -march=native -flto)
add_link_options(-flto)
#set_target_properties(${project} PROPERTIES LINK_FLAGS ${PROJ_LINK_FLAGS})
