/**
 * ----------------------------------------------------------------------------
 * \file get_uniq_num.h
 * \brief Header file
 * \author Anton S. (savver.engineer@gmail.com)
 * \date 2025-05-24
 * ----------------------------------------------------------------------------
 */

/*--- Includes ---------------------------------------------------------------*/

#include "get_uniq_num.h"
#include <string.h>

/*=== Functions ==============================================================*/
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
/*-----------------------------------------------------------------------------*/
