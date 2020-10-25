#include <assert.h>
#include <stdio.h>

#include "lk/argsparser/argsparser.h"

static lk_parser_t parser;

void print_help()
{
    lk_parser_print_help(&parser, "hdump");
}

int main(int argc, char** argv)
{
    LK_RESULT res = lk_parser_init(&parser);
    assert(res == LK_OK);

    lk_parser_option_t help_option;
    lk_parser_option_init_no_args(&help_option,
        'h',
        "help",
        "displays this help",
        print_help);
    res = lk_parser_add_option(&parser, &help_option);
    assert(res == LK_OK);

    lk_parser_option_t dummy_option;
    lk_parser_option_init_no_args(&dummy_option,
        'd',
        "dummy",
        "does nothing",
        NULL);
    res = lk_parser_add_option(&parser, &dummy_option);
    assert(res == LK_OK);

    lk_parser_parse(&parser, argc, argv);
}
