#include <stdio.h>
#include "fort.h"


int main()
{
    FTABLE *table = ft_create_table();

    ft_set_column_alignment(table, 2, LeftAligned);

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

    FT_NWRITE_LN(table, "123", "132445\n123\n123", "Anton", "Petr", "Pavel");

    fprintf(stderr, "Table:\n");
    fprintf(stderr, "%s\n", ft_to_string(table));

    ft_destroy_table(table);
}
