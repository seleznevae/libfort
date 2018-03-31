#include "tests.h"
#include <stdio.h>

struct test_case test_suit [] = {
    {"test_vector_basic", test_vector_basic},
    {"test_string_buffer", test_string_buffer},
    {"test_table_sizes", test_table_sizes},
    {"test_table_geometry", test_table_geometry},
    {"test_table_basic", test_table_basic},
    {"test_wcs_table_boundaries", test_wcs_table_boundaries},
    {"test_table_write", test_table_write},
    {"test_table_border_style", test_table_border_style},
    {"test_table_cell_options", test_table_cell_options},
    {"test_table_tbl_options", test_table_tbl_options},
};

int main(void)
{
    int n_tests = sizeof(test_suit) / sizeof(test_suit[0]);
    fprintf(stderr, "[==========] Running %d test(s).\n", n_tests);
    int i;
    for (i = 0; i < n_tests; ++i) {
        fprintf(stderr, "[ RUN      ] %s\n", test_suit[i].name);
        test_suit[i].test();
        fprintf(stderr, "[       OK ] %s\n", test_suit[i].name);
    }
    fprintf(stderr, "[==========] %d test(s) run.\n", n_tests);
    fprintf(stderr, "[  PASSED  ] %d test(s).\n", n_tests);
    return 0;
}
