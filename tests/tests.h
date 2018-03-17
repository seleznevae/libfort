#ifndef TESTS_H
#define TESTS_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#define WHEN(...)
#define THEN(...)


void test_vector_basic(void **state);

void test_table_sizes(void **state);
void test_table_geometry(void **state);
void test_table_basic(void **state);
void test_table_options(void **state);
void test_string_buffer(void **state);

struct test_case
{
    char name [128];
    void (*test)(void **);
};

#define assert_true(args) assert(args)

#endif // TESTS_H
