#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include "fort_impl.h"


/*****************************************************************************
 *               STRING BUFFER
 * ***************************************************************************/
struct string_buffer;
typedef struct string_buffer string_buffer_t;
struct string_buffer
{
    char *str;
    size_t str_sz;
};

string_buffer_t* create_string_buffer(size_t sz);
void destroy_string_buffer(string_buffer_t *buffer);
fort_status_t realloc_string_buffer_without_copy(string_buffer_t *buffer);
fort_status_t fill_buffer_from_string(string_buffer_t *buffer, const char *str);

size_t buffer_text_height(string_buffer_t *buffer);


size_t buffer_text_width(string_buffer_t *buffer);
int buffer_printf(string_buffer_t *buffer, size_t buffer_row, size_t table_column, char *buf, size_t buf_len, const context_t *context);


#endif // STRING_BUFFER_H
