#include "options.h"
#include "fort_impl.h"
#include "assert.h"
#include "vector.h"

/*****************************************************************************
 *               COLUMN OPTIONS
 * ***************************************************************************/

struct fort_cell_options g_default_cell_option =
{
    FT_ANY_ROW,    /* cell_row */
    FT_ANY_COLUMN, /* cell_col */

    /* options */
    FT_OPT_MIN_WIDTH  | FT_OPT_TEXT_ALIGN | FT_OPT_TOP_PADDING | FT_OPT_BOTTOM_PADDING
    | FT_OPT_LEFT_PADDING | FT_OPT_RIGHT_PADDING | FT_OPT_EMPTY_STR_HEIGHT ,

    0,             /* col_min_width */
    RightAligned   /* align */
};

static int get_option_value_if_exists_otherwise_default(const struct fort_cell_options *cell_opts, uint32_t option)
{
    if (cell_opts == NULL || !OPTION_IS_SET(cell_opts->options, option)) {
        cell_opts = &g_default_cell_option;
    }

    switch (option) {
        case FT_OPT_MIN_WIDTH:
            return cell_opts->col_min_width;
        case FT_OPT_TEXT_ALIGN:
            return cell_opts->align;
        default:
            // todo: implement later
            exit(333);
    }
}


fort_column_options_t g_column_options =
{
    0,           /* col_min_width*/
    RightAligned, /* align */
};

fort_column_options_t create_column_options()
{
    fort_column_options_t result;
    memset(&result, '\0', sizeof(result));
    memcpy(&result, &g_column_options, sizeof(result));
    return result;
}


#define DEFAULT_CELL_OPTION  {FT_ROW_UNSPEC, FT_COLUMN_UNSPEC, 0, 0, 0}

fort_cell_opt_container_t *create_cell_opt_container()
{
    fort_cell_opt_container_t *ret = create_vector(sizeof(fort_cell_options_t), DEFAULT_VECTOR_CAPACITY);
    return ret;
}


void destroy_cell_opt_container(fort_cell_opt_container_t *cont)
{
    if (cont)
        destroy_vector(cont);
}

const fort_cell_options_t* cget_cell_opt(const fort_cell_opt_container_t *cont, unsigned row, unsigned col)
{
    assert(cont);
    size_t sz = vector_size(cont);
    for (size_t i = 0; i < sz; ++i) {
        const fort_cell_options_t* opt = (const fort_cell_options_t*)vector_at_c(cont, i);
        if (opt->cell_row == row && opt->cell_col == col)
            return opt;
    }
    return NULL;
}

fort_cell_options_t* get_cell_opt_and_create_if_not_exists(fort_cell_opt_container_t *cont, unsigned row, unsigned col)
{
    assert(cont);
    size_t sz = vector_size(cont);
    for (size_t i = 0; i < sz; ++i) {
        fort_cell_options_t* opt = (fort_cell_options_t*)vector_at(cont, i);
        if (opt->cell_row == row && opt->cell_col == col)
            return opt;
    }
    fort_cell_options_t opt = DEFAULT_CELL_OPTION;
    opt.cell_row = row;
    opt.cell_col = col;
    if (IS_SUCCESS(vector_push(cont, &opt))) {
        return (fort_cell_options_t*)vector_at(cont, sz);
    }

    return NULL;
}

fort_status_t set_cell_option(fort_cell_opt_container_t *cont, unsigned row, unsigned col, uint32_t option, int value)
{
    fort_cell_options_t* opt = get_cell_opt_and_create_if_not_exists(cont, row, col);
    if (opt == NULL)
        return F_ERROR;

    OPTION_SET(opt->options, option);
    if (OPTION_IS_SET(option, FT_OPT_MIN_WIDTH)) {
        opt->col_min_width = value;
    } else if (OPTION_IS_SET(option, FT_OPT_TEXT_ALIGN)) {
        opt->align = value;
    }

    return F_SUCCESS;
}


int get_cell_opt_value_hierarcial(const fort_table_options_t *options, size_t row, size_t column, uint32_t option)
{
    assert(options);

    const fort_cell_options_t* opt = NULL;
    if (options->cell_options != NULL) {
        while (1) {
            opt = cget_cell_opt(options->cell_options, row, column);
            if (opt != NULL)
                break;
            if (row != FT_ANY_ROW) {
                row = FT_ANY_ROW;
                continue;
            }
            if (column != FT_ANY_COLUMN) {
                column = FT_ANY_COLUMN;
                continue;
            }

            opt = NULL;
            break;
        }
    }

    return get_option_value_if_exists_otherwise_default(opt, option);
}


/*****************************************************************************
 *               OPTIONS
 * ***************************************************************************/



//fort_table_options_t g_table_options = {
//    1,      /* cell_padding_top         */
//    1,      /* cell_padding_bottom      */
//    1,      /* cell_padding_left        */
//    1,      /* cell_padding_right       */
//    1,      /* cell_empty_string_height */

//    /* border_chars */
//    {
//     '+', '-', '+', '+',
//     '|', '|', '|',
//     '+', '-', '+', '+',
//     '+', '-', '+', '+'
//    },

//    /* header_border_chars */
//    {
//    '+', '-', '+', '+',
//    '|', '|', '|',
//    '+', '-', '+', '+',
//    '+', '-', '+', '+'
//    },

//    NULL,     /* col_options */
//};


fort_table_options_t g_table_options = {
    0,      /* cell_padding_top         */
    0,      /* cell_padding_bottom      */
    1,      /* cell_padding_left        */
    1,      /* cell_padding_right       */
    1,      /* cell_empty_string_height */

    /* border_chars */
    {
     '+', '-', '+', '+',
     '|', '|', '|',
     '\0', '\0', '\0', '\0',
     '+', '-', '+', '+'
    },

    /* header_border_chars */
    {
    '+', '-', '+', '+',
    '|', '|', '|',
    '+', '-', '+', '+',
    '+', '-', '+', '+'
    },

    /* separator_chars */
    {
    '+', '=', '+', '+',
    },

//    NULL,     /* col_options */
    NULL,     /* cell_options */
};


fort_table_options_t* create_table_options()
{
    fort_table_options_t* options = F_CALLOC(sizeof(fort_table_options_t), 1);
    if (options == NULL) {
        return NULL;
    }
    memcpy(options, &g_table_options, sizeof(fort_table_options_t));
    options->cell_options = create_cell_opt_container();
    if (options->cell_options == NULL) {
        destroy_table_options(options);
        options = NULL;
    }

    return options;
}

fort_table_options_t* copy_table_options(const fort_table_options_t *option)
{
    /* todo: normal implementation, do deep copy of col options */

    fort_table_options_t* new_opt = create_table_options();
    if (new_opt == NULL)
        return NULL;

    memcpy(new_opt, option, sizeof(fort_table_options_t));

    if (option->cell_options) {
        destroy_cell_opt_container(new_opt->cell_options);
        new_opt->cell_options = copy_vector(option->cell_options);
        if (new_opt->cell_options == NULL) {
            destroy_table_options(new_opt);
            new_opt = NULL;
        }
    }
    return new_opt;
}


void destroy_table_options(fort_table_options_t* options)
{
    if (options == NULL)
        return;

//    if (options->col_options != NULL) {
//        destroy_vector(options->col_options);
//    }
    if (options->cell_options != NULL) {
        destroy_cell_opt_container(options->cell_options);
    }
    F_FREE(options);
}

/*
#define FORT_OPTIONS_SET_COLUMN_OPTION(options, column, opt_name, opt_value) \
    assert(options);\
\
    if (options->col_options == NULL) {\
        options->col_options = create_vector(sizeof(fort_column_options_t), DEFAULT_VECTOR_CAPACITY);\
        if (options->col_options == NULL) \
            return F_MEMORY_ERROR; \
    } \
\
    while (vector_size(options->col_options) <= column) {\
        fort_column_options_t def_option = create_column_options();\
        vector_push(options->col_options, &def_option);\
    }\
\
    fort_column_options_t *col_option = (fort_column_options_t*)vector_at(options->col_options, column);\
    col_option->opt_name = opt_value;\
\
    return F_SUCCESS;*/

//fort_status_t fort_options_set_column_min_width(fort_table_options_t *options, size_t column, size_t width)
//{
//    FORT_OPTIONS_SET_COLUMN_OPTION(options, column, col_min_width, width);
//}

//fort_status_t fort_options_set_column_alignment(fort_table_options_t *options, size_t column, enum TextAlignment al)
//{
//    FORT_OPTIONS_SET_COLUMN_OPTION(options, column, align, al);
//}

//int fort_options_column_width(const fort_table_options_t *options, size_t column)
//{
//    assert(options);
//    if (options->col_options == NULL)
//        return -1;

//    if (vector_size(options->col_options) <= column)
//        return -1;

//    return ((fort_column_options_t*)vector_at(options->col_options, column))->col_min_width;
//}




//int fort_options_column_width(const fort_table_options_t *options, size_t column)
//{
//    assert(options);
//    if (options->cell_options == NULL)
//        return -1;

//    const fort_cell_options_t* col_opt = cget_cell_opt(options->cell_options, FT_ANY_ROW, column);
//    if (col_opt == NULL || ((col_opt->options & FT_OPT_MIN_WIDTH) == 0))
//        return -1;
//    else {
//        return col_opt->col_min_width;
//    }
//}







//int fort_options_column_alignment(const fort_table_options_t *options, size_t column)
//{
//    assert(options);

//    enum TextAlignment align = g_column_options.align;
//    if (options->col_options == NULL)
//        return align;

//    if (vector_size(options->col_options) <= column)
//        return align;

//    return ((fort_column_options_t*)vector_at(options->col_options, column))->align;
//}

//int fort_options_column_alignment(const fort_table_options_t *options, size_t column)
//{
//    assert(options);
//    enum TextAlignment defaultAlign = g_column_options.align;

//    if (options->cell_options == NULL)
//        return defaultAlign;

//    const fort_cell_options_t* col_opt = cget_cell_opt(options->cell_options, FT_ANY_ROW, column);
//    if (col_opt == NULL || ((col_opt->options & FT_OPT_TEXT_ALIGN) == 0))
//        return defaultAlign;
//    else {
//        return col_opt->align;
//    }
//}

