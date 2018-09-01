#ifndef OPTIONS_H
#define OPTIONS_H

#include "fort_utils.h"
#include <stdint.h>
#include <limits.h>

struct fort_column_options {
    int col_min_width;
    enum ft_text_alignment align;
};

extern fort_column_options_t g_column_options;

FT_INTERNAL
fort_column_options_t create_column_options(void);

#define OPTION_IS_SET(ft_opts, option) ((ft_opts) & (option))
#define OPTION_SET(ft_opts, option) ((ft_opts) |=(option))
#define OPTION_UNSET(ft_opts, option) ((ft_opts) &= ~((uint32_t)option))

struct fort_cell_options {
    size_t cell_row;
    size_t cell_col;
    uint32_t options;
    unsigned int col_min_width;
    enum ft_text_alignment align;
    unsigned int cell_padding_top;
    unsigned int cell_padding_bottom;
    unsigned int cell_padding_left;
    unsigned int cell_padding_right;
    unsigned int cell_empty_string_height;
    enum ft_row_type row_type;
};

typedef struct fort_cell_options fort_cell_options_t;
typedef vector_t fort_cell_opt_container_t;

FT_INTERNAL
fort_cell_opt_container_t *create_cell_opt_container(void);

FT_INTERNAL
void destroy_cell_opt_container(fort_cell_opt_container_t *cont);

FT_INTERNAL
const fort_cell_options_t *cget_cell_opt(const fort_cell_opt_container_t *cont, size_t row, size_t col);

FT_INTERNAL
fort_cell_options_t *get_cell_opt_and_create_if_not_exists(fort_cell_opt_container_t *cont, size_t row, size_t col);

FT_INTERNAL
fort_status_t set_cell_option(fort_cell_opt_container_t *cont, size_t row, size_t col, uint32_t option, int value);

FT_INTERNAL
int get_cell_opt_value_hierarcial(const fort_table_options_t *options, size_t row, size_t column, uint32_t option);

FT_INTERNAL
fort_status_t set_default_cell_option(uint32_t option, int value);

/*****************************************************************************
 *               TABLE BORDER
 *****************************************************************************/

/*
 *   TL TT TT TT TV TT TT TT TV TB TB TB TR        <----- TopSeparator
 *   LL          IV          IV          RR
 *   LH IH IH IH II IH IH IH II IH IH IH RH        <----- InsideSeparator
 *   LL          IV          IV          RR
 *   BL BB BB BB BV BB BB BB BV BB BB BB BR        <----- BottomSeparator
 */

enum HorSeparatorPos {
    TopSeparator,
    InsideSeparator,
    BottomSeparator
};

enum BorderItemPos {
    TL_bip = 0,
    TT_bip = 1,
    TV_bip = 2,
    TR_bip = 3,

    LL_bip = 4,
    IV_bip = 5,
    RR_bip = 6,

    LH_bip = 7,
    IH_bip = 8,
    II_bip = 9,
    RH_bip = 10,

    BL_bip = 11,
    BB_bip = 12,
    BV_bip = 13,
    BR_bip = 14,

    BorderItemPosSize
};


enum SeparatorItemPos {
    LH_sip = 0,
    IH_sip = 1,
    II_sip = 2,
    RH_sip = 3,

    SepratorItemPosSize
};


struct fort_border_style {
    const char *border_chars[BorderItemPosSize];
    const char *header_border_chars[BorderItemPosSize];
    const char *separator_chars[SepratorItemPosSize];
};
extern struct fort_border_style FORT_BASIC_STYLE;
extern struct fort_border_style FORT_BASIC2_STYLE;
extern struct fort_border_style FORT_SIMPLE_STYLE;
extern struct fort_border_style FORT_PLAIN_STYLE;
extern struct fort_border_style FORT_DOT_STYLE;
extern struct fort_border_style FORT_EMPTY_STYLE;
extern struct fort_border_style FORT_SOLID_STYLE;
extern struct fort_border_style FORT_SOLID_ROUND_STYLE;
extern struct fort_border_style FORT_DOUBLE_STYLE;
extern struct fort_border_style FORT_DOUBLE2_STYLE;
extern struct fort_border_style FORT_BOLD_STYLE;
extern struct fort_border_style FORT_BOLD2_STYLE;
extern struct fort_border_style FORT_FRAME_STYLE;


struct fort_entire_table_options {
    unsigned int left_margin;
    unsigned int top_margin;
    unsigned int right_margin;
    unsigned int bottom_margin;
};
typedef struct fort_entire_table_options fort_entire_table_options_t;
extern fort_entire_table_options_t g_entire_table_options;

FT_INTERNAL
fort_status_t set_entire_table_option(fort_table_options_t *table_options, uint32_t option, int value);

FT_INTERNAL
fort_status_t set_default_entire_table_option(uint32_t option, int value);

struct fort_table_options {
    struct fort_border_style border_style;
    fort_cell_opt_container_t *cell_options;
    fort_entire_table_options_t entire_table_options;
};
extern fort_table_options_t g_table_options;

FT_INTERNAL
size_t max_border_elem_strlen(struct fort_table_options *);

FT_INTERNAL
fort_table_options_t *create_table_options(void);

FT_INTERNAL
fort_table_options_t *copy_table_options(const fort_table_options_t *option);

FT_INTERNAL
void destroy_table_options(fort_table_options_t *options);

#endif /* OPTIONS_H */
