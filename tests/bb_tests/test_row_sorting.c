#include "tests.h"
#include "test_utils.h"
#include "fort.h"

enum sort_type {
    asc,
    desc
};

struct sort_cntx {
    size_t col_beg;
    size_t col_end;
    enum sort_type type;
};

int cmp_rows(const ft_row_t *r1, const ft_row_t *r2, void *arg)
{
    struct sort_cntx *cntx = (struct sort_cntx *)arg;
    for (size_t i = cntx->col_beg; i <= cntx->col_end; ++i) {
        int k = strcmp(ft_get_cell(r1, i), ft_get_cell(r2, i));
        if (k != 0) {
            if (cntx->type == asc) {
                return k;
            } else {
                return -k;
            }
        }
    }
    return 0;
}

// TODO: write more tests for string sorting.
int cmp_asc(int col, void *lhs, void *rhs)
{
    (void)col;
    return strcmp(lhs, rhs);
}


int cmp_desc(int col, void *lhs, void *rhs)
{
    (void)col;
    return -strcmp(lhs, rhs);
}

void test_table_sort(void)
{
    ft_table_t *table = NULL;

    SCENARIO("Test already sorted") {
        table = ft_create_table();
        assert_true(table != NULL);
        ft_printf_ln(table, "a");
        ft_printf_ln(table, "b");
        ft_printf_ln(table, "c");
        ft_printf_ln(table, "k");
        ft_printf_ln(table, "m");
        ft_printf_ln(table, "y");
        ft_printf_ln(table, "z");

        struct sort_cntx cntx = {0, 0, asc};
        ft_sort_rows(table, 0, ft_row_count(table) - 1, cmp_rows, &cntx);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+---+\n"
            "| a |\n"
            "| b |\n"
            "| c |\n"
            "| k |\n"
            "| m |\n"
            "| y |\n"
            "| z |\n"
            "+---+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    SCENARIO("Test ascending sorting") {
        table = ft_create_table();
        assert_true(table != NULL);
        ft_printf_ln(table, "a");
        ft_printf_ln(table, "z");
        ft_printf_ln(table, "b");
        ft_printf_ln(table, "m");
        ft_printf_ln(table, "y");
        ft_printf_ln(table, "c");
        ft_printf_ln(table, "k");

        struct sort_cntx cntx = {0, 0, asc};
        ft_sort_rows(table, 0, ft_row_count(table) - 1, cmp_rows, &cntx);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+---+\n"
            "| a |\n"
            "| b |\n"
            "| c |\n"
            "| k |\n"
            "| m |\n"
            "| y |\n"
            "| z |\n"
            "+---+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    SCENARIO("Test descending sorting") {
        table = ft_create_table();
        assert_true(table != NULL);
        ft_printf_ln(table, "a");
        ft_printf_ln(table, "z");
        ft_printf_ln(table, "b");
        ft_printf_ln(table, "m");
        ft_printf_ln(table, "y");
        ft_printf_ln(table, "c");
        ft_printf_ln(table, "k");

        struct sort_cntx cntx = {0, 0, desc};
        ft_sort_rows(table, 0, ft_row_count(table) - 1, cmp_rows, &cntx);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+---+\n"
            "| z |\n"
            "| y |\n"
            "| m |\n"
            "| k |\n"
            "| c |\n"
            "| b |\n"
            "| a |\n"
            "+---+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    SCENARIO("Test partial sorting") {
        table = ft_create_table();
        assert_true(table != NULL);
        ft_printf_ln(table, "a");
        ft_printf_ln(table, "z");
        ft_printf_ln(table, "b");
        ft_printf_ln(table, "m");
        ft_printf_ln(table, "y");
        ft_printf_ln(table, "c");
        ft_printf_ln(table, "k");

        struct sort_cntx cntx = {0, 0, asc};
        ft_sort_rows(table, 2, ft_row_count(table) - 1, cmp_rows, &cntx);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+---+\n"
            "| a |\n"
            "| z |\n"
            "| b |\n"
            "| c |\n"
            "| k |\n"
            "| m |\n"
            "| y |\n"
            "+---+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    SCENARIO("Test sorting by several columns") {
        table = ft_create_table();
        assert_true(table != NULL);
        ft_printf_ln(table, "1|zz|zz|11");
        ft_printf_ln(table, "2|we|zz|22");
        ft_printf_ln(table, "3|aa|zz|33");
        ft_printf_ln(table, "4|we|aa|44");
        ft_printf_ln(table, "5|we|aa|55");
        ft_printf_ln(table, "6|zz|za|66");
        ft_printf_ln(table, "7|aa|ab|77");

        struct sort_cntx cntx = {1, 2, asc};
        ft_sort_rows(table, 0, ft_row_count(table) - 1, cmp_rows, &cntx);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+---+----+----+----+\n"
            "| 7 | aa | ab | 77 |\n"
            "| 3 | aa | zz | 33 |\n"
            "| 4 | we | aa | 44 |\n"
            "| 5 | we | aa | 55 |\n"
            "| 2 | we | zz | 22 |\n"
            "| 6 | zz | za | 66 |\n"
            "| 1 | zz | zz | 11 |\n"
            "+---+----+----+----+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

    SCENARIO("Test sorting stability columns") {
        table = ft_create_table();
        assert_true(table != NULL);
        ft_printf_ln(table, "2|23|33|11");
        ft_printf_ln(table, "1|23|33|22");
        ft_printf_ln(table, "2|23|33|33");
        ft_printf_ln(table, "1|23|33|44");
        ft_printf_ln(table, "3|23|33|55");
        ft_printf_ln(table, "3|23|33|66");
        ft_printf_ln(table, "3|23|33|77");
        ft_printf_ln(table, "0|23|33|88");
        ft_printf_ln(table, "0|23|33|99");

        struct sort_cntx cntx = {0, 2, asc};
        ft_sort_rows(table, 0, ft_row_count(table) - 1, cmp_rows, &cntx);

        const char *table_str = ft_to_string(table);
        assert_true(table_str != NULL);
        const char *table_str_etalon =
            "+---+----+----+----+\n"
            "| 0 | 23 | 33 | 88 |\n"
            "| 0 | 23 | 33 | 99 |\n"
            "| 1 | 23 | 33 | 22 |\n"
            "| 1 | 23 | 33 | 44 |\n"
            "| 2 | 23 | 33 | 11 |\n"
            "| 2 | 23 | 33 | 33 |\n"
            "| 3 | 23 | 33 | 55 |\n"
            "| 3 | 23 | 33 | 66 |\n"
            "| 3 | 23 | 33 | 77 |\n"
            "+---+----+----+----+\n";
        assert_str_equal(table_str, table_str_etalon);
        ft_destroy_table(table);
    }

}
