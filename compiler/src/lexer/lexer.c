#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <utils/allocation.h>
#include <lexer/lexer.h>
#include <lexer/lexer_errors.h>

#define SLN_LEXER_INITIAL_SIZE 1024UL
#define SLN_LEXER_GROW_FACTOR 2
#define SLN_LEXER_SMALL_STRING_SIZE 64

static struct {
    const char* keyword;
    sln_lex_token_type_t type;
    uint8_t length;
} _keyword_table[] = {
    {"ARGS",      SLN_LEX_TOKEN_KW_ARGS, 4},
    {"MAIN",      SLN_LEX_TOKEN_KW_MAIN, 4},
    {"bln",       SLN_LEX_TOKEN_KW_BLN, 3},
    {"break",     SLN_LEX_TOKEN_KW_BREAK, 5},
    {"case",      SLN_LEX_TOKEN_KW_CASE, 4},
    {"continue",  SLN_LEX_TOKEN_KW_CONTINUE, 8},
    {"default",   SLN_LEX_TOKEN_KW_DEFAULT, 7},
    {"else",      SLN_LEX_TOKEN_KW_ELSE, 4},
    {"enum",      SLN_LEX_TOKEN_KW_ENUM, 4},
    {"for",       SLN_LEX_TOKEN_KW_FOR, 3},
    {"i16",       SLN_LEX_TOKEN_KW_I16, 3},
    {"i32",       SLN_LEX_TOKEN_KW_I32, 3},
    {"i64",       SLN_LEX_TOKEN_KW_I64, 3},
    {"i8",        SLN_LEX_TOKEN_KW_I8, 2},
    {"if",        SLN_LEX_TOKEN_KW_IF, 2},
    {"namespace", SLN_LEX_TOKEN_KW_NAMESPACE, 9},
    {"nil",       SLN_LEX_TOKEN_KW_NIL, 3},
    {"return",    SLN_LEX_TOKEN_KW_RETURN, 6},
    {"str",       SLN_LEX_TOKEN_KW_STR, 3},
    {"struct",    SLN_LEX_TOKEN_KW_STRUCT, 6},
    {"switch",    SLN_LEX_TOKEN_KW_SWITCH, 6},
    {"type",      SLN_LEX_TOKEN_KW_TYPE, 4},
    {"u16",       SLN_LEX_TOKEN_KW_U16, 3},
    {"u32",       SLN_LEX_TOKEN_KW_U32, 3},
    {"u64",       SLN_LEX_TOKEN_KW_U64, 3},
    {"u8",        SLN_LEX_TOKEN_KW_U8, 2},
    {"use",       SLN_LEX_TOKEN_KW_USE, 3},
    {"usize",     SLN_LEX_TOKEN_KW_USIZE, 5},
    {"var",       SLN_LEX_TOKEN_KW_VAR, 3},
    {"while",     SLN_LEX_TOKEN_KW_WHILE, 5}
};

static const size_t _keyword_table_size = sizeof(_keyword_table) / sizeof(_keyword_table[0]);

static inline bool _is_newline(const char* text, size_t* pos) {
    if (text[*pos] == '\n') {
        (*pos)++;
        return true;
    }
    if (text[*pos] == '\r' && text[*pos + 1] == '\n') {
        *pos += 2;
        return true;
    }
    return false;
}

static inline bool _is_whitespace(const char* text, size_t* pos) {
    if (text[*pos] == ' ' || text[*pos] == '\t') {
        (*pos)++;
        return true;
    }
    return false;
}

static sln_lex_token_type_t _get_keyword_type(const char* str, size_t len) {
    for (size_t i = 0; i < _keyword_table_size; i++) {
        if (_keyword_table[i].length == len && 
            memcmp(_keyword_table[i].keyword, str, len) == 0) {
            return _keyword_table[i].type;
        }
    }
    return SLN_LEX_TOKEN_UNKNOWN;
}

static char _process_escape_sequence(const char* text, size_t* pos) {
    (*pos)++;
    switch (text[*pos]) {
        case 'a': (*pos)++; return '\a';
        case 'b': (*pos)++; return '\b';
        case 'f': (*pos)++; return '\f';
        case 'n': (*pos)++; return '\n';
        case 'r': (*pos)++; return '\r';
        case 't': (*pos)++; return '\t';
        case 'v': (*pos)++; return '\v';
        case '\\': (*pos)++; return '\\';
        case '\'': (*pos)++; return '\'';
        case '"': (*pos)++; return '"';
        case '0': (*pos)++; return '\0';
        case 'x': {
            (*pos)++;
            if (!isxdigit(text[*pos])) return 0;
            char hex[3] = {0};
            hex[0] = text[(*pos)++];
            if (isxdigit(text[*pos])) hex[1] = text[(*pos)++];
            return (char)strtoul(hex, NULL, 16);
        }
        default: return text[(*pos)++];
    }
}

static bool _parse_comment(const char* text, size_t* pos, sln_lex_token_t* token) {
    if (text[*pos] == '#' && text[*pos + 1] != '#') {
        (*pos)++;
        size_t content_start = *pos;
        while (text[*pos] != '\0' && !_is_newline(text, pos)) (*pos)++;
        
        size_t length = *pos - content_start;
        if (length > 0) {
            char* comment_str = SLN_ALLOC(length + 1, char);
            if (!comment_str) return false;
            memcpy(comment_str, text + content_start, length);
            comment_str[length] = '\0';
            token->data.cstr = comment_str;
        }
        token->type = SLN_LEX_TOKEN_COMMENT;
        return true;
    }
    
    if (text[*pos] == '#' && text[*pos + 1] == '#') {
        *pos += 2;
        size_t content_start = *pos;
        
        while (text[*pos] != '\0') {
            if (text[*pos] == '#' && text[*pos + 1] == '#') {
                size_t content_length = *pos - content_start;
                if (content_length > 0) {
                    char* comment_str = SLN_ALLOC(content_length + 1, char);
                    if (!comment_str) return false;
                    memcpy(comment_str, text + content_start, content_length);
                    comment_str[content_length] = '\0';
                    token->data.cstr = comment_str;
                }
                token->type = SLN_LEX_TOKEN_COMMENT;
                *pos += 2;
                return true;
            }
            (*pos)++;
        }
        
        size_t content_length = *pos - content_start;
        if (content_length > 0) {
            char* comment_str = SLN_ALLOC(content_length + 1, char);
            if (!comment_str) return false;
            memcpy(comment_str, text + content_start, content_length);
            comment_str[content_length] = '\0';
            token->data.cstr = comment_str;
        }
        token->type = SLN_LEX_TOKEN_COMMENT;
        return true;
    }
    
    return false;
}

static bool _parse_identifier(const char* text, size_t* pos, sln_lex_token_t* token) {
    size_t start = *pos;
    while (isalnum(text[*pos]) || text[*pos] == '_') (*pos)++;
    
    size_t length = *pos - start;
    sln_lex_token_type_t keyword_type = _get_keyword_type(text + start, length);
    
    if (keyword_type != SLN_LEX_TOKEN_UNKNOWN) {
        token->type = keyword_type;
        return true;
    }
    
    token->type = SLN_LEX_TOKEN_IDENTIFIER;
    char* identifier = SLN_ALLOC(length + 1, char);
    if (!identifier) return false;
    memcpy(identifier, text + start, length);
    identifier[length] = '\0';
    token->data.cstr = identifier;
    return true;
}

static bool _parse_string(const char* text, size_t* pos, sln_lex_token_t* token, FILE* error_stream) {
    (*pos)++;
    (void)error_stream;
    char stack_buffer[SLN_LEXER_SMALL_STRING_SIZE];
    char* buffer = stack_buffer;
    size_t buf_size = SLN_LEXER_SMALL_STRING_SIZE;
    size_t buf_len = 0;
    
    while (text[*pos] != '\0' && text[*pos] != '"') {
        if (buf_len >= buf_size - 1) {
            buf_size *= 2;
            char* new_buf = SLN_ALLOC(buf_size, char);
            if (!new_buf) {
                if (buffer != stack_buffer) free(buffer);
                return false;
            }
            memcpy(new_buf, buffer, buf_len);
            if (buffer != stack_buffer) free(buffer);
            buffer = new_buf;
        }
        
        if (text[*pos] == '\\') {
            buffer[buf_len++] = _process_escape_sequence(text, pos);
        } else {
            buffer[buf_len++] = text[(*pos)++];
        }
    }
    
    if (text[*pos] == '"') {
        (*pos)++;
        buffer[buf_len] = '\0';
        
        if (buffer == stack_buffer) {
            char* final_str = SLN_ALLOC(buf_len + 1, char);
            if (final_str) {
                memcpy(final_str, buffer, buf_len + 1);
                token->data.cstr = final_str;
            } else {
                return false;
            }
        } else {
            if (buf_len + 1 < buf_size / 2) {
                char* final_str = SLN_ALLOC(buf_len + 1, char);
                if (final_str) {
                    memcpy(final_str, buffer, buf_len + 1);
                    free(buffer);
                    token->data.cstr = final_str;
                } else {
                    token->data.cstr = buffer;
                }
            } else {
                token->data.cstr = buffer;
            }
        }
        
        token->type = SLN_LEX_TOKEN_STRING_LITERAL;
        return true;
    }
    
    if (buffer != stack_buffer) free(buffer);
    return false;
}

static bool _parse_hex_number(const char* text, size_t* pos, sln_lex_token_t* token) {
    *pos += 2;
    size_t start = *pos;
    
    while (isxdigit(text[*pos])) (*pos)++;
    
    size_t length = *pos - start;
    if (length == 0) return false;
    
    char* num_str = SLN_ALLOC(length + 1, char);
    if (!num_str) return false;
    
    memcpy(num_str, text + start, length);
    num_str[length] = '\0';
    token->type = SLN_LEX_TOKEN_INT_LITERAL;
    token->data.u64 = strtoull(num_str, NULL, 16);
    free(num_str);
    return true;
}

static bool _parse_binary_number(const char* text, size_t* pos, sln_lex_token_t* token) {
    *pos += 2;
    size_t start = *pos;
    
    while (text[*pos] == '0' || text[*pos] == '1') (*pos)++;
    
    size_t length = *pos - start;
    if (length == 0) return false;
    
    char* num_str = SLN_ALLOC(length + 1, char);
    if (!num_str) return false;
    
    memcpy(num_str, text + start, length);
    num_str[length] = '\0';
    token->type = SLN_LEX_TOKEN_INT_LITERAL;
    token->data.u64 = strtoull(num_str, NULL, 2);
    free(num_str);
    return true;
}

static bool _parse_octal_number(const char* text, size_t* pos, sln_lex_token_t* token) {
    size_t start = *pos;
    while (text[*pos] >= '0' && text[*pos] <= '7') (*pos)++;
    
    size_t length = *pos - start;
    if (length == 0) return false;
    
    char* num_str = SLN_ALLOC(length + 1, char);
    if (!num_str) return false;
    
    memcpy(num_str, text + start, length);
    num_str[length] = '\0';
    token->type = SLN_LEX_TOKEN_INT_LITERAL;
    token->data.u64 = strtoull(num_str, NULL, 8);
    free(num_str);
    return true;
}

static bool _parse_decimal_number(const char* text, size_t* pos, sln_lex_token_t* token) {
    size_t start = *pos;
    bool has_decimal = false;
    bool has_exponent = false;
    
    while (isdigit(text[*pos])) (*pos)++;
    
    if (text[*pos] == '.') {
        has_decimal = true;
        (*pos)++;
        while (isdigit(text[*pos])) (*pos)++;
    }
    
    if (text[*pos] == 'e' || text[*pos] == 'E') {
        has_exponent = true;
        (*pos)++;
        if (text[*pos] == '+' || text[*pos] == '-') (*pos)++;
        while (isdigit(text[*pos])) (*pos)++;
    }
    
    size_t length = *pos - start;
    if (length == 0) return false;
    
    char* num_str = SLN_ALLOC(length + 1, char);
    if (!num_str) return false;
    
    memcpy(num_str, text + start, length);
    num_str[length] = '\0';
    
    if (has_decimal || has_exponent) {
        token->type = SLN_LEX_TOKEN_FLOAT_LITERAL;
        token->data.lfloat = strtold(num_str, NULL);
    } else {
        token->type = SLN_LEX_TOKEN_INT_LITERAL;
        token->data.u64 = strtoull(num_str, NULL, 10);
    }
    
    free(num_str);
    return true;
}

static bool _parse_number(const char* text, size_t* pos, sln_lex_token_t* token) {
    if (text[*pos] == '0') {
        char next_char = text[*pos + 1];
        if (next_char == 'x' || next_char == 'X') return _parse_hex_number(text, pos, token);
        if (next_char == 'b' || next_char == 'B') return _parse_binary_number(text, pos, token);
        if (isdigit(next_char)) return _parse_octal_number(text, pos, token);
    }
    return _parse_decimal_number(text, pos, token);
}

static bool _parse_char(const char* text, size_t* pos, sln_lex_token_t* token, FILE* error_stream) {
    (*pos)++;
    (void)error_stream;
    if (text[*pos] == '\0') return false;
    
    char char_value;
    if (text[*pos] == '\\') {
        char_value = _process_escape_sequence(text, pos);
    } else {
        char_value = text[(*pos)++];
    }
    
    if (text[*pos] == '\'') {
        (*pos)++;
        token->type = SLN_LEX_TOKEN_CHAR_LITERAL;
        token->data.i64 = (int64_t)char_value;
        return true;
    }
    return false;
}

static bool _parse_operator(const char* text, size_t* pos, sln_lex_token_t* token) {
    char current = text[*pos];
    char next = text[*pos + 1];
    
    switch (current) {
        case '+':
            if (next == '+') { token->type = SLN_LEX_TOKEN_INCREMENT; *pos += 2; }
            else if (next == '=') { token->type = SLN_LEX_TOKEN_PLUS_ASSIGN; *pos += 2; }
            else { token->type = SLN_LEX_TOKEN_PLUS; (*pos)++; }
            return true;
        case '-':
            if (next == '>') { token->type = SLN_LEX_TOKEN_ARROW; *pos += 2; }
            else if (next == '-') { token->type = SLN_LEX_TOKEN_DECREMENT; *pos += 2; }
            else if (next == '=') { token->type = SLN_LEX_TOKEN_MINUS_ASSIGN; *pos += 2; }
            else { token->type = SLN_LEX_TOKEN_MINUS; (*pos)++; }
            return true;
        case '*':
            if (next == '=') { token->type = SLN_LEX_TOKEN_STAR_ASSIGN; *pos += 2; }
            else { token->type = SLN_LEX_TOKEN_STAR; (*pos)++; }
            return true;
        case '/':
            if (next == '=') { token->type = SLN_LEX_TOKEN_SLASH_ASSIGN; *pos += 2; }
            else { token->type = SLN_LEX_TOKEN_SLASH; (*pos)++; }
            return true;
        case '=':
            if (next == '=') { token->type = SLN_LEX_TOKEN_EQ; *pos += 2; }
            else { token->type = SLN_LEX_TOKEN_ASSIGN; (*pos)++; }
            return true;
        case '!':
            if (next == '=') { token->type = SLN_LEX_TOKEN_NE; *pos += 2; }
            else { token->type = SLN_LEX_TOKEN_BANG; (*pos)++; }
            return true;
        case '<':
            if (next == '<') {
                if (text[*pos + 2] == '=') { token->type = SLN_LEX_TOKEN_LSHIFT_ASSIGN; *pos += 3; }
                else { token->type = SLN_LEX_TOKEN_LSHIFT; *pos += 2; }
            } else if (next == '=') { token->type = SLN_LEX_TOKEN_LE; *pos += 2; }
            else { token->type = SLN_LEX_TOKEN_LT; (*pos)++; }
            return true;
        case '>':
            if (next == '>') {
                if (text[*pos + 2] == '=') { token->type = SLN_LEX_TOKEN_RSHIFT_ASSIGN; *pos += 3; }
                else { token->type = SLN_LEX_TOKEN_RSHIFT; *pos += 2; }
            } else if (next == '=') { token->type = SLN_LEX_TOKEN_GE; *pos += 2; }
            else { token->type = SLN_LEX_TOKEN_GT; (*pos)++; }
            return true;
        case '&':
            if (next == '&') { token->type = SLN_LEX_TOKEN_AND_AND; *pos += 2; }
            else if (next == '=') { token->type = SLN_LEX_TOKEN_AMP_ASSIGN; *pos += 2; }
            else { token->type = SLN_LEX_TOKEN_AMP; (*pos)++; }
            return true;
        case '|':
            if (next == '|') { token->type = SLN_LEX_TOKEN_OR_OR; *pos += 2; }
            else if (next == '=') { token->type = SLN_LEX_TOKEN_PIPE_ASSIGN; *pos += 2; }
            else { token->type = SLN_LEX_TOKEN_PIPE; (*pos)++; }
            return true;
        case ':':
            if (next == ':') { token->type = SLN_LEX_TOKEN_DOUBLE_COLON; *pos += 2; }
            else { token->type = SLN_LEX_TOKEN_COLON; (*pos)++; }
            return true;
        case '.':
            if (next == '.' && text[*pos + 2] == '.') { token->type = SLN_LEX_TOKEN_ELLIPSIS; *pos += 3; }
            else { token->type = SLN_LEX_TOKEN_DOT; (*pos)++; }
            return true;
        case '%': token->type = SLN_LEX_TOKEN_PERCENT; break;
        case '^': token->type = SLN_LEX_TOKEN_CARET; break;
        case '~': token->type = SLN_LEX_TOKEN_TILDE; break;
        case '?': token->type = SLN_LEX_TOKEN_QUESTION; break;
        case '@': token->type = SLN_LEX_TOKEN_AT; break;
        case ',': token->type = SLN_LEX_TOKEN_COMMA; break;
        case ';': token->type = SLN_LEX_TOKEN_SEMICOLON; break;
        case '(': token->type = SLN_LEX_TOKEN_LPAREN; break;
        case ')': token->type = SLN_LEX_TOKEN_RPAREN; break;
        case '{': token->type = SLN_LEX_TOKEN_LBRACE; break;
        case '}': token->type = SLN_LEX_TOKEN_RBRACE; break;
        case '[': token->type = SLN_LEX_TOKEN_LBRACKET; break;
        case ']': token->type = SLN_LEX_TOKEN_RBRACKET; break;
        default: return false;
    }
    (*pos)++;
    return true;
}

void sln_lex_free_tokens(sln_lex_token_buffer_t* buffer) {
    if (!buffer || !buffer->tokens) return;
    
    for (size_t i = 0; i < buffer->len; i++) {
        sln_lex_token_t* token = &buffer->tokens[i];
        if ((token->type == SLN_LEX_TOKEN_IDENTIFIER || 
             token->type == SLN_LEX_TOKEN_STRING_LITERAL ||
             token->type == SLN_LEX_TOKEN_COMMENT) &&
            token->data.cstr != NULL) {
            free(token->data.cstr);
        }
    }
    
    free(buffer->tokens);
    buffer->tokens = NULL;
    buffer->len = 0;
}

sln_lex_error_t sln_lex_generate(const char* text, sln_lex_token_buffer_t* buffer, FILE* error_stream) {
    if (!text) return SLN_LEX_NO_FILE;
    if (!error_stream) return SLN_LEX_NO_ERROR_STREAM;
    if (!buffer) return SLN_LEX_NO_TOKEN_BUFFER;

    buffer->tokens = SLN_ALLOC(SLN_LEXER_INITIAL_SIZE, sln_lex_token_t);
    if (!buffer->tokens) return SLN_LEX_ALLOCATION_FAILED;
    
    buffer->len = 0;
    size_t capacity = SLN_LEXER_INITIAL_SIZE;

    for (size_t text_i = 0; text[text_i] != '\0';) {
        if (_is_whitespace(text, &text_i)) continue;
        
        if (_is_newline(text, &text_i)) {
            if (buffer->len >= capacity) {
                capacity *= SLN_LEXER_GROW_FACTOR;
                sln_lex_token_t* new_tokens = SLN_ALLOC(capacity, sln_lex_token_t);
                if (!new_tokens) goto allocation_error;
                memcpy(new_tokens, buffer->tokens, buffer->len * sizeof(sln_lex_token_t));
                free(buffer->tokens);
                buffer->tokens = new_tokens;
            }
            buffer->tokens[buffer->len++].type = SLN_LEX_TOKEN_EOL;
            continue;
        }
        
        if (text[text_i] == '#') {
            if (buffer->len >= capacity) {
                capacity *= SLN_LEXER_GROW_FACTOR;
                sln_lex_token_t* new_tokens = SLN_ALLOC(capacity, sln_lex_token_t);
                if (!new_tokens) goto allocation_error;
                memcpy(new_tokens, buffer->tokens, buffer->len * sizeof(sln_lex_token_t));
                free(buffer->tokens);
                buffer->tokens = new_tokens;
            }
            if (_parse_comment(text, &text_i, &buffer->tokens[buffer->len])) {
                buffer->len++;
            } else {
                text_i++;
            }
            continue;
        }
        
        if (buffer->len >= capacity) {
            capacity *= SLN_LEXER_GROW_FACTOR;
            sln_lex_token_t* new_tokens = SLN_ALLOC(capacity, sln_lex_token_t);
            if (!new_tokens) goto allocation_error;
            memcpy(new_tokens, buffer->tokens, buffer->len * sizeof(sln_lex_token_t));
            free(buffer->tokens);
            buffer->tokens = new_tokens;
        }
        
        sln_lex_token_t* current_token = &buffer->tokens[buffer->len];
        current_token->data.cstr = NULL;
        
        bool token_parsed = false;
        char first_char = text[text_i];
        
        if (isalpha(first_char) || first_char == '_') {
            token_parsed = _parse_identifier(text, &text_i, current_token);
        } else if (isdigit(first_char)) {
            token_parsed = _parse_number(text, &text_i, current_token);
        } else if (first_char == '"') {
            token_parsed = _parse_string(text, &text_i, current_token, error_stream);
        } else if (first_char == '\'') {
            token_parsed = _parse_char(text, &text_i, current_token, error_stream);
        } else {
            token_parsed = _parse_operator(text, &text_i, current_token);
        }
        
        if (token_parsed) {
            buffer->len++;
        } else {
            current_token->type = SLN_LEX_TOKEN_UNKNOWN;
            text_i++;
            buffer->len++;
        }
    }
    
    if (buffer->len >= capacity) {
        capacity += 1;
        sln_lex_token_t* new_tokens = SLN_ALLOC(capacity, sln_lex_token_t);
        if (!new_tokens) goto allocation_error;
        memcpy(new_tokens, buffer->tokens, buffer->len * sizeof(sln_lex_token_t));
        free(buffer->tokens);
        buffer->tokens = new_tokens;
    }
    buffer->tokens[buffer->len].type = SLN_LEX_TOKEN_EOF;
    buffer->tokens[buffer->len].data.cstr = NULL;
    buffer->len++;
    
    return SLN_LEX_OK;

allocation_error:
    sln_lex_free_tokens(buffer);
    return SLN_LEX_ALLOCATION_FAILED;
}