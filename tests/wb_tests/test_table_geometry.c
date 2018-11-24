#include "tests.h"
#include "table.h"
#include "test_utils.h"

void test_table_sizes(void)
{
    ft_table_t *table = ft_create_table();
    assert_true(table != NULL);
    assert_true(set_test_props_for_table(table) == FT_SUCCESS);


    size_t rows = 0;
    size_t cols = 0;
    int status = FT_SUCCESS;

    WHEN("Table is empty") {
        status = get_table_sizes(table, &rows, &cols);
        assert_true(FT_IS_SUCCESS(status));
        assert_true(rows == 0);
        assert_true(cols == 0);
    }

    WHEN("Insert one cell") {
        int n = ft_printf_ln(table, "%c", 'c');
        assert_true(n == 1);
        status = get_table_sizes(table, &rows, &cols);
        assert_true(FT_IS_SUCCESS(status));
        assert_true(rows == 1);
        assert_true(cols == 1);
    }

    WHEN("Insert two cells in the next row") {
        int n = ft_printf_ln(table, "%c|%c", 'c', 'd');
        assert_true(n == 2);
        status = get_table_sizes(table, &rows, &cols);
        assert_true(FT_IS_SUCCESS(status));
        assert_true(rows == 2);
        assert_true(cols == 2);
    }

    WHEN("Insert five cells in the next row") {
        int n = ft_printf_ln(table, "%d|%d|%d|%d|%d", 1, 2, 3, 4, 5);
        assert_true(n == 5);
        status = get_table_sizes(table, &rows, &cols);
        assert_true(FT_IS_SUCCESS(status));
        assert_true(rows == 3);
        assert_true(cols == 5);
    }

    ft_destroy_table(table);
}


void test_table_geometry(void)
{
    ft_table_t *table = ft_create_table();
    assert_true(table != NULL);
    assert_true(set_test_props_for_table(table) == FT_SUCCESS);

    size_t height = 0;
    size_t width = 0;
    int status = FT_SUCCESS;

    WHEN("Table is empty") {
        status = table_geometry(table, &height, &width);
        assert_true(FT_IS_SUCCESS(status));
        assert_true(height == 2);
        assert_true(width == 3);
    }

    WHEN("Table has one cell") {
        int n = ft_printf_ln(table, "%c", 'c');
        assert_true(n == 1);
        status = table_geometry(table, &height, &width);
        assert_true(FT_IS_SUCCESS(status));
        assert_true(height == 5);
        assert_true(width == 6);
    }

    WHEN("Inserting 3 cells in the next row") {
        int n = ft_printf_ln(table, "%c|%s|%c", 'c', "as", 'e');
        assert_true(n == 3);
        status = table_geometry(table, &height, &width);
        assert_true(FT_IS_SUCCESS(status));
        assert_true(height == 9);
        assert_true(width == 15);
    }

    ft_destroy_table(table);
}
