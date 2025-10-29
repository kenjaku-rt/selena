

#include "msg_resource.h"

const char* _msg_resource[] = {

    [SLN_MSG_INIT_INFO] = "Info",
    [SLN_MSG_INIT_WARN] = "Warning",
    [SLN_MSG_INIT_ERRR] = "Error",

    [SLN_MSG_NO_ARGS] = "no input files",

};

const char* sln_res_msg_get(sln_res_msg_t msg_code) {
    if (msg_code < _SLN_MSG_COUNT)
        return _msg_resource[msg_code];
    return "[INVALID RESOURCE CODE]; Check msg_recource.c/.h.";
}

