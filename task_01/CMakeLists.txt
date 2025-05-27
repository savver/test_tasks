cmake_minimum_required(VERSION 3.5)

project(get_uniq_num
                  VERSION 0.1
                  LANGUAGES C)

add_executable(get_uniq_num     main.c
                                get_uniq_num/get_uniq_num.c
                                get_uniq_num/get_uniq_num.h
                                get_uniq_num_mantests/manual_tests.c
                                get_uniq_num_mantests/manual_tests.h)

add_executable(test_runner_get_uniq_num test_runner.c
                                        get_uniq_num/get_uniq_num.c
                                        get_uniq_num/get_uniq_num.h
                                        unity/unity.c
                                        unity/unity.h
                                        get_uniq_num_mantests/manual_tests.c
                                        get_uniq_num_mantests/manual_tests.h
                                        unity/unity_internals.h)
