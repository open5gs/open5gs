/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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

#include "core-config-private.h"

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

#if HAVE_STDARG_H
#include <stdarg.h>
#endif

#include "ogs-core.h"

int ogs_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    int r = -1;

    /* Microsoft has finally implemented snprintf in Visual Studio 2015.
     * In previous versions, I will simulate it as below. */
#if defined(_MSC_VER) && _MSC_VER < 1900
    ogs_assert(str);

    if (size != 0)
        r = _vsnprintf_s(str, size, _TRUNCATE, format, ap);

    if (r == -1)
        r = _vscprintf(format, ap);
#else
    r = vsnprintf(str, size, format, ap);
#endif
    str[size-1] = '\0';

    return r;
}

int ogs_snprintf(char *str, size_t size, const char *format, ...)
{
    int r;
    va_list ap;

    va_start(ap, format);
    r = ogs_vsnprintf(str, size, format, ap);
    va_end(ap);

    return r;
}

char *ogs_vslprintf(char *str, char *last, const char *format, va_list ap)
{
    int r = -1;

    ogs_assert(last);

    if (!str)
        return NULL;

    if (str < last)
        r = ogs_vsnprintf(str, last - str, format, ap);

    return (str + r);
}

char *ogs_slprintf(char *str, char *last, const char *format, ...)
{
    char *r;
    va_list ap;

    va_start(ap, format);
    r = ogs_vslprintf(str, last, format, ap);
    va_end(ap);

    return r;
}

char *ogs_cpystrn(char *dst, const char *src, size_t dst_size)
{
    char *d = dst, *end;

    if (dst_size == 0) {
        return (dst);
    }

    if (src) {
        end = dst + dst_size - 1;

        for (; d < end; ++d, ++src) {
            if (!(*d = *src)) {
                return (d);
            }
        }
    }

    *d = '\0';    /* always null terminate */

    return (d);
}

/*****************************************
 * Memory Pool - Use talloc library
 *****************************************/

char *ogs_talloc_strdup(const void *t, const char *p)
{
    char *ptr = NULL;

    ogs_thread_mutex_lock(ogs_mem_get_mutex());

    ptr = talloc_strdup(t, p);
    ogs_expect(ptr);

    ogs_thread_mutex_unlock(ogs_mem_get_mutex());

    return ptr;
}

char *ogs_talloc_strndup(const void *t, const char *p, size_t n)
{
    char *ptr = NULL;

    ogs_thread_mutex_lock(ogs_mem_get_mutex());

    ptr = talloc_strndup(t, p, n);
    ogs_expect(ptr);

    ogs_thread_mutex_unlock(ogs_mem_get_mutex());

    return ptr;
}

void *ogs_talloc_memdup(const void *t, const void *p, size_t size)
{
    void *ptr = NULL;

    ogs_thread_mutex_lock(ogs_mem_get_mutex());

    ptr = talloc_memdup(t, p, size);
    ogs_expect(ptr);

    ogs_thread_mutex_unlock(ogs_mem_get_mutex());

    return ptr;
}

char *ogs_talloc_asprintf(const void *t, const char *fmt, ...)
{
    va_list ap;
    char *ret;

    ogs_thread_mutex_lock(ogs_mem_get_mutex());

    va_start(ap, fmt);
    ret = talloc_vasprintf(t, fmt, ap);
    ogs_expect(ret);
    va_end(ap);

    ogs_thread_mutex_unlock(ogs_mem_get_mutex());

    return ret;
}

char *ogs_talloc_asprintf_append(char *s, const char *fmt, ...)
{
    va_list ap;

    ogs_thread_mutex_lock(ogs_mem_get_mutex());

    va_start(ap, fmt);
    s = talloc_vasprintf_append(s, fmt, ap);
    ogs_expect(s);
    va_end(ap);

    ogs_thread_mutex_unlock(ogs_mem_get_mutex());

    return s;
}


/*****************************************
 * Memory Pool - Use pkbuf library
 *****************************************/

char *ogs_strdup_debug(const char *s, const char *file_line)
{
    char *res;
    size_t len;

    if (s == NULL)
        return NULL;

    len = strlen(s) + 1;
    res = ogs_memdup_debug(s, len, file_line);
    if (!res) {
        ogs_error("ogs_memdup_debug[len:%d] failed", (int)len);
        return res;
    }
    return res;
}

char *ogs_strndup_debug(
        const char *s, size_t n, const char *file_line)
{
    char *res;
    const char *end;

    if (s == NULL)
        return NULL;

    end = memchr(s, '\0', n);
    if (end != NULL)
        n = end - s;
    res = ogs_malloc_debug(n + 1, file_line);
    if (!res) {
        ogs_error("ogs_malloc_debug[n:%d] failed", (int)n);
        return res;
    }
    memcpy(res, s, n);
    res[n] = '\0';
    return res;
}

void *ogs_memdup_debug(
        const void *m, size_t n, const char *file_line)
{
    void *res;

    if (m == NULL)
        return NULL;

    res = ogs_malloc_debug(n, file_line);
    if (!res) {
        ogs_error("ogs_malloc_debug[n:%d] failed", (int)n);
        return res;
    }
    memcpy(res, m, n);
    return res;
}

/*
 * ogs_vasprintf_debug() is adapted from musl v1.2.5 src/stdio/vasprintf.c.
 * https://git.musl-libc.org/cgit/musl/tree/src/stdio/vasprintf.c?id=0784374d561435f7c787a555aeab8ede699ed298
 *
 * License: MIT (musl-derived code)
 * Copyright © 2005-2020 Rich Felker, et al.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
static char *ogs_vasprintf_debug(
        const char *file_line, const char *format, va_list args)
    OGS_GNUC_PRINTF(2, 0);

static char *ogs_vasprintf_debug(
        const char *file_line, const char *format, va_list args)
{
    va_list copy;
    int len;
    size_t size;
    char *out;

    if (!format)
        return NULL;

    va_copy(copy, args);
    len = vsnprintf(NULL, 0, format, copy);
    va_end(copy);

    if (len < 0)
        return NULL;

    size = (size_t)len + 1;
    out = ogs_malloc_debug(size, file_line);
    if (!out)
        return NULL;

    if (vsnprintf(out, size, format, args) != len) {
        ogs_free_debug(out);
        return NULL;
    }

    return out;
}

char *ogs_msprintf_debug(
        const char *file_line, const char *message, ...)
{
    va_list args;
    char *out;

    va_start(args, message);
    out = ogs_vasprintf_debug(file_line, message, args);
    va_end(args);

    return out;
}

char *ogs_mstrcatf_debug(
        char *source, const char *file_line, const char *message, ...)
{
    va_list args;
    char *suffix, *out = NULL;
    size_t source_len, suffix_len;

    va_start(args, message);
    suffix = ogs_vasprintf_debug(file_line, message, args);
    va_end(args);

    if (!source || !suffix)
        return suffix;

    source_len = strlen(source);
    suffix_len = strlen(suffix);
    if (source_len <= INT_MAX && suffix_len <= INT_MAX - source_len) {
        out = ogs_malloc_debug(source_len + suffix_len + 1, file_line);
        if (out) {
            memcpy(out, source, source_len);
            memcpy(out + source_len, suffix, suffix_len + 1);
        }
    }

    ogs_free_debug(suffix);
    /* Once the suffix is formatted, consume source even on allocation failure. */
    ogs_free(source);

    return out;
}

char *ogs_strrstr(const char *haystack, const char *needle)
{
    char *result = NULL;
    char *p = (char *)haystack;

    if (!haystack || !needle)
        return NULL;

    if (*needle == '\0')
        return (char *)haystack;

    while ((p = strstr(p, needle)) != NULL) {
        result = p;
        p++;
    }

    return result;
}
