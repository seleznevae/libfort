#include "tests.h"
#include "fort.h"

int set_test_options_for_table(FTABLE *table)
{
    assert(table);
    int status = FT_SUCCESS;
    status |= ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_BOTTOM_PADDING, 1);
    status |= ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_TOP_PADDING, 1);
    status |= ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_LEFT_PADDING, 1);
    status |= ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_RIGHT_PADDING, 1);
    status |= ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_EMPTY_STR_HEIGHT, 1);

    assert_true(status == FT_SUCCESS);


    struct ft_border_style brdr_style;
    brdr_style.border_chs.top_border_ch = '-';
    brdr_style.border_chs.separator_ch = '-';
    brdr_style.border_chs.bottom_border_ch = '-';
    brdr_style.border_chs.side_border_ch = '|';
    brdr_style.border_chs.out_intersect_ch = '+';
    brdr_style.border_chs.in_intersect_ch = '+';

    brdr_style.header_border_chs.top_border_ch = '-';
    brdr_style.header_border_chs.separator_ch = '-';
    brdr_style.header_border_chs.bottom_border_ch = '-';
    brdr_style.header_border_chs.side_border_ch = '|';
    brdr_style.header_border_chs.out_intersect_ch = '+';
    brdr_style.header_border_chs.in_intersect_ch = '+';

    brdr_style.hor_separator_char = '=';
    return ft_set_border_style(table, &brdr_style);
}

int set_test_options_as_default(void)
{
    int status = FT_SUCCESS;

    status |= ft_set_default_cell_option(FT_COPT_MIN_WIDTH, 0);
    status |= ft_set_default_cell_option(FT_COPT_TEXT_ALIGN, FT_ALIGNED_RIGHT);

    status |= ft_set_default_cell_option(FT_COPT_BOTTOM_PADDING, 1);
    status |= ft_set_default_cell_option(FT_COPT_TOP_PADDING, 1);
    status |= ft_set_default_cell_option(FT_COPT_LEFT_PADDING, 1);
    status |= ft_set_default_cell_option(FT_COPT_RIGHT_PADDING, 1);
    status |= ft_set_default_cell_option(FT_COPT_EMPTY_STR_HEIGHT, 1);

    assert_true(status == FT_SUCCESS);


    struct ft_border_style brdr_style;
    brdr_style.border_chs.top_border_ch = '-';
    brdr_style.border_chs.separator_ch = '-';
    brdr_style.border_chs.bottom_border_ch = '-';
    brdr_style.border_chs.side_border_ch = '|';
    brdr_style.border_chs.out_intersect_ch = '+';
    brdr_style.border_chs.in_intersect_ch = '+';

    brdr_style.header_border_chs.top_border_ch = '-';
    brdr_style.header_border_chs.separator_ch = '-';
    brdr_style.header_border_chs.bottom_border_ch = '-';
    brdr_style.header_border_chs.side_border_ch = '|';
    brdr_style.header_border_chs.out_intersect_ch = '+';
    brdr_style.header_border_chs.in_intersect_ch = '+';

    brdr_style.hor_separator_char = '=';

    return ft_set_default_border_style(&brdr_style);
}



FTABLE *create_test_int_table(int set_test_opts)
{
    FTABLE *table = NULL;

    table = ft_create_table();
    assert_true(table != NULL);
    if (set_test_opts) {
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);
    }

    assert_true(table != NULL);

    ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
//    int n = ft_printf_ln(table, "%d|%d|%d|%d", 3, 4, 55, 67);
    int n = ft_write_ln(table, "3", "4", "55", "67");
    assert(n == FT_SUCCESS);

    assert(ft_write(table, "3") == FT_SUCCESS);
    assert(ft_write(table, "4") == FT_SUCCESS);
    assert(ft_write(table, "55") == FT_SUCCESS);
    assert(ft_write_ln(table, "67") == FT_SUCCESS);

    assert(ft_write(table, "3") == FT_SUCCESS);
    assert(ft_write(table, "4") == FT_SUCCESS);
    assert(ft_write(table, "55") == FT_SUCCESS);
    assert(ft_write_ln(table, "67") == FT_SUCCESS);

    return table;
}

#ifdef FT_HAVE_WCHAR
FTABLE *create_test_int_wtable(int set_test_opts)
{
    FTABLE *table = NULL;

    table = ft_create_table();
    assert_true(table != NULL);
    if (set_test_opts) {
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);
    }

    assert_true(table != NULL);

    ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
//    int n = ft_printf_ln(table, "%d|%d|%d|%d", 3, 4, 55, 67);
    int n = ft_wwrite_ln(table, L"3", L"4", L"55", L"67");
    assert(n == FT_SUCCESS);

    assert(ft_wwrite(table, L"3") == FT_SUCCESS);
    assert(ft_wwrite(table, L"4") == FT_SUCCESS);
    assert(ft_wwrite(table, L"55") == FT_SUCCESS);
    assert(ft_wwrite_ln(table, L"67") == FT_SUCCESS);

    assert(ft_wwrite(table, L"3") == FT_SUCCESS);
    assert(ft_wwrite(table, L"4") == FT_SUCCESS);
    assert(ft_wwrite(table, L"55") == FT_SUCCESS);
    assert(ft_wwrite_ln(table, L"67") == FT_SUCCESS);

    return table;
}
#endif
