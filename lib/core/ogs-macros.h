/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_MACROS_H
#define OGS_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define OGS_GNUC_CHECK_VERSION(major, minor) \
    ((__GNUC__ > (major)) || \
     ((__GNUC__ == (major)) && (__GNUC_MINOR__ >= (minor))))
#else
#define OGS_GNUC_CHECK_VERSION(major, minor) 0
#endif

#if defined(_MSC_VER)
#define ogs_inline __inline
#else
#define ogs_inline __inline__
#endif

#if defined(_WIN32)
#define OGS_FUNC __FUNCTION__
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L
#define OGS_FUNC __FUNCTION__
#else
#define OGS_FUNC __func__
#endif

#if defined(__GNUC__)
#define ogs_likely(x) __builtin_expect (!!(x), 1)
#define ogs_unlikely(x) __builtin_expect (!!(x), 0)
#else
#define ogs_likely(v) v
#define ogs_unlikely(v) v
#endif

#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#if !defined (__clang__) && OGS_GNUC_CHECK_VERSION (4, 4)
#define OGS_GNUC_PRINTF(f, v) __attribute__ ((format (gnu_printf, f, v)))
#else
#define OGS_GNUC_PRINTF(f, v) __attribute__ ((format (__printf__, f, v)))
#endif
#define OGS_GNUC_NORETURN __attribute__((__noreturn__))
#else
#define OGS_GNUC_PRINTF(f, v) 
#define OGS_GNUC_NORETURN
#endif

#if __GNUC__ > 6
#define OGS_GNUC_FALLTHROUGH __attribute__ ((fallthrough))
#else
#define OGS_GNUC_FALLTHROUGH
#endif

#if defined(_WIN32)
#define htole16(x) (x)
#define htole32(x) (x)
#define htole64(x) (x)
#define le16toh(x) (x)
#define le32toh(x) (x)
#define le64toh(x) (x)

#define htobe16(x) htons((x))
#define htobe32(x) htonl((x))
#define htobe64(x) htonll((x))
#define be16toh(x) ntohs((x))
#define be32toh(x) ntohl((x))
#define be64toh(x) ntohll((x))

#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define htole16(x) OSSwapHostToLittleInt16((x))
#define htole32(x) OSSwapHostToLittleInt32((x))
#define htole64(x) OSSwapHostToLittleInt64((x))
#define le16toh(x) OSSwapLittleToHostInt16((x))
#define le32toh(x) OSSwapLittleToHostInt32((x))
#define le64toh(x) OSSwapLittleToHostInt64((x))

#define htobe16(x) OSSwapHostToBigInt16((x))
#define htobe32(x) OSSwapHostToBigInt32((x))
#define htobe64(x) OSSwapHostToBigInt64((x))
#define be16toh(x) OSSwapBigToHostInt16((x))
#define be32toh(x) OSSwapBigToHostInt32((x))
#define be64toh(x) OSSwapBigToHostInt64((x))

#elif defined(__FreeBSD__)
#include <sys/endian.h>

#elif defined(__linux__)
#include <endian.h>

#endif

#ifndef WORDS_BIGENDIAN
#if OGS_BYTE_ORDER == OGS_BIG_ENDIAN
#define WORDS_BIGENDIAN 1
#endif
#endif

typedef struct ogs_uint24_s {
    uint32_t v:24;
}  __attribute__ ((packed)) ogs_uint24_t;

static ogs_inline ogs_uint24_t ogs_be24toh(ogs_uint24_t x)
{
    uint32_t tmp = x.v;
    tmp = be32toh(tmp);
    x.v = tmp >> 8;
    return x;
}

static ogs_inline ogs_uint24_t ogs_htobe24(ogs_uint24_t x)
{
    uint32_t tmp = x.v;
    tmp = htobe32(tmp);
    x.v = tmp >> 8;
    return x;
}

#if OGS_BYTE_ORDER == OGS_BIG_ENDIAN
#define ED2(x1, x2) x1 x2
#define ED3(x1, x2, x3) x1 x2 x3
#define ED4(x1, x2, x3, x4) x1 x2 x3 x4
#define ED5(x1, x2, x3, x4, x5) x1 x2 x3 x4 x5
#define ED6(x1, x2, x3, x4, x5, x6) x1 x2 x3 x4 x5 x6
#define ED7(x1, x2, x3, x4, x5, x6, x7) x1 x2 x3 x4 x5 x6 x7
#define ED8(x1, x2, x3, x4, x5, x6, x7, x8) x1 x2 x3 x4 x5 x6 x7 x8
#else
#define ED2(x1, x2) x2 x1
#define ED3(x1, x2, x3) x3 x2 x1
#define ED4(x1, x2, x3, x4) x4 x3 x2 x1
#define ED5(x1, x2, x3, x4, x5) x5 x4 x3 x2 x1
#define ED6(x1, x2, x3, x4, x5, x6) x6 x5 x4 x3 x2 x1
#define ED7(x1, x2, x3, x4, x5, x6, x7) x7 x6 x5 x4 x3 x2 x1
#define ED8(x1, x2, x3, x4, x5, x6, x7, x8) x8 x7 x6 x5 x4 x3 x2 x1
#endif

#define OGS_STATIC_ASSERT(expr) \
    typedef char dummy_for_ogs_static_assert##__LINE__[(expr) ? 1 : -1]

#define OGS_ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define OGS_STRINGIFY(n)            OGS_STRINGIFY_HELPER(n)
#define OGS_STRINGIFY_HELPER(n)     #n

#define OGS_PASTE(n1, n2)           OGS_PASTE_HELPER(n1, n2)
#define OGS_PASTE_HELPER(n1, n2)    n1##n2

#define OGS_INET_NTOP(src, dst) \
    inet_ntop(AF_INET, (void *)(uintptr_t)(src), (dst), INET_ADDRSTRLEN)
#define OGS_INET6_NTOP(src, dst) \
    inet_ntop(AF_INET6, (void *)(src), (dst), INET6_ADDRSTRLEN)

#define ogs_max(x , y)  (((x) > (y)) ? (x) : (y))
#define ogs_min(x , y)  (((x) < (y)) ? (x) : (y))

#if defined(_WIN32)
#define OGS_IS_DIR_SEPARATOR(c) ((c) == OGS_DIR_SEPARATOR || (c) == '/')
#else
#define OGS_IS_DIR_SEPARATOR(c) ((c) == OGS_DIR_SEPARATOR)
#endif

#define ogs_container_of(ptr, type, member) \
    (type *)((unsigned char *)ptr - offsetof(type, member))

#ifndef SWITCH_CASE_INIT
#define SWITCH_CASE_INIT
    #define SWITCH(X)    {char *__switch_p__,  __switch_next__; \
                          for (__switch_p__ = \
                                  X ? (char *)X : (char *)"OGS_SWITCH_NULL", \
                                  __switch_next__ = 1; \
                              __switch_p__; \
                              __switch_p__ = 0, __switch_next__ = 1) { {
    #define CASE(X)            } if (!__switch_next__ || \
                                     !(__switch_next__ = \
                                         strcmp(__switch_p__, X))) {
    #define DEFAULT            } {
    #define END          }}}
#endif

#define OGS_ARG_MAX                     256
#define OGS_MAX_FILEPATH_LEN            256
#define OGS_MAX_IFNAME_LEN              32

#define OGS_FILE_LINE __FILE__ ":" OGS_STRINGIFY(__LINE__)

#define ogs_uint64_to_uint32(x) ((x >= 0xffffffffUL) ? 0xffffffffU : x)

#ifdef __cplusplus
}
#endif

#endif /* OGS_MACROS_H */
