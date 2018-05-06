#ifndef TABLE_H
#define TABLE_H

#include "fort_utils.h"

struct ft_table;
typedef struct ft_table ft_table_t;
struct ft_table {
    vector_t *rows;
    fort_table_options_t *options;
    string_buffer_t *conv_buffer;
    size_t cur_row;
    size_t cur_col;
    vector_t *separators;
};

static FT_INLINE
separator_t *create_separator(int enabled)
{
    separator_t *res = (separator_t *)F_CALLOC(1, sizeof(separator_t));
    if (res == NULL)
        return NULL;
    res->enabled = enabled;
    return res;
}

static FT_INLINE
void destroy_separator(separator_t *sep)
{
    F_FREE(sep);
}



fort_status_t get_table_sizes(const ft_table_t *table, size_t *rows, size_t *cols);
fort_row_t *get_row_implementation(ft_table_t *table, size_t row, enum PolicyOnNull policy);
fort_row_t *get_row(ft_table_t *table, size_t row);
const fort_row_t *get_row_c(const ft_table_t *table, size_t row);
fort_row_t *get_row_and_create_if_not_exists(ft_table_t *table, size_t row);

string_buffer_t *get_cur_str_buffer_and_create_if_not_exists(ft_table_t *table);



fort_status_t table_rows_and_cols_geometry(const ft_table_t *table,
        size_t **col_width_arr_p, size_t *col_width_arr_sz,
        size_t **row_height_arr_p, size_t *row_height_arr_sz);
fort_status_t table_geometry(const ft_table_t *table, size_t *height, size_t *width);

#endif /* TABLE_H */
