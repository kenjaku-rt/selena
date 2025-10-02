


#ifndef SELENA_MSG_ERRORS_H_
#define SELENA_MSG_ERRORS_H_

#include <stdio.h>

#include <utils/exit_codes.h>
#include <resources/msg_resource.h>

typedef enum {
    SLN_UTILS_MSG_TYPE_INFO = 1,
    SLN_UTILS_MSG_TYPE_WARN = 2,
    SLN_UTILS_MSG_TYPE_ERRR = 3,
} sln_utils_msg_type_t;

/**
 * @brief Displays the message text in a formatted form.
 * 
 *  To use: `msg_code_raw = (SLN_UTILS_MSG_TYPE_**** | msg_code)`
 * or use allias SLN_INFO_PRINT, etc. 
 * 
 * @param[in] msg_code_raw code of the output text.
 * @param[in] stream output stream (stdout/stderr).
 */
void sln_utils_msg_print(sln_res_msg_t msg_code, sln_utils_msg_type_t type, FILE* stream);

#endif // SELENA_MSG_ERRORS_H_