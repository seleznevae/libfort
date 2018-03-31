#include "cell.h"
#include "options.h"
#include "string_buffer.h"
#include "assert.h"

/*****************************************************************************
 *               CELL
 * ***************************************************************************/


struct fort_cell {
    string_buffer_t *str_buffer;
    fort_table_options_t *options;
};

fort_cell_t *create_cell(void)
{
    fort_cell_t *cell = (fort_cell_t *)F_CALLOC(sizeof(fort_cell_t), 1);
    if (cell == NULL)
        return NULL;
    cell->str_buffer = create_string_buffer(DEFAULT_STR_BUF_SIZE, CharBuf);
    if (cell->str_buffer == NULL) {
        F_FREE(cell);
        return NULL;
    }
    cell->options = NULL;
    /*init_cell_options(&(cell->options));*/
    return cell;
}

void destroy_cell(fort_cell_t *cell)
{
    if (cell == NULL)
        return;
    destroy_string_buffer(cell->str_buffer);
    F_FREE(cell->options);
    F_FREE(cell);
}

int hint_width_cell(const fort_cell_t *cell, const context_t *context)
{
    /* todo:
     * At the moment min width includes paddings. Maybe it is better that min width weren't include
     * paddings but be min width of the cell content without padding
     */

    assert(cell);
    assert(context);
    int cell_padding_left = get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_LEFT_PADDING);
    int cell_padding_right = get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_RIGHT_PADDING);
    int result = cell_padding_left + cell_padding_right;
    if (cell->str_buffer && cell->str_buffer->str.data) {
        result += buffer_text_width(cell->str_buffer);
    }
    result = MAX(result, get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_MIN_WIDTH));
    return result;
}

int hint_height_cell(const fort_cell_t *cell, const context_t *context)
{
    assert(cell);
    assert(context);
    int cell_padding_top = get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_TOP_PADDING);
    int cell_padding_bottom = get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_BOTTOM_PADDING);
    int cell_empty_string_height = get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_EMPTY_STR_HEIGHT);
    int result = cell_padding_top + cell_padding_bottom;
    if (cell->str_buffer && cell->str_buffer->str.data) {
        size_t text_height = buffer_text_height(cell->str_buffer);
        result += text_height == 0 ? cell_empty_string_height : text_height;
    }
    return result;
}


/*
 * Returns number of lines in cell. If cell is empty or
 * contains empty string, then 0 is returned.
 */
/*
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
*/

int cell_printf(fort_cell_t *cell, size_t row, char *buf, size_t buf_len, const context_t *context)
{
    char space_char = ' ';
    int (*buffer_printf_)(string_buffer_t *, size_t, char *, size_t, const context_t *) = buffer_printf;
    int (*snprint_n_chars_)(char *, size_t, size_t, char) = snprint_n_chars;


    if (cell == NULL || buf_len == 0
        || (buf_len <= hint_width_cell(cell, context))) {
        return -1;
    }

    int cell_padding_top = get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_TOP_PADDING);
    int cell_padding_left = get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_LEFT_PADDING);
    int cell_padding_right = get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_RIGHT_PADDING);

    if (row >= hint_height_cell(cell, context)
        || row < cell_padding_top
        || row >= (cell_padding_top + buffer_text_height(cell->str_buffer))) {
        return snprint_n_chars_(buf, buf_len, buf_len - 1, space_char);
    }


    int written = 0;
    int tmp = 0;
    int left = cell_padding_left;
    int right = cell_padding_right;

    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buf + written, buf_len - written, left, space_char));

    if (cell->str_buffer)
        CHCK_RSLT_ADD_TO_WRITTEN(buffer_printf_(cell->str_buffer, row - cell_padding_top, buf + written, buf_len - written - right, context));
    else
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buf + written, buf_len - written, buf_len - written - right, space_char));
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buf + written, buf_len - written, right, space_char));

    return written;

clear:
    return -1;
}

int cell_wprintf(fort_cell_t *cell, size_t row, wchar_t *buf, size_t buf_len, const context_t *context)
{
    wchar_t space_char = L' ';
    int (*buffer_printf_)(string_buffer_t *, size_t, wchar_t *, size_t, const context_t *) = buffer_wprintf;
    int (*snprint_n_chars_)(wchar_t *, size_t, size_t, wchar_t) = wsnprint_n_chars;


    if (cell == NULL || buf_len == 0
        || (buf_len <= hint_width_cell(cell, context))) {
        return -1;
    }

    int cell_padding_top = get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_TOP_PADDING);
    int cell_padding_left = get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_LEFT_PADDING);
    int cell_padding_right = get_cell_opt_value_hierarcial(context->table_options, context->row, context->column, FT_COPT_RIGHT_PADDING);

    if (row >= hint_height_cell(cell, context)
        || row < cell_padding_top
        || row >= (cell_padding_top + buffer_text_height(cell->str_buffer))) {
        return snprint_n_chars_(buf, buf_len, buf_len - 1, space_char);
    }

    int written = 0;
    int tmp = 0;
    int left = cell_padding_left;
    int right = cell_padding_right;

    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buf + written, buf_len - written, left, space_char));

    if (cell->str_buffer)
        CHCK_RSLT_ADD_TO_WRITTEN(buffer_printf_(cell->str_buffer, row - cell_padding_top, buf + written, buf_len - written - right, context));
    else
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buf + written, buf_len - written, buf_len - written - right, space_char));
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_chars_(buf + written, buf_len - written, right, space_char));

    return written;

clear:
    return -1;
}


fort_status_t fill_cell_from_string(fort_cell_t *cell, const char *str)
{
    assert(str);
    assert(cell);

    return fill_buffer_from_string(cell->str_buffer, str);
}

string_buffer_t *cell_get_string_buffer(fort_cell_t *cell)
{
    assert(cell);
    assert(cell->str_buffer);
    return cell->str_buffer;
}

