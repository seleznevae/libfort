#include "tests.h"
#include "fort.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
void test_table_basic(void **state)
{
    (void)state;
    FTABLE *table = ft_create_table();

    int n = ft_hdr_printf(table, "%d , %c|| %s|%f", 3, 'c', "234", 3.14);
    assert_true( n == 4 );

    n = ft_row_printf(table, 0, "%d , %c|| %s|%f", 3, 'c', "234", 3.14);
    assert_true( n == 4 );
    n = ft_row_printf(table, 1, "%d , %c|| %s|%f", 3, 'c', "234", 3.14);
    assert_true( n == 4 );


    char *table_str = ft_to_string(table);
    const char *table_str_etalon =
            "| 3 , c| |  234| 3.140000\n"
            "| 3 , c| |  234| 3.140000\n"
            "| 3 , c| |  234| 3.140000\n";

    assert_true( strcmp(table_str, table_str_etalon) == 0);

    free(table_str);

    ft_destroy_table(table);
}
