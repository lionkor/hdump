#include "argsparser.h"

#include "../ansi_format.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LK_RESULT lk_parser_init(lk_parser_t* parser)
{
    if (!parser) {
        return LK_ERROR;
    }
    parser->options = NULL;
    parser->options_size = 0;
    return LK_OK;
}

LK_RESULT lk_parser_add_option(lk_parser_t* parser, lk_parser_option_t* option)
{
    if (!parser || !option) {
        return LK_ERROR;
    }
    if (!parser->options) {
        // parser doesnt have any options yet, so we init the array and add one
        parser->options = (lk_parser_option_t**)malloc(sizeof(void*));
        if (!parser->options) {
            return LK_MALLOC_FAILED;
        }
        // copy the pointer to the option, note that we never hold on to any objects
        parser->options[0] = option;
        parser->options_size += 1;
    } else {
        parser->options_size += 1;
        size_t size_bytes = sizeof(void*) * parser->options_size;
        // put realloced ptr into a temporary variable, since realloc can fail.
        lk_parser_option_t** new_options = (lk_parser_option_t**)realloc(parser->options, size_bytes);
        if (!new_options) {
            return LK_REALLOC_FAILED;
        }
        // realloc went well, so we reassign
        parser->options = new_options;
        // set last element to the new element
        parser->options[parser->options_size - 1] = option;
    }
    return LK_OK;
}

// returns options_size on "not found"
static size_t find_index_of_long_arg(const lk_parser_t* parser, const char* str)
{
    assert(parser);
    assert(str);
    assert(parser->options);
    for (size_t i = 0; i < parser->options_size; ++i) {
        assert(parser->options[i]);
        assert(parser->options[i]->multi);
        if (strcmp(parser->options[i]->multi, str) == 0) {
            return i;
        } else {
            printf("INFO: long arg _%s_ != _%s_\n", parser->options[i]->multi, str);
        }
    }
    return parser->options_size;
}

// returns options_size on "not found"
static size_t find_index_of_short_arg(const lk_parser_t* parser, const char c)
{
    assert(parser);
    assert(parser->options);
    for (size_t i = 0; i < parser->options_size; ++i) {
        assert(parser->options[i]);
        assert(parser->options[i]->single);
        if (parser->options[i]->single == c) {
            return i;
        } else {
            printf("INFO: short arg _%c_ != _%c_\n", parser->options[i]->single, c);
        }
    }
    return parser->options_size;
}

static bool is_long_arg(const char* str)
{
    return strlen(str) > 2
        && str[0] == '-'
        && str[1] == '-'
        && str[2] != '-';
}

static bool is_short_arg(const char* str)
{
    return strlen(str) == 2
        && str[0] == '-'
        && str[1] != '-';
}

static bool is_combined_short_arg(const char* str)
{
    return strlen(str) > 2
        && str[0] == '-'
        && str[1] != '-';
}

static bool handle_long_arg(const lk_parser_t* parser, const char* str, size_t* index)
{
    const char* substr = str + 2;
    *index = find_index_of_long_arg(parser, substr);
    if (*index == parser->options_size) {
        // not found
        return false;
    } else {
        // found
        return true;
    }
}

void lk_parser_parse(const lk_parser_t* parser, int argc, char** argv)
{
    assert(parser);
    assert(parser->options);

    size_t index;
    bool found = false;

    if (argc == 1) {
    } else {
        for (int i = 1; i < argc; ++i) {
            if (argv[i][0] != '-') {
                // definitely not an argument, just skip
                continue;
            } else if (is_long_arg(argv[i])) {
                if (handle_long_arg(parser, argv[i], &index)) {
                    found = true;
                } else {
                    // not found
                    continue;
                }
            } else if (is_short_arg(argv[i])) {
                index = find_index_of_short_arg(parser, argv[i][1]);
                if (index == parser->options_size) {
                    // not found
                    continue;
                } else {
                    // found
                    found = true;
                }
            } else if (is_combined_short_arg(argv[i])) {
                printf("ERROR: combined args not yet supported.\n");
                // not found
                continue;
            } else {
                printf("ERROR: unknown arg \"%s\".\n", argv[i]);
                // not found
                continue;
            }

            // execute if it worked
            if (parser->options[index]->kind == LK_KIND_EXPECTS_ARG) {
                if (parser->options[index]->args_callback) {
                    assert(argc < i + 1);
                    parser->options[index]->args_callback(argv[i + 1]);
                }
            } else {
                if (parser->options[index]->no_args_callback) {
                    parser->options[index]->no_args_callback();
                }
            }
        }
    }

    if (!found) {
        lk_parser_print_help(parser, argv[0]);
    } else {
    }
}

void lk_parser_print_help(const lk_parser_t* parser, const char* name)
{
    assert(name);
    printf("%sUSAGE%s\n\t%s%s%s [OPTIONS]\n", LK_FMT_BOLD, LK_FMT_OFF, LK_FMT_BOLD, name, LK_FMT_OFF);
    printf(LK_FMT_BOLD "OPTIONS\n" LK_FMT_OFF);
    for (size_t i = 0; i < parser->options_size; ++i) {
        assert(parser->options[i]);
        assert(parser->options[i]->multi);
        assert(parser->options[i]->explanation);
        printf(LK_FMT_BOLD "\t-%c  \t--%s" LK_FMT_OFF "  \t%s\n",
            parser->options[i]->single,
            parser->options[i]->multi,
            parser->options[i]->explanation);
    }
}

LK_RESULT lk_parser_option_init_no_args(lk_parser_option_t* option, const char single, const char* multi, const char* explanation, lk_parser_void_fn_void_t callback)
{
    if (!option || !multi || !explanation) {
        return LK_ERROR;
    }
    memset(option, 0, sizeof(*option));
    option->single = single;
    option->multi = multi;
    option->explanation = explanation;
    option->no_args_callback = callback;
    return LK_OK;
}
