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
 *               LIBFORT
 * ***************************************************************************/

FTABLE * ft_create_table(void)
{
    FTABLE *result = (FTABLE *)F_CALLOC(1, sizeof(FTABLE));
    if (result == NULL)
        return NULL;

    result->rows = create_vector(sizeof(fort_row_t*), DEFAULT_VECTOR_CAPACITY);
    if (result->rows == NULL) {
        F_FREE(result);
        return NULL;
    }
    result->separators = create_vector(sizeof(separator_t*), DEFAULT_VECTOR_CAPACITY);
    if (result->separators == NULL) {
        destroy_vector(result->rows);
        F_FREE(result);
        return NULL;
    }
    result->options = NULL;
    result->conv_buffer = NULL;
    result->cur_row = 0;
    result->cur_col = 0;
    return result;
}


void ft_destroy_table(FTABLE *FT_RESTRICT table)
{
    size_t i = 0;

    if (table == NULL)
        return;

    if (table->rows) {
        size_t row_n = vector_size(table->rows);
        for (i = 0; i < row_n; ++i) {
            destroy_row(*(fort_row_t **)vector_at(table->rows, i));
        }
        destroy_vector(table->rows);
    }
    if (table->separators) {
        size_t row_n = vector_size(table->separators);
        for (i = 0; i < row_n; ++i) {
            destroy_separator(*(separator_t **)vector_at(table->separators, i));
        }
        destroy_vector(table->separators);
    }
    destroy_table_options(table->options);
    destroy_string_buffer(table->conv_buffer);
    F_FREE(table);
}

void ft_ln(FTABLE *FT_RESTRICT table)
{
    assert(table);
    table->cur_col = 0;
    table->cur_row++;
}


static int ft_row_printf_impl(FTABLE *FT_RESTRICT table, size_t row, const char* FT_RESTRICT fmt, va_list *va)
{
    size_t i = 0;
    size_t new_cols = 0;

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
        for (i = 0; i < push_n; ++i) {
            fort_row_t *padding_row = create_row();
            if (padding_row == NULL)
                goto clear;

            if (IS_ERROR(vector_push(table->rows, &padding_row))) {
                destroy_row(padding_row);
                goto clear;
            }
        }
    }
    /* todo: clearing pushed items in case of error */
    /* todo: this function always create new row, this is not correct, it should be more complicated */

    cur_row_p = (fort_row_t**)vector_at(table->rows, row);

    destroy_row(*cur_row_p);
    *cur_row_p = new_row;
    new_cols = columns_in_row(new_row);
    table->cur_col += new_cols;
    return new_cols;

clear:
    destroy_row(new_row);
    return -1;
}


#if defined(FT_CLANG_COMPILER) || defined(FT_GCC_COMPILER)
#define FT_PRINTF ft_printf
#define FT_PRINTF_LN ft_printf_ln
#else
#define FT_PRINTF ft_printf_impl
#define FT_PRINTF_LN ft_printf_ln_impl
#endif



int FT_PRINTF(FTABLE *FT_RESTRICT table, const char* FT_RESTRICT fmt, ...)
{
    assert(table);
    va_list va;
    va_start(va, fmt);
    int result = ft_row_printf_impl(table, table->cur_row, fmt, &va);
    va_end(va);
    return result;
}

int FT_PRINTF_LN(FTABLE *FT_RESTRICT table, const char* FT_RESTRICT fmt, ...)
{
    assert(table);
    va_list va;
    va_start(va, fmt);
    int result = ft_row_printf_impl(table, table->cur_row, fmt, &va);
    if (result >= 0) {
        ft_ln(table);
    }
    va_end(va);
    return result;
}

#undef FT_PRINTF
#undef FT_PRINTF_LN
#undef FT_HDR_PRINTF
#undef FT_HDR_PRINTF_LN


int ft_write(FTABLE *FT_RESTRICT table, const char* FT_RESTRICT cell_content)
{
    assert(table);
    string_buffer_t *str_buffer = get_cur_str_buffer_and_create_if_not_exists(table);
    if (str_buffer == NULL)
        return FT_ERROR;

    int status = fill_buffer_from_string(str_buffer, cell_content);
    if (IS_SUCCESS(status)) {
        table->cur_col++;
    }
    return status;
}

int ft_write_ln(FTABLE *FT_RESTRICT table, const char* FT_RESTRICT cell_content)
{
    assert(table);
    int status = ft_write(table, cell_content);
    if (IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}

int ft_wwrite(FTABLE *FT_RESTRICT table, const wchar_t* FT_RESTRICT cell_content)
{
    assert(table);
    string_buffer_t *str_buffer = get_cur_str_buffer_and_create_if_not_exists(table);
    if (str_buffer == NULL)
        return FT_ERROR;

    int status = fill_buffer_from_wstring(str_buffer, cell_content);
    if (IS_SUCCESS(status)) {
        table->cur_col++;
    }
    return status;
}

int ft_wwrite_ln(FTABLE *FT_RESTRICT table, const wchar_t* FT_RESTRICT cell_content)
{
    assert(table);
    int status = ft_wwrite(table, cell_content);
    if (IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}


int ft_nwrite(FTABLE *FT_RESTRICT table, size_t n, const char* FT_RESTRICT cell_content, ...)
{
    size_t i = 0;
    assert(table);
    int status = ft_write(table, cell_content);
    if (IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --n;
    for (i = 0; i < n; ++i) {
        const char *cell = va_arg(va, const char*);
        status = ft_write(table, cell);
        if (IS_ERROR(status))
            return status;
    }
    va_end(va);
    return status;
}

int ft_nwrite_ln(FTABLE *FT_RESTRICT table, size_t n, const char* FT_RESTRICT cell_content, ...)
{
    size_t i = 0;
    assert(table);
    int status = ft_write(table, cell_content);
    if (IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --n;
    for (i = 0; i < n; ++i) {
        const char *cell = va_arg(va, const char*);
        status = ft_write(table, cell);
        if (IS_ERROR(status)) {
            va_end(va);
            return status;
        }
    }
    va_end(va);

    ft_ln(table);
    return status;
}


int ft_nwwrite(FTABLE *FT_RESTRICT table, size_t n, const wchar_t* FT_RESTRICT cell_content, ...)
{
    size_t i = 0;
    assert(table);
    int status = ft_wwrite(table, cell_content);
    if (IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --n;
    for (i = 0; i < n; ++i) {
        const wchar_t *cell = va_arg(va, const wchar_t*);
        status = ft_wwrite(table, cell);
        if (IS_ERROR(status))
            return status;
    }
    va_end(va);
    return status;
}

int ft_nwwrite_ln(FTABLE *FT_RESTRICT table, size_t n, const wchar_t* FT_RESTRICT cell_content, ...)
{
    size_t i = 0;
    assert(table);
    int status = ft_wwrite(table, cell_content);
    if (IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --n;
    for (i = 0; i < n; ++i) {
        const wchar_t *cell = va_arg(va, const wchar_t*);
        status = ft_wwrite(table, cell);
        if (IS_ERROR(status)) {
            va_end(va);
            return status;
        }
    }
    va_end(va);

    ft_ln(table);
    return status;
}


int ft_row_write(FTABLE *FT_RESTRICT table, size_t cols, const char* FT_RESTRICT cells[])
{
    size_t i = 0;
    assert(table);
    for (i = 0; i < cols; ++i) {
        int status = ft_write(table, cells[i]);
        if (IS_ERROR(status)) {
            /* todo: maybe current pos in case of error should be equal to the one before function call? */
            return status;
        }
    }
    return FT_SUCCESS;
}

int ft_row_write_ln(FTABLE *FT_RESTRICT table, size_t cols, const char* FT_RESTRICT cells[])
{
    assert(table);
    int status = ft_row_write(table, cols, cells);
    if (IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}



#if !defined(__cplusplus) && !defined(FT_MICROSOFT_COMPILER)

int ft_s_table_write(FTABLE *FT_RESTRICT table, size_t rows, size_t cols, const char* FT_RESTRICT table_cells[rows][cols])
{
    size_t i = 0;
    assert(table);
    for (i = 0; i < rows; ++i) {
        int status = ft_row_write(table, cols, table_cells[i]);
        if (IS_ERROR(status)) {
            /* todo: maybe current pos in case of error should be equal to the one before function call? */
            return status;
        }
        if (i != rows - 1)
            ft_ln(table);
    }
    return FT_SUCCESS;
}

int ft_s_table_write_ln(FTABLE *FT_RESTRICT table, size_t rows, size_t cols, const char* FT_RESTRICT table_cells[rows][cols])
{
    assert(table);
    int status = ft_s_table_write(table, rows, cols, table_cells);
    if (IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}


int ft_table_write(FTABLE *FT_RESTRICT table, size_t rows, size_t cols, const char* * FT_RESTRICT table_cells[rows])
{
    size_t i = 0;
    assert(table);
    for (i = 0; i < rows; ++i) {
        int status = ft_row_write(table, cols, table_cells[i]);
        if (IS_ERROR(status)) {
            /* todo: maybe current pos in case of error should be equal to the one before function call? */
            return status;
        }
        if (i != rows - 1)
            ft_ln(table);
    }
    return FT_SUCCESS;
}

int ft_table_write_ln(FTABLE *FT_RESTRICT table, size_t rows, size_t cols, const char* * FT_RESTRICT table_cells[rows])
{
    assert(table);
    int status = ft_table_write(table, rows, cols, table_cells);
    if (IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}
#endif








/*****************************************************************************
 *               TABLE
 * ***************************************************************************/



const char* ft_to_string(const FTABLE *FT_RESTRICT table)
{
#define CHECK_RESULT_AND_MOVE_DEV(statement) \
    k = statement; \
    if (k < 0) {\
        goto clear; \
    } \
    dev += k;

    typedef char char_type;
    const char_type *empty_string = "";
    const enum str_buf_type buf_type = CharBuf;
    char space_char = ' ';
    char new_line_char = '\n';
#define cur_F_STRDUP F_STRDUP
    int (*snprintf_row_)(const fort_row_t *, char *, size_t, size_t *, size_t, size_t, const context_t *) = snprintf_row;
    int (*print_row_separator_)(char *, size_t ,
                                   const size_t *, size_t ,
                                   const fort_row_t *, const fort_row_t *,
                                   enum HorSeparatorPos , const separator_t *,
                                   const context_t *) = print_row_separator;
    int (*snprint_n_chars_)(char *, size_t , size_t , char) = snprint_n_chars;
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
        ((FTABLE *)table)->conv_buffer = create_string_buffer(sz, buf_type);
        if (table->conv_buffer == NULL)
            return NULL;
    }
    while (string_buffer_capacity(table->conv_buffer) < sz) {
        if (IS_ERROR(realloc_string_buffer_without_copy(table->conv_buffer))) {
            return NULL;
        }
    }
    char_type *buffer = (char_type *)buffer_get_data(table->conv_buffer);


    size_t cols = 0;
    size_t rows = 0;
    size_t *col_width_arr = NULL;
    size_t *row_height_arr = NULL;
    status = table_rows_and_cols_geometry(table, &col_width_arr, &cols, &row_height_arr, &rows);

    if (rows == 0)
        return cur_F_STRDUP(empty_string);

    if (IS_ERROR(status))
        return NULL;

    int dev = 0;
    int k = 0;
    size_t i = 0;
    context_t context;
    context.table_options = (table->options ? table->options : &g_table_options);
    fort_row_t *prev_row = NULL;
    fort_row_t *cur_row = NULL;
    separator_t *cur_sep = NULL;
    size_t sep_size = vector_size(table->separators);

    /* Print top margin */
    for (i = 0; i < context.table_options->entire_table_options.top_margin; ++i) {
        CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars_(buffer + dev, sz - dev, width - 1/* minus new_line*/, space_char));
        CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars_(buffer + dev, sz - dev, 1, new_line_char));
    }

    for (i = 0; i < rows; ++i) {
        cur_sep = (i < sep_size) ? (*(separator_t **)vector_at(table->separators, i)) : NULL;
        cur_row = *(fort_row_t**)vector_at(table->rows, i);
        enum HorSeparatorPos separatorPos = (i == 0) ? TopSeparator : InsideSeparator;
        context.row = i;
        CHECK_RESULT_AND_MOVE_DEV(print_row_separator_(buffer + dev, sz - dev, col_width_arr, cols, prev_row, cur_row, separatorPos, cur_sep, &context));
        CHECK_RESULT_AND_MOVE_DEV(snprintf_row_(cur_row, buffer + dev, sz - dev, col_width_arr, cols, row_height_arr[i], &context));
        prev_row = cur_row;
    }
    cur_row = NULL;
    cur_sep = (i < sep_size) ? (*(separator_t **)vector_at(table->separators, i)) : NULL;
    CHECK_RESULT_AND_MOVE_DEV(print_row_separator_(buffer + dev, sz - dev, col_width_arr, cols, prev_row, cur_row, BottomSeparator, cur_sep, &context));

    /* Print bottom margin */
    for (i = 0; i < context.table_options->entire_table_options.bottom_margin; ++i) {
        CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars_(buffer + dev, sz - dev, width - 1/* minus new_line*/, space_char));
        CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars_(buffer + dev, sz - dev, 1, new_line_char));
    }


    F_FREE(col_width_arr);
    F_FREE(row_height_arr);
    return buffer;

clear:
    F_FREE(col_width_arr);
    F_FREE(row_height_arr);
    F_FREE(buffer);
    return NULL;

#undef cur_F_STRDUP
#undef CHECK_RESULT_AND_MOVE_DEV
}


const wchar_t* ft_to_wstring(const FTABLE *FT_RESTRICT table)
{
#define CHECK_RESULT_AND_MOVE_DEV(statement) \
    k = statement; \
    if (k < 0) {\
        goto clear; \
    } \
    dev += k;

    typedef wchar_t char_type;
    const char_type *empty_string = L"";
    const enum str_buf_type buf_type = WCharBuf;
    wchar_t space_char = L' ';
    wchar_t new_line_char = L'\n';
#define cur_F_STRDUP F_WCSDUP
    int (*snprintf_row_)(const fort_row_t *, wchar_t *, size_t, size_t *, size_t, size_t, const context_t *) = wsnprintf_row;
    int (*print_row_separator_)(wchar_t *, size_t ,
                                   const size_t *, size_t ,
                                   const fort_row_t *, const fort_row_t *,
                                   enum HorSeparatorPos , const separator_t *,
                                   const context_t *) = wprint_row_separator;
    int (*snprint_n_chars_)(wchar_t *, size_t , size_t , wchar_t) = wsnprint_n_chars;

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
        ((FTABLE *)table)->conv_buffer = create_string_buffer(sz, buf_type);
        if (table->conv_buffer == NULL)
            return NULL;
    }
    while (string_buffer_capacity(table->conv_buffer) < sz) {
        if (IS_ERROR(realloc_string_buffer_without_copy(table->conv_buffer))) {
            return NULL;
        }
    }
    char_type *buffer = (char_type *)buffer_get_data(table->conv_buffer);


    size_t cols = 0;
    size_t rows = 0;
    size_t *col_width_arr = NULL;
    size_t *row_height_arr = NULL;
    status = table_rows_and_cols_geometry(table, &col_width_arr, &cols, &row_height_arr, &rows);

    if (rows == 0)
        return cur_F_STRDUP(empty_string);

    if (IS_ERROR(status))
        return NULL;

    int dev = 0;
    int k = 0;
    size_t i = 0;
    context_t context;
    context.table_options = (table->options ? table->options : &g_table_options);
    fort_row_t *prev_row = NULL;
    fort_row_t *cur_row = NULL;
    separator_t *cur_sep = NULL;
    size_t sep_size = vector_size(table->separators);

    /* Print top margin */
    for (i = 0; i < context.table_options->entire_table_options.top_margin; ++i) {
        CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars_(buffer + dev, sz - dev, width - 1/* minus new_line*/, space_char));
        CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars_(buffer + dev, sz - dev, 1, new_line_char));
    }

    for (i = 0; i < rows; ++i) {
        cur_sep = (i < sep_size) ? (*(separator_t **)vector_at(table->separators, i)) : NULL;
        cur_row = *(fort_row_t**)vector_at(table->rows, i);
        enum HorSeparatorPos separatorPos = (i == 0) ? TopSeparator : InsideSeparator;
        context.row = i;
        CHECK_RESULT_AND_MOVE_DEV(print_row_separator_(buffer + dev, sz - dev, col_width_arr, cols, prev_row, cur_row, separatorPos, cur_sep, &context));
        CHECK_RESULT_AND_MOVE_DEV(snprintf_row_(cur_row, buffer + dev, sz - dev, col_width_arr, cols, row_height_arr[i], &context));
        prev_row = cur_row;
    }
    cur_row = NULL;
    cur_sep = (i < sep_size) ? (*(separator_t **)vector_at(table->separators, i)) : NULL;
    CHECK_RESULT_AND_MOVE_DEV(print_row_separator_(buffer + dev, sz - dev, col_width_arr, cols, prev_row, cur_row, BottomSeparator, cur_sep, &context));

    /* Print bottom margin */
    for (i = 0; i < context.table_options->entire_table_options.bottom_margin; ++i) {
        CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars_(buffer + dev, sz - dev, width - 1/* minus new_line*/, space_char));
        CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars_(buffer + dev, sz - dev, 1, new_line_char));
    }

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
static int dummy_function()
#if defined(FT_CLANG_COMPILER) || defined(FT_GCC_COMPILER)
__attribute__ ((unused))
#endif
;

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




int ft_add_separator(FTABLE *table)
{
    assert(table);
    assert(table->separators);

    while(vector_size(table->separators) <= table->cur_row) {
        separator_t *sep_p = create_separator(F_FALSE);
        if (sep_p == NULL)
            return FT_MEMORY_ERROR;
        int status = vector_push(table->separators, &sep_p);
        if (IS_ERROR(status))
            return status;
    }

    separator_t **sep_p = (separator_t **)vector_at(table->separators, table->cur_row);
    if (*sep_p == NULL)
        *sep_p = create_separator(F_TRUE);
    else
        (*sep_p)->enabled = F_TRUE;

    if (*sep_p == NULL)
        return FT_ERROR;
    return FT_SUCCESS;
}




/* ******************************************************************************* */


struct ft_border_style * FT_BASIC_STYLE = (struct ft_border_style *)&FORT_BASIC_STYLE;
struct ft_border_style * FT_SIMPLE_STYLE = (struct ft_border_style *)&FORT_SIMPLE_STYLE;
struct ft_border_style * FT_PLAIN_STYLE =  (struct ft_border_style *)&FORT_PLAIN_STYLE;
struct ft_border_style * FT_DOT_STYLE = (struct ft_border_style *)&FORT_DOT_STYLE;
struct ft_border_style * FT_EMPTY_STYLE  = (struct ft_border_style *)&FORT_EMPTY_STYLE;



static void set_border_options_for_options(fort_table_options_t *options, struct ft_border_style *style)
{
    if ((struct fort_border_style *)style == &FORT_BASIC_STYLE
        || (struct fort_border_style *)style == &FORT_SIMPLE_STYLE
        || (struct fort_border_style *)style == &FORT_DOT_STYLE
        || (struct fort_border_style *)style == &FORT_PLAIN_STYLE
        || (struct fort_border_style *)style == &FORT_EMPTY_STYLE)
    {
        memcpy(&(options->border_style), (struct fort_border_style *)style, sizeof(struct fort_border_style));
        return;
    }

    struct ft_border_chars *border_chs = &(style->border_chs);
    struct ft_border_chars *header_border_chs = &(style->header_border_chs);

#define BOR_CHARS options->border_style.border_chars
#define H_BOR_CHARS options->border_style.header_border_chars
#define SEP_CHARS options->border_style.separator_chars

    /*
    BOR_CHARS[TL_bip] = BOR_CHARS[TT_bip] = BOR_CHARS[TV_bip] = BOR_CHARS[TR_bip] = border_chs->top_border_ch;
    BOR_CHARS[LH_bip] = BOR_CHARS[IH_bip] = BOR_CHARS[II_bip] = BOR_CHARS[RH_bip] = border_chs->separator_ch;
    BOR_CHARS[BL_bip] = BOR_CHARS[BB_bip] = BOR_CHARS[BV_bip] = BOR_CHARS[BR_bip] = border_chs->bottom_border_ch;
    BOR_CHARS[LL_bip] = BOR_CHARS[IV_bip] = BOR_CHARS[RR_bip] = border_chs->side_border_ch;

    H_BOR_CHARS[TL_bip] = H_BOR_CHARS[TT_bip] = H_BOR_CHARS[TV_bip] = H_BOR_CHARS[TR_bip] = header_border_chs->top_border_ch;
    H_BOR_CHARS[LH_bip] = H_BOR_CHARS[IH_bip] = H_BOR_CHARS[II_bip] = H_BOR_CHARS[RH_bip] = header_border_chs->separator_ch;
    H_BOR_CHARS[BL_bip] = H_BOR_CHARS[BB_bip] = H_BOR_CHARS[BV_bip] = H_BOR_CHARS[BR_bip] = header_border_chs->bottom_border_ch;
    H_BOR_CHARS[LL_bip] = H_BOR_CHARS[IV_bip] = H_BOR_CHARS[RR_bip] = header_border_chs->side_border_ch;
    */

    BOR_CHARS[TT_bip] = border_chs->top_border_ch;
    BOR_CHARS[IH_bip] = border_chs->separator_ch;
    BOR_CHARS[BB_bip] = border_chs->bottom_border_ch;
    BOR_CHARS[LL_bip] = BOR_CHARS[IV_bip] = BOR_CHARS[RR_bip] = border_chs->side_border_ch;

    BOR_CHARS[TL_bip] = BOR_CHARS[TV_bip] = BOR_CHARS[TR_bip] = border_chs->out_intersect_ch;
    BOR_CHARS[LH_bip] = BOR_CHARS[RH_bip] = border_chs->out_intersect_ch;
    BOR_CHARS[BL_bip] = BOR_CHARS[BV_bip] = BOR_CHARS[BR_bip] = border_chs->out_intersect_ch;
    BOR_CHARS[II_bip] = border_chs->in_intersect_ch;

    if (border_chs->separator_ch == '\0' && border_chs->in_intersect_ch == '\0') {
        BOR_CHARS[LH_bip] = BOR_CHARS[RH_bip] = '\0';
    }


    H_BOR_CHARS[TT_bip] = header_border_chs->top_border_ch;
    H_BOR_CHARS[IH_bip] = header_border_chs->separator_ch;
    H_BOR_CHARS[BB_bip] = header_border_chs->bottom_border_ch;
    H_BOR_CHARS[LL_bip] = H_BOR_CHARS[IV_bip] = H_BOR_CHARS[RR_bip] = header_border_chs->side_border_ch;

    H_BOR_CHARS[TL_bip] = H_BOR_CHARS[TV_bip] = H_BOR_CHARS[TR_bip] = header_border_chs->out_intersect_ch;
    H_BOR_CHARS[LH_bip] = H_BOR_CHARS[RH_bip] = header_border_chs->out_intersect_ch;
    H_BOR_CHARS[BL_bip] = H_BOR_CHARS[BV_bip] = H_BOR_CHARS[BR_bip] = header_border_chs->out_intersect_ch;
    H_BOR_CHARS[II_bip] = header_border_chs->in_intersect_ch;

    if (header_border_chs->separator_ch == '\0' && header_border_chs->in_intersect_ch == '\0') {
        H_BOR_CHARS[LH_bip] = H_BOR_CHARS[RH_bip] = '\0';
    }

    SEP_CHARS[LH_sip] = SEP_CHARS[RH_sip] = SEP_CHARS[II_sip] = header_border_chs->out_intersect_ch;
    SEP_CHARS[IH_sip] = style->hor_separator_char;


#undef BOR_CHARS
#undef H_BOR_CHARS
#undef SEP_CHARS
}


int ft_set_default_border_style(struct ft_border_style *style)
{
    set_border_options_for_options(&g_table_options, style);
    return FT_SUCCESS;
}

int ft_set_border_style(FTABLE *table, struct ft_border_style *style)
{
    assert(table);
    if (table->options == NULL) {
        table->options = create_table_options();
        if (table->options == NULL)
            return FT_MEMORY_ERROR;
    }
    set_border_options_for_options(table->options, style);
    return FT_SUCCESS;
}



int ft_set_cell_option(FTABLE *table, unsigned row, unsigned col, uint32_t option, int value)
{
    assert(table);

    if (table->options == NULL) {
        table->options = create_table_options();
        if (table->options == NULL)
            return FT_MEMORY_ERROR;
    }
    if (table->options->cell_options == NULL) {
        table->options->cell_options = create_cell_opt_container();
        if (table->options->cell_options == NULL) {
            return FT_ERROR;
        }
    }
    return set_cell_option(table->options->cell_options, row, col, option, value);
}

int ft_set_default_cell_option(uint32_t option, int value)
{
    return set_default_cell_option(option, value);
}


FT_EXTERN int ft_set_default_tbl_option(uint32_t option, int value)
{
    return set_default_entire_table_option(option, value);
}

FT_EXTERN int ft_set_tbl_option(FTABLE * FT_RESTRICT table, uint32_t option, int value)
{
    assert(table);

    if (table->options == NULL) {
        table->options = create_table_options();
        if (table->options == NULL)
            return FT_MEMORY_ERROR;
    }
    return set_entire_table_option(table->options, option, value);
}
