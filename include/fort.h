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
 * libfort structures and functions declarations
 */

FORT_BEGIN_DECLS

struct fort_table;
typedef struct fort_table FTABLE;

FORT_EXTERN FTABLE * ft_create_table(void);
FORT_EXTERN void ft_destroy_table(FTABLE *FORT_RESTRICT table);


//FORT_EXTERN int ft_printf(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...);
FORT_EXTERN int ft_hdr_printf(FTABLE *FORT_RESTRICT table, const char* FORT_RESTRICT fmt, ...);
FORT_EXTERN int ft_row_printf(FTABLE *FORT_RESTRICT table, size_t row, const char* FORT_RESTRICT fmt, ...);
//FORT_EXTERN int ft_cell_printf(FTABLE *FORT_RESTRICT table, size_t row, size_t col, const char* FORT_RESTRICT fmt, ...);

FORT_EXTERN char* ft_to_string(const FTABLE *FORT_RESTRICT table);
//FORT_EXTERN ssize_t ft_n_to_string(const FTABLE *FORT_RESTRICT table, char *FORT_RESTRICT dst, size_t dst_len);

//FORT_EXTERN char* ft_header_to_string(const FTABLE *FORT_RESTRICT table);
//FORT_EXTERN ssize_t ft_n_header_to_string(const FTABLE *FORT_RESTRICT table, char *FORT_RESTRICT dst, size_t dst_len);

//FORT_EXTERN char* ft_row_to_string(const FTABLE *FORT_RESTRICT table, size_t row);
//FORT_EXTERN ssize_t ft_n_row_to_string(const FTABLE *FORT_RESTRICT table, size_t row, char *FORT_RESTRICT dst, size_t dst_len);

FORT_END_DECLS

#endif // LIBFORT_H
