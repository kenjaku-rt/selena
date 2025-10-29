/**
 * @file input_args.h
 * @brief Application startup arguments.
 * @author Matvey Rybalkin
 * @date 14 October 2025
 */

 #ifndef SELENA_INPUT_ARGS_H_
 #define SELENA_INPUT_ARGS_H_
 
 #include <stddef.h>
 #include <stdbool.h>
 
 typedef enum {
     SLN_IN_ARG_TYPE_FILE = 0,
 
     SLN_IN_ARG_TYPE_CODE,      // --code <string>
     SLN_IN_ARG_TYPE_OUTP,      // -o/--out <path>
     SLN_IN_ARG_TYPE_LINK,      // -l/--link <lib or path>
 
     SLN_IN_ARG_TYPE_WARN,      // --warn {all|extra}
     SLN_IN_ARG_TYPE_FUNC,      // --func {custom}
 
     _SLN_IN_ARG_TYPE_COUNT,
 } sln_input_arg_type_t;
 
 typedef enum {
     SLN_IN_ARG_WARN_ALL = 0,
     SLN_IN_ARG_WARN_EXTRA,
 } sln_input_arg_warn_t;
 
 typedef enum {
     SLN_IN_ARG_FUNC_CUSTOM = 0,
 } sln_input_arg_func_t;
 
 typedef struct {
     sln_input_arg_type_t type;
     const char* cstr;
     
     union {
         sln_input_arg_warn_t warn;
         sln_input_arg_func_t func;
         int _unused;
     } subtype;
 } sln_input_arg_t;
 
 /**
  * Парсит argv в массив аргументов.
  * Возвращает true при успехе. При неуспехе out_args/out_count остаются нетронутыми.
  * Выделенную память освобождать через input_args_free().
  */
 bool input_args_parse(int argc, char* argv[],
                       sln_input_arg_t** out_args, size_t* out_count);
 
 /**
  * Удобная обёртка под твой старый прототип.
  * Возвращает NULL при ошибке. В *out_count кладёт размер.
  */
 sln_input_arg_t* input_args_get(int argc, char* argv[], size_t* out_count);
 
 /** Освобождает массив из parse/get. */
 void input_args_free(sln_input_arg_t* args, size_t count);
 
 #endif // SELENA_INPUT_ARGS_H_
 