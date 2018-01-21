#include "cell.h"
#include "options.h"
#include "string_buffer.h"
#include "assert.h"

/*****************************************************************************
 *               CELL
 * ***************************************************************************/

struct fort_cell
{
    string_buffer_t *str_buffer;
    fort_table_options_t *options;
};

fort_cell_t * create_cell()
{
    fort_cell_t *cell = F_CALLOC(sizeof(fort_cell_t), 1);
    if (cell == NULL)
        return NULL;
    cell->str_buffer = create_string_buffer(DEFAULT_STR_BUF_SIZE);
    if (cell->str_buffer == NULL) {
        F_FREE(cell);
        return NULL;
    }
    cell->options = NULL;
//    init_cell_options(&(cell->options));
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
    assert(cell);
    assert(context);
    int result = context->cell_padding_left + context->cell_padding_right;
    if (cell->str_buffer && cell->str_buffer->str) {
        result += buffer_text_width(cell->str_buffer);
    }
    return result;
}

int hint_height_cell(const fort_cell_t *cell, const context_t *context)
{
    assert(cell);
    assert(context);
    int result = context->cell_padding_top + context->cell_padding_bottom;
    if (cell->str_buffer && cell->str_buffer->str) {
        size_t text_height = buffer_text_height(cell->str_buffer);
        result += text_height == 0 ? context->cell_empty_string_height : text_height;
    }
    return result;
}


/*
 * Returns number of lines in cell. If cell is empty or
 * contains empty string, then 0 is returned.
 */
//static int lines_number_cell(fort_cell_t *cell)
//{
//    assert(cell);
//    if (cell->str_buffer == NULL || cell->str_buffer->str == NULL || cell->str_buffer->str[0] == '\0') {
//        return 0;
//    }

//    int result = 0;
//    char *pos = cell->str_buffer->str;
//    while ((pos = strchr(pos, '\n')) != NULL) {
//        result++;
//        pos++;
//    }
//    return result + 1;
//}

int cell_printf(fort_cell_t *cell, size_t row, size_t column, char *buf, size_t buf_len, const context_t *context)
{
    if (cell == NULL || buf_len == 0
            || (buf_len <= hint_width_cell(cell, context))) {
        return -1;
    }

    if (row >= hint_height_cell(cell, context)
            || row < context->cell_padding_top
            || row >= (context->cell_padding_top + buffer_text_height(cell->str_buffer))) {
        int k = snprint_n_chars(buf, buf_len, buf_len - 1, ' ');
        return k;
    } else {
        int written = 0;
        int left = context->cell_padding_left;
        int right = context->cell_padding_right;

        written += snprint_n_chars(buf + written, buf_len - written, left, ' ');

        if (cell->str_buffer)
            written += buffer_printf(cell->str_buffer, row - context->cell_padding_top, column, buf + written, buf_len - written - right, context);
        else
            written += snprint_n_chars(buf + written, buf_len - written, buf_len - written - right, ' ');
        written += snprint_n_chars(buf + written, buf_len - written, right, ' ');

        return written;
    }

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
