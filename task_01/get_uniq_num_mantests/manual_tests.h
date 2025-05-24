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

///! Print numbers read from files
#define MANTEST_DBG_PRINT_FILE_DATA         1

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

///! \brief user input array from keyboard, and see searching result
void
mantest_01_user_stdin(void);

///! \brief read array from single test file
void
mantest_02_read_fr_file(const char * file_path);

///! read all text files in dir
void
mantest_03_files_fr_dir(const char * file_path);

//------------------------------------------------------------------------------
#endif // MANUAL_TESTS_H
