#include <assert.h>
#include <ctype.h>
#include "row.h"
#include "cell.h"
#include "string_buffer.h"
#include "vector.h"


struct fort_row {
    vector_t *cells;
    /*enum ft_row_type type;*/
};


FT_INTERNAL
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

FT_INTERNAL
void destroy_row(fort_row_t *row)
{
    if (row == NULL)
        return;

    if (row->cells) {
        size_t cells_n = vector_size(row->cells);
        for (size_t i = 0; i < cells_n; ++i) {
            fort_cell_t *cell = *(fort_cell_t **)vector_at(row->cells, i);
            destroy_cell(cell);
        }
        destroy_vector(row->cells);
    }

    F_FREE(row);
}

FT_INTERNAL
fort_row_t *copy_row(fort_row_t *row)
{
    assert(row);
    fort_row_t *result = create_row();
    if (result == NULL)
        return NULL;

    size_t cols_n = vector_size(row->cells);
    for (size_t i = 0; i < cols_n; ++i) {
        fort_cell_t *cell = *(fort_cell_t **)vector_at(row->cells, i);
        fort_cell_t *new_cell = copy_cell(cell);
        if (new_cell == NULL) {
            destroy_row(result);
            return NULL;
        }
        vector_push(result->cells, &new_cell);
    }

    return result;
}

FT_INTERNAL
size_t columns_in_row(const fort_row_t *row)
{
    if (row == NULL || row->cells == NULL)
        return 0;

    return vector_size(row->cells);
}


static
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
                if (FT_IS_ERROR(vector_push(row->cells, &new_cell))) {
                    destroy_cell(new_cell);
                    return NULL;
                }
            }
            return *(fort_cell_t **)vector_at(row->cells, col);
            break;
    }
    return NULL;
}


FT_INTERNAL
fort_cell_t *get_cell(fort_row_t *row, size_t col)
{
    return get_cell_implementation(row, col, DoNotCreate);
}


FT_INTERNAL
const fort_cell_t *get_cell_c(const fort_row_t *row, size_t col)
{
    return get_cell((fort_row_t *)row, col);
}


FT_INTERNAL
fort_cell_t *get_cell_and_create_if_not_exists(fort_row_t *row, size_t col)
{
    return get_cell_implementation(row, col, Create);
}


FT_INTERNAL
fort_status_t swap_row(fort_row_t *cur_row, fort_row_t *ins_row, size_t pos)
{
    assert(cur_row);
    assert(ins_row);
    size_t cur_sz = vector_size(cur_row->cells);
    if (cur_sz == 0 && pos == 0) {
        fort_row_t tmp;
        memcpy(&tmp, cur_row, sizeof(fort_row_t));
        memcpy(cur_row, ins_row, sizeof(fort_row_t));
        memcpy(ins_row, &tmp, sizeof(fort_row_t));
        return FT_SUCCESS;
    }

    return vector_swap(cur_row->cells, ins_row->cells, pos);
}


FT_INTERNAL
size_t group_cell_number(const fort_row_t *row, size_t master_cell_col)
{
    assert(row);
    const fort_cell_t *master_cell = get_cell_c(row, master_cell_col);
    if (master_cell == NULL)
        return 0;

    if (get_cell_type(master_cell) != GroupMasterCell)
        return 1;

    size_t total_cols = vector_size(row->cells);
    size_t slave_col = master_cell_col + 1;
    while (slave_col < total_cols) {
        const fort_cell_t *cell = get_cell_c(row, slave_col);
        if (cell && get_cell_type(cell) == GroupSlaveCell) {
            ++slave_col;
        } else {
            break;
        }
    }
    return slave_col - master_cell_col;
}


FT_INTERNAL
int get_row_cell_types(const fort_row_t *row, enum CellType *types, size_t types_sz)
{
    assert(row);
    assert(types);
    size_t i = 0;
    for (i = 0; i < types_sz; ++i) {
        const fort_cell_t *cell = get_cell_c(row, i);
        if (cell) {
            types[i] = get_cell_type(cell);
        } else {
            types[i] = CommonCell;
        }
    }
    return FT_SUCCESS;
}


FT_INTERNAL
fort_status_t row_set_cell_span(fort_row_t *row, size_t cell_column, size_t hor_span)
{
    assert(row);

    if (hor_span < 2)
        return FT_EINVAL;

    fort_cell_t *main_cell = get_cell_and_create_if_not_exists(row, cell_column);
    if (main_cell == NULL) {
        return FT_ERROR;
    }
    set_cell_type(main_cell, GroupMasterCell);
    --hor_span;
    ++cell_column;

    while (hor_span) {
        fort_cell_t *slave_cell = get_cell_and_create_if_not_exists(row, cell_column);
        if (slave_cell == NULL) {
            return FT_ERROR;
        }
        set_cell_type(slave_cell, GroupSlaveCell);
        --hor_span;
        ++cell_column;
    }

    return FT_SUCCESS;
}


FT_INTERNAL
int print_row_separator(char *buffer, size_t buffer_sz,
                        const size_t *col_width_arr, size_t cols,
                        const fort_row_t *upper_row, const fort_row_t *lower_row,
                        enum HorSeparatorPos separatorPos,
                        const separator_t *sep, const context_t *context)
{
    int (*snprint_n_strings_)(char *, size_t, size_t, const char *) = snprint_n_strings;

    assert(buffer);
    assert(context);

    const char *space_char = " ";
    int status = -1;

    /* Get cell types
     *
     * Regions above top row and below bottom row areconsidered full of virtual
     * GroupSlaveCell cells
     */
    enum CellType *top_row_types = (enum CellType *)F_MALLOC(sizeof(enum CellType) * cols * 2);
    if (top_row_types == NULL) {
        return FT_MEMORY_ERROR;
    }
    enum CellType *bottom_row_types = top_row_types + cols;
    if (upper_row) {
        get_row_cell_types(upper_row, top_row_types, cols);
    } else {
        size_t i = 0;
        for (i = 0; i < cols; ++i)
            top_row_types[i] = GroupSlaveCell;
    }
    if (lower_row) {
        get_row_cell_types(lower_row, bottom_row_types, cols);
    } else {
        size_t i = 0;
        for (i = 0; i < cols; ++i)
            bottom_row_types[i] = GroupSlaveCell;
    }


    int written = 0;
    int tmp = 0;

    enum ft_row_type lower_row_type = FT_ROW_COMMON;
    if (lower_row != NULL) {
        lower_row_type = (enum ft_row_type)get_cell_property_value_hierarcial(context->table_properties, context->row, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
    }
    enum ft_row_type upper_row_type = FT_ROW_COMMON;
    if (upper_row != NULL) {
        upper_row_type = (enum ft_row_type)get_cell_property_value_hierarcial(context->table_properties, context->row - 1, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
    }

    /* Row separator anatomy
     *
     *  |      C11    |   C12         C13      |      C14           C15         |
     *  L  I  I  I   IV  I   I   IT  I  I  I  IB    I    I     II    I    I     R
     *  |      C21    |   C22     |   C23             C24           C25         |
    */
    const char **L = NULL;
    const char **I = NULL;
    const char **IV = NULL;
    const char **R = NULL;
    const char **IT = NULL;
    const char **IB = NULL;
    const char **II = NULL;


    typedef const char *(*border_chars_point_t)[BorderItemPosSize];
    const char *(*border_chars)[BorderItemPosSize] = NULL;
    border_chars = (border_chars_point_t)&context->table_properties->border_style.border_chars;
    if (upper_row_type == FT_ROW_HEADER || lower_row_type == FT_ROW_HEADER) {
        border_chars = (border_chars_point_t)&context->table_properties->border_style.header_border_chars;
    }

    if (sep && sep->enabled) {
        L = &(context->table_properties->border_style.separator_chars[LH_sip]);
        I = &(context->table_properties->border_style.separator_chars[IH_sip]);
        IV = &(context->table_properties->border_style.separator_chars[II_sip]);
        R = &(context->table_properties->border_style.separator_chars[RH_sip]);

        IT = &(context->table_properties->border_style.separator_chars[TI_sip]);
        IB = &(context->table_properties->border_style.separator_chars[BI_sip]);
        II = &(context->table_properties->border_style.separator_chars[IH_sip]);

        if (lower_row == NULL) {
            L = &(*border_chars)[BL_bip];
            R = &(*border_chars)[BR_bip];
        } else if (upper_row == NULL) {
            L = &(*border_chars)[TL_bip];
            R = &(*border_chars)[TR_bip];
        }
    } else {
        switch (separatorPos) {
            case TopSeparator:
                L = &(*border_chars)[TL_bip];
                I = &(*border_chars)[TT_bip];
                IV = &(*border_chars)[TV_bip];
                R = &(*border_chars)[TR_bip];

                IT = &(*border_chars)[TV_bip];
                IB = &(*border_chars)[TV_bip];
                II = &(*border_chars)[TT_bip];
                break;
            case InsideSeparator:
                L = &(*border_chars)[LH_bip];
                I = &(*border_chars)[IH_bip];
                IV = &(*border_chars)[II_bip];
                R = &(*border_chars)[RH_bip];

//                IT = &(*border_chars)[TV_bip];
//                IB = &(*border_chars)[BV_bip];
                IT = &(*border_chars)[TI_bip];
                IB = &(*border_chars)[BI_bip];
                II = &(*border_chars)[IH_bip];
                break;
            case BottomSeparator:
                L = &(*border_chars)[BL_bip];
                I = &(*border_chars)[BB_bip];
                IV = &(*border_chars)[BV_bip];
                R = &(*border_chars)[BR_bip];

                IT = &(*border_chars)[BV_bip];
                IB = &(*border_chars)[BV_bip];
                II = &(*border_chars)[BB_bip];
                break;
            default:
                break;
        }
    }

    size_t i = 0;

    /* If all chars are not printable, skip line separator */  /* todo: add processing for wchar_t */
//    if (!isprint(*L) && !isprint(*I) && !isprint(*IV) && !isprint(*R)) {
//        status = 0;
//        goto clear;
//    }
    if ((strlen(*L) == 0 || (strlen(*L) == 1 && !isprint(**L)))
        && (strlen(*I) == 0 || (strlen(*I) == 1 && !isprint(**I)))
        && (strlen(*IV) == 0 || (strlen(*IV) == 1 && !isprint(**IV)))
        && (strlen(*R) == 0 || (strlen(*R) == 1 && !isprint(**R)))) {
        status = 0;
        goto clear;
    }

    /* Print left margin */
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, context->table_properties->entire_table_properties.left_margin, space_char));

    for (i = 0; i < cols; ++i) {
        if (i == 0) {
            CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *L));
        } else {
            if ((top_row_types[i] == CommonCell || top_row_types[i] == GroupMasterCell)
                && (bottom_row_types[i] == CommonCell || bottom_row_types[i] == GroupMasterCell)) {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *IV));
            } else if (top_row_types[i] == GroupSlaveCell && bottom_row_types[i] == GroupSlaveCell) {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *II));
            } else if (top_row_types[i] == GroupSlaveCell) {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *IT));
            } else {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *IB));
            }
        }
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, col_width_arr[i], *I));
    }
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *R));

    /* Print right margin */
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, context->table_properties->entire_table_properties.right_margin, space_char));

    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, "\n"));

    status = written;

clear:
    F_FREE(top_row_types);
    return status;
}


#ifdef FT_HAVE_WCHAR
FT_INTERNAL
int wprint_row_separator(wchar_t *buffer, size_t buffer_sz,
                         const size_t *col_width_arr, size_t cols,
                         const fort_row_t *upper_row, const fort_row_t *lower_row,
                         enum HorSeparatorPos separatorPos, const separator_t *sep,
                         const context_t *context)
{
    int (*snprint_n_strings_)(wchar_t *, size_t, size_t, const char *) = wsnprint_n_string;

    assert(buffer);
    assert(context);

    const char *space_char = " ";
    int status = -1;

    /* Get cell types
     *
     * Regions above top row and below bottom row areconsidered full of virtual
     * GroupSlaveCell cells
     */
    enum CellType *top_row_types = (enum CellType *)F_MALLOC(sizeof(enum CellType) * cols * 2);
    if (top_row_types == NULL) {
        return FT_MEMORY_ERROR;
    }
    enum CellType *bottom_row_types = top_row_types + cols;
    if (upper_row) {
        get_row_cell_types(upper_row, top_row_types, cols);
    } else {
        size_t i = 0;
        for (i = 0; i < cols; ++i)
            top_row_types[i] = GroupSlaveCell;
    }
    if (lower_row) {
        get_row_cell_types(lower_row, bottom_row_types, cols);
    } else {
        size_t i = 0;
        for (i = 0; i < cols; ++i)
            bottom_row_types[i] = GroupSlaveCell;
    }


    int written = 0;
    int tmp = 0;

    enum ft_row_type lower_row_type = FT_ROW_COMMON;
    if (lower_row != NULL) {
        lower_row_type = (enum ft_row_type)get_cell_property_value_hierarcial(context->table_properties, context->row, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
    }
    enum ft_row_type upper_row_type = FT_ROW_COMMON;
    if (upper_row != NULL) {
        upper_row_type = (enum ft_row_type)get_cell_property_value_hierarcial(context->table_properties, context->row - 1, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
    }

    /* Row separator anatomy
     *
     *  |      C11    |   C12         C13      |      C14           C15         |
     *  L  I  I  I   IV  I   I   IT  I  I  I  IB    I    I     II    I    I     R
     *  |      C21    |   C22     |   C23             C24           C25         |
    */
    const char **L = NULL;
    const char **I = NULL;
    const char **IV = NULL;
    const char **R = NULL;
    const char **IT = NULL;
    const char **IB = NULL;
    const char **II = NULL;


    typedef const char *(*border_chars_point_t)[BorderItemPosSize];
    const char *(*border_chars)[BorderItemPosSize] = NULL;
    border_chars = (border_chars_point_t)&context->table_properties->border_style.border_chars;
    if (upper_row_type == FT_ROW_HEADER || lower_row_type == FT_ROW_HEADER) {
        border_chars = (border_chars_point_t)&context->table_properties->border_style.header_border_chars;
    }

    if (sep && sep->enabled) {
        L = &(context->table_properties->border_style.separator_chars[LH_sip]);
        I = &(context->table_properties->border_style.separator_chars[IH_sip]);
        IV = &(context->table_properties->border_style.separator_chars[II_sip]);
        R = &(context->table_properties->border_style.separator_chars[RH_sip]);

        IT = &(context->table_properties->border_style.separator_chars[TI_sip]);
        IB = &(context->table_properties->border_style.separator_chars[BI_sip]);
        II = &(context->table_properties->border_style.separator_chars[IH_sip]);

        if (lower_row == NULL) {
            L = &(*border_chars)[BL_bip];
            R = &(*border_chars)[BR_bip];
        } else if (upper_row == NULL) {
            L = &(*border_chars)[TL_bip];
            R = &(*border_chars)[TR_bip];
        }
    } else {
        switch (separatorPos) {
            case TopSeparator:
                L = &(*border_chars)[TL_bip];
                I = &(*border_chars)[TT_bip];
                IV = &(*border_chars)[TV_bip];
                R = &(*border_chars)[TR_bip];

                IT = &(*border_chars)[TV_bip];
                IB = &(*border_chars)[TV_bip];
                II = &(*border_chars)[TT_bip];
                break;
            case InsideSeparator:
                L = &(*border_chars)[LH_bip];
                I = &(*border_chars)[IH_bip];
                IV = &(*border_chars)[II_bip];
                R = &(*border_chars)[RH_bip];

//                IT = &(*border_chars)[TV_bip];
//                IB = &(*border_chars)[BV_bip];
                IT = &(*border_chars)[TI_bip];
                IB = &(*border_chars)[BI_bip];
                II = &(*border_chars)[IH_bip];
                break;
            case BottomSeparator:
                L = &(*border_chars)[BL_bip];
                I = &(*border_chars)[BB_bip];
                IV = &(*border_chars)[BV_bip];
                R = &(*border_chars)[BR_bip];

                IT = &(*border_chars)[BV_bip];
                IB = &(*border_chars)[BV_bip];
                II = &(*border_chars)[BB_bip];
                break;
            default:
                break;
        }
    }

    size_t i = 0;

    /* If all chars are not printable, skip line separator */  /* todo: add processing for wchar_t */
//    if (!isprint(*L) && !isprint(*I) && !isprint(*IV) && !isprint(*R)) {
//        status = 0;
//        goto clear;
//    }
    if ((strlen(*L) == 0 || (strlen(*L) == 1 && !isprint(**L)))
        && (strlen(*I) == 0 || (strlen(*I) == 1 && !isprint(**I)))
        && (strlen(*IV) == 0 || (strlen(*IV) == 1 && !isprint(**IV)))
        && (strlen(*R) == 0 || (strlen(*R) == 1 && !isprint(**R)))) {
        status = 0;
        goto clear;
    }

    /* Print left margin */
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, context->table_properties->entire_table_properties.left_margin, space_char));

    for (i = 0; i < cols; ++i) {
        if (i == 0) {
            CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *L));
        } else {
            if ((top_row_types[i] == CommonCell || top_row_types[i] == GroupMasterCell)
                && (bottom_row_types[i] == CommonCell || bottom_row_types[i] == GroupMasterCell)) {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *IV));
            } else if (top_row_types[i] == GroupSlaveCell && bottom_row_types[i] == GroupSlaveCell) {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *II));
            } else if (top_row_types[i] == GroupSlaveCell) {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *IT));
            } else {
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *IB));
            }
        }
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, col_width_arr[i], *I));
    }
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, *R));

    /* Print right margin */
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, context->table_properties->entire_table_properties.right_margin, space_char));

    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buffer_sz - written, 1, "\n"));

    status = written;

clear:
    F_FREE(top_row_types);
    return status;
}

#endif

FT_INTERNAL
fort_row_t *create_row_from_string(const char *str)
{
    typedef char char_type;
    char_type *(*strdup_)(const char_type * str) = F_STRDUP;
    const char_type zero_char = '\0';
    fort_status_t (*fill_cell_from_string_)(fort_cell_t *cell, const char *str) = fill_cell_from_string;
    const char_type *const zero_string = "";
#define STRCHR strchr

    char_type *pos = NULL;
    char_type *base_pos = NULL;
    unsigned int number_of_separators = 0;

    fort_row_t *row = create_row();
    if (row == NULL)
        return NULL;

    if (str == NULL)
        return row;

    char_type *str_copy = strdup_(str);
    if (str_copy == NULL)
        goto clear;

    pos = str_copy;
    base_pos = str_copy;
    number_of_separators = 0;
    while (*pos) {
        pos = STRCHR(pos, FORT_COL_SEPARATOR);
        if (pos != NULL) {
            *(pos) = zero_char;
            ++pos;
            number_of_separators++;
        }

        fort_cell_t *cell = create_cell();
        if (cell == NULL)
            goto clear;

        int status = fill_cell_from_string_(cell, base_pos);
        if (FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if (FT_IS_ERROR(status)) {
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

        int status = fill_cell_from_string_(cell, zero_string);
        if (FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if (FT_IS_ERROR(status)) {
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

#undef STRCHR
}


#ifdef FT_HAVE_WCHAR
FT_INTERNAL
fort_row_t *create_row_from_wstring(const wchar_t *str)
{
    typedef wchar_t char_type;
    char_type *(*strdup_)(const char_type * str) = F_WCSDUP;
    const char_type zero_char = L'\0';
    fort_status_t (*fill_cell_from_string_)(fort_cell_t *cell, const wchar_t *str) = fill_cell_from_wstring;
    const char_type *const zero_string = L"";
#define STRCHR wcschr

    char_type *pos = NULL;
    char_type *base_pos = NULL;
    unsigned int number_of_separators = 0;

    fort_row_t *row = create_row();
    if (row == NULL)
        return NULL;

    if (str == NULL)
        return row;

    char_type *str_copy = strdup_(str);
    if (str_copy == NULL)
        goto clear;

    pos = str_copy;
    base_pos = str_copy;
    number_of_separators = 0;
    while (*pos) {
        pos = STRCHR(pos, FORT_COL_SEPARATOR);
        if (pos != NULL) {
            *(pos) = zero_char;
            ++pos;
            number_of_separators++;
        }

        fort_cell_t *cell = create_cell();
        if (cell == NULL)
            goto clear;

        int status = fill_cell_from_string_(cell, base_pos);
        if (FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if (FT_IS_ERROR(status)) {
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

        int status = fill_cell_from_string_(cell, zero_string);
        if (FT_IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if (FT_IS_ERROR(status)) {
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
#undef STRCHR
}
#endif

FT_INTERNAL
fort_row_t *create_row_from_fmt_string(const char  *fmt, va_list *va_args)
{
#define VSNPRINTF vsnprintf
#define STR_FILED cstr
#define CREATE_ROW_FROM_STRING create_row_from_string
#define NUMBER_OF_COLUMNS_IN_FORMAT_STRING number_of_columns_in_format_string
#define FILL_CELL_FROM_STRING fill_cell_from_string
#define STR_BUF_TYPE CharBuf

    string_buffer_t *buffer = create_string_buffer(DEFAULT_STR_BUF_SIZE, STR_BUF_TYPE);
    if (buffer == NULL)
        return NULL;

    size_t cols_origin = NUMBER_OF_COLUMNS_IN_FORMAT_STRING(fmt);
    size_t cols = 0;

    while (1) {
        va_list va;
        va_copy(va, *va_args);
        int virtual_sz = VSNPRINTF(buffer->str.STR_FILED, string_buffer_capacity(buffer), fmt, va);
        va_end(va);
        /* If error encountered */
        if (virtual_sz < 0)
            goto clear;

        /* Successful write */
        if ((size_t)virtual_sz < string_buffer_capacity(buffer))
            break;

        /* Otherwise buffer was too small, so incr. buffer size ant try again. */
        if (!FT_IS_SUCCESS(realloc_string_buffer_without_copy(buffer)))
            goto clear;
    }

    cols = NUMBER_OF_COLUMNS_IN_FORMAT_STRING(buffer->str.STR_FILED);
    if (cols == cols_origin) {

        fort_row_t *row = CREATE_ROW_FROM_STRING(buffer->str.STR_FILED);
        if (row == NULL) {
            goto clear;
        }

        destroy_string_buffer(buffer);
        return row;
    }

    if (cols_origin == 1) {
        fort_row_t *row = create_row();
        if (row == NULL) {
            goto clear;
        }

        fort_cell_t *cell = get_cell_and_create_if_not_exists(row, 0);
        if (cell == NULL) {
            destroy_row(row);
            goto clear;
        }

        fort_status_t result = FILL_CELL_FROM_STRING(cell, buffer->str.STR_FILED);
        if (FT_IS_ERROR(result)) {
            destroy_row(row);
            goto clear;
        }

        destroy_string_buffer(buffer);
        return row;
    }

    /*
     * todo: add processing of cols != cols_origin in a general way
     * (when cols_origin != 1).
     */

clear:
    destroy_string_buffer(buffer);
    return NULL;
#undef VSNPRINTF
#undef STR_FILED
#undef CREATE_ROW_FROM_STRING
#undef NUMBER_OF_COLUMNS_IN_FORMAT_STRING
#undef FILL_CELL_FROM_STRING
#undef STR_BUF_TYPE
}

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
fort_row_t *create_row_from_fmt_wstring(const wchar_t  *fmt, va_list *va_args)
{
#define VSNPRINTF vswprintf
#define STR_FILED wstr
#define CREATE_ROW_FROM_STRING create_row_from_wstring
#define NUMBER_OF_COLUMNS_IN_FORMAT_STRING number_of_columns_in_format_wstring
#define FILL_CELL_FROM_STRING fill_cell_from_wstring
#define STR_BUF_TYPE WCharBuf

    string_buffer_t *buffer = create_string_buffer(DEFAULT_STR_BUF_SIZE, STR_BUF_TYPE);
    if (buffer == NULL)
        return NULL;

    size_t cols_origin = NUMBER_OF_COLUMNS_IN_FORMAT_STRING(fmt);
    size_t cols = 0;

    while (1) {
        va_list va;
        va_copy(va, *va_args);
        int virtual_sz = VSNPRINTF(buffer->str.STR_FILED, string_buffer_capacity(buffer), fmt, va);
        va_end(va);
        /* If error encountered */
        if (virtual_sz < 0)
            goto clear;

        /* Successful write */
        if ((size_t)virtual_sz < string_buffer_capacity(buffer))
            break;

        /* Otherwise buffer was too small, so incr. buffer size ant try again. */
        if (!FT_IS_SUCCESS(realloc_string_buffer_without_copy(buffer)))
            goto clear;
    }

    cols = NUMBER_OF_COLUMNS_IN_FORMAT_STRING(buffer->str.STR_FILED);
    if (cols == cols_origin) {

        fort_row_t *row = CREATE_ROW_FROM_STRING(buffer->str.STR_FILED);
        if (row == NULL) {
            goto clear;
        }

        destroy_string_buffer(buffer);
        return row;
    }

    if (cols_origin == 1) {
        fort_row_t *row = create_row();
        if (row == NULL) {
            goto clear;
        }

        fort_cell_t *cell = get_cell_and_create_if_not_exists(row, 0);
        if (cell == NULL) {
            destroy_row(row);
            goto clear;
        }

        fort_status_t result = FILL_CELL_FROM_STRING(cell, buffer->str.STR_FILED);
        if (FT_IS_ERROR(result)) {
            destroy_row(row);
            goto clear;
        }

        destroy_string_buffer(buffer);
        return row;
    }

    /*
     * todo: add processing of cols != cols_origin in a general way
     * (when cols_origin != 1).
     */

clear:
    destroy_string_buffer(buffer);
    return NULL;
#undef VSNPRINTF
#undef STR_FILED
#undef CREATE_ROW_FROM_STRING
#undef NUMBER_OF_COLUMNS_IN_FORMAT_STRING
#undef FILL_CELL_FROM_STRING
#undef STR_BUF_TYPE
}
#endif


FT_INTERNAL
int snprintf_row(const fort_row_t *row, char *buffer, size_t buf_sz, size_t *col_width_arr, size_t col_width_arr_sz,
                 size_t row_height, const context_t *context)
{
    int (*snprint_n_strings_)(char *, size_t, size_t, const char *) = snprint_n_strings;
    int (*cell_printf_)(fort_cell_t *, size_t, char *, size_t, const context_t *) = cell_printf;

    assert(context);
    const char *space_char = " ";
    const char *new_line_char = "\n";

    if (row == NULL)
        return -1;

    size_t cols_in_row = columns_in_row(row);
    if (cols_in_row > col_width_arr_sz)
        return -1;

    /*  Row separator anatomy
     *
     *  L    data    IV    data   IV   data    R
     */

    typedef const char *(*border_chars_point_t)[BorderItemPosSize];
    enum ft_row_type row_type = (enum ft_row_type)get_cell_property_value_hierarcial(context->table_properties, context->row, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
    const char *(*bord_chars)[BorderItemPosSize] = (row_type == FT_ROW_HEADER)
            ? (border_chars_point_t)(&context->table_properties->border_style.header_border_chars)
            : (border_chars_point_t)(&context->table_properties->border_style.border_chars);
    const char **L = &(*bord_chars)[LL_bip];
    const char **IV = &(*bord_chars)[IV_bip];
    const char **R = &(*bord_chars)[RR_bip];


    int written = 0;
    int tmp = 0;
    size_t i = 0;
    for (i = 0; i < row_height; ++i) {
        /* Print left margin */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, context->table_properties->entire_table_properties.left_margin, space_char));

        /* Print left table boundary */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, 1, *L));
        size_t j = 0;
        while (j < col_width_arr_sz) {
            if (j < cols_in_row) {
                ((context_t *)context)->column = j;
                fort_cell_t *cell = *(fort_cell_t **)vector_at(row->cells, j);
                size_t cell_width = 0;

                size_t group_slave_sz = group_cell_number(row, j);
                cell_width = col_width_arr[j];
                size_t slave_j = 0;
                size_t master_j = j;
                for (slave_j = master_j + 1; slave_j < (master_j + group_slave_sz); ++slave_j) {
                    cell_width += col_width_arr[slave_j] + FORT_COL_SEPARATOR_LENGTH;
                    ++j;
                }

                CHCK_RSLT_ADD_TO_WRITTEN(cell_printf_(cell, i, buffer + written, cell_width + 1, context));
            } else {
                /* Print empty cell */
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, col_width_arr[j], space_char));
            }

            /* Print boundary between cells */
            if (j < col_width_arr_sz - 1)
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, 1, *IV));

            ++j;
        }

        /* Print right table boundary */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, 1, *R));

        /* Print right margin */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, context->table_properties->entire_table_properties.right_margin, space_char));

        /* Print new line character */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, 1, new_line_char));
    }
    return written;

clear:
    return -1;
}


#ifdef FT_HAVE_WCHAR
FT_INTERNAL
int wsnprintf_row(const fort_row_t *row, wchar_t *buffer, size_t buf_sz, size_t *col_width_arr, size_t col_width_arr_sz,
                  size_t row_height, const context_t *context)
{
    int (*snprint_n_strings_)(wchar_t *, size_t, size_t, const char *) = wsnprint_n_string;
    int (*cell_printf_)(fort_cell_t *, size_t, wchar_t *, size_t, const context_t *) = cell_wprintf;

    assert(context);
    const char *space_char = " ";
    const char *new_line_char = "\n";

    if (row == NULL)
        return -1;

    size_t cols_in_row = columns_in_row(row);
    if (cols_in_row > col_width_arr_sz)
        return -1;

    /*  Row separator anatomy
     *
     *  L    data    IV    data   IV   data    R
     */

    typedef const char *(*border_chars_point_t)[BorderItemPosSize];
    enum ft_row_type row_type = (enum ft_row_type)get_cell_property_value_hierarcial(context->table_properties, context->row, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE);
    const char *(*bord_chars)[BorderItemPosSize] = (row_type == FT_ROW_HEADER)
            ? (border_chars_point_t)(&context->table_properties->border_style.header_border_chars)
            : (border_chars_point_t)(&context->table_properties->border_style.border_chars);
    const char **L = &(*bord_chars)[LL_bip];
    const char **IV = &(*bord_chars)[IV_bip];
    const char **R = &(*bord_chars)[RR_bip];


    int written = 0;
    int tmp = 0;
    size_t i = 0;
    for (i = 0; i < row_height; ++i) {
        /* Print left margin */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, context->table_properties->entire_table_properties.left_margin, space_char));

        /* Print left table boundary */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, 1, *L));
        size_t j = 0;
        while (j < col_width_arr_sz) {
            if (j < cols_in_row) {
                ((context_t *)context)->column = j;
                fort_cell_t *cell = *(fort_cell_t **)vector_at(row->cells, j);
                size_t cell_width = 0;

                size_t group_slave_sz = group_cell_number(row, j);
                cell_width = col_width_arr[j];
                size_t slave_j = 0;
                size_t master_j = j;
                for (slave_j = master_j + 1; slave_j < (master_j + group_slave_sz); ++slave_j) {
                    cell_width += col_width_arr[slave_j] + FORT_COL_SEPARATOR_LENGTH;
                    ++j;
                }

                CHCK_RSLT_ADD_TO_WRITTEN(cell_printf_(cell, i, buffer + written, cell_width + 1, context));
            } else {
                /* Print empty cell */
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, col_width_arr[j], space_char));
            }

            /* Print boundary between cells */
            if (j < col_width_arr_sz - 1)
                CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, 1, *IV));

            ++j;
        }

        /* Print right table boundary */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, 1, *R));

        /* Print right margin */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, context->table_properties->entire_table_properties.right_margin, space_char));

        /* Print new line character */
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buffer + written, buf_sz - written, 1, new_line_char));
    }
    return written;

clear:
    return -1;
}

#endif


