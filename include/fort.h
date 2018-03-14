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


#include "options.h"

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
#if defined(__cplusplus)
#if defined(FT_CLANG_COMPILER)
#define FT_RESTRICT __restrict__
#else
#define FT_RESTRICT __restrict
#endif /* if defined(FT_CLANG_COMPILER) */
#else
#if __STDC_VERSION__ < 199901L
#define FT_RESTRICT
#else
#define FT_RESTRICT restrict
#endif /* __STDC_VERSION__ < 199901L */
#endif /* if defined(__cplusplus) */

/* #define FT_RESTRICT*/


/*
 * Declare restrict
 */
#if defined(__cplusplus)
#define FT_INLINE inline
#else
#define FT_INLINE __inline
#endif /* if defined(__cplusplus) */

/* #define FT_RESTRICT*/


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
#ifdef	__cplusplus
# define FT_BEGIN_DECLS	extern "C" {
# define FT_END_DECLS	}
#else
# define FT_BEGIN_DECLS
# define FT_END_DECLS
#endif


#ifndef FT_EXTERN
#define FT_EXTERN extern
#endif


#define STRING2(x) #x
#define STRING(x) STRING2(x)


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


static FT_INLINE int ft_check_if_string_helper(const char*str)
{
    (void)str;
    return 0;
}

static FT_INLINE int ft_check_if_wstring_helper(const wchar_t*str)
{
    (void)str;
    return 0;
}

#define PP_ARG_N( \
     _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
    _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
    _31, N, ...) N
#define PP_RSEQ_N() \
    31,30, \
    29,28,27,26,25,24,23,22,21,20, \
    19,18,17,16,15,14,13,12,11,10, \
     9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define PP_NARG_(...) \
    PP_ARG_N(__VA_ARGS__)
#define PP_NARG(...) \
    PP_NARG_(__VA_ARGS__,PP_RSEQ_N())



#define CHECK_IF_STRING_32(checker,arg,...) (checker(arg),CHECK_IF_STRING_31(checker,__VA_ARGS__))
#define CHECK_IF_STRING_31(checker,arg,...) (checker(arg),CHECK_IF_STRING_30(checker,__VA_ARGS__))
#define CHECK_IF_STRING_30(checker,arg,...) (checker(arg),CHECK_IF_STRING_29(checker,__VA_ARGS__))
#define CHECK_IF_STRING_29(checker,arg,...) (checker(arg),CHECK_IF_STRING_28(checker,__VA_ARGS__))
#define CHECK_IF_STRING_28(checker,arg,...) (checker(arg),CHECK_IF_STRING_27(checker,__VA_ARGS__))
#define CHECK_IF_STRING_27(checker,arg,...) (checker(arg),CHECK_IF_STRING_26(checker,__VA_ARGS__))
#define CHECK_IF_STRING_26(checker,arg,...) (checker(arg),CHECK_IF_STRING_25(checker,__VA_ARGS__))
#define CHECK_IF_STRING_25(checker,arg,...) (checker(arg),CHECK_IF_STRING_24(checker,__VA_ARGS__))
#define CHECK_IF_STRING_24(checker,arg,...) (checker(arg),CHECK_IF_STRING_23(checker,__VA_ARGS__))
#define CHECK_IF_STRING_23(checker,arg,...) (checker(arg),CHECK_IF_STRING_22(checker,__VA_ARGS__))
#define CHECK_IF_STRING_22(checker,arg,...) (checker(arg),CHECK_IF_STRING_21(checker,__VA_ARGS__))
#define CHECK_IF_STRING_21(checker,arg,...) (checker(arg),CHECK_IF_STRING_20(checker,__VA_ARGS__))
#define CHECK_IF_STRING_20(checker,arg,...) (checker(arg),CHECK_IF_STRING_19(checker,__VA_ARGS__))
#define CHECK_IF_STRING_19(checker,arg,...) (checker(arg),CHECK_IF_STRING_18(checker,__VA_ARGS__))
#define CHECK_IF_STRING_18(checker,arg,...) (checker(arg),CHECK_IF_STRING_17(checker,__VA_ARGS__))
#define CHECK_IF_STRING_17(checker,arg,...) (checker(arg),CHECK_IF_STRING_16(checker,__VA_ARGS__))
#define CHECK_IF_STRING_16(checker,arg,...) (checker(arg),CHECK_IF_STRING_15(checker,__VA_ARGS__))
#define CHECK_IF_STRING_15(checker,arg,...) (checker(arg),CHECK_IF_STRING_14(checker,__VA_ARGS__))
#define CHECK_IF_STRING_14(checker,arg,...) (checker(arg),CHECK_IF_STRING_13(checker,__VA_ARGS__))
#define CHECK_IF_STRING_13(checker,arg,...) (checker(arg),CHECK_IF_STRING_12(checker,__VA_ARGS__))
#define CHECK_IF_STRING_12(checker,arg,...) (checker(arg),CHECK_IF_STRING_11(checker,__VA_ARGS__))
#define CHECK_IF_STRING_11(checker,arg,...) (checker(arg),CHECK_IF_STRING_10(checker,__VA_ARGS__))
#define CHECK_IF_STRING_10(checker,arg,...) (checker(arg),CHECK_IF_STRING_9(checker,__VA_ARGS__))
#define CHECK_IF_STRING_9(checker,arg,...) (checker(arg),CHECK_IF_STRING_8(checker,__VA_ARGS__))
#define CHECK_IF_STRING_8(checker,arg,...) (checker(arg),CHECK_IF_STRING_7(checker,__VA_ARGS__))
#define CHECK_IF_STRING_7(checker,arg,...) (checker(arg),CHECK_IF_STRING_6(checker,__VA_ARGS__))
#define CHECK_IF_STRING_6(checker,arg,...) (checker(arg),CHECK_IF_STRING_5(checker,__VA_ARGS__))
#define CHECK_IF_STRING_5(checker,arg,...) (checker(arg),CHECK_IF_STRING_4(checker,__VA_ARGS__))
#define CHECK_IF_STRING_4(checker,arg,...) (checker(arg),CHECK_IF_STRING_3(checker,__VA_ARGS__))
#define CHECK_IF_STRING_3(checker,arg,...) (checker(arg),CHECK_IF_STRING_2(checker,__VA_ARGS__))
#define CHECK_IF_STRING_2(checker,arg,...) (checker(arg),CHECK_IF_STRING_1(checker,__VA_ARGS__))
#define CHECK_IF_STRING_1(checker,arg) (checker(arg))

#define CHECK_IF_ARGS_ARE_STRINGS__(checker,func, ...) func(checker,__VA_ARGS__)
#define CHECK_IF_ARGS_ARE_STRINGS_(checker,basis, n, ...) CHECK_IF_ARGS_ARE_STRINGS__(checker,STR_2_CAT_(basis, n), __VA_ARGS__)
#define CHECK_IF_ARGS_ARE_STRINGS(...) CHECK_IF_ARGS_ARE_STRINGS_(ft_check_if_string_helper,CHECK_IF_STRING_,PP_NARG(__VA_ARGS__), __VA_ARGS__)
#ifdef FT_HAVE_WCHAR
#define CHECK_IF_ARGS_ARE_WSTRINGS(...) CHECK_IF_ARGS_ARE_STRINGS_(ft_check_if_wstring_helper,CHECK_IF_STRING_,PP_NARG(__VA_ARGS__), __VA_ARGS__)
#endif

/*
 * libfort structures and functions declarations
 */

FT_BEGIN_DECLS

struct fort_table;
typedef struct fort_table FTABLE;

FT_EXTERN FTABLE * ft_create_table(void);
FT_EXTERN void ft_destroy_table(FTABLE *FT_RESTRICT table);


FT_EXTERN void ft_ln(FTABLE *FT_RESTRICT table);


#if defined(FT_CLANG_COMPILER) || defined(FT_GCC_COMPILER)
FT_EXTERN int ft_printf(FTABLE *FT_RESTRICT table, const char* FT_RESTRICT fmt, ...) FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
FT_EXTERN int ft_printf_ln(FTABLE *FT_RESTRICT table, const char* FT_RESTRICT fmt, ...) FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);

#else

FT_EXTERN int ft_printf_impl(FTABLE *FT_RESTRICT table, const char* FT_RESTRICT fmt, ...) FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
FT_EXTERN int ft_printf_ln_impl(FTABLE *FT_RESTRICT table, const char* FT_RESTRICT fmt, ...) FT_PRINTF_ATTRIBUTE_FORMAT(2, 3);

#define ft_printf(table, ...) \
    (( 0 ? fprintf(stderr, __VA_ARGS__) : 1), ft_printf_impl(table, __VA_ARGS__))
#define ft_printf_ln(table, ...) \
    (( 0 ? fprintf(stderr, __VA_ARGS__) : 1), ft_printf_ln_impl(table, __VA_ARGS__))
#endif



FT_EXTERN int ft_write(FTABLE *FT_RESTRICT table, const char* FT_RESTRICT cell_content);
FT_EXTERN int ft_write_ln(FTABLE *FT_RESTRICT table, const char* FT_RESTRICT cell_content);

#define FT_NWRITE(table, ...)\
    (0 ? CHECK_IF_ARGS_ARE_STRINGS(__VA_ARGS__) : ft_nwrite(table, PP_NARG(__VA_ARGS__), __VA_ARGS__))
#define FT_NWRITE_LN(table, ...)\
    (0 ? CHECK_IF_ARGS_ARE_STRINGS(__VA_ARGS__) : ft_nwrite_ln(table, PP_NARG(__VA_ARGS__), __VA_ARGS__))
FT_EXTERN int ft_nwrite(FTABLE *FT_RESTRICT table, size_t n, const char* FT_RESTRICT cell_content, ...);
FT_EXTERN int ft_nwrite_ln(FTABLE *FT_RESTRICT table, size_t n, const char* FT_RESTRICT cell_content, ...);


#ifdef FT_HAVE_WCHAR
FT_EXTERN int ft_wwrite(FTABLE *FT_RESTRICT table, const wchar_t* FT_RESTRICT cell_content);
FT_EXTERN int ft_wwrite_ln(FTABLE *FT_RESTRICT table, const wchar_t* FT_RESTRICT cell_content);

#define FT_NWWRITE(table, ...)\
    (0 ? CHECK_IF_ARGS_ARE_WSTRINGS(__VA_ARGS__) : ft_nwwrite(table, PP_NARG(__VA_ARGS__), __VA_ARGS__))
#define FT_NWWRITE_LN(table, ...)\
    (0 ? CHECK_IF_ARGS_ARE_WSTRINGS(__VA_ARGS__) : ft_nwwrite_ln(table, PP_NARG(__VA_ARGS__), __VA_ARGS__))
FT_EXTERN int ft_nwwrite(FTABLE *FT_RESTRICT table, size_t n, const wchar_t* FT_RESTRICT cell_content, ...);
FT_EXTERN int ft_nwwrite_ln(FTABLE *FT_RESTRICT table, size_t n, const wchar_t* FT_RESTRICT cell_content, ...);
#endif



FT_EXTERN int ft_row_write(FTABLE *FT_RESTRICT table, size_t cols, const char* FT_RESTRICT row_cells[]);
FT_EXTERN int ft_row_write_ln(FTABLE *FT_RESTRICT table, size_t cols, const char* FT_RESTRICT row_cells[]);

FT_EXTERN int ft_s_table_write(FTABLE *FT_RESTRICT table, size_t rows, size_t cols, const char* FT_RESTRICT table_cells[rows][cols]);
FT_EXTERN int ft_s_table_write_ln(FTABLE *FT_RESTRICT table, size_t rows, size_t cols, const char* FT_RESTRICT table_cells[rows][cols]);

FT_EXTERN int ft_table_write(FTABLE *FT_RESTRICT table, size_t rows, size_t cols, const char* * FT_RESTRICT table_cells[rows]);
FT_EXTERN int ft_table_write_ln(FTABLE *FT_RESTRICT table, size_t rows, size_t cols, const char* * FT_RESTRICT table_cells[rows]);






FT_EXTERN int ft_add_separator(FTABLE *FT_RESTRICT table);






FT_EXTERN const char* ft_to_string(const FTABLE *FT_RESTRICT table);
FT_EXTERN const wchar_t* ft_to_wstring(const FTABLE *FT_RESTRICT table);
/*FT_EXTERN ssize_t ft_n_to_string(const FTABLE *FT_RESTRICT table, char *FT_RESTRICT dst, size_t dst_len);*/




struct ft_border_chars
{
    char top_border_ch;
    char separator_ch;
    char bottom_border_ch;
    char side_border_ch;
    char out_intersect_ch;
    char in_intersect_ch;
};

struct ft_border_style
{
    struct ft_border_chars border_chs;
    struct ft_border_chars header_border_chs;
    char hor_separator_char;
};

/* List of built-in table border styles */
extern struct ft_border_style * FT_BASIC_STYLE;
extern struct ft_border_style * FT_SIMPLE_STYLE;
extern struct ft_border_style * FT_PLAIN_STYLE;
extern struct ft_border_style * FT_DOT_STYLE;
extern struct ft_border_style * FT_EMPTY_STYLE;


FT_EXTERN int ft_set_default_borders(struct ft_border_style *style);
FT_EXTERN int ft_set_table_borders(FTABLE * FT_RESTRICT table, struct ft_border_style *style);

FT_EXTERN int ft_set_default_cell_option(uint32_t option, int value);
FT_EXTERN int ft_set_cell_option(FTABLE * FT_RESTRICT table, unsigned row, unsigned col, uint32_t option, int value);


FT_END_DECLS

#endif /* LIBFORT_H */
