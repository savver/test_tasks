/**
 * ----------------------------------------------------------------------------
 * \file get_uniq_num.h
 * \brief The library for function to find a unique number without a pair in
 * an array of numbers, having the form 2*N+1.
 * \author Anton S. (savver.engineer@gmail.com)
 * \date 2025-05-25
 * ----------------------------------------------------------------------------
 */

/*--- Includes ---------------------------------------------------------------*/

#include <string.h>
#include "get_uniq_num.h"

/*=== Functions ==============================================================*/
#ifdef UNIQNUM_OPTIMAL_VER
/*!
 * \brief Search in array for number without pair
 * \param array Ptr to array
 * \param len Count of numbers in the array
 * \return Unique value without pair
 * \details The function works correctly if there really is a unique number
 * and it is only one. If there are no unique numbers, and one number occurs
 * an odd number of times, and the remaining numbers have a pair, then the
 * function will return this number. No check for odd array size.
 * Space complexity O(1), time complexity O(n).
*/
int32_t
get_unique_num(int32_t *    array,
               size_t       len)
{
    if((array == NULL) || (len == 0))
        return 0;

    int32_t res = 0;

    for(int i = 0; i < len; i++)
    {
        res ^= array[i];
    }

    return res;
}
/*----------------------------------------------------------------------------*/
#else /* UNIQNUM_OPTIMAL_VER */
/*!
 * \brief Search in array for number without pair
 * \param array Ptr to array
 * \param len Count of numbers in the array
 * \return unique value without pair, if there is no uniq number, return null
 * \details if array ptr of len is null, return null,
 * space complexity O(1), time complexity O(n**2)
*/
int32_t
get_unique_num(int32_t *    array,
               size_t       len)
{
    int32_t res;

    if((array == NULL) || (len == 0))
        return 0;

    int i, j;
    for(i = 0; i < len; i++)
    {
        for(j = 0; j < len; j++)
        {
            if(i == j)
                continue;

            if(array[i] == array[j])
                break;
        }
        if(array[i] != array[j])
            break; //return array[i];
    }


    res = (i < len) ? array[i] : 0; //'res' must be returned according to the task
    return res;
}
#endif /* UNIQNUM_OPTIMAL_VER */
/*-----------------------------------------------------------------------------*/
