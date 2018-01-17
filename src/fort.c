/*
libfort

MIT License

Copyright (c) 2017 - 2018 Seleznev Anton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "stdlib.h"
#include "stdarg.h"
#include "stdio.h"
#include "fort.h"
#include "assert.h"
#include "string.h"
#include "wchar.h"
#include <ctype.h>

#include "vector.h"
#include "fort_impl.h"
#include "string_buffer.h"
#include "table.h"
#include "row.h"
#include "options.h"



/*****************************************************************************
 *               UTILITIES
 * ***************************************************************************/






/*****************************************************************************
 *               TABLE
 *****************************************************************************/




/*****************************************************************************
 *               LIBFORT
 * ***************************************************************************/

FTABLE * ft_create_table(void)
{
    FTABLE *result = F_CALLOC(1, sizeof(FTABLE));
    if (result == NULL)
        return NULL;

    result->rows = create_vector(sizeof(fort_row_t*), DEFAULT_VECTOR_CAPACITY);
    if (result->rows == NULL) {
        F_FREE(result);
        return NULL;
    }
    result->options = NULL;
    result->conv_buffer = NULL;
    result->cur_row = 0;
    result->cur_col = 0;
    return result;
}


void ft_destroy_table(FTABLE *FORT_RESTRICT table)
{
    if (table == NULL)
        return;

    if (table->rows) {
        size_t row_n = vector_size(table->rows);
        for (size_t i = 0; i < row_n; ++i) {
            destroy_row(*(fort_row_t **)vector_at(table->rows, i));
        }
        destroy_vector(table->rows);
    }
    destroy_table_options(table->options);
    destroy_string_buffer(table->conv_buffer);
    F_FREE(table);
}




static int ft_row_printf_impl(FTABLE *FORT_RESTRICT table, size_t row, const char* FORT_RESTRICT fmt, va_list *va)
{
    if (table == NULL)
        return -1;

    fort_row_t *new_row = create_row_from_fmt_string(fmt, va);

    if (new_row == NULL) {
        return -1;
    }

    fort_row_t **cur_row_p = NULL;
    size_t sz = vector_size(table->rows);
    if (row >= sz) {
        size_t push_n = row - sz + 1;
        for (size_t i = 0; i < push_n; ++i) {
            fort_row_t *new_row = create_row();
            if (new_row == NULL)
                goto clear;

            if (IS_ERROR(vector_push(table->rows, &new_row))) {
                destroy_row(new_row);
                goto clear;
            }
        }
    }
    /* todo clearing pushed items in case of error */

    cur_row_p = (fort_row_t**)vector_at(table->rows, row);

    destroy_row(*cur_row_p);
    *cur_row_p = new_row;
    table->cur_col = 0;
    table->cur_row++;
    return vector_size(new_row->cells);

clear:
    destroy_row(new_row);
    return -1;
}

int ft_hdr_printf(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int result = ft_row_printf_impl(table, 0, fmt, &va);
    va_end(va);
    if (result >= 0 && table->rows) {
        int sz = vector_size(table->rows);
        if (sz != 0) {
            (*(fort_row_t**)vector_at(table->rows, sz - 1))->is_header = F_TRUE;
        }
    }
    return result;
}

int ft_row_printf(FTABLE *FORT_RESTRICT table, size_t row, const char* FORT_RESTRICT fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int result = ft_row_printf_impl(table, row, fmt, &va);
    va_end(va);
    return result;
}

int ft_write(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT cell_content)
{
    string_buffer_t *str_buffer = get_cur_str_buffer_and_create_if_not_exists(table);
    if (str_buffer == NULL)
        return F_ERROR;

    int status = fill_buffer_from_string(str_buffer, cell_content);
    if (IS_SUCCESS(status)) {
        table->cur_col++;
    }
    return status;
}

int ft_write_ln(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT cell_content)
{
    int status = ft_write(table, cell_content);
    if (IS_SUCCESS(status)) {
        table->cur_col = 0;
        table->cur_row++;
    }
    return status;
}

int ft_set_default_options(const fort_table_options_t *options)
{
    memcpy(&g_table_options, options, sizeof(fort_table_options_t));
    return 0;
}

int ft_get_default_options(fort_table_options_t *options)
{
    memcpy(options, &g_table_options, sizeof(fort_table_options_t));
    return 0;
}

int ft_set_table_options(FTABLE * FORT_RESTRICT table, const fort_table_options_t * FORT_RESTRICT options)
{
    assert(table);
    if (options == NULL) {
        F_FREE(table->options);
        table->options = NULL;
        return 0;
    }

    fort_table_options_t *new_options = F_CALLOC(sizeof(fort_table_options_t), 1);
    if (new_options == NULL) {
        return -1;
    }
    memcpy(new_options, options, sizeof(fort_table_options_t));
    F_FREE(table->options);
    table->options = new_options;
    return 0;
}



int ft_set_column_min_width(FTABLE *table, size_t column, size_t width)
{
    if (table->options == NULL) {
        table->options = create_table_options();
        if (table->options == NULL)
            return F_MEMORY_ERROR;
    }

    int status = fort_options_set_column_min_width(table->options, column, width);
    return status;
}

int ft_set_column_alignment(FTABLE * FORT_RESTRICT table, size_t column, enum TextAlignment align)
{
    if (table->options == NULL) {
        table->options = create_table_options();
        if (table->options == NULL)
            return F_MEMORY_ERROR;
    }

    int status = fort_options_set_column_alignment(table->options, column, align);
    return status;
}




/*****************************************************************************
 *               TABLE
 * ***************************************************************************/



const char* ft_to_string(const FTABLE *FORT_RESTRICT table)
{
#define CHECK_RESULT_AND_MOVE_DEV(statement) \
    k = statement; \
    if (k < 0) {\
        goto clear; \
    } \
    dev += k;

    assert(table);

    /* Determing size of table string representation */
    size_t height = 0;
    size_t width = 0;
    int status = table_geometry(table, &height, &width);
    if (IS_ERROR(status)) {
        return NULL;
    }
    size_t sz = height * width + 1;

    /* Allocate string buffer for string representation */
    if (table->conv_buffer == NULL) {
        ((FTABLE *)table)->conv_buffer = create_string_buffer(sz);
        if (table->conv_buffer == NULL)
            return NULL;
    }
    while (table->conv_buffer->str_sz < sz) {
        if (IS_ERROR(realloc_string_buffer_without_copy(table->conv_buffer))) {
            return NULL;
        }
    }
    char *buffer = table->conv_buffer->str;


    size_t cols = 0;
    size_t rows = 0;
    size_t *col_width_arr = NULL;
    size_t *row_height_arr = NULL;
    status = table_rows_and_cols_geometry(table, &col_width_arr, &cols, &row_height_arr, &rows);

    if (rows == 0)
        return F_STRDUP("");

    if (IS_ERROR(status))
        return NULL;

    int dev = 0;
    int k = 0;
    context_t *context = (table->options ? table->options : &g_table_options);
    fort_row_t *prev_row = NULL;
    fort_row_t *cur_row = NULL;
    for (size_t i = 0; i < rows; ++i) {
        cur_row = *(fort_row_t**)vector_at(table->rows, i);
        enum HorSeparatorPos separatorPos = (i == 0) ? TopSeparator : InsideSeparator;
        CHECK_RESULT_AND_MOVE_DEV(print_row_separator(buffer + dev, sz - dev, col_width_arr, cols, prev_row, cur_row, separatorPos, context));
        CHECK_RESULT_AND_MOVE_DEV(snprintf_row(cur_row, buffer + dev, sz - dev, col_width_arr, cols, row_height_arr[i], context));
        prev_row = cur_row;
    }
    cur_row = NULL;
    CHECK_RESULT_AND_MOVE_DEV(print_row_separator(buffer + dev, sz - dev, col_width_arr, cols, prev_row, cur_row, BottomSeparator, context));


    F_FREE(col_width_arr);
    F_FREE(row_height_arr);
    return buffer;

clear:
    F_FREE(col_width_arr);
    F_FREE(row_height_arr);
    F_FREE(buffer);
    return NULL;

#undef CHECK_RESULT_AND_MOVE_DEV
}




/*
 *  TMP
 */
static int dummy_function() __attribute__ ((unused));

static int dummy_function()
{
    if (0) {
        vector_t *v = create_vector(1, DEFAULT_VECTOR_CAPACITY);
        vector_clear(v);
        vector_erase(v, 0);
        vector_index_of(v, NULL);
        vector_capacity(v);
    }
    return 0;
}



