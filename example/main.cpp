#include <iostream>
#include <vector>
#include "fort.hpp"

fort::Table create_basic_table(void)
{
    fort::Table table;
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

    table.set_cell_text_align(FT_ANY_ROW, 0, fort::TextAlign::Center);
    table.set_cell_text_align(FT_ANY_ROW, 1, fort::TextAlign::Left);

    std::cout << table.to_string() << std::endl;
    return table;
}

void base_example(void)
{
    fort::Table table;
    table << fort::header
        << "N" << "Driver" << "Time" << "Avg Speed" << fort::endr
        << "1" << "Ricciardo" << "1:25.945" << "47.362" << fort::endr
        << "2" << "Hamilton" << "1:26.373" << "35.02" << fort::endr
        << "3" << "Verstappen" << "1:26.469" << "29.78" << fort::endr;

    std::cout << table.to_string() << std::endl;
}

void different_cell_options_example(void)
{
    fort::Table table;
    /* Change border style */
    table.set_border_style(FT_DOUBLE2_STYLE);

    table << fort::header
        << "Movie title" << "Director" << "Year" << "Rating" << fort::endr
        << "The Shawshank Redemption" << "Frank Darabont" << "1994" << "9.5" << fort::endr
        << "The Godfather" << "Francis Ford Coppola" << "1972" << "9.2" << fort::endr
        << "2001: A Space Odyssey" << "Stanley Kubrick" << "1968" << "8.5" << fort::endr;

    /* Set center alignment for the 1st and 3rd columns */
    table.set_cell_text_align(FT_ANY_ROW, 1, fort::TextAlign::Center);
    table.set_cell_text_align(FT_ANY_ROW, 3, fort::TextAlign::Center);

    std::cout << table.to_string() << std::endl;
}


void fill_table_with_data_example(void)
{
    fort::Table table;
    table << fort::header;
    /* Fill each cell with operator[] */
    table [0][0] = "N";
    table [0][1] = "Planet";
    table [0][2] = "Speed, km/s";
    table [0][3] = "Temperature, K";
    table << fort::endr;


    /* Fill with iostream operator<< */
    table << 1 << "Mercury" << 47.362 << 340 << fort::endr;

    /* Fill row explicitly with strings */
    table.write_ln("2", "Venus", "35.02", "737");

    /* Fill row with data from the container */
    std::vector<std::string> arr = {"3", "Earth", "29.78", "288"};
    table.row_write_ln(std::begin(arr), std::end(arr));

    std::cout << table.to_string() << std::endl;
}


int main()
{
    base_example();
    different_cell_options_example();
    fill_table_with_data_example();

    {
        fort::Table table;
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

        using fort::CellOption;
        using fort::TableOption;
        table.set_option<CellOption::MinWidth>(0, 0, 20);
        table.set_option<CellOption::TextAlign>(0, 0, fort::TextAlign::Left);
        table.set_option<CellOption::RowType>(2, FT_ANY_COLUMN, fort::RowType::Header);

        std::cout << table.to_string() << std::endl;
    }


    {
        fort::Table table;
        // Fill table with data
        table << fort::header;
        table.write_ln("Rank", "Title", "Year", "Rating");
        table.write_ln("1", "The Shawshank Redemption", "1994", "9.5");
        table.write_ln("2", "12 Angry Men", "1957", "8.8");
        table.write_ln("3", "It's a Wonderful Life", "1946", "8.6");
        table.write_ln("4", "2001: A Space Odyssey", "1968", "8.5");
        table.write_ln("5", "Blade Runner", "1982", "8.1");

        using fort::CellOption;
        using fort::TableOption;
        table.set_cell_min_width(0, 0, 20);
        table.set_cell_text_align(0, 0, fort::TextAlign::Left);
        table.set_cell_row_type(2, FT_ANY_COLUMN, fort::RowType::Header);


        table.set_option<TableOption::LeftMargin>(4);

        table.set_border_style(FT_SOLID_STYLE);
        std::cout << table.to_string();
    }
    return 0;
}
