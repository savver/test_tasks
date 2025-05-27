/**
 * ----------------------------------------------------------------------------
 * \file main.c
 * \brief Unity tests for library 'search unique number in int32 array'.
 * Lib 'manual_tests' is need only for read & compare numbers from file
 * \author Anton S.
 * \date 2025-05-26
 * ----------------------------------------------------------------------------
 */

/*--- Includes ---------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "get_uniq_num/get_uniq_num.h"
#include "unity/unity.h"
#include "get_uniq_num_mantests/manual_tests.h"

/*--- Defines, Macroses -----------------------------------------------------*/

#if defined(_WIN64) ||  defined (_WIN32)
    #define TEST_FILE_1   "test_data\\test_001_arr21_res=11.txt"
#else
    #define TEST_FILE_1   "test_data/test_001_arr21_res=11.txt"
#endif

#define TEST_DIR_1   "test_data"

#ifndef countof
#define countof(a) sizeof(a)/sizeof(*(a))
#endif

typedef struct
{
    int32_t *   arr;
    size_t      arr_size; //in elements
    int32_t     uniq_num;
}
ut_arr_info_t;

/*--- Global Variables ------------------------------------------------------*/

int32_t     ut_arr01_res5[] = { 1,2,3,4,
                                     1,2,3,4,  5};

int32_t     ut_arr02_res6[] = { 1,2,3,4,5,
                                     4,3,2,1,5,  6};

int32_t     ut_arr03_res7[] = { 1,2,3,4,5,  7,  4,3,2,1,5};

static
ut_arr_info_t
ut_arr_info[]   =
{
    { ut_arr01_res5, countof(ut_arr01_res5), 5 },
    { ut_arr02_res6, countof(ut_arr02_res6), 6 },
    { ut_arr03_res7, countof(ut_arr03_res7), 7 },
};

/*--- Private  Functions ----------------------------------------------------*/

void setUp(void)
{

}

void tearDown(void)
{

}

void test_01_static_arrays(void)
{
    TEST_ASSERT_EQUAL(ut_arr_info[0].uniq_num,
                          get_unique_num(ut_arr_info[0].arr,
                                         ut_arr_info[0].arr_size));

    TEST_ASSERT_EQUAL(ut_arr_info[1].uniq_num,
                          get_unique_num(ut_arr_info[1].arr,
                                         ut_arr_info[1].arr_size));

    TEST_ASSERT_EQUAL(ut_arr_info[2].uniq_num,
                          get_unique_num(ut_arr_info[2].arr,
                                         ut_arr_info[2].arr_size));
}

///! not see error in specific array
void test_01_static_arrays_alt(void)
{
    for(uint i = 0; i < countof(ut_arr_info); i++)
    {
        TEST_ASSERT_EQUAL(ut_arr_info[i].uniq_num,
                          get_unique_num(ut_arr_info[i].arr,
                                         ut_arr_info[i].arr_size));
    }
}

void test_TEST_FILE_1(void)
{
    TEST_ASSERT_EQUAL(0, mantest_03_read_fr_file(TEST_FILE_1));
}

void test_TEST_DIR_1(void)
{
    TEST_ASSERT_EQUAL(0, mantest_04_files_fr_dir(TEST_DIR_1));
}

/*=== Functions ==============================================================*/
int main(void)
{
    printf("------------------------------------------" "\n"
           " Unity tests for library 'get_uniq_num'   " "\n"
           "------------------------------------------" "\n");

    UNITY_BEGIN();

    RUN_TEST(test_01_static_arrays);
  //RUN_TEST(test_01_static_arrays_alt);
    RUN_TEST(test_TEST_FILE_1);
    RUN_TEST(test_TEST_DIR_1);

    return UNITY_END();
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

