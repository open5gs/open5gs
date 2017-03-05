#ifndef __CORE_LIB_H__
#define __CORE_LIB_H__

/**
 * @file c_lib.h
 * This is collection of oddballs that didn't fit anywhere else,
 * and might move to more appropriate headers with the release
 * of CORE 1.0.
 * @brief CORE general purpose library routines
 */

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup c_lib General Purpose Library Routines
 * @ingroup CORE
 * This is collection of oddballs that didn't fit anywhere else,
 * and might move to more appropriate headers with the release
 * of CORE 1.0.
 * @{
 */

/** A constant representing a 'large' string. */
#define HUGE_STRING_LEN 8192

/*
 * Define the structures used by the CORE general-purpose library.
 */

/** @} */

/**
 * @defgroup c_ctype ctype functions
 * These macros allow correct support of 8-bit characters on systems which
 * support 8-bit characters.  Pretty dumb how the cast is required, but
 * that's legacy libc for ya.  These new macros do not support EOF like
 * the standard macros do.  Tough.
 * @{
 */
/** @see isalnum */
#define c_isalnum(c) (isalnum(((unsigned char)(c))))
/** @see isalpha */
#define c_isalpha(c) (isalpha(((unsigned char)(c))))
/** @see iscntrl */
#define c_iscntrl(c) (iscntrl(((unsigned char)(c))))
/** @see isdigit */
#define c_isdigit(c) (isdigit(((unsigned char)(c))))
/** @see isgraph */
#define c_isgraph(c) (isgraph(((unsigned char)(c))))
/** @see islower*/
#define c_islower(c) (islower(((unsigned char)(c))))
/** @see isascii */
#ifdef isascii
#define c_isascii(c) (isascii(((unsigned char)(c))))
#else
#define c_isascii(c) (((c) & ~0x7f)==0)
#endif
/** @see isprint */
#define c_isprint(c) (isprint(((unsigned char)(c))))
/** @see ispunct */
#define c_ispunct(c) (ispunct(((unsigned char)(c))))
/** @see isspace */
#define c_isspace(c) (isspace(((unsigned char)(c))))
/** @see isupper */
#define c_isupper(c) (isupper(((unsigned char)(c))))
/** @see isxdigit */
#define c_isxdigit(c) (isxdigit(((unsigned char)(c))))
/** @see tolower */
#define c_tolower(c) (tolower(((unsigned char)(c))))
/** @see toupper */
#define c_toupper(c) (toupper(((unsigned char)(c))))

#define c_max(x , y)  (((x) > (y)) ? (x) : (y))
#define c_min(x , y)  (((x) < (y)) ? (x) : (y))

#define c_uint64_to_array(array, uint64) \
    { \
        int i = 0; \
        for (i = 0; i < 8; i++) array[i] = ((uint64 >> (8-1-i) * 8) & 0xff); \
    }

CORE_DECLARE(status_t) core_generate_random_bytes(
        c_uint8_t *buf, int length);

CORE_DECLARE(void *) core_ascii_to_hex(char *in, int len, void *out);
CORE_DECLARE(void *) core_uint64_to_buffer(c_uint64_t num, 
        c_uint8_t *buffer, int size);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif    /* ! __CORE_LIB_H__ */
