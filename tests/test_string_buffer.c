#include "tests.h"

#include "string_buffer.h"
//#include "../src/fort.c"

size_t strchr_count(const char* str, int ch);
const char* str_n_substring_beg(const char* str, int ch, int n);
fort_status_t str_n_substring(const char* str, char ch_separator, size_t n, const char **begin, const char **end);
size_t buffer_text_width(string_buffer_t *buffer);


void test_strchr_count(void);
void test_str_n_substring(void);
void test_buffer_text_width(void);
void test_buffer_text_height(void);


void test_string_buffer(void **state)
{
    (void)state;

    test_strchr_count();
    test_str_n_substring();
    test_buffer_text_width();
    test_buffer_text_height();
}


void test_strchr_count(void)
{
    assert_true(strchr_count(NULL, '\n') == 0);
    assert_true(strchr_count("", '\n') == 0);
    assert_true(strchr_count("asbd", '\n') == 0);

    assert_true(strchr_count("asbd\n", '\n') == 1);
    assert_true(strchr_count("\nasbd", '\n') == 1);
    assert_true(strchr_count("a\nsbd", '\n') == 1);

    assert_true(strchr_count("\n12\n123", '\n') == 2);
    assert_true(strchr_count("\n12\n123\n", '\n') == 3);
    assert_true(strchr_count("\n\n\n", '\n') == 3);
    assert_true(strchr_count("\n123123\n123123\n\n\n123", '\n') == 5);

    assert_true(strchr_count("1a23123a123123aaa123", 'a') == 5);
}


void test_str_n_substring(void)
{
    const char *empty_str = "";
    assert_true(str_n_substring_beg(empty_str, '\n', 0) == empty_str);
    assert_true(str_n_substring_beg(empty_str, '\n', 1) == NULL);
    assert_true(str_n_substring_beg(empty_str, '\n', 2) == NULL);

    const char *str = "123\n5678\n9";
    assert_true(str_n_substring_beg(NULL, '\n', 0) == NULL);
    assert_true(str_n_substring_beg(str, '\n', 0) == str);
    assert_true(str_n_substring_beg(str, '1', 0) == str);

    assert_true(str_n_substring_beg(str, '\n', 1) == str + 4);
    assert_true(str_n_substring_beg(str, '\n', 2) == str + 9);
    assert_true(str_n_substring_beg(str, '\n', 3) == NULL);

    const char *str2 = "\n123\n56\n\n9\n";
    assert_true(str_n_substring_beg(str2, '\n', 0) == str2);
    assert_true(str_n_substring_beg(str2, '\n', 1) == str2 + 1);
    assert_true(str_n_substring_beg(str2, '\n', 2) == str2 + 5);
    assert_true(str_n_substring_beg(str2, '\n', 3) == str2 + 8);
    assert_true(str_n_substring_beg(str2, '\n', 4) == str2 + 9);
    assert_true(str_n_substring_beg(str2, '\n', 5) == str2 + 11);
    assert_true(str_n_substring_beg(str2, '\n', 6) == NULL);

    const char *beg = NULL;
    const char *end = NULL;
    str_n_substring(empty_str, '\n', 0, &beg, &end);
    assert_true(beg == empty_str && end == empty_str + strlen(empty_str));
    str_n_substring(empty_str, '\n', 1, &beg, &end);
    assert_true(beg == NULL && end == NULL);
    str_n_substring(empty_str, '\n', 2, &beg, &end);
    assert_true(beg == NULL && end == NULL);

    str_n_substring(NULL, '\n', 0, &beg, &end);
    assert_true(beg == NULL && end == NULL);
    str_n_substring(str, '\n', 0, &beg, &end);
    assert_true(beg == str && end == str + 3);
    str_n_substring(str, '2', 0, &beg, &end);
    assert_true(beg == str && end == str + 1);

    str_n_substring(str, '\n', 1, &beg, &end);
    assert_true(beg == str +4 && end == str + 8);
    str_n_substring(str, '\n', 2, &beg, &end);
    assert_true(beg == str + 9 && end == str + strlen(str));
    str_n_substring(str, '\n', 3, &beg, &end);
    assert_true(beg == NULL && end == NULL);


    str_n_substring(str2, '\n', 0, &beg, &end);
    assert_true(beg == str2 && end == str2);
    str_n_substring(str2, '\n', 1, &beg, &end);
    assert_true(beg == str2 + 1 && end == str2 + 4);
    str_n_substring(str2, '\n', 2, &beg, &end);
    assert_true(beg == str2 + 5 && end == str2 + 7);
    str_n_substring(str2, '\n', 3, &beg, &end);
    assert_true(beg == str2 + 8 && end == str2 + 8);
    str_n_substring(str2, '\n', 4, &beg, &end);
    assert_true(beg == str2 + 9 && end == str2 + 10);
    str_n_substring(str2, '\n', 5, &beg, &end);
    assert_true(beg == str2 + 11 && end == str2 + 11);
    str_n_substring(str2, '\n', 6, &beg, &end);
    assert_true(beg == NULL && end == NULL);
}

void test_buffer_text_width(void)
{
    string_buffer_t *buffer = create_string_buffer(200);
    char *old_value = buffer->str;

    buffer->str = "";
    assert_true(buffer_text_width(buffer) == 0);

    buffer->str = "\n\n\n\n";
    assert_true(buffer_text_width(buffer) == 0);

    buffer->str = "12345";
    assert_true(buffer_text_width(buffer) == 5);

    buffer->str = "12345\n1234567";
    assert_true(buffer_text_width(buffer) == 7);

    buffer->str = "12345\n1234567\n";
    assert_true(buffer_text_width(buffer) == 7);

    buffer->str = "12345\n1234567\n123";
    assert_true(buffer_text_width(buffer) == 7);

    buffer->str = old_value;
    destroy_string_buffer(buffer);
}


void test_buffer_text_height(void)
{
    string_buffer_t *buffer = create_string_buffer(200);
    char *old_value = buffer->str;

    buffer->str = "";
    assert_true(buffer_text_height(buffer) == 0);

    buffer->str = "\n";
    assert_true(buffer_text_height(buffer) == 2);

    buffer->str = "\n\n";
    assert_true(buffer_text_height(buffer) == 3);

    buffer->str = "\n\n\n\n";
    assert_true(buffer_text_height(buffer) == 5);

    buffer->str = "12345";
    assert_true(buffer_text_height(buffer) == 1);

    buffer->str = "\n12345";
    assert_true(buffer_text_height(buffer) == 2);

    buffer->str = "\n12345\n\n2";
    assert_true(buffer_text_height(buffer) == 4);

    buffer->str = old_value;
    destroy_string_buffer(buffer);
}
