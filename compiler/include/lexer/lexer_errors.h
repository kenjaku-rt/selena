#ifndef SELENA_LEXER_ERRORS_H_
#define SELENA_LEXER_ERRORS_H_

#include <stdio.h>
#include <stdint.h>

typedef enum {
    SLN_LEX_OK,
    SLN_LEX_NO_FILE,
    SLN_LEX_NO_ERROR_STREAM,
} sln_lex_error_t;


#endif // SELENA_LEXER_ERRORS_H_
