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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "list.h"
#include "files_checker.h"

typedef struct fch_finfo
{
    struct list_head    head_l;
    char *              name;
    int                 res;
}
fch_finfo_t;

void
fch_file_list_init(struct fch_finfo * finfo_l)
{
    INIT_LIST_HEAD(&finfo_l->head_l);
}

int
fch_file_list_add(struct fch_finfo * finfos_l, char * fname)
{
    struct fch_finfo *   new_finfo;
    int                  len;

    new_finfo = malloc(sizeof(struct fch_finfo));
    if(!new_finfo)
        return fch_err_mem_alloc;

    new_finfo->name = malloc(strlen(fname) + 1);
    if(!new_finfo->name)
    {
        free(new_finfo);
        return fch_err_mem_alloc;
    }

    list_add(&new_finfo->head_l, &finfos_l->head_l);

    return fch_ok;
}

int
fch_scan_dir(struct fch_finfo * finfo_l,
             const char * dirname);

struct fch_finfo    finfos;

/*=== Functions ==============================================================*/

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
int
fch_check_dir(const char * dirname)
{
    DIR *               dir;
  //struct fch_finfo    finfos;
    int                 res;

    dir = opendir(dirname); // Открываем текущий каталог
    if (dir == NULL)
    {
        printf("opendir");
        return fch_err_dir_open;
    }
    if(closedir(dir))
    {
        printf("opendir");
        return fch_err_dir_open;
    }

    fch_file_list_init(&finfos);

    res = fch_scan_dir(&finfos, dirname);

    return res;
}
/*----------------------------------------------------------------------------*/
int
fch_scan_dir(struct fch_finfo * finfo_l,
             const char *       dirname)
{
    DIR *               dir;
    struct dirent *     entry;
    struct stat         fstat;
    int                 res;

    printf("Read of '%s':\n",dirname);

    dir = opendir(dirname);
    if (dir == NULL)
    {
        return fch_err_dir_open;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name,".") && //not system files '.', '..'
            strcmp(entry->d_name,".."))
        {
            //if (opendir(entry->d_name)==NULL) - not work
            int new_name_len = strlen(dirname) + strlen(entry->d_name) + 2;
            char * new_fname = malloc(new_name_len);
            if(!new_fname)
                return fch_err_mem_alloc;

            snprintf(new_fname, new_name_len, "%s/%s", dirname, entry->d_name);

            res = stat(new_fname, &fstat);
            if (res == 0)
            {
                if (!S_ISDIR(fstat.st_mode))
                {
                    printf("new dir: %s", new_fname);
                    fch_file_list_add(finfo_l, new_fname);
                }
                else
                {
                    fch_scan_dir(finfo_l, new_fname);
                }
            }
            else
            {
                printf("errno = %d", errno);
            }
            free(new_fname);
        }
    }//while
    if (closedir(dir) != 0)
    {
        return fch_err_dir_open;
    }

    return fch_ok;
}
/*----------------------------------------------------------------------------*/
