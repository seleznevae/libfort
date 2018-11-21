#include "fort.hpp"
#include "tests.h"


bool set_cpp_test_props_for_table(fort::table *table)
{
    assert_true(table->set_cell_bottom_padding(1));
    assert_true(table->set_cell_top_padding(1));
    assert_true(table->set_cell_left_padding(1));
    assert_true(table->set_cell_right_padding(1));
    assert_true(table->set_cell_empty_str_height(1));

    struct ft_border_style brdr_style;
    brdr_style.border_chs.top_border_ch = "-";
    brdr_style.border_chs.separator_ch = "-";
    brdr_style.border_chs.bottom_border_ch = "-";
    brdr_style.border_chs.side_border_ch = "|";
    brdr_style.border_chs.out_intersect_ch = "+";
    brdr_style.border_chs.in_intersect_ch = "+";

    brdr_style.header_border_chs.top_border_ch = "-";
    brdr_style.header_border_chs.separator_ch = "-";
    brdr_style.header_border_chs.bottom_border_ch = "-";
    brdr_style.header_border_chs.side_border_ch = "|";
    brdr_style.header_border_chs.out_intersect_ch = "+";
    brdr_style.header_border_chs.in_intersect_ch = "+";

    brdr_style.hor_separator_char = "=";
    table->set_border_style(&brdr_style);

    return true;
}



fort::table create_cpp_test_int_table(int set_test_opts)
{
    fort::table table;

    if (set_test_opts) {
        assert_true(set_cpp_test_props_for_table(&table) == true);
    }

    table[0].set_cell_row_type(fort::row_type::header);
    assert(table.write_ln("3", "4", "55", "67"));
    assert(table.write_ln("3", "4", "55", "67"));
    assert(table.write_ln("3", "4", "55", "67"));

    return table;
}
