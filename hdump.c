#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lk/argsparser/argsparser.h"

static lk_parser_t parser;
static int row_count_padding = 8;

void print_help()
{
    lk_parser_print_help(&parser, "hdump");
}

static int columns = 16;

void set_columns(void* str)
{
    columns = atoi((char*)str);
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

    lk_parser_option_t test_option;
    lk_parser_option_init_args(&test_option,
        'c',
        "columns",
        "the number of bytes per row (columns), default 16",
        set_columns);
    res = lk_parser_add_option(&parser, &test_option);
    assert(res == LK_OK);

    int consumed = lk_parser_parse(&parser, argc, argv);

    // get the last argument

    if (argc - 1 > consumed) {
        const char* filename = argv[argc - 1];
        FILE* f = fopen(filename, "rb");
        if (!f) {
            printf("Error: file \"%s\" could not be opened: %s\n", filename, strerror(errno));
            return errno;
        }
        uint8_t* buf = (uint8_t*)calloc(columns, sizeof(uint8_t));
        assert(buf);
        bool end = false;
        size_t rows = 0;
        while (!end) {
            memset(buf, 0, columns * sizeof(uint8_t));
            size_t n = fread(buf, sizeof(uint8_t), columns, f);
            if (n != (size_t)columns) {
                end = true;
            }
            printf("%0*x  ", row_count_padding, rows * columns);
            for (size_t i = 0; i < n; ++i) {
                printf("%02x ", buf[i]);
                if (i != 0 && (i + 1) % 4 == 0) {
                    printf(" ");
                }
                if (i != 0 && (i + 1) % 8 == 0) {
                    printf(" ");
                }
            }
            printf("\n");
            ++rows;
        }
        free(buf);
        fclose(f);
    } else {
        printf("Error: no file name specified. file name should be last argument.\n");
        return -1;
    }
}
