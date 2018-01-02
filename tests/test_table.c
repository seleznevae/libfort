#define FORT_EXTERN static
#include "tests.h"
#include "fort.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "fort.c"

void test_table_sizes(void **state)
{
    (void)state;
    FTABLE *table = ft_create_table();

    size_t rows = 0;
    size_t cols = 0;
    int status = F_SUCCESS;

    WHEN("Table is empty") {
        status = get_table_sizes(table, &rows, &cols);
        assert_true( IS_SUCCESS(status) );
        assert_true( rows == 0 );
        assert_true( cols == 0 );
    }

    WHEN("Insert one cell") {
        int n = ft_hdr_printf(table, "%c", 'c');
        assert_true( n == 1 );
        status = get_table_sizes(table, &rows, &cols);
        assert_true( IS_SUCCESS(status) );
        assert_true( rows == 1 );
        assert_true( cols == 1 );
    }

    WHEN("Insert two cells in the next row") {
        int n = ft_row_printf(table, 1, "%c|%c", 'c', 'd');
        assert_true( n == 2 );
        status = get_table_sizes(table, &rows, &cols);
        assert_true( IS_SUCCESS(status) );
        assert_true( rows == 2 );
        assert_true( cols == 2 );
    }

    WHEN("Insert five cells in the next row") {
        int n = ft_row_printf(table, 2, "%d|%d|%d|%d|%d", 1, 2, 3, 4, 5);
        assert_true( n == 5 );
        status = get_table_sizes(table, &rows, &cols);
        assert_true( IS_SUCCESS(status) );
        assert_true( rows == 3 );
        assert_true( cols == 5 );
    }

    ft_destroy_table(table);
}


void test_table_geometry(void **state)
{
    (void)state;
    FTABLE *table = ft_create_table();

    size_t height = 0;
    size_t width = 0;
    int status = F_SUCCESS;

    WHEN("Table is empty") {
        status = table_geometry(table, &height, &width);
        assert_true( IS_SUCCESS(status) );
        assert_true( height == 2 );
        assert_true( width == 3 );
    }

    WHEN("Table has one cell") {
        int n = ft_hdr_printf(table, "%c", 'c');
        assert_true( n == 1 );
        status = table_geometry(table, &height, &width);
        assert_true( IS_SUCCESS(status) );
        assert_true( height == 5 );
        assert_true( width == 6 );
    }

    WHEN("Inserting 3 cells in the next row") {
        int n = ft_row_printf(table, 1, "%c|%s|%c", 'c', "as", 'e');
        assert_true( n == 3 );
        status = table_geometry(table, &height, &width);
        assert_true( IS_SUCCESS(status) );
        assert_true( height == 9 );
        assert_true( width == 15 );
    }

    ft_destroy_table(table);
}

void test_table_basic(void **state)
{
    (void)state;
    FTABLE *table = ft_create_table();

    WHEN("All columns are equal and not empty") {
        int n = ft_hdr_printf(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true( n == 4 );
        n = ft_row_printf(table, 1, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true( n == 4 );
        n = ft_row_printf(table, 2, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true( n == 4 );

        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "==========================\n"
                "|   |   |     |          |\n"
                "| 3 | c | 234 | 3.140000 |\n"
                "|   |   |     |          |\n"
                "==========================\n"
                "|   |   |     |          |\n"
                "| 3 | c | 234 | 3.140000 |\n"
                "|   |   |     |          |\n"
                "==========================\n"
                "|   |   |     |          |\n"
                "| 3 | c | 234 | 3.140000 |\n"
                "|   |   |     |          |\n"
                "==========================\n";
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }



    WHEN("All columns are not equal and not empty") {
        table = ft_create_table();

        int n = ft_hdr_printf(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true( n == 4 );
        n = ft_row_printf(table, 1, "%c|%s|%f|%d", 'c', "234", 3.14, 3);
        assert_true( n == 4 );
        n = ft_row_printf(table, 2, "%s|%f|%d|%c", "234", 3.14,  3, 'c');
        assert_true( n == 4 );

        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "========================================\n"
                "|     |          |          |          |\n"
                "|   3 |        c |      234 | 3.140000 |\n"
                "|     |          |          |          |\n"
                "========================================\n"
                "|     |          |          |          |\n"
                "|   c |      234 | 3.140000 |        3 |\n"
                "|     |          |          |          |\n"
                "========================================\n"
                "|     |          |          |          |\n"
                "| 234 | 3.140000 |        3 |        c |\n"
                "|     |          |          |          |\n"
                "========================================\n";

//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }

    WHEN("All columns are not equal and some cells are empty") {
        table = ft_create_table();

        int n = ft_hdr_printf(table, "||%s|%f", "234", 3.14);
        assert_true( n == 4 );
        n = ft_row_printf(table, 1, "%c|%s|%f", 'c', "234", 3.14);
        assert_true( n == 3 );
        n = ft_row_printf(table, 2, "%s|%f||%c", "234", 3.14, 'c');
        assert_true( n == 4 );

        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "========================================\n"
                "|     |          |          |          |\n"
                "|     |          |      234 | 3.140000 |\n"
                "|     |          |          |          |\n"
                "========================================\n"
                "|     |          |          |          |\n"
                "|   c |      234 | 3.140000 |          |\n"
                "|     |          |          |          |\n"
                "========================================\n"
                "|     |          |          |          |\n"
                "| 234 | 3.140000 |          |        c |\n"
                "|     |          |          |          |\n"
                "========================================\n";

//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }


}
