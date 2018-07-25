#include "tests.h"
#include <stdio.h>
#include "fort.h"

void test_cpp_table_basic(void);


struct test_case bb_test_suit [] = {
    {"test_cpp_table_basic", test_cpp_table_basic},
};


int run_bb_test_suit(void)
{
    int bb_n_tests = sizeof(bb_test_suit) / sizeof(bb_test_suit[0]);
    run_test_suit("BLACK BOX TEST SUITE", bb_n_tests, bb_test_suit);
    return 0;
}

int main(void)
{
    int status = 0;

    status |= run_bb_test_suit();

    return status;
}
