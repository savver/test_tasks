cmake_minimum_required(VERSION 3.5)

project(files_checker LANGUAGES C)

add_executable(files_checker main.c
                             program_info.h
                             files_checker.c
                             files_checker.h
                             list.h
                             MD5-hash-Calculator/md5c.c
                             MD5-hash-Calculator/md5.h)

target_link_libraries(files_checker m)
