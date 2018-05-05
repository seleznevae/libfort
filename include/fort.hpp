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
const TableManipulator endl(1);
const TableManipulator separator(2);

/**
 * Table - here is a short description.
 *
 * Here is detailed description.
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

    std::string to_string() const
    {
        const char *str = ft_to_string(table);
        if (str == NULL)
            throw std::runtime_error("Runtime error");
        return str;
    }

    const char *c_str() const
    {
        return ft_to_string(table);
    }

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
        else if (arg.value == endl.value)
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


private:
    FTABLE *table;
    std::stringstream stream;
};

}

#endif // LIBFORT_HPP
