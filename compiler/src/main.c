#include <selena.h>

sln_exit_code_t main(int argc, char* argv[]) {

    sln_utils_cli_color_enable(stdout);
    sln_utils_alloc_set_stream(stderr);

    if (argc < 2) {
        sln_utils_msg_print(SLN_MSG_NO_ARGS, SLN_UTILS_MSG_TYPE_ERRR, stderr);
        return SLN_EXIT_SUCCESS;
    }
    (void)argv;
    return SLN_EXIT_SUCCESS;
}