/**
 * ----------------------------------------------------------------------------
 * \file get_uniq_num.h
 * \brief The library for function to find a unique number without a pair in
 * an array of numbers, having the form 2*N+1.
 * \author Anton S. (savver.engineer@gmail.com)
 * \date 2025-05-25
 * ----------------------------------------------------------------------------
 */

#ifndef GET_UNIQ_NUM_H
#define GET_UNIQ_NUM_H

/*--- Includes ---------------------------------------------------------------*/

#include <stdint.h>

/*--- Settings ---------------------------------------------------------------*/

///! if 1 use optimal fast version, based on 'xor', but there are some nuances,
/// see function description
#define UNIQNUM_OPTIMAL_VER         1

/*--- Global Prototypes ------------------------------------------------------*/

int32_t  get_unique_num(int32_t *    array, size_t       len);

//------------------------------------------------------------------------------
#endif // GET_UNIQ_NUM_H
