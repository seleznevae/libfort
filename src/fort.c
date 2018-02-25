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
    FTABLE *result = F_CALLOC(1, sizeof(FTABLE));
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
    if (table->separators) {
        size_t row_n = vector_size(table->separators);
        for (size_t i = 0; i < row_n; ++i) {
            destroy_separator(*(separator_t **)vector_at(table->separators, i));
        }
        destroy_vector(table->separators);
    }
    destroy_table_options(table->options);
    destroy_string_buffer(table->conv_buffer);
    F_FREE(table);
}

void ft_ln(FTABLE *FORT_RESTRICT table)
{
    assert(table);
    table->cur_col = 0;
    table->cur_row++;
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
    /* todo: clearing pushed items in case of error */
    /* todo: this function always create new row, this is not correct, it should be more complicated */

    cur_row_p = (fort_row_t**)vector_at(table->rows, row);

    destroy_row(*cur_row_p);
    *cur_row_p = new_row;
    size_t new_cols = columns_in_row(new_row);
    table->cur_col += new_cols;
    return new_cols;

clear:
    destroy_row(new_row);
    return -1;
}


#if defined(FORT_CLANG_COMPILER) || defined(FORT_GCC_COMPILER)
#define FT_PRINTF ft_printf
#define FT_PRINTF_LN ft_printf_ln
#define FT_HDR_PRINTF ft_hdr_printf
#define FT_HDR_PRINTF_LN ft_hdr_printf_ln
#else
#define FT_PRINTF ft_printf_impl
#define FT_PRINTF_LN ft_printf_ln_impl
#define FT_HDR_PRINTF ft_hdr_printf_impl
#define FT_HDR_PRINTF_LN ft_hdr_printf_ln_impl
#endif

int FT_HDR_PRINTF(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...)
{
    assert(table);
    assert(fmt);

    va_list va;
    va_start(va, fmt);
    int result = ft_row_printf_impl(table, table->cur_row, fmt, &va);
    va_end(va);
    if (result >= 0 && table->rows) {
        int sz = vector_size(table->rows);
        if (sz != 0) {
            set_row_type(*(fort_row_t**)vector_at(table->rows, sz - 1), Header);
        }
    }
    return result;
}

int FT_HDR_PRINTF_LN(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...)
{
    assert(table);
    assert(fmt);

    va_list va;
    va_start(va, fmt);
    int result = ft_row_printf_impl(table, table->cur_row, fmt, &va);
    va_end(va);
    if (result >= 0 && table->rows) {
        int sz = vector_size(table->rows);
        if (sz != 0) {
            set_row_type(*(fort_row_t**)vector_at(table->rows, sz - 1), Header);
        }
    }
    if (result >= 0) {
        ft_ln(table);
    }
    return result;
}

int FT_PRINTF(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...)
{
    assert(table);
    va_list va;
    va_start(va, fmt);
    int result = ft_row_printf_impl(table, table->cur_row, fmt, &va);
    va_end(va);
    return result;
}

int FT_PRINTF_LN(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...)
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


int ft_write(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT cell_content)
{
    assert(table);
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
    assert(table);
    int status = ft_write(table, cell_content);
    if (IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}

FORT_EXTERN int ft_nwrite(FTABLE *FORT_RESTRICT table, size_t n, const char* FORT_RESTRICT cell_content, ...)
{
    assert(table);
    int status = ft_write(table, cell_content);
    if (IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --n;
    for (size_t i = 0; i < n; ++i) {
        const char *cell = va_arg(va, const char*);
        status = ft_write(table, cell);
        if (IS_ERROR(status))
            return status;
    }
    va_end(va);
    return status;
}

FORT_EXTERN int ft_nwrite_ln(FTABLE *FORT_RESTRICT table, size_t n, const char* FORT_RESTRICT cell_content, ...)
{
    assert(table);
    int status = ft_write(table, cell_content);
    if (IS_ERROR(status))
        return status;

    va_list va;
    va_start(va, cell_content);
    --n;
    for (size_t i = 0; i < n; ++i) {
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


FORT_EXTERN int ft_write_status(FTABLE *FORT_RESTRICT table, int status, const char* FORT_RESTRICT cell_content)
{
    assert(table);
    if (IS_ERROR(status))
        return status;

    return ft_write(table, cell_content);
}



FORT_EXTERN int ft_row_write(FTABLE *FORT_RESTRICT table, size_t cols, const char* FORT_RESTRICT cells[])
{
    assert(table);
    for (size_t i = 0; i < cols; ++i) {
        int status = ft_write(table, cells[i]);
        if (IS_ERROR(status)) {
            //todo: maybe current pos in case of error should be equal to the one before function call?
            return status;
        }
    }
    return F_SUCCESS;
}

FORT_EXTERN int ft_row_write_ln(FTABLE *FORT_RESTRICT table, size_t cols, const char* FORT_RESTRICT cells[])
{
    assert(table);
    int status = ft_row_write(table, cols, cells);
    if (IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}



//FORT_EXTERN int ft_table_write(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char** FORT_RESTRICT table_cells[]);
//FORT_EXTERN int ft_table_write_ln(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char** FORT_RESTRICT table_cells[]);


//int ft_table_write(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char* FORT_RESTRICT table_cells[rows][cols]);
//int ft_table_write_ln(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char** FORT_RESTRICT table_cells[rows][cols]);

int ft_s_table_write(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char* FORT_RESTRICT table_cells[rows][cols])
{
    assert(table);
    for (size_t i = 0; i < rows; ++i) {
        int status = ft_row_write(table, cols, table_cells[i]);
        if (IS_ERROR(status)) {
            //todo: maybe current pos in case of error should be equal to the one before function call?
            return status;
        }
        if (i != rows - 1)
            ft_ln(table);
    }
    return F_SUCCESS;
}

int ft_s_table_write_ln(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char* FORT_RESTRICT table_cells[rows][cols])
{
    assert(table);
    int status = ft_s_table_write(table, rows, cols, table_cells);
    if (IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}


int ft_table_write(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char* * FORT_RESTRICT table_cells[rows])
{
    assert(table);
    for (size_t i = 0; i < rows; ++i) {
        int status = ft_row_write(table, cols, table_cells[i]);
        if (IS_ERROR(status)) {
            //todo: maybe current pos in case of error should be equal to the one before function call?
            return status;
        }
        if (i != rows - 1)
            ft_ln(table);
    }
    return F_SUCCESS;
}

int ft_table_write_ln(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char* * FORT_RESTRICT table_cells[rows])
{
    assert(table);
    int status = ft_table_write(table, rows, cols, table_cells);
    if (IS_SUCCESS(status)) {
        ft_ln(table);
    }
    return status;
}





//int ft_set_default_options(const fort_table_options_t *options)
//{
//    memcpy(&g_table_options, options, sizeof(fort_table_options_t));
//    return 0;
//}

//int ft_get_default_options(fort_table_options_t *options)
//{
//    memcpy(options, &g_table_options, sizeof(fort_table_options_t));
//    return 0;
//}

//int ft_set_table_options(FTABLE * FORT_RESTRICT table, const fort_table_options_t * FORT_RESTRICT options)
//{
//    assert(table);
//    if (options == NULL) {
//        destroy_table_options(table->options);
//        table->options = NULL;
//        return 0;
//    }



//    fort_table_options_t *new_options = copy_table_options(options);
//    if (new_options == NULL) {
//        return -1;
//    }
//    destroy_table_options(table->options);
//    table->options = new_options;
//    return 0;


////    fort_table_options_t *new_options = F_CALLOC(sizeof(fort_table_options_t), 1);
////    if (new_options == NULL) {
////        return -1;
////    }
////    memcpy(new_options, options, sizeof(fort_table_options_t));
////    F_FREE(table->options);
////    table->options = new_options;
////    return 0;
//}



//int ft_set_column_min_width(FTABLE *table, size_t column, size_t width)
//{
//    if (table->options == NULL) {
//        table->options = create_table_options();
//        if (table->options == NULL)
//            return F_MEMORY_ERROR;
//    }

//    int status = fort_options_set_column_min_width(table->options, column, width);
//    return status;
//}

int ft_set_column_min_width(FTABLE *table, size_t column, size_t width)
{
    if (table->options == NULL) {
        table->options = create_table_options();
        if (table->options == NULL)
            return F_MEMORY_ERROR;
    }
    if (table->options->cell_options == NULL) {
        table->options->cell_options = create_cell_opt_container();
        if (table->options->cell_options == NULL) {
            return F_ERROR;
        }
    }
    return set_cell_option(table->options->cell_options, FT_ANY_ROW, column, FT_OPT_MIN_WIDTH, width);
}

//int ft_set_column_alignment(FTABLE * FORT_RESTRICT table, size_t column, enum TextAlignment align)
//{
//    if (table->options == NULL) {
//        table->options = create_table_options();
//        if (table->options == NULL)
//            return F_MEMORY_ERROR;
//    }

//    int status = fort_options_set_column_alignment(table->options, column, align);
//    return status;
//}

int ft_set_column_alignment(FTABLE *table, size_t column, enum TextAlignment align)
{
    if (table->options == NULL) {
        table->options = create_table_options();
        if (table->options == NULL)
            return F_MEMORY_ERROR;
    }
    if (table->options->cell_options == NULL) {
        table->options->cell_options = create_cell_opt_container();
        if (table->options->cell_options == NULL) {
            return F_ERROR;
        }
    }
    return set_cell_option(table->options->cell_options, FT_ANY_ROW, column, FT_OPT_TEXT_ALIGN, align);
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
    separator_t *cur_sep = NULL;
    size_t sep_size = vector_size(table->separators);
    size_t i = 0;
    for (i = 0; i < rows; ++i) {
        cur_sep = (i < sep_size) ? (*(separator_t **)vector_at(table->separators, i)) : NULL;
        cur_row = *(fort_row_t**)vector_at(table->rows, i);
        enum HorSeparatorPos separatorPos = (i == 0) ? TopSeparator : InsideSeparator;
        CHECK_RESULT_AND_MOVE_DEV(print_row_separator(buffer + dev, sz - dev, col_width_arr, cols, prev_row, cur_row, separatorPos, cur_sep, context));
        CHECK_RESULT_AND_MOVE_DEV(snprintf_row(cur_row, buffer + dev, sz - dev, col_width_arr, cols, row_height_arr[i], context));
        prev_row = cur_row;
    }
    cur_row = NULL;
    cur_sep = (i < sep_size) ? (*(separator_t **)vector_at(table->separators, i)) : NULL;
    CHECK_RESULT_AND_MOVE_DEV(print_row_separator(buffer + dev, sz - dev, col_width_arr, cols, prev_row, cur_row, BottomSeparator, cur_sep, context));


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




int ft_add_separator(FTABLE *table)
{
    assert(table);
    assert(table->separators);

    while(vector_size(table->separators) <= table->cur_row) {
        separator_t *sep_p = create_separator(F_FALSE);
        if (sep_p == NULL)
            return F_MEMORY_ERROR;
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
        return F_ERROR;
    return F_SUCCESS;
}

int ft_set_default_option(uint32_t option, int value)
{
    switch (option) {
    case FT_OPT_TOP_PADDING:
            g_table_options.cell_padding_top = value;
        break;
    case FT_OPT_BOTTOM_PADDING:
            g_table_options.cell_padding_bottom = value;
        break;
    case FT_OPT_LEFT_PADDING:
            g_table_options.cell_padding_left = value;
        break;
    case FT_OPT_RIGHT_PADDING:
            g_table_options.cell_padding_right = value;
        break;
    case FT_OPT_EMPTY_STR_HEIGHT:
            g_table_options.cell_empty_string_height = value;
        break;
    default:
        // todo
        exit(22);
    }
    return F_SUCCESS;
}


static void set_border_options_for_options(fort_table_options_t *options, struct border_chars *border_chs, struct border_chars *header_border_chs)
{
#define BOR_CHARS options->border_chars
#define H_BOR_CHARS options->header_border_chars

//        BOR_CHARS[TL_bip] = BOR_CHARS[TT_bip] = BOR_CHARS[TV_bip] = BOR_CHARS[TR_bip] = border_chs->top_border_ch;
//        BOR_CHARS[LH_bip] = BOR_CHARS[IH_bip] = BOR_CHARS[II_bip] = BOR_CHARS[RH_bip] = border_chs->separator_ch;
//        BOR_CHARS[BL_bip] = BOR_CHARS[BB_bip] = BOR_CHARS[BV_bip] = BOR_CHARS[BR_bip] = border_chs->bottom_border_ch;
//        BOR_CHARS[LL_bip] = BOR_CHARS[IV_bip] = BOR_CHARS[RR_bip] = border_chs->side_border_ch;

//    H_BOR_CHARS[TL_bip] = H_BOR_CHARS[TT_bip] = H_BOR_CHARS[TV_bip] = H_BOR_CHARS[TR_bip] = header_border_chs->top_border_ch;
//    H_BOR_CHARS[LH_bip] = H_BOR_CHARS[IH_bip] = H_BOR_CHARS[II_bip] = H_BOR_CHARS[RH_bip] = header_border_chs->separator_ch;
//    H_BOR_CHARS[BL_bip] = H_BOR_CHARS[BB_bip] = H_BOR_CHARS[BV_bip] = H_BOR_CHARS[BR_bip] = header_border_chs->bottom_border_ch;
//    H_BOR_CHARS[LL_bip] = H_BOR_CHARS[IV_bip] = H_BOR_CHARS[RR_bip] = header_border_chs->side_border_ch;

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

#undef BOR_CHARS
#undef H_BOR_CHARS
}

int ft_set_default_borders(struct border_chars *border_chs, struct border_chars *header_border_chs)
{
    set_border_options_for_options(&g_table_options, border_chs, header_border_chs);
    return F_SUCCESS;
}

int ft_set_table_borders(FTABLE *table, struct border_chars *border_chs, struct border_chars *header_border_chs)
{
    assert(table);
    if (table->options == NULL) {
        table->options = create_table_options();
        if (table->options == NULL)
            return F_MEMORY_ERROR;
    }
    set_border_options_for_options(table->options, border_chs, header_border_chs);
    return F_SUCCESS;
}

int ft_set_table_option(FTABLE *table, uint32_t option, int value)
{
    assert(table);
    if (table->options == NULL) {
        table->options = create_table_options();
        if (table->options == NULL)
            return F_MEMORY_ERROR;
    }
    switch (option) {
    case FT_OPT_TOP_PADDING:
            table->options->cell_padding_top = value;
        break;
    case FT_OPT_BOTTOM_PADDING:
            table->options->cell_padding_bottom = value;
        break;
    case FT_OPT_LEFT_PADDING:
            table->options->cell_padding_left = value;
        break;
    case FT_OPT_RIGHT_PADDING:
            table->options->cell_padding_right = value;
        break;
    case FT_OPT_EMPTY_STR_HEIGHT:
            table->options->cell_empty_string_height = value;
        break;
    default:
        // todo
        exit(22);
    }
    return F_SUCCESS;

}


