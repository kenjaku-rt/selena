#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lexer/lexer.h>
#include <utils/cli_colors.h>

// Функция для красивого вывода токенов
const char* token_type_to_string(sln_lex_token_type_t type) {
    switch (type) {
        case SLN_LEX_TOKEN_EOF: return "EOF";
        case SLN_LEX_TOKEN_EOL: return "EOL";
        case SLN_LEX_TOKEN_COMMENT: return "COMMENT";
        case SLN_LEX_TOKEN_UNKNOWN: return "UNKNOWN";
        case SLN_LEX_TOKEN_IDENTIFIER: return "IDENTIFIER";
        case SLN_LEX_TOKEN_INT_LITERAL: return "INT_LITERAL";
        case SLN_LEX_TOKEN_FLOAT_LITERAL: return "FLOAT_LITERAL";
        case SLN_LEX_TOKEN_CHAR_LITERAL: return "CHAR_LITERAL";
        case SLN_LEX_TOKEN_STRING_LITERAL: return "STRING_LITERAL";
        
        // Keywords
        case SLN_LEX_TOKEN_KW_NAMESPACE: return "KW_NAMESPACE";
        case SLN_LEX_TOKEN_KW_TYPE: return "KW_TYPE";
        case SLN_LEX_TOKEN_KW_STRUCT: return "KW_STRUCT";
        case SLN_LEX_TOKEN_KW_ENUM: return "KW_ENUM";
        case SLN_LEX_TOKEN_KW_USE: return "KW_USE";
        case SLN_LEX_TOKEN_KW_VAR: return "KW_VAR";
        case SLN_LEX_TOKEN_KW_RETURN: return "KW_RETURN";
        case SLN_LEX_TOKEN_KW_FOR: return "KW_FOR";
        case SLN_LEX_TOKEN_KW_WHILE: return "KW_WHILE";
        case SLN_LEX_TOKEN_KW_IF: return "KW_IF";
        case SLN_LEX_TOKEN_KW_ELSE: return "KW_ELSE";
        case SLN_LEX_TOKEN_KW_SWITCH: return "KW_SWITCH";
        case SLN_LEX_TOKEN_KW_CASE: return "KW_CASE";
        case SLN_LEX_TOKEN_KW_DEFAULT: return "KW_DEFAULT";
        case SLN_LEX_TOKEN_KW_BREAK: return "KW_BREAK";
        case SLN_LEX_TOKEN_KW_CONTINUE: return "KW_CONTINUE";
        case SLN_LEX_TOKEN_KW_NIL: return "KW_NIL";
        case SLN_LEX_TOKEN_KW_I8: return "KW_I8";
        case SLN_LEX_TOKEN_KW_I16: return "KW_I16";
        case SLN_LEX_TOKEN_KW_I32: return "KW_I32";
        case SLN_LEX_TOKEN_KW_I64: return "KW_I64";
        case SLN_LEX_TOKEN_KW_U8: return "KW_U8";
        case SLN_LEX_TOKEN_KW_U16: return "KW_U16";
        case SLN_LEX_TOKEN_KW_U32: return "KW_U32";
        case SLN_LEX_TOKEN_KW_U64: return "KW_U64";
        case SLN_LEX_TOKEN_KW_BLN: return "KW_BLN";
        case SLN_LEX_TOKEN_KW_USIZE: return "KW_USIZE";
        case SLN_LEX_TOKEN_KW_STR: return "KW_STR";
        case SLN_LEX_TOKEN_KW_MAIN: return "KW_MAIN";
        case SLN_LEX_TOKEN_KW_ARGS: return "KW_ARGS";
        
        // Operators
        case SLN_LEX_TOKEN_PLUS: return "PLUS";
        case SLN_LEX_TOKEN_MINUS: return "MINUS";
        case SLN_LEX_TOKEN_STAR: return "STAR";
        case SLN_LEX_TOKEN_SLASH: return "SLASH";
        case SLN_LEX_TOKEN_PERCENT: return "PERCENT";
        case SLN_LEX_TOKEN_INCREMENT: return "INCREMENT";
        case SLN_LEX_TOKEN_DECREMENT: return "DECREMENT";
        case SLN_LEX_TOKEN_AMP: return "AMP";
        case SLN_LEX_TOKEN_PIPE: return "PIPE";
        case SLN_LEX_TOKEN_CARET: return "CARET";
        case SLN_LEX_TOKEN_TILDE: return "TILDE";
        case SLN_LEX_TOKEN_LSHIFT: return "LSHIFT";
        case SLN_LEX_TOKEN_RSHIFT: return "RSHIFT";
        case SLN_LEX_TOKEN_BANG: return "BANG";
        case SLN_LEX_TOKEN_AND_AND: return "AND_AND";
        case SLN_LEX_TOKEN_OR_OR: return "OR_OR";
        case SLN_LEX_TOKEN_EQ: return "EQ";
        case SLN_LEX_TOKEN_NE: return "NE";
        case SLN_LEX_TOKEN_LT: return "LT";
        case SLN_LEX_TOKEN_GT: return "GT";
        case SLN_LEX_TOKEN_LE: return "LE";
        case SLN_LEX_TOKEN_GE: return "GE";
        case SLN_LEX_TOKEN_ASSIGN: return "ASSIGN";
        case SLN_LEX_TOKEN_PLUS_ASSIGN: return "PLUS_ASSIGN";
        case SLN_LEX_TOKEN_MINUS_ASSIGN: return "MINUS_ASSIGN";
        case SLN_LEX_TOKEN_STAR_ASSIGN: return "STAR_ASSIGN";
        case SLN_LEX_TOKEN_SLASH_ASSIGN: return "SLASH_ASSIGN";
        case SLN_LEX_TOKEN_PERCENT_ASSIGN: return "PERCENT_ASSIGN";
        case SLN_LEX_TOKEN_AMP_ASSIGN: return "AMP_ASSIGN";
        case SLN_LEX_TOKEN_PIPE_ASSIGN: return "PIPE_ASSIGN";
        case SLN_LEX_TOKEN_CARET_ASSIGN: return "CARET_ASSIGN";
        case SLN_LEX_TOKEN_LSHIFT_ASSIGN: return "LSHIFT_ASSIGN";
        case SLN_LEX_TOKEN_RSHIFT_ASSIGN: return "RSHIFT_ASSIGN";
        case SLN_LEX_TOKEN_COLON: return "COLON";
        case SLN_LEX_TOKEN_DOUBLE_COLON: return "DOUBLE_COLON";
        case SLN_LEX_TOKEN_AT: return "AT";
        case SLN_LEX_TOKEN_ARROW: return "ARROW";
        case SLN_LEX_TOKEN_COMMA: return "COMMA";
        case SLN_LEX_TOKEN_DOT: return "DOT";
        case SLN_LEX_TOKEN_ELLIPSIS: return "ELLIPSIS";
        case SLN_LEX_TOKEN_QUESTION: return "QUESTION";
        
        // Brackets
        case SLN_LEX_TOKEN_LPAREN: return "LPAREN";
        case SLN_LEX_TOKEN_RPAREN: return "RPAREN";
        case SLN_LEX_TOKEN_LBRACE: return "LBRACE";
        case SLN_LEX_TOKEN_RBRACE: return "RBRACE";
        case SLN_LEX_TOKEN_LBRACKET: return "LBRACKET";
        case SLN_LEX_TOKEN_RBRACKET: return "RBRACKET";
        case SLN_LEX_TOKEN_SEMICOLON: return "SEMICOLON";
        
        default: return "UNKNOWN_TYPE";
    }
}

void print_token_color(sln_lex_token_t token, int index) {
    sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_LIGHTGRAY);
    printf("[%3d] ", index);
    
    switch (token.type) {

        case SLN_LEX_TOKEN_EOL:
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_DARKGRAY);
            printf("%-20s", token_type_to_string(token.type));
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_WHITE);
            printf(" '\\n'\n");
            break;

        case SLN_LEX_TOKEN_IDENTIFIER:
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_CYAN);
            printf("%-20s", token_type_to_string(token.type));
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_WHITE);
            printf(" '%.*s'\n", 30, token.data.cstr ? token.data.cstr : "(null)");
            break;
            
        case SLN_LEX_TOKEN_STRING_LITERAL:
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_GREEN);
            printf("%-20s", token_type_to_string(token.type));
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_WHITE);
            printf(" \"%.*s\"\n", 30, token.data.cstr ? token.data.cstr : "(null)");
            break;
            
        case SLN_LEX_TOKEN_INT_LITERAL:
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_YELLOW);
            printf("%-20s", token_type_to_string(token.type));
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_WHITE);
            printf(" %lu\n", token.data.u64);
            break;
            
        case SLN_LEX_TOKEN_FLOAT_LITERAL:
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_YELLOW);
            printf("%-20s", token_type_to_string(token.type));
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_WHITE);
            printf(" %Lf\n", token.data.lfloat);
            break;
            
        case SLN_LEX_TOKEN_CHAR_LITERAL:
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_YELLOW);
            printf("%-20s", token_type_to_string(token.type));
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_WHITE);
            printf(" '%c'\n", (char)token.data.i64);
            break;
            
        case SLN_LEX_TOKEN_COMMENT:
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_DARKGRAY);
            printf("%-20s", token_type_to_string(token.type));
            sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_WHITE);
            if (token.data.cstr) {
                printf(" '%s'\n", token.data.cstr);
            } else {
                printf(" (empty)\n");
            }
            break;

        default:
            if (token.type >= SLN_LEX_TOKEN_KW_NAMESPACE && token.type <= SLN_LEX_TOKEN_KW_ARGS) {
                sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_MAGENTA);
            } else if (token.type >= SLN_LEX_TOKEN_PLUS && token.type <= SLN_LEX_TOKEN_QUESTION) {
                sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_LIGHTRED);
            } else if (token.type >= SLN_LEX_TOKEN_LPAREN && token.type <= SLN_LEX_TOKEN_SEMICOLON) {
                sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_LIGHTBLUE);
            } else {
                sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_WHITE);
            }
            printf("%-20s\n", token_type_to_string(token.type));
            break;
    }
    
    sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_WHITE);
}

int main(int argc, char* argv[]) {
    // Включим цвета в консоли
    sln_utils_cli_color_enable(stdout);

    (void)argc;
    (void)argv;

    const char* test_code = 
        "# Selena language example\n\n"
        "use cli:io;\n\n"
        "namespace main {\n"
        "    type args = struct {\n"
        "        num:usize,\n"
        "        content:str[num]\n"
        "    };\n\n"
        "    type exit_status = enum {\n"
        "        EXIT_SUCCESS,\n"
        "        EXIT_FAILURE,\n"
        "        @exit_status_ext\n"
        "    };\n"
        "}\n\n"
        "## Это многострочный комментарий\n"
        "   который может занимать несколько строк ##\n\n"
        "main::exit_status@exit_status_ext = {\n"
        "    EXIT_ERR1, EXIT_ERR2\n"
        "};\n\n"
        "MAIN(ARGS:(main::args)):i32 {\n\n"
        "    var1:i64 = 10;\n\n"
        "    cli:io.println(\"Selena app 1. You run it with:\");\n\n"
        "    for (i = 0; i < ARGS.num; i++) {\n"
        "        cli:io.println(\"arg[{i}]: \", ARGS.content[i]);\n"
        "    }\n"
        "    \n"
        "    cli:flush();\n\n"
        "    if (var1->usize == ARGS.num) {\n"
        "        return main::exit_status::EXIT_ERR1;\n"
        "    }\n\n"
        "    return main::exit_status::EXIT_SUCCESS;\n"
        "}";
        
    printf("=== Selena Lexer Test ===\n");
    printf("Input code:\n");
    sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_LIGHTGRAY);
    printf("%s\n", test_code);
    sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_WHITE);
    printf("\n=== Token Stream ===\n");
    
    sln_lex_token_buffer_t buffer = {0};
    sln_lex_error_t error = sln_lex_generate(test_code, &buffer, stderr);
    
    if (error != SLN_LEX_OK) {
        fprintf(stderr, "Lexer error: %d\n", error);
        return 1;
    }
    
    printf("Total tokens: %zu\n\n", buffer.len);
    
    for (size_t i = 0; i < buffer.len; i++) {
        print_token_color(buffer.tokens[i], (int)i);
    }
    
    // Освобождаем память
    sln_lex_free_tokens(&buffer);
    
    sln_utils_cli_color_set(stdout, SLN_UTILS_CLI_COLOR_WHITE);
    printf("\n=== Test completed successfully ===\n");
    
    return 0;
}