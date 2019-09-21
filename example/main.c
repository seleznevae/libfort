#include <stdio.h>
#include "fort.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>

void fill_table_with_data_example(void)
{
    ft_table_t *table = ft_create_table();
    /* Set "header" type for the first row */
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "N", "Planet", "Speed, km/s", "Temperature, K");

    /* Fill row with printf like function */
    ft_printf_ln(table, "1|%s|%6.3f|%d", "Mercury", 47.362, 340);

    /* Fill row explicitly with strings */
    ft_write_ln(table, "2", "Venus", "35.02", "737");

    /* Fill row with the array of strings */
    const char *arr[4] = {"3", "Earth", "29.78", "288"};
    ft_row_write_ln(table, 4, arr);

    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);
}



int main(void)
{
    fill_table_with_data_example();

    int result = 0;

    ft_table_t *table = NULL;

    table = ft_create_table();
    ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
    ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_LEFT);

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_printf_ln(table, "%d|%s|%5.2f km/s", 1, "Mercury", 47.362);

    ft_printf_ln(table, "%d|%s|%5.2f km/s", 1, "Mercury", 47.362);
    ft_printf_ln(table, "%d|%s|%5.2f km/s", 2, "Venus", 35.02);
    ft_printf_ln(table, "%d|%s|%5.2f km/s", 3, "Earth", 29.78);

    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    /*-------------------------------------------------------------*/

    table = ft_create_table();
    ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
    ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_LEFT);

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "Rank", "Title", "Year", "Rating");

    ft_write_ln(table, "1", "The Shawshank Redemption", "1994", "9.5");
    ft_write_ln(table, "2", "12 Angry Men", "1957", "8.8");
    ft_write_ln(table, "3", "2001: A Space Odyssey", "1968", "8.5");
    ft_write_ln(table, "4", "Blade Runner", "1982", "8.1");


    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);

    /*-------------------------------------------------------------*/

    table = ft_create_table();
    ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_LEFT);
    ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
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
    ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_LEFT);
    ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_printf_ln(table, "No.|Name|Avg. Mark");
    const char *ctab[2][3] = {
            {"1", "Joe Public", "3.14"},
            {"2", "John Doe", "4.50"}
        };
    ft_table_write_ln(table, 2, 3, (const char **)ctab);

    printf("Table:\n");
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);


    fflush(stdout);
    /*-------------------------------------------------------------*/
#if defined(FT_HAVE_WCHAR) && !defined(FT_MICROSOFT_COMPILER)
    setlocale(LC_CTYPE, "");

    ft_set_default_border_style(FT_BASIC_STYLE);

    table = ft_create_table();
    ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
    ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_LEFT);

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_wwrite_ln(table, L"Ранг", L"Название", L"Год", L"Рейтинг");

    ft_wwrite_ln(table, L"1", L"Побег из Шоушенка", L"1994", L"9.5");
    ft_wwrite_ln(table, L"2", L"12 разгневанных мужчин", L"1957", L"8.8");
    ft_wwrite_ln(table, L"3", L"Космическая одиссея 2001 года", L"1968", L"8.5");
    ft_wwrite_ln(table, L"4", L"Бегущий по лезвию", L"1982", L"8.1");

    /* Ранг | Название | Год | Рейтинг */
    /*FT_NWWRITE_LN(table, L"\x420\x430\x43d\x433", L"\x41d\x430\x437\x432\x430\x43d\x438\x435", L"\x413\x43e\x434", L"\x420\x435\x439\x442\x438\x43d\x433"); */

    /*FT_NWWRITE_LN(table, L"1", L"\x41f\x43e\x431\x435\x433 \x438\x437 \x428\x43e\x443\x448\x435\x43d\x43a\x430", L"1994", L"9.5");*/ /* Побег из Шоушенка */
    /*FT_NWWRITE_LN(table, L"2", L"12 \x440\x430\x437\x433\x43d\x435\x432\x430\x43d\x43d\x44b\x445 \x43c\x443\x436\x447\x438\x43d", L"1957", L"8.8");*/ /* 12 разгневанных мужчин */
    /*FT_NWWRITE_LN(table, L"3", L"\x41a\x43e\x441\x43c\x438\x447\x435\x441\x43a\x430\x44f \x43e\x434\x438\x441\x441\x435\x44f 2001 \x433\x43e\x434\x430", L"1968", L"8.5");*/ /* Космическая одиссея 2001 года */
    /*FT_NWWRITE_LN(table, L"4", L"\x411\x435\x433\x443\x449\x438\x439 \x43f\x43e \x43b\x435\x437\x432\x438\x44e", L"1982", L"8.1");*/ /* Бегущий по лезвию */

    const wchar_t* table_wstr = ft_to_wstring(table);
    if (table_wstr) {
        fwprintf(stderr, L"Table:\n%ls\n ", table_wstr);
    } else {
        fwprintf(stderr, L"Table conversion failed !!!\n ");
        result += 1;
    }

    ft_destroy_table(table);


#endif

    return result;
}
