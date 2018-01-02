#include "tests.h"
#include "fort.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "fort.c"

void test_table_geometry(void **state)
{
    (void)state;
    FTABLE *table = ft_create_table();

    int n = ft_hdr_printf(table, "%c", 'c');
    assert_true( n == 1 );

    size_t height = 0;
    size_t width = 0;
    int status = table_geometry(table, &height, &width);
    assert_true( IS_SUCCESS(status) );


    ft_destroy_table(table);
}

void test_table_basic(void **state)
{
    (void)state;
    FTABLE *table = ft_create_table();

    int n = ft_hdr_printf(table, "%d , %c|| %s|%f", 3, 'c', "234", 3.14);
    assert_true( n == 4 );

    n = ft_row_printf(table, 1, "%d , %c|| %s|%f", 3, 'c', "234", 3.14);
    assert_true( n == 4 );
    n = ft_row_printf(table, 2, "%d , %c|| %s|%f", 3, 'c', "234", 3.14);
    assert_true( n == 4 );
    n = ft_row_printf(table, 4, "%d , %c|| %s|%f", 3, 'c', "234", 3.14);
    assert_true( n == 4 );



    char *table_str = ft_to_string(table);
    const char *table_str_etalon =
            "| 3 , c| |  234| 3.140000\n"
            "| 3 , c| |  234| 3.140000\n"
            "| 3 , c| |  234| 3.140000\n"
            "\n"
            "| 3 , c| |  234| 3.140000\n";
    fprintf(stderr, "%s", table_str);

    assert_true( strcmp(table_str, table_str_etalon) == 0);

    free(table_str);

    ft_destroy_table(table);
}
