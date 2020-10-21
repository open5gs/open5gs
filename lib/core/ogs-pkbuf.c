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

#define OGS_CLUSTER_128_SIZE    128
#define OGS_CLUSTER_256_SIZE    256
#define OGS_CLUSTER_512_SIZE    512
#define OGS_CLUSTER_1024_SIZE   1024
#define OGS_CLUSTER_2048_SIZE   2048
#define OGS_CLUSTER_8192_SIZE   8192
#define OGS_CLUSTER_BIG_SIZE    1024*1024

typedef uint8_t ogs_cluster_128_t[OGS_CLUSTER_128_SIZE];
typedef uint8_t ogs_cluster_256_t[OGS_CLUSTER_256_SIZE];
typedef uint8_t ogs_cluster_512_t[OGS_CLUSTER_512_SIZE];
typedef uint8_t ogs_cluster_1024_t[OGS_CLUSTER_1024_SIZE];
typedef uint8_t ogs_cluster_2048_t[OGS_CLUSTER_2048_SIZE];
typedef uint8_t ogs_cluster_8192_t[OGS_CLUSTER_8192_SIZE];
typedef uint8_t ogs_cluster_big_t[OGS_CLUSTER_BIG_SIZE];

OGS_STATIC_ASSERT(sizeof(ogs_cluster_128_t) % sizeof(void *) == 0);
OGS_STATIC_ASSERT(sizeof(ogs_cluster_256_t) % sizeof(void *) == 0);
OGS_STATIC_ASSERT(sizeof(ogs_cluster_512_t) % sizeof(void *) == 0);
OGS_STATIC_ASSERT(sizeof(ogs_cluster_1024_t) % sizeof(void *) == 0);
OGS_STATIC_ASSERT(sizeof(ogs_cluster_2048_t) % sizeof(void *) == 0);
OGS_STATIC_ASSERT(sizeof(ogs_cluster_8192_t) % sizeof(void *) == 0);
OGS_STATIC_ASSERT(sizeof(ogs_cluster_big_t) % sizeof(void *) == 0);

typedef struct ogs_pkbuf_pool_s {
    OGS_POOL(pkbuf, ogs_pkbuf_t);
    OGS_POOL(cluster, ogs_cluster_t);

    OGS_POOL(cluster_128, ogs_cluster_128_t);
    OGS_POOL(cluster_256, ogs_cluster_256_t);
    OGS_POOL(cluster_512, ogs_cluster_512_t);
    OGS_POOL(cluster_1024, ogs_cluster_1024_t);
    OGS_POOL(cluster_2048, ogs_cluster_2048_t);
    OGS_POOL(cluster_8192, ogs_cluster_8192_t);
    OGS_POOL(cluster_big, ogs_cluster_big_t);

    ogs_thread_mutex_t mutex;
} ogs_pkbuf_pool_t;

static OGS_POOL(pkbuf_pool, ogs_pkbuf_pool_t);
static ogs_pkbuf_pool_t *default_pool = NULL;

static ogs_cluster_t *cluster_alloc(
        ogs_pkbuf_pool_t *pool, unsigned int size);
static void cluster_free(ogs_pkbuf_pool_t *pool, ogs_cluster_t *cluster);

void *ogs_pkbuf_put_data(
        ogs_pkbuf_t *pkbuf, const void *data, unsigned int len)
{
    void *tmp = ogs_pkbuf_put(pkbuf, len);

    memcpy(tmp, data, len);
    return tmp;
}

void ogs_pkbuf_init(void)
{
    ogs_pool_init(&pkbuf_pool, ogs_core()->pkbuf.pool);
}

void ogs_pkbuf_final(void)
{
    ogs_pool_final(&pkbuf_pool);
}

void ogs_pkbuf_default_init(ogs_pkbuf_config_t *config)
{
    ogs_assert(config);
    memset(config, 0, sizeof *config);

    config->cluster_128_pool = 32768;
    config->cluster_256_pool = 4096;
    config->cluster_512_pool = 2048;
    config->cluster_1024_pool = 1024;
    config->cluster_2048_pool = 512;
    config->cluster_8192_pool = 128;
    config->cluster_big_pool = 8;
}

void ogs_pkbuf_default_create(ogs_pkbuf_config_t *config)
{
    default_pool = ogs_pkbuf_pool_create(config);
}

void ogs_pkbuf_default_destroy(void)
{
    ogs_pkbuf_pool_destroy(default_pool);
}

ogs_pkbuf_pool_t *ogs_pkbuf_pool_create(ogs_pkbuf_config_t *config)
{
    ogs_pkbuf_pool_t *pool = NULL;
    int tmp = 0;

    ogs_assert(config);

    ogs_pool_alloc(&pkbuf_pool, &pool);
    ogs_assert(pool);
    memset(pool, 0, sizeof *pool);

    ogs_thread_mutex_init(&pool->mutex);

    tmp = config->cluster_128_pool + config->cluster_256_pool +
        config->cluster_512_pool + config->cluster_1024_pool +
        config->cluster_2048_pool + config->cluster_8192_pool +
        config->cluster_big_pool;

    ogs_pool_init(&pool->pkbuf, tmp);
    ogs_pool_init(&pool->cluster, tmp);

    ogs_pool_init(&pool->cluster_128, config->cluster_128_pool);
    ogs_pool_init(&pool->cluster_256, config->cluster_256_pool);
    ogs_pool_init(&pool->cluster_512, config->cluster_512_pool);
    ogs_pool_init(&pool->cluster_1024, config->cluster_1024_pool);
    ogs_pool_init(&pool->cluster_2048, config->cluster_2048_pool);
    ogs_pool_init(&pool->cluster_8192, config->cluster_8192_pool);
    ogs_pool_init(&pool->cluster_big, config->cluster_big_pool);

    return pool;
}

void ogs_pkbuf_pool_destroy(ogs_pkbuf_pool_t *pool)
{
    ogs_assert(pool);

    ogs_pool_final(&pool->pkbuf);
    ogs_pool_final(&pool->cluster);

    ogs_pool_final(&pool->cluster_128);
    ogs_pool_final(&pool->cluster_256);
    ogs_pool_final(&pool->cluster_512);
    ogs_pool_final(&pool->cluster_1024);
    ogs_pool_final(&pool->cluster_2048);
    ogs_pool_final(&pool->cluster_8192);
    ogs_pool_final(&pool->cluster_big);

    ogs_thread_mutex_destroy(&pool->mutex);

    ogs_pool_free(&pkbuf_pool, pool);
}

ogs_pkbuf_t *ogs_pkbuf_alloc(ogs_pkbuf_pool_t *pool, unsigned int size)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_cluster_t *cluster = NULL;

    if (pool == NULL)
        pool = default_pool;
    ogs_assert(pool);

    ogs_thread_mutex_lock(&pool->mutex);

    cluster = cluster_alloc(pool, size);
    if (!cluster) {
        ogs_error("ogs_pkbuf_alloc() failed [size=%d]", size);
        ogs_thread_mutex_unlock(&pool->mutex);
        return NULL;
    }
    ogs_assert(cluster);

    ogs_pool_alloc(&pool->pkbuf, &pkbuf);
    ogs_assert(pkbuf);
    memset(pkbuf, 0, sizeof(*pkbuf));

    cluster->ref++;

    ogs_thread_mutex_unlock(&pool->mutex);

    pkbuf->cluster = cluster;

    pkbuf->len = 0;

    pkbuf->data = cluster->buffer;
    pkbuf->head = cluster->buffer;
    pkbuf->tail = cluster->buffer;
    pkbuf->end = cluster->buffer + size;

    pkbuf->pool = pool;

    return pkbuf;
}

void ogs_pkbuf_free(ogs_pkbuf_t *pkbuf)
{
    ogs_pkbuf_pool_t *pool = NULL;
    ogs_cluster_t *cluster = NULL;
    ogs_assert(pkbuf);

    pool = pkbuf->pool;
    ogs_assert(pool);

    cluster = pkbuf->cluster;
    ogs_assert(cluster);

    ogs_thread_mutex_lock(&pool->mutex);

    cluster->ref--;
    if (cluster->ref == 0)
        cluster_free(pool, pkbuf->cluster);

    ogs_pool_free(&pool->pkbuf, pkbuf);

    ogs_thread_mutex_unlock(&pool->mutex);
}

ogs_pkbuf_t *ogs_pkbuf_copy(ogs_pkbuf_t *pkbuf)
{
    ogs_pkbuf_pool_t *pool = NULL;
    ogs_pkbuf_t *newbuf = NULL;

    ogs_assert(pkbuf);
    pool = pkbuf->pool;
    ogs_assert(pool);

    ogs_thread_mutex_lock(&pool->mutex);

    ogs_pool_alloc(&pool->pkbuf, &newbuf);
    if (!newbuf) {
        ogs_error("ogs_pkbuf_copy() failed");
        ogs_thread_mutex_unlock(&pool->mutex);
        return NULL;
    }
    ogs_assert(newbuf);
    memcpy(newbuf, pkbuf, sizeof *pkbuf);

    newbuf->cluster->ref++;

    ogs_thread_mutex_unlock(&pool->mutex);

    return newbuf;
}

static ogs_cluster_t *cluster_alloc(
        ogs_pkbuf_pool_t *pool, unsigned int size)
{
    ogs_cluster_t *cluster = NULL;
    void *buffer = NULL;
    ogs_assert(pool);

    ogs_pool_alloc(&pool->cluster, &cluster);
    ogs_assert(cluster);
    memset(cluster, 0, sizeof(*cluster));

    if (size <= OGS_CLUSTER_128_SIZE) {
        ogs_pool_alloc(&pool->cluster_128, (ogs_cluster_128_t**)&buffer);
        if (!buffer) {
            ogs_fatal("No OGS_CLUSTER_128_SIZE");
            return NULL;
        }
        cluster->size = OGS_CLUSTER_128_SIZE;
    } else if (size <= OGS_CLUSTER_256_SIZE) {
        ogs_pool_alloc(&pool->cluster_256, (ogs_cluster_256_t**)&buffer);
        if (!buffer) {
            ogs_fatal("No OGS_CLUSTER_256_SIZE");
            return NULL;
        }
        cluster->size = OGS_CLUSTER_256_SIZE;
    } else if (size <= OGS_CLUSTER_512_SIZE) {
        ogs_pool_alloc(&pool->cluster_512, (ogs_cluster_512_t**)&buffer);
        if (!buffer) {
            ogs_fatal("No OGS_CLUSTER_512_SIZE");
            return NULL;
        }
        cluster->size = OGS_CLUSTER_512_SIZE;
    } else if (size <= OGS_CLUSTER_1024_SIZE) {
        ogs_pool_alloc(&pool->cluster_1024, (ogs_cluster_1024_t**)&buffer);
        if (!buffer) {
            ogs_fatal("No OGS_CLUSTER_1024_SIZE");
            return NULL;
        }
        cluster->size = OGS_CLUSTER_1024_SIZE;
    } else if (size <= OGS_CLUSTER_2048_SIZE) {
        ogs_pool_alloc(&pool->cluster_2048, (ogs_cluster_2048_t**)&buffer);
        if (!buffer) {
            ogs_fatal("No OGS_CLUSTER_2048_SIZE");
            return NULL;
        }
        cluster->size = OGS_CLUSTER_2048_SIZE;
    } else if (size <= OGS_CLUSTER_8192_SIZE) {
        ogs_pool_alloc(&pool->cluster_8192, (ogs_cluster_8192_t**)&buffer);
        if (!buffer) {
            ogs_fatal("No OGS_CLUSTER_8192_SIZE");
            return NULL;
        }
        cluster->size = OGS_CLUSTER_8192_SIZE;
    } else if (size <= OGS_CLUSTER_BIG_SIZE) {
        ogs_pool_alloc(&pool->cluster_big, (ogs_cluster_big_t**)&buffer);
        if (!buffer) {
            ogs_fatal("No OGS_CLUSTER_BIG_SIZE");
        }
        cluster->size = OGS_CLUSTER_BIG_SIZE;
    } else {
        ogs_fatal("invalid size = %d", size);
        ogs_assert_if_reached();
    }
    cluster->buffer = buffer;

    return cluster;
}

static void cluster_free(ogs_pkbuf_pool_t *pool, ogs_cluster_t *cluster)
{
    ogs_assert(pool);
    ogs_assert(cluster);
    ogs_assert(cluster->buffer);

    switch (cluster->size) {
    case OGS_CLUSTER_128_SIZE:
        ogs_pool_free(&pool->cluster_128, (ogs_cluster_128_t*)cluster->buffer);
        break;
    case OGS_CLUSTER_256_SIZE:
        ogs_pool_free(&pool->cluster_256, (ogs_cluster_256_t*)cluster->buffer);
        break;
    case OGS_CLUSTER_512_SIZE:
        ogs_pool_free(&pool->cluster_512, (ogs_cluster_512_t*)cluster->buffer);
        break;
    case OGS_CLUSTER_1024_SIZE:
        ogs_pool_free(
                &pool->cluster_1024, (ogs_cluster_1024_t*)cluster->buffer);
        break;
    case OGS_CLUSTER_2048_SIZE:
        ogs_pool_free(
                &pool->cluster_2048, (ogs_cluster_2048_t*)cluster->buffer);
        break;
    case OGS_CLUSTER_8192_SIZE:
        ogs_pool_free(
                &pool->cluster_8192, (ogs_cluster_8192_t*)cluster->buffer);
        break;
    case OGS_CLUSTER_BIG_SIZE:
        ogs_pool_free(&pool->cluster_big, (ogs_cluster_big_t*)cluster->buffer);
        break;
    default:
        ogs_assert_if_reached();
    }

    ogs_pool_free(&pool->cluster, cluster);
}
