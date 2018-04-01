#include "tests.h"
#include <wchar.h>

void test_table_basic(void)
{
    FTABLE *table = NULL;

    WHEN("All columns are equal and not empty") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(FT_NWRITE_LN(table, "3", "c", "234", "3.140000") == FT_SUCCESS);
        assert_true(FT_NWRITE_LN(table, "3", "c", "234", "3.140000") == FT_SUCCESS);
        assert_true(FT_NWRITE_LN(table, "3", "c", "234", "3.140000") == FT_SUCCESS);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    WHEN("All columns are equal and not empty (wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(FT_NWWRITE_LN(table, L"3", L"c", L"234", L"3.140000") == FT_SUCCESS);
        assert_true(FT_NWWRITE_LN(table, L"3", L"c", L"234", L"3.140000") == FT_SUCCESS);
        assert_true(FT_NWWRITE_LN(table, L"3", L"c", L"234", L"3.140000") == FT_SUCCESS);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+---+---+-----+----------+\n"
            L"|   |   |     |          |\n"
            L"| 3 | c | 234 | 3.140000 |\n"
            L"|   |   |     |          |\n"
            L"+---+---+-----+----------+\n"
            L"|   |   |     |          |\n"
            L"| 3 | c | 234 | 3.140000 |\n"
            L"|   |   |     |          |\n"
            L"+---+---+-----+----------+\n"
            L"|   |   |     |          |\n"
            L"| 3 | c | 234 | 3.140000 |\n"
            L"|   |   |     |          |\n"
            L"+---+---+-----+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }



    WHEN("All columns are not equal and not empty") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(FT_NWRITE_LN(table, "3", "c", "234", "3.140000") == FT_SUCCESS);
        assert_true(FT_NWRITE_LN(table, "c", "234", "3.140000", "3") == FT_SUCCESS);
        assert_true(FT_NWRITE_LN(table, "234", "3.140000", "3", "c") == FT_SUCCESS);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "|   3 |        c |      234 | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "|   c |      234 | 3.140000 |        3 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 |        3 |        c |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    WHEN("All columns are not equal and not empty (wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(FT_NWWRITE_LN(table, L"3", L"c", L"234", L"3.140000") == FT_SUCCESS);
        assert_true(FT_NWWRITE_LN(table, L"c", L"234", L"3.140000", L"3") == FT_SUCCESS);
        assert_true(FT_NWWRITE_LN(table, L"234", L"3.140000", L"3", L"c") == FT_SUCCESS);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"|   3 |        c |      234 | 3.140000 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"|   c |      234 | 3.140000 |        3 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 234 | 3.140000 |        3 |        c |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    WHEN("All columns are not equal and some cells are empty") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(FT_NWRITE_LN(table, "", "", "234", "3.140000") == FT_SUCCESS);
        assert_true(FT_NWRITE_LN(table, "c", "234", "3.140000", "") == FT_SUCCESS);
        assert_true(FT_NWRITE_LN(table, "234", "3.140000", "", "") == FT_SUCCESS);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "|     |          |      234 | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "|   c |      234 | 3.140000 |          |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 |          |          |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    WHEN("All columns are not equal and some cells are empty (wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(FT_NWWRITE_LN(table, L"", L"", L"234", L"3.140000") == FT_SUCCESS);
        assert_true(FT_NWWRITE_LN(table, L"c", L"234", L"3.140000", L"") == FT_SUCCESS);
        assert_true(FT_NWWRITE_LN(table, L"234", L"3.140000", L"", L"") == FT_SUCCESS);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"|     |          |      234 | 3.140000 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"|   c |      234 | 3.140000 |          |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 234 | 3.140000 |          |          |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    WHEN("All cells are empty") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(FT_NWRITE_LN(table, "", "", "", "") == FT_SUCCESS);
        assert_true(FT_NWRITE_LN(table, "", "", "", "") == FT_SUCCESS);
        assert_true(FT_NWRITE_LN(table, "", "", "", "") == FT_SUCCESS);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+--+--+--+--+\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "+--+--+--+--+\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "+--+--+--+--+\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "+--+--+--+--+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    WHEN("All cells are empty (wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(FT_NWWRITE_LN(table, L"", L"", L"", L"") == FT_SUCCESS);
        assert_true(FT_NWWRITE_LN(table, L"", L"", L"", L"") == FT_SUCCESS);
        assert_true(FT_NWWRITE_LN(table, L"", L"", L"", L"") == FT_SUCCESS);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+--+--+--+--+\n"
            L"|  |  |  |  |\n"
            L"|  |  |  |  |\n"
            L"|  |  |  |  |\n"
            L"+--+--+--+--+\n"
            L"|  |  |  |  |\n"
            L"|  |  |  |  |\n"
            L"|  |  |  |  |\n"
            L"+--+--+--+--+\n"
            L"|  |  |  |  |\n"
            L"|  |  |  |  |\n"
            L"|  |  |  |  |\n"
            L"+--+--+--+--+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
}




void test_wcs_table_boundaries(void)
{
    FTABLE *table = NULL;

    WHEN("All columns are not equal and not empty (wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(FT_NWWRITE_LN(table, L"3", L"12345\x8888\x8888", L"c") == FT_SUCCESS);   /* \x8888,\x8888  - occupy 2 columns each */
        assert_true(FT_NWWRITE_LN(table, L"c", L"12345678\x500", L"c") == FT_SUCCESS);  /* \x500  - occupies 1 column */
        assert_true(FT_NWWRITE_LN(table, L"234", L"123456789", L"c") == FT_SUCCESS);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+-----------+---+\n"
            L"|     |           |   |\n"
            L"|   3 | 12345\x8888\x8888 | c |\n"
            L"|     |           |   |\n"
            L"+-----+-----------+---+\n"
            L"|     |           |   |\n"
            L"|   c | 12345678\x500 | c |\n"
            L"|     |           |   |\n"
            L"+-----+-----------+---+\n"
            L"|     |           |   |\n"
            L"| 234 | 123456789 | c |\n"
            L"|     |           |   |\n"
            L"+-----+-----------+---+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
}


void test_table_write(void)
{
    FTABLE *table = NULL;

    SCENARIO("Test row_write functions") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        const char *row_0[4] = {"3", "c", "234", "3.140000"};
        const char *row_1[4] = {"c", "234", "3.140000", "3"};
        const char *row_2[4] = {"234", "3.140000", "3", "c"};
        assert_true(ft_row_write_ln(table, 4, row_0) == FT_SUCCESS);
        assert_true(ft_row_write_ln(table, 4, row_1) == FT_SUCCESS);
        assert_true(ft_row_write_ln(table, 4, row_2) == FT_SUCCESS);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "|   3 |        c |      234 | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "|   c |      234 | 3.140000 |        3 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 |        3 |        c |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    SCENARIO("Test row_write functions(wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        const wchar_t *row_0[4] = {L"3", L"c", L"234", L"3.140000"};
        const wchar_t *row_1[4] = {L"c", L"234", L"3.140000", L"3"};
        const wchar_t *row_2[4] = {L"234", L"3.140000", L"3", L"c"};
        assert_true(ft_row_wwrite_ln(table, 4, row_0) == FT_SUCCESS);
        assert_true(ft_row_wwrite_ln(table, 4, row_1) == FT_SUCCESS);
        assert_true(ft_row_wwrite_ln(table, 4, row_2) == FT_SUCCESS);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"|   3 |        c |      234 | 3.140000 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"|   c |      234 | 3.140000 |        3 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 234 | 3.140000 |        3 |        c |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }


    SCENARIO("Test printf functions") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        int n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true(n == 4);
        n = ft_printf_ln(table, "%c|%s|%f|%d", 'c', "234", 3.14, 3);
        assert_true(n == 4);
        n = ft_printf_ln(table, "%s|%f|%d|%c", "234", 3.14,  3, 'c');
        assert_true(n == 4);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "|   3 |        c |      234 | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "|   c |      234 | 3.140000 |        3 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 |        3 |        c |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    SCENARIO("Test printf functions(wide strings)") {
        /* todo: need to implement */
    }

}
