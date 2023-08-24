#ifndef VECTOR
#define VECTOR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// creates the body of the vector
// should be used in object files in conjuction with vec_dec
#define vec_gen(type, name) \
    typedef struct vec_##name { \
        type* data; \
        size_t capacity; \
        size_t len; \
    } vec_##name \

// declares a vector but dosent decide in types
// should be used in header files (spesifically vector_decs.h)
#define vec_dec(name) \
    typedef struct vec_##name vec_##name  \

// creates a new vec and returns it
// callocs vec.data so you wont have access to memory youre not supposed to have
// panics:
//  size > 0 && vec.data == NULL
#define vec_new(size, type) ({ \
    size_t _size = (size); \
    vec_##type vec; \
    vec.data = (type*) calloc(_size, sizeof(type)); \
    if (assert(_size > 0)) { \
        assert(vec.data != NULL); \
    } \
    vec.capacity = _size; \
    vec.len = 0; \
    vec; \
})

// adds a value at vec.len
// if vec.len == vec.capacity it will reallocate at a rate of * 2 + 1
// type checks that the data is the same as the vector data
// panics:
//  vec.len > 0 && vec.data == NULL
//  vec.data == NULL after reallocation
#define vec_push(vec, _data) { \
    if (vec.len > 0) { \
        assert(vec.data != NULL); \
    } \
    __typeof__(*(vec.data)) __data = (_data); \
    \
    if (vec.len == vec.capacity) { \
        vec.capacity *= 2; \
        vec.capacity += 1; \
        vec.data = realloc(vec.data, sizeof(__typeof__(__data)) * vec.capacity); \
        assert(vec.data != NULL); \
    } \
    vec.data[vec.len] = __data; \
    vec.len++; \
}

// removes from the vector length the amount that is specified
// dosent reallocate memory
// dosent delete memory so you can still access the data if you dont use provided functions
// panics:
//  vec.len < _amt
//  vec.data == NULL
#define vec_pop(vec, amt) { \
    assert(vec.data != NULL); \
    size_t _amt = (amt); \
    assert(vec.len >= _amt); \
    vec.len -= _amt; \
}

// reallocates the data in vector by specified amount
// will delete vector data if enough elements are removed
// deletion of data is not garaunteed
// panics:
//  vec.data == NULL
//  vec.len < _amt
//  vec.capacity > 0 && vec.data == NULL
#define vec_shrink(vec, amt) { \
    assert(vec.data != NULL); \
    size_t _amt = (amt); \
    assert(vec.len >= _amt); \
    vec.capacity -= _amt; \
    vec.data = realloc(vec.data, sizeof(__typeof__(vec.data[0])) * vec.capacity); \
    if (assert(vec.capacity > 0)) { \
        assert(vec.data != NULL); \
    } \
    if (vec.len > vec.capacity) { \
        vec.len = vec.capacity; \
    } \
}

// removes an element at a spesific index
// reorders elements so that there will be no holes in the array
// dosent reallocate data
// panics:
//  vec.data == NULL
//  vec.len < _index
#define vec_remove(vec, index) { \
    assert(vec.data != NULL); \
    size_t _index = (index); \
    assert(vec.len > _index); \
    for (size_t i = _index; i < vec.len; i++) { \
        vec.data[i] = vec.data[i+1]; \
    } \
    vec.len--; \
}

// gets the element from a spesific index and returns it
// bounds checking is the reason to use this over raw access to the data
// if you want to avoid null and bounds checking dont use this macro
// panics:
//  vec.data == NULL
//  vec.len < _index
#define vec_get(vec, index) ({ \
    assert(vec.data != NULL); \
    size_t _index = (index); \
    assert(vec.len > _index); \
    vec.data[index]; \
})

// sets the element as a spesific index
// bounds checking is the reason to use this over raw access to the data
// if you want to avoid null and bounds checking dont use this macro
// panics:
//  vec.data == NULL
//  vec.len < _index
#define vec_set(vec, index, val) { \
    assert(vec.data != NULL); \
    size_t _index = (index); \
    assert(vec.len > _index); \
    __typeof__(*(vec.data)) _val = val; \
    vec.data[index] = _val; \
}

// currently only frees and sets the data to NULL
// currently this macro is very simple but if more stuff is needed to do when destroying a vector it can be added in this macro
#define vec_destroy(vec) { \
    free(vec.data); \
    vec.data = NULL; \
}

#endif
