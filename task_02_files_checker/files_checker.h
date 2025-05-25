/**
 * ----------------------------------------------------------------------------
 * \file files_checker.h
 * \brief
 * \author Anton S. (savver.engineer@gmail.com)
 * \date 2025-05-25
 * ----------------------------------------------------------------------------
 */

#ifndef FILES_CHECKER_H
#define FILES_CHECKER_H

/*--- Includes ---------------------------------------------------------------*/

#include <stdint.h>

/*--- Settings ---------------------------------------------------------------*/


/*--- Defines ----------------------------------------------------------------*/

typedef enum
{
    fch_ok              =  0,
    fch_err_dir_open    = -1,
    fch_err_file_read   = -2,
    fch_err_data_format = -3,
    fch_err_mem_alloc   = -4
}
fch_res_t;

/*--- Global Prototypes ------------------------------------------------------*/

int fch_check_dir(const char * dirname);

//------------------------------------------------------------------------------
#endif // FILES_CHECKER_H
