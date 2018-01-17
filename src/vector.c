#include "vector.h"
#include <assert.h>

/*****************************************************************************
 *               VECTOR IMPLEMENTATIONS
 * ***************************************************************************/

struct vector
{
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
    vector->m_data = realloc(vector->m_data, new_size);
    if (vector->m_data == NULL)
        return -1;
    return 0;
}

/* ------------ Constructors & Destructors ----------------------------- */

vector_t* create_vector(size_t item_size, size_t capacity)
{
    vector_t *vector = malloc(sizeof(vector_t));
    if (vector == NULL) {
        SYS_LOG_ERROR("Failed to allocate memory for asock vector");
        return NULL;
    }

    size_t init_size = MAX(item_size * capacity, 1);
    vector->m_data = malloc(init_size);
    if (vector->m_data == NULL) {
        SYS_LOG_ERROR("Failed to allocate memory for asock vector inern. buffer");
        free(vector);
        return NULL;
    }

    vector->m_size      = 0;
    vector->m_capacity  = capacity;
    vector->m_item_size = item_size;

    return vector;
}


void destroy_vector(vector_t* vector)
{
    assert(vector);
    free(vector->m_data);
    free(vector);
}


/* ----------- Nonmodifying functions --------------------------------- */

size_t vector_size(const vector_t* vector)
{
    assert(vector);
    return vector->m_size;
}


size_t vector_capacity(const vector_t* vector)
{
    assert(vector);
    return vector->m_capacity;
}

size_t vector_index_of(const vector_t* vector, const void *item)
{
    assert(vector);
    assert(item);

    for (size_t i = 0; i < vector->m_size; ++i) {
        void *data_pos = vector->m_data + i * vector->m_item_size;
        if (memcmp(data_pos, item, vector->m_item_size) == 0) {
            return i;
        }
    }
    return INVALID_VEC_INDEX;
}


/* ----------- Modifying functions ------------------------------------- */

int vector_push (vector_t* vector, const void* item)
{
    assert(vector);
    assert(item);

    if (vector->m_size == vector->m_capacity) {
        if (vector_reallocate_(vector, vector->m_capacity * 2) == -1)
            return F_ERROR;
        vector->m_capacity = vector->m_capacity * 2;
    }

    ptrdiff_t deviation = vector->m_size * vector->m_item_size;
    memcpy(vector->m_data + deviation, item, vector->m_item_size);

    ++(vector->m_size);

    return F_SUCCESS;
}


int vector_erase(vector_t *vector, size_t index)
{
    assert(vector);

    if (vector->m_size == 0 || index >= vector->m_size)
        return F_ERROR;

    memmove(vector->m_data + vector->m_item_size * index,
            vector->m_data + vector->m_item_size * (index + 1),
            (vector->m_size - 1 - index) * vector->m_item_size);
    vector->m_size--;
    return F_SUCCESS;
}


void vector_clear(vector_t *vector)
{
    vector->m_size = 0;
}



void *vector_at(vector_t *vector, size_t index)
{
    if (index >= vector->m_size)
        return NULL;

    return vector->m_data + index * vector->m_item_size;
}


