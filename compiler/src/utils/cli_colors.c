
#include <utils/cli_colors.h>

#if defined(__linux__)
#   include <unistd.h>
#elif defined(WIN_32)
#   include <windows.h>
static HANDLE _out_handle; // = ?
#else
#   warning "The platform does not support console colors."
#endif

void sln_util_cli_color_enable(FILE* stream) {
    // for linux nothing to do.
#if defined(WIN_32)
    _out_handle = (HANDLE)_get_osfhandle(fileno(stream)); 
#endif
}

static const char *_ansi_txt_color_seq[] =
{
    "\x1B[0;30m",
    "\x1B[0;34m",
    "\x1B[0;32m",
    "\x1B[0;36m",
    "\x1B[0;31m",
    "\x1B[0;35m",
    "\x1B[0;33m",
    "\x1B[0;37m",
    "\x1B[1;30m",
    "\x1B[1;34m",
    "\x1B[1;32m",
    "\x1B[1;36m",
    "\x1B[1;31m",
    "\x1B[1;35m",
    "\x1B[1;33m",
    "\x1B[1;37m"
};

void sln_util_cli_setcolor(FILE* stream, sln_util_cli_color_t text_clr) {
    fputs(_ansi_txt_color_seq[text_clr], stream);
#if defined(WIN_32)
    DWORD mode;
    if (GetConsoleMode(_out_handle, &mode)){
        SetConsoleTextAttribute(_out_handle, text_clr);
    }
#endif
}

