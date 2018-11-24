#include "tests.h"


void run_test_suit(const char *test_suit_name, int n_tests, struct test_case test_suit[])
{
    fprintf(stderr, " ==  RUNNING %s ==\n", test_suit_name);
    fprintf(stderr, "[==========] Running %d test(s).\n", n_tests);
    int i;
    for (i = 0; i < n_tests; ++i) {
        fprintf(stderr, "[ RUN      ] %s\n", test_suit[i].name);
        test_suit[i].test();
        fprintf(stderr, "[       OK ] %s\n", test_suit[i].name);
    }
    fprintf(stderr, "[==========] %d test(s) run.\n", n_tests);
    fprintf(stderr, "[  PASSED  ] %d test(s).\n", n_tests);
}
