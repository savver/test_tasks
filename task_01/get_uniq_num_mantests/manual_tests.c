/**
 * ----------------------------------------------------------------------------
 * \file manual_tests.c
 * \brief Manual tests, compiled into the program, for
 * library 'get_uniq_num'. May be used:
 * test_01 - static arrays in code
 * test_02 - array entered by the user from the keyboard
 * test_03 - the array is read from a text file
 * test_04 - files for test_03 are read from the folder
 * All tests print data to stdin (console).
 *
 * \author Anton S. (savver.engineer@gmail.com)
 * \date 2025-05-24
 * ----------------------------------------------------------------------------
 */

/*--- Includes ---------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include "manual_tests.h"
#include "../get_uniq_num/get_uniq_num.h"

/*--- Defines, Macroses -----------------------------------------------------*/

#if MANTEST_DBG_PRINT_DETAIL
#define printf_d    printf
#else
#define printf_d(...)    { }
#endif

#ifndef countof
#define countof(a) sizeof(a)/sizeof(*(a))
#endif

typedef struct
{
    int32_t *   arr;
    size_t      arr_size; //in elements
    int32_t     uniq_num;
}
mantest_arr_info_t;

/*--- Global Variables ------------------------------------------------------*/

int32_t     mantest_arr01_res5[] = { 1,2,3,4,
                                     1,2,3,4,  5};

int32_t     mantest_arr02_res6[] = { 1,2,3,4,5,
                                     4,3,2,1,5,  6};

int32_t     mantest_arr03_res7[] = { 1,2,3,4,5,  7,  4,3,2,1,5};

static
mantest_arr_info_t
mantest_arr_info[]   =
{
    { mantest_arr01_res5, countof(mantest_arr01_res5), 5 },
    { mantest_arr02_res6, countof(mantest_arr02_res6), 6 },
    { mantest_arr03_res7, countof(mantest_arr03_res7), 7 },
};

/*--- Private Prototypes ----------------------------------------------------*/

int
mantest_scan_array_stdin(int32_t**      data);

int
mantest_read_numbers_fr_file(const char *   file_path,
                             int32_t **     numb_arr_p,
                             int32_t        numb_max_count);

int
mantest_read_files_fr_dir(const char *  dir_path,
                          char **       fnames);

int32_t
mantest_parse_res_fr_fname(const char * fname);

/*=== Functions =============================================================*/
/*!
 *  \brief Сheck on ready static arrays inside test lib
 *  \return Number of errors, if 0, it means all check Ok
 *  \details This test use 'mantest_arr_info'
*/
int
mantest_01_static_arr(void)
{
    int32_t *   numb_arr;
    size_t      numb_arr_size;
    int         uniq_val;
    int         res = 0;

    for(int i = 0; i < countof(mantest_arr_info); i++)
    {
#if MANTEST_DBG_PRINT_TEST01_STAT_ARR
    printf_d("array[%d] = { ", i);
    for(int j = 0; j < mantest_arr_info[i].arr_size; j++)
        printf_d("%d, ", mantest_arr_info[i].arr[j]);
    printf_d("}\n");
#endif
        uniq_val = get_unique_num(mantest_arr_info[i].arr,
                                  mantest_arr_info[i].arr_size);

        if(uniq_val != mantest_arr_info[i].uniq_num)
            res++;

        printf_d("%s: get_unique_num() = %d, uniq_num = %d\n",
                 (uniq_val == mantest_arr_info[i].uniq_num) ? "OK" : "ERR",
                 uniq_val, mantest_arr_info[i].uniq_num);
    }
    printf("----------------------------\n");
    printf("total arrays = %d, errors = %d\n\n", countof(mantest_arr_info), res);

    return res;
}
/*---------------------------------------------------------------------------*/
/*!
 *  \brief The function interacts with the user by asking for input from the
 *  keyboard and waiting for his input
 *  \return None
 *  \details Infinite loops are used to wait for user input. Always print
 *  debug info.
*/
void
mantest_02_user_stdin(void)
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
/*!
 *  \brief Reading an array of numbers from a file and passing it to a
 *  unique number finder function.
 *  \param fname Full file name with extention.
 *  \return 0 if test pass, 1 if there were errors.
 *  \details Read no more than MANTEST_MAX_NUMBERS_FR_FILE numbers from file.
 *  The file name must have a pattern '*res=val.txt', where val is the unique
 *  number to search for. The test compares the found number with the number
 *  in the file name.
*/
int
mantest_03_read_fr_file(const char * fname)
{
    int32_t *   numb_arr;
    int         numb_arr_size;

    numb_arr_size = mantest_read_numbers_fr_file(fname, &numb_arr,
                                                 MANTEST_MAX_NUMBERS_FR_FILE);
    if(numb_arr_size <= 0)
        return mantest_err_file_open;

    int uniq_val = get_unique_num(numb_arr, numb_arr_size);
    printf_d("get_unique_num():uniq_val = %d\n", uniq_val);

    int32_t  true_res = mantest_parse_res_fr_fname(fname);
    printf("%s:res = %s\n", fname, (true_res == uniq_val) ? "OK" : "ERR");

    if(numb_arr)
        free(numb_arr);

    return (true_res == uniq_val) ? 0 : 1;
}
/*---------------------------------------------------------------------------*/
/*!
 *  \brief Search for all text files in a folder. For each file,
 *  mantest_03_read_fr_file() is called.
 *  \param dirname Full directory name.
 *  \return If greater than 0, it means errors amount. Number of files where
 *  the test failed. If less than 0, it means directory reading error.
 *  \details Read no more than MANTEST_MAX_NUMBERS_FR_FILE numbers from file.
 *  The file name must have a pattern '*res=val.txt', where val is the unique
 *  number to search for. The test compares the found number with the number
 *  in the file name.
*/
int
mantest_04_files_fr_dir(const char * dirname)
{
    char *  fnames;
    int     files_cnt;

    files_cnt = mantest_read_files_fr_dir(dirname, &fnames);
    if(files_cnt <= 0)
    {
        printf("ERR (func:%s, line:%d): stop test\n",
                __FUNCTION__, __LINE__);
        return -1;
    }

    int err = 0;
    char *   fname = fnames;
    for(int i = 0; i < files_cnt; i++)
    {
        err += mantest_03_read_fr_file(fname);
        fname = strchr(fname, '\0') + 1;
    }

    if(fnames)
        free(fnames);

    return err;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Read numbers entered by the user from the terminal (stdin).
 * \param data Pointer to data array, dynamically allocated.
 * \return Array size, if equal to zero, the array is not entered.
 * \details Infinite loops are used to wait for user input. Always print
 *  debug info. Don't forget to free up memory, ptr 'data'.
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
 * \brief Read number array from text file, numbers delimeter is ','.
 * \param fname Full file name with extention.
 * \param numb_max_count Read from files not more than this count,
 * for situations when the file is very large.
 * \return Size of array, equal to zero or less than zero means error,
 * for error detection see 'mantest_res_t'.
 * \details Don't forget to free up memory, ptr 'data'. The correct result
 * may be written in the file name.
 */
int
mantest_read_numbers_fr_file(const char *   fname,
                             int32_t **     numb_arr_p,
                             int32_t        numb_max_count)
{
    FILE *      fp;
    int32_t     res;

    //--- file size
    //for WIN: if need we can use __stat64 for files more than 4 Gb
#if defined(_WIN64) ||  defined (_WIN32)
    struct __stat64 stats;    
    res = __stat64(fname, &stats );
#else
    struct stat stats;
    res = stat(fname, &stats );
#endif
    if(res < 0)
    {
        printf("ERR (func:%s, line:%d): file isn't exist!\n",
               __FUNCTION__, __LINE__);
        return mantest_err_file_open;
    }
    printf_d("\nfile_name             = %s\n", fname);
    printf_d(  "file.stats.st_size    = %lld\n", stats.st_size);

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
    fp = fopen(fname, "r");
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
    printf_d("number amount in file = %d\n", file_numb_count);

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

#if MANTEST_DBG_PRINT_FILE_DATA
    printf_d("read data:\n");
#endif

    int32_t * data = *numb_arr_p;
    int32_t idx = 0;
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
#if MANTEST_DBG_PRINT_FILE_DATA
        printf_d("%d,", data[idx]);
#endif
        idx++;
    }
#if MANTEST_DBG_PRINT_FILE_DATA
    printf_d("\n");
#endif

    fclose(fp);
    return file_numb_count;
}
/*---------------------------------------------------------------------------*/
/*!
 * \brief Search for the desired unique number val in the file name using
 * the template "*res=val.txt"
 * \param fname Full file name (path) with extention
 * \return Parsed number, or 0 if there was error
 * \details File name must contains symbol '=' before extention,
 * it must be followed by an integer. Integer not greater than
 * 11 symbols
 */
int32_t
mantest_parse_res_fr_fname(const char * fname)
{
    char        num_str[11+1];
    int32_t     num = 0;

    char *  num_str_beg = strchr(fname, '=') + 1;
    char *  num_str_end = strstr(fname, ".txt");
    if((num_str_beg == NULL) || (num_str_end == NULL))
        return 0;

    int  num_str_len = num_str_end - num_str_beg;
    if(num_str_len > sizeof(num_str))
        return 0;

    strncpy(num_str, num_str_beg, num_str_len);
    sscanf(num_str, "%d", &num);

    return num;
}
/*---------------------------------------------------------------------------*/
/*!
 * \brief Search all text files in a folder.
 * \param dirname Full directory name (path).
 * \param Pointer to file names buffer. The buffer contains strings separated
 * by the '\0'.
 * \return Parsed number, or 0 if there was error.
 * \details Fix word separator in file name '\' (only for Windows)
 */
int
mantest_read_files_fr_dir(const char *  dirname,
                          char **       fnames)
{
    DIR *               dir;
    char *              file_sep = "\\";

    dir = opendir(dirname);
    if (dir == NULL)
    {
        printf("ERR (func:%s, line:%d): can't ohen dir '%s'\n",
               __FUNCTION__, __LINE__, dirname);
        return mantest_err_file_open;
    }

    struct dirent * dir_entry;
    int file_cnt = 0;
    while (dir_entry = readdir(dir))
    {
        if(strstr(dir_entry->d_name, ".txt"))
        {
            file_cnt++;
        }
    }
    printf_d("count of txt files in dir = %d\n", file_cnt);

    closedir(dir);
    dir = opendir(dirname);
    if (dir == NULL)
    {
        printf("ERR (func:%s, line:%d): can't ohen dir '%s'\n",
               __FUNCTION__, __LINE__, dirname);
        return mantest_err_file_open;
    }

    *fnames = malloc(file_cnt * MANTEST_MAX_FNAME_LEN);
    if(!*fnames)
    {
        printf("ERR (func:%s, line:%d): memory allocation, malloc(%d)\n",
               __FUNCTION__, __LINE__, file_cnt * MANTEST_MAX_FNAME_LEN);
        closedir(dir);
        return NULL;
    }
    char *  str_p = *fnames;
    while (dir_entry = readdir(dir))
    {
        if(strstr(dir_entry->d_name, ".txt"))
        {
            strcpy(str_p, dirname);
                   str_p += strlen(dirname);
            strcpy(str_p, file_sep);
                   str_p += strlen(file_sep);
            strcpy(str_p, dir_entry->d_name);
                   str_p += strlen(dir_entry->d_name) + 1;
        }
    }

    return file_cnt;
}
/*---------------------------------------------------------------------------*/

