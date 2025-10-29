
#include <stdio.h>
#include <utils/cli_colors.h>

#include <utils/msg_errors.h>
#include <resources/msg_resource.h>

void sln_utils_msg_print(sln_res_msg_t msg_code, sln_utils_msg_type_t type, FILE* stream) {

    sln_utils_cli_color_set(stream, SLN_UTILS_CLI_COLOR_LIGHTBLUE);
    fputs("selena: ", stream);
    sln_utils_cli_color_set(stream, SLN_UTILS_CLI_COLOR_LIGHTGRAY);

    fputs("[", stream);
    switch (type) {
        case SLN_UTILS_MSG_TYPE_INFO:
            sln_utils_cli_color_set(stream, SLN_UTILS_CLI_COLOR_LIGHTBLUE);
            fputs(sln_res_msg_get(SLN_MSG_INIT_INFO), stream);
            break;
        case SLN_UTILS_MSG_TYPE_WARN:
            sln_utils_cli_color_set(stream, SLN_UTILS_CLI_COLOR_YELLOW);
            fputs(sln_res_msg_get(SLN_MSG_INIT_WARN), stream);
            break;
        case SLN_UTILS_MSG_TYPE_ERRR:
            sln_utils_cli_color_set(stream, SLN_UTILS_CLI_COLOR_LIGHTRED);
            fputs(sln_res_msg_get(SLN_MSG_INIT_ERRR), stream);
            break;
    }
    sln_utils_cli_color_set(stream, SLN_UTILS_CLI_COLOR_WHITE);
    fputs("]: ", stream);
    fputs(sln_res_msg_get(msg_code), stream);
    fputs(".\n", stream);
    sln_utils_cli_color_set(stream, SLN_UTILS_CLI_COLOR_LIGHTGRAY);
    // it can be improved
}
