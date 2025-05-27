/**
 * ----------------------------------------------------------------------------
 * \file main.c
 * \brief Unity tests for library 'files_checker'
 * \author Anton S.
 * \date 2025-05-26
 * ----------------------------------------------------------------------------
 */

/*--- Includes ---------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include "files_checker.h"
#include "unity/unity.h"

/*--- Private  Functions ----------------------------------------------------*/

void setUp(void)
{

}

void tearDown(void)
{

}

void test_01_correct_dir(void)
{
    TEST_ASSERT_EQUAL(fch_ok, fch_check_dir("test_files/test_01_all_files_ok"));
}

void test_02_nonexist_dir(void)
{
    TEST_ASSERT_EQUAL(fch_err_dir_open, fch_check_dir("nonexis_dir"));
}

void test_03_2files_bad_md5(void)
{
    TEST_ASSERT_EQUAL(2, fch_check_dir("test_files/test_03_2filesErrMd5"));
}

void test_04_2files_miss_md5(void)
{
    TEST_ASSERT_EQUAL(2, fch_check_dir("test_files/test_04_2missMd5"));
}

/*=== Functions ==============================================================*/
int main(void)
{
    printf("------------------------------------------" "\n"
           " Unity tests for library 'files_checker'  " "\n"
           "------------------------------------------" "\n");

    UNITY_BEGIN();

    RUN_TEST(test_01_correct_dir);
    RUN_TEST(test_02_nonexist_dir);
    RUN_TEST(test_03_2files_bad_md5);
    RUN_TEST(test_04_2files_miss_md5);

    return UNITY_END();
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

