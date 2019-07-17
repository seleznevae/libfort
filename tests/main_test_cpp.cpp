#include "tests.h"
#include <stdio.h>
#include "fort.h"

/* Test cases */
void test_cpp_table_basic(void);
void test_cpp_table_write(void);
void test_cpp_table_tbl_properties(void);
void test_cpp_table_cell_properties(void);
void test_cpp_table_text_styles(void);



struct test_case bb_test_suite [] = {
    {"test_cpp_table_basic", test_cpp_table_basic},
    {"test_cpp_table_write", test_cpp_table_write},
    {"test_cpp_table_tbl_properties", test_cpp_table_tbl_properties},
    {"test_cpp_table_cell_properties", test_cpp_table_cell_properties},
    {"test_cpp_table_text_styles", test_cpp_table_text_styles},
};


int run_bb_test_suite(void)
{
    int bb_n_tests = sizeof(bb_test_suite) / sizeof(bb_test_suite[0]);
    run_test_suite("BLACK BOX TEST SUITE", bb_n_tests, bb_test_suite);
    return 0;
}

int main(void)
{
    int status = 0;

    status |= run_bb_test_suite();

    return status;
}
