#include <stdio.h>
#include "fort.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>

static ft_table_t *create_basic_table(void)
{
    ft_table_t *table = ft_create_table();
    ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
    ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_LEFT);

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "Rank", "Title", "Year", "Rating");

    ft_write_ln(table, "1", "The Shawshank Redemption", "1994", "9.5");
    ft_write_ln(table, "2", "12 Angry Men", "1957", "8.8");
    ft_write_ln(table, "3", "It's a Wonderful Life", "1946", "8.6");
    ft_add_separator(table);
    ft_write_ln(table, "4", "2001: A Space Odyssey", "1968", "8.5");
    ft_write_ln(table, "5", "Blade Runner", "1982", "8.1");
    return table;
}

#if defined(FT_HAVE_WCHAR)
static ft_table_t *create_basic_wtable(const struct ft_border_style *style)
{
    ft_table_t *table = ft_create_table();
    ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
    ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_LEFT);

    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_wwrite_ln(table, L"Rank", L"Title", L"Year", L"Rating");

    ft_wwrite_ln(table, L"1", L"The Shawshank Redemption", L"1994", L"9.5");
    ft_wwrite_ln(table, L"2", L"12 Angry Men", L"1957", L"8.8");
    ft_wwrite_ln(table, L"3", L"It's a Wonderful Life", L"1946", L"8.6");
    ft_add_separator(table);
    ft_wwrite_ln(table, L"4", L"2001: A Space Odyssey", L"1968", L"8.5");
    ft_wwrite_ln(table, L"5", L"Blade Runner", L"1982", L"8.1");

    ft_set_border_style(table, style);

    return table;
}
#endif

void print_char_str(const char *str)
{
    printf("Char_repr:\n");
    while (*str) {
        if (*str == '\n')
            printf("\n");
        printf("%x", (unsigned)*str);
        str++;
    }

}

void base_example(void)
{
    ft_table_t *table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "N", "Driver", "Time", "Avg Speed");

    ft_write_ln(table, "1", "Ricciardo", "1:25.945", "222.128");
    ft_write_ln(table, "2", "Hamilton", "1:26.373", "221.027");
    ft_write_ln(table, "3", "Verstappen", "1:26.469", "220.782");

    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);
}

void complex_layout_example(void)
{
    ft_table_t *table = ft_create_table();
    /* Change border style */
    ft_set_border_style(table, FT_DOUBLE2_STYLE);


    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "Sed", "Aenean", "Text");

    ft_write_ln(table, "Duis", "Aliquam",
                "Lorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
                "In accumsan felis eros, nec malesuada sapien bibendum eget.");
    ft_write_ln(table, "Mauris", "Curabitur",
                "Proin condimentum eros viverra nunc ultricies, at fringilla \n"
                "quam pellentesque.");
    ft_write_ln(table, "Summary", "", "Sed tempor est eget odio varius dignissim.");

    ft_set_cell_prop(table, 0, 2, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
    ft_set_cell_prop(table, 3, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
    ft_set_cell_span(table, 3, 0, 2);
    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);
}

void different_cell_properties_example(void)
{
    ft_table_t *table = ft_create_table();
    /* Change border style */
    ft_set_border_style(table, FT_DOUBLE2_STYLE);

    /* Set "header" type for the first row */
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "Movie title", "Director", "Year", "Rating");

    ft_write_ln(table, "The Shawshank Redemption", "Frank Darabont", "1994", "9.5");
    ft_write_ln(table, "The Godfather", "Francis Ford Coppola", "1972", "9.2");
    ft_write_ln(table, "2001: A Space Odyssey", "Stanley Kubrick", "1968", "8.5");

    /* Set center alignment for the 1st and 3rd columns */
    ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
    ft_set_cell_prop(table, FT_ANY_ROW, 3, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);

    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);
}

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

void custom_border_style_example(void)
{
    ft_table_t *table = ft_create_table();
    ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "N", "Driver", "Time", "Avg Speed");

    ft_write_ln(table, "1", "Ricciardo", "1:25.945", "222.128");
    ft_write_ln(table, "2", "Hamilton", "1:26.373", "221.027");
    ft_add_separator(table);
    ft_write_ln(table, "3", "Verstappen", "1:26.469", "220.782");

    /* Set custom border style */
    struct ft_border_chars border;
    border.top_border_ch = "^";
    border.separator_ch  = "=";
    border.bottom_border_ch  = "v";
    border.side_border_ch  = "|";
    border.out_intersect_ch  = "x";
    border.in_intersect_ch  = "x";

    struct ft_border_style border_style;
    memcpy(&border_style.border_chs, &border, sizeof(struct ft_border_chars));
    memcpy(&border_style.header_border_chs, &border, sizeof(struct ft_border_chars));
    border_style.hor_separator_char = "~";
    ft_set_border_style(table, &border_style);

    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);
}


void colorfull_table(void)
{
#if defined(FT_HAVE_WCHAR)
    {
        setlocale(LC_CTYPE, "");

        ft_table_t *table = ft_create_table();
        ft_set_border_style(table, FT_NICE_STYLE);
        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);

        /* Filling table with data */
        ft_wwrite_ln(table, L"Test", L"Iterations", L"ms/op", L"Ticks", L"Passed");
        ft_wwrite_ln(table, L"n-body", L"1000", L"1.6", L"1,500,000", L"✔");
        ft_add_separator(table);
        ft_wwrite_ln(table, L"regex-redux", L"1000", L"0.8", L"8,000,000");
        ft_wwrite_ln(table, L"", L"2500", L"3.9", L"27,000,000", L"✖");
        ft_wwrite_ln(table, L"", L"10000", L"12.5", L"96,800,000");
        ft_add_separator(table);
        ft_wwrite_ln(table, L"mandelbrot", L"1000", L"8.1", L"89,000,000");
        ft_wwrite_ln(table, L"", L"2500", L"19.8", L"320,000,000", L"✔");
        ft_wwrite_ln(table, L"", L"10000", L"60.7", L"987,000,000");
        ft_add_separator(table);
        ft_set_cell_span(table, 8, 0, 4);
        ft_wwrite_ln(table, L"Total result", L"", L"", L"", L"✖");

        /* Setting text styles */
        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
        ft_set_cell_prop(table, 8, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
        ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
        ft_set_cell_prop(table, FT_ANY_ROW, 4, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
        ft_set_cell_prop(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);

        /* Set alignment */
        ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_RIGHT);
        ft_set_cell_prop(table, FT_ANY_ROW, 2, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_RIGHT);
        ft_set_cell_prop(table, FT_ANY_ROW, 3, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_RIGHT);
        ft_set_cell_prop(table, FT_ANY_ROW, 4, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
        ft_set_cell_prop(table, 8, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);

        /* Set colors */
        ft_set_cell_prop(table, 1, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
        ft_set_cell_prop(table, 3, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
        ft_set_cell_prop(table, 6, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
        ft_set_cell_prop(table, 8, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
        ft_set_cell_prop(table, 3, 2, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
        ft_set_cell_prop(table, 4, 3, FT_CPROP_CONT_BG_COLOR, FT_COLOR_LIGHT_RED);
        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_BLUE);

        /* Move table to the center of the screen */
        ft_set_tbl_prop(table, FT_TPROP_TOP_MARGIN, 1);
        ft_set_tbl_prop(table, FT_TPROP_LEFT_MARGIN, 10);

        const wchar_t *table_wstr = ft_to_wstring(table);
        if (table_wstr) {
            fwprintf(stderr, L"Table:\n%ls\n\n ", table_wstr);
        } else {
            fwprintf(stderr, L"Table conversion failed !!!\n ");
        }

        ft_destroy_table(table);
    }
#endif


#if defined(FT_HAVE_UTF8)
    {
        ft_table_t *table = ft_create_table();
        ft_set_border_style(table, FT_NICE_STYLE);
        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);

        /* Filling table with data */
        ft_u8write_ln(table, "Тест", "Итерации", "ms/op", "Тики", "Результат");
        ft_u8write_ln(table, "n-body", "1000", "1.6", "1,500,000", "✔");
        ft_add_separator(table);
        ft_u8write_ln(table, "regex-redux", "1000", "0.8", "8,000,000");
        ft_u8write_ln(table, "", "2500", "3.9", "27,000,000", "✖");
        ft_u8write_ln(table, "", "10000", "12.5", "96,800,000");
        ft_add_separator(table);
        ft_u8write_ln(table, "mandelbrot", "1000", "8.1", "89,000,000");
        ft_u8write_ln(table, "", "2500", "19.8", "320,000,000", "✔");
        ft_u8write_ln(table, "", "10000", "60.7", "987,000,000");
        ft_add_separator(table);
        ft_set_cell_span(table, 8, 0, 4);
        ft_u8write_ln(table, "Итог", "", "", "", "✖");

        /* Setting text styles */
        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
        ft_set_cell_prop(table, 8, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
        ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
        ft_set_cell_prop(table, FT_ANY_ROW, 4, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_BOLD);
        ft_set_cell_prop(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_CPROP_CONT_TEXT_STYLE, FT_TSTYLE_ITALIC);

        /* Set alignment */
        ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_RIGHT);
        ft_set_cell_prop(table, FT_ANY_ROW, 2, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_RIGHT);
        ft_set_cell_prop(table, FT_ANY_ROW, 3, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_RIGHT);
        ft_set_cell_prop(table, FT_ANY_ROW, 4, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);
        ft_set_cell_prop(table, 8, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_CENTER);

        /* Set colors */
        ft_set_cell_prop(table, 1, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
        ft_set_cell_prop(table, 3, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
        ft_set_cell_prop(table, 6, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_GREEN);
        ft_set_cell_prop(table, 8, 4, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
        ft_set_cell_prop(table, 3, 2, FT_CPROP_CONT_FG_COLOR, FT_COLOR_RED);
        ft_set_cell_prop(table, 4, 3, FT_CPROP_CONT_BG_COLOR, FT_COLOR_LIGHT_RED);
        ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_CONT_FG_COLOR, FT_COLOR_LIGHT_BLUE);

        /* Move table to the center of the screen */
        ft_set_tbl_prop(table, FT_TPROP_TOP_MARGIN, 1);
        ft_set_tbl_prop(table, FT_TPROP_LEFT_MARGIN, 10);

        const char *table_str = ft_to_u8string(table);
        if (table_str) {
            printf("Table:\n%s\n\n ", table_str);
        } else {
            printf("Table conversion failed !!!\n ");
        }

        ft_destroy_table(table);
    }
#endif
}


void print_different_border_styles()
{
#if defined(FT_HAVE_WCHAR)

    ft_table_t *table = ft_create_table();
    ft_set_border_style(table, FT_EMPTY_STYLE);

    ft_set_cell_prop(table, FT_ANY_ROW, FT_ANY_COLUMN, FT_CPROP_TOP_PADDING, 3);
    ft_set_cell_prop(table, FT_ANY_ROW, 1, FT_CPROP_LEFT_PADDING, 10);

    const struct ft_border_style *styles[] = {
        FT_BASIC_STYLE,
        FT_BASIC2_STYLE,
        FT_SIMPLE_STYLE,
        FT_PLAIN_STYLE,
        FT_DOT_STYLE,
        FT_EMPTY_STYLE,
        FT_EMPTY2_STYLE,
        FT_SOLID_STYLE,
        FT_SOLID_ROUND_STYLE,
        FT_NICE_STYLE,
        FT_DOUBLE_STYLE,
        FT_DOUBLE2_STYLE,
        FT_BOLD_STYLE,
        FT_BOLD2_STYLE,
        FT_FRAME_STYLE

    };

    const wchar_t *style_names[] = {
        L"FT_BASIC_STYLE",
        L"FT_BASIC2_STYLE",
        L"FT_SIMPLE_STYLE",
        L"FT_PLAIN_STYLE",
        L"FT_DOT_STYLE",
        L"FT_EMPTY_STYLE",
        L"FT_EMPTY2_STYLE",
        L"FT_SOLID_STYLE",
        L"FT_SOLID_ROUND_STYLE",
        L"FT_NICE_STYLE",
        L"FT_DOUBLE_STYLE",
        L"FT_DOUBLE2_STYLE",
        L"FT_BOLD_STYLE",
        L"FT_BOLD2_STYLE",
        L"FT_FRAME_STYLE"
    };

    for (size_t i = 0; i < sizeof(styles)/sizeof(styles[0]); i += 2) {
        ft_table_t *table_tmp_1 = create_basic_wtable(styles[i]);
        ft_wprintf(table,    L"                 %ls    \n\n%ls", style_names[i], ft_to_wstring(table_tmp_1));
        ft_destroy_table(table_tmp_1);

        if (i + 1 < sizeof(styles)/sizeof(styles[0])) {
            ft_table_t *table_tmp_2 = create_basic_wtable(styles[i + 1]);
            ft_wprintf_ln(table, L"                 %ls    \n\n%ls", style_names[i + 1], ft_to_wstring(table_tmp_2));
            ft_destroy_table(table_tmp_2);
        }
    }
    fwprintf(stderr, L"Table:\n%ls\n ", ft_to_wstring(table));
    ft_destroy_table(table);
#endif
}

int main(void)
{
    base_example();
    different_cell_properties_example();
    fill_table_with_data_example();
    complex_layout_example();
    custom_border_style_example();
    colorfull_table();

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

    /*----------------  Different styles  --------------------*/

    ft_set_default_border_style(FT_BASIC_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_SIMPLE_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_PLAIN_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_DOT_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_EMPTY_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_EMPTY2_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_SOLID_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_SOLID_ROUND_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_DOUBLE_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_DOUBLE2_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_BOLD_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_BOLD2_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    ft_set_default_border_style(FT_FRAME_STYLE);
    table = create_basic_table();
    printf("Table:\n%s\n", ft_to_string(table));
    ft_destroy_table(table);

    /* Debug */
    ft_set_default_border_style(FT_SOLID_STYLE);
    table = create_basic_table();
    ft_set_cell_prop(table, FT_CUR_ROW, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "Summary", "", "", "8.7");
    ft_set_cell_span(table, 6, 0, 3);
    ft_set_cell_span(table, 0, 0, 3);
    printf("Table:\n%s\n", ft_to_string(table));
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


    print_different_border_styles();
#endif

    return result;
}
