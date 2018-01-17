#ifndef OPTIONS_H
#define OPTIONS_H

#include "fort_impl.h"

enum TextAlignment
{
    LeftAligned,
    CenterAligned,
    RightAligned
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



struct fort_table_options
{
    int cell_padding_top;
    int cell_padding_bottom;
    int cell_padding_left;
    int cell_padding_right;
    int cell_empty_string_height;

    char border_chars[BorderItemPosSize];
    char header_border_chars[BorderItemPosSize];
    vector_t *col_options;

};
typedef struct fort_table_options fort_table_options_t;
typedef fort_table_options_t context_t;
extern fort_table_options_t g_table_options;



fort_table_options_t* create_table_options();
void destroy_table_options(fort_table_options_t* options);
fort_status_t fort_options_set_column_min_width(fort_table_options_t *options, size_t column, size_t width);
fort_status_t fort_options_set_column_alignment(fort_table_options_t *options, size_t column, enum TextAlignment al);
int fort_options_column_width(const fort_table_options_t *options, size_t column);
int fort_options_column_alignment(const fort_table_options_t *options, size_t column);

#endif // OPTIONS_H
