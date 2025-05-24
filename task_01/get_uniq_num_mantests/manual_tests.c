/**
 * ----------------------------------------------------
 * \file manual_tests.c
 * \brief
 * \author Anton S. (savver.engineer@gmail.com)
 * \date 2025-05-24
 * ----------------------------------------------------
 */

/*--- Includes ---------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "manual_tests.h"
#include "../get_uniq_num/get_uniq_num.h"

/*--- Private Prototypes -----------------------------------------------------*/

int
mantest_scan_array_stdin(int32_t**      data);

int
mantest_read_numbers_fr_file(const char *   file_path,
                             int32_t **     numb_arr_p,
                             int32_t        numb_max_count);

/*=== Functions =============================================================*/
void
mantest_01_user_stdin(void)
{
    int32_t *   numb_arr;
    size_t      numb_arr_size;

    numb_arr_size = mantest_scan_array_stdin(&numb_arr);

    int uniq_val = get_unique_num(numb_arr, numb_arr_size);
    printf("get_unique_num():uniq_val = %d", uniq_val);

    if(numb_arr)
    {
        free(numb_arr);
    }
}
/*---------------------------------------------------------------------------*/
void
mantest_02_read_fr_files(void)
{
    int32_t *   numb_arr;
    size_t      numb_arr_size;

    numb_arr_size = mantest_read_numbers_fr_file("test_data/test_001_arr21_res=11.txt", &numb_arr, 100);

    int uniq_val = get_unique_num(numb_arr, numb_arr_size);
    printf("get_unique_num():uniq_val = %d", uniq_val);

    if(numb_arr)
    {
        free(numb_arr);
    }
}
/*---------------------------------------------------------------------------*/
/*!
 * \brief Read numbers entered by the user from the terminal (stdin)
 * \param data Pointer to data array, dynamically allocated
 * \return Array size, if equal to zero, the array is not entered
 * \details don't forget to free up memory, ptr 'data'
 */
int
mantest_scan_array_stdin(int32_t**      data)
{
    int     n   = 0;
    size_t  size = 0;

    //--- getting number of pairs from user
    while(1)
    {
        printf("Please enter count of pairs (N):\n");
        scanf("%d", &n);
        if(n > 0)
            break;

        fflush( stdin ); //clear input stream
        printf("N must be a number, greater then null\n\n");
    }

    //--- memory allocation for future array
    size = 2 * n + 1;
    *data = malloc(size);
    if(*data == NULL)
    {
        printf("ERR (func:%s, line:%d): memory allocation, malloc(%d)\n",
               __FUNCTION__, __LINE__, size);
        return 0;
    }

    //---
    printf("\nPlease enter 2*N+1 = %d numbers in cycle, press <Enter> after each number\n", size);
    size_t  idx = 0;
    int res;
    int32_t * arr = *data;
    while(idx < size)
    {
        printf("Please enter value:\n");
        res = scanf("%d", &arr[idx]);
        if(res <= 0)
        {
            printf("WARN: can't recognaze number, try again\n\n");
            fflush( stdin );
            continue;
        }
        idx++;
    }
    printf("Entering numbers is complete, you enter:\n");
    for(int i = 0; i < size; i++)
    {
        printf("%d, ", arr[i]);
    }
    printf("\n");

    return size;
}
/*---------------------------------------------------------------------------*/
/*!
 * \brief Read number array from text file, numbers delimeter in ','
 * \param file_path File path with extention
 * \param numb_max_count Read from files not more than this count,
 * for situations when the file is very large
 * \return Size of array, equal to zero or less than zero means error,
 * for error detection see 'mantest_res_t'
 * \details Don't forget to free up memory, ptr 'data'. The correct result
 * may be written in the file name.
 */
int
mantest_read_numbers_fr_file(const char *   file_path,
                             int32_t **     numb_arr_p,
                             int32_t        numb_max_count)
{
    FILE *fp;
    int32_t res;

    //--- file size, file may be greater than 4 Gb
    struct __stat64 stats;
    res = __stat64(file_path, &stats );
    if(res < 0)
    {
        printf("ERR (func:%s, line:%d): file isn't exist!\n",
               __FUNCTION__, __LINE__);
        return mantest_err_file_open;
    }
    uint64_t file_size = stats.st_size;
    printf("file.stats.st_size  = %lld\n", stats.st_size);

#if 0
    //--- reserve space for the maximum possible number of numbers
    //not optimal space usage, but not view file before parsing
    numb_max_count = (numb_max_count < stats.st_size/2) ?
                                         numb_max_count :
                                         stats.st_size/2;

    numb_arr = malloc(numb_max_count * sizeof(int32_t));
    if(!numb_arr)
    {
        printf("ERR (file:%s, line:%d): memory allocation, malloc(%d)\n",
               __FUNCTION__, __LINE__, numb_max_count * sizeof(int32_t));
        return 0;
    }
#endif

    //--- count number of delimeters in file, it equal count of numbers + 1
    fp = fopen(file_path, "r");
    if(!fp)
    {
       printf("ERR (file:%s, line:%d): can't open file!\n",
              __FUNCTION__, __LINE__);
       return mantest_err_file_open;
    }
    int file_numb_count = 0;
    while(!feof(fp))
    {
        if(fgetc(fp) == ',')
           file_numb_count++;

        if(file_numb_count >= numb_max_count)
            break;
    }
    file_numb_count += 1;
    printf("number amount in file  = %d\n", file_numb_count);

    //--- allocate memory for number array
    *numb_arr_p = malloc(file_numb_count * sizeof(int32_t));
    if(!*numb_arr_p)
    {
        printf("ERR (func:%s, line:%d): memory allocation, malloc(%d)\n",
               __FUNCTION__, __LINE__, file_numb_count * sizeof(int32_t));
        fclose(fp);
        return mantest_err_mem_alloc;
    }

    //--- read numbers from file
    rewind(fp);

    int32_t * data = *numb_arr_p;
    int32_t idx = 0;
    printf("read data:\n");
    while(!feof(fp))
    {
        res = fscanf(fp, "%d,", &data[idx]);
        if(res <= 0)
        {
            printf("ERR (func:%s, line:%d): fscanf, file_pos = %d\n",
                   __FUNCTION__, __LINE__, ftell(fp));
            fclose(fp);
            free(*numb_arr_p);
            return mantest_err_data_format;
        }
        printf("%d,", data[idx]);
        idx++;
    }
    printf("\n", data[idx]);

    fclose(fp);
    return file_numb_count;
}
/*---------------------------------------------------------------------------*/
