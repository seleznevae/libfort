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

/**
 * @file fort.hpp
 * @brief Main header file describing libfort C++ API .
 *
 * This files contains C++ wrappers around libfort API that can
 * be used in C++ code.
 */
#ifndef LIBFORT_HPP
#define LIBFORT_HPP

#include <string>
#include <stdexcept>
#include <sstream>
#include "fort.h"

namespace fort
{

enum class CellOption {
    MinWidth,
    TextAlign,
    TopPadding,
    BottomPadding,
    LeftPadding,
    RightPading,
    EmptyStrHeight,
    RowType
};

enum class TableOption {
    LeftMargin,
    TopMargin,
    RightMargin,
    BottomMargin,
};

enum class TextAlign {
    Left = 0,
    Center,
    Right
};

enum class RowType {
    Common = 0,
    Header
};

class TableManipulator {
public:
    explicit TableManipulator(int i)
        :value(i)
    {
    }
    friend class Table;
private:
    int value;
};

const TableManipulator header(0);
const TableManipulator endr(1);
const TableManipulator separator(2);

/**
 * Table - formatted table.
 *
 * Table class is a C++ wrapper around struct ft_table.
 */
class Table {
public:
    Table()
        :table(ft_create_table())
    {
        if (table == NULL)
            throw std::runtime_error("Runtime error");
    }

    ~Table()
    {
        ft_destroy_table(table);
    }

    /**
     * Convert table to string representation.
     *
     * @return
     *   - String representation of formatted table, on success.
     *   - In case of error std::runtime_error is thrown.
     */
    std::string to_string() const
    {
        const char *str = ft_to_string(table);
        if (str == NULL)
            throw std::runtime_error("Runtime error");
        return str;
    }

    /**
     * Convert table to string representation.
     *
     * Table object has ownership of the returned pointer. So there is no need to
     * free it. To take ownership user should explicitly copy the returned
     * string with strdup or similar functions.
     *
     * Returned pointer may be later invalidated by:
     * - Calling destroying the table;
     * - Other invocations of c_str or to_string.
     *
     * @return
     *   - The pointer to the string representation of formatted table, on success.
     *   - NULL on error.
     */
    const char *c_str() const
    {
        return ft_to_string(table);
    }

    /**
     * Write provided object to the the table.
     *
     * To convert object to the string representation conversion for
     * std::ostream is used.
     *
     * @param arg
     *   Obect that would be inserted in the current cell.
     * @return
     *   - Reference to the current table.
     */
    template <typename T>
    Table &operator<<(const T &arg)
    {
        stream << arg;
        if (stream.tellp()) {
            ft_nwrite(table, 1, stream.str().c_str());
            stream.str(std::string());
        }
        return *this;
    }

    Table &operator<<(const TableManipulator &arg)
    {
        if (arg.value == header.value)
            ft_set_cell_option(table, FT_CUR_ROW, FT_ANY_ROW, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        else if (arg.value == endr.value)
            ft_ln(table);
        else if (arg.value == separator.value)
            ft_add_separator(table);
        return *this;
    }

    bool write(const char *str)
    {
        return FT_IS_SUCCESS(ft_write(table, str));
    }

    bool write_ln(const char *str)
    {
        return FT_IS_SUCCESS(ft_write_ln(table, str));
    }

    bool write(const std::string &str)
    {
        return write(str.c_str());
    }

    bool write_ln(const std::string &str)
    {
        return write_ln(str.c_str());
    }

#ifdef __cpp_variadic_templates
    template <typename T, typename ...Ts>
    bool write(const T &arg, const Ts &...args)
    {
        return write(arg) && write(args...);
    }

    template <typename T, typename ...Ts>
    bool write_ln(const T &arg, const Ts &...args)
    {
        if (sizeof...(args) == 0)
            return write_ln(arg);
        return write(arg) && write_ln(args...);
    }
#else /* __cpp_variadic_templates */

    template <typename T_0, typename T_1>
    bool write(const T_0 &arg_0, const T_1 &arg_1)
    {
        return write(arg_0) && write(arg_1);
    }

    template <typename T_0, typename T_1, typename T_2>
    bool write(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2)
    {
        return write(arg_0) && write(arg_1, arg_2);
    }

    template <typename T_0, typename T_1, typename T_2, typename T_3>
    bool write(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2, const T_3 &arg_3)
    {
        return write(arg_0) && write(arg_1, arg_2, arg_3);
    }

    template <typename T_0, typename T_1, typename T_2, typename T_3, typename T_4>
    bool write(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2, const T_3 &arg_3, const T_4 &arg_4)
    {
        return write(arg_0) && write(arg_1, arg_2, arg_3, arg_4);
    }

    template <typename T_0, typename T_1, typename T_2, typename T_3, typename T_4, typename T_5>
    bool write(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2, const T_3 &arg_3, const T_4 &arg_4, const T_5 &arg_5)
    {
        return write(arg_0) && write(arg_1, arg_2, arg_3, arg_4, arg_5);
    }

    template <typename T_0, typename T_1, typename T_2, typename T_3, typename T_4, typename T_5, typename T_6>
    bool write(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2, const T_3 &arg_3, const T_4 &arg_4, const T_5 &arg_5, const T_6 &arg_6)
    {
        return write(arg_0) && write(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6);
    }

    template <typename T_0, typename T_1, typename T_2, typename T_3, typename T_4, typename T_5, typename T_6, typename T_7>
    bool write(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2, const T_3 &arg_3, const T_4 &arg_4, const T_5 &arg_5, const T_6 &arg_6, const T_7 &arg_7)
    {
        return write(arg_0) && write(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7);
    }


    template <typename T_0, typename T_1>
    bool write_ln(const T_0 &arg_0, const T_1 &arg_1)
    {
        return write(arg_0) && write_ln(arg_1);
    }

    template <typename T_0, typename T_1, typename T_2>
    bool write_ln(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2)
    {
        return write(arg_0) && write_ln(arg_1, arg_2);
    }

    template <typename T_0, typename T_1, typename T_2, typename T_3>
    bool write_ln(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2, const T_3 &arg_3)
    {
        return write(arg_0) && write_ln(arg_1, arg_2, arg_3);
    }

    template <typename T_0, typename T_1, typename T_2, typename T_3, typename T_4>
    bool write_ln(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2, const T_3 &arg_3, const T_4 &arg_4)
    {
        return write(arg_0) && write_ln(arg_1, arg_2, arg_3, arg_4);
    }

    template <typename T_0, typename T_1, typename T_2, typename T_3, typename T_4, typename T_5>
    bool write_ln(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2, const T_3 &arg_3, const T_4 &arg_4, const T_5 &arg_5)
    {
        return write(arg_0) && write_ln(arg_1, arg_2, arg_3, arg_4, arg_5);
    }

    template <typename T_0, typename T_1, typename T_2, typename T_3, typename T_4, typename T_5, typename T_6>
    bool write_ln(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2, const T_3 &arg_3, const T_4 &arg_4, const T_5 &arg_5, const T_6 &arg_6)
    {
        return write(arg_0) && write_ln(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6);
    }

    template <typename T_0, typename T_1, typename T_2, typename T_3, typename T_4, typename T_5, typename T_6, typename T_7>
    bool write_ln(const T_0 &arg_0, const T_1 &arg_1, const T_2 &arg_2, const T_3 &arg_3, const T_4 &arg_4, const T_5 &arg_5, const T_6 &arg_6, const T_7 &arg_7)
    {
        return write(arg_0) && write_ln(arg_1, arg_2, arg_3, arg_4, arg_5, arg_6, arg_7);
    }

#endif /* __cpp_variadic_templates */


    template <typename InputIt>
    bool row_write(InputIt first, InputIt last)
    {
        while (first != last) {
            *this << *first;
            ++first;
        }
        return true;
    }

    template <typename InputIt>
    bool row_write_ln(InputIt first, InputIt last)
    {
        while (first != last) {
            *this << *first;
            ++first;
        }
        ft_ln(table);
        return true;
    }

    /**
     * Set min width for the specified cell of the table.
     *
     * @param row
     *   Cell row.
     * @param col
     *   Cell column.
     * @param value
     *   Value of the min width.
     * @return
     *   - 0: Success; cell option was changed.
     *   - (<0): In case of error
     */
    bool set_cell_min_width(size_t row, size_t col, unsigned value)
    {
        return FT_IS_SUCCESS(ft_set_cell_option(table, row, col, FT_COPT_MIN_WIDTH, value));
    }

    /**
     * Set text alignment for the specified cell of the table.
     *
     * @param row
     *   Cell row.
     * @param col
     *   Cell column.
     * @param value
     *   Value of the text alignment.
     * @return
     *   - 0: Success; cell option was changed.
     *   - (<0): In case of error
     */
    bool set_cell_text_align(size_t row, size_t col, enum TextAlign value)
    {
        return FT_IS_SUCCESS(ft_set_cell_option(table, row, col, FT_COPT_TEXT_ALIGN, static_cast<int>(value)));
    }

    /**
     * Set top padding for the specified cell of the table.
     *
     * @param row
     *   Cell row.
     * @param col
     *   Cell column.
     * @param value
     *   Value of the top padding.
     * @return
     *   - 0: Success; cell option was changed.
     *   - (<0): In case of error
     */
    bool set_cell_top_padding(size_t row, size_t col, unsigned value)
    {
        return FT_IS_SUCCESS(ft_set_cell_option(table, row, col, FT_COPT_TOP_PADDING, value));
    }

    /**
     * Set bottom padding for the specified cell of the table.
     *
     * @param row
     *   Cell row.
     * @param col
     *   Cell column.
     * @param value
     *   Value of the bottom padding.
     * @return
     *   - 0: Success; cell option was changed.
     *   - (<0): In case of error
     */
    bool set_cell_bottom_padding(size_t row, size_t col, unsigned value)
    {
        return FT_IS_SUCCESS(ft_set_cell_option(table, row, col, FT_COPT_BOTTOM_PADDING, value));
    }

    /**
     * Set left padding for the specified cell of the table.
     *
     * @param row
     *   Cell row.
     * @param col
     *   Cell column.
     * @param value
     *   Value of the left padding.
     * @return
     *   - 0: Success; cell option was changed.
     *   - (<0): In case of error
     */
    bool set_cell_left_padding(size_t row, size_t col, unsigned value)
    {
        return FT_IS_SUCCESS(ft_set_cell_option(table, row, col, FT_COPT_LEFT_PADDING, value));
    }

    /**
     * Set right padding for the specified cell of the table.
     *
     * @param row
     *   Cell row.
     * @param col
     *   Cell column.
     * @param value
     *   Value of the left padding.
     * @return
     *   - 0: Success; cell option was changed.
     *   - (<0): In case of error
     */
    bool set_cell_right_padding(size_t row, size_t col, unsigned value)
    {
        return FT_IS_SUCCESS(ft_set_cell_option(table, row, col, FT_COPT_RIGHT_PADDING, value));
    }

    /**
     * Set empty string height for the specified cell of the table.
     *
     * @param row
     *   Cell row.
     * @param col
     *   Cell column.
     * @param value
     *   Value of the empty string height.
     * @return
     *   - 0: Success; cell option was changed.
     *   - (<0): In case of error
     */
    bool set_cell_empty_str_height(size_t row, size_t col, unsigned value)
    {
        return FT_IS_SUCCESS(ft_set_cell_option(table, row, col, FT_COPT_EMPTY_STR_HEIGHT, value));
    }

    /**
     * Set row type for the specified cell of the table.
     *
     * @param row
     *   Cell row.
     * @param col
     *   Cell column.
     * @param value
     *   Value of the row type.
     * @return
     *   - 0: Success; cell option was changed.
     *   - (<0): In case of error
     */
    bool set_cell_row_type(size_t row, size_t col, enum RowType value)
    {
        return FT_IS_SUCCESS(ft_set_cell_option(table, row, col, FT_COPT_ROW_TYPE, static_cast<int>(value)));
    }

    template <CellOption option>
    bool set_option(size_t row, size_t col, unsigned value);

    template <CellOption option>
    bool set_option(size_t row, size_t col, enum TextAlign align);

    template <CellOption option>
    bool set_option(size_t row, size_t col, enum RowType rowType);

    template <TableOption option>
    bool set_option(unsigned value);

    /**
     * Set border style for the table.
     *
     * @param style
     *   Pointer to border style.
     * @return
     *   - True: Success; table border style was changed.
     *   - False: Error
     */
    bool set_border_style(struct ft_border_style *style)
    {
        return FT_IS_SUCCESS(ft_set_border_style(table, style));
    }

    /**
     * Set default border style for all new formatted tables.
     *
     * @param style
     *   Pointer to border style.
     * @return
     *   - True: Success; table border style was changed.
     *   - False: Error
     */
    bool set_default_border_style(struct ft_border_style *style)
    {
        return FT_IS_SUCCESS(ft_set_default_border_style(style));
    }
private:
    ft_table_t *table;
    std::stringstream stream;
};






/*
 * Declare specializations for set_option functions
 */
#define DECLARE_SPECS_FOR_CELL_OPTIONS_X \
        SET_CELL_OPTION_SPEC(CellOption::MinWidth, FT_COPT_MIN_WIDTH, unsigned) \
        SET_CELL_OPTION_SPEC(CellOption::TextAlign, FT_COPT_TEXT_ALIGN, TextAlign) \
        SET_CELL_OPTION_SPEC(CellOption::TopPadding, FT_COPT_TOP_PADDING, unsigned) \
        SET_CELL_OPTION_SPEC(CellOption::BottomPadding, FT_COPT_BOTTOM_PADDING, unsigned) \
        SET_CELL_OPTION_SPEC(CellOption::LeftPadding, FT_COPT_LEFT_PADDING, unsigned) \
        SET_CELL_OPTION_SPEC(CellOption::RightPading, FT_COPT_RIGHT_PADDING, unsigned) \
        SET_CELL_OPTION_SPEC(CellOption::EmptyStrHeight, FT_COPT_EMPTY_STR_HEIGHT, unsigned) \
        SET_CELL_OPTION_SPEC(CellOption::RowType, FT_COPT_ROW_TYPE, RowType)

#define SET_CELL_OPTION_SPEC(CELL_OPTION, C_OPTION, VALUE_TYPE) \
template <> \
bool Table::set_option<CELL_OPTION>(size_t row, size_t col, VALUE_TYPE value) \
{ \
    return FT_IS_SUCCESS(ft_set_cell_option(table, row, col, C_OPTION, static_cast<int>(value))); \
}

DECLARE_SPECS_FOR_CELL_OPTIONS_X

#undef SET_TABLE_OPTION_SPEC
#undef DECLARE_SPECS_FOR_CELL_OPTIONS_X



#define DECLARE_SPECS_FOR_TABLE_OPTIONS_X \
        SET_TABLE_OPTION_SPEC(TableOption::LeftMargin, FT_TOPT_LEFT_MARGIN) \
        SET_TABLE_OPTION_SPEC(TableOption::TopMargin, FT_TOPT_TOP_MARGIN) \
        SET_TABLE_OPTION_SPEC(TableOption::RightMargin, FT_TOPT_RIGHT_MARGIN) \
        SET_TABLE_OPTION_SPEC(TableOption::BottomMargin, FT_TOPT_BOTTOM_MARGIN)

#define SET_TABLE_OPTION_SPEC(TABLE_OPTION, TBL_OPTION) \
template <> \
bool Table::set_option<TABLE_OPTION>(unsigned value) \
{ \
    return FT_IS_SUCCESS(ft_set_tbl_option(table, TBL_OPTION, static_cast<int>(value))); \
}

DECLARE_SPECS_FOR_TABLE_OPTIONS_X

#undef SET_TABLE_OPTION_SPEC
#undef DECLARE_SPECS_FOR_TABLE_OPTIONS_X




}

#endif // LIBFORT_HPP
