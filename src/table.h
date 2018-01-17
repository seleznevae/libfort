#ifndef TABLE_H
#define TABLE_H

#include "fort_impl.h"
#include "fort.h"
struct fort_table;
typedef struct fort_table fort_table_t;
struct fort_table
{
    vector_t *rows;
    fort_table_options_t *options;
    string_buffer_t *conv_buffer;
    size_t cur_row;
    size_t cur_col;
};


fort_status_t get_table_sizes(const FTABLE *table, size_t *rows, size_t *cols);
fort_row_t *get_row_implementation(fort_table_t *table, size_t row, enum PolicyOnNull policy);
fort_row_t *get_row(fort_table_t *table, size_t row);
const fort_row_t *get_row_c(const fort_table_t *table, size_t row);
fort_row_t *get_row_and_create_if_not_exists(fort_table_t *table, size_t row);

string_buffer_t * get_cur_str_buffer_and_create_if_not_exists(FTABLE *FORT_RESTRICT table);



fort_status_t table_rows_and_cols_geometry(const FTABLE *table,
                                                  size_t **col_width_arr_p, size_t *col_width_arr_sz,
                                                  size_t **row_height_arr_p, size_t *row_height_arr_sz);
fort_status_t table_geometry(const FTABLE *table, size_t *height, size_t *width);

#endif // TABLE_H
