#include "stringops.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void lk_sanitize_ascii(char* buffer, size_t size)
{
    assert(buffer);
    for (size_t i = 0; buffer[i] && i < size; ++i) {
        if (iscntrl(buffer[i])) {
            buffer[i] = ' ';
        }
    }
}
