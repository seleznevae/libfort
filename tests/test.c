#include "tests.h"


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_vector_basic),
        cmocka_unit_test(test_table_sizes),
        cmocka_unit_test(test_table_geometry),
        cmocka_unit_test(test_table_basic),
        cmocka_unit_test(test_table_options),
        cmocka_unit_test(test_string_buffer),

    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
