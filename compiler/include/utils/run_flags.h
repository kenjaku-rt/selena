#ifndef SELENA_UTILS_RUN_FLAGS
#define SELENA_UTILS_RUN_FLAGS

typedef enum {
    SLN_UTILS_RUN_OUT = (int)'o',
    SLN_UTILS_RUN_CODE = (int)'c',
    SLN_UTILS_RUN_FUNC = (int)'f',
    SLN_UTILS_RUN_WARN = (int)'W',
    SLN_UTILS_RUN_NOTH = (int)' ',
} sln_utils_run_flag_prefix_t;

#define SLN_UTILS_IS_RUN_FLAG__DSL\
    (char)SLN_UTILS_RUN_OUT:\
    case (char)SLN_UTILS_RUN_CODE:\
    case (char)SLN_UTILS_RUN_FUNC:\
    case (char)SLN_UTILS_RUN_WARN

typedef enum {
    SLN_UTILS_RUN_
} sln_utils_run_flags_t;

extern const char* sln_utils_run_flag_get_cstr(sln_utils_run_flags_t flag);

#endif // SELENA_UTILS_RUN_FLAGS