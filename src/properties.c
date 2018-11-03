#include <assert.h>
#include "properties.h"
#include "fort_utils.h"
#include "vector.h"

/*****************************************************************************
 *               COLUMN PROPERTIES
 * ***************************************************************************/

struct fort_cell_props g_default_cell_properties = {
    FT_ANY_ROW,    /* cell_row */
    FT_ANY_COLUMN, /* cell_col */

    /* properties */
    FT_CPROP_MIN_WIDTH  | FT_CPROP_TEXT_ALIGN | FT_CPROP_TOP_PADDING
    | FT_CPROP_BOTTOM_PADDING | FT_CPROP_LEFT_PADDING | FT_CPROP_RIGHT_PADDING
    | FT_CPROP_EMPTY_STR_HEIGHT,

    0,             /* col_min_width */
    FT_ALIGNED_LEFT,  /* align */
    0,      /* cell_padding_top         */
    0,      /* cell_padding_bottom      */
    1,      /* cell_padding_left        */
    1,      /* cell_padding_right       */
    1,      /* cell_empty_string_height */

    FT_ROW_COMMON, /* row_type */
};

static int get_prop_value_if_exists_otherwise_default(const struct fort_cell_props *cell_opts, uint32_t property)
{
    if (cell_opts == NULL || !PROP_IS_SET(cell_opts->properties, property)) {
        cell_opts = &g_default_cell_properties;
    }

    switch (property) {
        case FT_CPROP_MIN_WIDTH:
            return cell_opts->col_min_width;
        case FT_CPROP_TEXT_ALIGN:
            return cell_opts->align;
        case FT_CPROP_TOP_PADDING:
            return cell_opts->cell_padding_top;
        case FT_CPROP_BOTTOM_PADDING:
            return cell_opts->cell_padding_bottom;
        case FT_CPROP_LEFT_PADDING:
            return cell_opts->cell_padding_left;
        case FT_CPROP_RIGHT_PADDING:
            return cell_opts->cell_padding_right;
        case FT_CPROP_EMPTY_STR_HEIGHT:
            return cell_opts->cell_empty_string_height;
        case FT_CPROP_ROW_TYPE:
            return cell_opts->row_type;
        default:
            /* todo: implement later */
            exit(333);
    }
}


FT_INTERNAL
fort_cell_prop_container_t *create_cell_prop_container(void)
{
    fort_cell_prop_container_t *ret = create_vector(sizeof(fort_cell_props_t), DEFAULT_VECTOR_CAPACITY);
    return ret;
}


FT_INTERNAL
void destroy_cell_prop_container(fort_cell_prop_container_t *cont)
{
    if (cont)
        destroy_vector(cont);
}


FT_INTERNAL
const fort_cell_props_t *cget_cell_prop(const fort_cell_prop_container_t *cont, size_t row, size_t col)
{
    assert(cont);
    size_t sz = vector_size(cont);
    size_t i = 0;
    for (i = 0; i < sz; ++i) {
        const fort_cell_props_t *opt = (const fort_cell_props_t *)vector_at_c(cont, i);
        if (opt->cell_row == row && opt->cell_col == col)
            return opt;
    }
    return NULL;
}


FT_INTERNAL
fort_cell_props_t *get_cell_prop_and_create_if_not_exists(fort_cell_prop_container_t *cont, size_t row, size_t col)
{
    assert(cont);
    size_t sz = vector_size(cont);
    size_t i = 0;
    for (i = 0; i < sz; ++i) {
        fort_cell_props_t *opt = (fort_cell_props_t *)vector_at(cont, i);
        if (opt->cell_row == row && opt->cell_col == col)
            return opt;
    }

    fort_cell_props_t opt;
    if (row == FT_ANY_ROW && col == FT_ANY_COLUMN)
        memcpy(&opt, &g_default_cell_properties, sizeof(fort_cell_props_t));
    else
        memset(&opt, 0, sizeof(fort_cell_props_t));

    opt.cell_row = row;
    opt.cell_col = col;
    if (FT_IS_SUCCESS(vector_push(cont, &opt))) {
        return (fort_cell_props_t *)vector_at(cont, sz);
    }

    return NULL;
}


FT_INTERNAL
int get_cell_property_value_hierarcial(const fort_table_properties_t *propertiess, size_t row, size_t column, uint32_t property)
{
    assert(propertiess);

    const fort_cell_props_t *opt = NULL;
    if (propertiess->cell_properties != NULL) {
        while (1) {
            opt = cget_cell_prop(propertiess->cell_properties, row, column);
            if (opt != NULL && PROP_IS_SET(opt->properties, property))
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

    return get_prop_value_if_exists_otherwise_default(opt, property);
}


static fort_status_t set_cell_property_impl(fort_cell_props_t *opt, uint32_t property, int value)
{
    assert(opt);

    PROP_SET(opt->properties, property);
    if (PROP_IS_SET(property, FT_CPROP_MIN_WIDTH)) {
        CHECK_NOT_NEGATIVE(value);
        opt->col_min_width = value;
    } else if (PROP_IS_SET(property, FT_CPROP_TEXT_ALIGN)) {
        opt->align = (enum ft_text_alignment)value;
    } else if (PROP_IS_SET(property, FT_CPROP_TOP_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_top = value;
    } else if (PROP_IS_SET(property, FT_CPROP_BOTTOM_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_bottom = value;
    } else if (PROP_IS_SET(property, FT_CPROP_LEFT_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_left = value;
    } else if (PROP_IS_SET(property, FT_CPROP_RIGHT_PADDING)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_padding_right = value;
    } else if (PROP_IS_SET(property, FT_CPROP_EMPTY_STR_HEIGHT)) {
        CHECK_NOT_NEGATIVE(value);
        opt->cell_empty_string_height = value;
    } else if (PROP_IS_SET(property, FT_CPROP_ROW_TYPE)) {
        opt->row_type = (enum ft_row_type)value;
    }

    return FT_SUCCESS;

fort_fail:
    return FT_EINVAL;
}


FT_INTERNAL
fort_status_t set_cell_property(fort_cell_prop_container_t *cont, size_t row, size_t col, uint32_t property, int value)
{
    fort_cell_props_t *opt = get_cell_prop_and_create_if_not_exists(cont, row, col);
    if (opt == NULL)
        return FT_ERROR;

    return set_cell_property_impl(opt, property, value);
    /*
    PROP_SET(opt->propertiess, property);
    if (PROP_IS_SET(property, FT_CPROP_MIN_WIDTH)) {
        opt->col_min_width = value;
    } else if (PROP_IS_SET(property, FT_CPROP_TEXT_ALIGN)) {
        opt->align = value;
    }

    return FT_SUCCESS;
    */
}


FT_INTERNAL
fort_status_t set_default_cell_property(uint32_t property, int value)
{
    return set_cell_property_impl(&g_default_cell_properties, property, value);
}

/*****************************************************************************
 *               PROPERTIESS
 * ***************************************************************************/


#define BASIC_STYLE  {            \
    /* border_chars */            \
    {                             \
     "+", "-", "+", "+",          \
     "|", "|", "|",               \
     "\0", "\0", "\0", "\0",      \
     "+", "-", "+", "+",          \
     "+", "+", "+", "+",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    "+", "-", "+", "+",           \
    "|", "|", "|",                \
    "+", "-", "+", "+",           \
    "+", "-", "+", "+",           \
    "+", "+", "+", "+",           \
    },                            \
    /* separator_chars */         \
    {                             \
    "+", "-", "+", "+",           \
    },                            \
}

#define BASIC2_STYLE  {           \
    /* border_chars */            \
    {                             \
     "+", "-", "+", "+",          \
     "|", "|", "|",               \
     "+", "-", "+", "+",          \
     "+", "-", "+", "+",          \
     "+", "+", "+", "+",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    "+", "-", "+", "+",           \
    "|", "|", "|",                \
    "+", "-", "+", "+",           \
    "+", "-", "+", "+",           \
    "+", "+", "+", "+",           \
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
     " ", " ", " ", " ",          \
     " ", " ", " ", " ",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    " ", " ", " ", " ",           \
    " ", " ", " ",                \
    " ", "-", " ", " ",           \
    " ", " ", " ", " ",           \
    " ", "-", " ", "-",           \
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
     " ", " ", " ", " ",          \
     " ", " ", " ", " ",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    " ", "-", "-", " ",           \
    " ", " ", " ",                \
    " ", "-", "-", " ",           \
    " ", "-", "-", " ",           \
    " ", "-", " ", "-",           \
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
     ":", ".", ":", ":",          \
     "+", ":", "+", ":",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    ".", ".", ".", ".",           \
    ":", ":", ":",                \
    ":", ".", ":", ":",           \
    ":", ".", ":", ":",           \
    "+", ".", "+", ".",           \
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
     " ", " ", " ", " ",          \
     " ", " ", " ", " ",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    " ", " ", " ", " ",           \
    " ", " ", " ",                \
    "\0", "\0", "\0", "\0",       \
    " ", " ", " ", " ",           \
    " ", " ", " ", " ",           \
    },                            \
    /* separator_chars */         \
    {                             \
    " ", " ", " ", " ",           \
    },                            \
}


#define SOLID_STYLE  {            \
    /* border_chars */            \
    {                             \
     "┌", "─", "┬", "┐",          \
     "│", "│", "│",               \
     "", "", "", "",              \
     "└", "─", "┴", "╯",          \
     "│", "─", "│", "─",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    "┌", "─", "┬", "┐",           \
    "│", "│", "│",                \
    "├", "─", "┼", "┤",           \
    "└", "─", "┴", "┘",           \
    "┼", "┬", "┼", "┴",           \
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
     "╰", "─", "┴", "╯",          \
     "│", "─", "│", "─",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    "╭", "─", "┬", "╮",           \
    "│", "│", "│",                \
    "├", "─", "┼", "┤",           \
    "╰", "─", "┴", "╯",           \
    "┼", "┬", "┼", "┴",           \
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
     "╚", "═", "╩", "╝",          \
     "┣", "┻", "┣", "┳",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    "╔", "═", "╦", "╗",           \
    "║", "║", "║",                \
    "╠", "═", "╬", "╣",           \
    "╚", "═", "╩", "╝",           \
    "┣", "╦", "┣", "╩",           \
    },                            \
    /* separator_chars */         \
    {                             \
    "╠", "═", "╬", "╣",           \
    },                            \
}


#define DOUBLE2_STYLE  {          \
    /* border_chars */            \
    {                             \
     "╔", "═", "╤", "╗",          \
     "║", "│", "║",               \
     "╟", "─", "┼", "╢",          \
     "╚", "═", "╧", "╝",          \
     "├", "┬", "┤", "┴",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    "╔", "═", "╤", "╗",           \
    "║", "│", "║",                \
    "╠", "═", "╪", "╣",           \
    "╚", "═", "╧", "╝",           \
    "├", "╤", "┤", "╧",           \
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
     "┗", "━", "┻", "┛",          \
     "┣", "┻", "┣", "┳",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    "┏", "━", "┳", "┓",           \
    "┃", "┃", "┃",                \
    "┣", "━", "╋", "┫",           \
    "┗", "━", "┻", "┛",           \
    "┣", "┳", "┣", "┻",           \
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
     "┗", "━", "┷", "┛",          \
     "┣", "┬", "┣", "┴",          \
    },                            \
    /* header_border_chars */     \
    {                             \
    "┏", "━", "┯", "┓",           \
    "┃", "│", "┃",                \
    "┣", "━", "┿", "┫",           \
    "┗", "━", "┷", "┛",           \
    "┣", "┯", "┣", "┷",           \
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
     "▙", "▄", "▄", "▟",          \
     "┣", "━", "┣", "━"           \
    },                            \
    /* header_border_chars */     \
    {                             \
    "▛", "▀", "▀", "▜",           \
    "▌", "┃", "▐",                \
    "▌", "━", "╋", "▐",           \
    "▙", "▄", "▄", "▟",           \
    "┣", "━", "┣", "━",           \
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



fort_entire_table_properties_t g_entire_table_properties = {
    0, /* left_margin */
    0, /* top_margin */
    0, /* right_margin */
    0, /* bottom_margin */
};

static fort_status_t set_entire_table_property_internal(fort_entire_table_properties_t *properties, uint32_t property, int value)
{
    assert(properties);
    CHECK_NOT_NEGATIVE(value);
    if (PROP_IS_SET(property, FT_TPROP_LEFT_MARGIN)) {
        properties->left_margin = value;
    } else if (PROP_IS_SET(property, FT_TPROP_TOP_MARGIN)) {
        properties->top_margin = value;
    } else if (PROP_IS_SET(property, FT_TPROP_RIGHT_MARGIN)) {
        properties->right_margin = value;
    } else if (PROP_IS_SET(property, FT_TPROP_BOTTOM_MARGIN)) {
        properties->bottom_margin = value;
    } else {
        return FT_EINVAL;
    }
    return FT_SUCCESS;

fort_fail:
    return FT_EINVAL;
}


FT_INTERNAL
fort_status_t set_entire_table_property(fort_table_properties_t *table_properties, uint32_t property, int value)
{
    assert(table_properties);
    return set_entire_table_property_internal(&table_properties->entire_table_properties, property, value);
}


FT_INTERNAL
fort_status_t set_default_entire_table_property(uint32_t property, int value)
{
    return set_entire_table_property_internal(&g_entire_table_properties, property, value);
}


FT_INTERNAL
size_t max_border_elem_strlen(struct fort_table_properties *properties)
{
    assert(properties);
    size_t result = 1;
    int i = 0;
    for (i = 0; i < BorderItemPosSize; ++i) {
        result = MAX(result, strlen(properties->border_style.border_chars[i]));
    }

    i = 0;
    for (i = 0; i < BorderItemPosSize; ++i) {
        result = MAX(result, strlen(properties->border_style.header_border_chars[i]));
    }

    i = 0;
    for (i = 0; i < SepratorItemPosSize; ++i) {
        result = MAX(result, strlen(properties->border_style.separator_chars[i]));
    }
    return result;
}


fort_table_properties_t g_table_properties = {
    /* border_style */
    BASIC_STYLE,
    NULL,     /* cell_properties */
    /* entire_table_properties */
    {
        0, /* left_margin */
        0, /* top_margin */
        0, /* right_margin */
        0  /* bottom_margin */
    }
};


FT_INTERNAL
fort_table_properties_t *create_table_properties(void)
{
    fort_table_properties_t *properties = (fort_table_properties_t *)F_CALLOC(sizeof(fort_table_properties_t), 1);
    if (properties == NULL) {
        return NULL;
    }
    memcpy(properties, &g_table_properties, sizeof(fort_table_properties_t));
    properties->cell_properties = create_cell_prop_container();
    if (properties->cell_properties == NULL) {
        destroy_table_properties(properties);
        return NULL;
    }
    memcpy(&properties->entire_table_properties, &g_entire_table_properties, sizeof(fort_entire_table_properties_t));
    return properties;
}

FT_INTERNAL
void destroy_table_properties(fort_table_properties_t *properties)
{
    if (properties == NULL)
        return;

    if (properties->cell_properties != NULL) {
        destroy_cell_prop_container(properties->cell_properties);
    }
    F_FREE(properties);
}

static
fort_cell_prop_container_t *copy_cell_properties(fort_cell_prop_container_t *cont)
{
    fort_cell_prop_container_t *result = create_cell_prop_container();
    if (result == NULL)
        return NULL;

    size_t sz = vector_size(cont);
    for (size_t i = 0; i < sz; ++i) {
        fort_cell_props_t *opt = (fort_cell_props_t *)vector_at(cont, i);
        if (FT_IS_ERROR(vector_push(result, opt))) {
            destroy_cell_prop_container(result);
            return NULL;
        }
    }
    return result;
}

FT_INTERNAL
fort_table_properties_t *copy_table_properties(const fort_table_properties_t *properties)
{
    fort_table_properties_t *new_opt = create_table_properties();
    if (new_opt == NULL)
        return NULL;

    destroy_vector(new_opt->cell_properties);
    new_opt->cell_properties = copy_cell_properties(properties->cell_properties);
    if (new_opt == NULL) {
        destroy_table_properties(new_opt);
        return NULL;
    }

    memcpy(&new_opt->border_style, &properties->border_style, sizeof(struct fort_border_style));
    memcpy(&new_opt->entire_table_properties,
           &properties->entire_table_properties, sizeof(fort_entire_table_properties_t));

    return new_opt;
}



