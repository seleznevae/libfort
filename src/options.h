#ifndef OPTIONS_H
#define OPTIONS_H

#include "fort_impl.h"
#include <stdint.h>
#include <limits.h>

enum TextAlignment
{
    LeftAligned,
    CenterAligned,
    RightAligned
};

enum RowType
{
    Common,
    Header
};


struct fort_column_options
{
    int col_min_width;
    enum TextAlignment align;
};
typedef struct fort_column_options fort_column_options_t;

extern fort_column_options_t g_column_options;
fort_column_options_t create_column_options();



struct vector;
typedef struct vector vector_t;

#define FT_ANY_COLUMN  (UINT_MAX)
#define FT_ANY_ROW  (UINT_MAX)

#define FT_ROW_UNSPEC  (UINT_MAX-1)
#define FT_COLUMN_UNSPEC  (UINT_MAX-1)

#define FT_OPT_MIN_WIDTH  ((uint32_t)(0x01U << (0)))
#define FT_OPT_TEXT_ALIGN ((uint32_t)(0x01U << (1)))
#define FT_OPT_TOP_PADDING  ((uint32_t)(0x01U << (2)))
#define FT_OPT_BOTTOM_PADDING ((uint32_t)(0x01U << (3)))
#define FT_OPT_LEFT_PADDING ((uint32_t)(0x01U << (4)))
#define FT_OPT_RIGHT_PADDING ((uint32_t)(0x01U << (5)))
#define FT_OPT_EMPTY_STR_HEIGHT ((uint32_t)(0x01U << (6)))
#define FT_OPT_ROW_TYPE ((uint32_t)(0x01U << (7)))

#define OPTION_IS_SET(ft_opts, option) ((ft_opts) & (option))
#define OPTION_SET(ft_opts, option) ((ft_opts) |=(option))
#define OPTION_UNSET(ft_opts, option) ((ft_opts) &= ~((uint32_t)option))

struct fort_cell_options
{
    unsigned cell_row;
    unsigned cell_col;
    uint32_t options;
    int col_min_width;
    enum TextAlignment align;
    int cell_padding_top;
    int cell_padding_bottom;
    int cell_padding_left;
    int cell_padding_right;
    int cell_empty_string_height;
    enum RowType row_type;
};

typedef struct fort_cell_options fort_cell_options_t;



typedef vector_t fort_cell_opt_container_t;
fort_cell_opt_container_t *create_cell_opt_container();
void destroy_cell_opt_container(fort_cell_opt_container_t *cont);
const fort_cell_options_t* cget_cell_opt(const fort_cell_opt_container_t *cont, unsigned row, unsigned col);
fort_cell_options_t* get_cell_opt_and_create_if_not_exists(fort_cell_opt_container_t *cont, unsigned row, unsigned col);
fort_status_t set_cell_option(fort_cell_opt_container_t *cont, unsigned row, unsigned col, uint32_t option, int value);
fort_status_t unset_cell_option(fort_cell_opt_container_t *cont, unsigned row, unsigned col, uint32_t option);

int get_cell_opt_value_hierarcial(const fort_table_options_t *options, size_t row, size_t column, uint32_t option);

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

enum HorSeparatorPos
{
    TopSeparator,
    InsideSeparator,
    BottomSeparator
};

enum BorderItemPos
{
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


enum SeparatorItemPos
{
    LH_sip = 0,
    IH_sip = 1,
    II_sip = 2,
    RH_sip = 3,

    SepratorItemPosSize
};


struct fort_table_options
{
    char border_chars[BorderItemPosSize];
    char header_border_chars[BorderItemPosSize];
    char separator_chars[SepratorItemPosSize];

    fort_cell_opt_container_t * cell_options;
};
typedef struct fort_table_options fort_table_options_t;
/*typedef fort_table_options_t context_t;*/
extern fort_table_options_t g_table_options;



fort_table_options_t* create_table_options();
fort_table_options_t* copy_table_options(const fort_table_options_t *option);
void destroy_table_options(fort_table_options_t* options);



#endif /* OPTIONS_H */
