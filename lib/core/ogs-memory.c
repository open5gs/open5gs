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

#include "ogs-core.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __ogs_mem_domain

/*****************************************
 * Memory Pool - Use talloc library
 *****************************************/

void *__ogs_talloc_core;

static ogs_thread_mutex_t mutex;

void ogs_mem_init(void)
{
    ogs_thread_mutex_init(&mutex);

    talloc_enable_null_tracking();

#define TALLOC_MEMSIZE 1
    __ogs_talloc_core = talloc_named_const(NULL, TALLOC_MEMSIZE, "core");
}

void ogs_mem_final(void)
{
    if (talloc_total_size(__ogs_talloc_core) != TALLOC_MEMSIZE)
        talloc_report_full(__ogs_talloc_core, stderr);

    talloc_free(__ogs_talloc_core);

    ogs_thread_mutex_destroy(&mutex);
}

void *ogs_mem_get_mutex(void)
{
    return &mutex;
}

void *ogs_talloc_size(const void *ctx, size_t size, const char *name)
{
    void *ptr = NULL;

    ogs_thread_mutex_lock(&mutex);

    ptr = talloc_named_const(ctx, size, name);
    ogs_expect(ptr);

    ogs_thread_mutex_unlock(&mutex);

    return ptr;
}

void *ogs_talloc_zero_size(const void *ctx, size_t size, const char *name)
{
    void *ptr = NULL;

    ogs_thread_mutex_lock(&mutex);

    ptr = _talloc_zero(ctx, size, name);
    ogs_expect(ptr);

    ogs_thread_mutex_unlock(&mutex);

    return ptr;
}

void *ogs_talloc_realloc_size(
        const void *context, void *oldptr, size_t size, const char *name)
{
    void *ptr = NULL;

    ogs_thread_mutex_lock(&mutex);

    ptr = _talloc_realloc(context, oldptr, size, name);
    ogs_expect(ptr);

    ogs_thread_mutex_unlock(&mutex);

    return ptr;
}

int ogs_talloc_free(void *ptr, const char *location)
{
    int ret;

    ogs_thread_mutex_lock(&mutex);

    ret = _talloc_free(ptr, location);

    ogs_thread_mutex_unlock(&mutex);

    return ret;
}

/*****************************************
 * Memory Pool - Use pkbuf library
 *****************************************/

void *ogs_malloc_debug(size_t size, const char *file_line)
{
    size_t headroom = 0;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(size);

    headroom = sizeof(ogs_pkbuf_t *);
    pkbuf = ogs_pkbuf_alloc_debug(NULL, headroom + size, file_line);
    if (!pkbuf) {
        ogs_error("ogs_pkbuf_alloc_debug[headroom:%d, size:%d] failed",
                (int)headroom, (int)size);
        return NULL;
    }

    ogs_pkbuf_reserve(pkbuf, headroom);
    memcpy(pkbuf->head, &pkbuf, headroom);
    ogs_pkbuf_put(pkbuf, size);

    return pkbuf->data;
}

int ogs_free_debug(void *ptr)
{
    size_t headroom;
    ogs_pkbuf_t *pkbuf = NULL;

    if (!ptr)
        return OGS_ERROR;

    headroom = sizeof(ogs_pkbuf_t *);
    memcpy(&pkbuf, (unsigned char*)ptr - headroom, headroom);
    ogs_assert(pkbuf);

    ogs_pkbuf_free(pkbuf);

    return OGS_OK;
}

void *ogs_calloc_debug(size_t nmemb, size_t size, const char *file_line)
{
    void *ptr = NULL;

    ptr = ogs_malloc_debug(nmemb * size, file_line);
    if (!ptr) {
        ogs_error("ogs_malloc_debug[nmemb:%d, size:%d] failed",
                (int)nmemb, (int)size);
        return NULL;
    }

    memset(ptr, 0, nmemb * size);
    return ptr;
}

void *ogs_realloc_debug(void *ptr, size_t size, const char *file_line)
{
    size_t headroom = 0;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_cluster_t *cluster = NULL;

    if (!ptr)
        return ogs_malloc(size);

    headroom = sizeof(ogs_pkbuf_t *);

    memcpy(&pkbuf, (unsigned char*)ptr - headroom, headroom);

    if (!pkbuf) {
        ogs_error("Cannot get pkbuf from ptr[%p], headroom[%d]",
                ptr, (int)headroom);
        return NULL;
    }

    cluster = pkbuf->cluster;
    if (!cluster) {
        ogs_error("No cluster");
        return NULL;
    }

    if (!size) {
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

    if (size > (cluster->size - headroom)) {
        void *new = NULL;

        new = ogs_malloc_debug(size, file_line);
        if (!new) {
            ogs_error("ogs_malloc_debug[%d] failed", (int)size);
            return NULL;
        }

        memcpy(new, ptr, pkbuf->len);

        ogs_pkbuf_free(pkbuf);
        return new;
    } else {
        pkbuf->tail = pkbuf->data + size;
        pkbuf->len = size;
        return ptr;
    }
}
