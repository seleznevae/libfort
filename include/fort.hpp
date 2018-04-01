#ifndef LIBFORT_HPP
#define LIBFORT_HPP

#include <string>
#include <stdexcept>
#include <sstream>
#include "fort.h"

namespace fort
{

class FTableManipulator {
public:
    explicit FTableManipulator(int i)
        :value(i)
    {
    }
    friend class FTable;
private:
    int value;
};

const FTableManipulator header(0);
const FTableManipulator endl(1);
const FTableManipulator separator(2);

class FTable {
public:
    FTable()
        :table(ft_create_table())
    {
        if (table == NULL)
            throw std::runtime_error("Runtime error");
    }

    ~FTable()
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
    FTable &operator<<(const T &arg)
    {
        std::stringstream stream;
        stream << arg;
        ft_nwrite(table, 1, stream.str().c_str());
        return *this;
    }

    FTable &operator<<(const FTableManipulator &arg)
    {
        if (arg.value == header.value)
            ft_set_cell_option(table, FT_CUR_ROW, FT_ANY_ROW, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
        else if (arg.value == endl.value)
            ft_ln(table);
        else if (arg.value == separator.value)
            ft_add_separator(table);
        return *this;
    }



private:
    FTABLE *table;
};

}

#endif // LIBFORT_HPP
