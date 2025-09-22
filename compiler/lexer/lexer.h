#ifndef SELENA_LEXER_H_
#define SELENA_LEXER_H_

#include <stdint.h>
#include <stddef.h>

typedef enum {
    // --- Spectial ---
    SLN_LEX_TOKEN_EOF,        // End of the file
    SLN_LEX_TOKEN_EON,        // End of the line
    SLN_LEX_TOKEN_UNKNOWN,

    // --- Identifiers ---
    SLN_LEX_TOKEN_IDENTIFIER,
    SLN_LEX_TOKEN_INT_LITERAL,
    SLN_LEX_TOKEN_FLOAT_LITERAL,
    SLN_LEX_TOKEN_CHAR_LITERAL,
    SLN_LEX_TOKEN_STRING_LITERAL,

    // --- Keywords ---
    SLN_LEX_TOKEN_KW_IF,
    SLN_LEX_TOKEN_KW_ELSE,
    SLN_LEX_TOKEN_KW_FOR,
    SLN_LEX_TOKEN_KW_WHILE,
    SLN_LEX_TOKEN_KW_DO,
    SLN_LEX_TOKEN_KW_SWITCH,
    SLN_LEX_TOKEN_KW_CASE,
    SLN_LEX_TOKEN_KW_DEFAULT,
    SLN_LEX_TOKEN_KW_BREAK,
    SLN_LEX_TOKEN_KW_CONTINUE,
    SLN_LEX_TOKEN_KW_RETURN,
    SLN_LEX_TOKEN_KW_GOTO,

    SLN_LEX_TOKEN_KW_STRUCT,
    SLN_LEX_TOKEN_KW_UNION,
    SLN_LEX_TOKEN_KW_ENUM,
    SLN_LEX_TOKEN_KW_TYPEDEF,

    SLN_LEX_TOKEN_KW_CONST,
    SLN_LEX_TOKEN_KW_VOLATILE,
    SLN_LEX_TOKEN_KW_STATIC,
    SLN_LEX_TOKEN_KW_EXTERN,
    SLN_LEX_TOKEN_KW_INLINE,
    SLN_LEX_TOKEN_KW_SIZEOF,

    SLN_LEX_TOKEN_KW_VOID,
    SLN_LEX_TOKEN_KW_CHAR,
    SLN_LEX_TOKEN_KW_SHORT,
    SLN_LEX_TOKEN_KW_INT,
    SLN_LEX_TOKEN_KW_LONG,
    SLN_LEX_TOKEN_KW_FLOAT,
    SLN_LEX_TOKEN_KW_DOUBLE,
    SLN_LEX_TOKEN_KW_SIGNED,
    SLN_LEX_TOKEN_KW_UNSIGNED,
    SLN_LEX_TOKEN_KW_BOOL,
    SLN_LEX_TOKEN_KW_TRUE,
    SLN_LEX_TOKEN_KW_FALSE,
    SLN_LEX_TOKEN_KW_NULLPTR,

    // --- Операторы ---
    // Арифметические
    SLN_LEX_TOKEN_PLUS,       // +
    SLN_LEX_TOKEN_MINUS,      // -
    SLN_LEX_TOKEN_STAR,       // *
    SLN_LEX_TOKEN_SLASH,      // /
    SLN_LEX_TOKEN_PERCENT,    // %

    // Инкремент/декремент
    SLN_LEX_TOKEN_PLUS_PLUS,  // ++
    SLN_LEX_TOKEN_MINUS_MINUS,// --

    // Побитовые
    SLN_LEX_TOKEN_AMP,        // &
    SLN_LEX_TOKEN_PIPE,       // |
    SLN_LEX_TOKEN_CARET,      // ^
    SLN_LEX_TOKEN_TILDE,      // ~
    SLN_LEX_TOKEN_LSHIFT,     // <<
    SLN_LEX_TOKEN_RSHIFT,     // >>

    // Логические
    SLN_LEX_TOKEN_BANG,       // !
    SLN_LEX_TOKEN_AND_AND,    // &&
    SLN_LEX_TOKEN_OR_OR,      // ||

    // Сравнения
    SLN_LEX_TOKEN_EQ,         // ==
    SLN_LEX_TOKEN_NE,         // !=
    SLN_LEX_TOKEN_LT,         // <
    SLN_LEX_TOKEN_GT,         // >
    SLN_LEX_TOKEN_LE,         // <=
    SLN_LEX_TOKEN_GE,         // >=

    // Присваивания
    SLN_LEX_TOKEN_ASSIGN,     // =
    SLN_LEX_TOKEN_PLUS_ASSIGN,// +=
    SLN_LEX_TOKEN_MINUS_ASSIGN,// -=
    SLN_LEX_TOKEN_STAR_ASSIGN,// *=
    SLN_LEX_TOKEN_SLASH_ASSIGN,// /=
    SLN_LEX_TOKEN_PERCENT_ASSIGN,// %=
    SLN_LEX_TOKEN_AMP_ASSIGN, // &=
    SLN_LEX_TOKEN_PIPE_ASSIGN,// |=
    SLN_LEX_TOKEN_CARET_ASSIGN,// ^=
    SLN_LEX_TOKEN_LSHIFT_ASSIGN,// <<=
    SLN_LEX_TOKEN_RSHIFT_ASSIGN,// >>=

    // Прочие операторы
    SLN_LEX_TOKEN_QUESTION,   // ?
    SLN_LEX_TOKEN_COLON,      // :
    SLN_LEX_TOKEN_COMMA,      // ,
    SLN_LEX_TOKEN_DOT,        // .
    SLN_LEX_TOKEN_ARROW,      // ->
    SLN_LEX_TOKEN_ELLIPSIS,   // ...

    // --- Скобки и разделители ---
    SLN_LEX_TOKEN_LPAREN,     // (
    SLN_LEX_TOKEN_RPAREN,     // )
    SLN_LEX_TOKEN_LBRACE,     // {
    SLN_LEX_TOKEN_RBRACE,     // }
    SLN_LEX_TOKEN_LBRACKET,   // [
    SLN_LEX_TOKEN_RBRACKET,   // ]
    SLN_LEX_TOKEN_SEMICOLON,  // ;

    // --- Препроцессор ---
    SLN_LEX_TOKEN_PP_HASH,    // #
    SLN_LEX_TOKEN_PP_INCLUDE,
    SLN_LEX_TOKEN_PP_DEFINE,
    SLN_LEX_TOKEN_PP_IF,
    SLN_LEX_TOKEN_PP_ELIF,
    SLN_LEX_TOKEN_PP_ELSE,
    SLN_LEX_TOKEN_PP_ENDIF,
    SLN_LEX_TOKEN_PP_UNDEF,
    SLN_LEX_TOKEN_PP_PRAGMA,

    SLN_LEX_TOKEN_COUNT       // Число токенов (служебный)
} sln_lex_token_type_t;


typedef struct {
    sln_lex_token_type_t type;
    union {

    } data;
} sln_lex_token_t;

#endif // SELENA_LEXER_H_