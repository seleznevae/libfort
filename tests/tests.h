#ifndef TESTS_H
#define TESTS_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "fort_impl.h"
#include <assert.h>
#include "wchar.h"
#include "locale.h"

#define WHEN(...)
#define THEN(...)
#define SCENARIO(...)

/* Test cases */
void test_vector_basic(void);
void test_string_buffer(void);
void test_table_sizes(void);
void test_table_geometry(void);
void test_table_basic(void);
void test_wcs_table_boundaries(void);
void test_table_write(void);
void test_table_border_style(void);
void test_table_cell_options(void);
void test_table_tbl_options(void);
void test_memory_errors(void);

struct test_case
{
    char name [128];
    void (*test)(void);
};


/*
 *  Test utility funcitons
 */

#define assert_true(args) assert(args)

#define assert_str_equal(str1, str2) \
    if (strcmp(str1, str2) != 0) \
    { \
        fprintf(stderr, "%s:%d(%s):Abort! Not equals strings:\n",__FILE__,__LINE__, __FUNCTION__); \
        fprintf(stderr, "Left string:\n%s\n", str1); \
        fprintf(stderr, "Right string:\n%s\n", str2); \
        exit(EXIT_FAILURE); \
    }

#define assert_wcs_equal(str1, str2) \
    if (wcscmp(str1, str2) != 0) \
    { \
        fprintf(stderr, "%s:%d(%s):Abort! Not equals strings:\n",__FILE__,__LINE__, __FUNCTION__); \
        setlocale(LC_CTYPE, ""); \
        fwprintf(stdout, L"Left string:\n%ls\n", str1); \
        fwprintf(stdout, L"Right string:\n%ls\n", str2); \
        fflush(stdout); \
        exit(EXIT_FAILURE); \
    }

int set_test_options_for_table(FTABLE *table);
int set_test_options_as_default();
FTABLE *create_test_int_table(int set_test_opts);
FTABLE *create_test_int_wtable(int set_test_opts);







#endif // TESTS_H
