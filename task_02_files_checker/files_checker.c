/**
 * ----------------------------------------------------------------------------
 * \file files_checker.c
 * \brief Check md5 hash for all files in folder and subfolders.
 * \author Anton S. (savver.engineer@gmail.com)
 * \date 2025-05-26
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
#include <ctype.h>
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

/*! @brief Global only for debug
*/
struct fch_finfo    finfos;

/*--- Private Prototypes -----------------------------------------------------*/

void
fch_flist_init(struct fch_finfo * finfo_l);

int
fch_flist_add(struct fch_finfo * finfos_l, char * fname);

void
fch_flist_rm(struct fch_finfo * file);

void
fch_flist_rm_all(struct fch_finfo * finfo_l);

void
fch_flist_print(struct fch_finfo * finfo_l);

int
fch_scan_dir(struct fch_finfo * finfo_l,
             const char * dirname);

int
fch_scan_dir_v2(struct fch_finfo * finfo_l,
                const char *       dirname);

char *
fch_calc_md5(const char * fname);

int
fch_flist_search_check_md5(struct fch_finfo * finfo_l);

char *
fch_str_tolower(char * str);

/*=== Functions ==============================================================*/
/*!
 * \brief Check md5 hash for all files in folder and subfolders.
 * \param dirname Folder name
 * \return If greater than 0 - number of error files, if 0 - all ok,
 * if less than 0 - runtime error code, enum 'fch_res_t'
 * \details Scan folder & subfolders, generate locally sorted list of file names
 * with extentions '.bin', '.md5', analyze this list, calc md5 hash for each
 * file, and compare hash string in '.md5' file
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
        fprintf(stderr, "ERR: %s(),%d: can't open dir (%s), errno=%d\n",
                __FUNCTION__, __LINE__, dirname, errno);
        return fch_err_dir_open;
    }
    if(closedir(dir)) //unlikely mistake
    {
        fprintf(stderr, "ERR: %s(),%d: can't close dir (%s), errno=%d\n",
                __FUNCTION__, __LINE__, dirname, errno);
        return fch_err_dir_open;
    }

    fch_flist_init(&finfos);

    res = fch_scan_dir_v2(&finfos, dirname);
    fch_flist_print(&finfos);               //debug print

    fch_flist_search_check_md5(&finfos);    //main process

    fch_flist_rm_all(&finfos);

    return res;
}
/*----------------------------------------------------------------------------*/
/*!
 * \brief Old version , see 'fch_scan_dir_v2'
 */
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
        fprintf(stderr, "ERR: %s(),%d: can't open dir (%s), errno=%d\n",
                __FUNCTION__, __LINE__, dirname, errno);
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
            {
                fprintf(stderr, "ERR: %s(),%d: can't allocate memory, errno=%d\n",
                        __FUNCTION__, __LINE__, errno);
                return fch_err_mem_alloc;
            }

            snprintf(new_fname, new_name_len, "%s/%s", dirname, entry->d_name);

            res = stat(new_fname, &fstat);
            if (res == 0)
            {
                if (!S_ISDIR(fstat.st_mode))
                {
                    printf("new file: %s\n", new_fname);
                    fch_flist_add(finfo_l, new_fname);
                }
                else
                {
                    fch_scan_dir(finfo_l, new_fname);
                }
            }
            else
            {
                fprintf(stderr, "ERR: %s(),%d: stat(%s), errno=%d\n",
                        __FUNCTION__, __LINE__, new_fname, errno);
                //may be not critical, try continue work
            }
            free(new_fname);
        }
    }//while
    if (closedir(dir) != 0)
    {
        fprintf(stderr, "ERR: %s(),%d: can't open dir (%s), errno=%d\n",
                __FUNCTION__, __LINE__, dirname, errno);
        return fch_err_dir_open;
    }

    return fch_ok;
}
/*----------------------------------------------------------------------------*/
/*!
 * \brief Scan folder & subfolders, generate locally sorted list of file names
 * with extentions '.bin', '.md5'
 * \param finfo_l File names list
 * \param dirname Folder name
 * \return If 0 - all ok, if less than 0 - runtime error code, enum 'fch_res_t'
 * \details
*/
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
        fprintf(stderr, "ERR: %s(),%d: can't scan dir (%s), errno=%d\n",
                __FUNCTION__, __LINE__, dirname, errno);
         return fch_err_dir_open;
    }

    for(int i = 0; i < fnum; i++)
    {
        if (strcmp(namelist[i]->d_name,".") && //not see system files
            strcmp(namelist[i]->d_name,".."))
        {
            int new_name_len = strlen(dirname) + strlen(namelist[i]->d_name) + 2;
            char * new_fname = malloc(new_name_len);
            if(!new_fname)
            {
                fprintf(stderr, "ERR: %s(),%d: can't allocate memory, errno=%d\n",
                        __FUNCTION__, __LINE__, errno);
                return fch_err_mem_alloc;
            }

            snprintf(new_fname, new_name_len, "%s/%s", dirname, namelist[i]->d_name);

            res = stat(new_fname, &fstat);
            if (res == 0)
            {
                if (!S_ISDIR(fstat.st_mode))
                {
                    if (strstr(namelist[i]->d_name,".bin") ||
                        strstr(namelist[i]->d_name,".md5"))
                    {
                        printf("new file: %s\n", new_fname);
                        fch_flist_add(finfo_l, new_fname);
                    }
                }
                else
                {
                    fch_scan_dir_v2(finfo_l, new_fname);
                }
            }
            else
            {
                fprintf(stderr, "ERR: %s(),%d: stat(%s), errno=%d\n",
                        __FUNCTION__, __LINE__, new_fname, errno);
                //may be not critical, try continue work
            }
            free(new_fname);
            free(namelist[i]);
        }//if
    }//for i
    free(namelist);

    return fch_ok;
}
/*----------------------------------------------------------------------------*/
/*!
 * @brief Init linked list.
 */
void
fch_flist_init(struct fch_finfo * finfo_l)
{
    INIT_LIST_HEAD(&finfo_l->head_l);
}
/*----------------------------------------------------------------------------*/
/*!
 * \brief Add new file name to list.
 * \param finfos_l List for file names.
 * \param fname Ptr on file name.
 * \return Enum 'fch_res_t' codes.
 * \details Allocate memory for new list node and for name string.
*/
int
fch_flist_add(struct fch_finfo * finfos_l, char * fname)
{
    struct fch_finfo *   new_finfo;

    new_finfo = malloc(sizeof(struct fch_finfo));
    if(!new_finfo)
    {
        fprintf(stderr, "ERR: %s(),%d: can't allocate memory, errno=%d\n",
                __FUNCTION__, __LINE__, errno);
        return fch_err_mem_alloc;
    }

    new_finfo->name = malloc(strlen(fname) + 1);
    if(!new_finfo->name)
    {
        free(new_finfo);
        fprintf(stderr, "ERR: %s(),%d: can't allocate memory, errno=%d\n",
                __FUNCTION__, __LINE__, errno);
        return fch_err_mem_alloc;
    }

    strcpy(new_finfo->name, fname);
    list_add(&new_finfo->head_l, &finfos_l->head_l);

    return fch_ok;
}
/*----------------------------------------------------------------------------*/
/*!
 * @brief When list is deleted, memory for name string must free
 */
void
fch_flist_rm(struct fch_finfo * file)
{
    free(file->name);
}
/*----------------------------------------------------------------------------*/
/*!
 * @brief Debug printf file names from all nodes in list, and calc md5 hash for
 * each file. The reverse order of viewing the list is needed so that files
 *  with the .bin extension come before .md5 files.
 */
void
fch_flist_print(struct fch_finfo * finfo_l)
{
    struct fch_finfo *  file;
    char *              hash;

  //list_for_each_entry(file, &finfo_l->head_l, head_l)
    list_for_each_entry_reverse(file, &finfo_l->head_l, head_l)
    {
        hash = fch_calc_md5(file->name);
        printf("%s  calc_md5 = %s\n", file->name, (hash != NULL) ? hash : "ERR");
        free(hash);
    }
}
/*----------------------------------------------------------------------------*/
/*!
 * @brief Remove all nodes from list. Also free memory for file name strings.
 */
void
fch_flist_rm_all(struct fch_finfo * finfo_l)
{
    struct fch_finfo *file, *tmp;

    list_for_each_entry_safe(file, tmp, &finfo_l->head_l, head_l)
    {
        list_del(&file->head_l);
        fch_flist_rm(file);
    }
}
/*----------------------------------------------------------------------------*/
/*!
 * \brief Calc md5 hash for file
 * \param fname Ptr on file name.
 * \return Ptr to hash string, or NULL if errors
 * \details Base on makeBlocks(), md5() from lib 'MD5-hash-Calculator'.
 * 'fstat.st_size' gives the correct size only if the file size not more 4 Gb.
*/
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
        fprintf(stderr, "ERR: %s(),%d: can't allocate memory, errno=%d\n",
                __FUNCTION__, __LINE__, errno);
        return NULL;
    }

    text = malloc(fstat.st_size + 1);
    if(!text)
    {
        fprintf(stderr, "ERR: %s(),%d: can't allocate memory, errno=%d\n",
                __FUNCTION__, __LINE__, errno);
        return NULL;
    }

    fp = fopen(fname, "r");
    if(!fp)
    {
       printf("ERR (file:%s, line:%d): can't open file %s, errno=%d\n",
              __FUNCTION__, __LINE__, fname, errno);
       return NULL;
    }

    if(fread(text, 1, fstat.st_size, fp) != fstat.st_size)
    {
        printf("ERR (file:%s, line:%d): can't read file %s, errno=%d\n",
               __FUNCTION__, __LINE__, fname, errno);
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
/*!
 * \brief Analyze list with file names, search .bin & .md5 files, calc md5 and
 * read it from .md5 file, and compare
 * \param finfo_l File names list
 * \return Number of errors
 * \details scandir() gives us sorted file names, so our list within one folder
 * is sorted. So files with the same name but different extensions come in
 * pairs. First the .bin file, then the .md5 file. If the .bin file is not
 * followed by the .md5 file, this is an error. If the .md5 file is not paired
 * with the .bin file, this is a warning.
*/
int
fch_flist_search_check_md5(struct fch_finfo * finfo_l)
{
    struct fch_finfo *  file;
    char *              hash;
    FILE *              fp;
    char                state = 0;
    int                 err_cnt = 0;
    char                rd_md5[FCH_MD5_STR_SIZE];
    char *              fname_prev;

    list_for_each_entry_reverse(file, &finfo_l->head_l, head_l)
    {
        switch(state)
        {
            case 0:
            //----------
            if(strstr(file->name, ".bin"))
            {
                hash = fch_calc_md5(file->name); //ret NULL if err
              //printf("hash = %s\n", hash);
                state = 1; //wait .md5
            }
            else //.md5 now, not .bin
            {
                printf("WARN: md5 wo bin (%s)\n", file->name);
            }
            //----------
            break;

            case 1:
            //---------
            if(strstr(file->name, ".md5"))
            {
                fp = fopen(file->name, "r");
                if(!fp)
                {
                    fprintf(stderr, "ERR: %s(),%d: can't read file (%s), errno=%d\n",
                            __FUNCTION__, __LINE__, file->name, errno);
                    //maybe not critical error, try continue work...

                    memset(rd_md5, 0, sizeof(rd_md5)); // for printf below
                }
                else
                {
                    fgets(rd_md5, FCH_MD5_STR_SIZE, fp);
                    fch_str_tolower(rd_md5);
                  //printf("rd_md5 = %s\n", rd_md5);
                }

                if(hash && fp &&
                   !strcmp(rd_md5, hash))
                {
                    printf("MD5: %s: OK\n", file->name);
                }
                else
                {
                    printf("MD5: %s: ERR (need_md5=%s, calc_md5=%s)\n",
                           file->name, rd_md5, (hash != NULL) ? hash : '\0');
                    err_cnt++;
                }

                free(hash);
                state = 0; //wait .bin
            }
            else //.bin again
            {
                printf("ERR: no md5 for bin (%s)\n", fname_prev);

                free(hash);
                hash = fch_calc_md5(file->name);
            }
            //---------
            break;
        }//swithc
        fname_prev = file->name;
    }
    return err_cnt;
}
/*----------------------------------------------------------------------------*/
char *
fch_str_tolower(char * str)
{
    char * str_i = str;
    while(*str_i != '\0')
    {
        *str_i = tolower(*str_i);
        str_i++;
    }
    return str;
}
/*----------------------------------------------------------------------------*/
