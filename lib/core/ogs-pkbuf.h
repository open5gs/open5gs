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

#ifndef OGS_PKBUF_H
#define OGS_PKBUF_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_cluster_s {
    unsigned char *buffer;
    unsigned int size;

    unsigned int ref;
} ogs_cluster_t;

#if OGS_USE_TALLOC
typedef void ogs_pkbuf_pool_t;
#else
typedef struct ogs_pkbuf_pool_s ogs_pkbuf_pool_t;
#endif
typedef struct ogs_pkbuf_s {
    ogs_lnode_t lnode;

    /* Currently it is used in SCTP stream number and PPID. */
    uint64_t param[2];

    ogs_cluster_t *cluster;

    unsigned int len;

    unsigned char *head;
    unsigned char *tail;
    unsigned char *data;
    unsigned char *end;

    const char *file_line;
    
    ogs_pkbuf_pool_t *pool;

    unsigned char _data[0]; /*!< optional immediate data array */
} ogs_pkbuf_t;

typedef struct ogs_pkbuf_config_s {
    int cluster_128_pool;
    int cluster_256_pool;
    int cluster_512_pool;
    int cluster_1024_pool;
    int cluster_2048_pool;
    int cluster_8192_pool;
    int cluster_big_pool;
} ogs_pkbuf_config_t;

void ogs_pkbuf_init(void);
void ogs_pkbuf_final(void);

void ogs_pkbuf_default_init(ogs_pkbuf_config_t *config);
void ogs_pkbuf_default_create(ogs_pkbuf_config_t *config);
void ogs_pkbuf_default_destroy(void);

ogs_pkbuf_pool_t *ogs_pkbuf_pool_create(ogs_pkbuf_config_t *config);
void ogs_pkbuf_pool_destroy(ogs_pkbuf_pool_t *pool);

#define ogs_pkbuf_alloc(pool, size) \
    ogs_pkbuf_alloc_debug(pool, size, OGS_FILE_LINE)
ogs_pkbuf_t *ogs_pkbuf_alloc_debug(
        ogs_pkbuf_pool_t *pool, unsigned int size, const char *file_line);
void ogs_pkbuf_free(ogs_pkbuf_t *pkbuf);

void *ogs_pkbuf_put_data(
        ogs_pkbuf_t *pkbuf, const void *data, unsigned int len);
#define ogs_pkbuf_copy(pkbuf) \
    ogs_pkbuf_copy_debug(pkbuf, OGS_FILE_LINE)
ogs_pkbuf_t *ogs_pkbuf_copy_debug(ogs_pkbuf_t *pkbuf, const char *file_line);

static ogs_inline int ogs_pkbuf_tailroom(const ogs_pkbuf_t *pkbuf)
{
    return pkbuf->end - pkbuf->tail;
}

static ogs_inline int ogs_pkbuf_headroom(const ogs_pkbuf_t *pkbuf)
{
    return pkbuf->data - pkbuf->head;
}

static ogs_inline void ogs_pkbuf_reserve(ogs_pkbuf_t *pkbuf, int len)
{
    pkbuf->data += len;
    pkbuf->tail += len;
}

static ogs_inline void *ogs_pkbuf_put(ogs_pkbuf_t *pkbuf, unsigned int len)
{
    void *tmp = pkbuf->tail;

    if (ogs_unlikely(ogs_pkbuf_tailroom(pkbuf) < (int)len))
        ogs_assert_if_reached();

    pkbuf->tail += len;
    pkbuf->len += len;

    return tmp;
}

static ogs_inline void ogs_pkbuf_put_u8(ogs_pkbuf_t *pkbuf, uint8_t val)
{
    *(uint8_t *)ogs_pkbuf_put(pkbuf, 1) = val;
}

static ogs_inline void ogs_pkbuf_put_u16(ogs_pkbuf_t *pkbuf, uint16_t val)
{
    uint8_t *p = ogs_pkbuf_put(pkbuf, 2);
    uint16_t tmp = htobe16(val);
    memcpy(p, &tmp, 2);
}

static ogs_inline void ogs_pkbuf_put_u32(ogs_pkbuf_t *pkbuf, uint32_t val)
{
    uint8_t *p = ogs_pkbuf_put(pkbuf, 4);
    uint32_t tmp = htobe32(val);
    memcpy(p, &tmp, 4);
}

static ogs_inline void *ogs_pkbuf_push(ogs_pkbuf_t *pkbuf, unsigned int len)
{
    if (ogs_unlikely(ogs_pkbuf_headroom(pkbuf) < (int)len))
        ogs_assert_if_reached();

    pkbuf->data -= len;
    pkbuf->len += len;

    return pkbuf->data;
}

static ogs_inline void *ogs_pkbuf_pull_inline(
        ogs_pkbuf_t *pkbuf, unsigned int len)
{
    pkbuf->len -= len;
    return pkbuf->data += len;
}

static ogs_inline void *ogs_pkbuf_pull(ogs_pkbuf_t *pkbuf, unsigned int len)
{
    return ogs_unlikely(len > pkbuf->len) ?
        NULL : ogs_pkbuf_pull_inline(pkbuf, len);
}

static ogs_inline int ogs_pkbuf_trim(ogs_pkbuf_t *pkbuf, int len)
{
    if (ogs_unlikely(len < 0))
        ogs_assert_if_reached();
    if (ogs_unlikely(len > pkbuf->len)) {
        ogs_error("len(%d) > pkbuf->len(%d)", len, pkbuf->len);
        return OGS_ERROR;
    }

    pkbuf->tail = pkbuf->data + len;
    pkbuf->len = len;

    return OGS_OK;
}

#ifdef __cplusplus
}
#endif

#endif /* OGS_PKBUF_H */
