//#define FT_EXTERN static
#include "tests.h"
#include "fort.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include "fort.c"
#include "table.h"
#include "options.h"
#include "vector.h"


int set_test_options_for_table(FTABLE *table)
{
    assert(table);
    int status = F_SUCCESS;
    status |= ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_BOTTOM_PADDING, 1);
    status |= ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_TOP_PADDING, 1);
    status |= ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_LEFT_PADDING, 1);
    status |= ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_RIGHT_PADDING, 1);
    status |= ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_EMPTY_STR_HEIGHT, 1);

    assert_true( status == F_SUCCESS );


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
    return ft_set_table_borders(table, &brdr_style);
}

int set_test_options_as_default()
{
    int status = F_SUCCESS;

    status |= ft_set_default_cell_option(FT_OPT_MIN_WIDTH, 0);
    status |= ft_set_default_cell_option(FT_OPT_TEXT_ALIGN, RightAligned);

    status |= ft_set_default_cell_option(FT_OPT_BOTTOM_PADDING, 1);
    status |= ft_set_default_cell_option(FT_OPT_TOP_PADDING, 1);
    status |= ft_set_default_cell_option(FT_OPT_LEFT_PADDING, 1);
    status |= ft_set_default_cell_option(FT_OPT_RIGHT_PADDING, 1);
    status |= ft_set_default_cell_option(FT_OPT_EMPTY_STR_HEIGHT, 1);

    assert_true( status == F_SUCCESS );


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

    return ft_set_default_borders(&brdr_style);
}

void test_table_sizes(void **state)
{
    (void)state;
    FTABLE *table = ft_create_table();
    assert_true( table != NULL );
//    ft_set_table_options(table, &test_table_opts);

    assert_true( set_test_options_for_table(table) == F_SUCCESS);


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
        int n = ft_printf_ln(table, "%c", 'c');
        assert_true( n == 1 );
        status = get_table_sizes(table, &rows, &cols);
        assert_true( IS_SUCCESS(status) );
        assert_true( rows == 1 );
        assert_true( cols == 1 );
    }

    WHEN("Insert two cells in the next row") {
        int n = ft_printf_ln(table, "%c|%c", 'c', 'd');
        assert_true( n == 2 );
        status = get_table_sizes(table, &rows, &cols);
        assert_true( IS_SUCCESS(status) );
        assert_true( rows == 2 );
        assert_true( cols == 2 );
    }

    WHEN("Insert five cells in the next row") {
        int n = ft_printf_ln(table, "%d|%d|%d|%d|%d", 1, 2, 3, 4, 5);
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
    assert_true( table != NULL );
//    ft_set_table_options(table, &test_table_opts);
    assert_true( set_test_options_for_table(table) == F_SUCCESS);

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
        int n = ft_printf_ln(table, "%c", 'c');
        assert_true( n == 1 );
        status = table_geometry(table, &height, &width);
        assert_true( IS_SUCCESS(status) );
        assert_true( height == 5 );
        assert_true( width == 6 );
    }

    WHEN("Inserting 3 cells in the next row") {
        int n = ft_printf_ln(table, "%c|%s|%c", 'c', "as", 'e');
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

    FTABLE *table = NULL;

    WHEN("All columns are equal and not empty") {
        table = ft_create_table();
        assert_true( table != NULL );
//        ft_set_table_options(table, &test_table_opts);
        assert_true( set_test_options_for_table(table) == F_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
        int n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);

        assert_true( n == 4 );
        n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true( n == 4 );
        n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true( n == 4 );

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
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
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }



    WHEN("All columns are not equal and not empty") {
        table = ft_create_table();
        assert_true( table != NULL );
//        ft_set_table_options(table, &test_table_opts);
        assert_true( set_test_options_for_table(table) == F_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
        int n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);

        assert_true( n == 4 );
        n = ft_printf_ln(table, "%c|%s|%f|%d", 'c', "234", 3.14, 3);
        assert_true( n == 4 );
        n = ft_printf_ln(table, "%s|%f|%d|%c", "234", 3.14,  3, 'c');
        assert_true( n == 4 );

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
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

//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("All columns are not equal and some cells are empty") {
        table = ft_create_table();
        assert_true( table != NULL );
//        ft_set_table_options(table, &test_table_opts);
        assert_true( set_test_options_for_table(table) == F_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
        int n = ft_printf_ln(table, "||%s|%f", "234", 3.14);

        assert_true( n == 4 );
        n = ft_printf_ln(table, "%c|%s|%f", 'c', "234", 3.14);
        assert_true( n == 3 );
        n = ft_printf_ln(table, "%s|%f||", "234", 3.14);
        assert_true( n == 4 );

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
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

//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("All cells are empty") {
        table = ft_create_table();
        assert_true( table != NULL );
//        ft_set_table_options(table, &test_table_opts);
        assert_true( set_test_options_for_table(table) == F_SUCCESS);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
        int n = ft_printf_ln(table, "|||");

        assert_true( n == 4 );
        n = ft_printf_ln(table, "|||");
        assert_true( n == 4 );
        n = ft_printf_ln(table, "|||");
        assert_true( n == 4 );

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
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


//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }
}



FTABLE *create_test_int_table(int set_test_opts)
{
    FTABLE *table = NULL;

    table = ft_create_table();
    assert_true( table != NULL );
    if (set_test_opts) {
        assert_true( set_test_options_for_table(table) == F_SUCCESS);
    }
//        ft_set_table_options(table, &test_table_opts);

    assert_true (table != NULL);

    ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
    int n = ft_printf_ln(table, "%d|%d|%d|%d", 3, 4, 55, 67);

    assert_true( n == 4 );

    assert(ft_write(table, "3") == F_SUCCESS);
    assert(ft_write(table, "4") == F_SUCCESS);
    assert(ft_write(table, "55") == F_SUCCESS);
    assert(ft_write_ln(table, "67") == F_SUCCESS);

    assert(ft_write(table, "3") == F_SUCCESS);
    assert(ft_write(table, "4") == F_SUCCESS);
    assert(ft_write(table, "55") == F_SUCCESS);
    assert(ft_write_ln(table, "67") == F_SUCCESS);

    return table;
}

void test_table_options(void **state)
{
    (void)state;
    FTABLE *table = NULL;

//    fort_table_options_t def_options =;
//    ft_get_default_options(&def_options);


    WHEN("All paddings = 1") {
        set_test_options_as_default();

        ft_set_default_cell_option(FT_OPT_BOTTOM_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_TOP_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_LEFT_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_RIGHT_PADDING, 1);

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
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("Separator testing") {
        table = create_test_int_table(1);
        ft_add_separator(table);

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
        int n = ft_printf_ln(table, "%d|%d|%d|%d", 3, 4, 55, 67);

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
                "+===+===+====+====+\n"
                "|   |   |    |    |\n"
                "| 3 | 4 | 55 | 67 |\n"
                "|   |   |    |    |\n"
                "+---+---+----+----+\n";
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("Top and bottom padding = 0") {

        ft_set_default_cell_option(FT_OPT_BOTTOM_PADDING, 0);
        ft_set_default_cell_option(FT_OPT_TOP_PADDING, 0);
        ft_set_default_cell_option(FT_OPT_LEFT_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_RIGHT_PADDING, 1);

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
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("Left and right padding = 0") {

        ft_set_default_cell_option(FT_OPT_BOTTOM_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_TOP_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_LEFT_PADDING, 0);
        ft_set_default_cell_option(FT_OPT_RIGHT_PADDING, 0);

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
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("All paddings = 0") {

        ft_set_default_cell_option(FT_OPT_BOTTOM_PADDING, 0);
        ft_set_default_cell_option(FT_OPT_TOP_PADDING, 0);
        ft_set_default_cell_option(FT_OPT_LEFT_PADDING, 0);
        ft_set_default_cell_option(FT_OPT_RIGHT_PADDING, 0);

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
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("Empty string has 0 heigt") {

        ft_set_default_cell_option(FT_OPT_BOTTOM_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_TOP_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_LEFT_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_RIGHT_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_EMPTY_STR_HEIGHT, 0);

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
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("Changing cell separators") {

        struct ft_border_style brdr_style;
        brdr_style.border_chs.top_border_ch = '|';
        brdr_style.border_chs.separator_ch = '|';
        brdr_style.border_chs.bottom_border_ch = '|';
        brdr_style.border_chs.side_border_ch = '=';
        brdr_style.border_chs.out_intersect_ch = '+';
        brdr_style.border_chs.in_intersect_ch = '#';

        brdr_style.header_border_chs.top_border_ch = '*';
        brdr_style.header_border_chs.separator_ch = '*';
        brdr_style.header_border_chs.bottom_border_ch = '*';
        brdr_style.header_border_chs.side_border_ch = 'v';
        brdr_style.header_border_chs.out_intersect_ch = '+';
        brdr_style.header_border_chs.in_intersect_ch = '#';
        ft_set_default_borders(&brdr_style);



        table = create_test_int_table(0);
        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
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
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);



        brdr_style.border_chs.top_border_ch = '|';
        brdr_style.border_chs.separator_ch = '\0';
        brdr_style.border_chs.bottom_border_ch = '|';
        brdr_style.border_chs.side_border_ch = '=';
        brdr_style.border_chs.out_intersect_ch = '+';
        brdr_style.border_chs.in_intersect_ch = '\0';

        brdr_style.header_border_chs.top_border_ch = '*';
        brdr_style.header_border_chs.separator_ch = '*';
        brdr_style.header_border_chs.bottom_border_ch = '*';
        brdr_style.header_border_chs.side_border_ch = 'v';
        brdr_style.header_border_chs.out_intersect_ch = '+';
        brdr_style.header_border_chs.in_intersect_ch = '#';

        ft_set_default_borders(&brdr_style);

        ft_set_default_cell_option(FT_OPT_BOTTOM_PADDING, 0);
        ft_set_default_cell_option(FT_OPT_TOP_PADDING, 0);
        ft_set_default_cell_option(FT_OPT_LEFT_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_RIGHT_PADDING, 1);
        ft_set_default_cell_option(FT_OPT_EMPTY_STR_HEIGHT, 0);


        table = create_test_int_table(0);
        table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        table_str_etalon =
                "+***+***+****+****+\n"
                "v 3 v 4 v 55 v 67 v\n"
                "+***#***#****#****+\n"
                "= 3 = 4 = 55 = 67 =\n"
                "= 3 = 4 = 55 = 67 =\n"
                "+|||+|||+||||+||||+\n";
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("Setting options for a particular table") {

        table = create_test_int_table(0);
        set_test_options_for_table(table);

        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_BOTTOM_PADDING, 0);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_TOP_PADDING, 0);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_LEFT_PADDING, 0);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_RIGHT_PADDING, 0);

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
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);


        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_BOTTOM_PADDING, 1);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_TOP_PADDING, 1);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_LEFT_PADDING, 0);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_RIGHT_PADDING, 0);
        ft_set_cell_option(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_OPT_EMPTY_STR_HEIGHT, 0);

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
//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }



    WHEN("Set table width and column alignment") {

        set_test_options_as_default();

        table = create_test_int_table(0);
        int status = F_SUCCESS;

        status |= ft_set_cell_option(table, FT_ANY_ROW, 1, FT_OPT_MIN_WIDTH, 7);
        status |= ft_set_cell_option(table, FT_ANY_ROW, 1, FT_OPT_TEXT_ALIGN, LeftAligned);
        status |= ft_set_cell_option(table, FT_ANY_ROW, 2, FT_OPT_MIN_WIDTH, 8);
        status |= ft_set_cell_option(table, FT_ANY_ROW, 2, FT_OPT_TEXT_ALIGN, CenterAligned);

        status |= ft_set_cell_option(table, 2, 3, FT_OPT_MIN_WIDTH, 6);
        status |= ft_set_cell_option(table, 2, 3, FT_OPT_TEXT_ALIGN, LeftAligned);
        assert_true( status == F_SUCCESS);


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
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("Set table width and column alignment as default") {

        set_test_options_as_default();

        int status = F_SUCCESS;
        status |= ft_set_default_cell_option(FT_OPT_MIN_WIDTH, 5);
        status |= ft_set_default_cell_option(FT_OPT_TEXT_ALIGN, CenterAligned);
        assert_true( status == F_SUCCESS);

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
        //        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("All columns are equal and not empty") {
        set_test_options_as_default();

        table = ft_create_table();

        ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
        int n = ft_printf_ln(table, "%d|%c|%s|%f", 4, 'c', "234", 3.14);

        assert_true( n == 4 );
        n = FT_NWRITE_LN(table, "5", "c", "234\n12", "3.140000");
        assert_true( n == F_SUCCESS );
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
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

}
