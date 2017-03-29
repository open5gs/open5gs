#define TRACE_MODULE _gtp_xact
#include "core_debug.h"
#include "core_pool.h"
#include "core_event.h"

#include "gtp_xact.h"
#include "gtp_tlv.h"
#include "gtp_types.h"

#define SIZE_OF_GTP_XACT_POOL  32

#define GTP_XACT_NEXT_ID(__id) \
    ((__id) == 0x800000 ? 1 : ((__id) + 1))
#define GTP_XACT_COMPARE_ID(__id1, __id2) \
    ((__id2) > (__id1) ? ((__id2) - (__id1) < 0x7fffff ? -1 : 1) : \
     (__id1) > (__id2) ? ((__id1) - (__id2) < 0x7fffff ? 1 : -1) : 0)

#define GTPV2C_HEADER_LEN   12
#define GTPV2C_TEID_LEN     4

#define GTP_XID_TO_SQN(__xid) ((__xid) << 8)
#define GTP_SQN_TO_XID(__sqn) ((__sqn) >> 8)

static int gtp_xact_pool_initialized = 0;
pool_declare(gtp_xact_pool, gtp_xact_t, SIZE_OF_GTP_XACT_POOL);

/**
 * Initialize the transaction framework
 */
status_t gtp_xact_init(gtp_xact_ctx_t *context, tm_service_t *tm_service, 
        c_uintptr_t event, c_uint32_t duration, int retry_count)
{
    if (gtp_xact_pool_initialized == 0)
    {
        pool_init(&gtp_xact_pool, SIZE_OF_GTP_XACT_POOL);
    }
    gtp_xact_pool_initialized = 1;

    memset(context, 0, sizeof(gtp_xact_ctx_t));

    context->g_xact_id = 1;

    context->tm_service = tm_service;
    context->event = event;;
    context->duration = duration;;
    context->retry_count = retry_count;;

    return CORE_OK;
}

/**
 * Finalize the transaction framework
 */
status_t gtp_xact_final(void)
{
    if (gtp_xact_pool_initialized == 1)
    {
        pool_final(&gtp_xact_pool);
    }
    gtp_xact_pool_initialized = 0;

    return CORE_OK;
}

/**
 * Create a new transaction which was initiated by local ASN node.
 */
gtp_xact_t *gtp_xact_new_local(gtp_xact_ctx_t *context, 
    c_uint8_t type, net_sock_t *sock, gtp_node_t *gnode, pkbuf_t *pkbuf)
{
    gtp_xact_t *xact = NULL;

    d_assert(context, return NULL, "Null param");
    d_assert(sock, return NULL, "Null param");
    d_assert(gnode, return NULL, "Null param");
    d_assert(pkbuf, return NULL, "Null param");

    pool_alloc_node(&gtp_xact_pool, &xact);
    d_assert(xact, return NULL, "Transaction allocation failed");
    memset(xact, 0, sizeof(gtp_xact_t));

    xact->xid = GTP_XACT_NEXT_ID(context->g_xact_id);
    xact->org = GTP_LOCAL_ORIGINATOR;
    xact->type = type;
    xact->sock = sock;
    xact->gnode = gnode;
    xact->pkbuf = pkbuf;

    xact->tm_wait = event_timer(context->tm_service, context->event, 
            context->duration, (c_uintptr_t)xact);
    d_assert(xact->tm_wait, return NULL, "Timer allocation failed");
    xact->retry_count = context->retry_count;

    list_append(&gnode->local_xlist, xact);

    return xact;
}

/**
 * Create a new transaction which was initiated by remote node
 */
gtp_xact_t *gtp_xact_new_remote(gtp_xact_ctx_t *context, 
    net_sock_t *sock, gtp_node_t *gnode, pkbuf_t *pkbuf)
{
    gtpv2c_header_t *h = NULL;
    gtp_xact_t *xact = NULL;

    d_assert(sock, return NULL, "Null param");
    d_assert(gnode, return NULL, "Null param");
    d_assert(pkbuf, return NULL, "Null param");

    h = pkbuf->payload;
    d_assert(h, return NULL, "Null param");

    pool_alloc_node(&gtp_xact_pool, &xact);
    d_assert(xact, return NULL, "Transaction allocation failed");
    memset(xact, 0, sizeof(gtp_xact_t));

    xact->xid = GTP_SQN_TO_XID(h->sqn);
    xact->org = GTP_REMOTE_ORIGINATOR;
    xact->type = h->type;
    xact->sock = sock;
    xact->gnode = gnode;
    xact->pkbuf = pkbuf;

    xact->tm_wait = event_timer(context->tm_service, context->event, 
            context->duration * context->retry_count, (c_uintptr_t)xact);
    d_assert(xact->tm_wait, return NULL, "Timer allocation failed");
    xact->retry_count = 1;

    tm_start(xact->tm_wait);

    list_append(&gnode->remote_xlist, xact);

    return xact;
}
/**
 * Delete a transaction
 */
status_t gtp_xact_delete(gtp_xact_t *xact)
{
    d_assert(xact, return CORE_ERROR, "Null param");
    d_assert(xact->tm_wait, return CORE_ERROR, "Null param");
    d_assert(xact->gnode, return CORE_ERROR, "Null param");

    tm_delete(xact->tm_wait);

    list_remove(xact->org == GTP_LOCAL_ORIGINATOR ?  &xact->gnode->local_xlist :
            &xact->gnode->remote_xlist, xact);
    pool_free_node(&gtp_xact_pool, xact);

    return CORE_OK;
}

/**
 * Update the transaction with the new packet to be sent for the next step
 */
status_t gtp_xact_update_tx(gtp_xact_t *xact, pkbuf_t *pkb)
{
    return CORE_OK;
}

/**
 * Update the transaction with the new received packet for the next step
 */
status_t gtp_xact_update_rx(gtp_xact_t *xact)
{
    return CORE_OK;
}

/**
 * Apply and commit the updated of the transcation
 */
status_t gtp_xact_commit(gtp_xact_t *xact)
{
    return CORE_OK;
}

/**
 * Find the transaction with the given ASN header
 */
gtp_xact_t *gtp_xact_find(gtp_node_t *gnode, pkbuf_t *pkbuf)
{
    gtpv2c_header_t *h = NULL;
    c_uint32_t xid;
    gtp_xact_t *xact = NULL;

    d_assert(gnode, return NULL, "Null param");
    d_assert(pkbuf, return NULL, "Null param");
    d_assert(pkbuf->payload, return NULL, "Null param");

    h = pkbuf->payload;
    switch(h->type)
    {
        case GTP_CREATE_SESSION_REQUEST_TYPE:
        case GTP_MODIFY_BEARER_REQUEST_TYPE:
        case GTP_DELETE_SESSION_REQUEST_TYPE:
        case GTP_CREATE_BEARER_REQUEST_TYPE:
        case GTP_UPDATE_BEARER_REQUEST_TYPE:
        case GTP_DELETE_BEARER_REQUEST_TYPE:
        case GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE:
        case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
        case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
            xact = list_first(&gnode->local_xlist);
            break;

        case GTP_CREATE_SESSION_RESPONSE_TYPE:
        case GTP_MODIFY_BEARER_RESPONSE_TYPE:
        case GTP_DELETE_SESSION_RESPONSE_TYPE:
        case GTP_CREATE_BEARER_RESPONSE_TYPE:
        case GTP_UPDATE_BEARER_RESPONSE_TYPE:
        case GTP_DELETE_BEARER_RESPONSE_TYPE:
        case GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
        case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
        case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
            xact = list_first(&gnode->remote_xlist);
            break;

        default:
            d_error("Not implemented GTPv2 Message Type(%d)", h->type);
            return NULL;
    }

    xid = GTP_SQN_TO_XID(h->sqn);
    while(xact)
    {
        if (xact->xid == xid)
            break;
        xact = list_next(xact);
    }

    return xact;
}
