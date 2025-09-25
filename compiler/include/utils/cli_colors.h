
/**
 * @file cli_colors.h
 * @brief Functions for changing the console color.
 * @author Matvey Rybalkin
 * @date 24 September 2025
 */

#ifndef SELENA_UTILS_CLI_COLORS_H_
#define SELENA_UTILS_CLI_COLORS_H_

#include <stdio.h>


/**
 * @enum sln_util_cli_color_t
 * @brief Standard text color codes.
 *
 * Values match ANSI escape codes on Linux
 * and Windows console attributes on Win32.
 */
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

/**
 * @brief Enable console coloring for a given output stream.
 *
 * On Linux this does nothing, as ANSI codes are always supported.
 * On Windows it initializes the console handle.
 *
 * @param stream Output stream (stdout/stderr).
 */
void sln_util_cli_color_enable(FILE* stream);

/**
 * @brief Set text color for a given output stream.
 *
 * On Linux, writes ANSI escape codes.
 * On Windows, calls SetConsoleTextAttribute().
 *
 * @param stream Output stream (stdout/stderr).
 * @param text_clr Desired text color.
 */
void sln_util_cli_color_set(FILE* stream, sln_util_cli_color_t text_clr);

#endif // SELENA_UTILS_CLI_COLORS_H_
