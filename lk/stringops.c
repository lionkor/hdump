#include "stringops.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void lk_sanitize_ascii(char* buffer, size_t size)
{
    assert(buffer);
    for (size_t i = 0; i < size; ++i) {
        if (buffer[i] == 0 || !isprint(buffer[i])) {
            buffer[i] = '.';
        } else {
        }
    }
}
