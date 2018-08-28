#ifndef CELL_H
#define CELL_H

#include "fort_utils.h"

FT_INTERNAL
fort_cell_t *create_cell(void);

FT_INTERNAL
void destroy_cell(fort_cell_t *cell);

FT_INTERNAL
size_t hint_width_cell(const fort_cell_t *cell, const context_t *context);

FT_INTERNAL
size_t hint_height_cell(const fort_cell_t *cell, const context_t *context);

FT_INTERNAL
void set_cell_type(fort_cell_t *cell, enum CellType type);

FT_INTERNAL
enum CellType get_cell_type(const fort_cell_t *cell);

FT_INTERNAL
int cell_printf(fort_cell_t *cell, size_t row, char *buf, size_t buf_len, const context_t *context);

FT_INTERNAL
fort_status_t fill_cell_from_string(fort_cell_t *cell, const char *str);

#ifdef FT_HAVE_WCHAR
FT_INTERNAL
int cell_wprintf(fort_cell_t *cell, size_t row, wchar_t *buf, size_t buf_len, const context_t *context);

FT_INTERNAL
fort_status_t fill_cell_from_wstring(fort_cell_t *cell, const wchar_t *str);
#endif

FT_INTERNAL
string_buffer_t *cell_get_string_buffer(fort_cell_t *cell);

#endif /* CELL_H */
