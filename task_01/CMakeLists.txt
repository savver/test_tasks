cmake_minimum_required(VERSION 3.5)

project(get_uniq_num
                  VERSION 0.1
                  LANGUAGES C)

#include_directories(${PROJECT_SOURCE_DIR}/tests)
#add_library(mantests STATIC tests/manual_tests.c tests/manual_tests.h)
#target_include_directories(mantests PUBLIC ${PROJECT_SOURCE_DIR}/tests)

add_executable(get_uniq_num     main.c
                                get_uniq_num/get_uniq_num.c
                                get_uniq_num/get_uniq_num.h
                                get_uniq_num_mantests/manual_tests.c
                                get_uniq_num_mantests/manual_tests.h)
