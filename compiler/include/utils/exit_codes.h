
/**
 * @file exit_codes.h
 * @brief Application exit codes enumerations.
 * @author Matvey Rybalkin
 * @date 24 September 2025
 */

#ifndef SELENA_EXIT_CODES_H_
#define SELENA_EXIT_CODES_H_

#include <stdlib.h>

/// @brief Application exit code.
typedef int sln_exit_code_t;

/**
 * @brief Enumiration of application exit codes.
 */
enum sln_exit_code {
    SLN_EXIT_SUCCESS = EXIT_SUCCESS,
    SLN_EXIT_FAILURE = EXIT_FAILURE,
    SLN_EXIT_FAILURE_INTERNAL,
};

#endif // SELENA_EXIT_CODES_H_