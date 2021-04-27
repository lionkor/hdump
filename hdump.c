#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "lk/ansi_format.h"
#include "lk/argsparser/argsparser.h"
#include "lk/stringops.h"

static lk_parser_t parser;
static int row_count_padding = 8;
static int columns = 16;
static bool show_ascii = false;

void print_help()
{
    lk_parser_print_help(&parser, "hdump", "<file>");
    printf(LK_FMT_BOLD "COPYRIGHT\n" LK_FMT_OFF "\tCopyright (c) 2020 Lion Kortlepel. Licensed under GNU GPL v3.0.\n");
    exit(0);
}

void set_columns(void* str)
{
    columns = atoi((char*)str);
}

void enable_ascii()
{
    show_ascii = true;
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

    lk_parser_option_t columns_option;
    lk_parser_option_init_args(&columns_option,
        'c',
        "columns",
        "the number of bytes per row (columns), default 16",
        set_columns);
    res = lk_parser_add_option(&parser, &columns_option);
    assert(res == LK_OK);

    lk_parser_option_t ascii_option;
    lk_parser_option_init_no_args(&ascii_option,
        'a',
        "ascii",
        "enables ascii view to the right of the hex display",
        enable_ascii);
    res = lk_parser_add_option(&parser, &ascii_option);
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

        struct stat file_stat;
        stat(filename, &file_stat);
        size_t file_size = (size_t)file_stat.st_size;
        if (file_size == 0) {
            printf("\"%s\" is empty.\n", filename);
            return 0;
        }
        row_count_padding = (size_t)((log((double)file_size)) / log(16)) + 3;

        while (!end) {
            memset(buf, 0, columns * sizeof(uint8_t));
            size_t n = fread(buf, sizeof(uint8_t), columns, f);
            if (n != (size_t)columns) {
                end = true;
            }
            printf("%0*lx  ", row_count_padding, rows * columns);
            for (size_t i = 0; i < columns; ++i) {
                if (i >= n && n < columns) {
                    printf("   ");
                } else {
                    printf("%02x ", buf[i]);
                }
                if (i != 0 && (i + 1) % 4 == 0) {
                    printf(" ");
                }
                if (i != 0 && (i + 1) % 8 == 0) {
                    printf(" ");
                }
            }
            if (show_ascii) {
                lk_sanitize_ascii((char*)buf, columns);
                for (size_t i = 0; i < columns; ++i) {
                    putchar(buf[i]);
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
