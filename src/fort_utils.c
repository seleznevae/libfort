#include "fort_utils.h"
#ifdef FT_HAVE_WCHAR
#include <wchar.h>
#endif



/*****************************************************************************
 *               LIBFORT helpers
 *****************************************************************************/

#ifndef FT_MICROSOFT_COMPILER
void *(*fort_malloc)(size_t size) = &malloc;
void (*fort_free)(void *ptr) = &free;
void *(*fort_calloc)(size_t nmemb, size_t size) = &calloc;
void *(*fort_realloc)(void *ptr, size_t size) = &realloc;
#else
static void *local_malloc(size_t size)
{
    return malloc(size);
}

static void local_free(void *ptr)
{
    free(ptr);
}

static void *local_calloc(size_t nmemb, size_t size)
{
    return calloc(nmemb, size);
}

static void *local_realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}

void *(*fort_malloc)(size_t size) = &local_malloc;
void (*fort_free)(void *ptr) = &local_free;
void *(*fort_calloc)(size_t nmemb, size_t size) = &local_calloc;
void *(*fort_realloc)(void *ptr, size_t size) = &local_realloc;
#endif

static void *custom_fort_calloc(size_t nmemb, size_t size)
{
    size_t total_size = nmemb * size;
    void *result = F_MALLOC(total_size);
    if (result != NULL)
        memset(result, 0, total_size);
    return result;
}

static void *custom_fort_realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return F_MALLOC(size);
    if (size == 0) {
        F_FREE(ptr);
        return NULL;
    }

    void *new_chunk = F_MALLOC(size);
    if (new_chunk == NULL)
        return NULL;

    /*
     * In theory we should copy MIN(size, size allocated for ptr) bytes,
     * but this is rather dummy implementation so we don't care about it
     */
    memcpy(new_chunk, ptr, size);
    F_FREE(ptr);
    return new_chunk;
}


FT_INTERNAL
void set_memory_funcs(void *(*f_malloc)(size_t size), void (*f_free)(void *ptr))
{
    assert((f_malloc == NULL && f_free == NULL) /* Use std functions */
           || (f_malloc != NULL && f_free != NULL) /* Use custom functions */);

    if (f_malloc == NULL && f_free == NULL) {
#ifndef FT_MICROSOFT_COMPILER
        fort_malloc = &malloc;
        fort_free = &free;
        fort_calloc = &calloc;
        fort_realloc = &realloc;
#else
        fort_malloc = &local_malloc;
        fort_free = &local_free;
        fort_calloc = &local_calloc;
        fort_realloc = &local_realloc;
#endif
    } else {
        fort_malloc = f_malloc;
        fort_free = f_free;
        fort_calloc = &custom_fort_calloc;
        fort_realloc = &custom_fort_realloc;
    }

}

FT_INTERNAL
char *fort_strdup(const char *str)
{
    if (str == NULL)
        return NULL;

    size_t sz = strlen(str);
    char *str_copy = (char *)F_MALLOC((sz + 1) * sizeof(char));
    if (str_copy == NULL)
        return NULL;

    strcpy(str_copy, str);
    return str_copy;
}

#if defined(FT_HAVE_WCHAR)
FT_INTERNAL
wchar_t *fort_wcsdup(const wchar_t *str)
{
    if (str == NULL)
        return NULL;

    size_t sz = wcslen(str);
    wchar_t *str_copy = (wchar_t *)F_MALLOC((sz + 1) * sizeof(wchar_t));
    if (str_copy == NULL)
        return NULL;

    wcscpy(str_copy, str);
    return str_copy;
}
#endif


FT_INTERNAL
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


#if defined(FT_HAVE_WCHAR)
FT_INTERNAL
size_t number_of_columns_in_format_wstring(const wchar_t *fmt)
{
    int separator_counter = 0;
    const wchar_t *pos = fmt;
    while (1) {
        pos = wcschr(pos, FORT_COL_SEPARATOR);
        if (pos == NULL)
            break;

        separator_counter++;
        ++pos;
    }
    return separator_counter + 1;
}
#endif


FT_INTERNAL
int snprint_n_strings(char *buf, size_t length, size_t n, const char *str)
{
    size_t str_len = strlen(str);
    if (length <= n * str_len)
        return -1;

    if (n == 0)
        return 0;

    /* To ensure valid return value it is safely not print such big strings */
    if (n * str_len > INT_MAX)
        return -1;

    if (str_len == 0)
        return 0;

    int status = snprintf(buf, length, "%0*d", (int)(n * str_len), 0);
    if (status < 0)
        return status;

    size_t i = 0;
    for (i = 0; i < n; ++i) {
        const char *str_p = str;
        while (*str_p)
            *(buf++) = *(str_p++);
    }
    return (int)(n * str_len);
}



#if defined(FT_HAVE_WCHAR)
#define WCS_SIZE 64

FT_INTERNAL
int wsnprint_n_string(wchar_t *buf, size_t length, size_t n, const char *str)
{
    size_t str_len = strlen(str);

    /* note: baybe it's, better to return -1 in case of multibyte character
     * strings (not sure this case is done correctly).
     */
    if (str_len > 1) {
        const unsigned char *p = (const unsigned char *)str;
        while (*p) {
            if (*p <= 127)
                p++;
            else {
                wchar_t wcs[WCS_SIZE];
                const char *ptr = str;
                size_t wcs_len;
                mbstate_t mbst;
                memset(&mbst, 0, sizeof(mbst));
                wcs_len = mbsrtowcs(wcs, (const char **)&ptr, WCS_SIZE, &mbst);
                /* for simplicity */
                if ((wcs_len == (size_t) - 1) || wcs_len > 1) {
                    return -1;
                } else {
                    wcs[wcs_len] = L'\0';
                    size_t k = n;
                    while (k) {
                        *buf = *wcs;
                        ++buf;
                        --k;
                    }
                    buf[n] = L'\0';
                    return (int)n;
                }
            }
        }
    }

    if (length <= n * str_len)
        return -1;

    if (n == 0)
        return 0;

    /* To ensure valid return value it is safely not print such big strings */
    if (n * str_len > INT_MAX)
        return -1;

    if (str_len == 0)
        return 0;

    int status = swprintf(buf, length, L"%0*d", (int)(n * str_len), 0);
    if (status < 0)
        return status;

    size_t i = 0;
    for (i = 0; i < n; ++i) {
        const char *str_p = str;
        while (*str_p)
            *(buf++) = (wchar_t) * (str_p++);
    }
    return (int)(n * str_len);
}
#endif
