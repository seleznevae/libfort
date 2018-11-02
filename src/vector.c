#include "vector.h"
#include <assert.h>
#include <string.h>

/*****************************************************************************
 *               VECTOR IMPLEMENTATIONS
 * ***************************************************************************/

struct vector {
    size_t m_size;
    void  *m_data;
    size_t m_capacity;
    size_t m_item_size;
};

static int vector_reallocate_(vector_t *vector, size_t new_capacity)
{
    assert(vector);
    assert(new_capacity > vector->m_capacity);

    size_t new_size = new_capacity * vector->m_item_size;
    vector->m_data = F_REALLOC(vector->m_data, new_size);
    if (vector->m_data == NULL)
        return -1;
    return 0;
}

/* ------------ Constructors & Destructors ----------------------------- */
FT_INTERNAL
vector_t *create_vector(size_t item_size, size_t capacity)
{
    vector_t *vector = (vector_t *)F_MALLOC(sizeof(vector_t));
    if (vector == NULL) {
        SYS_LOG_ERROR("Failed to allocate memory for asock vector");
        return NULL;
    }

    size_t init_size = MAX(item_size * capacity, 1);
    vector->m_data = F_MALLOC(init_size);
    if (vector->m_data == NULL) {
        SYS_LOG_ERROR("Failed to allocate memory for asock vector inern. buffer");
        F_FREE(vector);
        return NULL;
    }

    vector->m_size      = 0;
    vector->m_capacity  = capacity;
    vector->m_item_size = item_size;

    return vector;
}


FT_INTERNAL
void destroy_vector(vector_t *vector)
{
    assert(vector);
    F_FREE(vector->m_data);
    F_FREE(vector);
}


/*
FT_INTERNAL

*/


/* ----------- Nonmodifying functions --------------------------------- */
FT_INTERNAL
size_t vector_size(const vector_t *vector)
{
    assert(vector);
    return vector->m_size;
}


FT_INTERNAL
size_t vector_capacity(const vector_t *vector)
{
    assert(vector);
    return vector->m_capacity;
}

/* ----------- Modifying functions ------------------------------------- */
FT_INTERNAL
int vector_push(vector_t *vector, const void *item)
{
    assert(vector);
    assert(item);

    if (vector->m_size == vector->m_capacity) {
        if (vector_reallocate_(vector, vector->m_capacity * 2) == -1)
            return FT_ERROR;
        vector->m_capacity = vector->m_capacity * 2;
    }

    ptrdiff_t deviation = vector->m_size * vector->m_item_size;
    memcpy((char *)vector->m_data + deviation, item, vector->m_item_size);

    ++(vector->m_size);

    return FT_SUCCESS;
}


FT_INTERNAL
const void *vector_at_c(const vector_t *vector, size_t index)
{
    if (index >= vector->m_size)
        return NULL;

    return (char *)vector->m_data + index * vector->m_item_size;
}


FT_INTERNAL
void *vector_at(vector_t *vector, size_t index)
{
    if (index >= vector->m_size)
        return NULL;

    return (char *)vector->m_data + index * vector->m_item_size;
}


FT_INTERNAL
fort_status_t vector_swap(vector_t *cur_vec, vector_t *mv_vec, size_t pos)
{
    assert(cur_vec);
    assert(mv_vec);
    assert(cur_vec != mv_vec);
    assert(cur_vec->m_item_size == mv_vec->m_item_size);

    size_t cur_sz = vector_size(cur_vec);
    size_t mv_sz = vector_size(mv_vec);
    if (mv_sz == 0) {
        return FT_SUCCESS;
    }

    size_t min_targ_size = pos + mv_sz;
    if (vector_capacity(cur_vec) < min_targ_size) {
        if (vector_reallocate_(cur_vec, min_targ_size) == -1)
            return FT_ERROR;
        cur_vec->m_capacity = min_targ_size;
    }

    ptrdiff_t deviation = pos * cur_vec->m_item_size;
    void *tmp = NULL;
    size_t new_mv_sz = 0;
    if (cur_sz > pos) {
        new_mv_sz = MIN(cur_sz - pos, mv_sz);
        tmp = F_MALLOC(cur_vec->m_item_size * new_mv_sz);
        if (tmp == NULL) {
            return FT_MEMORY_ERROR;
        }
    }

    if (tmp) {
        memcpy(tmp,
               (char *)cur_vec->m_data + deviation,
               cur_vec->m_item_size * new_mv_sz);
    }

    memcpy((char *)cur_vec->m_data + deviation,
           mv_vec->m_data,
           cur_vec->m_item_size * mv_sz);

    if (tmp) {
        memcpy(mv_vec->m_data,
               tmp,
               cur_vec->m_item_size * new_mv_sz);
    }

    cur_vec->m_size = MAX(cur_vec->m_size, min_targ_size);
    mv_vec->m_size = new_mv_sz;
    F_FREE(tmp);
    return FT_SUCCESS;
}

vector_t *copy_vector(vector_t *v)
{
    if (v == NULL)
        return NULL;

    vector_t *new_vector = create_vector(v->m_item_size, v->m_capacity);
    if (new_vector == NULL)
        return NULL;

    memcpy(new_vector->m_data, v->m_data, v->m_item_size * v->m_size);
    new_vector->m_size      = v->m_size ;
    new_vector->m_item_size = v->m_item_size ;
    return new_vector;
}

#ifdef FT_TEST_BUILD

size_t vector_index_of(const vector_t *vector, const void *item)
{
    assert(vector);
    assert(item);

    size_t i = 0;
    for (i = 0; i < vector->m_size; ++i) {
        void *data_pos = (char *)vector->m_data + i * vector->m_item_size;
        if (memcmp(data_pos, item, vector->m_item_size) == 0) {
            return i;
        }
    }
    return INVALID_VEC_INDEX;
}


int vector_erase(vector_t *vector, size_t index)
{
    assert(vector);

    if (vector->m_size == 0 || index >= vector->m_size)
        return FT_ERROR;

    memmove((char *)vector->m_data + vector->m_item_size * index,
            (char *)vector->m_data + vector->m_item_size * (index + 1),
            (vector->m_size - 1 - index) * vector->m_item_size);
    vector->m_size--;
    return FT_SUCCESS;
}


void vector_clear(vector_t *vector)
{
    vector->m_size = 0;
}
#endif
