#ifndef TESTS_H
#define TESTS_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#define WHEN(...)
#define THEN(...)


void test_vector_basic(void **state);

void test_table_sizes(void **state);
void test_table_geometry(void **state);
void test_table_basic(void **state);


#endif // TESTS_H
