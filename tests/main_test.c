#include "tests.h"
#include <stdio.h>
#include "fort.h"

int main(void)
{
    int status = 0;

#ifdef FORT_WB_TESTING_ENABLED
    status |= run_wb_test_suit();
    fprintf(stderr, "\n");
#endif
    status |= run_bb_test_suit();

    return status;
}
