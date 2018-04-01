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

#ifndef LIBFORT_H
#define LIBFORT_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>


/*
 * Determine compiler
 */
#if defined(__clang__)
#define FT_CLANG_COMPILER
#elif defined(__GNUC__)
#define FT_GCC_COMPILER
#elif defined(_MSC_VER)
#define FT_MICROSOFT_COMPILER
#else
#define FT_UNDEFINED_COMPILER
#endif



/*
 * Declare restrict
 */
/*
#if defined(__cplusplus)
#if defined(FT_CLANG_COMPILER)
#define FT_RESTRICT __restrict__
#else
#define FT_RESTRICT __restrict
#endif // if defined(FT_CLANG_COMPILER)
#else
#if __STDC_VERSION__ < 199901L
#define FT_RESTRICT
#else
#define FT_RESTRICT restrict
#endif // __STDC_VERSION__ < 199901L
#endif // if defined(__cplusplus)
*/


/*
 * Declare restrict
 */
#if defined(__cplusplus)
#define FT_INLINE inline
#else
#define FT_INLINE __inline
#endif /* if defined(__cplusplus) */



/*
 * Attribute format for argument checking
 */
#if defined(FT_CLANG_COMPILER) || defined(FT_GCC_COMPILER)
#define FT_PRINTF_ATTRIBUTE_FORMAT(string_index, first_to_check) \
    __attribute__ ((format (printf, string_index, first_to_check)))
#else
#define FT_PRINTF_ATTRIBUTE_FORMAT(string_index, first_to_check)
#endif /* defined(FT_CLANG_COMPILER) || defined(FT_GCC_COMPILER) */


/*
 *  C++ needs to know that types and declarations are C, not C++.
 */
#ifdef __cplusplus
# define FT_BEGIN_DECLS extern "C" {
# define FT_END_DECLS }
#else
# define FT_BEGIN_DECLS
# define FT_END_DECLS
#endif


#ifndef FT_EXTERN
#define FT_EXTERN extern
#endif



/*
 *  Wchar support
 */
#define FT_HAVE_WCHAR

/*
 * Helper macros
 */

#define STR_2_CAT_(arg1, arg2) \
    arg1##arg2
#define STR_2_CAT(arg1, arg2) \
    STR_2_CAT_(arg1, arg2)


static FT_INLINE int ft_check_if_string_helper(const char *str)
{
    (void)str;
    return 0;
}

static FT_INLINE int ft_check_if_wstring_helper(const wchar_t *str)
{
    (void)str;
    return 0;
}

#define FORT_NARGS_IMPL_(x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15,N,...) N
#define FT_EXPAND_(x) x
#define PP_NARG(...) \
    FT_EXPAND_(FORT_NARGS_IMPL_(__VA_ARGS__,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))

#define CHECK_IF_STRING_32(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_31(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_31(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_30(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_30(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_29(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_29(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_28(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_28(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_27(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_27(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_26(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_26(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_25(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_25(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_24(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_24(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_23(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_23(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_22(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_22(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_21(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_21(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_20(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_20(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_19(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_19(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_18(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_18(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_17(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_17(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_16(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_16(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_15(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_15(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_14(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_14(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_13(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_13(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_12(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_12(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_11(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_11(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_10(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_10(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_9(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_9(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_8(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_8(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_7(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_7(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_6(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_6(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_5(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_5(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_4(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_4(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_3(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_3(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_2(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_2(checker,arg,...) (checker(arg),FT_EXPAND_(CHECK_IF_STRING_1(checker,__VA_ARGS__)))
#define CHECK_IF_STRING_1(checker,arg) (checker(arg))

#define CHECK_IF_ARGS_ARE_STRINGS__(checker,func, ...) \
    FT_EXPAND_(func(checker,__VA_ARGS__))
#define CHECK_IF_ARGS_ARE_STRINGS_(checker,basis, n, ...) \
    CHECK_IF_ARGS_ARE_STRINGS__(checker,STR_2_CAT_(basis, n), __VA_ARGS__)
#define CHECK_IF_ARGS_ARE_STRINGS(...) \
    CHECK_IF_ARGS_ARE_STRINGS_(ft_check_if_string_helper,CHECK_IF_STRING_,PP_NARG(__VA_ARGS__), __VA_ARGS__)
#ifdef FT_HAVE_WCHAR
#define CHECK_IF_ARGS_ARE_WSTRINGS(...) \
    CHECK_IF_ARGS_ARE_STRINGS_(ft_check_if_wstring_helper,CHECK_IF_STRING_,PP_NARG(__VA_ARGS__), __VA_ARGS__)
#endif

/*
 * libfort structures and functions declarations
 */

FT_BEGIN_DECLS

struct fort_table;
typedef struct fort_table FTABLE;

/**
 * Create formatted table.
 *
 * @return
 *   The pointer to the new allocated FTABLE, on success. NULL on error
 *   with ft_errno set appropriately.
 */
FT_EXTERN FTABLE *ft_create_table(void);

/**
 * Destroy formatted table.
 *
 * Destroy formatted table and free all resources allocated during table creation
 * and work with it.
 *
 * @param table
 *   Pointer to formatted table previousley created with ft_create_table.
 */
FT_EXTERN void ft_destroy_table(FTABLE *table);

/**
 * Move current position to the first cell of the next line(row).
 *
 * @param table
 *   Pointer to formatted table.
 */
FT_EXTERN void ft_ln(FTABLE *table);
FT_EXTERN size_t ft_cur_row(FTABLE *table);
FT_EXTERN size_t ft_cur_col(FTABLE *table);


#if defined(FT_CLANG_COMPILER) || defined(FT_GCC_COMPILER)
FT_EXTERN int ft_printf(FTABLE *table, const char *fmt, ...) FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
FT_EXTERN int ft_printf_ln(FTABLE *table, const char *fmt, ...) FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);

#else

FT_EXTERN int ft_printf_impl(FTABLE *table, const char *fmt, ...) FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
FT_EXTERN int ft_printf_ln_impl(FTABLE *table, const char *fmt, ...) FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);

#define ft_printf(table, ...) \
    (( 0 ? fprintf(stderr, __VA_ARGS__) : 1), ft_printf_impl(table, __VA_ARGS__))
#define ft_printf_ln(table, ...) \
    (( 0 ? fprintf(stderr, __VA_ARGS__) : 1), ft_printf_ln_impl(table, __VA_ARGS__))
#endif



FT_EXTERN int ft_write(FTABLE *table, const char *cell_content);
FT_EXTERN int ft_write_ln(FTABLE *table, const char *cell_content);

#define FT_NWRITE(table, ...)\
    (0 ? CHECK_IF_ARGS_ARE_STRINGS(__VA_ARGS__) : ft_nwrite(table, PP_NARG(__VA_ARGS__), __VA_ARGS__))
#define FT_NWRITE_LN(table, ...)\
    (0 ? CHECK_IF_ARGS_ARE_STRINGS(__VA_ARGS__) : ft_nwrite_ln(table, PP_NARG(__VA_ARGS__), __VA_ARGS__))
FT_EXTERN int ft_nwrite(FTABLE *table, size_t n, const char *cell_content, ...);
FT_EXTERN int ft_nwrite_ln(FTABLE *table, size_t n, const char *cell_content, ...);






FT_EXTERN int ft_row_write(FTABLE *table, size_t cols, const char *row_cells[]);
FT_EXTERN int ft_row_write_ln(FTABLE *table, size_t cols, const char *row_cells[]);

#if !defined(__cplusplus) && !defined(FT_MICROSOFT_COMPILER)
FT_EXTERN int ft_s_table_write(FTABLE *table, size_t rows, size_t cols, const char *table_cells[rows][cols]);
FT_EXTERN int ft_s_table_write_ln(FTABLE *table, size_t rows, size_t cols, const char *table_cells[rows][cols]);

FT_EXTERN int ft_table_write(FTABLE *table, size_t rows, size_t cols, const char * *  table_cells[rows]);
FT_EXTERN int ft_table_write_ln(FTABLE *table, size_t rows, size_t cols, const char * *  table_cells[rows]);
#endif




/**
 * Add separator after the current row.
 *
 * @param table
 *   Formatted table.
 * @return
 *   - 0: Success; separator was added.
 *   - (-1): !!!!!!!!  todo
 */
FT_EXTERN int ft_add_separator(FTABLE *table);




/**
 * Convert table to string representation.
 *
 * FTABLE has ownership of the returned pointer. So there is no need to
 * free it. To take ownership user should explicitly copy the returned
 * string with strdup or similar functions. Returned pointer remaines valid
 * until table is destroyed with ft_destroy_table or other invocations of
 * ft_to_string.
 *
 * @param table
 *   Formatted table.
 * @return
 *   The pointer to the string representation of formatted table, on success.
 *   NULL on error with ft_errno set appropriately.
 */
FT_EXTERN const char *ft_to_string(const FTABLE *table);














struct ft_border_chars {
    char top_border_ch;
    char separator_ch;
    char bottom_border_ch;
    char side_border_ch;
    char out_intersect_ch;
    char in_intersect_ch;
};

struct ft_border_style {
    struct ft_border_chars border_chs;
    struct ft_border_chars header_border_chs;
    char hor_separator_char;
};

/**
 *  Built-in table border styles
 */
extern struct ft_border_style *FT_BASIC_STYLE;
extern struct ft_border_style *FT_SIMPLE_STYLE;
extern struct ft_border_style *FT_PLAIN_STYLE;
extern struct ft_border_style *FT_DOT_STYLE;
extern struct ft_border_style *FT_EMPTY_STYLE;

/**
 * Set default border style for all new formatted tables.
 *
 * @param style
 *   Pointer to border style
 * @return
 *   - 0: Success; default border style was changed.
 *   - (-1): !!!!!!!!  todo
 */
FT_EXTERN int ft_set_default_border_style(struct ft_border_style *style);

/**
 * Set border style for the table.
 *
 * @param table
 *   A pointer to the FTABLE structure
 * @param style
 *   Pointer to border style
 * @return
 *   - 0: Success; table border style was changed.
 *   - (-1): !!!!!!!!  todo
 */
FT_EXTERN int ft_set_border_style(FTABLE *table, struct ft_border_style *style);

/**
 *  Special macros to define cell position (row and column)
 */
#define FT_ANY_COLUMN    (UINT_MAX)
#define FT_CUR_COLUMN    (UINT_MAX - 1)
#define FT_ANY_ROW       (UINT_MAX)
#define FT_CUR_ROW       (UINT_MAX - 1)

/**
 *  Cell options
 */
#define FT_COPT_MIN_WIDTH        (0x01U << 0) /**< Minimum width */
#define FT_COPT_TEXT_ALIGN       (0x01U << 1) /**< Text alignmemnt */
#define FT_COPT_TOP_PADDING      (0x01U << 2) /**< Top padding for cell content */
#define FT_COPT_BOTTOM_PADDING   (0x01U << 3) /**< Bottom padding for cell content */
#define FT_COPT_LEFT_PADDING     (0x01U << 4) /**< Left padding for cell content */
#define FT_COPT_RIGHT_PADDING    (0x01U << 5) /**< Right padding for cell content */
#define FT_COPT_EMPTY_STR_HEIGHT (0x01U << 6) /**< Height of empty cell */
#define FT_COPT_ROW_TYPE         (0x01U << 7) /**< Row type */

/**
 *  Alignment of cell content
 */
enum ft_text_alignment {
    FT_ALIGNED_LEFT,
    FT_ALIGNED_CENTER,
    FT_ALIGNED_RIGHT
};

/**
 *  Type of table row
 */
enum ft_row_type {
    FT_ROW_COMMON,
    FT_ROW_HEADER
};

FT_EXTERN int ft_set_default_cell_option(uint32_t option, int value);
FT_EXTERN int ft_set_cell_option(FTABLE *table, unsigned row, unsigned col, uint32_t option, int value);

/**
 *  Table options
 */
#define FT_TOPT_LEFT_MARGIN   (0x01U << 0)
#define FT_TOPT_TOP_MARGIN    (0x01U << 1)
#define FT_TOPT_RIGHT_MARGIN  (0x01U << 2)
#define FT_TOPT_BOTTOM_MARGIN (0x01U << 3)

FT_EXTERN int ft_set_default_tbl_option(uint32_t option, int value);
FT_EXTERN int ft_set_tbl_option(FTABLE *table, uint32_t option, int value);




/*
 * WChar support
 */

#ifdef FT_HAVE_WCHAR

FT_EXTERN int ft_wwrite(FTABLE *table, const wchar_t *cell_content);
FT_EXTERN int ft_wwrite_ln(FTABLE *table, const wchar_t *cell_content);

#define FT_NWWRITE(table, ...)\
    (0 ? CHECK_IF_ARGS_ARE_WSTRINGS(__VA_ARGS__) : ft_nwwrite(table, PP_NARG(__VA_ARGS__), __VA_ARGS__))
#define FT_NWWRITE_LN(table, ...)\
    (0 ? CHECK_IF_ARGS_ARE_WSTRINGS(__VA_ARGS__) : ft_nwwrite_ln(table, PP_NARG(__VA_ARGS__), __VA_ARGS__))
FT_EXTERN int ft_nwwrite(FTABLE *table, size_t n, const wchar_t *cell_content, ...);
FT_EXTERN int ft_nwwrite_ln(FTABLE *table, size_t n, const wchar_t *cell_content, ...);

FT_EXTERN int ft_row_wwrite(FTABLE *table, size_t cols, const wchar_t *row_cells[]);
FT_EXTERN int ft_row_wwrite_ln(FTABLE *table, size_t cols, const wchar_t *row_cells[]);

FT_EXTERN const wchar_t *ft_to_wstring(const FTABLE *table);
#endif



FT_END_DECLS

#endif /* LIBFORT_H */
