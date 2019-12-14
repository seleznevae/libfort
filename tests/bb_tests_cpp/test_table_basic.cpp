#include "tests.h"
#include "fort.hpp"
#include "test_utils.hpp"


void test_cpp_bug_fixes(void)
{
    SCENARIO("Issue 11 - https://github.com/seleznevae/libfort/issues/11") {
        fort::char_table table;
        table << fort::header
              << "1" << "2" << fort::endr
              << "3" << "4" << fort::endr;

        table.set_border_style(FT_PLAIN_STYLE);
        table.set_cell_bottom_padding(0);
        table.set_cell_top_padding(0);
        table.set_cell_left_padding(1);
        table.set_cell_right_padding(1);
        table.set_cell_empty_str_height(0);

        std::string table_str = table.to_string();
        std::string table_str_etalon =
            "-------\n"
            " 1   2 \n"
            "-------\n"
            " 3   4 \n";
        assert_string_equal(table_str, table_str_etalon);
    }
}

void test_cpp_table_basic(void)
{
    WHEN("Empty table.") {
        fort::char_table table;
        assert_true(set_cpp_test_props_for_table(&table));

        std::string table_str = table.to_string();
        std::string table_str_etalon = "";
        assert_string_equal(table_str, table_str_etalon);
        assert_true(table.is_empty());
    }

    WHEN("All columns are equal and not empty.") {
        fort::char_table table;
        assert_true(set_cpp_test_props_for_table(&table));

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
        assert_true(table.is_empty() == false);
    }

    WHEN("Checking basic constructors and assignmets.") {
        fort::char_table table;
        assert_true(set_cpp_test_props_for_table(&table));

        table << fort::header
              << "3" << "c" << "234" << "3.140000" << fort::endr
              << "3" << "c" << "234" << "3.140000" << fort::endr
              << "3" << "c" << "234" << "3.140000" << fort::endr;

        fort::char_table table2(std::move(table));
        fort::char_table table3;
        table3 = std::move(table2);

        fort::char_table table4(table3);
        fort::char_table table5;
        table5 = table4;

        std::string table_str = table5.to_string();
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
        fort::char_table table;
        assert_true(set_cpp_test_props_for_table(&table));

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
        fort::char_table table;
        assert_true(set_cpp_test_props_for_table(&table));

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
        fort::char_table table;
        assert_true(set_cpp_test_props_for_table(&table));

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
        fort::char_table table;
        assert_true(set_cpp_test_props_for_table(&table));
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
        table[1][2] = std::string("3.140000");
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
        fort::char_table table;
        assert_true(set_cpp_test_props_for_table(&table));
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

    SCENARIO("Test range_write functions") {
        fort::char_table table;
        assert_true(set_cpp_test_props_for_table(&table));

        table << fort::header;
        const char *row_0[4] = {"3", "c", "234", "3.140000"};
        const char *row_1[4] = {"c", "235", "3.150000", "5"};
        const char *row_2[4] = {"234", "3.140000", "3", "c"};
        assert_true(table.range_write_ln(std::begin(row_0), std::end(row_0)));
        assert_true(table.range_write(std::begin(row_1), std::end(row_1)));
        table << fort::endr;
        assert_true(table.range_write_ln(std::begin(row_2), std::end(row_2)));

        /* Replace old values */
        table[1][1] = "234";
        const char *row_11[2] = {"3.140000", "3"};
        assert_true(table.range_write_ln(std::begin(row_11), std::end(row_11)));


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

void test_cpp_table_changing_cell(void)
{
    WHEN("All columns are equal and not empty") {
        fort::char_table table;
        assert_true(set_cpp_test_props_for_table(&table));

        table << fort::header
              << "3" << "c" << "234" << "3.140000" << fort::endr
              << "3" << "c" << "234" << "3.140000" << fort::endr
              << "3" << "c" << "234" << "3.140000" << fort::endr;

        assert_true(table.cur_row() == 3);
        assert_true(table.cur_col() == 0);

        table.set_cur_cell(1, 1);
        assert_true(table.cur_row() == 1);
        assert_true(table.cur_col() == 1);
        table << "A";
        assert_true(table.cur_row() == 1);
        assert_true(table.cur_col() == 2);
        table.cur_cell() = "432";

        std::string table_str = table.to_string();
        std::string table_str_etalon =
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | A | 432 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n"
            "|   |   |     |          |\n"
            "| 3 | c | 234 | 3.140000 |\n"
            "|   |   |     |          |\n"
            "+---+---+-----+----------+\n";
        assert_string_equal(table_str, table_str_etalon);
    }
}
