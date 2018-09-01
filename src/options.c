#include <assert.h>
#include "options.h"
#include "fort_utils.h"
#include "vector.h"

/*****************************************************************************
 *               COLUMN OPTIONS
 * ***************************************************************************/

struct fort_cell_options g_default_cell_option = {
    FT_ANY_ROW,    /* cell_row */
    FT_ANY_COLUMN, /* cell_col */

    /* options */
    FT_COPT_MIN_WIDTH  | FT_COPT_TEXT_ALIGN | FT_COPT_TOP_PADDING
    | FT_COPT_BOTTOM_PADDING | FT_COPT_LEFT_PADDING | FT_COPT_RIGHT_PADDING
    | FT_COPT_EMPTY_STR_HEIGHT,

    0,             /* col_min_width */
    FT_ALIGNED_LEFT,  /* align */
    0,      /* cell_padding_top         */
    0,      /* cell_padding_bottom      */
    1,      /* cell_padding_left        */
    1,      /* cell_padding_right       */
    1,      /* cell_empty_string_height */

    FT_ROW_COMMON, /* row_type */
};

static int get_option_value_if_exists_otherwise_default(const struct fort_cell_options *cell_opts, uint32_t option)
{
    if (cell_opts == NULL || !OPTION_IS_SET(cell_opts->options, option)) {
        cell_opts = &g_default_cell_option;
    }

    switch (option) {
        case FT_COPT_MIN_WIDTH:
            return cell_opts->col_min_width;
        case FT_COPT_TEXT_ALIGN:
            return cell_opts->align;
        case FT_COPT_TOP_PADDING:
            return cell_opts->cell_padding_top;
        case FT_COPT_BOTTOM_PADDING:
            return cell_opts->cell_padding_bottom;
        case FT_COPT_LEFT_PADDING:
            return cell_opts->cell_padding_left;
        case FT_COPT_RIGHT_PADDING:
            return cell_opts->cell_padding_right;
        case FT_COPT_EMPTY_STR_HEIGHT:
            return cell_opts->cell_empty_string_height;
        case FT_COPT_ROW_TYPE:
            return cell_opts->row_type;
        default:
            /* todo: implement later */
            exit(333);
    }
}


fort_column_options_t g_column_options = {
    0,           /* col_min_width*/
    FT_ALIGNED_RIGHT, /* align */
};


FT_INTERNAL
fort_column_options_t create_column_options(void)
{
    fort_column_options_t result;
    memset(&result, '\0', sizeof(result));
    memcpy(&result, &g_column_options, sizeof(result));
    return result;
}


//#define DEFAULT_CELL_OPTION  {FT_ROW_UNSPEC, FT_COLUMN_UNSPEC, 0, 0, 0}
FT_INTERNAL
fort_cell_opt_container_t *create_cell_opt_container(void)
{
    fort_cell_opt_container_t *ret = create_vector(sizeof(fort_cell_options_t), DEFAULT_VECTOR_CAPACITY);
    return ret;
}


FT_INTERNAL
void destroy_cell_opt_container(fort_cell_opt_container_t *cont)
{
    if (cont)
        destroy_vector(cont);
}


FT_INTERNAL
const fort_cell_options_t *cget_cell_opt(const fort_cell_opt_container_t *cont, size_t row, size_t col)
{
    assert(cont);
    size_t sz = vector_size(cont);
    size_t i = 0;
    for (i = 0; i < sz; ++i) {
        const fort_cell_options_t *opt = (const fort_cell_options_t *)vector_at_c(cont, i);
        if (opt->cell_row == row && opt->cell_col == col)
            return opt;
    }
    return NULL;
}


FT_INTERNAL
fort_cell_options_t *get_cell_opt_and_create_if_not_exists(fort_cell_opt_container_t *cont, size_t row, size_t col)
{
    assert(cont);
    size_t sz = vector_size(cont);
    size_t i = 0;
    for (i = 0; i < sz; ++i) {
        fort_cell_options_t *opt = (fort_cell_options_t *)vector_at(cont, i);
        if (opt->cell_row == row && opt->cell_col == col)
            return opt;
    }
    fort_cell_options_t opt = g_default_cell_option;// DEFAULT_CELL_OPTION;
    opt.cell_row = row;
    opt.cell_col = col;
    if (FT_IS_SUCCESS(vector_push(cont, &opt))) {
        return (fort_cell_options_t *)vector_at(cont, sz);
    }

    return NULL;
}


FT_INTERNAL
int get_cell_opt_value_hierarcial(const fort_table_options_t *options, size_t row, size_t column, uint32_t option)
{
    assert(options);

    const fort_cell_options_t *opt = NULL;
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


static fort_status_t set_cell_option_impl(fort_cell_options_t *opt, uint32_t option, int value)
{
    assert(opt);

    OPTION_SET(opt->options, option);
    if (OPTION_IS_SET(option, FT_COPT_MIN_WIDTH)) {
        CHECK_NOT_NEGATIVE(value);
        opt->col_min_width = value;
    } else if (OPTION_IS_SET(option, FT_COPT_TEXT_ALIGN)) {
        opt->align = (enum ft_text_alignment)value;
    } else if (OPTION_IS_SET(option, FT_COPT_TOP_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_top = value;
    } else if (OPTION_IS_SET(option, FT_COPT_BOTTOM_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_bottom = value;
    } else if (OPTION_IS_SET(option, FT_COPT_LEFT_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_left = value;
    } else if (OPTION_IS_SET(option, FT_COPT_RIGHT_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_right = value;
    } else if (OPTION_IS_SET(option, FT_COPT_EMPTY_STR_HEIGHT)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_empty_string_height = value;
    } else if (OPTION_IS_SET(option, FT_COPT_ROW_TYPE)) {
        opt->row_type = (enum ft_row_type)value;
    }

    return FT_SUCCESS;

fort_fail:
    return FT_EINVAL;
}


FT_INTERNAL
fort_status_t set_cell_option(fort_cell_opt_container_t *cont, size_t row, size_t col, uint32_t option, int value)
{
    fort_cell_options_t *opt = get_cell_opt_and_create_if_not_exists(cont, row, col);
    if (opt == NULL)
        return FT_ERROR;

    return set_cell_option_impl(opt, option, value);
    /*
    OPTION_SET(opt->options, option);
    if (OPTION_IS_SET(option, FT_COPT_MIN_WIDTH)) {
        opt->col_min_width = value;
    } else if (OPTION_IS_SET(option, FT_COPT_TEXT_ALIGN)) {
        opt->align = value;
    }

    return FT_SUCCESS;
    */
}


FT_INTERNAL
fort_status_t set_default_cell_option(uint32_t option, int value)
{
    return set_cell_option_impl(&g_default_cell_option, option, value);
}

/*****************************************************************************
 *               OPTIONS
 * ***************************************************************************/


#define BASIC_STYLE  {            \
    /* border_chars */            \
    {                             \
     "+", "-", "+", "+",          \
     "|", "|", "|",               \
     "\0", "\0", "\0", "\0",      \
     "+", "-", "+", "+"           \
    },                            \
    /* header_border_chars */     \
    {                             \
    "+", "-", "+", "+",           \
    "|", "|", "|",                \
    "+", "-", "+", "+",           \
    "+", "-", "+", "+"            \
    },                            \
    /* separator_chars */         \
    {                             \
    "+", "-", "+", "+",           \
    },                            \
}

#define BASIC2_STYLE  {            \
    /* border_chars */            \
    {                             \
     "+", "-", "+", "+",          \
     "|", "|", "|",               \
     "+", "-", "+", "+",           \
     "+", "-", "+", "+"           \
    },                            \
    /* header_border_chars */     \
    {                             \
    "+", "-", "+", "+",           \
    "|", "|", "|",                \
    "+", "-", "+", "+",           \
    "+", "-", "+", "+"            \
    },                            \
    /* separator_chars */         \
    {                             \
    "+", "-", "+", "+",           \
    },                            \
}

#define SIMPLE_STYLE  {           \
    /* border_chars */            \
    {                             \
     " ", " ", " ", " ",          \
     " ", " ", " ",               \
     "\0", "\0", "\0", "\0",      \
     " ", " ", " ", " "           \
    },                            \
    /* header_border_chars */     \
    {                             \
    " ", " ", " ", " ",           \
    " ", " ", " ",                \
    " ", "-", " ", " ",           \
    " ", " ", " ", " "            \
    },                            \
    /* separator_chars */         \
    {                             \
    " ", "-", " ", " ",           \
    },                            \
}

#define PLAIN_STYLE  {            \
    /* border_chars */            \
    {                             \
     " ", " ", " ", " ",          \
     " ", " ", " ",               \
     "\0", "\0", "\0", "\0",      \
     " ", " ", " ", " "           \
    },                            \
    /* header_border_chars */     \
    {                             \
    " ", "-", "-", " ",           \
    " ", " ", " ",                \
    " ", "-", "-", " ",           \
    " ", "-", "-", " "            \
    },                            \
    /* separator_chars */         \
    {                             \
    " ", "-", "-", " ",           \
    },                            \
}

#define DOT_STYLE  {              \
    /* border_chars */            \
    {                             \
     ".", ".", ".", ".",          \
     ":", ":", ":",               \
     "\0", "\0", "\0", "\0",      \
     ":", ".", ":", ":"           \
    },                            \
    /* header_border_chars */     \
    {                             \
    ".", ".", ".", ".",           \
    ":", ":", ":",                \
    ":", ".", ":", ":",           \
    ":", ".", ":", ":"            \
    },                            \
    /* separator_chars */         \
    {                             \
    ":", ".", ":", ":",           \
    },                            \
}

#define EMPTY_STYLE  {            \
    /* border_chars */            \
    {                             \
     " ", " ", " ", " ",          \
     " ", " ", " ",               \
     "\0", "\0", "\0", "\0",      \
     " ", " ", " ", " "           \
    },                            \
    /* header_border_chars */     \
    {                             \
    " ", " ", " ", " ",           \
    " ", " ", " ",                \
    "\0", "\0", "\0", "\0",       \
    " ", " ", " ", " "            \
    },                            \
    /* separator_chars */         \
    {                             \
    " ", " ", " ", " ",           \
    },                            \
}


#define SOLID_STYLE  {           \
    /* border_chars */            \
    {                             \
     "┌", "─", "┬", "┐",          \
     "│", "│", "│",               \
     "", "", "", "",              \
     "└", "─", "┴", "╯"           \
    },                            \
    /* header_border_chars */     \
    {                             \
    "┌", "─", "┬", "┐",           \
    "│", "│", "│",                \
    "├", "─", "┼", "┤",           \
    "└", "─", "┴", "┘"            \
    },                            \
    /* separator_chars */         \
    {                             \
    "├", "─", "┼", "┤",           \
    },                            \
}

#define SOLID_ROUND_STYLE  {      \
    /* border_chars */            \
    {                             \
     "╭", "─", "┬", "╮",          \
     "│", "│", "│",               \
     "", "", "", "",              \
     "╰", "─", "┴", "╯"           \
    },                            \
    /* header_border_chars */     \
    {                             \
    "╭", "─", "┬", "╮",           \
    "│", "│", "│",                \
    "├", "─", "┼", "┤",           \
    "╰", "─", "┴", "╯"            \
    },                            \
    /* separator_chars */         \
    {                             \
    "├", "─", "┼", "┤",           \
    },                            \
}


#define DOUBLE_STYLE  {           \
    /* border_chars */            \
    {                             \
     "╔", "═", "╦", "╗",          \
     "║", "║", "║",               \
     "", "", "", "",              \
     "╚", "═", "╩", "╝"           \
    },                            \
    /* header_border_chars */     \
    {                             \
    "╔", "═", "╦", "╗",           \
    "║", "║", "║",                \
    "╠", "═", "╬", "╣",           \
    "╚", "═", "╩", "╝"            \
    },                            \
    /* separator_chars */         \
    {                             \
    "╠", "═", "╬", "╣",           \
    },                            \
}


#define DOUBLE2_STYLE  {           \
    /* border_chars */            \
    {                             \
     "╔", "═", "╤", "╗",          \
     "║", "│", "║",               \
     "╟", "─", "┼", "╢",          \
     "╚", "═", "╧", "╝"           \
    },                            \
    /* header_border_chars */     \
    {                             \
    "╔", "═", "╤", "╗",           \
    "║", "│", "║",                \
    "╠", "═", "╪", "╣",           \
    "╚", "═", "╧", "╝"            \
    },                            \
    /* separator_chars */         \
    {                             \
    "╠", "═", "╪", "╣",           \
    },                            \
}


#define BOLD_STYLE  {             \
    /* border_chars */            \
    {                             \
     "┏", "━", "┳", "┓",          \
     "┃", "┃", "┃",               \
     "", "", "", "",              \
     "┗", "━", "┻", "┛"           \
    },                            \
    /* header_border_chars */     \
    {                             \
    "┏", "━", "┳", "┓",           \
    "┃", "┃", "┃",                \
    "┣", "━", "╋", "┫",           \
    "┗", "━", "┻", "┛"            \
    },                            \
    /* separator_chars */         \
    {                             \
    "┣", "━", "╋", "┫",           \
    },                            \
}

#define BOLD2_STYLE  {            \
    /* border_chars */            \
    {                             \
     "┏", "━", "┯", "┓",          \
     "┃", "│", "┃",               \
     "┠", "─", "┼", "┨",          \
     "┗", "━", "┷", "┛"           \
    },                            \
    /* header_border_chars */     \
    {                             \
    "┏", "━", "┯", "┓",           \
    "┃", "│", "┃",                \
    "┣", "━", "┿", "┫",           \
    "┗", "━", "┷", "┛"            \
    },                            \
    /* separator_chars */         \
    {                             \
    "┣", "━", "┿", "┫",           \
    },                            \
}

#define FRAME_STYLE  {             \
    /* border_chars */            \
    {                             \
     "▛", "▀", "▀", "▜",          \
     "▌", "┃", "▐",               \
     "", "", "", "",              \
     "▙", "▄", "▄", "▟"           \
    },                            \
    /* header_border_chars */     \
    {                             \
    "▛", "▀", "▀", "▜",           \
    "▌", "┃", "▐",                \
    "▌", "━", "╋", "▐",           \
    "▙", "▄", "▄", "▟"            \
    },                            \
    /* separator_chars */         \
    {                             \
    "▌", "━", "╋", "▐",           \
    },                            \
}


struct fort_border_style FORT_BASIC_STYLE = BASIC_STYLE;
struct fort_border_style FORT_BASIC2_STYLE = BASIC2_STYLE;
struct fort_border_style FORT_SIMPLE_STYLE = SIMPLE_STYLE;
struct fort_border_style FORT_PLAIN_STYLE = PLAIN_STYLE;
struct fort_border_style FORT_DOT_STYLE = DOT_STYLE;
struct fort_border_style FORT_EMPTY_STYLE = EMPTY_STYLE;
struct fort_border_style FORT_SOLID_STYLE = SOLID_STYLE;
struct fort_border_style FORT_SOLID_ROUND_STYLE = SOLID_ROUND_STYLE;
struct fort_border_style FORT_DOUBLE_STYLE = DOUBLE_STYLE;
struct fort_border_style FORT_DOUBLE2_STYLE = DOUBLE2_STYLE;
struct fort_border_style FORT_BOLD_STYLE = BOLD_STYLE;
struct fort_border_style FORT_BOLD2_STYLE = BOLD2_STYLE;
struct fort_border_style FORT_FRAME_STYLE = FRAME_STYLE;



fort_entire_table_options_t g_entire_table_options = {
    0, /* left_margin */
    0, /* top_margin */
    0, /* right_margin */
    0, /* bottom_margin */
};

static fort_status_t set_entire_table_option_internal(fort_entire_table_options_t *options, uint32_t option, int value)
{
    assert(options);
    CHECK_NOT_NEGATIVE(value);
    if (OPTION_IS_SET(option, FT_TOPT_LEFT_MARGIN)) {
        options->left_margin = value;
    } else if (OPTION_IS_SET(option, FT_TOPT_TOP_MARGIN)) {
        options->top_margin = value;
    } else if (OPTION_IS_SET(option, FT_TOPT_RIGHT_MARGIN)) {
        options->right_margin = value;
    } else if (OPTION_IS_SET(option, FT_TOPT_BOTTOM_MARGIN)) {
        options->bottom_margin = value;
    } else {
        return FT_EINVAL;
    }
    return FT_SUCCESS;

fort_fail:
    return FT_EINVAL;
}


FT_INTERNAL
fort_status_t set_entire_table_option(fort_table_options_t *table_options, uint32_t option, int value)
{
    assert(table_options);
    return set_entire_table_option_internal(&table_options->entire_table_options, option, value);
}


FT_INTERNAL
fort_status_t set_default_entire_table_option(uint32_t option, int value)
{
    return set_entire_table_option_internal(&g_entire_table_options, option, value);
}


FT_INTERNAL
size_t max_border_elem_strlen(struct fort_table_options *options)
{
    assert(options);
    size_t result = 1;
    int i = 0;
    for (i = 0; i < BorderItemPosSize; ++i) {
        result = MAX(result, strlen(options->border_style.border_chars[i]));
    }

    i = 0;
    for (i = 0; i < BorderItemPosSize; ++i) {
        result = MAX(result, strlen(options->border_style.header_border_chars[i]));
    }

    i = 0;
    for (i = 0; i < SepratorItemPosSize; ++i) {
        result = MAX(result, strlen(options->border_style.separator_chars[i]));
    }
    return result;
}


fort_table_options_t g_table_options = {
    /* border_style */
    BASIC_STYLE,
    NULL,     /* cell_options */
    /* entire_table_options */
    {
        0, /* left_margin */
        0, /* top_margin */
        0, /* right_margin */
        0  /* bottom_margin */
    }
};


FT_INTERNAL
fort_table_options_t *create_table_options(void)
{
    fort_table_options_t *options = (fort_table_options_t *)F_CALLOC(sizeof(fort_table_options_t), 1);
    if (options == NULL) {
        return NULL;
    }
    memcpy(options, &g_table_options, sizeof(fort_table_options_t));
    options->cell_options = create_cell_opt_container();
    if (options->cell_options == NULL) {
        destroy_table_options(options);
        return NULL;
    }
    memcpy(&options->entire_table_options, &g_entire_table_options, sizeof(fort_entire_table_options_t));
    return options;
}


FT_INTERNAL
fort_table_options_t *copy_table_options(const fort_table_options_t *option)
{
    /* todo: normal implementation, do deep copy of col options */

    fort_table_options_t *new_opt = create_table_options();
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


FT_INTERNAL
void destroy_table_options(fort_table_options_t *options)
{
    if (options == NULL)
        return;

    if (options->cell_options != NULL) {
        destroy_cell_opt_container(options->cell_options);
    }
    F_FREE(options);
}



