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
#define FORT_CLANG_COMPILER
#elif defined(__GNUC__)
#define FORT_GCC_COMPILER
#elif defined(_MSC_VER)
#define FORT_MICROSOFT_COMPILER
#else
#define FORT_UNDEFINED_COMPILER
#endif

/*
 * Declare restrict
 */
#if defined(__cplusplus)
#if defined(FORT_CLANG_COMPILER)
#define FORT_RESTRICT __restrict__
#else
#define FORT_RESTRICT __restrict
#endif /* if defined(FORT_CLANG_COMPILER) */
#else
#define FORT_RESTRICT restrict
#endif /* if defined(__cplusplus) */

/*
 * Attribute format for argument checking
 */
#if defined(FORT_CLANG_COMPILER) || defined(FORT_GCC_COMPILER)
#define FORT_PRINTF_ATTRIBUTE_FORMAT(string_index, first_to_check) \
    __attribute__ ((format (printf, string_index, first_to_check)))
#else
#define FORT_PRINTF_ATTRIBUTE_FORMAT(string_index, first_to_check)
#endif /* defined(FORT_CLANG_COMPILER) || defined(FORT_GCC_COMPILER) */


/*
 *  C++ needs to know that types and declarations are C, not C++.
 */
#ifdef	__cplusplus
# define FORT_BEGIN_DECLS	extern "C" {
# define FORT_END_DECLS	}
#else
# define FORT_BEGIN_DECLS
# define FORT_END_DECLS
#endif


#ifndef FORT_EXTERN
#define FORT_EXTERN extern
#endif


/*
 * Helper macros
 */

#define STR_2_CAT_(arg1, arg2) \
    arg1##arg2
#define STR_2_CAT(arg1, arg2) \
    STR_2_CAT_(arg1, arg2)


static inline int fort_check_if_string_helper(const char*str)
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

#define CHECK_IF_ARG_IS_STRING_32(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_31(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_31(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_30(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_30(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_29(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_29(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_28(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_28(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_27(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_27(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_26(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_26(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_25(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_25(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_24(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_24(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_23(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_23(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_22(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_22(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_21(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_21(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_20(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_20(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_19(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_19(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_18(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_18(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_17(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_17(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_16(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_16(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_15(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_15(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_14(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_14(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_13(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_13(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_12(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_12(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_11(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_11(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_10(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_10(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_9(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_9(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_8(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_8(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_7(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_7(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_6(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_6(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_5(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_5(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_4(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_4(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_3(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_3(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_2(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_2(arg,...) (fort_check_if_string_helper(arg),CHECK_IF_ARG_IS_STRING_1(__VA_ARGS__))
#define CHECK_IF_ARG_IS_STRING_1(arg) (fort_check_if_string_helper(arg))

#define CHECK_IF_ARGS_ARE_STRINGS__(func, ...) func(__VA_ARGS__)
#define CHECK_IF_ARGS_ARE_STRINGS_(basis, n, ...) CHECK_IF_ARGS_ARE_STRINGS__(STR_2_CAT_(basis, n), __VA_ARGS__)
#define CHECK_IF_ARGS_ARE_STRINGS(...) CHECK_IF_ARGS_ARE_STRINGS_(CHECK_IF_ARG_IS_STRING_,PP_NARG(__VA_ARGS__), __VA_ARGS__)



/*
 * libfort structures and functions declarations
 */

FORT_BEGIN_DECLS

struct fort_table;
typedef struct fort_table FTABLE;

FORT_EXTERN FTABLE * ft_create_table(void);
FORT_EXTERN void ft_destroy_table(FTABLE *FORT_RESTRICT table);


FORT_EXTERN void ft_ln(FTABLE *FORT_RESTRICT table);


#if defined(FORT_CLANG_COMPILER) || defined(FORT_GCC_COMPILER)
FORT_EXTERN int ft_printf(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...) FORT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
FORT_EXTERN int ft_printf_ln(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...) FORT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
FORT_EXTERN int ft_hdr_printf(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...)  FORT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
FORT_EXTERN int ft_hdr_printf_ln(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...)  FORT_PRINTF_ATTRIBUTE_FORMAT(2, 3);

#else


FORT_EXTERN int ft_printf_impl(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...) FORT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
FORT_EXTERN int ft_printf_ln_impl(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...) FORT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
FORT_EXTERN int ft_hdr_printf_impl(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...)  FORT_PRINTF_ATTRIBUTE_FORMAT(2, 3);
FORT_EXTERN int ft_hdr_printf_ln_impl(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...)  FORT_PRINTF_ATTRIBUTE_FORMAT(2, 3);

#define ft_printf(table, ...) \
    (( 0 ? fprintf(stderr, __VA_ARGS__) : 1), ft_printf_impl(table, __VA_ARGS__))
#define ft_printf_ln(table, ...) \
    (( 0 ? fprintf(stderr, __VA_ARGS__) : 1), ft_printf_ln_impl(table, __VA_ARGS__))
#define ft_hdr_printf(table, ...) \
    (( 0 ? fprintf(stderr, __VA_ARGS__) : 1), ft_hdr_printf_impl(table, __VA_ARGS__))
#define ft_hdr_printf_ln(table, ...) \
    (( 0 ? fprintf(stderr, __VA_ARGS__) : 1), ft_hdr_printf_ln_impl(table, __VA_ARGS__))
#endif



FORT_EXTERN int ft_write(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT cell_content);
FORT_EXTERN int ft_write_ln(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT cell_content);




#define FT_NWRITE(table, ...)\
    (0 ? CHECK_IF_ARGS_ARE_STRINGS(__VA_ARGS__) : ft_nwrite(table, PP_NARG(__VA_ARGS__), __VA_ARGS__))

#define FT_NWRITE_LN(table, ...)\
    (0 ? CHECK_IF_ARGS_ARE_STRINGS(__VA_ARGS__) : ft_nwrite_ln(table, PP_NARG(__VA_ARGS__), __VA_ARGS__))

FORT_EXTERN int ft_nwrite(FTABLE *FORT_RESTRICT table, size_t n, const char* FORT_RESTRICT cell_content, ...);
FORT_EXTERN int ft_nwrite_ln(FTABLE *FORT_RESTRICT table, size_t n, const char* FORT_RESTRICT cell_content, ...);



FORT_EXTERN int ft_row_write(FTABLE *FORT_RESTRICT table, size_t cols, const char* FORT_RESTRICT row_cells[]);
FORT_EXTERN int ft_row_write_ln(FTABLE *FORT_RESTRICT table, size_t cols, const char* FORT_RESTRICT row_cells[]);

FORT_EXTERN int ft_s_table_write(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char* FORT_RESTRICT table_cells[rows][cols]);
FORT_EXTERN int ft_s_table_write_ln(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char* FORT_RESTRICT table_cells[rows][cols]);

FORT_EXTERN int ft_table_write(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char* * FORT_RESTRICT table_cells[rows]);
FORT_EXTERN int ft_table_write_ln(FTABLE *FORT_RESTRICT table, size_t rows, size_t cols, const char* * FORT_RESTRICT table_cells[rows]);






FORT_EXTERN int ft_add_separator(FTABLE *FORT_RESTRICT table);






FORT_EXTERN const char* ft_to_string(const FTABLE *FORT_RESTRICT table);
//FORT_EXTERN ssize_t ft_n_to_string(const FTABLE *FORT_RESTRICT table, char *FORT_RESTRICT dst, size_t dst_len);









struct border_chars
{
    char top_border_ch;
    char separator_ch;
    char bottom_border_ch;
    char side_border_ch;
    char out_intersect_ch;
    char in_intersect_ch;
};

FORT_EXTERN int ft_set_default_borders(struct border_chars *border_chs, struct border_chars *header_border_chs);
FORT_EXTERN int ft_set_table_borders(FTABLE * FORT_RESTRICT table, struct border_chars *border_chs, struct border_chars *header_border_chs);

FORT_EXTERN int ft_set_default_cell_option(uint32_t option, int value);
FORT_EXTERN int ft_set_cell_option(FTABLE * FORT_RESTRICT table, unsigned row, unsigned col, uint32_t option, int value);


FORT_END_DECLS

#endif // LIBFORT_H
