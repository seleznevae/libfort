#include "tests.h"

#include "vector.h"
//#include "../src/fort.c"


void test_vector_basic(void **state)
{
    (void)state;

    typedef short item_t;
    const size_t init_capacity = 10;
    vector_t *vector = create_vector(sizeof(item_t), init_capacity);

    assert_true( vector != NULL );
    assert_true( vector_size(vector)     == 0 );
    assert_true( vector_capacity(vector) == init_capacity );

    WHEN("Pushing less items than initial capacity") {
        for (size_t i = 0; i < init_capacity; ++i) {
            item_t item = i;
            vector_push(vector, &item);
        }

        THEN("Then capacity is not changed") {
            assert_true( vector_size(vector)     == init_capacity );
            assert_true( vector_capacity(vector) == init_capacity );
        }
    }

    WHEN("Clearing vector") {
        vector_clear(vector);

        THEN("Then size == 0") {
            assert_true( vector_size(vector) == 0 );
        }
    }

    WHEN("Pushing more items than initial capacity") {
        for (size_t i = 0; i < 2 * init_capacity; ++i) {
            item_t item = 2 * i;
            vector_push(vector, &item);
        }

        THEN("Then capacity is increased") {
            assert_true( vector_size(vector)     == 2 * init_capacity );
            assert_true( vector_capacity(vector)  > init_capacity );
        }

        WHEN("Checking indexes of items") {
            item_t item =  6;
            assert_true( vector_index_of(vector, &item) == 3 );
            item =  14;
            assert_true( vector_index_of(vector, &item) == 7 );
            item =  25;
            assert_true( vector_index_of(vector, &item) == INVALID_VEC_INDEX );
        }

        WHEN("Checking access to items") {
            assert_true( *(item_t *)vector_at(vector,  0) == 0 );
            assert_true( *(item_t *)vector_at(vector, 10) == 20 );
            assert_true(  (item_t *)vector_at(vector, 20) == NULL );
        }

        WHEN("Erasing items") {
            assert_true( vector_erase(vector, 20) != F_SUCCESS );

            assert_true( vector_erase(vector,  0) == F_SUCCESS );
            assert_true( vector_erase(vector, 10) == F_SUCCESS );

            item_t first_item = *(item_t*)vector_at(vector, 0);
            assert_true( first_item == 2 );
            item_t item =  6;
            assert_true( vector_index_of(vector, &item) == 2 );
            item =  26;
            assert_true( vector_index_of(vector, &item) == 11 );
        }
    }

    WHEN("Clearing vector") {
        vector_clear(vector);

        THEN("Then size == 0") {
            assert_true( vector_size(vector) == 0 );
        }
    }

    destroy_vector( vector );
}
