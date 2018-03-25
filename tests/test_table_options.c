//#define FT_EXTERN static
#include "tests.h"
#include "fort.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "options.h"
#include "vector.h"



void test_table_tbl_options(void)
{
    FTABLE *table = NULL;

    WHEN("Test setting entire table options") {
        set_test_options_as_default();

        table = create_test_int_table(0);

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
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
                "+---+---+----+----+\n";
        assert_str_equal(table_str, table_str_etalon);

        /* Now set table options */
        ft_set_tbl_option(table, FT_TOPT_TOP_MARGIN, 3);
        ft_set_tbl_option(table, FT_TOPT_BOTTOM_MARGIN, 4);
        ft_set_tbl_option(table, FT_TOPT_LEFT_MARGIN, 1);
        ft_set_tbl_option(table, FT_TOPT_RIGHT_MARGIN, 2);
        table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        table_str_etalon =
                "                      \n"
                "                      \n"
                "                      \n"
                " +---+---+----+----+  \n"
                " |   |   |    |    |  \n"
                " | 3 | 4 | 55 | 67 |  \n"
                " |   |   |    |    |  \n"
                " +---+---+----+----+  \n"
                " |   |   |    |    |  \n"
                " | 3 | 4 | 55 | 67 |  \n"
                " |   |   |    |    |  \n"
                " +---+---+----+----+  \n"
                " |   |   |    |    |  \n"
                " | 3 | 4 | 55 | 67 |  \n"
                " |   |   |    |    |  \n"
                " +---+---+----+----+  \n"
                "                      \n"
                "                      \n"
                "                      \n"
                "                      \n";
        assert_str_equal(table_str, table_str_etalon);

        ft_destroy_table(table);
    }


    WHEN("Test setting entire table options") {
        set_test_options_as_default();

        table = create_test_int_wtable(0);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true( table_str != NULL );
        const wchar_t *table_str_etalon =
                L"+---+---+----+----+\n"
                L"|   |   |    |    |\n"
                L"| 3 | 4 | 55 | 67 |\n"
                L"|   |   |    |    |\n"
                L"+---+---+----+----+\n"
                L"|   |   |    |    |\n"
                L"| 3 | 4 | 55 | 67 |\n"
                L"|   |   |    |    |\n"
                L"+---+---+----+----+\n"
                L"|   |   |    |    |\n"
                L"| 3 | 4 | 55 | 67 |\n"
                L"|   |   |    |    |\n"
                L"+---+---+----+----+\n";
        assert_wcs_equal(table_str, table_str_etalon);

        /* Now set table options */
        ft_set_tbl_option(table, FT_TOPT_TOP_MARGIN, 3);
        ft_set_tbl_option(table, FT_TOPT_BOTTOM_MARGIN, 4);
        ft_set_tbl_option(table, FT_TOPT_LEFT_MARGIN, 1);
        ft_set_tbl_option(table, FT_TOPT_RIGHT_MARGIN, 2);
        table_str = ft_to_wstring(table);
        assert_true( table_str != NULL );
        table_str_etalon =
                L"                      \n"
                L"                      \n"
                L"                      \n"
                L" +---+---+----+----+  \n"
                L" |   |   |    |    |  \n"
                L" | 3 | 4 | 55 | 67 |  \n"
                L" |   |   |    |    |  \n"
                L" +---+---+----+----+  \n"
                L" |   |   |    |    |  \n"
                L" | 3 | 4 | 55 | 67 |  \n"
                L" |   |   |    |    |  \n"
                L" +---+---+----+----+  \n"
                L" |   |   |    |    |  \n"
                L" | 3 | 4 | 55 | 67 |  \n"
                L" |   |   |    |    |  \n"
                L" +---+---+----+----+  \n"
                L"                      \n"
                L"                      \n"
                L"                      \n"
                L"                      \n";
        assert_wcs_equal(table_str, table_str_etalon);

        ft_destroy_table(table);
    }
}



void test_table_cell_options(void)
{
    FTABLE *table = NULL;


    WHEN("All paddings = 1") {
        set_test_options_as_default();

        ft_set_default_cell_option(FT_COPT_BOTTOM_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_TOP_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_LEFT_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_RIGHT_PADDING, 1);

        table = create_test_int_table(0);

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
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
                "+---+---+----+----+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }



    WHEN("Top and bottom padding = 0") {

        ft_set_default_cell_option(FT_COPT_BOTTOM_PADDING, 0);
        ft_set_default_cell_option(FT_COPT_TOP_PADDING, 0);
        ft_set_default_cell_option(FT_COPT_LEFT_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_RIGHT_PADDING, 1);

        table = create_test_int_table(0);

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "+---+---+----+----+\n"
                "| 3 | 4 | 55 | 67 |\n"
                "+---+---+----+----+\n"
                "| 3 | 4 | 55 | 67 |\n"
                "+---+---+----+----+\n"
                "| 3 | 4 | 55 | 67 |\n"
                "+---+---+----+----+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    WHEN("Left and right padding = 0") {

        ft_set_default_cell_option(FT_COPT_BOTTOM_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_TOP_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_LEFT_PADDING, 0);
        ft_set_default_cell_option(FT_COPT_RIGHT_PADDING, 0);

        table = create_test_int_table(0);

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "+-+-+--+--+\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "+-+-+--+--+\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "+-+-+--+--+\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "+-+-+--+--+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    WHEN("All paddings = 0") {

        ft_set_default_cell_option(FT_COPT_BOTTOM_PADDING, 0);
        ft_set_default_cell_option(FT_COPT_TOP_PADDING, 0);
        ft_set_default_cell_option(FT_COPT_LEFT_PADDING, 0);
        ft_set_default_cell_option(FT_COPT_RIGHT_PADDING, 0);

        table = create_test_int_table(0);

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "+-+-+--+--+\n"
                "|3|4|55|67|\n"
                "+-+-+--+--+\n"
                "|3|4|55|67|\n"
                "+-+-+--+--+\n"
                "|3|4|55|67|\n"
                "+-+-+--+--+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    WHEN("Empty string has 0 heigt") {

        ft_set_default_cell_option(FT_COPT_BOTTOM_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_TOP_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_LEFT_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_RIGHT_PADDING, 1);
        ft_set_default_cell_option(FT_COPT_EMPTY_STR_HEIGHT, 0);

        table = create_test_int_table(0);
        int n = ft_printf_ln(table, "|||");
        assert_true( n == 4 );

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
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
                "+---+---+----+----+\n"
                "|   |   |    |    |\n"
                "|   |   |    |    |\n"
                "+---+---+----+----+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }


    WHEN("Setting options for a particular table") {

        table = create_test_int_table(0);
        set_test_options_for_table(table);

        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_BOTTOM_PADDING, 0);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_TOP_PADDING, 0);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_LEFT_PADDING, 0);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_RIGHT_PADDING, 0);

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "+-+-+--+--+\n"
                "|3|4|55|67|\n"
                "+-+-+--+--+\n"
                "|3|4|55|67|\n"
                "+-+-+--+--+\n"
                "|3|4|55|67|\n"
                "+-+-+--+--+\n";
        assert_str_equal(table_str, table_str_etalon);


        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_BOTTOM_PADDING, 1);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_TOP_PADDING, 1);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_LEFT_PADDING, 0);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_RIGHT_PADDING, 0);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_COPT_EMPTY_STR_HEIGHT, 0);

        table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        table_str_etalon =
                "+-+-+--+--+\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "+-+-+--+--+\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "+-+-+--+--+\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "+-+-+--+--+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }



    WHEN("Set table width and column alignment") {

        set_test_options_as_default();

        table = create_test_int_table(0);
        int status = FT_SUCCESS;

        status |= ft_set_cell_option(table, FT_ANY_ROW, 1, FT_COPT_MIN_WIDTH, 7);
        status |= ft_set_cell_option(table, FT_ANY_ROW, 1, FT_COPT_TEXT_ALIGN, LeftAligned);
        status |= ft_set_cell_option(table, FT_ANY_ROW, 2, FT_COPT_MIN_WIDTH, 8);
        status |= ft_set_cell_option(table, FT_ANY_ROW, 2, FT_COPT_TEXT_ALIGN, CenterAligned);

        status |= ft_set_cell_option(table, 2, 3, FT_COPT_MIN_WIDTH, 6);
        status |= ft_set_cell_option(table, 2, 3, FT_COPT_TEXT_ALIGN, LeftAligned);
        assert_true( status == FT_SUCCESS);


        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "+---+-------+--------+------+\n"
                "|   |       |        |      |\n"
                "| 3 | 4     |   55   |   67 |\n"
                "|   |       |        |      |\n"
                "+---+-------+--------+------+\n"
                "|   |       |        |      |\n"
                "| 3 | 4     |   55   |   67 |\n"
                "|   |       |        |      |\n"
                "+---+-------+--------+------+\n"
                "|   |       |        |      |\n"
                "| 3 | 4     |   55   | 67   |\n"
                "|   |       |        |      |\n"
                "+---+-------+--------+------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    WHEN("Set table width and column alignment as default") {

        set_test_options_as_default();

        int status = FT_SUCCESS;
        status |= ft_set_default_cell_option(FT_COPT_MIN_WIDTH, 5);
        status |= ft_set_default_cell_option(FT_COPT_TEXT_ALIGN, CenterAligned);
        assert_true( status == FT_SUCCESS);

        table = create_test_int_table(0);

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "+-----+-----+-----+-----+\n"
                "|     |     |     |     |\n"
                "|  3  |  4  | 55  | 67  |\n"
                "|     |     |     |     |\n"
                "+-----+-----+-----+-----+\n"
                "|     |     |     |     |\n"
                "|  3  |  4  | 55  | 67  |\n"
                "|     |     |     |     |\n"
                "+-----+-----+-----+-----+\n"
                "|     |     |     |     |\n"
                "|  3  |  4  | 55  | 67  |\n"
                "|     |     |     |     |\n"
                "+-----+-----+-----+-----+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    WHEN("Multiline cell") {
        set_test_options_as_default();

        table = ft_create_table();

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, Header);
        int n = ft_printf_ln(table, "%d|%c|%s|%f", 4, 'c', "234", 3.14);

        assert_true( n == 4 );
        n = FT_NWRITE_LN(table, "5", "c", "234\n12", "3.140000");
        assert_true( n == FT_SUCCESS );
        n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true( n == 4 );

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "+---+---+-----+----------+\n"
                "|   |   |     |          |\n"
                "| 4 | c | 234 | 3.140000 |\n"
                "|   |   |     |          |\n"
                "+---+---+-----+----------+\n"
                "|   |   |     |          |\n"
                "| 5 | c | 234 | 3.140000 |\n"
                "|   |   | 12  |          |\n"
                "|   |   |     |          |\n"
                "+---+---+-----+----------+\n"
                "|   |   |     |          |\n"
                "| 3 | c | 234 | 3.140000 |\n"
                "|   |   |     |          |\n"
                "+---+---+-----+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
}
