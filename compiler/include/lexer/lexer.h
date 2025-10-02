
/**
 * @file lexer.h
 * @brief Lexer/tokenizer functionality.
 * @author Matvey Rybalkin
 * @date 24 September 2025
 */

#ifndef SELENA_LEXER_H_
#define SELENA_LEXER_H_

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#include "lexer_errors.h"

/**
 * @enum sln_lex_token_type_t
 * @brief Tokens used by Selena lexer.
 *
 * Note: Keep ordering consistent with parser expectations.
 */
typedef enum {
    // --- Special ---
    SLN_LEX_TOKEN_EOF,        /**< End of file */
    SLN_LEX_TOKEN_EON,        /**< End of line */
    SLN_LEX_TOKEN_UNKNOWN,    /**< Unknown/invalid token */

    // --- Identifiers & literals ---
    SLN_LEX_TOKEN_IDENTIFIER, /**< User-defined names */
    SLN_LEX_TOKEN_INT_LITERAL,
    SLN_LEX_TOKEN_FLOAT_LITERAL,
    SLN_LEX_TOKEN_CHAR_LITERAL,
    SLN_LEX_TOKEN_STRING_LITERAL,

    // --- Keywords ---
    SLN_LEX_TOKEN_KW_NAMESPACE,
    SLN_LEX_TOKEN_KW_TYPE,
    SLN_LEX_TOKEN_KW_STRUCT,
    SLN_LEX_TOKEN_KW_ENUM,
    SLN_LEX_TOKEN_KW_USE,
    SLN_LEX_TOKEN_KW_VAR,
    SLN_LEX_TOKEN_KW_RETURN,
    SLN_LEX_TOKEN_KW_FOR,
    SLN_LEX_TOKEN_KW_WHILE,
    SLN_LEX_TOKEN_KW_IF,
    SLN_LEX_TOKEN_KW_ELSE,
    SLN_LEX_TOKEN_KW_SWITCH,
    SLN_LEX_TOKEN_KW_CASE,
    SLN_LEX_TOKEN_KW_DEFAULT,
    SLN_LEX_TOKEN_KW_BREAK,
    SLN_LEX_TOKEN_KW_CONTINUE,

    // --- Operators ---
    // Arithmetic
    SLN_LEX_TOKEN_PLUS,       /**< + */
    SLN_LEX_TOKEN_MINUS,      /**< - */
    SLN_LEX_TOKEN_STAR,       /**< * */
    SLN_LEX_TOKEN_SLASH,      /**< / */
    SLN_LEX_TOKEN_PERCENT,    /**< % */

    // Increment/decrement
    SLN_LEX_TOKEN_INCREMENT,  /**< ++ */
    SLN_LEX_TOKEN_DECREMENT,  /**< -- */

    // Bitwise
    SLN_LEX_TOKEN_AMP,        /**< & */
    SLN_LEX_TOKEN_PIPE,       /**< | */
    SLN_LEX_TOKEN_CARET,      /**< ^ */
    SLN_LEX_TOKEN_TILDE,      /**< ~ */
    SLN_LEX_TOKEN_LSHIFT,     /**< << */
    SLN_LEX_TOKEN_RSHIFT,     /**< >> */

    // Logical
    SLN_LEX_TOKEN_BANG,       /**< ! */
    SLN_LEX_TOKEN_AND_AND,    /**< && */
    SLN_LEX_TOKEN_OR_OR,      /**< || */

    // Comparisons
    SLN_LEX_TOKEN_EQ,         /**< == */
    SLN_LEX_TOKEN_NE,         /**< != */
    SLN_LEX_TOKEN_LT,         /**< < */
    SLN_LEX_TOKEN_GT,         /**< > */
    SLN_LEX_TOKEN_LE,         /**< <= */
    SLN_LEX_TOKEN_GE,         /**< >= */

    // Assignments
    SLN_LEX_TOKEN_ASSIGN,         /**< = */
    SLN_LEX_TOKEN_PLUS_ASSIGN,    /**< += */
    SLN_LEX_TOKEN_MINUS_ASSIGN,   /**< -= */
    SLN_LEX_TOKEN_STAR_ASSIGN,    /**< *= */
    SLN_LEX_TOKEN_SLASH_ASSIGN,   /**< /= */
    SLN_LEX_TOKEN_PERCENT_ASSIGN, /**< %= */
    SLN_LEX_TOKEN_AMP_ASSIGN,     /**< &= */
    SLN_LEX_TOKEN_PIPE_ASSIGN,    /**< |= */
    SLN_LEX_TOKEN_CARET_ASSIGN,   /**< ^= */
    SLN_LEX_TOKEN_LSHIFT_ASSIGN,  /**< <<= */
    SLN_LEX_TOKEN_RSHIFT_ASSIGN,  /**< >>= */

    // Other operators
    SLN_LEX_TOKEN_COLON,      /**< : */
    SLN_LEX_TOKEN_DOUBLE_COLON, /**< :: */
    SLN_LEX_TOKEN_AT,         /**< @ */
    SLN_LEX_TOKEN_ARROW,      /**< -> */
    SLN_LEX_TOKEN_COMMA,      /**< , */
    SLN_LEX_TOKEN_DOT,        /**< . */
    SLN_LEX_TOKEN_ELLIPSIS,   /**< ... */
    SLN_LEX_TOKEN_QUESTION,   /**< ? */

    // --- Brackets & separators ---
    SLN_LEX_TOKEN_LPAREN,     /**< ( */
    SLN_LEX_TOKEN_RPAREN,     /**< ) */
    SLN_LEX_TOKEN_LBRACE,     /**< { */
    SLN_LEX_TOKEN_RBRACE,     /**< } */
    SLN_LEX_TOKEN_LBRACKET,   /**< [ */
    SLN_LEX_TOKEN_RBRACKET,   /**< ] */
    SLN_LEX_TOKEN_SEMICOLON,  /**< ; */

    _SLN_LEX_TOKEN_COUNT       /**< Number of tokens */
} sln_lex_token_type_t;

/**
 * @struct sln_lex_token_t
 * @brief Represents a single token with optional data.
 */
typedef struct {
    sln_lex_token_type_t type;
    union {
        const char* cstr;   /**< For identifiers, strings */
        uint64_t u64;       /**< For unsigned literals */
        int64_t i64;        /**< For signed literals */
        long double lfloat; /**< For floating literals */
    } data;
} sln_lex_token_t;

/**
 * @struct sln_lex_token_buffer_t
 * @brief Buffer of tokens, used as lexer output.
 */
typedef struct {
    sln_lex_token_t* tokens; /**< Token array */
    size_t len;              /**< Number of tokens */
} sln_lex_token_buffer_t;

/**
 * @brief Lexical analysis of input text.
 *
 * @param text Input source string
 * @param buffer Output token buffer
 * @param error_stream Error reporting stream
 * @return Lexer error code
 */
extern sln_lex_error_t sln_lex_generate(
    const char* text,
    sln_lex_token_buffer_t* buffer,
    FILE* error_stream);

#endif // SELENA_LEXER_H_
