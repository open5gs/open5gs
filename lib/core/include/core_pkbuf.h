#ifndef __CORE_PKBUF_H__
#define __CORE_PKBUF_H__

/**
 * @file core_pkbuf.h
 * @brief Packet Buffer Routines
 */

#include "core.h"
#include "core_debug.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Cluster buffer structre */
typedef struct _clbuf_t {

    /**
     * reference count */
    c_uint16_t ref;

    /**
     * pointer to cluster */
    void *cluster;

    /**
     * the size of cluster */
    c_uint16_t size;

} clbuf_t;

/**
 * Packet buffer structure */
typedef struct _pkbuf_t {

    /** next pkbuf in singly linked pkbuf chain */
    struct _pkbuf_t *next;

    /** pointer to cluster */
    clbuf_t *clbuf;

    /** this buffer */
    void *payload;

    /**
     * total length of this buffer and all next buffers in chain
     * belonging to the same packet.
     *
     * For non-queue packet chains this is the invariant:
     * p->tot_len == p->len + (p->next? p->next->tot_len: 0)
     */
    c_uint16_t tot_len;

    /** length of this buffer */
    c_uint16_t len;  

    /** misc flags */
    c_uint8_t flags;

} pkbuf_t;

/**
 * Initializes the packet buffer library */
CORE_DECLARE(status_t)  pkbuf_init(void);

/**
 * Finalizes the packet buffer library */
CORE_DECLARE(status_t)  pkbuf_final(void);

/**
 * Show the pkbuf/clbuf/cluster count */
CORE_DECLARE(void)  pkbuf_show(void);

/**
 * Allocates a pkbuf of the given type (possibly a chain).
 *
 * The actual memory allocated for the pkbuf is determined by the
 * headroom at which the pkbuf is allocated and the requested size
 * (from the headroom parameter).
 *
 * @param headroom define header size
 * @param type define header size
 * should be allocated as follows:
 *
 * @return the allocated pkbuf.  If multiple pkbufs where allocated, this
 * is the first pkbuf of a pkbuf chain.
 */
#define MAX_SIZEOF_HEADROOM 128
CORE_DECLARE(pkbuf_t *) pkbuf_alloc(c_uint16_t headroom, c_uint16_t length);

/**
 * Dereference a pkbuf chain and deallocate any no-longer-used
 * pkbufs at the head of this chain.
 *
 * Decrements the reference count of the pkbuf's cluster buffer. 
 * If it reaches zero, the pkbuf is deallocated.
 *
 * For a pkbuf chain, this is repeated for each pkbuf in the chain,
 * up to the first pkbuf which has a non-zero reference count after
 * decrementing. So, when all reference counts are one, the whole
 * chain is free'd.
 *
 * @param pkbuf The pkbuf (chain) to be dereferenced.
 *
 * @return the number of pkbufs that were de-allocated
 * from the head of the chain.
 */
CORE_DECLARE(void) pkbuf_free(pkbuf_t *pkbuf);

/**
 * Adjusts the payload pointer to hide or reveal headers in the payload.
 *
 * Adjusts the ->payload pointer so that space for a header
 * (dis)appears in the pkbuf payload.
 *
 * The ->payload, ->tot_len and ->len fields are adjusted.
 *
 * @param pkbuf pkbuf to change the header size.
 * @param size Number of bytes to increment header size which
 * increases the size of the pkbuf. New space is on the front.
 * (Using a negative value decreases the header size.)
 * If hdr_size_inc is 0, this function does nothing and returns succesful.
 *
 * @return non-zero on failure, zero on success.
 */
CORE_DECLARE(status_t)  pkbuf_header(pkbuf_t *pkbuf, c_int16_t size);

/**
 * Concatenate two pkbufs (each may be a pkbuf chain) and take over
 * the caller's reference of the tail pkbuf.
 * 
 * @note The caller MAY NOT reference the tail pkbuf afterwards.
 * Use pkbuf_chain() for that purpose.
 * 
 * @see pkbuf_chain()
 */
CORE_DECLARE(void) pkbuf_join(pkbuf_t *h, pkbuf_t *t);

/**
 * Create copies of pkbufs.
 *
 * @param pkbuf     pkbuf source of the copy
 */
CORE_DECLARE(pkbuf_t*) pkbuf_copy(pkbuf_t *pkbuf);

/**
 * Create copies of some part of pkbufs.
 *
 * @param pkbuf     pkbuf source of the copy
 * @param offset    offset into the packet buffer from where to begin copying
 *                  len bytes
 * @param len       length of data to copy
 */
CORE_DECLARE(pkbuf_t*) pkbuf_copy_partial(pkbuf_t *pkbuf,
        c_uint16_t offset, c_uint16_t len);

/**
 * Copy the whole contents of a packet buffer to an application supplied
 * buffer.
 *
 * @param pkbuf     pkbuf from which to copy data
 * @param buf       buffer to copy to
 * @param buflen    
 */
CORE_DECLARE(status_t) pkbuf_tobuf(pkbuf_t *pkbuf,
        void *buf, c_uint16_t *buflen);

/**
 * Copy part of the contents of a packet buffer to an application supplied
 * buffer.
 *
 * @param pkbuf     pkbuf from which to copy data
 * @param buf       buffer to copy to
 * @param buflen    
 * @param offset    offset into the packet buffer from where 
 *                  to begin copying len bytes
 * @param len       length of data to copy
 */
CORE_DECLARE(status_t) pkbuf_tobuf_partial(pkbuf_t *pkbuf,
        void *buf, c_uint16_t *buflen, c_uint16_t offset, c_uint16_t len);

/**
 * Get the total length of packet buffer */
#define pkbuf_length(__pkbuf) (__pkbuf) ? ((__pkbuf)->tot_len : -1)

/* memory handling library like OS-function(malloc,free) */
CORE_DECLARE(void *) core_malloc(size_t size);
#define CORE_FREE(__pTR)   d_assert(core_free(__pTR) == CORE_OK,,);
CORE_DECLARE(status_t) core_free(void *ptr);
CORE_DECLARE(void *) core_calloc(size_t nmemb, size_t size);
CORE_DECLARE(void *) core_realloc(void *ptr, size_t size);

CORE_DECLARE(char *) core_strdup(const char *s);
CORE_DECLARE(char *) core_strndup(const char *s, size_t n);
CORE_DECLARE(void *) core_memdup(const void *m, size_t n);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* ! __PKBUF_H__ */
