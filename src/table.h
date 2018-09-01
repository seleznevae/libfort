#ifndef TABLE_H
#define TABLE_H

#include "fort_utils.h"

struct ft_table {
    vector_t *rows;
    fort_table_options_t *options;
    string_buffer_t *conv_buffer;
    size_t cur_row;
    size_t cur_col;
    vector_t *separators;
};

FT_INTERNAL
separator_t *create_separator(int enabled);

FT_INTERNAL
void destroy_separator(separator_t *sep);

FT_INTERNAL
fort_status_t get_table_sizes(const ft_table_t *table, size_t *rows, size_t *cols);

FT_INTERNAL
fort_row_t *get_row(ft_table_t *table, size_t row);

FT_INTERNAL
const fort_row_t *get_row_c(const ft_table_t *table, size_t row);

FT_INTERNAL
fort_row_t *get_row_and_create_if_not_exists(ft_table_t *table, size_t row);

FT_INTERNAL
string_buffer_t *get_cur_str_buffer_and_create_if_not_exists(ft_table_t *table);


FT_INTERNAL
fort_status_t table_rows_and_cols_geometry(const ft_table_t *table,
        size_t **col_width_arr_p, size_t *col_width_arr_sz,
        size_t **row_height_arr_p, size_t *row_height_arr_sz);

FT_INTERNAL
fort_status_t table_geometry(const ft_table_t *table, size_t *height, size_t *width);

#endif /* TABLE_H */
