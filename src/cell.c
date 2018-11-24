#include "cell.h"
#include "properties.h"
#include "string_buffer.h"
#include <assert.h>

struct fort_cell {
    string_buffer_t *str_buffer;
    enum CellType cell_type;
};

FT_INTERNAL
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
    cell->cell_type = CommonCell;
    return cell;
}

FT_INTERNAL
void destroy_cell(fort_cell_t *cell)
{
    if (cell == NULL)
        return;
    destroy_string_buffer(cell->str_buffer);
    F_FREE(cell);
}

FT_INTERNAL
fort_cell_t *copy_cell(fort_cell_t *cell)
{
    assert(cell);

    fort_cell_t *result = create_cell();
    destroy_string_buffer(result->str_buffer);
    result->str_buffer = copy_string_buffer(cell->str_buffer);
    if (result->str_buffer == NULL) {
        destroy_cell(result);
        return NULL;
    }
    result->cell_type = cell->cell_type;
    return result;
}

FT_INTERNAL
void set_cell_type(fort_cell_t *cell, enum CellType type)
{
    assert(cell);
    cell->cell_type = type;
}

FT_INTERNAL
enum CellType get_cell_type(const fort_cell_t *cell)
{
    assert(cell);
    return cell->cell_type;
}

FT_INTERNAL
size_t hint_width_cell(const fort_cell_t *cell, const context_t *context, enum request_geom_type geom)
{
    /* todo:
     * At the moment min width includes paddings. Maybe it is better that min width weren't include
     * paddings but be min width of the cell content without padding
     */

    assert(cell);
    assert(context);
    size_t cell_padding_left = get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_LEFT_PADDING);
    size_t cell_padding_right = get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_RIGHT_PADDING);
    size_t result = cell_padding_left + cell_padding_right;
    if (cell->str_buffer && cell->str_buffer->str.data) {
        result += buffer_text_width(cell->str_buffer);
    }
    result = MAX(result, (size_t)get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_MIN_WIDTH));

    if (geom == INTERN_REPR_GEOMETRY) {
        char cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
        get_style_tag_for_cell(context->table_properties, context->row, context->column, cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
        result += strlen(cell_style_tag);

        char reset_cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
        get_reset_style_tag_for_cell(context->table_properties, context->row, context->column, reset_cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
        result += strlen(reset_cell_style_tag);

        char content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
        get_style_tag_for_content(context->table_properties, context->row, context->column, content_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
        result += strlen(content_style_tag);

        char reset_content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
        get_reset_style_tag_for_content(context->table_properties, context->row, context->column, reset_content_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
        result += strlen(reset_content_style_tag);
    }

    return result;
}

FT_INTERNAL
size_t hint_height_cell(const fort_cell_t *cell, const context_t *context)
{
    assert(cell);
    assert(context);
    size_t cell_padding_top = get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_TOP_PADDING);
    size_t cell_padding_bottom = get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_BOTTOM_PADDING);
    size_t cell_empty_string_height = get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_EMPTY_STR_HEIGHT);
    size_t result = cell_padding_top + cell_padding_bottom;
    if (cell->str_buffer && cell->str_buffer->str.data) {
        size_t text_height = buffer_text_height(cell->str_buffer);
        result += text_height == 0 ? cell_empty_string_height : text_height;
    }
    return result;
}


FT_INTERNAL
int cell_printf(fort_cell_t *cell, size_t row, char *buf, size_t buf_len, const context_t *context)
{
    const char *space_char = " ";
    int (*buffer_printf_)(string_buffer_t *, size_t, char *, size_t, const context_t *, const char *, const char *) = buffer_printf;
    int (*snprint_n_strings_)(char *, size_t, size_t, const char *) = snprint_n_strings;

    if (cell == NULL || buf_len == 0
        || (buf_len <= hint_width_cell(cell, context, VISIBLE_GEOMETRY))) {
        return -1;
    }

    unsigned int cell_padding_top = get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_TOP_PADDING);
    unsigned int cell_padding_left = get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_LEFT_PADDING);
    unsigned int cell_padding_right = get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_RIGHT_PADDING);

    int written = 0;
    int invisible_written = 0;
    int tmp = 0;

    /* todo: Dirty hack with changing buf_len! need refactoring. */
    /* Also maybe it is better to move all struff with colors to buffers? */
    char cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_style_tag_for_cell(context->table_properties, context->row, context->column, cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(cell_style_tag);

    char reset_cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_reset_style_tag_for_cell(context->table_properties, context->row, context->column, reset_cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(reset_cell_style_tag);

    char content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_style_tag_for_content(context->table_properties, context->row, context->column, content_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(content_style_tag);

    char reset_content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_reset_style_tag_for_content(context->table_properties, context->row, context->column, reset_content_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(reset_content_style_tag);

    /*    CELL_STYLE_T   LEFT_PADDING   CONTENT_STYLE_T  CONTENT   RESET_CONTENT_STYLE_T    RIGHT_PADDING   RESET_CELL_STYLE_T
     *  |              |              |                |         |                       |                |                    |
     *        L1                                                                                                    R1
     *                     L2                                                                   R2
     *                                     L3                               R3
     */

    size_t L2 = cell_padding_left;

    size_t R2 = cell_padding_right;
    size_t R3 = strlen(reset_cell_style_tag);

#define TOTAL_WRITTEN (written + invisible_written)
#define RIGHT (cell_padding_right + extra_right)

#define WRITE_CELL_STYLE_TAG        CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, 1, cell_style_tag))
#define WRITE_RESET_CELL_STYLE_TAG  CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, 1, reset_cell_style_tag))
#define WRITE_CONTENT_STYLE_TAG        CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, 1, content_style_tag))
#define WRITE_RESET_CONTENT_STYLE_TAG  CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, 1, reset_content_style_tag))

    if (row >= hint_height_cell(cell, context)
        || row < cell_padding_top
        || row >= (cell_padding_top + buffer_text_height(cell->str_buffer))) {
        WRITE_CELL_STYLE_TAG;
        WRITE_CONTENT_STYLE_TAG;
        WRITE_RESET_CONTENT_STYLE_TAG;
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len, buf_len - 1 - TOTAL_WRITTEN - R3, space_char));
        WRITE_RESET_CELL_STYLE_TAG;
        return TOTAL_WRITTEN;
    }

    WRITE_CELL_STYLE_TAG;
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, L2, space_char));
    if (cell->str_buffer) {
        CHCK_RSLT_ADD_TO_WRITTEN(buffer_printf_(cell->str_buffer, row - cell_padding_top, buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN - R2 - R3, context, content_style_tag, reset_content_style_tag));
    } else {
        WRITE_CONTENT_STYLE_TAG;
        WRITE_RESET_CONTENT_STYLE_TAG;
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN - R2 - R3, space_char));
    }
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, R2, space_char));
    WRITE_RESET_CELL_STYLE_TAG;

    return TOTAL_WRITTEN;

clear:
    return -1;
#undef WRITE_CELL_STYLE_TAG
#undef WRITE_RESET_CELL_STYLE_TAG
#undef WRITE_CONTENT_STYLE_TAG
#undef WRITE_RESET_CONTENT_STYLE_TAG
#undef TOTAL_WRITTEN
#undef RIGHT
}

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
int cell_wprintf(fort_cell_t *cell, size_t row, wchar_t *buf, size_t buf_len, const context_t *context)
{
    const char *space_char = " ";
    int (*buffer_printf_)(string_buffer_t *, size_t, wchar_t *, size_t, const context_t *, const char *, const char *) = buffer_wprintf;
    int (*snprint_n_strings_)(wchar_t *, size_t, size_t, const char *) = wsnprint_n_string;

    if (cell == NULL || buf_len == 0
        || (buf_len <= hint_width_cell(cell, context, VISIBLE_GEOMETRY))) {
        return -1;
    }

    unsigned int cell_padding_top = get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_TOP_PADDING);
    unsigned int cell_padding_left = get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_LEFT_PADDING);
    unsigned int cell_padding_right = get_cell_property_value_hierarcial(context->table_properties, context->row, context->column, FT_CPROP_RIGHT_PADDING);

    int written = 0;
    int invisible_written = 0;
    int tmp = 0;

    /* todo: Dirty hack with changing buf_len! need refactoring. */
    /* Also maybe it is better to move all struff with colors to buffers? */
    char cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_style_tag_for_cell(context->table_properties, context->row, context->column, cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(cell_style_tag);

    char reset_cell_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_reset_style_tag_for_cell(context->table_properties, context->row, context->column, reset_cell_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(reset_cell_style_tag);

    char content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_style_tag_for_content(context->table_properties, context->row, context->column, content_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(content_style_tag);

    char reset_content_style_tag[TEXT_STYLE_TAG_MAX_SIZE];
    get_reset_style_tag_for_content(context->table_properties, context->row, context->column, reset_content_style_tag, TEXT_STYLE_TAG_MAX_SIZE);
    buf_len += strlen(reset_content_style_tag);

    /*    CELL_STYLE_T   LEFT_PADDING   CONTENT_STYLE_T  CONTENT   RESET_CONTENT_STYLE_T    RIGHT_PADDING   RESET_CELL_STYLE_T
     *  |              |              |                |         |                       |                |                    |
     *        L1                                                                                                    R1
     *                     L2                                                                   R2
     *                                     L3                               R3
     */

    size_t L2 = cell_padding_left;

    size_t R2 = cell_padding_right;
    size_t R3 = strlen(reset_cell_style_tag);

#define TOTAL_WRITTEN (written + invisible_written)
#define RIGHT (right + extra_right)

#define WRITE_CELL_STYLE_TAG        CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, 1, cell_style_tag))
#define WRITE_RESET_CELL_STYLE_TAG  CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, 1, reset_cell_style_tag))
#define WRITE_CONTENT_STYLE_TAG        CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, 1, content_style_tag))
#define WRITE_RESET_CONTENT_STYLE_TAG  CHCK_RSLT_ADD_TO_INVISIBLE_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, 1, reset_content_style_tag))

    if (row >= hint_height_cell(cell, context)
        || row < cell_padding_top
        || row >= (cell_padding_top + buffer_text_height(cell->str_buffer))) {
        WRITE_CELL_STYLE_TAG;
        WRITE_CONTENT_STYLE_TAG;
        WRITE_RESET_CONTENT_STYLE_TAG;
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len, buf_len - 1 - TOTAL_WRITTEN - R3, space_char));
        WRITE_RESET_CELL_STYLE_TAG;
        return TOTAL_WRITTEN;
    }

    WRITE_CELL_STYLE_TAG;
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, L2, space_char));
    if (cell->str_buffer) {
        CHCK_RSLT_ADD_TO_WRITTEN(buffer_printf_(cell->str_buffer, row - cell_padding_top, buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN - R2 - R3, context, content_style_tag, reset_content_style_tag));
    } else {
        WRITE_CONTENT_STYLE_TAG;
        WRITE_RESET_CONTENT_STYLE_TAG;
        CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN - R2 - R3, space_char));
    }
    CHCK_RSLT_ADD_TO_WRITTEN(snprint_n_strings_(buf + TOTAL_WRITTEN, buf_len - TOTAL_WRITTEN, R2, space_char));
    WRITE_RESET_CELL_STYLE_TAG;

    return TOTAL_WRITTEN;

clear:
    return -1;
#undef WRITE_CELL_STYLE_TAG
#undef WRITE_RESET_CELL_STYLE_TAG
#undef WRITE_CONTENT_STYLE_TAG
#undef WRITE_RESET_CONTENT_STYLE_TAG
#undef TOTAL_WRITTEN
#undef RIGHT
}
#endif

FT_INTERNAL
fort_status_t fill_cell_from_string(fort_cell_t *cell, const char *str)
{
    assert(str);
    assert(cell);

    return fill_buffer_from_string(cell->str_buffer, str);
}

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
fort_status_t fill_cell_from_wstring(fort_cell_t *cell, const wchar_t *str)
{
    assert(str);
    assert(cell);

    return fill_buffer_from_wstring(cell->str_buffer, str);
}

#endif

FT_INTERNAL
string_buffer_t *cell_get_string_buffer(fort_cell_t *cell)
{
    assert(cell);
    assert(cell->str_buffer);
    return cell->str_buffer;
}

