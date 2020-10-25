#ifndef ARGSPARSER_H
#define ARGSPARSER_H

#include "../result.h"
#include <stddef.h>

typedef void (*lk_parser_void_fn_void_t)();
typedef void (*lk_parser_void_fn_voidptr_t)(void*);

typedef enum {
    LK_KIND_NO_ARG, // just a flag, no args
    LK_KIND_EXPECTS_ARG // flag with args, like "-o binary.bin"
} lk_parser_option_kind_t;

typedef struct {
    lk_parser_option_kind_t kind;
    char single;
    char* multi;
    char* explanation;
    lk_parser_void_fn_void_t no_args_callback;
    lk_parser_void_fn_voidptr_t args_callback;
} lk_parser_option_t;

typedef struct {
    lk_parser_option_t** options;
    size_t options_size;
} lk_parser_t;

typedef struct {
    size_t index;
} lk_parser_result_t;

// initializes the parser
LK_RESULT lk_parser_init(lk_parser_t* parser);
// adds an option to the parser - no sanity checks are made here
LK_RESULT lk_parser_add_option(lk_parser_t* parser, lk_parser_option_t* option);
// parses the arguments given to it. returns an index of parser->options_size on error.
// asserts parser being non-NULL.
void lk_parser_parse(const lk_parser_t* parser, int argc, char** argv);
// prints the help.
void lk_parser_print_help(const lk_parser_t* parser, const char* name);

LK_RESULT lk_parser_option_init_no_args(lk_parser_option_t* option, const char single, const char* multi, const char* explanation, lk_parser_void_fn_void_t callback);

#endif // ARGSPARSER_H
