#define TRACE_MODULE _gtp_xact
#include "core_debug.h"
#include "core_pool.h"

#include "gtp_xact.h"

#define SIZE_OF_GTP_XACT_POOL  32

#define GTP_XACT_NEXT_ID(__id) \
    ((__id) == 0x800000 ? 1 : ((__id) + 1))
#define GTP_XACT_COMPARE_ID(__id1, __id2) \
    ((__id2) > (__id1) ? ((__id2) - (__id1) < 0x7fffff ? -1 : 1) : \
     (__id1) > (__id2) ? ((__id1) - (__id2) < 0x7fffff ? 1 : -1) : 0)

static int g_gtp_xact_initialized = 0;

pool_declare(gtp_xact_pool, gtp_xact_ctx_t, SIZE_OF_GTP_XACT_POOL);

/**
 * Initialize the transaction framework
 */
status_t gtp_xact_init(void)
{
    d_assert(g_gtp_xact_initialized == 0, return CORE_ERROR,
            "XACTION already has been initialized");

    pool_init(&gtp_xact_pool, SIZE_OF_GTP_XACT_POOL);

    g_gtp_xact_initialized = 1;

    return CORE_OK;
}

/**
 * Finalize the transaction framework
 */
status_t gtp_xact_final(void)
{
    d_assert(g_gtp_xact_initialized == 1, return CORE_ERROR,
            "XACTION ASNGW context already has been finalized");

    pool_final(&gtp_xact_pool);

    g_gtp_xact_initialized = 0;

    return CORE_OK;
}

/**
 * Create a new transaction which was initiated by local ASN node.
 */
status_t gtp_xact_new_local(gtp_xact_ctx_t **xact, c_uint8_t type, 
    pkbuf_t *pkb, gtp_node_t *gnode)
{
    return CORE_OK;
}

/**
 * Create a new transaction which was initiated by remote node
 */
status_t gtp_xact_new_remote(gtp_xact_ctx_t **xact)
{
    return CORE_OK;
}
/**
 * Delete a transaction
 */
status_t gtp_xact_delete(gtp_xact_ctx_t *xact)
{
    return CORE_OK;
}

/**
 * Update the transaction with the new packet to be sent for the next step
 */
status_t gtp_xact_update_tx(gtp_xact_ctx_t *xact, pkbuf_t *pkb)
{
    return CORE_OK;
}

/**
 * Update the transaction with the new received packet for the next step
 */
status_t gtp_xact_update_rx(gtp_xact_ctx_t *xact)
{
    return CORE_OK;
}

/**
 * Apply and commit the updated of the transcation
 */
status_t gtp_xact_commit(gtp_xact_ctx_t *xact)
{
    return CORE_OK;
}

/**
 * Find the transaction with the given ASN header
 */
gtp_xact_ctx_t * gtp_xact_find()
{
    return CORE_OK;
}
