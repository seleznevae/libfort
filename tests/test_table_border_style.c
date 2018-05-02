#include "tests.h"






void test_table_border_style(void)
{
    FTABLE *table = NULL;

    set_test_options_as_default();


    WHEN("Changing cell separators") {

        struct ft_border_style brdr_style;
        brdr_style.border_chs.top_border_ch = "|";
        brdr_style.border_chs.separator_ch = "|";
        brdr_style.border_chs.bottom_border_ch = "|";
        brdr_style.border_chs.side_border_ch = "=";
        brdr_style.border_chs.out_intersect_ch = "+";
        brdr_style.border_chs.in_intersect_ch = "#";

        brdr_style.header_border_chs.top_border_ch = "*";
        brdr_style.header_border_chs.separator_ch = "*";
        brdr_style.header_border_chs.bottom_border_ch = "*";
        brdr_style.header_border_chs.side_border_ch = "v";
        brdr_style.header_border_chs.out_intersect_ch = "+";
        brdr_style.header_border_chs.in_intersect_ch = "#";

        brdr_style.hor_separator_char = "|";
        ft_set_default_border_style(&brdr_style);



        table = create_test_int_table(0);
        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+***+***+****+****+\n"
            "v   v   v    v    v\n"
            "v 3 v 4 v 55 v 67 v\n"
            "v   v   v    v    v\n"
            "+***#***#****#****+\n"
            "=   =   =    =    =\n"
            "= 3 = 4 = 55 = 67 =\n"
            "=   =   =    =    =\n"
            "+|||#|||#||||#||||+\n"
            "=   =   =    =    =\n"
            "= 3 = 4 = 55 = 67 =\n"
            "=   =   =    =    =\n"
            "+|||+|||+||||+||||+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);



        brdr_style.border_chs.top_border_ch = "|";
        brdr_style.border_chs.separator_ch = "\0";
        brdr_style.border_chs.bottom_border_ch = "|";
        brdr_style.border_chs.side_border_ch = "=";
        brdr_style.border_chs.out_intersect_ch = "+";
        brdr_style.border_chs.in_intersect_ch = "\0";

        brdr_style.header_border_chs.top_border_ch = "*";
        brdr_style.header_border_chs.separator_ch = "*";
        brdr_style.header_border_chs.bottom_border_ch = "*";
        brdr_style.header_border_chs.side_border_ch = "v";
        brdr_style.header_border_chs.out_intersect_ch = "+";
        brdr_style.header_border_chs.in_intersect_ch = "#";

        brdr_style.hor_separator_char = "";

        ft_set_default_border_style(&brdr_style);

        ft_set_default_cell_option(FT_COPT_BOTTOM_PADDING, 0);
        ft_set_default_cell_option(FT_COPT_TOP_PADDING, 0);
        ft_set_default_cell_option(FT_COPT_LEFT_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_RIGHT_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_EMPTY_STR_HEIGHT, 0);


        table = create_test_int_table(0);
        table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        table_str_etalon =
            "+***+***+****+****+\n"
            "v 3 v 4 v 55 v 67 v\n"
            "+***#***#****#****+\n"
            "= 3 = 4 = 55 = 67 =\n"
            "= 3 = 4 = 55 = 67 =\n"
            "+|||+|||+||||+||||+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }


    WHEN("Separator testing") {
        table = create_test_int_table(1);
        ft_add_separator(table);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        int n = ft_printf_ln(table, "%d|%d|%d|%d", 3, 4, 55, 67);

        assert_true(n == 4);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+---+---+----+----+\n"
            "|   |   |    |    |\n"
            "| 3 | 4 | 55 | 67 |\n"
            "|   |   |    |    |\n"
            "+---+---+----+----+\n"
            "|   |   |    |    |\n"
            "| 3 | 4 | 55 | 67 |\n"
            "|   |   |    |    |\n"
            "+---+---+----+----+\n"
            "|   |   |    |    |\n"
            "| 3 | 4 | 55 | 67 |\n"
            "|   |   |    |    |\n"
            "+===+===+====+====+\n"
            "|   |   |    |    |\n"
            "| 3 | 4 | 55 | 67 |\n"
            "|   |   |    |    |\n"
            "+---+---+----+----+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
}
