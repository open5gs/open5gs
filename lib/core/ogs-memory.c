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

void *ogs_malloc(size_t size)
{
    size_t headroom = 0;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(size);

    headroom = sizeof(ogs_pkbuf_t *);
    pkbuf = ogs_pkbuf_alloc(NULL, headroom + size);
    ogs_assert(pkbuf);
    ogs_pkbuf_reserve(pkbuf, headroom);
    memcpy(pkbuf->head, &pkbuf, headroom);
    ogs_pkbuf_put(pkbuf, size);

    return pkbuf->data;
}

void ogs_free(void *ptr)
{
    size_t headroom;
    ogs_pkbuf_t *pkbuf = NULL;

    if (!ptr)
        return;

    headroom = sizeof(ogs_pkbuf_t *);
    memcpy(&pkbuf, (unsigned char*)ptr - headroom, headroom);
    ogs_assert(pkbuf);

    ogs_pkbuf_free(pkbuf);
}

void *ogs_calloc(size_t nmemb, size_t size)
{
    void *ptr = NULL;

    ptr = ogs_malloc(nmemb * size);
    ogs_assert(ptr);

    memset(ptr, 0, nmemb * size);
    return ptr;
}

void *ogs_realloc(void *ptr, size_t size)
{
    size_t headroom = 0;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_cluster_t *cluster = NULL;

    if (!ptr)
        return ogs_malloc(size);

    headroom = sizeof(ogs_pkbuf_t *);

    memcpy(&pkbuf, (unsigned char*)ptr - headroom, headroom);
    ogs_assert(pkbuf);
    cluster = pkbuf->cluster;
    ogs_assert(cluster);

    if (!size) {
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

    if (size > (cluster->size - headroom)) {
        void *new = NULL;

        new = ogs_malloc(size);
        ogs_assert(new);
        memcpy(new, ptr, pkbuf->len);

        ogs_pkbuf_free(pkbuf);
        return new;
    } else {
        pkbuf->tail = pkbuf->data + size;
        pkbuf->len = size;
        return ptr;
    }
}
