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

/*--- Global Prototypes ------------------------------------------------------*/

///! \brief user input array from keyboard, and see searching result
void
mantest_01_user_stdin(void);

///! \brief read array from test file
void
mantest_02_read_fr_files(void);

//------------------------------------------------------------------------------
#endif // MANUAL_TESTS_H
