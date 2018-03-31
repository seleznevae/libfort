#include "string_buffer.h"
#include "options.h"
#include "assert.h"
#include "wchar.h"
#include "wcwidth.h"
/*****************************************************************************
 *               STRING BUFFER
 * ***************************************************************************/


static int str_iter_width(const char *beg, const char *end)
{
    return end - beg;
}

static int wcs_iter_width(const wchar_t *beg, const wchar_t *end)
{
    return mk_wcswidth(beg, (end - beg));
}


static size_t buf_str_len(const string_buffer_t *buf)
{
    assert(buf);
    if (buf->type == CharBuf) {
        return strlen(buf->str.cstr);
    } else {
        return wcslen(buf->str.wstr);
    }
}

size_t strchr_count(const char *str, char ch)
{
    if (str == NULL)
        return 0;

    size_t count = 0;
    str = strchr(str, ch);
    while (str) {
        count++;
        str++;
        str = strchr(str, ch);
    }
    return count;
}

size_t wstrchr_count(const wchar_t *str, wchar_t ch)
{
    if (str == NULL)
        return 0;

    size_t count = 0;
    str = wcschr(str, ch);
    while (str) {
        count++;
        str++;
        str = wcschr(str, ch);
    }
    return count;
}

const char *str_n_substring_beg(const char *str, char ch_separator, size_t n)
{
    if (str == NULL)
        return NULL;

    if (n == 0)
        return str;

    str = strchr(str, ch_separator);
    --n;
    while (n > 0) {
        if (str == NULL)
            return NULL;
        --n;
        str++;
        str = strchr(str, ch_separator);
    }
    return str ? (str + 1) : NULL;
}

const wchar_t *wstr_n_substring_beg(const wchar_t *str, wchar_t ch_separator, size_t n)
{
    if (str == NULL)
        return NULL;

    if (n == 0)
        return str;

    str = wcschr(str, ch_separator);
    --n;
    while (n > 0) {
        if (str == NULL)
            return NULL;
        --n;
        str++;
        str = wcschr(str, ch_separator);
    }
    return str ? (str + 1) : NULL;
}

void str_n_substring(const char *str, char ch_separator, size_t n, const char **begin, const char **end)
{
    const char *beg = str_n_substring_beg(str, ch_separator, n);
    if (beg == NULL) {
        *begin = NULL;
        *end = NULL;
        return;
    }

    const char *en = strchr(beg, ch_separator);
    if (en == NULL) {
        en = str + strlen(str);
    }

    *begin = beg;
    *end = en;
    return;
}

void wstr_n_substring(const wchar_t *str, wchar_t ch_separator, size_t n, const wchar_t **begin, const wchar_t **end)
{
    const wchar_t *beg = wstr_n_substring_beg(str, ch_separator, n);
    if (beg == NULL) {
        *begin = NULL;
        *end = NULL;
        return;
    }

    const wchar_t *en = wcschr(beg, ch_separator);
    if (en == NULL) {
        en = str + wcslen(str);
    }

    *begin = beg;
    *end = en;
    return;
}


string_buffer_t *create_string_buffer(size_t number_of_chars, enum str_buf_type type)
{
    size_t sz = (number_of_chars) * (type == CharBuf ? sizeof(char) : sizeof(wchar_t));
    string_buffer_t *result = (string_buffer_t *)F_MALLOC(sizeof(string_buffer_t));
    if (result == NULL)
        return NULL;
    result->str.data = F_MALLOC(sz);
    if (result->str.data == NULL) {
        F_FREE(result);
        return NULL;
    }
    result->data_sz = sz;
    result->type = type;

    return result;
}

void destroy_string_buffer(string_buffer_t *buffer)
{
    if (buffer == NULL)
        return;
    F_FREE(buffer->str.data);
    buffer->str.data = NULL;
    F_FREE(buffer);
}

fort_status_t realloc_string_buffer_without_copy(string_buffer_t *buffer)
{
    assert(buffer);
    char *new_str = (char *)F_MALLOC(buffer->data_sz * 2);
    if (new_str == NULL) {
        return FT_MEMORY_ERROR;
    }
    F_FREE(buffer->str.data);
    buffer->str.data = new_str;
    buffer->data_sz *= 2;
    return FT_SUCCESS;
}

fort_status_t fill_buffer_from_string(string_buffer_t *buffer, const char *str)
{
    assert(buffer);
    assert(str);

    size_t sz = strlen(str);
    char *copy = F_STRDUP(str);
    if (copy == NULL)
        return FT_MEMORY_ERROR;

    while (sz >= string_buffer_capacity(buffer)) {
        int status = realloc_string_buffer_without_copy(buffer);
        if (!IS_SUCCESS(status)) {
            return status;
        }
    }
    F_FREE(buffer->str.data);
    buffer->str.cstr = copy;
    buffer->type = CharBuf;

    return FT_SUCCESS;
}

fort_status_t fill_buffer_from_wstring(string_buffer_t *buffer, const wchar_t *str)
{
    assert(buffer);
    assert(str);

    size_t sz = wcslen(str);
    wchar_t *copy = F_WCSDUP(str);
    if (copy == NULL)
        return FT_MEMORY_ERROR;

    while (sz >= string_buffer_capacity(buffer)) {
        int status = realloc_string_buffer_without_copy(buffer);
        if (!IS_SUCCESS(status)) {
            return status;
        }
    }
    F_FREE(buffer->str.data);
    buffer->str.wstr = copy;
    buffer->type = WCharBuf;

    return FT_SUCCESS;
}



size_t buffer_text_height(string_buffer_t *buffer)
{
    if (buffer == NULL || buffer->str.data == NULL || buf_str_len(buffer) == 0) {
        return 0;
    }
    if (buffer->type == CharBuf)
        return 1 + strchr_count(buffer->str.cstr, '\n');
    else
        return 1 + wstrchr_count(buffer->str.wstr, L'\n');
}

size_t buffer_text_width(string_buffer_t *buffer)
{
    size_t max_length = 0;
    int n = 0;
    if (buffer->type == CharBuf) {
        while (1) {
            const char *beg = NULL;
            const char *end = NULL;
            str_n_substring(buffer->str.cstr, '\n', n, &beg, &end);
            if (beg == NULL || end == NULL)
                return max_length;

            max_length = MAX(max_length, (size_t)(end - beg));
            ++n;
        }
    } else {
        while (1) {
            const wchar_t *beg = NULL;
            const wchar_t *end = NULL;
            wstr_n_substring(buffer->str.wstr, L'\n', n, &beg, &end);
            if (beg == NULL || end == NULL)
                return max_length;

            int line_width = mk_wcswidth(beg, end - beg);
            max_length = MAX(max_length, line_width);

            ++n;
        }
    }
}



int buffer_printf(string_buffer_t *buffer, size_t buffer_row, char *buf, size_t buf_len, const context_t *context)
{
#define CHAR_TYPE char
#define NULL_CHAR '\0'
#define NEWLINE_CHAR '\n'
#define SPACE_CHAR ' '
#define SNPRINTF_FMT_STR "%*s"
#define SNPRINTF snprintf
#define BUFFER_STR str.cstr
#define SNPRINT_N_CHARS  snprint_n_chars
#define STR_N_SUBSTRING str_n_substring
#define STR_ITER_WIDTH str_iter_width

    if (buffer == NULL || buffer->str.data == NULL
        || buffer_row >= buffer_text_height(buffer) || buf_len == 0) {
        return -1;
    }

    size_t content_width = buffer_text_width(buffer);
    if ((buf_len - 1) < content_width)
        return -1;

    int left = 0;
    int right = 0;

    switch (get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_TEXT_ALIGN)) {
        case LeftAligned:
            left = 0;
            right = (buf_len - 1) - content_width;
            break;
        case CenterAligned:
            left = ((buf_len - 1) - content_width) / 2;
            right = ((buf_len - 1) - content_width) - left;
            break;
        case RightAligned:
            left = (buf_len - 1) - content_width;
            right = 0;
            break;
        default:
            assert(0);
            break;
    }
    if (left < 0 || right < 0)
        return -1;


    int  written = 0;
    written += SNPRINT_N_CHARS(buf + written, buf_len - written, left, SPACE_CHAR);
    if (written < 0)
        return written;

    const CHAR_TYPE *beg = NULL;
    const CHAR_TYPE *end = NULL;
    STR_N_SUBSTRING(buffer->BUFFER_STR, NEWLINE_CHAR, buffer_row, &beg, &end);
    if (beg == NULL || end == NULL)
        return -1;
    CHAR_TYPE old_value = *end;
    *(CHAR_TYPE *)end = NULL_CHAR;

    written += SNPRINTF(buf + written, buf_len - written, SNPRINTF_FMT_STR, (int)(end - beg), beg);
    *(CHAR_TYPE *)end = old_value;
    if (written < 0)
        return written;
    written += SNPRINT_N_CHARS(buf + written,  buf_len - written, (int)(content_width - STR_ITER_WIDTH(beg, end)), SPACE_CHAR);
    if (written < 0)
        return written;


    written += SNPRINT_N_CHARS(buf + written, buf_len - written, right, SPACE_CHAR);
    return written;

#undef CHAR_TYPE
#undef NULL_CHAR
#undef NEWLINE_CHAR
#undef SPACE_CHAR
#undef SNPRINTF_FMT_STR
#undef SNPRINTF
#undef BUFFER_STR
#undef SNPRINT_N_CHARS
#undef STR_N_SUBSTRING
#undef STR_ITER_WIDTH
}


int buffer_wprintf(string_buffer_t *buffer, size_t buffer_row, wchar_t *buf, size_t buf_len, const context_t *context)
{
#define CHAR_TYPE wchar_t
#define NULL_CHAR L'\0'
#define NEWLINE_CHAR L'\n'
#define SPACE_CHAR L' '
#define SNPRINTF_FMT_STR L"%*ls"
#define SNPRINTF swprintf
#define BUFFER_STR str.wstr
#define SNPRINT_N_CHARS  wsnprint_n_chars
#define STR_N_SUBSTRING wstr_n_substring
#define STR_ITER_WIDTH wcs_iter_width

    if (buffer == NULL || buffer->str.data == NULL
        || buffer_row >= buffer_text_height(buffer) || buf_len == 0) {
        return -1;
    }

    size_t content_width = buffer_text_width(buffer);
    if ((buf_len - 1) < content_width)
        return -1;

    int left = 0;
    int right = 0;

    switch (get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_TEXT_ALIGN)) {
        case LeftAligned:
            left = 0;
            right = (buf_len - 1) - content_width;
            break;
        case CenterAligned:
            left = ((buf_len - 1) - content_width) / 2;
            right = ((buf_len - 1) - content_width) - left;
            break;
        case RightAligned:
            left = (buf_len - 1) - content_width;
            right = 0;
            break;
        default:
            assert(0);
            break;
    }
    if (left < 0 || right < 0)
        return -1;

    int  written = 0;
    written += SNPRINT_N_CHARS(buf + written, buf_len - written, left, SPACE_CHAR);
    if (written < 0)
        return written;

    const CHAR_TYPE *beg = NULL;
    const CHAR_TYPE *end = NULL;
    STR_N_SUBSTRING(buffer->BUFFER_STR, NEWLINE_CHAR, buffer_row, &beg, &end);
    if (beg == NULL || end == NULL)
        return -1;
    CHAR_TYPE old_value = *end;
    *(CHAR_TYPE *)end = NULL_CHAR;

    written += SNPRINTF(buf + written, buf_len - written, SNPRINTF_FMT_STR, (int)(end - beg), beg);
    *(CHAR_TYPE *)end = old_value;
    if (written < 0)
        return written;
    written += SNPRINT_N_CHARS(buf + written,  buf_len - written, (int)(content_width - STR_ITER_WIDTH(beg, end)), SPACE_CHAR);
    if (written < 0)
        return written;

    written += SNPRINT_N_CHARS(buf + written, buf_len - written, right, SPACE_CHAR);
    return written;

#undef CHAR_TYPE
#undef NULL_CHAR
#undef NEWLINE_CHAR
#undef SPACE_CHAR
#undef SNPRINTF_FMT_STR
#undef SNPRINTF
#undef BUFFER_STR
#undef SNPRINT_N_CHARS
#undef STR_N_SUBSTRING
#undef STR_ITER_WIDTH
}

size_t string_buffer_capacity(const string_buffer_t *buffer)
{
    assert(buffer);
    if (buffer->type == CharBuf)
        return buffer->data_sz;
    else
        return buffer->data_sz / sizeof(wchar_t);
}

void *buffer_get_data(string_buffer_t *buffer)
{
    assert(buffer);
    return buffer->str.data;
}

