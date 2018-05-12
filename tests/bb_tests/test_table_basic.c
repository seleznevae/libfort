#include "tests.h"
#include <wchar.h>
#include "fort.h"

void test_table_basic(void)
{
    ft_table_t *table = NULL;

    WHEN("All columns are equal and not empty") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_write_ln(table, "3", "c", "234", "3.140000") == FT_SUCCESS);
        assert_true(ft_write_ln(table, "3", "c", "234", "3.140000") == FT_SUCCESS);
        assert_true(ft_write_ln(table, "3", "c", "234", "3.140000") == FT_SUCCESS);

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

#ifdef FT_HAVE_WCHAR
    WHEN("All columns are equal and not empty (wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_wwrite_ln(table, L"3", L"c", L"234", L"3.140000") == FT_SUCCESS);
        assert_true(ft_wwrite_ln(table, L"3", L"c", L"234", L"3.140000") == FT_SUCCESS);
        assert_true(ft_wwrite_ln(table, L"3", L"c", L"234", L"3.140000") == FT_SUCCESS);

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
#endif



    WHEN("All columns are not equal and not empty") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_write_ln(table, "3", "c", "234", "3.140000") == FT_SUCCESS);
        assert_true(ft_write_ln(table, "c", "234", "3.140000", "3") == FT_SUCCESS);
        assert_true(ft_write_ln(table, "234", "3.140000", "3", "c") == FT_SUCCESS);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 3   | c        | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 | 3        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 | 3        | c        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

#ifdef FT_HAVE_WCHAR
    WHEN("All columns are not equal and not empty (wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_wwrite_ln(table, L"3", L"c", L"234", L"3.140000") == FT_SUCCESS);
        assert_true(ft_wwrite_ln(table, L"c", L"234", L"3.140000", L"3") == FT_SUCCESS);
        assert_true(ft_wwrite_ln(table, L"234", L"3.140000", L"3", L"c") == FT_SUCCESS);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 3   | c        | 234      | 3.140000 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| c   | 234      | 3.140000 | 3        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 234 | 3.140000 | 3        | c        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
#endif

    WHEN("All columns are not equal and some cells are empty") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_write_ln(table, "", "", "234", "3.140000") == FT_SUCCESS);
        assert_true(ft_write_ln(table, "c", "234", "3.140000", "") == FT_SUCCESS);
        assert_true(ft_write_ln(table, "234", "3.140000", "", "") == FT_SUCCESS);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "|     |          | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 |          |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 |          |          |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

#ifdef FT_HAVE_WCHAR
    WHEN("All columns are not equal and some cells are empty (wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_wwrite_ln(table, L"", L"", L"234", L"3.140000") == FT_SUCCESS);
        assert_true(ft_wwrite_ln(table, L"c", L"234", L"3.140000", L"") == FT_SUCCESS);
        assert_true(ft_wwrite_ln(table, L"234", L"3.140000", L"", L"") == FT_SUCCESS);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"|     |          | 234      | 3.140000 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| c   | 234      | 3.140000 |          |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 234 | 3.140000 |          |          |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
#endif

    WHEN("All cells are empty") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_write_ln(table, "", "", "", "") == FT_SUCCESS);
        assert_true(ft_write_ln(table, "", "", "", "") == FT_SUCCESS);
        assert_true(ft_write_ln(table, "", "", "", "") == FT_SUCCESS);

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

#ifdef FT_HAVE_WCHAR
    WHEN("All cells are empty (wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_wwrite_ln(table, L"", L"", L"", L"") == FT_SUCCESS);
        assert_true(ft_wwrite_ln(table, L"", L"", L"", L"") == FT_SUCCESS);
        assert_true(ft_wwrite_ln(table, L"", L"", L"", L"") == FT_SUCCESS);

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
#endif
}



#ifdef FT_HAVE_WCHAR
void test_wcs_table_boundaries(void)
{
    ft_table_t *table = NULL;

    WHEN("All columns are not equal and not empty (wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_wwrite_ln(table, L"3", L"12345\x8888\x8888", L"c") == FT_SUCCESS);   /* \x8888,\x8888  - occupy 2 columns each */
        assert_true(ft_wwrite_ln(table, L"c", L"12345678\x500", L"c") == FT_SUCCESS);  /* \x500  - occupies 1 column */
        assert_true(ft_wwrite_ln(table, L"234", L"123456789", L"c") == FT_SUCCESS);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+-----------+---+\n"
            L"|     |           |   |\n"
            L"| 3   | 12345\x8888\x8888 | c |\n"
            L"|     |           |   |\n"
            L"+-----+-----------+---+\n"
            L"|     |           |   |\n"
            L"| c   | 12345678\x500 | c |\n"
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
#endif


void test_table_write(void)
{
    ft_table_t *table = NULL;

    SCENARIO("Test write functions") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(FT_IS_SUCCESS(ft_write(table, "3")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "c")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "234")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "3.140000")));
        ft_ln(table);
        assert_true(FT_IS_SUCCESS(ft_write(table, "c")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "235")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "3.150000")));
        assert_true(FT_IS_SUCCESS(ft_write_ln(table, "5")));

        assert_true(FT_IS_SUCCESS(ft_write(table, "234")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "3.140000")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "3")));
        assert_true(FT_IS_SUCCESS(ft_write_ln(table, "c")));

        /* Replace old values */
        ft_set_cur_cell(table, 1, 1);
        assert_true(FT_IS_SUCCESS(ft_write(table, "234")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "3.140000")));
        assert_true(FT_IS_SUCCESS(ft_write_ln(table, "3")));

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 3   | c        | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 | 3        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 | 3        | c        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

#ifdef FT_HAVE_WCHAR
    SCENARIO("Test wwrite functions(wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"3")));
        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"c")));
        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"234")));
        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"3.140000")));
        ft_ln(table);
        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"c")));
        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"235")));
        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"3.150000")));
        assert_true(FT_IS_SUCCESS(ft_wwrite_ln(table, L"5")));

        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"234")));
        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"3.140000")));
        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"3")));
        assert_true(FT_IS_SUCCESS(ft_wwrite_ln(table, L"c")));

        /* Replace old values */
        ft_set_cur_cell(table, 1, 1);
        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"234")));
        assert_true(FT_IS_SUCCESS(ft_wwrite(table, L"3.140000")));
        assert_true(FT_IS_SUCCESS(ft_wwrite_ln(table, L"3")));

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 3   | c        | 234      | 3.140000 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| c   | 234      | 3.140000 | 3        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 234 | 3.140000 | 3        | c        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
#endif

    SCENARIO("Test nwrite functions") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_nwrite(table, 4, "3", "c", "234", "3.140000") == FT_SUCCESS);
        ft_ln(table);
        assert_true(ft_nwrite_ln(table, 4, "c", "235", "3.150000", "5") == FT_SUCCESS);
        assert_true(ft_nwrite_ln(table, 4, "234", "3.140000", "3", "c") == FT_SUCCESS);

        /* Replace old values */
        ft_set_cur_cell(table, 1, 1);
        assert_true(ft_nwrite_ln(table, 3, "234", "3.140000", "3") == FT_SUCCESS);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 3   | c        | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 | 3        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 | 3        | c        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

#ifdef FT_HAVE_WCHAR
    SCENARIO("Test nwwrite functions(wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_nwwrite(table, 4, L"3", L"c", L"234", L"3.140000") == FT_SUCCESS);
        ft_ln(table);
        assert_true(ft_nwwrite_ln(table, 4, L"c", L"235", L"3.150000", L"5") == FT_SUCCESS);
        assert_true(ft_nwwrite_ln(table, 4, L"234", L"3.140000", L"3", L"c") == FT_SUCCESS);

        /* Replace old values */
        ft_set_cur_cell(table, 1, 1);
        assert_true(ft_nwwrite_ln(table, 3, L"234", L"3.140000", L"3") == FT_SUCCESS);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 3   | c        | 234      | 3.140000 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| c   | 234      | 3.140000 | 3        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 234 | 3.140000 | 3        | c        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
#endif


    SCENARIO("Test row_write functions") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        const char *row_0[4] = {"3", "c", "234", "3.140000"};
        const char *row_1[4] = {"c", "235", "3.150000", "5"};
        const char *row_2[4] = {"234", "3.140000", "3", "c"};
        assert_true(ft_row_write_ln(table, 4, row_0) == FT_SUCCESS);
        assert_true(ft_row_write(table, 4, row_1) == FT_SUCCESS);
        ft_ln(table);
        assert_true(ft_row_write_ln(table, 4, row_2) == FT_SUCCESS);

        /* Replace old values */
        ft_set_cur_cell(table, 1, 1);
        const char *row_11[3] = {"234", "3.140000", "3"};
        assert_true(ft_row_write(table, 3, row_11) == FT_SUCCESS);
        ft_ln(table);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 3   | c        | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 | 3        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 | 3        | c        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

#ifdef FT_HAVE_WCHAR
    SCENARIO("Test row_write functions(wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        const wchar_t *row_0[4] = {L"3", L"c", L"234", L"3.140000"};
        const wchar_t *row_1[4] = {L"c", L"235", L"3.150000", L"5"};
        const wchar_t *row_2[4] = {L"234", L"3.140000", L"3", L"c"};
        assert_true(ft_row_wwrite_ln(table, 4, row_0) == FT_SUCCESS);
        assert_true(ft_row_wwrite(table, 4, row_1) == FT_SUCCESS);
        ft_ln(table);
        assert_true(ft_row_wwrite_ln(table, 4, row_2) == FT_SUCCESS);

        /* Replace old values */
        ft_set_cur_cell(table, 1, 1);
        const wchar_t *row_11[3] = {L"234", L"3.140000", L"3"};
        assert_true(ft_row_wwrite(table, 3, row_11) == FT_SUCCESS);
        ft_ln(table);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 3   | c        | 234      | 3.140000 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| c   | 234      | 3.140000 | 3        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 234 | 3.140000 | 3        | c        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
#endif

    SCENARIO("Test table_write functions") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        const char *table_cont[3][4] = {
            {"3", "c", "234", "3.140000"},
            {"c", "234", "3.140000", "3"},
            {"234", "3.140000", "3", "c"}
        };
        assert_true(ft_table_write_ln(table, 3, 4, (const char **)table_cont) == FT_SUCCESS);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 3   | c        | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 | 3        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 | 3        | c        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

#ifdef FT_HAVE_WCHAR
    SCENARIO("Test table_write functions(wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        const wchar_t *table_cont[3][4] = {
            {L"3", L"c", L"234", L"3.140000"},
            {L"c", L"234", L"3.140000", L"3"},
            {L"234", L"3.140000", L"3", L"c"}
        };
        assert_true(ft_table_wwrite_ln(table, 3, 4, (const wchar_t **)table_cont) == FT_SUCCESS);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 3   | c        | 234      | 3.140000 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| c   | 234      | 3.140000 | 3        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 234 | 3.140000 | 3        | c        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
#endif

    SCENARIO("Test printf functions") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        int n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true(n == 4);
        n = ft_printf(table, "%c|%s|%f|%d", 'c', "235", 3.15, 5);
        assert_true(n == 4);
        ft_ln(table);
        n = ft_printf_ln(table, "%s|%f|%d|%c", "234", 3.14,  3, 'c');
        assert_true(n == 4);

        /* Replace old values */
        ft_set_cur_cell(table, 1, 1);
        n = ft_printf(table, "%s|%f|%d", "234", 3.14, 3);
        assert_true(n == 3);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 3   | c        | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 | 3        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 | 3        | c        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

#ifdef FT_HAVE_WCHAR
    SCENARIO("Test printf functions(wide strings)") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        int n = ft_wprintf_ln(table, L"%d|%c|%ls|%f", 3, 'c', L"234", 3.14);
        assert_true(n == 4);
        n = ft_wprintf(table, L"%c|%ls|%f|%d", 'c', L"235", 3.15, 5);
        assert_true(n == 4);
        ft_ln(table);
        n = ft_wprintf_ln(table, L"%ls|%f|%d|%c", L"234", 3.14,  3, 'c');
        assert_true(n == 4);

        /* Replace old values */
        ft_set_cur_cell(table, 1, 1);
        n = ft_wprintf_ln(table, L"%ls|%f|%d", L"234", 3.14, 3);
        assert_true(n == 3);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 3   | c        | 234      | 3.140000 |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| c   | 234      | 3.140000 | 3        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n"
            L"|     |          |          |          |\n"
            L"| 234 | 3.140000 | 3        | c        |\n"
            L"|     |          |          |          |\n"
            L"+-----+----------+----------+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
#endif

    SCENARIO("Test printf functions with strings with separators inside them") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        int n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true(n == 4);
        n = ft_printf(table, "%c", 'c');
        assert_true(n == 1);
        n = ft_printf(table, "%s", "234");
        assert_true(n == 1);
        n = ft_printf(table, "%s", "string|with separator");
        assert_true(n == 1);
        n = ft_printf(table, "3");
        assert_true(n == 1);
        ft_ln(table);
        n = ft_printf_ln(table, "%s|%f|%d|%c", "234", 3.14,  3, 'c');
        assert_true(n == 4);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+-----+----------+-----------------------+----------+\n"
            "|     |          |                       |          |\n"
            "| 3   | c        | 234                   | 3.140000 |\n"
            "|     |          |                       |          |\n"
            "+-----+----------+-----------------------+----------+\n"
            "|     |          |                       |          |\n"
            "| c   | 234      | string|with separator | 3        |\n"
            "|     |          |                       |          |\n"
            "+-----+----------+-----------------------+----------+\n"
            "|     |          |                       |          |\n"
            "| 234 | 3.140000 | 3                     | c        |\n"
            "|     |          |                       |          |\n"
            "+-----+----------+-----------------------+----------+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

}
