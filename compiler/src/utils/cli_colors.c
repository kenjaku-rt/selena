
#include <stdbool.h>

#include <utils/cli_colors.h>

#if defined(__linux__)
#   include <unistd.h>
#elif defined(_WIN32)
#   include <windows.h>
static HANDLE _out_handle;
#else
#   warning "The platform does not support console colors."
#endif

static bool _sln_color_is_enable = false;

void sln_utils_cli_color_enable(FILE* stream) {
#if defined(_WIN32)
    _out_handle = (HANDLE)_get_osfhandle(fileno(stream));
    // no-op on Linux
#else
    (void)stream; // avoid unused param warning
#endif
    _sln_color_is_enable = true;
}

void sln_utils_cli_color_disable(void) {
    _sln_color_is_enable = false;
}

/* ANSI escape sequences for Linux/Unix terminals */
static const char* _ansi_txt_color_seq[] = {
    "\x1B[0;30m", "\x1B[0;34m", "\x1B[0;32m", "\x1B[0;36m",
    "\x1B[0;31m", "\x1B[0;35m", "\x1B[0;33m", "\x1B[0;37m",
    "\x1B[1;30m", "\x1B[1;34m", "\x1B[1;32m", "\x1B[1;36m",
    "\x1B[1;31m", "\x1B[1;35m", "\x1B[1;33m", "\x1B[1;37m"
};

void sln_utils_cli_color_set(FILE* stream, sln_utils_cli_color_t text_clr) {
    if (!_sln_color_is_enable)
        return;
#if defined(__linux__)
    fputs(_ansi_txt_color_seq[text_clr], stream);
#elif defined(_WIN32)
    DWORD mode;
    if (GetConsoleMode(_out_handle, &mode)) {
        SetConsoleTextAttribute(_out_handle, text_clr);
    }
#else
    (void)stream;
    (void)text_clr;
#endif
}
