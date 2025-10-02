
#include <stdlib.h>
#include <stdio.h>

#include <utils/exit_codes.h>
#include <utils/allocation.h>
#include <utils/cli_colors.h>

static FILE *_sln_utils_error_stream = NULL;

int sln_utils_alloc_set_stream(FILE* error_stream) {
    if (!error_stream)
        return 1;
    _sln_utils_error_stream = error_stream;
    return 0;
}

void* sln_utils_alloc(size_t num, size_t size_of_element, const char* function_info) {
    
    void* buff = calloc(num, size_of_element);
    if (buff != NULL) {
        return buff;
    }

    size_t total_bytes = num * size_of_element;

    if (_sln_utils_error_stream) {
        sln_utils_cli_color_set(_sln_utils_error_stream, SLN_UTILS_CLI_COLOR_WHITE);
        fputs("[", _sln_utils_error_stream);

        sln_utils_cli_color_set(_sln_utils_error_stream, SLN_UTILS_CLI_COLOR_LIGHTRED);
        fputs("Internal error", _sln_utils_error_stream);

        sln_utils_cli_color_set(_sln_utils_error_stream, SLN_UTILS_CLI_COLOR_WHITE);
        fprintf(_sln_utils_error_stream, "]: Allocation failure (%zu bytes) in %s.\n", total_bytes, function_info);

        fflush(_sln_utils_error_stream);
    }

    return NULL;
}
