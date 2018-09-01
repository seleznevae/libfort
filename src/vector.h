#ifndef VECTOR_H
#define VECTOR_H

#include "fort_utils.h"


#define INVALID_VEC_INDEX ((size_t) -1)

FT_INTERNAL
vector_t *create_vector(size_t item_size, size_t capacity);

FT_INTERNAL
void destroy_vector(vector_t *);

FT_INTERNAL
size_t vector_size(const vector_t *);

FT_INTERNAL
size_t vector_capacity(const vector_t *);

FT_INTERNAL
int vector_push(vector_t *, const void *item);

FT_INTERNAL
const void *vector_at_c(const vector_t *vector, size_t index);

FT_INTERNAL
void *vector_at(vector_t *, size_t index);

FT_INTERNAL
fort_status_t vector_swap(vector_t *cur_vec, vector_t *mv_vec, size_t pos);


#define FOR_EACH_(type, item, vector, index_name) \
    size_t index_name = 0; \
    for (index_name = 0; (index_name < vector_size(vector)) ? ((item = *(type*)vector_at(vector, index_name)), 1) : 0;  ++index_name)

#define FOR_EACH(type, item, vector) \
    FOR_EACH_(type, item, vector, UNIQUE_NAME(i))

#ifdef FT_TEST_BUILD
vector_t *copy_vector(vector_t *);
size_t vector_index_of(const vector_t *, const void *item);
int vector_erase(vector_t *, size_t index);
void vector_clear(vector_t *);
#endif

#endif /* VECTOR_H */
