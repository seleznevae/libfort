#include <stdio.h>
#include "fort.h"


int main()
{
    FTABLE *table = ft_create_table();

    ft_hdr_printf_ln(table, "%d , %c|| %s|%f", 3, 'c', "234", 3.14);

    const char *row[] = {"AAA", " ADf qwer", "qwerwqer", "11111 23333", "qwe"};

    ft_row_write_ln(table, 5, row);
    ft_row_write(table, 5, row);
    ft_row_write(table, 5, row);
    ft_ln(table);



    const char *ctab[2][2] = {
        {"AAA", " ADf qwer"},
        {"AAA", " ADf 2222"}
    };
    ft_s_table_write_ln(table, 2, 2, ctab);


    const char **tab[2] = {
        row,
        row
    };
    ft_table_write(table, 2, 5, tab);
    ft_ln(table);

    ft_nwrite(table, 3, "123", "2345", "4567");
    ft_ln(table);

    FT_NWRITE(table, "123", "345", "Anton", "Petr", "Pavel");

    fprintf(stderr, "Table:\n");
    fprintf(stderr, "%s\n", ft_to_string(table));

//    printf("Hello, world!\n");
//    char buffer[3] = {'a', 'b', 'c'};
//    int ret = snprintf(buffer, 3, "so");
//    printf("result is %s = lent = %d\n", buffer, ret);
//    fprintf(stderr, "asd %d|", 3);
//    fprintf(stderr, "%d\n", 3.14);
    ft_destroy_table(table);
}
