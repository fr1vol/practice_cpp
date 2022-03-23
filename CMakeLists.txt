cmake_minimum_required(VERSION 3.10)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(practice_cpp LANGUAGES CXX)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/test UT_SOURCES)
add_executable(test)
target_sources(test PUBLIC ${UT_SOURCES})

target_include_directories(test
    PRIVATE 
        include   
)