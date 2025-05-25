#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include "program_info.h"
#include "files_checker.h"

void usage(void);

/*=== Functions ===============================*/
int main(int argc, char *argv[])
{
    char *   dirname = NULL;
    int      path_len;

    if(argc < 1) // if no options -> show help
    {
        usage();
        exit(1);
    }
    else
    {
        char* opts = PROG_OPTIONS;
        int opt;
        while((opt = getopt(argc, argv, opts)) != -1)
        {
            switch(opt)
            {
               case PROG_OPT_HELP:
               //---------------------
               usage();
               exit(1);
               //---------------------
               break;

               case PROG_OPT_DIR:
               //---------------------
               path_len = strlen(optarg);
               if(path_len > PATH_MAX)
               {
                   fprintf(stderr, "ERR: %s(),%d: path_len (%d) > PATH_MAX\n",
                           __FUNCTION__, __LINE__, path_len);
                   exit(1);
               }
               dirname = malloc(path_len+2);
             //memset(dirname, 0, path_len+2);
               if(!dirname)
               {
                   fprintf(stderr, "ERR: %s(),%d: allocate %d bytes\n",
                           __FUNCTION__, __LINE__, path_len);
                   exit(1);
               }
               strcpy(dirname, optarg); //PATH_MAX, maybe strcpy, len check higher
               //---------------------
               break;

                default:
                break;
            }
        }
    }

    if(!dirname)
    {
        fprintf(stderr, "ERR: %s(),%d: please enter dir path (option: -%c)\n",
                __FUNCTION__, __LINE__, PROG_OPT_DIR);
        exit(1);
    }

    //----------------
    //fch_check_dir("111");
    fch_check_dir(dirname);
    //----------------

    if(dirname)
        free(dirname);

    printf("Program Finish!\n\n");
    return 0;
}
/*-----------------------------------------------------*/
void usage(void)
{
    printf(PROG_STR_SEP  "\n");
    printf(PROG_NAME     "\n");
    printf(PROG_VER      "\n");
    printf(PROG_STR_SEP  "\n");
    printf(PROG_HELP     "\n");
}
/*-----------------------------------------------------*/
/*-----------------------------------------------------*/
