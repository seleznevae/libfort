#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fort.h"


int main(void)
{
#if defined(FT_HAVE_UTF8)
    /* Example of utf8 table */
    {
        ft_table_t *table = ft_create_table();
        ft_set_border_style(table, FT_NICE_STYLE);

        ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
        ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_LEFT);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        ft_u8write_ln(table, "Ранг", "Название", "Год", "Рейтинг");

        ft_u8write_ln(table, "1", "Побег из Шоушенка", "1994", "9.5");
        ft_u8write_ln(table, "2", "12 разгневанных мужчин", "1957", "8.8");
        ft_u8write_ln(table, "3", "Космическая одиссея 2001 года", "1968", "8.5");
        ft_u8write_ln(table, "4", "Бегущий по лезвию", "1982", "8.1");
        const char *table_str = (const char *)ft_to_u8string(table);
        printf("%s\n", table_str);
        ft_destroy_table(table);
    }
#endif

    /* Example of wchar table */
#if defined(FT_HAVE_WCHAR) && !defined(FT_MICROSOFT_COMPILER)
    {
        setlocale(LC_CTYPE, "");

        ft_set_default_border_style(FT_BASIC_STYLE);

        ft_table_t *table = ft_create_table();
        ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
        ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_LEFT);

        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
        ft_wwrite_ln(table, L"Ранг", L"Название", L"Год", L"Рейтинг");

        ft_wwrite_ln(table, L"1", L"Побег из Шоушенка", L"1994", L"9.5");
        ft_wwrite_ln(table, L"2", L"12 разгневанных мужчин", L"1957", L"8.8");
        ft_wwrite_ln(table, L"3", L"Космическая одиссея 2001 года", L"1968", L"8.5");
        ft_wwrite_ln(table, L"4", L"Бегущий по лезвию", L"1982", L"8.1");
        const wchar_t* table_wstr = ft_to_wstring(table);
        fwprintf(stderr, L"%ls\n", table_wstr);
        ft_destroy_table(table);
    }
#endif

    return 0;
}
