#include <stdio.h>
#include "fort.h"


int main()
{
    FTABLE *table = ft_create_table();
    ft_set_column_alignment(table, 0, CenterAligned);
    ft_set_column_alignment(table, 1, LeftAligned);
    FT_HDR_PRINTF_LN(table, "#|Planet|Avg. speed");
    FT_PRINTF_LN(table, "%d|%s|%5.2f km/s", 1, "Mercury", 47.362);
    FT_PRINTF_LN(table, "%d|%s|%5.2f km/s", 2, "Venus", 35.02);
    FT_PRINTF_LN(table, "%d|%s|%5.2f km/s", 3, "Earth", 29.78);

    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    /*-------------------------------------------------------------*/

    table = ft_create_table();
    ft_set_column_alignment(table, 0, CenterAligned);
    ft_set_column_alignment(table, 1, LeftAligned);
    FT_HDR_PRINTF_LN(table, "Rank|Title|Year|Rating");

    FT_NWRITE_LN(table, "1", "The Shawshank Redemption", "1994", "9.5");
    FT_NWRITE_LN(table, "2", "12 Angry Men", "1957", "8.8");
    FT_NWRITE_LN(table, "3", "2001: A Space Odyssey", "1968", "8.5");
    FT_NWRITE_LN(table, "4", "Blade Runner", "1982", "8.1");


    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    /*-------------------------------------------------------------*/

    table = ft_create_table();
    ft_set_column_alignment(table, 0, LeftAligned);
    ft_set_column_alignment(table, 1, CenterAligned);
    FT_HDR_PRINTF_LN(table, "Commodity|Farm price|Avg. spread");

    const char *row1[] = {"Potatoes", "$1.60", "200.94%"};
    const char *row2[] = {"Carrots", "$0.32 ", "190.63%"};
    ft_row_write_ln(table, 3, row1);
    ft_row_write_ln(table, 3, row2);

    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    /*-------------------------------------------------------------*/

    table = ft_create_table();
    ft_set_column_alignment(table, 0, CenterAligned);
    ft_set_column_alignment(table, 1, LeftAligned);
    FT_HDR_PRINTF_LN(table, "No.|Name|Avg. Mark");

    const char *ctab[2][3] = {
            {"1", "Joe Public", "3.14"},
            {"2", "John Doe", "4.50"}
        };
    ft_s_table_write_ln(table, 2, 3, ctab);

    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    /*-------------------------------------------------------------*/

    table = ft_create_table();
    ft_set_column_alignment(table, 0, CenterAligned);
    ft_set_column_alignment(table, 1, LeftAligned);
    FT_HDR_PRINTF_LN(table, "No.|Name|Avg. Mark");

    const char **tab[2] = {
        row1,
        row2
    };
    ft_table_write_ln(table, 2, 3, tab);

    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);
    table = NULL;

//    FTABLE *table = ft_create_table();

//    ft_set_column_alignment(table, 2, LeftAligned);

//    ft_hdr_printf_ln(table, "%d , %c|| %s|%f", 3, 'c', "234", 3.14);

//    const char *row[] = {"AAA", " ADf qwer", "qwerwqer", "11111 23333", "qwe"};

//    ft_row_write_ln(table, 5, row);
//    ft_row_write(table, 5, row);
//    ft_row_write(table, 5, row);
//    ft_ln(table);



//    const char *ctab[2][2] = {
//        {"AAA", " ADf qwer"},
//        {"AAA", " ADf 2222"}
//    };
//    ft_s_table_write_ln(table, 2, 2, ctab);


//    const char **tab[2] = {
//        row,
//        row
//    };
//    ft_table_write(table, 2, 5, tab);
//    ft_ln(table);

//    ft_nwrite(table, 3, "123", "2345", "4567");
//    ft_ln(table);

//    FT_NWRITE_LN(table, "123", "132445\n123\n123", "Anton", "Petr", "Pavel");

//    fprintf(stderr, "Table:\n");
//    fprintf(stderr, "%s\n", ft_to_string(table));

//    ft_destroy_table(table);
}
