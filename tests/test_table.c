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
        n = ft_row_printf(table, 2, "%s|%f||", "234", 3.14);
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
                "| 234 | 3.140000 |          |          |\n"
                "|     |          |          |          |\n"
                "========================================\n";

//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }

    WHEN("All cells are empty") {
        table = ft_create_table();

        int n = ft_hdr_printf(table, "|||");
        assert_true( n == 4 );
        n = ft_row_printf(table, 1, "|||");
        assert_true( n == 4 );
        n = ft_row_printf(table, 2, "|||");
        assert_true( n == 4 );

        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "=============\n"
                "|  |  |  |  |\n"
                "|  |  |  |  |\n"
                "|  |  |  |  |\n"
                "=============\n"
                "|  |  |  |  |\n"
                "|  |  |  |  |\n"
                "|  |  |  |  |\n"
                "=============\n"
                "|  |  |  |  |\n"
                "|  |  |  |  |\n"
                "|  |  |  |  |\n"
                "=============\n";


//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }
}



FTABLE *create_test_int_table()
{
    FTABLE *table = NULL;

    table = ft_create_table();

    assert_true (table != NULL);

    int n = ft_hdr_printf(table, "%d|%d|%d|%d", 3, 4, 55, 67);
    assert_true( n == 4 );
    n = ft_row_printf(table, 1, "%d|%d|%d|%d", 3, 4, 55, 67);
    assert_true( n == 4 );
    n = ft_row_printf(table, 2, "%d|%d|%d|%d", 3, 4, 55, 67);
    assert_true( n == 4 );

    return table;
}

void test_table_options(void **state)
{
    (void)state;
    FTABLE *table = NULL;

    fort_table_options_t def_options;
    ft_get_default_options(&def_options);


    WHEN("All paddings = 1") {
        fort_table_options_t table_options;
        memcpy(&table_options, &def_options, sizeof(fort_table_options_t));
        table_options.cell_padding_bottom = 1;
        table_options.cell_padding_top = 1;
        table_options.cell_padding_left = 1;
        table_options.cell_padding_right = 1;
        ft_set_default_options(&table_options);

        table = create_test_int_table();

        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "===================\n"
                "|   |   |    |    |\n"
                "| 3 | 4 | 55 | 67 |\n"
                "|   |   |    |    |\n"
                "===================\n"
                "|   |   |    |    |\n"
                "| 3 | 4 | 55 | 67 |\n"
                "|   |   |    |    |\n"
                "===================\n"
                "|   |   |    |    |\n"
                "| 3 | 4 | 55 | 67 |\n"
                "|   |   |    |    |\n"
                "===================\n";
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }

    WHEN("Top and bottom padding = 0") {
        fort_table_options_t table_options;
        memcpy(&table_options, &def_options, sizeof(fort_table_options_t));
        table_options.cell_padding_bottom = 0;
        table_options.cell_padding_top = 0;
        table_options.cell_padding_left = 1;
        table_options.cell_padding_right = 1;
        ft_set_default_options(&table_options);

        table = create_test_int_table();

        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "===================\n"
                "| 3 | 4 | 55 | 67 |\n"
                "===================\n"
                "| 3 | 4 | 55 | 67 |\n"
                "===================\n"
                "| 3 | 4 | 55 | 67 |\n"
                "===================\n";
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }

    WHEN("Left and right padding = 0") {
        fort_table_options_t table_options;
        memcpy(&table_options, &def_options, sizeof(fort_table_options_t));
        table_options.cell_padding_bottom = 1;
        table_options.cell_padding_top = 1;
        table_options.cell_padding_left = 0;
        table_options.cell_padding_right = 0;
        ft_set_default_options(&table_options);

        table = create_test_int_table();

        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "===========\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "===========\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "===========\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "===========\n";
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }

    WHEN("All paddings = 0") {
        fort_table_options_t table_options;
        memcpy(&table_options, &def_options, sizeof(fort_table_options_t));
        table_options.cell_padding_bottom = 0;
        table_options.cell_padding_top = 0;
        table_options.cell_padding_left = 0;
        table_options.cell_padding_right = 0;
        ft_set_default_options(&table_options);

        table = create_test_int_table();

        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "===========\n"
                "|3|4|55|67|\n"
                "===========\n"
                "|3|4|55|67|\n"
                "===========\n"
                "|3|4|55|67|\n"
                "===========\n";
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }

    WHEN("Empty string has 0 heigt") {
        fort_table_options_t table_options;
        memcpy(&table_options, &def_options, sizeof(fort_table_options_t));
        table_options.cell_padding_bottom = 1;
        table_options.cell_padding_top = 1;
        table_options.cell_padding_left = 1;
        table_options.cell_padding_right = 1;
        table_options.cell_empty_string_height = 0;
        ft_set_default_options(&table_options);

        table = create_test_int_table();
        int n = ft_row_printf(table, 3, "|||");
        assert_true( n == 4 );

        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "===================\n"
                "|   |   |    |    |\n"
                "| 3 | 4 | 55 | 67 |\n"
                "|   |   |    |    |\n"
                "===================\n"
                "|   |   |    |    |\n"
                "| 3 | 4 | 55 | 67 |\n"
                "|   |   |    |    |\n"
                "===================\n"
                "|   |   |    |    |\n"
                "| 3 | 4 | 55 | 67 |\n"
                "|   |   |    |    |\n"
                "===================\n"
                "|   |   |    |    |\n"
                "|   |   |    |    |\n"
                "===================\n";
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }

    WHEN("Changing cell separators") {
        fort_table_options_t table_options;
        memcpy(&table_options, &def_options, sizeof(fort_table_options_t));

#define BOR_CHARS table_options.border_chars
#define H_BOR_CHARS table_options.header_border_chars

        BOR_CHARS[TL_bip] = BOR_CHARS[TT_bip] = BOR_CHARS[TV_bip] = BOR_CHARS[TR_bip] = '|';
        BOR_CHARS[LH_bip] = BOR_CHARS[IH_bip] = BOR_CHARS[II_bip] = BOR_CHARS[RH_bip] = '|';
        BOR_CHARS[BL_bip] = BOR_CHARS[BB_bip] = BOR_CHARS[BV_bip] = BOR_CHARS[BR_bip] = '|';
        BOR_CHARS[LL_bip] = BOR_CHARS[IV_bip] = BOR_CHARS[RR_bip] = '=';


        H_BOR_CHARS[TL_bip] = H_BOR_CHARS[TT_bip] = H_BOR_CHARS[TV_bip] = H_BOR_CHARS[TR_bip] = '*';
        H_BOR_CHARS[LH_bip] = H_BOR_CHARS[IH_bip] = H_BOR_CHARS[II_bip] = H_BOR_CHARS[RH_bip] = '*';
        H_BOR_CHARS[BL_bip] = H_BOR_CHARS[BB_bip] = H_BOR_CHARS[BV_bip] = H_BOR_CHARS[BR_bip] = '*';
        H_BOR_CHARS[LL_bip] = H_BOR_CHARS[IV_bip] = H_BOR_CHARS[RR_bip] = 'v';

#undef BOR_CHARS
#undef H_BOR_CHARS

        ft_set_default_options(&table_options);

        table = create_test_int_table();


        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "*******************\n"
                "v   v   v    v    v\n"
                "v 3 v 4 v 55 v 67 v\n"
                "v   v   v    v    v\n"
                "*******************\n"
                "=   =   =    =    =\n"
                "= 3 = 4 = 55 = 67 =\n"
                "=   =   =    =    =\n"
                "|||||||||||||||||||\n"
                "=   =   =    =    =\n"
                "= 3 = 4 = 55 = 67 =\n"
                "=   =   =    =    =\n"
                "|||||||||||||||||||\n";
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }

    WHEN("Setting options for a particular table") {
        fort_table_options_t table_options;
        memcpy(&table_options, &def_options, sizeof(fort_table_options_t));
        table_options.cell_padding_bottom = 0;
        table_options.cell_padding_top = 0;
        table_options.cell_padding_left = 0;
        table_options.cell_padding_right = 0;
        ft_set_default_options(&table_options);

        table = create_test_int_table();

        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "===========\n"
                "|3|4|55|67|\n"
                "===========\n"
                "|3|4|55|67|\n"
                "===========\n"
                "|3|4|55|67|\n"
                "===========\n";
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);
        free(table_str);

        table_options.cell_padding_bottom = 1;
        table_options.cell_padding_top = 1;
        table_options.cell_padding_left = 0;
        table_options.cell_padding_right = 0;
        ft_set_table_options(table, &table_options);
        table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        table_str_etalon =
                "===========\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "===========\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "===========\n"
                "| | |  |  |\n"
                "|3|4|55|67|\n"
                "| | |  |  |\n"
                "===========\n";
//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }



    WHEN("Set table width and column alignment") {
        fort_table_options_t table_options;
        memcpy(&table_options, &def_options, sizeof(fort_table_options_t));
        table_options.cell_padding_bottom = 1;
        table_options.cell_padding_top = 1;
        table_options.cell_padding_left = 1;
        table_options.cell_padding_right = 1;
        ft_set_default_options(&table_options);


        table = create_test_int_table();
        ft_set_column_min_width(table, 1, 7);
        ft_set_column_alignment(table, 1, LeftAligned);

        char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "=======================\n"
                "|   |       |    |    |\n"
                "| 3 | 4     | 55 | 67 |\n"
                "|   |       |    |    |\n"
                "=======================\n"
                "|   |       |    |    |\n"
                "| 3 | 4     | 55 | 67 |\n"
                "|   |       |    |    |\n"
                "=======================\n"
                "|   |       |    |    |\n"
                "| 3 | 4     | 55 | 67 |\n"
                "|   |       |    |    |\n"
                "=======================\n";
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        free(table_str);
        ft_destroy_table(table);
    }
}
