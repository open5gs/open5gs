/**
 *
 * Orcania library
 *
 * Different functions for different purposes but that can be shared between
 * other projects
 *
 * orcania.c: main functions definitions
 *
 * Copyright 2015-2020 Nicolas Mora <mail@babelouest.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation;
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU GENERAL PUBLIC LICENSE for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
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

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

#if HAVE_STDARG_H
#include <stdarg.h>
#endif

#if HAVE_CTYPE_H
#include <ctype.h>
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
    ogs_expect_or_return_val(res, res);
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
    ogs_expect_or_return_val(res, res);
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
    ogs_expect_or_return_val(res, res);
    memcpy(res, m, n);
    return res;
}

/*
 * char *ogs_msprintf(const char *message, ...)
 * char *mstrcatf(char *source, const char *message, ...)
 *
 * Orcania library
 * Copyright 2015-2018 Nicolas Mora <mail@babelouest.org>
 * License: LGPL-2.1
 *
 * https://github.com/babelouest/orcania.git
 */
char *ogs_msprintf_debug(
        const char *file_line, const char *message, ...)
{
    va_list argp, argp_cpy;
    size_t out_len = 0;
    char *out = NULL;
    if (message != NULL) {
        va_start(argp, message);
        va_copy(argp_cpy, argp); /* We make a copy because
                                    in some architectures,
                                    vsnprintf can modify argp */
        out_len = vsnprintf(NULL, 0, message, argp);
        out = ogs_malloc_debug(out_len + sizeof(char), file_line);
        if (out == NULL) {
            va_end(argp);
            va_end(argp_cpy);
            return NULL;
        }
        vsnprintf(out, (out_len + sizeof(char)), message, argp_cpy);
        va_end(argp);
        va_end(argp_cpy);
    }
    return out;
}

char *ogs_mstrcatf_debug(
        char *source, const char *file_line, const char *message, ...)
{
    va_list argp, argp_cpy;
    char *out = NULL, *message_formatted = NULL;
    size_t message_formatted_len = 0, out_len = 0;

    if (message != NULL) {
        if (source != NULL) {
            va_start(argp, message);
            va_copy(argp_cpy, argp); /* We make a copy because
                                        in some architectures,
                                        vsnprintf can modify argp */
            message_formatted_len = vsnprintf(NULL, 0, message, argp);
            message_formatted = ogs_malloc(message_formatted_len+sizeof(char));
            if (message_formatted != NULL) {
                vsnprintf(message_formatted,
                    (message_formatted_len+sizeof(char)), message, argp_cpy);
                out = ogs_msprintf_debug(
                        file_line, "%s%s", source, message_formatted);
                ogs_free(message_formatted);
                ogs_free(source);
            }
            va_end(argp);
            va_end(argp_cpy);
        } else {
            va_start(argp, message);
            va_copy(argp_cpy, argp); /* We make a copy because
                                        in some architectures,
                                        vsnprintf can modify argp */
            out_len = vsnprintf(NULL, 0, message, argp);
            out = ogs_malloc_debug(out_len+sizeof(char), file_line);
            if (out != NULL) {
                vsnprintf(out, (out_len+sizeof(char)), message, argp_cpy);
            }
            va_end(argp);
            va_end(argp_cpy);
        }
    }
    return out;
}

char *ogs_trimwhitespace(char *str)
{
    char *end;

    if (str == NULL) {
        return NULL;
    } else if (*str == 0) {
        return str;
    }

    while (isspace((unsigned char)*str)) str++;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) {
        end--;
    }

    *(end+1) = 0;

    return str;
}

char *ogs_left_trimcharacter(char *str, char to_remove)
{
    if (str == NULL) {
        return NULL;
    } else if (*str == 0) {
        return str;
    }

    while(*str == to_remove) str++;

    return str;
}

char *ogs_right_trimcharacter(char *str, char to_remove)
{
    char *end;

    if (str == NULL) {
        return NULL;
    } else if (*str == 0) {
        return str;
    }

    end = str + strlen(str) - 1;
    while(end > str && (*end == to_remove)) {
        end--;
    }

    *(end+1) = 0;

    return str;
}

char *ogs_trimcharacter(char *str, char to_remove)
{
    return ogs_right_trimcharacter(
            ogs_left_trimcharacter(str, to_remove), to_remove);
}
