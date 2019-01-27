#include "tests.h"
#include "test_utils.h"
#include <wchar.h>
#include "fort.h"

#ifdef FT_HAVE_WCHAR
void test_bug_fixes(void)
{
    SCENARIO("Bug 1") {
        ft_table_t *table = ft_create_table();

        ft_table_t *table_tmp_1 = ft_create_table();

//        ft_set_border_style(table_tmp_1, FT_EMPTY_STYLE);
        ft_set_cell_prop(table_tmp_1, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        ft_wwrite_ln(table_tmp_1, L"Rank", L"Title", L"Year", L"Rating");
        ft_wwrite_ln(table_tmp_1, L"1", L"The Shawshank Redemption", L"3");

        const wchar_t *str = ft_to_wstring(table_tmp_1);
        (void)str;
        ft_wprintf_ln(table, str);

        ft_destroy_table(table_tmp_1);
        ft_destroy_table(table);
    }

    SCENARIO("Bug 2") {
        ft_table_t *table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_border_style(table, FT_BASIC_STYLE);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        ft_set_cell_prop(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_CPROP_EMPTY_STR_HEIGHT, 1);
        ft_set_cell_prop(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_CPROP_TOP_PADDING, 0);
        ft_set_cell_prop(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_CPROP_BOTTOM_PADDING, 0);
        assert_true(FT_IS_SUCCESS(ft_write(table, "3")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "c")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "234")));
        assert_true(FT_IS_SUCCESS(ft_write_ln(table, "3.140000")));
        ft_ln(table);
        ft_ln(table);

        assert_true(FT_IS_SUCCESS(ft_write(table, "3")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "c")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "234")));
        assert_true(FT_IS_SUCCESS(ft_write_ln(table, "3.140000")));
        ft_ln(table);
        ft_ln(table);

        assert_true(FT_IS_SUCCESS(ft_write(table, "3")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "c")));
        assert_true(FT_IS_SUCCESS(ft_write(table, "234")));
        assert_true(FT_IS_SUCCESS(ft_write_ln(table, "3.140000")));

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+---+---+-----+----------+\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "+---+---+-----+----------+\n";
        assert_str_equal(table_str, table_str_etalon);


        ft_set_cell_prop(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_CPROP_EMPTY_STR_HEIGHT, 0);
        table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        table_str_etalon =
            "+---+---+-----+----------+\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "+---+---+-----+----------+\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "+---+---+-----+----------+\n";
        assert_str_equal(table_str, table_str_etalon);

        ft_destroy_table(table);
    }
}
#endif

void test_table_basic(void)
{
    ft_table_t *table = NULL;

    WHEN("All columns are equal and not empty") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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

#ifdef FT_HAVE_WCHAR
    SCENARIO("Test printf functions with strings with separators inside them") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        int n = ft_wprintf_ln(table, L"%d|%c|%ls|%f", 3, 'c', L"234", 3.14);
        assert_true(n == 4);
        n = ft_wprintf(table, L"%c", 'c');
        assert_true(n == 1);
        n = ft_wprintf(table, L"%ls", L"234");
        assert_true(n == 1);
        n = ft_wprintf(table, L"%ls", L"string|with separator");
        assert_true(n == 1);
        n = ft_wprintf(table, L"3");
        assert_true(n == 1);
        ft_ln(table);
        n = ft_wprintf_ln(table, L"%ls|%f|%d|%c", L"234", 3.14,  3, 'c');
        assert_true(n == 4);

        const wchar_t *table_str = ft_to_wstring(table);
        assert_true(table_str != NULL);
        const wchar_t *table_str_etalon =
            L"+-----+----------+-----------------------+----------+\n"
            L"|     |          |                       |          |\n"
            L"| 3   | c        | 234                   | 3.140000 |\n"
            L"|     |          |                       |          |\n"
            L"+-----+----------+-----------------------+----------+\n"
            L"|     |          |                       |          |\n"
            L"| c   | 234      | string|with separator | 3        |\n"
            L"|     |          |                       |          |\n"
            L"+-----+----------+-----------------------+----------+\n"
            L"|     |          |                       |          |\n"
            L"| 234 | 3.140000 | 3                     | c        |\n"
            L"|     |          |                       |          |\n"
            L"+-----+----------+-----------------------+----------+\n";
        assert_wcs_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }
#endif

    SCENARIO("Test printf functions with custom separator") {
        ft_set_default_printf_field_separator('$');
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        int n = ft_printf_ln(table, "%d$%c$%s$%f", 3, 'c', "234", 3.14);
        assert_true(n == 4);
        n = ft_printf(table, "%c$%s$%f$%d", 'c', "235", 3.15, 5);
        assert_true(n == 4);
        ft_ln(table);
        n = ft_printf_ln(table, "%s$%f$%d$%c", "234", 3.14,  3, 'c');
        assert_true(n == 4);

        /* Replace old values */
        ft_set_cur_cell(table, 1, 1);
        n = ft_printf(table, "%s$%f$%d", "234", 3.14, 3);
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
        ft_set_default_printf_field_separator('|');
    }

#ifdef FT_HAVE_WCHAR
    SCENARIO("Test printf functions(wide strings)  with custom separator") {
        ft_set_default_printf_field_separator('$');
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        int n = ft_wprintf_ln(table, L"%d$%c$%ls$%f", 3, 'c', L"234", 3.14);
        assert_true(n == 4);
        n = ft_wprintf(table, L"%c$%ls$%f$%d", 'c', L"235", 3.15, 5);
        assert_true(n == 4);
        ft_ln(table);
        n = ft_wprintf_ln(table, L"%ls$%f$%d$%c", L"234", 3.14,  3, 'c');
        assert_true(n == 4);

        /* Replace old values */
        ft_set_cur_cell(table, 1, 1);
        n = ft_wprintf_ln(table, L"%ls$%f$%d", L"234", 3.14, 3);
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
        ft_set_default_printf_field_separator('|');
    }
#endif

}


void test_table_copy(void)
{
    ft_table_t *table = NULL;

    WHEN("Test table copy") {
        table = ft_create_table();
        assert_true(table != NULL);

        assert_true(ft_set_cell_prop(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_CPROP_BOTTOM_PADDING, 1) == FT_SUCCESS);
        assert_true(ft_set_cell_prop(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_CPROP_TOP_PADDING, 1) == FT_SUCCESS);
        assert_true(ft_set_cell_prop(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_CPROP_LEFT_PADDING, 2) == FT_SUCCESS);
        assert_true(ft_set_cell_prop(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_CPROP_RIGHT_PADDING, 2) == FT_SUCCESS);


        ft_set_border_style(table, FT_DOUBLE2_STYLE);

        /* Set "header" type for the first row */
        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        ft_write_ln(table, "Movie title", "Director", "Year", "Rating");

        ft_write_ln(table, "The Shawshank Redemption", "Frank Darabont", "1994", "9.5");
        ft_write_ln(table, "The Godfather", "Francis Ford Coppola", "1972", "9.2");
        ft_add_separator(table);

        ft_write_ln(table, "2001: A Space Odyssey", "Stanley Kubrick", "1968", "8.5");

        /* Set center alignment for the 1st and 3rd columns */
        ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
        ft_set_cell_prop(table, FT_ANY_ROW, 3, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);


        ft_table_t *table_copy = ft_copy_table(table);

        assert_true(table != NULL);
        const char *table_str = ft_to_string(table_copy);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "╔════════════════════════════╤════════════════════════╤════════╤══════════╗\n"
            "║                            │                        │        │          ║\n"
            "║  Movie title               │        Director        │  Year  │  Rating  ║\n"
            "║                            │                        │        │          ║\n"
            "╠════════════════════════════╪════════════════════════╪════════╪══════════╣\n"
            "║                            │                        │        │          ║\n"
            "║  The Shawshank Redemption  │     Frank Darabont     │  1994  │   9.5    ║\n"
            "║                            │                        │        │          ║\n"
            "╟────────────────────────────┼────────────────────────┼────────┼──────────╢\n"
            "║                            │                        │        │          ║\n"
            "║  The Godfather             │  Francis Ford Coppola  │  1972  │   9.2    ║\n"
            "║                            │                        │        │          ║\n"
            "╠════════════════════════════╪════════════════════════╪════════╪══════════╣\n"
            "║                            │                        │        │          ║\n"
            "║  2001: A Space Odyssey     │    Stanley Kubrick     │  1968  │   8.5    ║\n"
            "║                            │                        │        │          ║\n"
            "╚════════════════════════════╧════════════════════════╧════════╧══════════╝\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
        ft_destroy_table(table_copy);
    }
}



void test_table_changing_cell(void)
{
    ft_table_t *table = NULL;

    WHEN("All columns are equal and not empty") {
        table = ft_create_table();
        assert_true(table != NULL);
        assert_true(set_test_props_for_table(table) == FT_SUCCESS);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        assert_true(ft_write_ln(table, "3", "c", "234", "3.140000") == FT_SUCCESS);
        assert_true(ft_write_ln(table, "3", "c", "234", "3.140000") == FT_SUCCESS);
        assert_true(ft_write_ln(table, "3", "c", "234", "3.140000") == FT_SUCCESS);

        assert_true(ft_cur_row(table) == 3);
        assert_true(ft_cur_col(table) == 0);

        ft_set_cur_cell(table, 1, 1);
        assert_true(ft_cur_row(table) == 1);
        assert_true(ft_cur_col(table) == 1);
        assert_true(ft_write(table, "A") == FT_SUCCESS);
        assert_true(ft_cur_row(table) == 1);
        assert_true(ft_cur_col(table) == 2);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | A | 234 | 3.140000 |\n"
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
