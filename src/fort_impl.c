#include "fort_impl.h"
#include <wchar.h>



/*****************************************************************************
 *               LIBFORT helpers
 *****************************************************************************/
char *fort_strdup(const char* str)
{
    if (str == NULL)
        return NULL;

    size_t sz = strlen(str);
    char *str_copy = (char*)F_MALLOC((sz + 1)*sizeof(char));
    if (str_copy == NULL)
        return NULL;

    strcpy(str_copy, str);
    return str_copy;
}

wchar_t *fort_wcsdup(const wchar_t* str)
{
    if (str == NULL)
        return NULL;

    size_t sz = wcslen(str);
    wchar_t *str_copy = (wchar_t*)F_MALLOC((sz + 1)*sizeof(wchar_t));
    if (str_copy == NULL)
        return NULL;

    wcscpy(str_copy, str);
    return str_copy;
}

size_t number_of_columns_in_format_string(const char *fmt)
{
    int separator_counter = 0;
    const char *pos = fmt;
    while (1) {
        pos = strchr(pos, FORT_COL_SEPARATOR);
        if (pos == NULL)
            break;

        separator_counter++;
        ++pos;
    }
    return separator_counter + 1;
}

int snprint_n_chars(char *buf, size_t length, size_t n, char ch)
{
    if (length <= n)
        return -1;

    if (n == 0)
        return 0;

    int status = snprintf(buf, length, "%0*d", (int)n, 0);
    if (status < 0)
        return status;

    size_t i = 0;
    for (i = 0; i < n; ++i) {
        *buf = ch;
        buf++;
    }
    return n;
}


int wsnprint_n_chars(wchar_t *buf, size_t length, size_t n, wchar_t ch)
{
    if (length <= n)
        return -1;

    if (n == 0)
        return 0;

    int status = swprintf(buf, length, L"%0*d", (int)n, 0);
    if (status < 0)
        return status;

    size_t i = 0;
    for (i = 0; i < n; ++i) {
        *buf = ch;
        buf++;
    }
    return n;
}
