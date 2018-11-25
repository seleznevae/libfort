# LIBFORT 

These pages contain the API documentation of **libfort** - simple library to create formatted tables for console applications.

- @link fort.h `libfort C API` @endlink
  - Functions
    - Table lifetime management
        - @link ft_create_table ft_create_table @endlink -- create table
        - @link ft_destroy_table ft_destroy_table @endlink -- destroy table
    - Table navigation
        - @link ft_set_cur_cell ft_set_cur_cell @endlink -- set current cell
        - @link ft_ln ft_ln @endlink -- move current position to the first cell of the next row
        - @link ft_cur_row ft_cur_row @endlink -- get current row
        - @link ft_cur_col ft_cur_col @endlink -- get current column
    - Fill table with content
        - @link ft_printf ft_printf @endlink -- fill cells according to the format string
        - @link ft_printf_ln ft_printf_ln @endlink -- fill cells according to the format string and move to the next line
        - @link ft_write ft_write @endlink -- fill cells with the specified strings
        - @link ft_write ft_write_ln @endlink -- fill cells with the specified strings  and move to the next line
        - @link ft_nwrite ft_nwrite @endlink -- fill cells with the specified strings
        - @link ft_nwrite ft_nwrite_ln @endlink -- fill cells with the specified strings and move to the next line
        - @link ft_row_write ft_row_write @endlink -- fill cells with the strings from the array
        - @link ft_row_write_ln ft_row_write_ln @endlink -- fill cells with the strings from the array and move to the next line
        - @link ft_table_write ft_table_write @endlink -- fill cells with the strings from the 2D array
        - @link ft_table_write_ln ft_table_write_ln @endlink -- fill cells with the strings from the 2D array and move to the next line
        - @link ft_add_separator ft_add_separator @endlink -- add horizontal separator
    - Modify appearance of the table
        - @link ft_set_default_border_style ft_set_default_border_style @endlink -- set default border style for all new created tables
        - @link ft_set_border_style ft_set_border_style @endlink -- modify border style of the table
        - @link ft_set_default_cell_option ft_set_default_cell_option @endlink -- set default cell option for all new created tables
        - @link ft_set_cell_option ft_set_cell_option @endlink -- set cell option for the table
        - @link ft_set_default_tbl_option ft_set_default_tbl_option @endlink -- set default table option for all new created tables
        - @link ft_set_tbl_option ft_set_tbl_option @endlink -- set table option for the table
        - @link ft_set_cell_span ft_set_cell_span @endlink -- set cell span
    - Others
        - @link ft_set_memory_funcs ft_set_memory_funcs @endlink -- set memory allocation functions for the library
  - Data structures and types
    - @link ft_table_t ft_table_t @endlink -- table handler
    - @link ft_border_chars ft_border_chars @endlink -- structure describing border appearance
    - @link ft_text_alignment ft_text_alignment @endlink -- alignment of cell content
    - @link ft_row_type ft_row_type @endlink -- alignment of cell content




```text
          FT_BASIC_STYLE                                                    FT_SIMPLE_STYLE

+------+--------------------------+------+--------+                                                             
| Rank | Title                    | Year | Rating |            Rank   Title                      Year   Rating  
+------+--------------------------+------+--------+           ------ -------------------------- ------ -------- 
|  1   | The Shawshank Redemption | 1994 | 9.5    |             1     The Shawshank Redemption   1994   9.5     
|  2   | 12 Angry Men             | 1957 | 8.8    |             2     12 Angry Men               1957   8.8     
|  3   | It's a Wonderful Life    | 1946 | 8.6    |             3     It's a Wonderful Life      1946   8.6     
+------+--------------------------+------+--------+           ------ -------------------------- ------ -------- 
|  4   | 2001: A Space Odyssey    | 1968 | 8.5    |             4     2001: A Space Odyssey      1968   8.5     
|  5   | Blade Runner             | 1982 | 8.1    |             5     Blade Runner               1982   8.1     
+------+--------------------------+------+--------+          
```
```text
             FT_PLAIN_STYLE                                                    FT_DOT_STYLE

 -------------------------------------------------              
  Rank   Title                      Year   Rating               
 -------------------------------------------------              
   1     The Shawshank Redemption   1994   9.5                  
   2     12 Angry Men               1957   8.8                  
   3     It's a Wonderful Life      1946   8.6                  
 -------------------------------------------------              
   4     2001: A Space Odyssey      1968   8.5                  
   5     Blade Runner               1982   8.1                 

```


        

@author Seleznev Anton
@see https://github.com/seleznevae/libfort to download the source code