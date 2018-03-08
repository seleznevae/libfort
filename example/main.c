#include <stdio.h>
#include "fort.h"
#include <wchar.h>
#include <locale.h>


int main()
{
    FTABLE *table = NULL;

#ifdef FORT_HAVE_WCHAR
    setlocale(LC_CTYPE, "");


    table = ft_create_table();
    ft_set_cell_option(table, FT_ANY_ROW, 0, FT_OPT_TEXT_ALIGN, CenterAligned);
    ft_set_cell_option(table, FT_ANY_ROW, 1, FT_OPT_TEXT_ALIGN, LeftAligned);

    ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
    FT_NWWRITE_LN(table, L"Rank", L"Title", L"Year", L"Rating");

    FT_NWWRITE_LN(table, L"1", L"Побег из Шоушенка", L"1994", L"9.5");
    FT_NWWRITE_LN(table, L"2", L"12 разгневанных мужчин", L"1957", L"8.8");
    FT_NWWRITE_LN(table, L"3", L"Космическая одиссея 2001 года", L"1968", L"8.5");
    FT_NWWRITE_LN(table, L"4", L"Бегущий по лезвию", L"1982", L"8.1");

    fwprintf(stderr, L"Table:\n%ls\n ", ft_to_wstring(table));

    ft_destroy_table(table);
#endif









    /*-------------------------------------------------------------*/
    table = ft_create_table();
    ft_set_cell_option(table, FT_ANY_ROW, 0, FT_OPT_TEXT_ALIGN, CenterAligned);
    ft_set_cell_option(table, FT_ANY_ROW, 1, FT_OPT_TEXT_ALIGN, LeftAligned);

//    ft_hdr_printf_ln(table, "#|Planet|Avg. speed");
    ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
    ft_printf_ln(table, "%d|%s|%5.2f km/s", 1, "Mercury", 47.362);

    ft_printf_ln(table, "%d|%s|%5.2f km/s", 1, "Mercury", 47.362);
    ft_printf_ln(table, "%d|%s|%5.2f km/s", 2, "Venus", 35.02);
    ft_printf_ln(table, "%d|%s|%5.2f km/s", 3, "Earth", 29.78);

    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    /*-------------------------------------------------------------*/

    table = ft_create_table();
    ft_set_cell_option(table, FT_ANY_ROW, 0, FT_OPT_TEXT_ALIGN, CenterAligned);
    ft_set_cell_option(table, FT_ANY_ROW, 1, FT_OPT_TEXT_ALIGN, LeftAligned);

//    ft_hdr_printf_ln(table, "Rank|Title|Year|Rating");
    ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
    ft_printf_ln(table, "Rank|Title|Year|Rating");

    FT_NWRITE_LN(table, "1", "The Shawshank Redemption", "1994", "9.5");
    FT_NWRITE_LN(table, "2", "12 Angry Men", "1957", "8.8");
    FT_NWRITE_LN(table, "3", "2001: A Space Odyssey", "1968", "8.5");
    FT_NWRITE_LN(table, "4", "Blade Runner", "1982", "8.1");


    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    /*-------------------------------------------------------------*/

    table = ft_create_table();
    ft_set_cell_option(table, FT_ANY_ROW, 0, FT_OPT_TEXT_ALIGN, LeftAligned);
    ft_set_cell_option(table, FT_ANY_ROW, 1, FT_OPT_TEXT_ALIGN, CenterAligned);

//    ft_hdr_printf_ln(table, "Commodity|Farm price|Avg. spread");
    ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
    ft_printf_ln(table, "Commodity|Farm price|Avg. spread");

    const char *row1[] = {"Potatoes", "$1.60", "200.94%"};
    const char *row2[] = {"Carrots", "$0.32 ", "190.63%"};
    ft_row_write_ln(table, 3, row1);
    ft_row_write_ln(table, 3, row2);

    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    /*-------------------------------------------------------------*/

    table = ft_create_table();
    ft_set_cell_option(table, FT_ANY_ROW, 0, FT_OPT_TEXT_ALIGN, LeftAligned);
    ft_set_cell_option(table, FT_ANY_ROW, 1, FT_OPT_TEXT_ALIGN, CenterAligned);

//    ft_hdr_printf_ln(table, "No.|Name|Avg. Mark");
    ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
    ft_printf_ln(table, "No.|Name|Avg. Mark");
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
    ft_set_cell_option(table, FT_ANY_ROW, 0, FT_OPT_TEXT_ALIGN, CenterAligned);
    ft_set_cell_option(table, FT_ANY_ROW, 1, FT_OPT_TEXT_ALIGN, LeftAligned);

//    ft_hdr_printf_ln(table, "No.|Name|Avg. Mark");
    ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
    ft_printf_ln(table, "No.|Name|Avg. Mark");
    const char **tab[2] = {
        row1,
        row2
    };
    ft_table_write_ln(table, 2, 3, tab);

    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    /*-------------------------------------------------------------*/




    table = NULL;
}
