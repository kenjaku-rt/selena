
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>

#include <lexer/lexer.h>
#include <lexer/lexer_errors.h>

sln_lex_error_t sln_lex_generate(const char* text, sln_lex_token_buffer_t* buffer, FILE* error_stream) {
    if (!text)
        return SLN_LEX_NO_FILE;
    if (!error_stream)
        return SLN_LEX_NO_ERROR_STREAM;


    if 

    for (size_t text_i = 0; text[text_i] != '\0'; ++text_i) {
        
        if (_is_newline(text, &text_i))

    }
    
}

