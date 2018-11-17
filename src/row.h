#ifndef ROW_H
#define ROW_H

#include "fort_utils.h"
#include "fort.h"
#include <stdarg.h>
#include "properties.h"
#ifdef FT_HAVE_WCHAR
#include <wchar.h>
#endif

FT_INTERNAL
fort_row_t *create_row(void);

FT_INTERNAL
void destroy_row(fort_row_t *row);

FT_INTERNAL
fort_row_t *copy_row(fort_row_t *row);

FT_INTERNAL
fort_row_t *create_row_from_string(const char *str);

FT_PRINTF_ATTRIBUTE_FORMAT(1, 0)
FT_INTERNAL
fort_row_t *create_row_from_fmt_string(const char  *fmt, va_list *va_args);

FT_INTERNAL
size_t columns_in_row(const fort_row_t *row);

FT_INTERNAL
fort_cell_t *get_cell(fort_row_t *row, size_t col);

FT_INTERNAL
const fort_cell_t *get_cell_c(const fort_row_t *row, size_t col);

FT_INTERNAL
fort_cell_t *get_cell_and_create_if_not_exists(fort_row_t *row, size_t col);

FT_INTERNAL
fort_status_t swap_row(fort_row_t *cur_row, fort_row_t *ins_row, size_t pos);

FT_INTERNAL
size_t group_cell_number(const fort_row_t *row, size_t master_cell_col);

FT_INTERNAL
int get_row_cell_types(const fort_row_t *row, enum CellType *types, size_t types_sz);

FT_INTERNAL
fort_status_t row_set_cell_span(fort_row_t *row, size_t cell_column, size_t hor_span);

FT_INTERNAL
int print_row_separator(char *buffer, size_t buffer_sz,
                        const size_t *col_width_arr, size_t cols,
                        const fort_row_t *upper_row, const fort_row_t *lower_row,
                        enum HorSeparatorPos separatorPos, const separator_t *sep,
                        const context_t *context);

FT_INTERNAL
int snprintf_row(const fort_row_t *row, char *buffer, size_t buf_sz, size_t *col_width_arr, size_t col_width_arr_sz,
                 size_t row_height, const context_t *context);

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
fort_row_t *create_row_from_wstring(const wchar_t *str);

FT_INTERNAL
fort_row_t *create_row_from_fmt_wstring(const wchar_t  *fmt, va_list *va_args);

FT_INTERNAL
int wprint_row_separator(wchar_t *buffer, size_t buffer_sz,
                         const size_t *col_width_arr, size_t cols,
                         const fort_row_t *upper_row, const fort_row_t *lower_row,
                         enum HorSeparatorPos separatorPos, const separator_t *sep,
                         const context_t *context);

FT_INTERNAL
int wsnprintf_row(const fort_row_t *row, wchar_t *buffer, size_t buf_sz, size_t *col_width_arr, size_t col_width_arr_sz,
                  size_t row_height, const context_t *context);
#endif


#endif /* ROW_H */
