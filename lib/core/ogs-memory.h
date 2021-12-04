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

#define OGS_MEM_CLEAR(__dATA) \
    do { \
        if ((__dATA)) { \
            ogs_free((__dATA)); \
            (__dATA) = NULL; \
        } \
    } while(0)

#define ogs_malloc(size) ogs_malloc_debug(size, OGS_FILE_LINE, false)
#define ogs_malloc_or_assert(size) \
    ogs_malloc_debug(size, OGS_FILE_LINE, true)
void *ogs_malloc_debug(size_t size, const char *file_line, bool abort);
void ogs_free(void *ptr);
#define ogs_calloc(nmemb, size) \
    ogs_calloc_debug(nmemb, size, OGS_FILE_LINE, false)
#define ogs_calloc_or_assert(nmemb, size) \
    ogs_calloc_debug(nmemb, size, OGS_FILE_LINE, true)
void *ogs_calloc_debug(
        size_t nmemb, size_t size, const char *file_line, bool abort);
#define ogs_realloc(ptr, size) \
    ogs_realloc_debug(ptr, size, OGS_FILE_LINE, false)
#define ogs_realloc_or_assert(ptr, size) \
    ogs_realloc_debug(ptr, size, OGS_FILE_LINE, true)
void *ogs_realloc_debug(
        void *ptr, size_t size, const char *file_line, bool abort);

#ifdef __cplusplus
}
#endif

#endif /* OGS_MEMORY_H */
