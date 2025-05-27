/**
 * ----------------------------------------------------------------------------
 * \file files_checker.h
 * \brief Check md5 hash for all files in folder and subfolders.
 * \author Anton S. (savver.engineer@gmail.com)
 * \date 2025-05-26
 * ----------------------------------------------------------------------------
 */

#ifndef FILES_CHECKER_H
#define FILES_CHECKER_H

/*--- Includes ---------------------------------------------------------------*/

#include <stdint.h>

/*--- Settings ---------------------------------------------------------------*/

/*! @brief Print detail debug info during tests
*/
#define MANTEST_DBG_PRINT_DETAIL            0

/*--- Defines ----------------------------------------------------------------*/

/*! @brief Return codes for some functions
*/
typedef enum
{
    fch_ok              =  0,
    fch_err_dir_open    = -1,
    fch_err_file_read   = -2,
    fch_err_data_format = -3,
    fch_err_mem_alloc   = -4
}
fch_res_t;

#define FCH_MD5_STR_SIZE      (32+1)

/*--- Global Prototypes ------------------------------------------------------*/

int fch_check_dir(const char * dirname);

//------------------------------------------------------------------------------
#endif // FILES_CHECKER_H
