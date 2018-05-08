# libfort (Library to create FORmatted Tables)

[![Build Status](https://travis-ci.org/seleznevae/libfort.svg?branch=master)](https://travis-ci.org/seleznevae/libfort)
[![Build status](https://ci.appveyor.com/api/projects/status/ll1qygb56pho95xw/branch/master?svg=true)](https://ci.appveyor.com/project/seleznevae/libfort/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/seleznevae/libfort/badge.svg?branch=master)](https://coveralls.io/github/seleznevae/libfort?branch=master)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)


**libfort** - a simple library to create formatted ASCII tables.

**Features:**
- Easy to integrate (only 2 files: **fort.c** and **fort.h** from **lib** direrctory)
- Customization of appearance (various border styles and row/column/cell options for indentation, alignment, padding)
- A number of functions to fill the table (add content by adding separate cells, rows or use _printf_ like functions)
- Support of multiple lines in cells
- Support of wide characters

## Examples

### Basic using

```C
#include <stdio.h>
#include "fort.h"
int main(void)
{
    ft_table_t *table = ft_create_table();
    /* Set "header" type for the first row */
    ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "N", "Planet", "Speed, km/s");

    ft_write_ln(table, "1", "Mercury", "47.362");
    ft_write_ln(table, "2", "Venus", "35.02");
    ft_write_ln(table, "3", "Earth", "29.78");

    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);
}
```
Output:
```text
+---+---------+-------------+
| N | Planet  | Speed, km/s |
+---+---------+-------------+
| 1 | Mercury | 47.362      |
| 2 | Venus   | 35.02       |
| 3 | Earth   | 29.78       |
+---+---------+-------------+
```


### Customize table appearance

```C
#include <stdio.h>
#include "fort.h"
int main(void)
{
    ft_table_t *table = ft_create_table();
    ft_set_border_style(table, FT_DOUBLE2_STYLE);
    /* Set center alignment for the 0th column */
    ft_set_cell_option(table, FT_ANY_ROW, 0, FT_COPT_TEXT_ALIGN, FT_ALIGNED_CENTER);
    /* Set center alignment for the 3rd column */
    ft_set_cell_option(table, FT_ANY_ROW, 3, FT_COPT_TEXT_ALIGN, FT_ALIGNED_CENTER);

    /* Set "header" type for the first row */
    ft_set_cell_option(table, 0, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);
    ft_write_ln(table, "Rank", "Title", "Year", "Rating");

    ft_write_ln(table, "1", "The Shawshank Redemption", "1994", "9.5");
    ft_write_ln(table, "2", "12 Angry Men", "1957", "8.8");
    ft_write_ln(table, "3", "2001: A Space Odyssey", "1968", "8.5");
    ft_write_ln(table, "4", "Blade Runner", "1982", "8.1");

    ft_write_ln(table, "Average", "", "", "8.7");
    ft_set_cell_span(table, 5, 0, 3);
    ft_set_cell_option(table, 5, FT_ANY_COLUMN, FT_COPT_ROW_TYPE, FT_ROW_HEADER);

    printf("%s\n", ft_to_string(table));
    ft_destroy_table(table);
}
```
Output:
```text
╔══════╤══════════════════════════╤══════╤════════╗
║ Rank │ Title                    │ Year │ Rating ║
╠══════╪══════════════════════════╪══════╪════════╣
║  1   │ The Shawshank Redemption │ 1994 │  9.5   ║
╟──────┼──────────────────────────┼──────┼────────╢
║  2   │ 12 Angry Men             │ 1957 │  8.8   ║
╟──────┼──────────────────────────┼──────┼────────╢
║  3   │ 2001: A Space Odyssey    │ 1968 │  8.5   ║
╟──────┼──────────────────────────┼──────┼────────╢
║  4   │ Blade Runner             │ 1982 │  8.1   ║
╠══════╧══════════════════════════╧══════╪════════╣
║                Average                 │  8.7   ║
╚════════════════════════════════════════╧════════╝
```


## Installation

Copy 2 files **fort.c** and **fort.h** from [**lib**](https://github.com/seleznevae/libfort/tree/master/lib) direrctory to your project. Include **fort.h** in all sources where you will use libfort and compile with **fort.c**.


## Supported platforms and compilers

The following compilers are currently used in continuous integration at [Travis](https://travis-ci.org/seleznevae/libfort) and [AppVeyor](https://ci.appveyor.com/project/seleznevae/libfort):

| Compiler           | Operating System             |
|--------------------|------------------------------|
| GCC 4.8.4          | Ubuntu 14.04.3               | 
| Clang 5.0.0        | Ubuntu 14.04.3               | 
| Visual Studio 2017?| Windows Server 2016         ?|

## License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

The class is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright &copy; 2017 - 2018 Seleznev Anton

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

