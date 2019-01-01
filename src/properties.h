#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "fort_utils.h"
#include <stdint.h>
#include <limits.h>

#define PROP_IS_SET(ft_props, property) ((ft_props) & (property))
#define PROP_SET(ft_props, property) ((ft_props) |=(property))
#define PROP_UNSET(ft_props, property) ((ft_props) &= ~((uint32_t)property))

#define TEXT_STYLE_TAG_MAX_SIZE 64

FT_INTERNAL
void get_style_tag_for_cell(const fort_table_properties_t *props,
                            size_t row, size_t col, char *style_tag, size_t sz);

FT_INTERNAL
void get_reset_style_tag_for_cell(const fort_table_properties_t *props,
                                  size_t row, size_t col, char *style_tag, size_t sz);

FT_INTERNAL
void get_style_tag_for_content(const fort_table_properties_t *props,
                               size_t row, size_t col, char *style_tag, size_t sz);

FT_INTERNAL
void get_reset_style_tag_for_content(const fort_table_properties_t *props,
                                     size_t row, size_t col, char *style_tag, size_t sz);


struct fort_cell_props {
    size_t cell_row;
    size_t cell_col;
    uint32_t properties;
    unsigned int col_min_width;
    enum ft_text_alignment align;
    unsigned int cell_padding_top;
    unsigned int cell_padding_bottom;
    unsigned int cell_padding_left;
    unsigned int cell_padding_right;
    unsigned int cell_empty_string_height;
    enum ft_row_type row_type;
    unsigned int content_fg_color_number;
    unsigned int content_bg_color_number;
    unsigned int cell_bg_color_number;
    enum ft_text_style cell_text_style;
    enum ft_text_style content_text_style;
};

typedef struct fort_cell_props fort_cell_props_t;
typedef vector_t fort_cell_prop_container_t;

FT_INTERNAL
fort_cell_prop_container_t *create_cell_prop_container(void);

FT_INTERNAL
void destroy_cell_prop_container(fort_cell_prop_container_t *cont);

FT_INTERNAL
const fort_cell_props_t *cget_cell_prop(const fort_cell_prop_container_t *cont, size_t row, size_t col);

FT_INTERNAL
fort_cell_props_t *get_cell_prop_and_create_if_not_exists(fort_cell_prop_container_t *cont, size_t row, size_t col);

FT_INTERNAL
fort_status_t set_cell_property(fort_cell_prop_container_t *cont, size_t row, size_t col, uint32_t property, int value);

FT_INTERNAL
int get_cell_property_value_hierarcial(const fort_table_properties_t *properties, size_t row, size_t column, uint32_t property);

FT_INTERNAL
fort_status_t set_default_cell_property(uint32_t property, int value);


/*         TABLE BORDER DESRIPTION
 *
 *
 *   TL TT TT TT TV TT TT TT TT TT TT TT TR
 *   LL          IV                      RR
 *   LL          IV                      RR
 *   LH IH IH IH II IH IH IH TI IH IH IH RH
 *   LL          IV          IV          RR
 *   LL          IV          IV          RR
 *   LL          LI IH IH IH RI          RH
 *   LL          IV          IV          RR
 *   LL          IV          IV          RR
 *   LH IH IH IH BI IH IH IH II IH IH IH RH
 *   LL                      IV          RR
 *   LL                      IV          RR
 *   BL BB BB BB BV BB BB BB BV BB BB BB BR
 */


/*      HORIZONTAL SEPARATOR DESCRIPTION
 *
 *
 *   TL TT TT TT TV TT TT TT TV TT TT TT TR        <----- TopSeparator
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

    LI_bip = 15,
    TI_bip = 16,
    RI_bip = 17,
    BI_bip = 18,

    BorderItemPosSize
};


enum SeparatorItemPos {
    LH_sip = 0,
    IH_sip = 1,
    II_sip = 2,
    RH_sip = 3,

    TI_sip = 4,
    BI_sip = 5,

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
extern struct fort_border_style FORT_EMPTY2_STYLE;
extern struct fort_border_style FORT_SOLID_STYLE;
extern struct fort_border_style FORT_SOLID_ROUND_STYLE;
extern struct fort_border_style FORT_NICE_STYLE;
extern struct fort_border_style FORT_DOUBLE_STYLE;
extern struct fort_border_style FORT_DOUBLE2_STYLE;
extern struct fort_border_style FORT_BOLD_STYLE;
extern struct fort_border_style FORT_BOLD2_STYLE;
extern struct fort_border_style FORT_FRAME_STYLE;


struct fort_entire_table_properties {
    unsigned int left_margin;
    unsigned int top_margin;
    unsigned int right_margin;
    unsigned int bottom_margin;
};
typedef struct fort_entire_table_properties fort_entire_table_properties_t;
extern fort_entire_table_properties_t g_entire_table_properties;

FT_INTERNAL
fort_status_t set_entire_table_property(fort_table_properties_t *table_properties, uint32_t property, int value);

FT_INTERNAL
fort_status_t set_default_entire_table_property(uint32_t property, int value);

struct fort_table_properties {
    struct fort_border_style border_style;
    fort_cell_prop_container_t *cell_properties;
    fort_entire_table_properties_t entire_table_properties;
};
extern fort_table_properties_t g_table_properties;

FT_INTERNAL
size_t max_border_elem_strlen(struct fort_table_properties *);

FT_INTERNAL
fort_table_properties_t *create_table_properties(void);

FT_INTERNAL
void destroy_table_properties(fort_table_properties_t *properties);

FT_INTERNAL
fort_table_properties_t *copy_table_properties(const fort_table_properties_t *property);

#endif /* PROPERTIES_H */
