#ifndef ROW_H
#define ROW_H

#include "fort_impl.h"
#include "fort.h"
#include "stdarg.h"
#include "options.h"

struct fort_row;
typedef struct fort_row fort_row_t;

/*
enum RowType
{
    Common,
    Header
};
*/


fort_row_t * create_row();
void destroy_row(fort_row_t *row);
fort_row_t * create_row_from_string(const char *str);
fort_row_t* create_row_from_fmt_string(const char* FT_RESTRICT fmt, va_list *va_args);


int columns_in_row(const fort_row_t *row);

fort_cell_t *get_cell_implementation(fort_row_t *row, size_t col, enum PolicyOnNull policy);
fort_cell_t *get_cell(fort_row_t *row, size_t col);
const fort_cell_t *get_cell_c(const fort_row_t *row, size_t col);
fort_cell_t *get_cell_and_create_if_not_exists(fort_row_t *row, size_t col);



int print_row_separator(char *buffer, size_t buffer_sz,
                               const size_t *col_width_arr, size_t cols,
                               const fort_row_t *upper_row, const fort_row_t *lower_row,
                               enum HorSeparatorPos separatorPos, const separator_t *sep,
                               const context_t *context);

int wprint_row_separator(wchar_t *buffer, size_t buffer_sz,
                               const size_t *col_width_arr, size_t cols,
                               const fort_row_t *upper_row, const fort_row_t *lower_row,
                               enum HorSeparatorPos separatorPos, const separator_t *sep,
                               const context_t *context);




fort_row_t* create_row_from_string(const char *str);
fort_row_t* create_row_from_fmt_string(const char* FT_RESTRICT fmt, va_list *va_args);

int snprintf_row(const fort_row_t *row, char *buffer, size_t buf_sz, size_t *col_width_arr, size_t col_width_arr_sz,
                        size_t row_height, const context_t *context);
int wsnprintf_row(const fort_row_t *row, wchar_t *buffer, size_t buf_sz, size_t *col_width_arr, size_t col_width_arr_sz,
                        size_t row_height, const context_t *context);


#endif /* ROW_H */
