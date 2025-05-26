/**
 * ----------------------------------------------------------------------------
 * \file main.c
 * \brief Test task 01 "search unique number in int32 array"
 * \author Anton S.
 * \date 2025-05-24
 * ----------------------------------------------------------------------------
 */

/*--- Includes ---------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "get_uniq_num_mantests/manual_tests.h"

#if defined(_WIN64) ||  defined (_WIN32)
    #define TEST_FILE   "test_data\\test_001_arr21_res=11.txt"
#else
    #define TEST_FILE   "test_data/test_001_arr21_res=11.txt"
#endif

/*=== Functions =============================================================*/
int main()
{
    printf("-------- Search Uniq Num in Array -----------\n");

    mantest_01_static_arr();
  //mantest_02_user_stdin();
    mantest_03_read_fr_file(TEST_FILE);
    mantest_04_files_fr_dir("test_data");
}
/*---------------------------------------------------------------------------*/
