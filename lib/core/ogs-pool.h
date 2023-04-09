/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef OGS_POOL_H
#define OGS_POOL_H

#ifdef __cplusplus
extern "C" {
#endif

#if OGS_USE_TALLOC == 1
#define ogs_pool_create ogs_malloc
#define ogs_pool_destroy ogs_free
#else
#define ogs_pool_create malloc
#define ogs_pool_destroy free
#endif

typedef uint32_t ogs_pool_id_t;

#define OGS_POOL(pool, type) \
    struct { \
        const char *name; \
        int head, tail; \
        int size, avail; \
        type **free, *array, **index; \
    } pool

#define ogs_pool_init(pool, _size) do { \
    int i; \
    (pool)->name = #pool; \
    (pool)->free = ogs_pool_create(sizeof(*(pool)->free) * _size); \
    ogs_assert((pool)->free); \
    (pool)->array = ogs_pool_create(sizeof(*(pool)->array) * _size); \
    ogs_assert((pool)->array); \
    (pool)->index = ogs_pool_create(sizeof(*(pool)->index) * _size); \
    ogs_assert((pool)->index); \
    (pool)->size = (pool)->avail = _size; \
    (pool)->head = (pool)->tail = 0; \
    for (i = 0; i < _size; i++) { \
        (pool)->free[i] = &((pool)->array[i]); \
        (pool)->index[i] = NULL; \
    } \
} while (0)

#define ogs_pool_final(pool) do { \
    if (((pool)->size != (pool)->avail)) \
        ogs_error("%d in '%s[%d]' were not released.", \
                (pool)->size - (pool)->avail, (pool)->name, (pool)->size); \
    ogs_pool_destroy((pool)->free); \
    ogs_pool_destroy((pool)->array); \
    ogs_pool_destroy((pool)->index); \
} while (0)

#define ogs_pool_index(pool, node) (((node) - (pool)->array)+1)
#define ogs_pool_find(pool, _index) \
    (_index > 0 && _index <= (pool)->size) ? (pool)->index[_index-1] : NULL
#define ogs_pool_cycle(pool, node) \
    ogs_pool_find((pool), ogs_pool_index((pool), (node)))

#define ogs_pool_alloc(pool, node) do { \
    *(node) = NULL; \
    if ((pool)->avail > 0) { \
        (pool)->avail--; \
        *(node) = (void*)(pool)->free[(pool)->head]; \
        (pool)->free[(pool)->head] = NULL; \
        (pool)->head = ((pool)->head + 1) % ((pool)->size); \
        (pool)->index[ogs_pool_index(pool, *(node))-1] = *(node); \
    } \
} while (0)

#define ogs_pool_free(pool, node) do { \
    if ((pool)->avail < (pool)->size) { \
        (pool)->avail++; \
        (pool)->free[(pool)->tail] = (void*)(node); \
        (pool)->tail = ((pool)->tail + 1) % ((pool)->size); \
        (pool)->index[ogs_pool_index(pool, node)-1] = NULL; \
    } \
} while (0)

#define ogs_pool_size(pool) ((pool)->size)
#define ogs_pool_avail(pool) ((pool)->avail)

#define ogs_pool_sequence_id_generate(pool) do { \
    int i; \
    for (i = 0; i < (pool)->size; i++) \
        (pool)->array[i] = i+1; \
} while (0)

#define ogs_pool_random_id_generate(pool) do { \
    int i, j; \
    ogs_pool_id_t temp; \
    for (i = 0; i < (pool)->size; i++) \
        (pool)->array[i] = i+1; \
    for (i = (pool)->size - 1; i > 0; i--) { \
       j = ogs_random32() % (i + 1); \
       temp = (pool)->array[i]; \
       (pool)->array[i] = (pool)->array[j]; \
       (pool)->array[j] = temp; \
    } \
} while (0)

#ifdef __cplusplus
}
#endif

#endif /* OGS_POOL_H */
