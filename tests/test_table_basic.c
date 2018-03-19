#include "tests.h"

void test_table_basic(void)
{
    FTABLE *table = NULL;

    WHEN("All columns are equal and not empty") {
        table = ft_create_table();
        assert_true( table != NULL );
        assert_true( set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
        int n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);

        assert_true( n == 4 );
        n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true( n == 4 );
        n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);
        assert_true( n == 4 );

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
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
//        fprintf(stderr, "content:\n%s", table_str);

        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }



    WHEN("All columns are not equal and not empty") {
        table = ft_create_table();
        assert_true( table != NULL );
        assert_true( set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
        int n = ft_printf_ln(table, "%d|%c|%s|%f", 3, 'c', "234", 3.14);

        assert_true( n == 4 );
        n = ft_printf_ln(table, "%c|%s|%f|%d", 'c', "234", 3.14, 3);
        assert_true( n == 4 );
        n = ft_printf_ln(table, "%s|%f|%d|%c", "234", 3.14,  3, 'c');
        assert_true( n == 4 );

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "+-----+----------+----------+----------+\n"
                "|     |          |          |          |\n"
                "|   3 |        c |      234 | 3.140000 |\n"
                "|     |          |          |          |\n"
                "+-----+----------+----------+----------+\n"
                "|     |          |          |          |\n"
                "|   c |      234 | 3.140000 |        3 |\n"
                "|     |          |          |          |\n"
                "+-----+----------+----------+----------+\n"
                "|     |          |          |          |\n"
                "| 234 | 3.140000 |        3 |        c |\n"
                "|     |          |          |          |\n"
                "+-----+----------+----------+----------+\n";

//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("All columns are not equal and some cells are empty") {
        table = ft_create_table();
        assert_true( table != NULL );
//        ft_set_table_options(table, &test_table_opts);
        assert_true( set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
        int n = ft_printf_ln(table, "||%s|%f", "234", 3.14);

        assert_true( n == 4 );
        n = ft_printf_ln(table, "%c|%s|%f", 'c', "234", 3.14);
        assert_true( n == 3 );
        n = ft_printf_ln(table, "%s|%f||", "234", 3.14);
        assert_true( n == 4 );

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
                "+-----+----------+----------+----------+\n"
                "|     |          |          |          |\n"
                "|     |          |      234 | 3.140000 |\n"
                "|     |          |          |          |\n"
                "+-----+----------+----------+----------+\n"
                "|     |          |          |          |\n"
                "|   c |      234 | 3.140000 |          |\n"
                "|     |          |          |          |\n"
                "+-----+----------+----------+----------+\n"
                "|     |          |          |          |\n"
                "| 234 | 3.140000 |          |          |\n"
                "|     |          |          |          |\n"
                "+-----+----------+----------+----------+\n";

//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }

    WHEN("All cells are empty") {
        table = ft_create_table();
        assert_true( table != NULL );
        assert_true( set_test_options_for_table(table) == FT_SUCCESS);

        ft_set_option(table, 0, FT_ANY_COLUMN, FT_OPT_ROW_TYPE, Header);
        int n = ft_printf_ln(table, "|||");

        assert_true( n == 4 );
        n = ft_printf_ln(table, "|||");
        assert_true( n == 4 );
        n = ft_printf_ln(table, "|||");
        assert_true( n == 4 );

        const char *table_str = ft_to_string(table);
        assert_true( table_str != NULL );
        const char *table_str_etalon =
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


//        fprintf(stderr, "content:\n%s", table_str);
        assert_true( strcmp(table_str, table_str_etalon) == 0);

        ft_destroy_table(table);
    }
}
