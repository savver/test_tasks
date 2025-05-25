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
#include "MD5-hash-Calculator/md5.h"

/*--- Defines ----------------------------------------------------------------*/

typedef struct fch_finfo
{
    struct list_head    head_l;
    char *              name;
    int                 res;
}
fch_finfo_t;

/*--- Global Variables -------------------------------------------------------*/

struct fch_finfo    finfos;

/*--- Private Prototypes -----------------------------------------------------*/

void
fch_file_list_init(struct fch_finfo * finfo_l);

int
fch_file_list_add(struct fch_finfo * finfos_l, char * fname);

void
fch_file_list_print(struct fch_finfo * finfo_l);

int
fch_scan_dir(struct fch_finfo * finfo_l,
             const char * dirname);

int
fch_scan_dir_v2(struct fch_finfo * finfo_l,
                const char *       dirname);

char *
fch_calc_md5(const char * fname);

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

  //res = fch_scan_dir(&finfos, dirname);
    res = fch_scan_dir_v2(&finfos, dirname);

    fch_file_list_print(&finfos);

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

    printf("scan dir_name: '%s'...\n",dirname);

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
                    printf("new file: %s\n", new_fname);
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
int
fch_scan_dir_v2(struct fch_finfo * finfo_l,
                const char *       dirname)
{
    struct stat         fstat;
    int                 fnum;
    int                 res;
    struct dirent **    namelist;

    printf("scan dir_name: '%s'...\n",dirname);

    fnum = scandir(dirname, &namelist, NULL, alphasort);
    if(fnum < 0)
    {
         return fch_err_dir_open;
    }

    for(int i = 0; i < fnum; i++)
    {
        if (strcmp(namelist[i]->d_name,".") && //not system files '.', '..'
            strcmp(namelist[i]->d_name,".."))
        {
            int new_name_len = strlen(dirname) + strlen(namelist[i]->d_name) + 2;
            char * new_fname = malloc(new_name_len);
            if(!new_fname)
                return fch_err_mem_alloc;

            snprintf(new_fname, new_name_len, "%s/%s", dirname, namelist[i]->d_name);

            res = stat(new_fname, &fstat);
            if (res == 0)
            {
                if (!S_ISDIR(fstat.st_mode))
                {
                    printf("new file: %s\n", new_fname);
                    fch_file_list_add(finfo_l, new_fname);
                }
                else
                {
                    fch_scan_dir_v2(finfo_l, new_fname);
                }
            }
            else
            {
                printf("errno = %d", errno);
            }
            free(new_fname);
            free(namelist[i]);
        }//if
    }//for i
    free(namelist);

    return fch_ok;
}
/*----------------------------------------------------------------------------*/
void
fch_file_list_init(struct fch_finfo * finfo_l)
{
    INIT_LIST_HEAD(&finfo_l->head_l);
}
/*----------------------------------------------------------------------------*/
int
fch_file_list_add(struct fch_finfo * finfos_l, char * fname)
{
    struct fch_finfo *   new_finfo;

    new_finfo = malloc(sizeof(struct fch_finfo));
    if(!new_finfo)
        return fch_err_mem_alloc;

    new_finfo->name = malloc(strlen(fname) + 1);
    if(!new_finfo->name)
    {
        free(new_finfo);
        return fch_err_mem_alloc;
    }

    strcpy(new_finfo->name, fname);
    list_add(&new_finfo->head_l, &finfos_l->head_l);

    return fch_ok;
}
/*----------------------------------------------------------------------------*/
void
fch_file_list_print(struct fch_finfo * finfo_l)
{
    struct fch_finfo *  file;
    char *              hash;

  //list_for_each_entry(file, &finfo_l->head_l, head_l)
    list_for_each_entry_reverse(file, &finfo_l->head_l, head_l)
    {
        hash = fch_calc_md5(file->name);
        printf("%s  calc_md5 = %s\n", file->name, (hash != NULL) ? hash : "ERR");
    }
}
/*----------------------------------------------------------------------------*/
char *
fch_calc_md5(const char * fname)
{
    struct stat         fstat;
    int                 res;
    uint8_t *           text;
    FILE *              fp;
    char*               hash;
    Blocks*             blocks;

    res = stat(fname, &fstat);
    if(res)
    {
        printf("errno = %d", errno);
        return NULL;
    }

    text = malloc(fstat.st_size + 1); //work if file size not greater 4Gb
    if(!text)
    {
        printf("errno = %d", errno);
        return NULL;
    }

    fp = fopen(fname, "r");
    if(!fp)
    {
       printf("ERR (file:%s, line:%d): can't open file!\n",
              __FUNCTION__, __LINE__);
       return NULL;
    }

    if(fread(text, 1, fstat.st_size, fp) != fstat.st_size)
    {
        free(text);
        return NULL;
    }
    text[fstat.st_size] = '\0';

    //make blocks of input and call hash function
    blocks = makeBlocks(text, fstat.st_size);
    hash = md5(blocks);

    free(text);
    return hash;
}
/*----------------------------------------------------------------------------*/
