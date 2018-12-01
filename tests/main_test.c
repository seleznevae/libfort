#include "tests.h"
#include <stdio.h>
#include "fort.h"

/* Test cases */
#ifdef FT_HAVE_WCHAR
void test_bug_fixes(void);
#endif
void test_vector_basic(void);
void test_vector_stress(void);
void test_string_buffer(void);
void test_table_sizes(void);
void test_table_geometry(void);
void test_table_basic(void);
void test_table_copy(void);
void test_table_changing_cell(void);
#ifdef FT_HAVE_WCHAR
void test_wcs_table_boundaries(void);
#endif
void test_table_write(void);
void test_table_border_style(void);
void test_table_builtin_border_styles(void);
void test_table_cell_properties(void);
void test_table_text_styles(void);
void test_table_tbl_properties(void);
void test_memory_errors(void);


#ifdef FORT_WB_TESTING_ENABLED
struct test_case wb_test_suit [] = {
    {"test_vector_basic", test_vector_basic},
    {"test_vector_stress", test_vector_stress},
    {"test_string_buffer", test_string_buffer},
    {"test_table_sizes", test_table_sizes},
    {"test_table_geometry", test_table_geometry},
    {"test_table_copy", test_table_copy},
};
#endif


struct test_case bb_test_suit [] = {
#ifdef FT_HAVE_WCHAR
    {"test_bug_fixes", test_bug_fixes},
#endif
    {"test_table_basic", test_table_basic},
#ifdef FT_HAVE_WCHAR
    {"test_wcs_table_boundaries", test_wcs_table_boundaries},
#endif
    {"test_table_write", test_table_write},
    {"test_table_changing_cell", test_table_changing_cell},
    {"test_table_border_style", test_table_border_style},
    {"test_table_builtin_border_styles", test_table_builtin_border_styles},
    {"test_table_cell_properties", test_table_cell_properties},
    {"test_table_tbl_properties", test_table_tbl_properties},
    {"test_table_text_styles", test_table_text_styles},
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

    /*
     * Essential for OSX, because swprintf can fail in case of real unicode
     * chars.
     */
#if defined(FT_HAVE_WCHAR)
    setlocale(LC_CTYPE, "");
#endif

#ifdef FORT_WB_TESTING_ENABLED
    status |= run_wb_test_suit();
    fprintf(stderr, "\n");
#endif
    status |= run_bb_test_suit();


    return status;
}
