#ifndef LK_RESULT_H
#define LK_RESULT_H

enum lk_result_t {
    LK_OK = 0,
    LK_ERROR,
    LK_MALLOC_FAILED,
    LK_REALLOC_FAILED,
    LK_CALLOC_FAILED,
};

#define LK_RESULT enum lk_result_t

#endif // LK_RESULT_H
