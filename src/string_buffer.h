#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include "fort_impl.h"


/*****************************************************************************
 *               STRING BUFFER
 * ***************************************************************************/
enum str_buf_type
{
    CharBuf,
    WCharBuf
};

struct string_buffer;
typedef struct string_buffer string_buffer_t;
struct string_buffer
{
    union {
        char *cstr;
        wchar_t *wstr;
        void *data;
    };
    size_t data_sz;
    enum str_buf_type type;
};

string_buffer_t* create_string_buffer(size_t number_of_chars, enum str_buf_type type);
void destroy_string_buffer(string_buffer_t *buffer);
fort_status_t realloc_string_buffer_without_copy(string_buffer_t *buffer);

fort_status_t fill_buffer_from_string(string_buffer_t *buffer, const char *str);
fort_status_t fill_buffer_from_wstring(string_buffer_t *buffer, const wchar_t *str);


size_t buffer_text_height(string_buffer_t *buffer);
size_t string_buffer_capacity(const string_buffer_t *buffer);
void *buffer_get_data(string_buffer_t *buffer);


size_t buffer_text_width(string_buffer_t *buffer);
int buffer_printf(string_buffer_t *buffer, size_t buffer_row, size_t table_column, char *buf, size_t buf_len, const context_t *context);
int buffer_wprintf(string_buffer_t *buffer, size_t buffer_row, size_t table_column, wchar_t *buf, size_t buf_len, const context_t *context);



#endif // STRING_BUFFER_H
