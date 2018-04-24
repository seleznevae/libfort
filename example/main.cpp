#include <iostream>
#include "fort.hpp"


int main()
{
    {
        fort::Table table;
        // Fill table with data
        table << fort::header
            << "Rank" << "Title" << "Year" << "Rating" << fort::endl
            << "1" << "The Shawshank Redemption" << "1994" << "9.5" << fort::endl
            << "2" << "12 Angry Men" << "1957" << "8.8" << fort::endl
            << "3" << "It's a Wonderful Life" << "1946" << "8.6" << fort::endl
            << fort::separator
            << "4" << "2001: A Space Odyssey" << "1968" << "8.5" << fort::endl
            << "5" << "Blade Runner" << "1982" << "8.1" << fort::endl
            << fort::endl;
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
        std::cout << table.to_string();
    }
    return 0;
}
