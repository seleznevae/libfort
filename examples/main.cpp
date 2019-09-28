#include <iostream>
#include <vector>
#include "fort.hpp"


int main()
{
    {
        fort::char_table table;
        // Fill table with data
        table << fort::header
            << "Rank" << "Title" << "Year" << "Rating" << fort::endr
            << "1" << "The Shawshank Redemption" << "1994" << "9.5" << fort::endr
            << "2" << "12 Angry Men" << "1957" << "8.8" << fort::endr
            << "3" << "It's a Wonderful Life" << "1946" << "8.6" << fort::endr
            << fort::separator
            << "4" << "2001: A Space Odyssey" << "1968" << "8.5" << fort::endr
            << "5" << "Blade Runner" << "1982" << "8.1" << fort::endr
            << fort::endr;

        table[0][0].set_cell_min_width(20);
        table[0][0].set_cell_text_align(fort::text_align::left);
        table[2].set_cell_row_type(fort::row_type::header);

        std::cout << table.to_string() << std::endl;
    }


    {
        fort::char_table table;
        // Fill table with data
        table << fort::header;
        table.write_ln("Rank", "Title", "Year", "Rating");
        table.write_ln("1", "The Shawshank Redemption", "1994", "9.5");
        table.write_ln("2", "12 Angry Men", "1957", "8.8");
        table.write_ln("3", "It's a Wonderful Life", "1946", "8.6");
        table.write_ln("4", "2001: A Space Odyssey", "1968", "8.5");
        table.write_ln("5", "Blade Runner", "1982", "8.1");


        table[0][0].set_cell_min_width(20);
        table[0][0].set_cell_text_align(fort::text_align::left);
        table.row(2).set_cell_row_type(fort::row_type::header);

        table.set_left_margin(4);

        table.set_border_style(FT_SOLID_STYLE);
        std::cout << table.to_string();
    }

    {
        fort::char_table table;
        // Fill table with data
        table << fort::header
            << "Rank" << "Title" << "Year" << "Rating" << fort::endr
            << "1" << "The Shawshank Redemption" << "1994" << "9.5" << fort::endr
            << "2" << "12 Angry Men" << "1957" << "8.8" << fort::endr
            << "3" << "It's a Wonderful Life" << "1946" << "8.6" << fort::endr
            << fort::endr;

        table[0][0].set_cell_min_width(20);
        table.column(1).set_cell_text_align(fort::text_align::center);
        table[3][3].set_cell_left_padding(15);

        std::cout << table.to_string() << std::endl;
    }

    {
        fort::char_table table;
        /* Set table border style */
        table.set_border_style(FT_BASIC_STYLE);

        // Fill table with data
        table << fort::header
            << "Rank" << "Title" << "Year" << "Rating" << fort::endr
            << "1" << "The Shawshank Redemption" << "1994" << "9.5" << fort::endr
            << "2" << "12 Angry Men" << "1957" << "8.8" << fort::endr
            << "3" << "It's a Wonderful Life" << "1946" << "8.6" << fort::endr
            << fort::separator
            << "4" << "2001: A Space Odyssey" << "1968" << "8.5" << fort::endr
            << "5" << "Blade Runner" << "1982" << "8.1" << fort::endr
            << fort::endr;

        table.column(0).set_cell_text_align(fort::text_align::center);
        table.column(1).set_cell_text_align(fort::text_align::left);

        std::cout << table.to_string() << std::endl;
    }
    return 0;
}
