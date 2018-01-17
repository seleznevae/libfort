#include "string_buffer.h"
#include "options.h"
#include "assert.h"

/*****************************************************************************
 *               STRING BUFFER
 * ***************************************************************************/

string_buffer_t* create_string_buffer(size_t sz)
{
    string_buffer_t *result = (string_buffer_t *)F_MALLOC(sizeof(string_buffer_t));
    if (result == NULL)
        return NULL;
    result->str = F_MALLOC(sz);
    if (result->str == NULL) {
        F_FREE(result);
        return NULL;
    }
    result->str_sz = sz;

    return result;
}

void destroy_string_buffer(string_buffer_t *buffer)
{
    if (buffer == NULL)
        return;
    F_FREE(buffer->str);
    buffer->str = NULL;
    F_FREE(buffer);
}

fort_status_t realloc_string_buffer_without_copy(string_buffer_t *buffer)
{
    assert(buffer);
    char *new_str = (char*)F_MALLOC(buffer->str_sz * 2);
    if (new_str == NULL) {
        return F_MEMORY_ERROR;
    }
    F_FREE(buffer->str);
    buffer->str = new_str;
    buffer->str_sz *= 2;
    return F_SUCCESS;
}

fort_status_t fill_buffer_from_string(string_buffer_t *buffer, const char *str)
{
    assert(buffer);
    assert(str);

    size_t sz = strlen(str);
    char * copy = F_STRDUP(str);
    if (copy == NULL)
        return F_MEMORY_ERROR;

    while (sz >= buffer->str_sz) {
        int status = realloc_string_buffer_without_copy(buffer);
        if (!IS_SUCCESS(status)) {
            return status;
        }
    }
    F_FREE(buffer->str);
    buffer->str = copy;

    return F_SUCCESS;
}



size_t buffer_text_height(string_buffer_t *buffer)
{
    if (buffer == NULL || buffer->str == NULL || strlen(buffer->str) == 0) {
        return 0;
    }
    return 1;
}

size_t buffer_text_width(string_buffer_t *buffer)
{
    if (buffer == NULL || buffer->str == NULL) {
        return 0;
    }
    return strlen(buffer->str);
}


int buffer_printf(string_buffer_t *buffer, size_t buffer_row, size_t table_column, char *buf, size_t buf_len, const context_t *context)
{
    if (buffer == NULL || buffer->str == NULL
            || buffer_row >= buffer_text_height(buffer) || buf_len == 0) {
        return -1;
    }

    size_t content_width = buffer_text_width(buffer);
    if ((buf_len - 1) < content_width)
        return -1;

    int left = 0;
    int right = 0;

    switch (fort_options_column_alignment(context, table_column)) {
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
    written += snprint_n_chars(buf + written, buf_len - written, left, ' ');
    if (written < 0)
        return written;

    written += snprintf(buf + written, buf_len - written, "%*s", (int)content_width, buffer->str);
    if (written < 0)
        return written;

    written += snprint_n_chars(buf + written, buf_len - written, right, ' ');
    return written;
}

