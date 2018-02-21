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

#define CORE_HEX(I, I_LEN, O) core_ascii_to_hex(I, I_LEN, O, sizeof(O))
CORE_DECLARE(void *) core_ascii_to_hex(
        char *in, int in_len, void *out, int out_len);

CORE_DECLARE(void *) core_hex_to_ascii(
        void *in, int in_len, void *out, int out_len);

CORE_DECLARE(void *) core_uint64_to_buffer(
        c_uint64_t num, int size, void *buffer);

CORE_DECLARE(c_uint64_t) core_buffer_to_uint64(void *buffer, int size);

CORE_DECLARE(void *) core_bcd_to_buffer(c_int8_t *in, void *out, int *out_len);
CORE_DECLARE(void *) core_buffer_to_bcd(c_uint8_t *in, int in_len, void *out);

/*
 * Apache's "replacement" for the strncpy() function. We roll our
 * own to implement these specific changes:
 *   (1) strncpy() doesn't always null terminate and we want it to.
 *   (2) strncpy() null fills, which is bogus, esp. when copy 8byte
 *       strings into 8k blocks.
 *   (3) Instead of returning the pointer to the beginning of
 *       the destination string, we return a pointer to the
 *       terminating '\0' to allow us to "check" for truncation
 *   (4) If src is NULL, null terminate dst (empty string copy)
 *
 * apr_cpystrn() follows the same call structure as strncpy().
 */
CORE_DECLARE(char *)core_cpystrn(char *dst, const char *src, size_t dst_size);

/**
 * Get the value of an environment variable
 * @param value the returned value, allocated from @a pool
 * @param envvar the name of the environment variable
 */
CORE_DECLARE(char *) core_env_get(const char *envvar);

/**
 * Set the value of an environment variable
 * @param envvar the name of the environment variable
 * @param value the value to set
 */
CORE_DECLARE(status_t) core_env_set(const char *envvar, const char *value);

/**
 * Delete a variable from the environment
 * @param envvar the name of the environment variable
 */
CORE_DECLARE(status_t) core_env_delete(const char *envvar);

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif    /* ! __CORE_LIB_H__ */
