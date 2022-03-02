cmake_minimum_required(VERSION 3.10)

project(practice_cpp)

aux_source_directory(./test UT_SOURCES)
add_executable(test ${UT_SOURCES})

target_include_directories(test
    PRIVATE 
        include   
)