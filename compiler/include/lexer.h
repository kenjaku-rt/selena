#ifndef SELENA_LEXER_H_
#define SELENA_LEXER_H_

// 09.03.2025

#include <stack>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>
#include <ctype.h>
#include <math.h>

typedef enum {
    SLN_TOKEN_EOF,

    SLN_TOKEN_KEYWORD,
    SLN_TOKEN_TYPE,
    SLN_TOKEN_IDENTIFIER,

    SLN_TOKEN_LITERAL_STRING,
    SLN_TOKEN_LITERAL_UINTEGER,
    SLN_TOKEN_LITERAL_DECIMAL,

    SLN_TOKEN_OPERATOR,
    SLN_TOKEN_BRACKET,
    SLN_TOKEN_NEWLINE,

} sln_token_type_t;

typedef struct {
    sln_token_type_t type;
    union {
        struct {
            char* data;
            size_t length;
        } string;
        long double decimal;
        int64_t     i64;
        uint64_t    u64;
    };
} sln_token_t;


typedef struct {
    sln_token_t* tokens;
    size_t num_of_tokens;
} sln_token_buffer_t;

typedef enum {
    SLN_OPERATOR_INVALID,

    // arithmetic
    SLN_OPERATOR_PLUS,
    SLN_OPERATOR_MINUS,
    SLN_OPERATOR_MUL,
    SLN_OPERATOR_DIV,
    SLN_OPERATOR_MOD,

    // logic
    SLN_OPERATOR_LESS,
    SLN_OPERATOR_GREATER,
    SLN_OPERATOR_EQUAL,
    // ! - SLN_OPERATOR_EXCL_MARK

    // Binary
    SLN_OPERATOR_BITWISE_AND,
    SLN_OPERATOR_BITWISE_OR,
    SLN_OPERATOR_BITWISE_NOT,
    SLN_OPERATOR_BITWISE_XOR,

    // Other
    SLN_OPERATOR_COMMA,
    SLN_OPERATOR_EXCL_MARK,
} sln_operator_t;

typedef enum {
    SLN_BRACKET_ROUND_L,
    SLN_BRACKET_CURLY_L,
    SLN_BRACKET_SQUARE_L,

    SLN_BRACKET_ROUND_R,
    SLN_BRACKET_CURLY_R,
    SLN_BRACKET_SQUARE_R,
} sln_bracket_t;


extern char get_char_from_operator(sln_operator_t op);
extern char get_char_from_bracket(sln_bracket_t bracket);

extern void print_token(sln_token_t token);
extern int sln_tokenize(const char* text, sln_token_buffer_t* target_buffer);

#endif // SELENA_LEXER_H_