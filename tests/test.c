#include "tests.h"


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_vector_basic),
        cmocka_unit_test(test_table_basic)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
