#define TRACE_MODULE _gtp_xact
#include "core_debug.h"
#include "core_pool.h"
#include "core_event.h"

#include "gtp_xact.h"
#include "gtp_tlv.h"

#define SIZE_OF_GTP_XACT_POOL  32

#define GTP_XACT_NEXT_ID(__id) \
    ((__id) == 0x800000 ? 1 : ((__id) + 1))
#define GTP_XACT_COMPARE_ID(__id1, __id2) \
    ((__id2) > (__id1) ? ((__id2) - (__id1) < 0x7fffff ? -1 : 1) : \
     (__id1) > (__id2) ? ((__id1) - (__id2) < 0x7fffff ? 1 : -1) : 0)

/* 5.1 General format */
#define GTPV2C_HEADER_LEN   12
#define GTPV2C_TEID_LEN     4
typedef struct _gtpv2c_header_t {
ED4(c_uint8_t version:3;,
    c_uint8_t piggybacked:1;,
    c_uint8_t teid_presence:1;,
    c_uint8_t spare1:3;)
    c_uint8_t type;
    c_uint16_t length;
    union {
        struct {
            c_uint32_t teid;
            /* sqn : 31bit ~ 8bit, spare : 7bit ~ 0bit */
#define GTP_XID_TO_SQN(__xid) ((__xid) << 8)
#define GTP_SQN_TO_XID(__sqn) ((__sqn) >> 8)
            c_uint32_t sqn; 
        };
        /* sqn : 31bit ~ 8bit, spare : 7bit ~ 0bit */
        c_uint32_t spare2;
    };
} __attribute__ ((packed)) gtpv2c_header_t;

static int g_gtp_xact_initialized = 0;
pool_declare(gtp_xact_pool, gtp_xact_ctx_t, SIZE_OF_GTP_XACT_POOL);

/**
 * Initialize the transaction framework
 */
status_t gtp_xact_init()
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
 * Config Transaction 
 */
void gtp_xact_config(gtp_xact_config_t *config, tm_service_t *tm_service, 
        c_uintptr_t event, c_uint32_t duration, int retry_count)
{
    memset(config, 0, sizeof(gtp_xact_config_t));

    config->g_xact_id = 1;

    config->tm_service = tm_service;
    config->event = event;;
    config->duration = duration;;
    config->retry_count = retry_count;;
}

/**
 * Create a new transaction which was initiated by local ASN node.
 */
status_t gtp_xact_new_local(gtp_xact_config_t *config, 
    gtp_xact_ctx_t **xact, c_uint8_t type, net_sock_t *sock, 
    gtp_node_t *gnode, pkbuf_t *pkbuf)
{
    gtp_xact_ctx_t *new = NULL;

    d_assert(xact, return CORE_ERROR, "Null param");
    d_assert(sock, return CORE_ERROR, "Null param");
    d_assert(gnode, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    pool_alloc_node(&gtp_xact_pool, &new);
    d_assert(new, return CORE_ERROR, "Transaction allocation failed");
    memset(new, 0, sizeof(gtp_xact_ctx_t));

    new->xid = GTP_XACT_NEXT_ID(config->g_xact_id);
    new->org = GTP_LOCAL_ORIGINATOR;
    new->type = type;
    new->sock = sock;
    new->gnode = gnode;
    new->pkbuf = pkbuf;

    new->tm_wait = event_timer(config->tm_service, config->event, 
            config->duration, (c_uintptr_t)new);
    d_assert(new->tm_wait, return CORE_ERROR, "Timer allocation failed");
    new->retry_count = config->retry_count;

    list_append(&gnode->local_xlist, new);
    *xact = new;

    return CORE_OK;
}

/**
 * Create a new transaction which was initiated by remote node
 */
status_t gtp_xact_new_remote(gtp_xact_config_t *config, 
    gtp_xact_ctx_t **xact, net_sock_t *sock, gtp_node_t *gnode, pkbuf_t *pkbuf)
{
    gtpv2c_header_t *h = NULL;
    gtp_xact_ctx_t *new = NULL;

    d_assert(xact, return CORE_ERROR, "Null param");
    d_assert(sock, return CORE_ERROR, "Null param");
    d_assert(gnode, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");
    d_assert(pkbuf->payload, return CORE_ERROR, "Null param");

    pool_alloc_node(&gtp_xact_pool, &new);
    d_assert(new, return CORE_ERROR, "Transaction allocation failed");
    memset(new, 0, sizeof(gtp_xact_ctx_t));

    h = pkbuf->payload;

    new->xid = GTP_SQN_TO_XID(h->sqn);
    new->org = GTP_REMOTE_ORIGINATOR;
    new->type = h->type;
    new->sock = sock;
    new->gnode = gnode;
    new->pkbuf = pkbuf;

    new->tm_wait = event_timer(config->tm_service, config->event, 
            config->duration * config->retry_count, (c_uintptr_t)new);
    d_assert(new->tm_wait, return CORE_ERROR, "Timer allocation failed");
    new->retry_count = 1;

    list_append(&gnode->remote_xlist, new);
    *xact = new;

    return CORE_OK;
}
/**
 * Delete a transaction
 */
status_t gtp_xact_delete(gtp_xact_ctx_t *xact)
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
gtp_xact_ctx_t *gtp_xact_find(gtp_node_t *gnode, pkbuf_t *pkbuf)
{
    gtpv2c_header_t *h = NULL;
    c_uint32_t xid;
    gtp_xact_ctx_t *xact = NULL;

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
