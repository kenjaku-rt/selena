#include <selena.h>

int main(int argc, char* argv[]) {

    sln_util_cli_color_enable(stdout);

    if (argc < 1) {
        sln_util_cli_color_set(stdout, SLN_UTIL_CLI_COLOR_RED);
        return EXIT_SUCCESS;
    }

    sln_utils_run_stx_t current_section = SLN_UTILS_RUN_NOTH;
    for (int i = 1; i <= argc; ++i) {
        for (int j = 0; argv[i][j] != '\0'; j++) {
            // '-flag'
            if (argv[i][j] == '-')
                ++j;

            // '--flag'
            if (argv[i][j] == '-') {
                
            } 
            
            // '-(f/o/c)'
            else {
                switch (argv[i][0]) {
                    case SLN_UTILS_IS_RUN_FLAG__DSL:
                        current_section = (sln_utils_run_stx_t)argv[i][0];
                        break;
                    default:
                        current_section = SLN_UTILS_RUN_NOTH;
                }
            }
        }

    }

    return EXIT_SUCCESS;
}