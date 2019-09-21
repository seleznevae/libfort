#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fort.h"


int main(void)
{
#if defined(FT_HAVE_WCHAR) && !defined(FT_MICROSOFT_COMPILER)
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

    /* Ранг | Название | Год | Рейтинг */
    /*FT_NWWRITE_LN(table, L"\x420\x430\x43d\x433", L"\x41d\x430\x437\x432\x430\x43d\x438\x435", L"\x413\x43e\x434", L"\x420\x435\x439\x442\x438\x43d\x433"); */

    /*FT_NWWRITE_LN(table, L"1", L"\x41f\x43e\x431\x435\x433 \x438\x437 \x428\x43e\x443\x448\x435\x43d\x43a\x430", L"1994", L"9.5");*/ /* Побег из Шоушенка */
    /*FT_NWWRITE_LN(table, L"2", L"12 \x440\x430\x437\x433\x43d\x435\x432\x430\x43d\x43d\x44b\x445 \x43c\x443\x436\x447\x438\x43d", L"1957", L"8.8");*/ /* 12 разгневанных мужчин */
    /*FT_NWWRITE_LN(table, L"3", L"\x41a\x43e\x441\x43c\x438\x447\x435\x441\x43a\x430\x44f \x43e\x434\x438\x441\x441\x435\x44f 2001 \x433\x43e\x434\x430", L"1968", L"8.5");*/ /* Космическая одиссея 2001 года */
    /*FT_NWWRITE_LN(table, L"4", L"\x411\x435\x433\x443\x449\x438\x439 \x43f\x43e \x43b\x435\x437\x432\x438\x44e", L"1982", L"8.1");*/ /* Бегущий по лезвию */

    const wchar_t* table_wstr = ft_to_wstring(table);
    fwprintf(stderr, L"%ls\n ", table_wstr);
    ft_destroy_table(table);
#endif

    return 0;
}
