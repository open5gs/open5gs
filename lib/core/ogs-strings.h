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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_STRINGS_H
#define OGS_STRINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_HUGE_LEN        8192

#if defined(_WIN32)
#define ogs_strtok_r strtok_s
#define ogs_strcasecmp _stricmp
#define ogs_strncasecmp _strnicmp
#else
#define ogs_strtok_r strtok_r
#define ogs_strcasecmp strcasecmp
#define ogs_strncasecmp strncasecmp
#endif

int ogs_vsnprintf(char *str, size_t size, const char *format, va_list ap)
    OGS_GNUC_PRINTF (3, 0);
int ogs_snprintf(char *str, size_t size, const char *format, ...)
    OGS_GNUC_PRINTF(3, 4);
char *ogs_vslprintf(char *str, char *last, const char *format, va_list ap)
    OGS_GNUC_PRINTF (3, 0);
char *ogs_slprintf(char *str, char *last, const char *format, ...)
    OGS_GNUC_PRINTF(3, 4);

#define OGS_STRING_DUP(__dST, __sRC) \
    do { \
        OGS_MEM_CLEAR(__dST); \
        __dST = ogs_strdup(__sRC); \
        ogs_assert(__dST); \
    } while(0)

char *ogs_cpystrn(char *dst, const char *src, size_t dst_size);

char *ogs_talloc_strdup(const void *t, const char *p);
char *ogs_talloc_strndup(const void *t, const char *p, size_t n);
void *ogs_talloc_memdup(const void *t, const void *p, size_t size);
char *ogs_talloc_asprintf(const void *t, const char *fmt, ...)
    OGS_GNUC_PRINTF(2, 3);
char *ogs_talloc_asprintf_append(char *s, const char *fmt, ...)
    OGS_GNUC_PRINTF(2, 3);

char *ogs_strdup_debug(const char *s, const char *file_line);
char *ogs_strndup_debug(const char *s, size_t n, const char *file_line);
void *ogs_memdup_debug(const void *m, size_t n, const char *file_line);

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
char *ogs_msprintf_debug(const char *file_line, const char *message, ...)
    OGS_GNUC_PRINTF(2, 3);
char *ogs_mstrcatf_debug(
    char *source, const char *file_line, const char *message, ...)
    OGS_GNUC_PRINTF(3, 4);

#if OGS_USE_TALLOC

/*****************************************
 * Memory Pool - Use talloc library
 *****************************************/

#define ogs_strdup(p) \
    ogs_talloc_strdup(__ogs_talloc_core, p)
#define ogs_strndup(p, n) \
    ogs_talloc_strndup(__ogs_talloc_core, p, n)
#define ogs_memdup(p, size) \
    ogs_talloc_memdup(__ogs_talloc_core, p, size)
#define ogs_msprintf(...) \
    ogs_talloc_asprintf(__ogs_talloc_core, __VA_ARGS__)
#define ogs_mstrcatf(s, ...) \
    ogs_talloc_asprintf_append(s, __VA_ARGS__)

#else

/*****************************************
 * Memory Pool - Use pkbuf library
 *****************************************/

#define ogs_strdup(s) ogs_strdup_debug(s, OGS_FILE_LINE)
#define ogs_strndup(s, n) ogs_strndup_debug(s, n, OGS_FILE_LINE)
#define ogs_memdup(m, n) ogs_memdup_debug(m, n, OGS_FILE_LINE)
#define ogs_msprintf(...) ogs_msprintf_debug(OGS_FILE_LINE, __VA_ARGS__)
#define ogs_mstrcatf(source, ...) \
    ogs_mstrcatf_debug(source, OGS_FILE_LINE, __VA_ARGS__)

#endif


char *ogs_trimwhitespace(char *str);

char *ogs_left_trimcharacter(char *str, char to_remove);
char *ogs_right_trimcharacter(char *str, char to_remove);
char *ogs_trimcharacter(char *str, char to_remove);

#ifdef __cplusplus
}
#endif

#endif /* OGS_STRINGS_H */
