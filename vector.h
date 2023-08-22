#ifndef VECTOR
#define VECTOR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define vec_gen(type)           \
    typedef struct vec_##type { \
        type* data;             \
        size_t capacity;        \
        size_t len;             \
    } vec_##type;               \

#define vec_dec(type)                     \ 
    typedef struct vec_##type vec_##type; \

#define vec_new(size, type) ({                                  \
    size_t _size = (size);                                      \
    vec_##type vec;                                             \
    vec.data = (type*) malloc(sizeof(type) * _size);            \
    if (vec.data == NULL) {                                     \
        printf("Error in vec_new: could not allocate data");    \
    }                                                           \
    vec.capacity = _size;                                       \
    vec.len = 0;                                                \
    vec;                                                        \
})

#define vec_push(vec, _data) ({                                                       \
    __typeof__(vec.data[0]) __data = (_data);                                         \
    if (vec.len == vec.capacity) {                                                    \
        vec.capacity *= 2;                                                            \
        vec.data = realloc(vec.data, sizeof(__typeof__(vec.data[0])) * vec.capacity); \
        if (vec.data == NULL) {                                                       \
            printf("Error in vec_push: could not reallocate data");                   \
        }                                                                             \
    }                                                                                 \
    vec.data[vec.len] = __data;                                                       \
    vec.len++;                                                                        \
})

#define vec_pop(vec, amt) ({ \
    size_t _amt = (amt);     \
    vec.len -= _amt;         \
})

#define vec_shrink(vec, amt) ({                                                     \
    size_t _amt = (amt);                                                            \
    vec.capacity -= _amt;                                                           \
    vec.data = realloc(vec.data, sizeof(__typeof__(vec.data[0])) * vec.capacity);   \
    if (vec.data == NULL) {                                                         \
        printf("Error in vec_shrink: could not reallocate data");                   \
        exit(EXIT_FAILURE);                                                         \
    }                                                                               \
    if (vec.len > vec.capacity) {                                                   \
        vec.len = vec.capacity;                                                     \
    }                                                                               \
})

#define vec_remove(vec, index) ({                                          \
    size_t _index = (index);                                               \
    if (_index > vec.len) {                                                \
        printf("Error in vec_remove: index is bigger then vector length"); \
        exit(EXIT_FAILURE);                                                \
    }                                                                      \
    for (size_t i = _index; i < vec.len; i++) {                            \
        vec.data[i] = vec.data[i+1];                                       \
    }                                                                      \
    vec.len--;                                                             \
})

#define vec_destroy(vec) ({ \
    free(vec.data);         \
    vec.data = NULL;        \
})

#endif