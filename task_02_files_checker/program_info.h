/**
 * ----------------------------------------------------------------------------
 * \file program_info.h
 * \brief Printable help about the program and the command line options used.
 * \author Anton S.
 * \date 2025-05-26
 * ----------------------------------------------------------------------------
 */

#ifndef PROGRAM_INFO_H
#define PROGRAM_INFO_H

#define PROG_NAME   "Files MD5 Checker"
#define PROG_VER    "v.2025.05.26a"
#define PROG_STR_SEP "------------------------------------"

#define PROG_HELP                               \
"usage:                         \n"             \
" files_checker -d files_dir -v \n"

#define PROG_OPTIONS        "d:v:h"
#define PROG_OPT_DIR        'd'
#define PROG_OPT_HELP       'h'
#define PROG_OPT_VERB       'v'

//--------------------------------------------------------
#endif // PROGRAM_INFO_H
