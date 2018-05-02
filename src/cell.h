#ifndef CELL_H
#define CELL_H

#include "fort_impl.h"




/*****************************************************************************
 *               CELL
 * ***************************************************************************/
fort_cell_t * create_cell(void);


void destroy_cell(fort_cell_t *cell);
size_t hint_width_cell(const fort_cell_t *cell, const context_t *context);
size_t hint_height_cell(const fort_cell_t *cell, const context_t *context);


void set_cell_type(fort_cell_t *cell, enum CellType type);
enum CellType get_cell_type(const fort_cell_t *cell);

/*
 * Returns number of lines in cell. If cell is empty or
 * contains empty string, then 0 is returned.
 */
/* static int lines_number_cell(fort_cell_t *cell); */

int cell_printf(fort_cell_t *cell, size_t row, char *buf, size_t buf_len, const context_t *context);
fort_status_t fill_cell_from_string(fort_cell_t *cell, const char *str);

#ifdef FT_HAVE_WCHAR
int cell_wprintf(fort_cell_t *cell, size_t row, wchar_t *buf, size_t buf_len, const context_t *context);
fort_status_t fill_cell_from_wstring(fort_cell_t *cell, const wchar_t *str);
#endif

string_buffer_t* cell_get_string_buffer(fort_cell_t *cell);

#endif /* CELL_H */
