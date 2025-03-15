#include "lexer.h"

typedef enum {
    SLN_TOKEN_EOF,

    SLN_TOKEN_KEYWORD,
    SLN_TOKEN_TYPE,
    SLN_TOKEN_IDENTIFIER,

    SLN_TOKEN_LITERAL_STRING,
    SLN_TOKEN_LITERAL_INTEGER,
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

sln_token_buffer_t sln_token_buffer_allocate(size_t num_of_token) {
    sln_token_buffer_t buffer;
    buffer.num_of_tokens = num_of_token;
    buffer.tokens = (sln_token_t*)calloc(num_of_token, sizeof(sln_token_t));
    return buffer;
}

bool sln_token_buffer_valid(sln_token_buffer_t buffer) {
    return (buffer.num_of_tokens != 0UL && buffer.tokens != NULL);
}

void sln_token_buffer_free(sln_token_buffer_t* buffer) {
    if (!buffer)
        return;
    if (buffer->tokens) {
        free(buffer->tokens);
    }
    buffer->num_of_tokens = 0UL;
}

static int count_newline_bytes(const char* str) {
    int result = 0;
    if ((*str) == '\n' || (*str) == '\r') {
        if ((*str) == '\r' && (*(str + 1)) == '\n') {
            ++result;
        }
        ++result;
    }
    return result;
}

/* ======================================== */

/**
 * Template SLN strlen<>() functions
 */

inline bool _sln_is_digit(const char** a) {
    bool result = isdigit(**a); (*a)++;
    return result;
}


template <typename T, bool (*compare_func)(const T**)>
static size_t sln_strlen(const T* str) {
    if (!str)
        return 0UL;
    size_t length = 0;
    while ((*str) != '\0') {
        if (!compare_func(&str))
            break;
        ++length;
    }
    return length;
}

/* ======================================== */

/**
 * SLN Numbers
 */

typedef enum {
    SLN_INT64,
    SLN_UINT64,
    SLN_DECIMAL,
    SLN_INVALID
} sln_number_type_t;

sln_number_type_t sln_detect_int_type(const char* str) {
    int len = sln_strlen<char, _sln_is_digit>(str);

    if (len < 19) return SLN_INT64;
    if (len == 19) return (strcmp(str, "9223372036854775807") <= 0) ? SLN_INT64 : SLN_UINT64;
    if (len == 20) return (strcmp(str, "18446744073709551615") <= 0) ? SLN_UINT64 : SLN_INVALID;
    return SLN_INVALID;
}

sln_number_type_t sln_detect_number_type(const char* str, size_t* word_length = NULL) {
    bool has_dot = false;

    size_t length = 0UL;
    for (size_t  i = 0;; ++i, ++length) {
        if (!isdigit(str[i])) {
            if (str[i] == '.') {
                has_dot = true;
            } else if (str[i] == 'e' || str[i] == 'E') {
                (*word_length) = length;
                return SLN_DECIMAL;
            }
            break;
        }
    }
    (*word_length) = length;
    if (has_dot) return SLN_DECIMAL;
    return sln_detect_int_type(str);
}


/* ======================================== */

/**
 * SLN Identifiers
 */

static inline bool _is_identifier(const char** a) {
    bool result = (isalnum((**a)) || (**a) == '_'); ++(*a);
    return result;
}

/**
 * SLN Keywords
 */

static inline bool _is_keyword(const char** a) {
    bool result = isalpha((**a)); ++(*a);
    return result;
}

#define SLN_KEYWORDS_LIST \
    X(SLN_KEYWORD_IF, "if") \
    X(SLN_KEYWORD_ELSE, "else") \
    X(SLN_KEYWORD_TRUE, "true") \
    X(SLN_KEYWORD_FALSE, "false") \
    X(SLN_KEYWORD_NIL, "nil") \
    X(SLN_KEYWORD_NAN, "nan") \
    X(SLN_KEYWORD_INF, "inf") \
    X(SLN_KEYWORD_WHILE, "while") \
    X(SLN_KEYWORD_FOR, "for") \
    X(SLN_KEYWORD_IN, "in")

typedef enum {
#define X(name, str) name,
    SLN_KEYWORDS_LIST
#undef X
} sln_keyword_t;

static const char* _sln_keywords_lookup[] = {
#define X(name, str) [name] = str,
    SLN_KEYWORDS_LIST
#undef X
};

#define SLN_TYPES_LIST     \
    X(SLN_TYPE_BLN, "bln") \
    X(SLN_TYPE_F16, "f16") \
    X(SLN_TYPE_F32, "f32") \
    X(SLN_TYPE_F64, "f64") \
    X(SLN_TYPE_I8,  "i8")  \
    X(SLN_TYPE_I16, "i16") \
    X(SLN_TYPE_I32, "i32") \
    X(SLN_TYPE_I64, "i64") \
    X(SLN_TYPE_U8,  "u8")  \
    X(SLN_TYPE_U16, "u16") \
    X(SLN_TYPE_U32, "u32") \
    X(SLN_TYPE_U64, "u64")

typedef enum {
#define X(name, str) name,
    SLN_TYPES_LIST
#undef X
} sln_type_t;

static const char* _sln_types_lookup[] = {
#define X(name, str) [name] = str,
    SLN_TYPES_LIST
#undef X
};

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


#define SLN_KEYWORD_MAX_LEN 5
#define SLN_NUMBER_OF_KEYWORDS sizeof(_sln_keywords_lookup) / sizeof(_sln_keywords_lookup[0])
#define SLN_NUMBER_OF_TYPES sizeof(_sln_types_lookup) / sizeof(_sln_types_lookup[0])

static size_t _strlen_alpha_chk_digits(const char* str, bool* has_digits) {
    if (!str)
        return 0UL;
    size_t length = 0; 
    (*has_digits) = false;
    while ((*str) != '\0') {
        if (isdigit((*str)))
            (*has_digits) = true;
        if (!isalnum((*str)))
            break;
        ++length; ++str;
    }
    return length;
}

/*

inline bool _sln_is_digit(const char** a) {
    bool result = isdigit(**a); (*a)++;
    return result;
}


template <typename T, bool (*compare_func)(const T**)>
static size_t sln_strlen(const T* str) {
    if (!str)
        return 0UL;
    size_t length = 0;
    while ((*str) != '\0') {
        if (!compare_func(&str))
            break;
        ++length;
    }
    return length;
}
*/

static char get_open_bracket_for(char bracket) {
    switch (bracket) {
        case '}': return '{';
        case ']': return '[';
        case ')': return '(';
        default : return '\0';
    }
}

static sln_operator_t get_operator_from(char symbol) {
    switch (symbol) {
        case '!': return SLN_OPERATOR_EXCL_MARK;
        case ':': return SLN_OPERATOR_COMMA;
        case '=': return SLN_OPERATOR_EQUAL;
        case '<': return SLN_OPERATOR_LESS;
        case '>': return SLN_OPERATOR_GREATER;
        case '~': return SLN_OPERATOR_BITWISE_NOT;
        case '&': return SLN_OPERATOR_BITWISE_AND;
        case '|': return SLN_OPERATOR_BITWISE_OR;
        case '^': return SLN_OPERATOR_BITWISE_XOR;
        case '+': return SLN_OPERATOR_PLUS;
        case '-': return SLN_OPERATOR_MINUS;
        case '*': return SLN_OPERATOR_MUL;
        case '/': return SLN_OPERATOR_DIV;
        case '%': return SLN_OPERATOR_MOD;
        default : return SLN_OPERATOR_INVALID;
    }
}

static sln_bracket_t get_bracket_from(char symbol) {
    switch (symbol) {
        case '(': return SLN_BRACKET_ROUND_L;
        case '{': return SLN_BRACKET_CURLY_L;
        case '[': return SLN_BRACKET_SQUARE_L;
        case ')': return SLN_BRACKET_ROUND_R;
        case '}': return SLN_BRACKET_CURLY_R;
        case ']': return SLN_BRACKET_SQUARE_R;
    }
    __builtin_unreachable();
}



/* ======================================== */

int sln_tokenize(const char* text, sln_token_buffer_t* target_buffer) {
    if (!text)
        return -1;

    sln_token_buffer_t buffer = sln_token_buffer_allocate(4096);
    
    if (!sln_token_buffer_valid(buffer))
        return -1;

    std::stack<char> st;

    size_t text_i = 0UL;
    for (size_t token_i = 0, text_i = 0UL; text[text_i] != '\0'; ++text_i, ++token_i) {
        size_t text_substep = 0UL;
        char* text_cursor = (char*)text + text_i;

        printf("Current character = (%lu)[%c]\n", text_i, (*text_cursor));
        printf("Character: %c (%d)\n", *text_cursor, *text_cursor);

        while ((*text_cursor) == '\n' || (*text_cursor) == '\r') {
            if (*text_cursor == '\r' && *(text_cursor + 1) == '\n') {
                ++text_i; ++text_cursor;
            }
            ++text_i; ++text_cursor;
        }

        while (isblank(*text_cursor)) {
            // like goto token_is_ready;
            ++text_i; ++text_cursor;
        } if ((*text_cursor) == '\0')
            goto end;

        printf("Current character (no spaces) = (%lu)[%c]\n", text_i, (*text_cursor));
        printf("Character: %c (%d)\n", *text_cursor, *text_cursor);

        // ========== LITERALS - Numbers.
        if (isdigit((*text_cursor))) {
            printf("Numbers");
            sln_number_type_t type = sln_detect_number_type(text_cursor, &text_substep);
            
            switch (type) {
                case SLN_INT64:
                    // можно систему счисления поменять на лету.
                    buffer.tokens[token_i].u64 = strtoll(text_cursor, NULL, 10);
                    buffer.tokens[token_i].type = SLN_TOKEN_LITERAL_INTEGER;
                    break;
                case SLN_UINT64:
                    buffer.tokens[token_i].u64 = strtoull(text_cursor, NULL, 10);
                    buffer.tokens[token_i].type = SLN_TOKEN_LITERAL_UINTEGER;
                    break;
                case SLN_DECIMAL:
                    // увы не везде.
                    buffer.tokens[token_i].decimal = strtold(text_cursor, NULL);
                    buffer.tokens[token_i].type = SLN_TOKEN_LITERAL_DECIMAL;
                    break;
                default:
                    goto error_handler;
            }
        } else 

        // ========== KEYWORDS / TYPES / IDENTIFIERS
        if (isalpha((*text_cursor))) {
            printf("KEYWORDS / TYPES / IDENTIFIERS\n");
            bool has_digits = false;
            size_t word_length = _strlen_alpha_chk_digits(text_cursor, &has_digits);
            printf("STRLEN = %lu\n", word_length);
            text_substep = word_length;


            if (word_length <= SLN_KEYWORD_MAX_LEN) {
                printf("word_length <= SLN_KEYWORD_MAX_LEN\n");
                // Keywords don't have digits
                if (!has_digits) {
                    for (size_t i = 0; i < SLN_NUMBER_OF_KEYWORDS; ++i) {
                        if (memcmp(text_cursor, _sln_keywords_lookup[i], word_length) == 0) {
                            buffer.tokens[token_i].type = SLN_TOKEN_KEYWORD;
                            buffer.tokens[token_i].u64 = i;
                            goto token_is_ready;
                        } // !!!! UTF-8
                    }
                }

                // Types can be with and without  digits
                for (size_t i = 0; i < SLN_NUMBER_OF_TYPES; ++i) {
                    if (memcmp(text_cursor, _sln_types_lookup[i], word_length) == 0) {
                        buffer.tokens[token_i].type = SLN_TOKEN_TYPE;
                        buffer.tokens[token_i].u64 = i;
                        goto token_is_ready;
                    } // !!!! UTF-8
                }

            }
            printf("ELSE\n");
            // Identifiers
            buffer.tokens[token_i].string.data = (char*)text_cursor;
            buffer.tokens[token_i].string.length = word_length;
            buffer.tokens[token_i].type = SLN_TOKEN_IDENTIFIER;
        } else {
            printf("Symbols\n");
            // ========== SYMBOLS
            char symbol = (*text_cursor);
            switch (symbol) {
                // ========== LITERALS - strings
                case '\'':
                case '\"': {
                        printf("STRING: \n");
                        if (*(text_cursor + 1) != '\0') {
                            buffer.tokens[token_i].string.data = (text_cursor + 1);
                            buffer.tokens[token_i].type = SLN_TOKEN_LITERAL_STRING;
                        }
                        else
                            goto error_handler;

                        ++text_cursor;
                        do {
                            if ((*text_cursor) == symbol) {
                                break;
                            }
                            printf("Writing string: %c\n", (*text_cursor));
                            ++text_i; ++text_cursor;
                            ++buffer.tokens[token_i].string.length;
                        } while ((*text_cursor) != '\0');
                        ++text_i;
                        printf("STRING FINISHED\n");
                    } break;

                // ========== CONSTRUCTIONS - brackets
                
                case '[':
                case '{':
                case '(':
                    printf("OPENED [%c]\n", symbol);
                    st.push(symbol);
                    buffer.tokens[token_i].type = SLN_TOKEN_OPERATOR;
                    buffer.tokens[token_i].u64 = get_bracket_from(symbol);
                    break;
                case ']':
                case '}':
                case ')':
                    printf("CLOSED [%c]\n", symbol);
                    if (!st.empty() && st.top() == get_open_bracket_for(symbol)) {
                        st.pop();
                        buffer.tokens[token_i].type = SLN_TOKEN_OPERATOR;
                        buffer.tokens[token_i].u64 = get_bracket_from(symbol);
                    } else {
                        printf("STACK EMPTY: %c", st.top());
                        goto error_handler;
                    }
                    break;
                
                // ========== OPERATORS and SYMBOLS

                case '!':
                case ':':
                case '=':
                

                case '<':
                case '>':
                case '~':
                case '&':
                case '|':
                case '^':
                case '+':
                case '-':
                case '*':
                case '/':
                case '%': {
                    printf("Operator detected!\n");
                    buffer.tokens[token_i].type = SLN_TOKEN_OPERATOR;
                    if ((buffer.tokens[token_i].u64 = get_operator_from(symbol)) == SLN_OPERATOR_INVALID) {
                        printf("ERROR! INVALID OPERATOR!\n");
                        goto error_handler;
                    }
                    if (*(text_cursor + 1) == '=') {
                        ++token_i; 
                        printf("SECOND = \n");
                        buffer.tokens[token_i].type = SLN_TOKEN_OPERATOR;
                        buffer.tokens[token_i].u64 = SLN_OPERATOR_EQUAL;
                        ++text_substep;
                    }
                } break;
                
                case '#': {
                    int newline_bytes = 0;
                    while ((newline_bytes = count_newline_bytes(text_cursor)) == 0) {
                        if ((*text_cursor) == '\0') {
                            goto end;
                        }
                        ++text_substep; ++text_cursor;
                    }
                    text_substep += newline_bytes;
                    buffer.tokens[token_i].type = SLN_TOKEN_NEWLINE;
                } break;
                default:
                    printf("INVALID CHARACTER!\n");
                    goto error_handler;
            }
            ++text_substep;
        }

token_is_ready:
        text_i += text_substep - 1;
        printf("substep = %lu\n", text_substep);
        printf("text_i = %lu\n", text_i);
    }
    
// ========== SUCCESS
end:
    (*target_buffer) = buffer;
    return 0;

// ========== ERROR
error_handler:
    printf("Error!");
    sln_token_buffer_free(&buffer);
    return 1;
}

void print_token(sln_token_t token) {
    switch (token.type) {
    case SLN_TOKEN_KEYWORD:
        printf("%s(SLN_TOKEN_KEYWORD)\n", _sln_keywords_lookup[token.u64]);
        break;
    case SLN_TOKEN_TYPE:
        printf("%s(SLN_TOKEN_TYPE)\n", _sln_types_lookup[token.u64]);
        break;
    case SLN_TOKEN_IDENTIFIER:
        printf("%s(SLN_TOKEN_IDENTIFIER)\n", token.string.data);
        break;
    case SLN_TOKEN_LITERAL_STRING:
        printf("%s(SLN_TOKEN_LITERAL_STRING)\n", token.string.data);
        break;
    case SLN_TOKEN_LITERAL_INTEGER:
        printf("%lu(SLN_TOKEN_LITERAL_INTEGER)\n", token.u64);
        break;
    case SLN_TOKEN_LITERAL_DECIMAL:
        printf("%lu(SLN_TOKEN_LITERAL_DECIMAL)\n", token.u64);
        break;
    
    case SLN_TOKEN_BRACKET:
        printf("%lu(SLN_TOKEN_BRACKET)\n", token.u64);
        break;

    case SLN_TOKEN_OPERATOR:
        printf("%lu(SLN_TOKEN_OPERATOR)\n", token.u64);
        break;

    default:
        printf("Not yet! = %i\n", token.type);
        break;
    }
}

/* ======================================== */

const char* text = 
"a:i32 = 100     # Muichiro Tokito was here :->\n"
"b:i64! = 0xFFFF\n"
"\n"
"c:str = \"Moon breathing :::->\"\n"
"\n"
"if (a != b) {"
"\tprint(\"a != b\")\n"
"} else {"
"\tprint(\"a == b\")\n"
"}\n"
"";

int main(void) {
    printf("selena\n");
    sln_token_buffer_t buffer = (sln_token_buffer_t){
        .num_of_tokens = 0
    };
    printf("%d - status;", sln_tokenize(text, &buffer));

    printf("Tokens: (%lu)\n", buffer.num_of_tokens);
    for (size_t i = 0; i < buffer.num_of_tokens; ++i) {
        print_token(buffer.tokens[i]);
        if (buffer.tokens[i].type == SLN_TOKEN_EOF)
            break;
    }

    return 0;
}


/*

    parse_number("12345");                        // int64
    parse_number("-12345");                       // int64
    parse_number("9223372036854775807");          // int64
    parse_number("9223372036854775808");          // uint64
    parse_number("18446744073709551615");         // uint64
    parse_number("18446744073709551616");         // Invalid
    parse_number("-9223372036854775809");         // Invalid

void parse_number(const char* str) {
    sln_number_type_t type = sln_detect_number_type(text + i);

    switch (type) {
        case SLN_INT64:
            printf("Parsed int64: %lld\n", strtoll(str, NULL, 10));
            break;
        case SLN_UINT64:
            printf("Parsed uint64: %llu\n", strtoull(str, NULL, 10));
            break;
        case SLN_DECIMAL:
            printf("Parsed long double: %.21Lf\n", strtold(str, NULL));
            break;
        default:
            printf("Invalid number: %s\n", str);
    }
}

*/