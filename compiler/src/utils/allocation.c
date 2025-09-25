
#include <stdlib.h>
#include <stdio.h>

#include <utils/exit_codes.h>
#include <utils/allocation.h>
#include <utils/cli_colors.h>

void* sln_util_alloc(size_t num, size_t size_of_element, FILE* error_stream, const char* function_info) {
    void* buff = calloc(num, size_of_element);
    if (buff != NULL) {
        return buff;
    }

    size_t total_bytes = num * size_of_element;

    if (error_stream) {
        sln_util_cli_color_set(error_stream, SLN_UTIL_CLI_COLOR_WHITE);
        fputs("[", error_stream);

        sln_util_cli_color_set(error_stream, SLN_UTIL_CLI_COLOR_LIGHTRED);
        fputs("Internal error", error_stream);

        sln_util_cli_color_set(error_stream, SLN_UTIL_CLI_COLOR_WHITE);
        fprintf(error_stream, "]: Allocation failure (%zu bytes) in %s.\n", total_bytes, function_info);

        fflush(error_stream);
    }

    return NULL;
}
