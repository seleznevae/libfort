#ifndef TESTS_H
#define TESTS_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <assert.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>

#define WHEN(...)
#define THEN(...)
#define SCENARIO(...)

#if defined(FT_CONGIG_HAVE_WCHAR)
#define FT_HAVE_WCHAR
#endif

/* Test cases */
void test_vector_basic(void);
void test_vector_stress(void);
void test_string_buffer(void);
void test_table_sizes(void);
void test_table_geometry(void);
void test_table_basic(void);
void test_table_copy(void);
#ifdef FT_HAVE_WCHAR
void test_wcs_table_boundaries(void);
#endif
void test_table_write(void);
void test_table_border_style(void);
void test_table_builtin_border_styles(void);
void test_table_cell_properties(void);
void test_table_text_styles(void);
void test_table_tbl_properties(void);
void test_memory_errors(void);

struct test_case {
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
        fprintf(stderr, "%s:%d(%s):Abort! Not equals strings:\n",__FILE__,__LINE__, __func__); \
        fprintf(stderr, "Left string(len = %d):\n%s\n", (int)strlen(str1), str1); \
        fprintf(stderr, "Right string(len = %d):\n%s\n", (int)strlen(str2), str2); \
        exit(EXIT_FAILURE); \
    }

#define assert_wcs_equal(str1, str2) \
    if (wcscmp(str1, str2) != 0) \
    { \
        fprintf(stderr, "%s:%d(%s):Abort! Not equals strings:\n",__FILE__,__LINE__, __func__); \
        setlocale(LC_CTYPE, ""); \
        fwprintf(stdout, L"Left string(len = %d):\n%ls\n", (int)wcslen(str1), str1); \
        fwprintf(stdout, L"Right string(len = %d):\n%ls\n", (int)wcslen(str2), str2); \
        fflush(stdout); \
        exit(EXIT_FAILURE); \
    }

struct ft_table;

#ifdef __cplusplus
extern "C" {
#endif

int set_test_props_for_table(struct ft_table *table);
int set_test_properties_as_default(void);
struct ft_table *create_test_int_table(int set_test_opts);
struct ft_table *create_test_int_wtable(int set_test_opts);
void run_test_suit(const char *test_suit_name, int n_tests, struct test_case test_suit[]);

#ifdef __cplusplus
}
#endif

#endif // TESTS_H
