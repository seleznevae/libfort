#ifndef VECTOR_H
#define VECTOR_H

#include "fort_utils.h"


/*****************************************************************************
 *               VECTOR
 * ***************************************************************************/

struct vector;
typedef struct vector vector_t;

#define INVALID_VEC_INDEX ((size_t) -1)

extern vector_t* create_vector(size_t item_size, size_t capacity);
extern void destroy_vector(vector_t*);
extern vector_t* copy_vector(vector_t*);

extern size_t vector_size(const vector_t*);
extern size_t vector_capacity(const vector_t*);
extern size_t vector_index_of(const vector_t*, const void *item);

extern int vector_push(vector_t*, const void *item);
extern int vector_erase(vector_t*, size_t index);
extern void vector_clear(vector_t*);
extern const void *vector_at_c(const vector_t *vector, size_t index);
extern void* vector_at(vector_t*, size_t index);
extern fort_status_t vector_swap(vector_t *cur_vec, vector_t *mv_vec, size_t pos);


#define FOR_EACH_(type, item, vector, index_name) \
    size_t index_name = 0; \
    for (index_name = 0; (index_name < vector_size(vector)) ? ((item = *(type*)vector_at(vector, index_name)), 1) : 0;  ++index_name)

#define FOR_EACH(type, item, vector) \
    FOR_EACH_(type, item, vector, UNIQUE_NAME(i))

#endif /* VECTOR_H */
