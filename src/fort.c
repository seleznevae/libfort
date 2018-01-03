/*
libfort

MIT License

Copyright (c) 2017 - 2018 Seleznev Anton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "stdlib.h"
#include "stdarg.h"
#include "stdio.h"
#include "fort.h"
#include "assert.h"
#include "string.h"
#include "wchar.h"

#define FORT_COL_SEPARATOR '|'
#define FORT_HOR_CELL_SEPARATOR (g_table_options.hor_separator)
#define FORT_VER_CELL_SEPARATOR (g_table_options.ver_separator)
#define FORT_UNUSED  __attribute__((unused))

#define F_CALLOC calloc
#define F_MALLOC malloc
#define F_REALLOC realloc
#define F_FREE free
#define F_STRDUP fort_strdup

#define F_CREATE(type) ((type *)F_CALLOC(sizeof(type), 1))

#define MAX(a,b) ((a) > (b) ? (a) : b)
#define MIN(a,b) ((a) < (b) ? (a) : b)



#define STR_2_CAT_(arg1, arg2) \
    arg1##arg2
#define STR_2_CAT(arg1, arg2) \
    STR_2_CAT_(arg1, arg2)

#define UNIQUE_NAME_(prefix) \
    STR_2_CAT(prefix,__COUNTER__)
#define UNIQUE_NAME(prefix) \
    UNIQUE_NAME_(prefix)


/*****************************************************************************
 *               LOGGER
 *****************************************************************************/
#define SYS_LOG_ERROR(...)


/*****************************************************************************
 *               RETURN CODES
 * ***************************************************************************/
typedef int fort_status_t;
#define F_SUCCESS        0
#define F_MEMORY_ERROR   1
#define F_ERROR          2
#define IS_SUCCESS(arg) ((arg) == F_SUCCESS)
#define IS_ERROR(arg) (!IS_SUCCESS(arg))

/*****************************************************************************
 *               UTILITIES
 * ***************************************************************************/
static int snprint_n_chars(char *buf, size_t length, size_t n, char ch)
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

static string_buffer_t* create_string_buffer(size_t sz);
static void destroy_string_buffer(string_buffer_t *buffer);
static fort_status_t realloc_string_buffer_without_copy(string_buffer_t *buffer);
static fort_status_t fill_buffer_from_string(string_buffer_t *buffer, const char *str);

static size_t buffer_text_height(string_buffer_t *buffer)
{
    if (buffer == NULL || buffer->str == NULL || strlen(buffer->str) == 0) {
        return 0;
    }
    return 1;
}

static size_t buffer_text_width(string_buffer_t *buffer)
{
    if (buffer == NULL || buffer->str == NULL) {
        return 0;
    }
    return strlen(buffer->str);
}

static int buffer_printf(string_buffer_t *buffer, size_t row, char *buf, size_t buf_len)
{
    if (buffer == NULL || buffer->str == NULL
            || row >= buffer_text_height(buffer) || buf_len == 0) {
        return -1;
    }

    return snprintf(buf, buf_len, "%*s", (int)(buf_len - 1), buffer->str);
}


/*****************************************************************************
 *               VECTOR
 * ***************************************************************************/

struct vector;
typedef struct vector vector_t;

#define INVALID_VEC_INDEX ((size_t) -1)

static vector_t* create_vector(size_t item_size, size_t capacity);
static void destroy_vector(vector_t*);

static size_t vector_size(const vector_t*);
static size_t vector_capacity(const vector_t*);
static size_t vector_index_of(const vector_t*, const void *item);

static int vector_push(vector_t*, const void *item);
static int vector_erase(vector_t*, size_t index);
static void vector_clear(vector_t*);
static void* vector_at(vector_t*, size_t index);


/*****************************************************************************
 *               CELL
 * ***************************************************************************/
//struct cell_options
//{
//    int padding_top;
//    int padding_bottom;
//    int padding_left;
//    int padding_right;
//};
//typedef struct cell_options cell_options_t;
static fort_table_options_t g_table_options = {1, 1, 1, 1, 1, '=', '|'};

static void init_cell_options(fort_table_options_t *options)
{
    assert(options);
    options->cell_padding_top = g_table_options.cell_padding_top;
    options->cell_padding_bottom = g_table_options.cell_padding_bottom;
    options->cell_padding_left = g_table_options.cell_padding_left;
    options->cell_padding_right = g_table_options.cell_padding_right;
    options->cell_empty_string_height = g_table_options.cell_empty_string_height;
}

struct fort_cell;
typedef struct fort_cell fort_cell_t;
struct fort_cell
{
    string_buffer_t *str_buffer;
    fort_table_options_t options;
};

static fort_cell_t * create_cell()
{
    fort_cell_t *cell = F_CALLOC(sizeof(fort_cell_t), 1);
    if (cell == NULL)
        return NULL;
    cell->str_buffer = create_string_buffer(1024);
    if (cell->str_buffer == NULL) {
        F_FREE(cell);
        return NULL;
    }
    init_cell_options(&(cell->options));
    return cell;
}

static void destroy_cell(fort_cell_t *cell)
{
    if (cell == NULL)
        return;
    destroy_string_buffer(cell->str_buffer);
    F_FREE(cell);
}

static int hint_width_cell(const fort_cell_t *cell)
{
    assert(cell);
    int result = cell->options.cell_padding_left + cell->options.cell_padding_right;
    if (cell->str_buffer && cell->str_buffer->str) {
        result += buffer_text_width(cell->str_buffer);
    }
    return result;
}

static int hint_height_cell(const fort_cell_t *cell)
{
    assert(cell);
    int result = cell->options.cell_padding_top + cell->options.cell_padding_bottom;
    if (cell->str_buffer && cell->str_buffer->str) {
        size_t text_height = buffer_text_height(cell->str_buffer);
        result += text_height == 0 ? cell->options.cell_empty_string_height : text_height;
    }
    return result;
}


/*
 * Returns number of lines in cell. If cell is empty or
 * contains empty string, then 0 is returned.
 */
static int lines_number_cell(fort_cell_t *cell)
{
    assert(cell);
    if (cell->str_buffer == NULL || cell->str_buffer->str == NULL || cell->str_buffer->str[0] == '\0') {
        return 0;
    }

    int result = 0;
    char *pos = cell->str_buffer->str;
    while ((pos = strchr(pos, '\n')) != NULL) {
        result++;
        pos++;
    }
    return result + 1;
}

static int cell_printf(fort_cell_t *cell, size_t row, char *buf, size_t buf_len)
{
    if (cell == NULL || buf_len == 0 || row >= hint_height_cell(cell)
            || (buf_len <= hint_width_cell(cell))) {
        return -1;
    }

    if (row < cell->options.cell_padding_top
            || row >= (cell->options.cell_padding_top + buffer_text_height(cell->str_buffer))) {
        int k = snprint_n_chars(buf, buf_len, buf_len - 1, ' ');
        return k;
    } else {
        int written = 0;
        int left = cell->options.cell_padding_left;
        int right = cell->options.cell_padding_right;

        written += snprint_n_chars(buf + written, buf_len - written, left, ' ');

        if (cell->str_buffer)
            written += buffer_printf(cell->str_buffer, row - cell->options.cell_padding_top, buf + written, buf_len - written - right);
        else
            written += snprint_n_chars(buf + written, buf_len - written, buf_len - written - right, ' ');
        written += snprint_n_chars(buf + written, buf_len - written, right, ' ');

        return written;
    }

}

/*****************************************************************************
 *               ROW
 * ***************************************************************************/
struct fort_row;
typedef struct fort_row fort_row_t;
struct fort_row
{
    vector_t *cells;
};


static fort_row_t * create_row()
{
    fort_row_t * row = F_CALLOC(sizeof(fort_row_t), 1);
    if (row == NULL)
        return NULL;
    row->cells = create_vector(sizeof(fort_cell_t*), 10);
    if (row->cells == NULL) {
        F_FREE(row);
        return NULL;
    }
    return row;
}

static void destroy_row(fort_row_t *row)
{
    if (row == NULL)
        return;

    if (row->cells) {
        size_t cells_n = vector_size(row->cells);
        for (size_t i = 0; i < cells_n; ++i) {
            fort_cell_t *cell = *(fort_cell_t **)vector_at(row->cells, i);
            destroy_cell(cell);
        }
        destroy_vector(row->cells);
    }

    F_FREE(row);
}

static fort_row_t * create_row_from_string(const char *str);
static fort_row_t* create_row_from_fmt_string(const char* FORT_RESTRICT fmt, va_list *va_args);

static int columns_in_row(const fort_row_t *row)
{
    if (row == NULL || row->cells == NULL)
        return 0;

    return vector_size(row->cells);
}

static fort_cell_t *get_cell(fort_row_t *row, size_t col)
{
    if (row == NULL || row->cells == NULL) {
        return NULL;
    }

    if (col < columns_in_row(row)) {
        return *(fort_cell_t**)vector_at(row->cells, col);
    }
    return NULL;
}

static const fort_cell_t *get_cell_c(const fort_row_t *row, size_t col)
{
    return get_cell((fort_row_t *)row, col);
}


/*****************************************************************************
 *               TABLE
 *****************************************************************************/
struct fort_table;
typedef struct fort_table fort_table_t;
struct fort_table
{
    vector_t *rows;
};

static fort_status_t get_table_sizes(const FTABLE *table, size_t *rows, size_t *cols);



static fort_row_t *get_row(fort_table_t *table, size_t row)
{
    if (table == NULL || table->rows == NULL) {
        return NULL;
    }

    size_t rows = vector_size(table->rows);
    if (row < rows) {
        return *(fort_row_t**)vector_at(table->rows, row);
    }
    return NULL;
}

static const fort_row_t *get_row_c(const fort_table_t *table, size_t row)
{
    return get_row((fort_table_t *)table, row);
}
/*****************************************************************************
 *               LIBFORT helpers
 *****************************************************************************/
static char *fort_strdup(const char* str)
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

static size_t number_of_columns_in_format_string(const char *fmt)
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

/*****************************************************************************
 *               LIBFORT
 * ***************************************************************************/

FTABLE * ft_create_table(void)
{
    FTABLE *result = F_CALLOC(1, sizeof(FTABLE));
    if (result == NULL)
        return NULL;

    result->rows = create_vector(sizeof(fort_row_t*), 10);
    if (result->rows == NULL) {
        F_FREE(result);
        return NULL;
    }
    return result;
}


void ft_destroy_table(FTABLE *FORT_RESTRICT table)
{
    if (table == NULL)
        return;

    if (table->rows) {
        size_t row_n = vector_size(table->rows);
        for (size_t i = 0; i < row_n; ++i) {
            destroy_row(*(fort_row_t **)vector_at(table->rows, i));
        }
        destroy_vector(table->rows);
    }
    F_FREE(table);
}




static int ft_row_printf_impl(FTABLE *FORT_RESTRICT table, size_t row, const char* FORT_RESTRICT fmt, va_list *va)
{
    if (table == NULL)
        return -1;

    fort_row_t *new_row = create_row_from_fmt_string(fmt, va);

    if (new_row == NULL) {
        return -1;
    }

    fort_row_t **cur_row_p = NULL;
    size_t sz = vector_size(table->rows);
    if (row >= sz) {
        size_t push_n = row - sz + 1;
        for (size_t i = 0; i < push_n; ++i) {
            fort_row_t *new_row = create_row();
            if (new_row == NULL)
                goto clear;

            if (IS_ERROR(vector_push(table->rows, &new_row))) {
                destroy_row(new_row);
                goto clear;
            }
        }
    }
    /* todo clearing pushed items in case of error */

    cur_row_p = (fort_row_t**)vector_at(table->rows, row);

    destroy_row(*cur_row_p);
    *cur_row_p = new_row;
    return vector_size(new_row->cells);

clear:
    destroy_row(new_row);
    return -1;
}

int ft_hdr_printf(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int result = ft_row_printf_impl(table, 0, fmt, &va);
    va_end(va);
    return result;
}

int ft_row_printf(FTABLE *FORT_RESTRICT table, size_t row, const char* FORT_RESTRICT fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    int result = ft_row_printf_impl(table, row, fmt, &va);
    va_end(va);
    return result;
}

void ft_set_default_options(const fort_table_options_t *options)
{
    memcpy(&g_table_options, options, sizeof(fort_table_options_t));
}

void ft_get_default_options(fort_table_options_t *options)
{
    memcpy(options, &g_table_options, sizeof(fort_table_options_t));
}


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

static fort_status_t fill_buffer_from_string(string_buffer_t *buffer, const char *str)
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

/*****************************************************************************
 *               VECTOR IMPLEMENTATIONS
 * ***************************************************************************/

struct vector
{
    size_t m_size;
    void  *m_data;
    size_t m_capacity;
    size_t m_item_size;
};

static int vector_reallocate_(vector_t *vector, size_t new_capacity)
{
    assert(vector);
    assert(new_capacity > vector->m_capacity);

    size_t new_size = new_capacity * vector->m_item_size;
    vector->m_data = realloc(vector->m_data, new_size);
    if (vector->m_data == NULL)
        return -1;
    return 0;
}

/* ------------ Constructors & Destructors ----------------------------- */

static vector_t* create_vector(size_t item_size, size_t capacity)
{
    vector_t *vector = malloc(sizeof(vector_t));
    if (vector == NULL) {
        SYS_LOG_ERROR("Failed to allocate memory for asock vector");
        return NULL;
    }

    size_t init_size = MAX(item_size * capacity, 1);
    vector->m_data = malloc(init_size);
    if (vector->m_data == NULL) {
        SYS_LOG_ERROR("Failed to allocate memory for asock vector inern. buffer");
        free(vector);
        return NULL;
    }

    vector->m_size      = 0;
    vector->m_capacity  = capacity;
    vector->m_item_size = item_size;

    return vector;
}


static void destroy_vector(vector_t* vector)
{
    assert(vector);
    free(vector->m_data);
    free(vector);
}


/* ----------- Nonmodifying functions --------------------------------- */

static size_t vector_size(const vector_t* vector)
{
    assert(vector);
    return vector->m_size;
}


static size_t vector_capacity(const vector_t* vector)
{
    assert(vector);
    return vector->m_capacity;
}

static size_t vector_index_of(const vector_t* vector, const void *item)
{
    assert(vector);
    assert(item);

    for (size_t i = 0; i < vector->m_size; ++i) {
        void *data_pos = vector->m_data + i * vector->m_item_size;
        if (memcmp(data_pos, item, vector->m_item_size) == 0) {
            return i;
        }
    }
    return INVALID_VEC_INDEX;
}


/* ----------- Modifying functions ------------------------------------- */

static int vector_push (vector_t* vector, const void* item)
{
    assert(vector);
    assert(item);

    if (vector->m_size == vector->m_capacity) {
        if (vector_reallocate_(vector, vector->m_capacity * 2) == -1)
            return F_ERROR;
        vector->m_capacity = vector->m_capacity * 2;
    }

    ptrdiff_t deviation = vector->m_size * vector->m_item_size;
    memcpy(vector->m_data + deviation, item, vector->m_item_size);

    ++(vector->m_size);

    return F_SUCCESS;
}


static int vector_erase(vector_t *vector, size_t index)
{
    assert(vector);

    if (vector->m_size == 0 || index >= vector->m_size)
        return F_ERROR;

    memmove(vector->m_data + vector->m_item_size * index,
            vector->m_data + vector->m_item_size * (index + 1),
            (vector->m_size - 1 - index) * vector->m_item_size);
    vector->m_size--;
    return F_SUCCESS;
}


static void vector_clear(vector_t *vector)
{
    vector->m_size = 0;
}



static void *vector_at(vector_t *vector, size_t index)
{
    if (index >= vector->m_size)
        return NULL;

    return vector->m_data + index * vector->m_item_size;
}


#define FOR_EACH_(type, item, vector, index_name) \
    for (size_t index_name = 0; (index_name < vector_size(vector)) ? ((item = *(type*)vector_at(vector, index_name)), 1) : 0;  ++index_name)

#define FOR_EACH(type, item, vector) \
    FOR_EACH_(type, item, vector, UNIQUE_NAME(i))

/*****************************************************************************
 *               ROW
 * ***************************************************************************/

static fort_row_t* create_row_from_string(const char *str)
{
    fort_row_t * row = create_row();
    if (row == NULL)
        return NULL;

    if (str == NULL)
        return row;

    char *str_copy = F_STRDUP(str);
    if (str_copy == NULL)
        goto clear;

    char *pos = str_copy;
    char *base_pos = str_copy;
    int number_of_separators = 0;
    while (*pos) {
        pos = strchr(pos, FORT_COL_SEPARATOR);
        if (pos != NULL) {
            *(pos) = '\0';
            ++pos;
            number_of_separators++;
        }

        fort_cell_t *cell = create_cell();
        if (cell == NULL)
            goto clear;

        int status = fill_buffer_from_string(cell->str_buffer, base_pos);
        if (IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if (IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        if (pos == NULL)
            break;
        base_pos = pos;
    }

    /* special case if in format string last cell is empty */
    while (vector_size(row->cells) < (number_of_separators + 1)) {
        fort_cell_t *cell = create_cell();
        if (cell == NULL)
            goto clear;

        int status = fill_buffer_from_string(cell->str_buffer, "");
        if (IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }

        status = vector_push(row->cells, &cell);
        if (IS_ERROR(status)) {
            destroy_cell(cell);
            goto clear;
        }
    }

    F_FREE(str_copy);
    return row;

clear:
    destroy_row(row);
    F_FREE(str_copy);
    return NULL;
}


static fort_row_t* create_row_from_fmt_string(const char* FORT_RESTRICT fmt, va_list *va_args)
{
    string_buffer_t *buffer = create_string_buffer(1024);
    if (buffer == NULL)
        return NULL;

    int cols_origin = number_of_columns_in_format_string(fmt);

    while (1) {
        va_list va;
        va_copy(va, *va_args);
        int virtual_sz = vsnprintf(buffer->str, buffer->str_sz, fmt, va);
        va_end(va);
        /* If error encountered */
        if (virtual_sz == -1)
            goto clear;

        /* Successful write */
        if (virtual_sz < buffer->str_sz)
            break;

        /* Otherwise buffer was too small, so incr. buffer size ant try again. */
        if (!IS_SUCCESS(realloc_string_buffer_without_copy(buffer)))
            goto clear;
    }

    int cols = number_of_columns_in_format_string(buffer->str);
    if (cols == cols_origin) {

        fort_row_t *row = create_row_from_string(buffer->str);
        if (row == NULL) {
            goto clear;
        }

        destroy_string_buffer(buffer);
        return row;
    }

    /* todo: add processing of cols != cols_origin */

clear:
    destroy_string_buffer(buffer);
    return NULL;
}

static int snprintf_row(const fort_row_t *row, char *buffer, size_t buf_sz, size_t *col_width_arr, size_t col_width_arr_sz,
                        size_t row_height)
{
    if (row == NULL)
        return -1;

    int cols_in_row = columns_in_row(row);
    if (cols_in_row > col_width_arr_sz)
        return -1;

    int dev = 0;
    for (size_t i = 0; i < row_height; ++i) {
        dev += snprint_n_chars(buffer + dev, buf_sz - dev, 1, FORT_VER_CELL_SEPARATOR);
        for (size_t j = 0; j < col_width_arr_sz; ++j) {
            if (j < cols_in_row) {
                fort_cell_t *cell = *(fort_cell_t**)vector_at(row->cells, j);
                dev += cell_printf(cell, i, buffer + dev, col_width_arr[j] + 1);
            } else {
                dev += snprint_n_chars(buffer + dev, buf_sz - dev, col_width_arr[j], ' ');
            }
            dev += snprint_n_chars(buffer + dev, buf_sz - dev, 1, FORT_VER_CELL_SEPARATOR);
        }
        dev += snprint_n_chars(buffer + dev, buf_sz - dev, 1, '\n');
    }
    return dev;
}

/*****************************************************************************
 *               TABLE
 * ***************************************************************************/

/*
 * Returns number of cells (rows * cols)
 */
static fort_status_t get_table_sizes(const FTABLE *table, size_t *rows, size_t *cols)
{
    *rows = 0;
    *cols = 0;
    if (table && table->rows) {
        *rows = vector_size(table->rows);
        fort_row_t *row = NULL;
        FOR_EACH(fort_row_t*, row, table->rows) {
            size_t cols_in_row = columns_in_row(row);
            if (cols_in_row > *cols)
                *cols = cols_in_row;
        }
    }
    return F_SUCCESS;
}

static fort_status_t table_rows_and_cols_geometry(const FTABLE *table,
                                                  size_t **col_width_arr_p, size_t *col_width_arr_sz,
                                                  size_t **row_height_arr_p, size_t *row_height_arr_sz)
{
    if (table == NULL) {
        return F_ERROR;
    }

    size_t cols = 0;
    size_t rows = 0;
    int status = get_table_sizes(table, &rows, &cols);
    if (IS_ERROR(status))
        return status;

    size_t *col_width_arr = F_CALLOC(sizeof(size_t), cols);
    size_t *row_height_arr = F_CALLOC(sizeof(size_t), rows);
    if (col_width_arr == NULL || row_height_arr == NULL) {
        F_FREE(col_width_arr);
        F_FREE(row_height_arr);
        return F_ERROR;
    }

    for (size_t col = 0; col < cols; ++col) {
        col_width_arr[col] = 0;
        for (size_t row = 0; row < rows; ++row) {
            const fort_row_t *row_p = get_row_c(table, row);
            const fort_cell_t *cell = get_cell_c(row_p, col);
            if (cell) {
                col_width_arr[col] = MAX(col_width_arr[col], hint_width_cell(cell));
                row_height_arr[row] = MAX(row_height_arr[row], hint_height_cell(cell));
            }
        }
    }

    *col_width_arr_p = col_width_arr;
    *col_width_arr_sz = cols;
    *row_height_arr_p = row_height_arr;
    *row_height_arr_sz = rows;
    return F_SUCCESS;
}

/*
 * Returns geometry in characters
 */
static fort_status_t table_geometry(const FTABLE *table, size_t *height, size_t *width)
{
    if (table == NULL)
        return F_ERROR;

    *height = 0;
    *width = 0;
    size_t cols = 0;
    size_t rows = 0;
    size_t *col_width_arr = NULL;
    size_t *row_height_arr = NULL;

    int status = table_rows_and_cols_geometry(table, &col_width_arr, &cols, &row_height_arr, &rows);
    if (IS_ERROR(status))
        return status;

    *width = 1 + (cols == 0 ? 1 : cols) + 1; // for boundaries (that take 1 symbol) + newline
    for (size_t i = 0; i < cols; ++i) {
        *width += col_width_arr[i];
    }

    *height = 1 + (rows == 0 ? 1 : rows); // for boundaries (that take 1 symbol)
    for (size_t i = 0; i < rows; ++i) {
        *height += row_height_arr[i];
    }
    F_FREE(col_width_arr);
    F_FREE(row_height_arr);
    return F_SUCCESS;

}



char* ft_to_string(const FTABLE *FORT_RESTRICT table)
{
#define CHECK_RESULT_AND_MOVE_DEV(statement) \
    k = statement; \
    if (k < 0) {\
        goto clear; \
    } \
    dev += k;

    assert(table);

    size_t height = 0;
    size_t width = 0;
    int status = table_geometry(table, &height, &width);
    if (IS_ERROR(status)) {
        return NULL;
    }
    size_t sz = height * width + 1;
    char *buffer = F_MALLOC(sz);
    if (buffer == NULL)
        return NULL;

    size_t cols = 0;
    size_t rows = 0;
    size_t *col_width_arr = NULL;
    size_t *row_height_arr = NULL;
    status = table_rows_and_cols_geometry(table, &col_width_arr, &cols, &row_height_arr, &rows);

    if (IS_ERROR(status))
        return NULL;

    int dev = 0;
    int k = 0;
    CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars(buffer + dev, sz - dev, width - 1, FORT_HOR_CELL_SEPARATOR));
    CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars(buffer + dev, sz - dev, 1, '\n'));


    for (size_t i = 0; i < rows; ++i) {
        fort_row_t *row = *(fort_row_t**)vector_at(table->rows, i);
        CHECK_RESULT_AND_MOVE_DEV(snprintf_row(row, buffer + dev, sz - dev, col_width_arr, cols, row_height_arr[i]));
        CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars(buffer + dev, sz - dev, width - 1, FORT_HOR_CELL_SEPARATOR));
        CHECK_RESULT_AND_MOVE_DEV(snprint_n_chars(buffer + dev, sz - dev, 1, '\n'));
    }

    F_FREE(col_width_arr);
    F_FREE(row_height_arr);
    return buffer;

clear:
    F_FREE(col_width_arr);
    F_FREE(row_height_arr);
    F_FREE(buffer);
    return NULL;

#undef CHECK_RESULT_AND_MOVE_DEV
}





