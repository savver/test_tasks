/**
 * ----------------------------------------------------------------------------
 * \file manual_tests.h
 * \brief
 * \author Anton S. (savver.engineer@gmail.com)
 * \date 2025-05-24
 * ----------------------------------------------------------------------------
 */

#ifndef MANUAL_TESTS_H
#define MANUAL_TESTS_H

/*--- Includes ---------------------------------------------------------------*/

#include <stdint.h>

/*--- Settings ---------------------------------------------------------------*/

///! Print detail debug info during tests
#define MANTEST_DBG_PRINT_DETAIL            1

///! Print numbers from static arrays from this test lib
#define MANTEST_DBG_PRINT_TEST01_STAT_ARR   1

///! Print numbers read from files
#define MANTEST_DBG_PRINT_FILE_DATA         1

///! Max numbres read from file. To avoid allocating a large amount of memory
#define MANTEST_MAX_NUMBERS_FR_FILE         100

/*--- Defines ----------------------------------------------------------------*/

typedef enum
{
    mantest_ok              =  0,
    mantest_err_file_open   = -1,
    mantest_err_file_read   = -2,
    mantest_err_data_format = -3,
    mantest_err_mem_alloc   = -4
}
mantest_res_t;

#define MANTEST_MAX_FNAME_LEN   260

/*--- Global Prototypes ------------------------------------------------------*/

int     mantest_01_static_arr(void);
void    mantest_02_user_stdin(void);
int     mantest_03_read_fr_file(const char * fname);
int     mantest_04_files_fr_dir(const char * dirname);

//------------------------------------------------------------------------------
#endif // MANUAL_TESTS_H
