#ifndef SELENA_UTILS_CLI_COLORS_H_
#define SELENA_UTILS_CLI_COLORS_H_

/**
 * @file cli_colors.h
 * @brief Changing console colors on different OS
 * @details This file has conditional compilation for Windows and Linux.
 */

#include <stdio.h>

typedef enum {
    SLN_UTIL_CLI_COLOR_BLACK    = 0,
    SLN_UTIL_CLI_COLOR_BLUE     = 1,
    SLN_UTIL_CLI_COLOR_GREEN    = 2,
    SLN_UTIL_CLI_COLOR_CYAN     = 3,
    SLN_UTIL_CLI_COLOR_RED      = 4,
    SLN_UTIL_CLI_COLOR_MAGENTA  = 5,
    SLN_UTIL_CLI_COLOR_BROWN    = 6,
    SLN_UTIL_CLI_COLOR_LIGHTGRAY    = 7,
    SLN_UTIL_CLI_COLOR_DARKGRAY     = 8,
    SLN_UTIL_CLI_COLOR_LIGHTBLUE    = 9,
    SLN_UTIL_CLI_COLOR_LIGHTGREEN   = 10,
    SLN_UTIL_CLI_COLOR_LIGHTCYAN    = 11,
    SLN_UTIL_CLI_COLOR_LIGHTRED     = 12,
    SLN_UTIL_CLI_COLOR_LIGHTMAGENTA = 13,
    SLN_UTIL_CLI_COLOR_YELLOW       = 14,
    SLN_UTIL_CLI_COLOR_WHITE        = 15
} sln_util_cli_color_t;

extern void sln_util_cli_color_enable(FILE* stream);
extern void sln_util_cli_color_set(FILE* stream, sln_util_cli_color_t text_clr);

#endif // SELENA_UTILS_CLI_COLORS_H_
