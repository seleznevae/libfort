#include "tests.h"
#include <stdio.h>
#include "fort.h"

void run_test_suit(const char *test_suit_name, int n_tests, struct test_case test_suit[])
{
    fprintf(stderr, " ==  RUNNING %s ==\n", test_suit_name);
    fprintf(stderr, "[==========] Running %d test(s).\n", n_tests);
    int i;
    for (i = 0; i < n_tests; ++i) {
        fprintf(stderr, "[ RUN      ] %s\n", test_suit[i].name);
        test_suit[i].test();
        fprintf(stderr, "[       OK ] %s\n", test_suit[i].name);
    }
    fprintf(stderr, "[==========] %d test(s) run.\n", n_tests);
    fprintf(stderr, "[  PASSED  ] %d test(s).\n", n_tests);
}

#ifdef FORT_WB_TESTING_ENABLED
struct test_case wb_test_suit [] = {
    {"test_vector_basic", test_vector_basic},
    {"test_vector_stress", test_vector_stress},
    {"test_string_buffer", test_string_buffer},
    {"test_table_sizes", test_table_sizes},
    {"test_table_geometry", test_table_geometry},
};
#endif


struct test_case bb_test_suit [] = {
    {"test_table_basic", test_table_basic},
#ifdef FT_HAVE_WCHAR
    {"test_wcs_table_boundaries", test_wcs_table_boundaries},
#endif
    {"test_table_write", test_table_write},
    {"test_table_border_style", test_table_border_style},
    {"test_table_cell_options", test_table_cell_options},
    {"test_table_tbl_options", test_table_tbl_options},
    {"test_memory_errors", test_memory_errors},
};

#ifdef FORT_WB_TESTING_ENABLED
int run_wb_test_suit(void)
{
    int wb_n_tests = sizeof(wb_test_suit) / sizeof(wb_test_suit[0]);
    run_test_suit("WHITE BOX TEST SUITE", wb_n_tests, wb_test_suit);
    return 0;
}
#endif


int run_bb_test_suit(void)
{
    int bb_n_tests = sizeof(bb_test_suit) / sizeof(bb_test_suit[0]);
    run_test_suit("BLACK BOX TEST SUITE", bb_n_tests, bb_test_suit);
    return 0;
}

int main(void)
{
    int status = 0;

#ifdef FORT_WB_TESTING_ENABLED
    status |= run_wb_test_suit();
    fprintf(stderr, "\n");
#endif
    status |= run_bb_test_suit();

    return status;
}
