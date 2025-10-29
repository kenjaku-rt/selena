#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <utils/allocation.h>
#include <utils/input_args.h>


// ------- Вспомогалки -------

static char* sln_strdup(const char* s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char* p = SLN_ALLOC(n, char);
    if (p) memcpy(p, s, n);
    return p;
}

typedef struct {
    sln_input_arg_t* data;
    size_t count;
    size_t cap;
} args_vec_t;

static bool vec_reserve(args_vec_t* v, size_t need_cap) {
    if (v->cap >= need_cap) return true;
    size_t new_cap = v->cap ? v->cap * 2 : 8;
    if (new_cap < need_cap) new_cap = need_cap;
    void* np = realloc(v->data, new_cap * sizeof(*v->data));
    if (!np) return false;
    v->data = (sln_input_arg_t*)np;
    v->cap = new_cap;
    return true;
}

static bool vec_push(args_vec_t* v, sln_input_arg_t const* x) {
    if (!vec_reserve(v, v->count + 1)) return false;
    v->data[v->count++] = *x;
    return true;
}

static void free_one(sln_input_arg_t* a) {
    if (a && a->cstr) {
        free((void*)a->cstr);
        a->cstr = NULL;
    }
}

static bool match_long_opt(const char* arg, const char* name, const char** out_value) {
    // --name or --name=value
    if (strncmp(arg, "--", 2) != 0) return false;
    const char* p = arg + 2;
    size_t len = strlen(name);
    if (strncmp(p, name, len) != 0) return false;
    if (p[len] == '\0') { // exact match
        if (out_value) *out_value = NULL;
        return true;
    }
    if (p[len] == '=') {
        if (out_value) *out_value = p + len + 1;
        return true;
    }
    return false;
}

static bool equals_ignore_case(const char* a, const char* b) {
    for (; *a && *b; ++a, ++b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return false;
    }
    return *a == '\0' && *b == '\0';
}

static bool parse_warn_value(const char* v, sln_input_arg_warn_t* out) {
    if (!v) return false;
    if (equals_ignore_case(v, "all"))   { *out = SLN_IN_ARG_WARN_ALL;   return true; }
    if (equals_ignore_case(v, "extra")) { *out = SLN_IN_ARG_WARN_EXTRA; return true; }
    return false;
}

static bool parse_func_value(const char* v, sln_input_arg_func_t* out) {
    if (!v) return false;
    if (equals_ignore_case(v, "custom")) { *out = SLN_IN_ARG_FUNC_CUSTOM; return true; }
    return false;
}

// ------- Parser -------

bool input_args_parse(int argc, char* argv[],
                      sln_input_arg_t** out_args, size_t* out_count)
{
    assert(argv);
    if (!out_args || !out_count) return false;

    args_vec_t vec = {0};
    bool stop_opts = false; // после "--" все — файлы

    for (int i = 1; i < argc; ++i) {
        const char* arg = argv[i];
        if (!arg) continue;

        if (!stop_opts && arg[0] == '-' && arg[1] != '\0') {
            // "--" — конец опций
            if (strcmp(arg, "--") == 0) {
                stop_opts = true;
                continue;
            }

            // Длинные опции
            const char* val = NULL;

            // --out[=val] / -o val
            if (match_long_opt(arg, "out", &val)) {
                if (!val) {
                    if (i + 1 >= argc) { fprintf(stderr, "error: --out requires a value\n"); goto fail; }
                    val = argv[++i];
                }
                sln_input_arg_t a = { .type = SLN_IN_ARG_TYPE_OUTP, .cstr = sln_strdup(val) };
                if (!a.cstr || !vec_push(&vec, &a)) { free_one(&a); goto oom; }
                continue;
            }

            // --link[=val] / -l val
            if (match_long_opt(arg, "link", &val)) {
                if (!val) {
                    if (i + 1 >= argc) { fprintf(stderr, "error: --link requires a value\n"); goto fail; }
                    val = argv[++i];
                }
                sln_input_arg_t a = { .type = SLN_IN_ARG_TYPE_LINK, .cstr = sln_strdup(val) };
                if (!a.cstr || !vec_push(&vec, &a)) { free_one(&a); goto oom; }
                continue;
            }

            // --code[=val]
            if (match_long_opt(arg, "code", &val)) {
                if (!val) {
                    if (i + 1 >= argc) { fprintf(stderr, "error: --code requires a value\n"); goto fail; }
                    val = argv[++i];
                }
                sln_input_arg_t a = { .type = SLN_IN_ARG_TYPE_CODE, .cstr = sln_strdup(val) };
                if (!a.cstr || !vec_push(&vec, &a)) { free_one(&a); goto oom; }
                continue;
            }

            // --warn[=all|extra]
            if (match_long_opt(arg, "warn", &val)) {
                if (!val) {
                    if (i + 1 >= argc) { fprintf(stderr, "error: --warn requires a value (all|extra)\n"); goto fail; }
                    val = argv[++i];
                }
                sln_input_arg_warn_t w;
                if (!parse_warn_value(val, &w)) {
                    fprintf(stderr, "error: unknown --warn value '%s' (expected all|extra)\n", val);
                    goto fail;
                }
                sln_input_arg_t a = { .type = SLN_IN_ARG_TYPE_WARN, .cstr = NULL };
                a.subtype.warn = w;
                if (!vec_push(&vec, &a)) goto oom;
                continue;
            }

            // --func[=custom]
            if (match_long_opt(arg, "func", &val)) {
                if (!val) {
                    if (i + 1 >= argc) { fprintf(stderr, "error: --func requires a value (custom)\n"); goto fail; }
                    val = argv[++i];
                }
                sln_input_arg_func_t f;
                if (!parse_func_value(val, &f)) {
                    fprintf(stderr, "error: unknown --func value '%s'\n", val);
                    goto fail;
                }
                sln_input_arg_t a = { .type = SLN_IN_ARG_TYPE_FUNC, .cstr = NULL };
                a.subtype.func = f;
                if (!vec_push(&vec, &a)) goto oom;
                continue;
            }

            // Короткие опции (простые, без кластеризации -xyz)
            if (arg[0] == '-' && arg[1] != '-' && arg[2] == '\0') {
                char k = arg[1];
                switch (k) {
                    case 'o': {
                        if (i + 1 >= argc) { fprintf(stderr, "error: -o requires a value\n"); goto fail; }
                        const char* v = argv[++i];
                        sln_input_arg_t a = { .type = SLN_IN_ARG_TYPE_OUTP, .cstr = sln_strdup(v) };
                        if (!a.cstr || !vec_push(&vec, &a)) { free_one(&a); goto oom; }
                        break;
                    }
                    case 'l': {
                        if (i + 1 >= argc) { fprintf(stderr, "error: -l requires a value\n"); goto fail; }
                        const char* v = argv[++i];
                        sln_input_arg_t a = { .type = SLN_IN_ARG_TYPE_LINK, .cstr = sln_strdup(v) };
                        if (!a.cstr || !vec_push(&vec, &a)) { free_one(&a); goto oom; }
                        break;
                    }
                    default:
                        fprintf(stderr, "error: unknown option '%s'\n", arg);
                        goto fail;
                }
                continue;
            }

            // Если дошли сюда — это что-то вроде "--foo" неизвестное
            if (strncmp(arg, "--", 2) == 0) {
                fprintf(stderr, "error: unknown option '%s'\n", arg);
                goto fail;
            }

            // fallback: позиционный
        }

        // Позиционный аргумент (или после "--")
        {
            sln_input_arg_t a = { .type = SLN_IN_ARG_TYPE_FILE, .cstr = sln_strdup(arg) };
            if (!a.cstr || !vec_push(&vec, &a)) { free_one(&a); goto oom; }
        }
    }

    // Успех
    *out_args = vec.data;
    *out_count = vec.count;
    return true;

oom:
    fprintf(stderr, "error: out of memory while parsing arguments\n");
fail:
    for (size_t j = 0; j < vec.count; ++j) free_one(&vec.data[j]);
    free(vec.data);
    return false;
}

sln_input_arg_t* input_args_get(int argc, char* argv[], size_t* out_count) {
    if (!out_count) return NULL;
    sln_input_arg_t* args = NULL;
    size_t count = 0;
    if (!input_args_parse(argc, argv, &args, &count)) return NULL;
    *out_count = count;
    return args;
}

void input_args_free(sln_input_arg_t* args, size_t count) {
    if (!args) return;
    for (size_t i = 0; i < count; ++i) free_one(&args[i]);
    free(args);
}
