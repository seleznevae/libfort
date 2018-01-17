#include "fort_impl.h"



/*****************************************************************************
 *               LIBFORT helpers
 *****************************************************************************/
char *fort_strdup(const char* str)
{
    if (str == NULL)
        return NULL;

    size_t sz = strlen(str);
    char *str_copy = (char*)F_MALLOC(sz + 1);
    if (str_copy == NULL)
        return NULL;

    strcpy(str_copy, str);
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

    int status = snprintf(buf, length, "%0*d", (int)n, 0);
    if (status < 0)
        return status;

    for (size_t i = 0; i < n; ++i) {
        *buf = ch;
        buf++;
    }
    return n;
}
