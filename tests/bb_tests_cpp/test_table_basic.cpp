#include "tests.h"
#include "fort.hpp"

#define assert_string_equal(str1, str2) assert_str_equal(str1.c_str(), str2.c_str())

bool set_test_options_for_table(fort::Table *table)
{
    assert_true(table->set_cell_bottom_padding(FT_ANY_ROW, FT_ANY_COLUMN, 1));
    assert_true(table->set_cell_top_padding(FT_ANY_ROW, FT_ANY_COLUMN, 1));
    assert_true(table->set_cell_left_padding(FT_ANY_ROW, FT_ANY_COLUMN, 1));
    assert_true(table->set_cell_right_padding(FT_ANY_ROW, FT_ANY_COLUMN, 1));
    assert_true(table->set_cell_empty_str_height(FT_ANY_ROW, FT_ANY_COLUMN, 1));

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

void test_cpp_table_basic(void)
{
    WHEN("All columns are equal and not empty.") {
        fort::Table table;
        assert_true(set_test_options_for_table(&table));

        table << fort::header
              << "3" << "c" << "234" << "3.140000" << fort::endr
              << "3" << "c" << "234" << "3.140000" << fort::endr
              << "3" << "c" << "234" << "3.140000" << fort::endr;

        std::string table_str = table.to_string();
        std::string table_str_etalon =
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n";
        assert_string_equal(table_str, table_str_etalon);
    }

    WHEN("Checking basic constructors and assignmets.") {
        fort::Table table;
        assert_true(set_test_options_for_table(&table));

        table << fort::header
              << "3" << "c" << "234" << "3.140000" << fort::endr
              << "3" << "c" << "234" << "3.140000" << fort::endr
              << "3" << "c" << "234" << "3.140000" << fort::endr;

        fort::Table table2(std::move(table));
        fort::Table table3;
        table3 = std::move(table2);

        std::string table_str = table3.to_string();
        std::string table_str_etalon =
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n";
        assert_string_equal(table_str, table_str_etalon);
    }

    WHEN("All columns are not equal and not empty") {
        fort::Table table;
        assert_true(set_test_options_for_table(&table));

        table << fort::header
              << "3" << "c" << "234" << "3.140000" << fort::endr
              << "c" << "234" << "3.140000" << "3" << fort::endr
              << "234" << "3.140000" << "3" << "c" << fort::endr;

        std::string table_str = table.to_string();
        std::string table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 3   | c        | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 | 3        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 | 3        | c        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_string_equal(table_str, table_str_etalon);
    }

    WHEN("All columns are not equal and some cells are empty") {
        fort::Table table;
        assert_true(set_test_options_for_table(&table));

        table << fort::header
              << "" << "" << "234" << "3.140000" << fort::endr
              << "c" << "234" << "3.140000" << "" << fort::endr
              << "234" << "3.140000" << "" << "" << fort::endr;

        std::string table_str = table.to_string();
        std::string table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "|     |          | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 |          |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 |          |          |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_string_equal(table_str, table_str_etalon);
    }

    WHEN("All cells are empty") {
        fort::Table table;
        assert_true(set_test_options_for_table(&table));

        table << fort::header
              << "" << "" << "" << "" << fort::endr
              << "" << "" << "" << "" << fort::endr
              << "" << "" << "" << "" << fort::endr;

        std::string table_str = table.to_string();
        std::string table_str_etalon =
            "+--+--+--+--+\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "+--+--+--+--+\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "+--+--+--+--+\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "|  |  |  |  |\n"
            "+--+--+--+--+\n";
        assert_string_equal(table_str, table_str_etalon);
    }
}


void test_cpp_table_write(void)
{
    SCENARIO("Test write functions") {
        fort::Table table;
        assert_true(set_test_options_for_table(&table));
        table << fort::header;
        assert_true(table.write("3"));
        assert_true(table.write("c"));
        assert_true(table.write("234"));
        assert_true(table.write_ln("3.140000"));

        assert_true(table.write("c"));
        assert_true(table.write("235"));
        assert_true(table.write("3.150000"));
        assert_true(table.write_ln("5"));

        assert_true(table.write("234"));
        assert_true(table.write("3.140000"));
        assert_true(table.write("3"));
        assert_true(table.write_ln("c"));

        /* Replace old values */
        table[1][1] = "234";
        assert_true(table.write("3.140000"));
        assert_true(table.write_ln("3"));

        std::string table_str = table.to_string();
        std::string table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 3   | c        | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 | 3        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 | 3        | c        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_string_equal(table_str, table_str_etalon);
    }

    SCENARIO("Test n write functions") {
        fort::Table table;
        assert_true(set_test_options_for_table(&table));
        table << fort::header;
        assert_true(table.write("3", "c", "234", "3.140000"));
        table << fort::endr;

        assert_true(table.write_ln("c", "235", "3.150000", "5"));
        assert_true(table.write_ln("234", "3.140000", "3", "c"));

        /* Replace old values */
        table[1][1] = "234";
        assert_true(table.write_ln("3.140000", "3"));

        std::string table_str = table.to_string();
        std::string table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 3   | c        | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 | 3        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 | 3        | c        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_string_equal(table_str, table_str_etalon);
    }

    SCENARIO("Test row_write functions") {
        fort::Table table;
        assert_true(set_test_options_for_table(&table));

        table << fort::header;
        const char *row_0[4] = {"3", "c", "234", "3.140000"};
        const char *row_1[4] = {"c", "235", "3.150000", "5"};
        const char *row_2[4] = {"234", "3.140000", "3", "c"};
        assert_true(table.row_write_ln(std::begin(row_0), std::end(row_0)));
        assert_true(table.row_write(std::begin(row_1), std::end(row_1)));
        table << fort::endr;
        assert_true(table.row_write_ln(std::begin(row_2), std::end(row_2)));

        /* Replace old values */
        table[1][1] = "234";
        const char *row_11[3] = {"3.140000", "3"};
        assert_true(table.row_write_ln(std::begin(row_11), std::end(row_11)));


        std::string table_str = table.to_string();
        std::string table_str_etalon =
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 3   | c        | 234      | 3.140000 |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| c   | 234      | 3.140000 | 3        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n"
            "|     |          |          |          |\n"
            "| 234 | 3.140000 | 3        | c        |\n"
            "|     |          |          |          |\n"
            "+-----+----------+----------+----------+\n";
        assert_string_equal(table_str, table_str_etalon);
    }
}
