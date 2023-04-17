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

#ifndef OGS_MEMORY_H
#define OGS_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

void ogs_mem_init(void);
void ogs_mem_final(void);

void *ogs_mem_get_mutex(void);

#define OGS_MEM_CLEAR(__dATA) \
    do { \
        if ((__dATA)) { \
            ogs_free((__dATA)); \
            (__dATA) = NULL; \
        } \
    } while(0)

#include <talloc.h>

extern void *__ogs_talloc_core;

void *ogs_talloc_size(const void *ctx, size_t size, const char *name);
void *ogs_talloc_zero_size(const void *ctx, size_t size, const char *name);
void *ogs_talloc_realloc_size(
        const void *context, void *oldptr, size_t size, const char *name);
int ogs_talloc_free(void *ptr, const char *location);

void *ogs_malloc_debug(size_t size, const char *file_line);
void *ogs_calloc_debug(
        size_t nmemb, size_t size, const char *file_line);
void *ogs_realloc_debug(
        void *ptr, size_t size, const char *file_line);
int ogs_free_debug(void *ptr);

#if OGS_USE_TALLOC == 1

/*****************************************
 * Memory Pool - Use talloc library
 *****************************************/

#define ogs_malloc(size) \
    ogs_talloc_size(__ogs_talloc_core, size, __location__)
#define ogs_calloc(nmemb, size) \
    ogs_talloc_zero_size(__ogs_talloc_core, (nmemb) * (size), __location__)
#define ogs_realloc(oldptr, size) \
    ogs_talloc_realloc_size(__ogs_talloc_core, oldptr, size, __location__)
#define ogs_free(ptr) ogs_talloc_free(ptr, __location__)

#else

/*****************************************
 * Memory Pool - Use pkbuf library
 *****************************************/

#define ogs_malloc(size) ogs_malloc_debug(size, OGS_FILE_LINE)
#define ogs_calloc(nmemb, size) ogs_calloc_debug(nmemb, size, OGS_FILE_LINE)
#define ogs_realloc(ptr, size) ogs_realloc_debug(ptr, size, OGS_FILE_LINE)
#define ogs_free(ptr) ogs_free_debug(ptr)

#endif

#ifdef __cplusplus
}
#endif

#endif /* OGS_MEMORY_H */
