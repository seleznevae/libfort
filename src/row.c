#include "row.h"
#include "cell.h"
#include "string_buffer.h"
#include "assert.h"
#include "vector.h"
#include "ctype.h"

struct fort_row {
    vector_t *cells;
    /*enum ft_row_type type;*/
};



fort_row_t *create_row(void)
{
    fort_row_t *row = (fort_row_t *)F_CALLOC(sizeof(fort_row_t), 1);
    if (row == NULL)
        return NULL;
    row->cells = create_vector(sizeof(fort_cell_t *), DEFAULT_VECTOR_CAPACITY);
    if (row->cells == NULL) {
        F_FREE(row);
        return NULL;
    }

    /*
    row->is_header = F_FALSE;
    row->type = FT_ROW_COMMON;
    */
    return row;
}

void destroy_row(fort_row_t *row)
{
    size_t i = 0;
    if (row == NULL)
        return;

    if (row->cells) {
        size_t cells_n = vector_size(row->cells);
        for (i = 0; i < cells_n; ++i) {
            fort_cell_t *cell = *(fort_cell_t **)vector_at(row->cells, i);
            destroy_cell(cell);
        }
        destroy_vector(row->cells);
    }

    F_FREE(row);
}





unsigned int columns_in_row(const fort_row_t *row)
{
    if (row == NULL || row->cells == NULL)
        return 0;

    return vector_size(row->cells);
}


fort_cell_t *get_cell_implementation(fort_row_t *row, size_t col, enum PolicyOnNull policy)
{
    if (row == NULL || row->cells == NULL) {
        return NULL;
    }

    switch (policy) {
        case DoNotCreate:
            if (col < columns_in_row(row)) {
                return *(fort_cell_t **)vector_at(row->cells, col);
            }
            return NULL;
            break;
        case Create:
            while (col >= columns_in_row(row)) {
                fort_cell_t *new_cell = create_cell();
                if (new_cell == NULL)
                    return NULL;
                if (IS_ERROR(vector_push(row->cells, &new_cell))) {
                    destroy_cell(new_cell);
                    return NULL;
                }
            }
            return *(fort_cell_t **)vector_at(row->cells, col);
            break;
    }
    return NULL;
}


fort_cell_t *get_cell(fort_row_t *row, size_t col)
{
    return get_cell_implementation(row, col, DoNotCreate);
}

const fort_cell_t *get_cell_c(const fort_row_t *row, size_t col)
{
    return get_cell((fort_row_t *)row, col);
}

fort_cell_t *get_cell_and_create_if_not_exists(fort_row_t *row, size_t col)
{
    return get_cell_implementation(row, col, Create);
}





int print_row_separator(char *buffer, size_t buffer_sz,
                        const size_t *col_width_arr, size_t cols,
                        const fort_row_t *upper_row, const fort_row_t *lower_row,
                        enum HorSeparatorPos separatorPos,
                        const separator_t *sep, const context_t *context)
{
    typedef char char_type;
    char new_line_char = '\n';
    int (*snprint_n_chars_)(char *, size_t, size_t, char) = snprint_n_chars;
    char space_char = ' ';

    assert(buffer);
    assert(context);

    int written = 0;
    int tmp = 0;

    enum ft_row_type lower_row_type = FT_ROW_COMMON;
    if (lower_row != NULL) {
        lower_row_type = (enum ft_row_type)get_cell_opt_value_hierarcial(context->table_options, context->row, FT_ANY_COLUMN, FT_COPT_ROW_TYPE);
    }
    enum ft_row_type upper_row_type = FT_ROW_COMMON;
    if (upper_row != NULL) {
        upper_row_type = (enum ft_row_type)get_cell_opt_value_hierarcial(context->table_options, context->row - 1, FT_ANY_COLUMN, FT_COPT_ROW_TYPE);
    }


    /*  Row separator anatomy
     *
     *  L  I  I  I  IV  I   I   I  R
     */
    const char *L = NULL;
    const char *I = NULL;
    const char *IV = NULL;
    const char *R = NULL;

    typedef const char (*border_chars_point_t)[BorderItemPosSize];
    const char (*border_chars)[BorderItemPosSize] = NULL;
    border_chars = (border_chars_point_t)&context->table_options->border_style.border_chars;
    if (upper_row_type == FT_ROW_HEADER || lower_row_type == FT_ROW_HEADER) {
        border_chars = (border_chars_point_t)&context->table_options->border_style.header_border_chars;
    }

    if (sep && sep->enabled) {
        L = &(context->table_options->border_style.separator_chars[LH_sip]);
        I = &(context->table_options->border_style.separator_chars[IH_sip]);
        IV = &(context->table_options->border_style.separator_chars[II_sip]);
        R = &(context->table_options->border_style.separator_chars[RH_sip]);
    } else {
        switch (separatorPos) {
            case TopSeparator:
                L = &(*border_chars)[TL_bip];
                I = &(*border_chars)[TT_bip];
                IV = &(*border_chars)[TV_bip];
                R = &(*border_chars)[TR_bip];
                break;
            case InsideSeparator:
                L = &(*border_chars)[LH_bip];
                I = &(*border_chars)[IH_bip];
                IV = &(*border_chars)[II_bip];
                R = &(*border_chars)[RH_bip];
                break;
            case BottomSeparator:
                L = &(*border_chars)[BL_bip];
                I = &(*border_chars)[BB_bip];
                IV = &(*border_chars)[BV_bip];
                R = &(*border_chars)[BR_bip];
                break;
            default:
                break;
        }
    }

    /* If all chars are not printable, skip line separator */  /* todo: add processing for wchar_t */
    if (!isprint(*L) && !isprint(*I) && !isprint(*IV) && !isprint(*R))
        return 0;

    size_t i = 0;

    /* Print left margin */
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, context->table_options->entire_table_options.left_margin, space_char));

    for (i = 0; i < cols; ++i) {
        if (i == 0) {
            CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, 1, (char_type)*L));
        } else {
            CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, 1, (char_type)*IV));
        }
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, col_width_arr[i], (char_type)*I));
    }
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, 1, (char_type)*R));

    /* Print right margin */
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, context->table_options->entire_table_options.right_margin, space_char));

    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, 1, new_line_char));

    return written;

clear:
    return -1;
}



int wprint_row_separator(wchar_t *buffer, size_t buffer_sz,
                         const size_t *col_width_arr, size_t cols,
                         const fort_row_t *upper_row, const fort_row_t *lower_row,
                         enum HorSeparatorPos separatorPos, const separator_t *sep,
                         const context_t *context)
{
    typedef wchar_t char_type;
    char new_line_char = L'\n';
    int (*snprint_n_chars_)(wchar_t *, size_t, size_t, wchar_t) = wsnprint_n_chars;
    wchar_t space_char = L' ';

    assert(buffer);
    assert(context);

    int written = 0;
    int tmp = 0;

    enum ft_row_type lower_row_type = FT_ROW_COMMON;
    if (lower_row != NULL) {
        lower_row_type = (enum ft_row_type)get_cell_opt_value_hierarcial(context->table_options, context->row, FT_ANY_COLUMN, FT_COPT_ROW_TYPE);
    }
    enum ft_row_type upper_row_type = FT_ROW_COMMON;
    if (upper_row != NULL) {
        upper_row_type = (enum ft_row_type)get_cell_opt_value_hierarcial(context->table_options, context->row - 1, FT_ANY_COLUMN, FT_COPT_ROW_TYPE);
    }

    /*  Row separator anatomy
     *
     *  L  I  I  I  IV  I   I   I  R
     */
    const char *L = NULL;
    const char *I = NULL;
    const char *IV = NULL;
    const char *R = NULL;

    typedef const char (*border_chars_point_t)[BorderItemPosSize];
    const char (*border_chars)[BorderItemPosSize] = NULL;
    border_chars = (border_chars_point_t)&context->table_options->border_style.border_chars;
    if (upper_row_type == FT_ROW_HEADER || lower_row_type == FT_ROW_HEADER) {
        border_chars = (border_chars_point_t)&context->table_options->border_style.header_border_chars;
    }


    if (sep && sep->enabled) {
        L = &(context->table_options->border_style.separator_chars[LH_sip]);
        I = &(context->table_options->border_style.separator_chars[IH_sip]);
        IV = &(context->table_options->border_style.separator_chars[II_sip]);
        R = &(context->table_options->border_style.separator_chars[RH_sip]);
    } else {
        switch (separatorPos) {
            case TopSeparator:
                L = &(*border_chars)[TL_bip];
                I = &(*border_chars)[TT_bip];
                IV = &(*border_chars)[TV_bip];
                R = &(*border_chars)[TR_bip];
                break;
            case InsideSeparator:
                L = &(*border_chars)[LH_bip];
                I = &(*border_chars)[IH_bip];
                IV = &(*border_chars)[II_bip];
                R = &(*border_chars)[RH_bip];
                break;
            case BottomSeparator:
                L = &(*border_chars)[BL_bip];
                I = &(*border_chars)[BB_bip];
                IV = &(*border_chars)[BV_bip];
                R = &(*border_chars)[BR_bip];
                break;
            default:
                break;
        }
    }

    /* If all chars are not printable, skip line separator */  /* todo: add processing for wchar_t */
    if (!isprint(*L) && !isprint(*I) && !isprint(*IV) && !isprint(*R))
        return 0;

    size_t i = 0;

    /* Print left margin */
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, context->table_options->entire_table_options.left_margin, space_char));

    for (i = 0; i < cols; ++i) {
        if (i == 0) {
            CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, 1, (char_type)*L));
        } else {
            CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, 1, (char_type)*IV));
        }
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, col_width_arr[i], (char_type)*I));
    }
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, 1, (char_type)*R));

    /* Print right margin */
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, context->table_options->entire_table_options.right_margin, space_char));

    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buffer_sz - written, 1, new_line_char));

    return written;

clear:
    return -1;
}



fort_row_t *create_row_from_string(const char *str)
{
    char *pos = NULL;
    char *base_pos = NULL;
    unsigned int number_of_separators = 0;

    fort_row_t *row = create_row();
    if (row == NULL)
        return NULL;

    if (str == NULL)
        return row;

    char *str_copy = F_STRDUP(str);
    if (str_copy == NULL)
        goto clear;

    pos = str_copy;
    base_pos = str_copy;
    number_of_separators = 0;
    while (*pos) {
        pos = strchr(pos, FORT_COL_SEPARATOR);
        if (pos != NULL) {
            *(pos) = '\0';
            ++pos;
            number_of_separators++;
        }

        fort_cell_t *cell = create_cell();
        if (cell == NULL)
            goto clear;

        /*        int status = fill_buffer_from_string(cell->str_buffer, base_pos);  */
        int status = fill_cell_from_string(cell, base_pos);
        if (IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if (IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        if (pos == NULL)
            break;
        base_pos = pos;
    }

    /* special case if in format string last cell is empty */
    while (vector_size(row->cells) < (number_of_separators + 1)) {
        fort_cell_t *cell = create_cell();
        if (cell == NULL)
            goto clear;

        /*        int status = fill_buffer_from_string(cell->str_buffer, "");  */
        int status = fill_cell_from_string(cell, "");
        if (IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if (IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }
    }

    F_FREE(str_copy);
    return row;

clear:
    destroy_row(row);
    F_FREE(str_copy);
    return NULL;
}




fort_row_t *create_row_from_fmt_string(const char  *fmt, va_list *va_args)
{
    string_buffer_t *buffer = create_string_buffer(DEFAULT_STR_BUF_SIZE, CharBuf);
    if (buffer == NULL)
        return NULL;

    size_t cols_origin = number_of_columns_in_format_string(fmt);
    size_t cols = 0;

    while (1) {
        va_list va;
        va_copy(va, *va_args);
        int virtual_sz = vsnprintf(buffer->str.cstr, string_buffer_capacity(buffer)/*buffer->str_sz*/, fmt, va);
        va_end(va);
        /* If error encountered */
        if (virtual_sz < 0)
            goto clear;

        /* Successful write */
        if ((size_t)virtual_sz < string_buffer_capacity(buffer))
            break;

        /* Otherwise buffer was too small, so incr. buffer size ant try again. */
        if (!IS_SUCCESS(realloc_string_buffer_without_copy(buffer)))
            goto clear;
    }

    cols = number_of_columns_in_format_string(buffer->str.cstr);
    if (cols == cols_origin) {

        fort_row_t *row = create_row_from_string(buffer->str.cstr);
        if (row == NULL) {
            goto clear;
        }

        destroy_string_buffer(buffer);
        return row;
    }

    /* todo: add processing of cols != cols_origin */

clear:
    destroy_string_buffer(buffer);
    return NULL;
}


int snprintf_row(const fort_row_t *row, char *buffer, size_t buf_sz, size_t *col_width_arr, size_t col_width_arr_sz,
                 size_t row_height, const context_t *context)
{
    typedef char char_type;
    char space_char = ' ';
    char new_line_char = '\n';
    int (*snprint_n_chars_)(char *, size_t, size_t, char) = snprint_n_chars;
    int (*cell_printf_)(fort_cell_t *, size_t, char *, size_t, const context_t *) = cell_printf;


    assert(context);
    if (row == NULL)
        return -1;

    unsigned int cols_in_row = columns_in_row(row);
    if (cols_in_row > col_width_arr_sz)
        return -1;

    /*  Row separator anatomy
     *
     *  L    data    IV    data   IV   data    R
     */

    typedef const char (*border_chars_point_t)[BorderItemPosSize];
    enum ft_row_type row_type = (enum ft_row_type)get_cell_opt_value_hierarcial(context->table_options, context->row, FT_ANY_COLUMN, FT_COPT_ROW_TYPE);
    const char (*bord_chars)[BorderItemPosSize] = (row_type == FT_ROW_HEADER)
            ? (border_chars_point_t)(&context->table_options->border_style.header_border_chars)
            : (border_chars_point_t)(&context->table_options->border_style.border_chars);
    const char *L = &(*bord_chars)[LL_bip];
    const char *IV = &(*bord_chars)[IV_bip];
    const char *R = &(*bord_chars)[RR_bip];


    int written = 0;
    int tmp = 0;
    size_t i = 0;
    for (i = 0; i < row_height; ++i) {
        /* Print left margin */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, context->table_options->entire_table_options.left_margin, space_char));

        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, 1, (char_type)*L));
        size_t j = 0;
        for (j = 0; j < col_width_arr_sz; ++j) {
            ((context_t *)context)->column = j;
            if (j < cols_in_row) {
                fort_cell_t *cell = *(fort_cell_t **)vector_at(row->cells, j);
                CHCK_RSLT_ADD_TO_WRITTEN(cell_printf_(cell, i, buffer + written, col_width_arr[j] + 1, context));
            } else {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, col_width_arr[j], space_char));
            }
            if (j == col_width_arr_sz - 1) {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, 1, (char_type)*R));
            } else {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, 1, (char_type)*IV));
            }
        }

        /* Print right margin */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, context->table_options->entire_table_options.right_margin, space_char));

        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, 1, new_line_char));
    }
    return written;

clear:
    return -1;
}



int wsnprintf_row(const fort_row_t *row, wchar_t *buffer, size_t buf_sz, size_t *col_width_arr, size_t col_width_arr_sz,
                  size_t row_height, const context_t *context)
{
    typedef wchar_t char_type;
    char space_char = L' ';
    char new_line_char = L'\n';
    int (*snprint_n_chars_)(wchar_t *, size_t, size_t, wchar_t) = wsnprint_n_chars;
    int (*cell_printf_)(fort_cell_t *, size_t, wchar_t *, size_t, const context_t *) = cell_wprintf;


    assert(context);
    if (row == NULL)
        return -1;

    unsigned int cols_in_row = columns_in_row(row);
    if (cols_in_row > col_width_arr_sz)
        return -1;

    /*  Row separator anatomy
     *
     *  L    data    IV    data   IV   data    R
     */

    typedef const char (*border_chars_point_t)[BorderItemPosSize];
    enum ft_row_type row_type = (enum ft_row_type)get_cell_opt_value_hierarcial(context->table_options, context->row, FT_ANY_COLUMN, FT_COPT_ROW_TYPE);
    const char (*bord_chars)[BorderItemPosSize] = (row_type)
            ? (border_chars_point_t)(&context->table_options->border_style.header_border_chars)
            : (border_chars_point_t)(&context->table_options->border_style.border_chars);
    const char *L = &(*bord_chars)[LL_bip];
    const char *IV = &(*bord_chars)[IV_bip];
    const char *R = &(*bord_chars)[RR_bip];


    int written = 0;
    int tmp = 0;
    size_t i = 0;
    for (i = 0; i < row_height; ++i) {
        /* Print left margin */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, context->table_options->entire_table_options.left_margin, space_char));

        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, 1, (char_type)*L));
        size_t j = 0;
        for (j = 0; j < col_width_arr_sz; ++j) {
            ((context_t *)context)->column = j;
            if (j < cols_in_row) {
                fort_cell_t *cell = *(fort_cell_t **)vector_at(row->cells, j);
                CHCK_RSLT_ADD_TO_WRITTEN(cell_printf_(cell, i, buffer + written, col_width_arr[j] + 1, context));
            } else {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, col_width_arr[j], space_char));
            }
            if (j == col_width_arr_sz - 1) {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, 1, (char_type)*R));
            } else {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, 1, (char_type)*IV));
            }
        }

        /* Print right margin */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, context->table_options->entire_table_options.right_margin, space_char));

        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buffer + written, buf_sz - written, 1, new_line_char));
    }
    return written;

clear:
    return -1;
}

