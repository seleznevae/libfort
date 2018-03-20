#ifndef TESTS_H
#define TESTS_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "fort_impl.h"
#include <assert.h>

#define WHEN(...)
#define THEN(...)
#define SCENARIO(...)

/* Test cases */
void test_vector_basic(void);
void test_string_buffer(void);
void test_table_sizes(void);
void test_table_geometry(void);
void test_table_basic(void);
void test_table_write(void);
void test_table_border_style(void);
void test_table_options(void);

struct test_case
{
    char name [128];
    void (*test)(void);
};


/*
 *  Test utility funcitons
 */

#define assert_true(args) assert(args)

int set_test_options_for_table(FTABLE *table);
int set_test_options_as_default();
FTABLE *create_test_int_table(int set_test_opts);


#endif // TESTS_H
