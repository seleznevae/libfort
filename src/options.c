#include "options.h"
#include "fort_impl.h"
#include "assert.h"
#include "vector.h"

/*****************************************************************************
 *               COLUMN OPTIONS
 * ***************************************************************************/

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

/*****************************************************************************
 *               OPTIONS
 * ***************************************************************************/



fort_table_options_t g_table_options = {
    1,      /* cell_padding_top         */
    1,      /* cell_padding_bottom      */
    1,      /* cell_padding_left        */
    1,      /* cell_padding_right       */
    1,      /* cell_empty_string_height */

    /* border_chars */
    {
     '+', '-', '+', '+',
     '|', '|', '|',
     '+', '-', '+', '+',
     '+', '-', '+', '+'
    },

    /* header_border_chars */
    {
    '+', '-', '+', '+',
    '|', '|', '|',
    '+', '-', '+', '+',
    '+', '-', '+', '+'
    },

    NULL,     /* col_options */
};


fort_table_options_t* create_table_options()
{
    fort_table_options_t* options = F_CALLOC(sizeof(fort_table_options_t), 1);
    if (options == NULL) {
        return NULL;
    }
    memcpy(options, &g_table_options, sizeof(fort_table_options_t));
    return options;
}

fort_table_options_t* copy_table_options(const fort_table_options_t *option)
{
    /* todo: normal implementation, do deep copy of col options */

    fort_table_options_t* new_opt = create_table_options();
    if (new_opt == NULL)
        return NULL;

    memcpy(new_opt, option, sizeof(fort_table_options_t));
    return new_opt;
}


void destroy_table_options(fort_table_options_t* options)
{
    if (options == NULL)
        return;

    if (options->col_options != NULL) {
        destroy_vector(options->col_options);
    }
    F_FREE(options);
}


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
    return F_SUCCESS;

fort_status_t fort_options_set_column_min_width(fort_table_options_t *options, size_t column, size_t width)
{
    FORT_OPTIONS_SET_COLUMN_OPTION(options, column, col_min_width, width);
}

fort_status_t fort_options_set_column_alignment(fort_table_options_t *options, size_t column, enum TextAlignment al)
{
    FORT_OPTIONS_SET_COLUMN_OPTION(options, column, align, al);
}

int fort_options_column_width(const fort_table_options_t *options, size_t column)
{
    assert(options);
    if (options->col_options == NULL)
        return -1;

    if (vector_size(options->col_options) <= column)
        return -1;

    return ((fort_column_options_t*)vector_at(options->col_options, column))->col_min_width;
}

int fort_options_column_alignment(const fort_table_options_t *options, size_t column)
{
    assert(options);

    enum TextAlignment align = g_column_options.align;
    if (options->col_options == NULL)
        return align;

    if (vector_size(options->col_options) <= column)
        return align;

    return ((fort_column_options_t*)vector_at(options->col_options, column))->align;
}
