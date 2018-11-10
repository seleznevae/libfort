#include "tests.h"
#include <stdio.h>
#include "fort.h"

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
    {"test_table_basic", test_table_basic},
#ifdef FT_HAVE_WCHAR
    {"test_wcs_table_boundaries", test_wcs_table_boundaries},
#endif
    {"test_table_write", test_table_write},
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
//#if defined(FT_HAVE_WCHAR)
//    setlocale(LC_CTYPE, "");
//#endif

//#ifdef FORT_WB_TESTING_ENABLED
//    status |= run_wb_test_suit();
//    fprintf(stderr, "\n");
//#endif
//    status |= run_bb_test_suit();


    setlocale(LC_CTYPE, "");

    ft_table_t *table = NULL;
    table = ft_create_table();
    ft_set_border_style(table, FT_DOUBLE2_STYLE);
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);

    ft_wwrite_ln(table, L"some", L"some3333", L"11111");
    ft_wwrite_ln(table, L"s", L"some333377777", L"1111111111");
    ft_wwrite_ln(table, L"some", L"some3333", L"111111");
    ft_wwrite_ln(table, L"✖", L"☑", L"✔");


    ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
    ft_set_cell_prop(table, 0, 1, FT_CPROP_CELL_TEXT_STYLE, FT_TSTYLE_UNDERLINED);
    ft_set_cell_prop(table, 0, 2, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_UNDERLINED);
    ft_set_cell_prop(table, 2, 1, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_UNDERLINED);

    ft_set_cell_prop(table, 0, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_YELLOW);
    ft_set_cell_prop(table, 0, 1, FT_CPROP_CELL_BG_COLOR, FT_COLOR_MAGENTA);
    ft_set_cell_prop(table, 0, 1, FT_CPROP_CELL_TEXT_STYLE, FT_TSTYLE_UNDERLINED);

    ft_set_cell_prop(table, 1, 1, FT_CPROP_CONT_BG_COLOR, FT_COLOR_RED);
    ft_set_cell_prop(table, 0, 2, FT_CPROP_CONT_BG_COLOR, FT_COLOR_RED);

    ft_set_cell_prop(table, 3, FT_ANY_COLUMN, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
    ft_set_cell_prop(table, 3, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);

    const wchar_t *table_wstr = ft_to_wstring(table);
    if (table_wstr) {
        fwprintf(stdout, L"Table:\n%ls\n ", table_wstr);
    } else {
        fwprintf(stdout, L"Table conversion failed !!!\n ");
    }

    ft_destroy_table(table);



    // /////////////////////////


    table = ft_create_table();
    ft_set_border_style(table, FT_DOUBLE_STYLE);
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);

    ft_wwrite_ln(table, L"Test", L"Iterations", L"ms/op", L"Passed");
    ft_wwrite_ln(table, L"n-body", L"1000", L"1.629");
    ft_wwrite_ln(table, L"", L"2500", L"6.3", L"✔");
    ft_wwrite_ln(table, L"", L"10000", L"10.5");
    ft_add_separator(table);
    ft_wwrite_ln(table, L"regex-redux", L"1000", L"1.629");
    ft_wwrite_ln(table, L"", L"2500", L"6.3", L"✖");
    ft_wwrite_ln(table, L"", L"10000", L"10.5");
    ft_add_separator(table);
    ft_wwrite_ln(table, L"mandelbrot", L"1000", L"1.629");
    ft_wwrite_ln(table, L"", L"2500", L"6.3", L"✔");
    ft_wwrite_ln(table, L"", L"10000", L"10.5");
    ft_add_separator(table);
    ft_set_cell_span(table, 10, 0, 3);
    ft_wwrite_ln(table, L"Total result", L"", L"", L"✖");


    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
    ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);

    ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_RIGHT);
    ft_set_cell_prop(table, FT_ANY_ROW, 3, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
    ft_set_cell_prop(table, 10, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);

    ft_set_cell_prop(table, 2, 3, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
    ft_set_cell_prop(table, 5, 3, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
    ft_set_cell_prop(table, 8, 3, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);


//    ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
//    ft_set_cell_prop(table, 0, 1, FT_CPROP_CELL_TEXT_STYLE, FT_TSTYLE_UNDERLINED);
//    ft_set_cell_prop(table, 0, 2, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_UNDERLINED);
//    ft_set_cell_prop(table, 2, 1, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_UNDERLINED);

//    ft_set_cell_prop(table, 0, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_YELLOW);
//    ft_set_cell_prop(table, 0, 1, FT_CPROP_CELL_BG_COLOR, FT_COLOR_MAGENTA);
//    ft_set_cell_prop(table, 0, 1, FT_CPROP_CELL_TEXT_STYLE, FT_TSTYLE_UNDERLINED);

//    ft_set_cell_prop(table, 1, 1, FT_CPROP_CONT_BG_COLOR, FT_COLOR_RED);
//    ft_set_cell_prop(table, 0, 2, FT_CPROP_CONT_BG_COLOR, FT_COLOR_RED);

//    ft_set_cell_prop(table, 3, FT_ANY_COLUMN, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
//    ft_set_cell_prop(table, 3, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);

    table_wstr = ft_to_wstring(table);
    if (table_wstr) {
        fwprintf(stdout, L"Table:\n%ls\n ", table_wstr);
    } else {
        fwprintf(stdout, L"Table conversion failed !!!\n ");
    }

    ft_destroy_table(table);


//    printf("\n\n------ Some debug stuff delete later --------\n\n\n");


//    ft_table_t *table = ft_create_table();
//    ft_set_cell_prop(table, 0, 0, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
//    ft_set_cell_prop(table, 1, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
//    ft_set_cell_prop(table, FT_ANY_ROW, 2, FT_CPROP_CONT_FG_COLOR, FT_COLOR_MAGENTA);
//    ft_set_cell_prop(table, FT_ANY_ROW, 2, FT_CPROP_CONT_BG_COLOR, FT_COLOR_YELLOW);

//    ft_set_cell_prop(table, 0, 1, FT_CPROP_CONT_FG_COLOR, FT_COLOR_YELLOW);
//    ft_set_cell_prop(table, 0, 1, FT_CPROP_CELL_BG_COLOR, FT_COLOR_MAGENTA);
//    ft_set_cell_prop(table, 0, 1, FT_CPROP_CELL_TEXT_STYLE, FT_TSTYLE_UNDERLINED);

//    ft_set_cell_prop(table, 2, FT_ANY_COLUMN, FT_CPROP_CELL_TEXT_STYLE, FT_TSTYLE_BOLD);
//    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
//    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
//    ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
//    ft_set_cell_prop(table, 0, 1, FT_CPROP_CELL_TEXT_STYLE, FT_TSTYLE_UNDERLINED);
//    ft_set_cell_prop(table, 0, 2, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_UNDERLINED);
//    ft_set_cell_prop(table, 2, 1, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_UNDERLINED);


//    ft_write_ln(table, "some", "some3333", "11111");
//    ft_write_ln(table, "s", "some333377777", "1111111111");
//    ft_write_ln(table, "some", "some3333", "111111");
//    const char *str_repr = ft_to_string(table);
//    printf("%s\n", str_repr ? str_repr : "NULL");
//    ft_destroy_table(table);

//    printf("aa \033[42m 11 \033[4m 222 \033[49m 123 \033[24m 666\n");
    return status;
}
