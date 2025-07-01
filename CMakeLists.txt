cmake_minimum_required(VERSION 3.10)
project(Projet-Info)


find_package(Boost REQUIRED)
add_compile_options(-mavx -O3)

add_executable(Projet-Info main.cpp)
target_link_libraries(Projet-Info boost_program_options)
target_link_libraries(Projet-Info boost_timer)