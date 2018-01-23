#define TRACE_MODULE _pkbuf

#include "core.h"
#include "core_pkbuf.h"
#include "core_errno.h"
#include "core_lib.h"
#include "core_debug.h"
#include "core_pool.h"

#define MAX_NUM_OF_CLBUF 256
#define MAX_NUM_OF_PKBUF 256

pool_declare(clbuf_pool, clbuf_t, MAX_NUM_OF_CLBUF);
pool_declare(pkbuf_pool, pkbuf_t, MAX_NUM_OF_PKBUF);

#undef BOUNDARY
#define BOUNDARY                SIZEOF_VOIDP

#define SIZEOF_CLUSTER_128      CORE_ALIGN(128+MAX_SIZEOF_HEADROOM, BOUNDARY)
#define SIZEOF_CLUSTER_256      CORE_ALIGN(256+MAX_SIZEOF_HEADROOM, BOUNDARY)
#define SIZEOF_CLUSTER_512      CORE_ALIGN(512+MAX_SIZEOF_HEADROOM, BOUNDARY)
#define SIZEOF_CLUSTER_1024     CORE_ALIGN(1024+MAX_SIZEOF_HEADROOM, BOUNDARY)
#define SIZEOF_CLUSTER_2048     CORE_ALIGN(2048+MAX_SIZEOF_HEADROOM, BOUNDARY)
#define SIZEOF_CLUSTER_8192     CORE_ALIGN(8192+MAX_SIZEOF_HEADROOM, BOUNDARY)

#define MAX_NUM_OF_CLUSTER_128      256
#define MAX_NUM_OF_CLUSTER_256      256 
#define MAX_NUM_OF_CLUSTER_512      256
#define MAX_NUM_OF_CLUSTER_1024     256
#define MAX_NUM_OF_CLUSTER_2048     256
#define MAX_NUM_OF_CLUSTER_8192     256

typedef c_uint8_t cluster_128_t[SIZEOF_CLUSTER_128];
typedef c_uint8_t cluster_256_t[SIZEOF_CLUSTER_256];
typedef c_uint8_t cluster_512_t[SIZEOF_CLUSTER_512];
typedef c_uint8_t cluster_1024_t[SIZEOF_CLUSTER_1024];
typedef c_uint8_t cluster_2048_t[SIZEOF_CLUSTER_2048];
typedef c_uint8_t cluster_8192_t[SIZEOF_CLUSTER_8192];

pool_declare(cluster_128_pool, cluster_128_t, MAX_NUM_OF_CLUSTER_128);
pool_declare(cluster_256_pool, cluster_256_t, MAX_NUM_OF_CLUSTER_256);
pool_declare(cluster_512_pool, cluster_512_t, MAX_NUM_OF_CLUSTER_512);
pool_declare(cluster_1024_pool, cluster_1024_t, MAX_NUM_OF_CLUSTER_1024);
pool_declare(cluster_2048_pool, cluster_2048_t, MAX_NUM_OF_CLUSTER_2048);
pool_declare(cluster_8192_pool, cluster_8192_t, MAX_NUM_OF_CLUSTER_8192);

static mutex_id mutex;

status_t pkbuf_init(void)
{
    mutex_create(&mutex, MUTEX_DEFAULT);

    pool_init(&clbuf_pool, MAX_NUM_OF_CLBUF);
    pool_init(&pkbuf_pool, MAX_NUM_OF_PKBUF);

    pool_init(&cluster_128_pool, MAX_NUM_OF_CLUSTER_128);
    pool_init(&cluster_256_pool, MAX_NUM_OF_CLUSTER_256);
    pool_init(&cluster_512_pool, MAX_NUM_OF_CLUSTER_512);
    pool_init(&cluster_1024_pool, MAX_NUM_OF_CLUSTER_1024);
    pool_init(&cluster_2048_pool, MAX_NUM_OF_CLUSTER_2048);
    pool_init(&cluster_8192_pool, MAX_NUM_OF_CLUSTER_8192);

    return CORE_OK;
}
status_t pkbuf_final(void)
{
    pkbuf_show();

    pool_final(&clbuf_pool);
    pool_final(&pkbuf_pool);

    pool_final(&cluster_128_pool);
    pool_final(&cluster_256_pool);
    pool_final(&cluster_512_pool);
    pool_final(&cluster_1024_pool);
    pool_final(&cluster_2048_pool);
    pool_final(&cluster_8192_pool);

    mutex_delete(mutex);

    return CORE_OK;
}

void pkbuf_show(void)
{
    if (pool_used(&pkbuf_pool))
        d_error("%d not freed in pkbuf_pool[%d]",
            pool_used(&pkbuf_pool), pool_size(&pkbuf_pool));
    d_trace(9, "%d not freed in pkbuf_pool[%d]\n",
            pool_used(&pkbuf_pool), pool_size(&pkbuf_pool));

    if (pool_used(&clbuf_pool))
        d_error("%d not freed in clbuf_pool[%d]",
            pool_used(&clbuf_pool), pool_size(&clbuf_pool));
    d_trace(9, "%d not freed in clbuf_pool[%d]\n",
            pool_used(&clbuf_pool), pool_size(&clbuf_pool));

    if (pool_used(&cluster_128_pool))
        d_error("%d not freed in cluster128_pool[%d]",
            pool_used(&cluster_128_pool), pool_size(&cluster_128_pool));
    d_trace(9, "%d not freed in cluster128_pool[%d]\n",
            pool_used(&cluster_128_pool), pool_size(&cluster_128_pool));

    if (pool_used(&cluster_256_pool))
        d_error("%d not freed in cluster256_pool[%d]",
            pool_used(&cluster_256_pool), pool_size(&cluster_256_pool));
    d_trace(9, "%d not freed in cluster256_pool[%d]\n",
            pool_used(&cluster_256_pool), pool_size(&cluster_256_pool));

    if (pool_used(&cluster_512_pool))
        d_error("%d not freed in cluster512_pool[%d]",
            pool_used(&cluster_512_pool), pool_size(&cluster_512_pool));
    d_trace(9, "%d not freed in cluster512_pool[%d]\n",
            pool_used(&cluster_512_pool), pool_size(&cluster_512_pool));

    if (pool_used(&cluster_1024_pool))
        d_error("%d not freed in cluster1024_pool[%d]",
            pool_used(&cluster_1024_pool), pool_size(&cluster_1024_pool));
    d_trace(9, "%d not freed in cluster1024_pool[%d]\n",
            pool_used(&cluster_1024_pool), pool_size(&cluster_1024_pool));

    if (pool_used(&cluster_2048_pool))
        d_error("%d not freed in cluster2048_pool[%d]",
            pool_used(&cluster_2048_pool), pool_size(&cluster_2048_pool));
    d_trace(9, "%d not freed in cluster2048_pool[%d]\n",
            pool_used(&cluster_2048_pool), pool_size(&cluster_2048_pool));

    if (pool_used(&cluster_8192_pool))
        d_error("%d not freed in cluster8192_pool[%d]",
            pool_used(&cluster_8192_pool), pool_size(&cluster_8192_pool));
    d_trace(9, "%d not freed in cluster8192_pool[%d]\n",
            pool_used(&cluster_8192_pool), pool_size(&cluster_8192_pool));
}

static clbuf_t* clbuf_alloc(c_uint16_t length);
static void clbuf_free(clbuf_t *clbuf);

static clbuf_t* clbuf_alloc(c_uint16_t length)
{
    clbuf_t *clbuf = NULL;
    c_uint8_t *cluster = NULL;

    pool_alloc_node(&clbuf_pool, &clbuf);
    d_assert(clbuf, return NULL, "No more free clbuf. ");

    if (length <= 128)
    {
        pool_alloc_node(&cluster_128_pool, &cluster);
        clbuf->size = SIZEOF_CLUSTER_128;
    }
    else if (length <= 256)
    {
        pool_alloc_node(&cluster_256_pool, &cluster);
        clbuf->size = SIZEOF_CLUSTER_256;
    }
    else if (length <= 512)
    {
        pool_alloc_node(&cluster_512_pool, &cluster);
        clbuf->size = SIZEOF_CLUSTER_512;
    }
    else if (length <= 1024)
    {
        pool_alloc_node(&cluster_1024_pool, &cluster);
        clbuf->size = SIZEOF_CLUSTER_1024;
    }
    else if (length <= 2048)
    {
        pool_alloc_node(&cluster_2048_pool, &cluster);
        clbuf->size = SIZEOF_CLUSTER_2048;
    }
    else if (length <= 8192)
    {
        pool_alloc_node(&cluster_8192_pool, &cluster);
        clbuf->size = SIZEOF_CLUSTER_8192;
    }

    d_assert(cluster, pool_free_node(&clbuf_pool, clbuf); return NULL,
            "No more free cluster. length:%d requested", length);

    clbuf->ref = 0;
    clbuf->cluster = cluster;

    return clbuf;
}

static void clbuf_free(clbuf_t *clbuf)
{
    d_assert(clbuf, return, "Null param");
    d_assert(clbuf->cluster, return, "clbuf has no cluster");

    switch (clbuf->size)
    {
        case SIZEOF_CLUSTER_128:
            pool_free_node(&cluster_128_pool, clbuf->cluster);
            break;
        case SIZEOF_CLUSTER_256:
            pool_free_node(&cluster_256_pool, clbuf->cluster);
            break;
        case SIZEOF_CLUSTER_512:
            pool_free_node(&cluster_512_pool, clbuf->cluster);
            break;
        case SIZEOF_CLUSTER_1024:
            pool_free_node(&cluster_1024_pool, clbuf->cluster);
            break;
        case SIZEOF_CLUSTER_2048:
            pool_free_node(&cluster_2048_pool, clbuf->cluster);
            break;
        case SIZEOF_CLUSTER_8192:
            pool_free_node(&cluster_8192_pool, clbuf->cluster);
            break;
        default:
            d_assert(0, return, "clbuf has invalid size %d", clbuf->size);
            break;
    }

    pool_free_node(&clbuf_pool, clbuf);

    return;
}

pkbuf_t* pkbuf_alloc(c_uint16_t headroom, c_uint16_t length)
{
    pkbuf_t *np = NULL, *pnp, *ret;
    clbuf_t *clbuf = NULL;
    c_uint16_t rem_length;

    d_assert(headroom <= MAX_SIZEOF_HEADROOM, return NULL,
            "Max size of headroom is %d, but %d requested",
            MAX_SIZEOF_HEADROOM, headroom);

    clbuf = clbuf_alloc(length);
    d_assert(clbuf, return NULL, "Can't allocate clbuf(length:%d)", length);

    pool_alloc_node(&pkbuf_pool, &np);
    d_assert(np, clbuf_free(clbuf); return NULL, "No more free pkbuf");
    ret = np;

    np->next = NULL;
    np->clbuf = clbuf;
    np->payload = (void*)CORE_ALIGN((c_uintptr_t)(clbuf->cluster + headroom),
                                    BOUNDARY);
    np->tot_len = length;
    np->len = c_min(length, clbuf->size - (np->payload - clbuf->cluster));
    np->flags = 0;
    clbuf->ref = 1;

    pnp = np;
    rem_length = length - np->len;

    while (rem_length > 0)
    {
        clbuf = clbuf_alloc(rem_length);
        d_assert(clbuf, break, "Can't allocate clbuf");

        pool_alloc_node(&pkbuf_pool, &np);
        d_assert(np, clbuf_free(clbuf); break, "No more free pkbuf");

        /* Chaining */
        pnp->next = np;

        np->next = NULL;
        np->clbuf = clbuf;
        np->payload = clbuf->cluster;
        np->tot_len = rem_length;
        np->len = c_min(rem_length, clbuf->size);
        np->flags = 0;
        clbuf->ref = 1;

        pnp = np;
        rem_length -= np->len;
    }

    /* Abnormal break */
    if (rem_length > 0)
    {
        if (ret)
            pkbuf_free(ret);
        ret = NULL;
    }

    return ret;
}

status_t pkbuf_header(pkbuf_t *pkbuf, c_int16_t increment)
{
    clbuf_t *clbuf;

    d_assert(pkbuf, return CORE_ERROR, "Null param");
    d_assert(pkbuf->clbuf, return CORE_ERROR, "pkbuf has no clbuf");
    d_assert(pkbuf->clbuf->cluster, return CORE_ERROR, "clbuf has no cluster");

    clbuf = pkbuf->clbuf;

    /* No change */
    if (increment == 0)
        return CORE_OK;

    if (increment > 0)
    {
        if (pkbuf->payload - clbuf->cluster < increment)
            return CORE_ERROR;
    }
    else
    {
        if (pkbuf->len < -increment)
            return CORE_ERROR;
    }

    pkbuf->payload -= increment;
    pkbuf->tot_len += increment;
    pkbuf->len += increment;

    return CORE_OK;
}

void pkbuf_free(pkbuf_t *pkbuf)
{
    pkbuf_t *p, *q;

    d_assert(pkbuf, return, "Null param");

    p = pkbuf;
    while (p)
    {
        d_assert(p->clbuf, return, "param 'pkbuf' has no clbuf");

        q = p->next;

        mutex_lock(mutex);
        p->clbuf->ref--;
        mutex_unlock(mutex);

        if (p->clbuf->ref == 0)
            clbuf_free(p->clbuf);

        pool_free_node(&pkbuf_pool, p);

        p = q;
    }

    return;
}

void pkbuf_join(pkbuf_t *h, pkbuf_t *t)
{
    pkbuf_t *p;

    d_assert(h, return, "Null param");
    d_assert(t, return, "Null param");

    /* proceed to last pbuf of chain */
    for (p = h; p->next != NULL; p = p->next) 
    {
        /* add total length of second chain to all totals of first chain */
        p->tot_len += t->tot_len;
    }

    d_assert(p->tot_len == p->len, return, 
            "p->tot_len(%d) == p->len(%d) (of last pbuf in chain)",
            p->tot_len, p->len);

    d_assert(p->next == NULL, return, "p->next == NULL");

    /* add total length of second chain to last pbuf total of first chain */
    p->tot_len += t->tot_len;

    /* chain last pbuf of head (p) with first of tail (t) */
    p->next = t;

    /* p->next now references t, but the caller will drop its reference to t,
     * so netto there is no change to the reference count of t. */
}

pkbuf_t* pkbuf_copy(pkbuf_t *pkbuf)
{
    pkbuf_t *p, *np, *pnp = NULL, *ret = NULL;

    d_assert(pkbuf, return NULL, "Null param");

    p = pkbuf;

    while (p)
    {
        pool_alloc_node(&pkbuf_pool, &np);
        d_assert(np, break, "No more free pkbuf. ");
        if (ret == NULL) ret = np;

        if (pnp)
            pnp->next = np;

        np->next = NULL;
        np->clbuf = p->clbuf;
        np->payload = p->payload;
        np->tot_len = p->tot_len;
        np->len = p->len;
        np->flags = p->flags;

        mutex_lock(mutex);
        p->clbuf->ref++;
        mutex_unlock(mutex);

        pnp = np;
        p = p->next;
    }

    /* Abnormal break */
    if (p)
    {
        if (ret)
            pkbuf_free(ret);
        ret = NULL;
    }

    return ret;
}

pkbuf_t* pkbuf_copy_partial(pkbuf_t *pkbuf, c_uint16_t offset, c_uint16_t len)
{
    pkbuf_t *p, *np, *pnp = NULL, *ret = NULL;
    c_uint16_t copied = 0, bytes = 0, skipped = 0;

    d_assert(pkbuf, return NULL, "Null param");

    if (pkbuf->tot_len < offset + len)
        return NULL;

    p = pkbuf;

    while (p)
    {
        bytes += p->len;

        if (bytes > offset)
        {
            pool_alloc_node(&pkbuf_pool, &np);
            d_assert(np, break, "No more free pkbuf. ");

            /* First block */
            if (ret == NULL)
            {
                ret = np;
                np->payload = p->payload + (offset - skipped);
                np->tot_len = len;
                np->len = p->len - (offset - skipped);
            }
            else
            {
                np->payload = p->payload;
                np->tot_len = pnp->tot_len - pnp->len;
                np->len = p->len;
            }

            np->next = NULL;
            np->flags = p->flags;
            np->clbuf = p->clbuf;

            mutex_lock(mutex);
            p->clbuf->ref++;
            mutex_unlock(mutex);

            if (pnp)
                pnp->next = np;
            pnp = np;

            copied += np->len;

            /* Check the last block */
            if (copied >= len)
            {
                np->len -= copied - len;
                break;
            }
        }

        skipped += p->len;

        p = p->next;
    }

    /* Abnormal break */
    if (copied < len)
    {
        if (ret)
            pkbuf_free(ret);
        ret = NULL;
    }

    return ret;
}

status_t pkbuf_tobuf(pkbuf_t *pkbuf, void *buf, c_uint16_t *buflen)
{
    pkbuf_t *p;
    c_uint16_t copied = 0;

    d_assert(pkbuf, return CORE_ERROR, "Null param");
    d_assert(buf, return CORE_ERROR, "Null param");
    d_assert(buflen, return CORE_ERROR, "Null param");

    if (pkbuf->tot_len > *buflen)
        return CORE_ERROR;

    *buflen = 0;

    p = pkbuf;

    while (p)
    {
        d_assert(p->clbuf, return CORE_ERROR, "pkbuf has no clbuf");
        d_assert(p->clbuf->cluster, return CORE_ERROR, "clbuf has no cluster");

        memcpy(buf + copied, p->payload, p->len);
        copied += p->len;

        p = p->next;
    }

    d_assert(copied == pkbuf->tot_len, return CORE_ERROR,
            "Copy length isn't same with total length");

    *buflen = copied;

    return CORE_OK;;
}

status_t pkbuf_tobuf_partial(pkbuf_t *pkbuf, void *buf, c_uint16_t *buflen,
        c_uint16_t offset, c_uint16_t len)
{

    return CORE_OK;
}

void *core_malloc(size_t size)
{
    c_uint16_t headroom = 0;
    pkbuf_t *p = NULL;

    d_assert(size, return NULL, "if size == 0, then returns NULL");
    headroom = sizeof(pkbuf_t *);
    p = pkbuf_alloc(headroom, size);
    d_assert(p, return NULL, "pkbuf_alloc failed(headroom:%d, size:%d)", 
            headroom, size);
    d_assert(p->next == NULL, pkbuf_free(p);return NULL, 
            "core_malloc should not be fragmented");

    memcpy(p->payload - headroom, &p, headroom);

    return p->payload;
}

status_t core_free(void *ptr)
{
    c_uint16_t headroom = sizeof(pkbuf_t *);
    pkbuf_t *p = NULL;

    if (!ptr) 
        return CORE_OK;

    memcpy(&p, ptr - headroom, headroom);
    d_assert(p, return CORE_ERROR, "Null param");

    pkbuf_free(p);

    return CORE_OK;
}

void *core_calloc(size_t nmemb, size_t size)
{
    void *ptr = NULL;

    ptr = core_malloc(nmemb * size);
    d_assert(ptr, return NULL, "nmeb = %d, sizeo = %d", nmemb, size);

    memset(ptr, 0, nmemb * size);
    return ptr;
}

void *core_realloc(void *ptr, size_t size)
{
    c_uint16_t headroom = sizeof(pkbuf_t *);
    pkbuf_t *p = NULL;

    if (!ptr) 
    {
        return core_malloc(size);
    }

    memcpy(&p, ptr - headroom, headroom);
    d_assert(p, return NULL, "Null param");

    if (!size)
    {
        pkbuf_free(p);
        return NULL;
    }

    d_assert(p->clbuf, return NULL, "Null param");
    if (size >= (p->clbuf->size - headroom))
    {
        void *new = NULL;

        new = core_malloc(size);
        d_assert(new, return NULL, "Null param");
        memcpy(new, ptr, p->len);

        pkbuf_free(p);

        return new;
    }
    else
    {
        p->tot_len = size;
        p->len = c_min(size, p->clbuf->size - (p->payload - p->clbuf->cluster));
        return ptr;
    }
}

char *core_strdup(const char *s)
{
    char *res;
    size_t len;

    if (s == NULL)
        return NULL;

    len = strlen(s) + 1;
    res = core_memdup(s, len);
    return res;
}

char *core_strndup(const char *s, size_t n)
{
    char *res;
    const char *end;

    if (s == NULL)
        return NULL;

    end = memchr(s, '\0', n);
    if (end != NULL)
        n = end - s;
    res = core_malloc(n + 1);
    memcpy(res, s, n);
    res[n] = '\0';
    return res;
}

void *core_memdup(const void *m, size_t n)
{
    void *res;

    if (m == NULL)
        return NULL;

    res = core_malloc(n);
    memcpy(res, m, n);
    return res;
}
