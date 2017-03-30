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


static int gtp_xact_pool_initialized = 0;
pool_declare(gtp_xact_pool, gtp_xact_t, SIZE_OF_GTP_XACT_POOL);

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

status_t gtp_xact_final(void)
{
    if (gtp_xact_pool_initialized == 1)
    {
        pool_final(&gtp_xact_pool);
    }
    gtp_xact_pool_initialized = 0;

    return CORE_OK;
}

gtp_xact_t *gtp_xact_create(gtp_xact_ctx_t *context, 
    net_sock_t *sock, gtp_node_t *gnode, c_uint8_t org, c_uint32_t xid, 
    c_uint8_t type, pkbuf_t *pkbuf)
{
    gtp_xact_t *xact = NULL;

    d_assert(context, goto out1, "Null param");
    d_assert(sock, goto out1, "Null param");
    d_assert(gnode, goto out1, "Null param");
    d_assert(pkbuf, goto out1, "Null param");

    pool_alloc_node(&gtp_xact_pool, &xact);
    d_assert(xact, goto out1, "Transaction allocation failed");

    memset(xact, 0, sizeof(gtp_xact_t));

    xact->org = org;
    xact->xid = xid;
    xact->sock = sock;
    xact->gnode = gnode;
    xact->type = type;
    xact->pkbuf = pkbuf;

    xact->tm_wait = event_timer(context->tm_service, context->event, 
            context->duration, (c_uintptr_t)xact);
    d_assert(xact->tm_wait, goto out2, "Timer allocation failed");
    xact->retry_count = context->retry_count;

    list_append(xact->org == GTP_LOCAL_ORIGINATOR ?  
            xact->gnode->local_list : xact->gnode->remote_list, xact);

    return xact;

out2:
    pool_free_node(&gtp_xact_pool, xact);

out1:
    pkbuf_free(pkbuf);
    return NULL;
}

status_t gtp_xact_delete(gtp_xact_t *xact)
{
    d_assert(xact, , "Null param");
    d_assert(xact->gnode, , "Null param");

    d_assert(xact->pkbuf, , "Null param");
    pkbuf_free(xact->pkbuf);

    d_assert(xact->tm_wait, , "Null param");
    tm_delete(xact->tm_wait);

    list_remove(xact->org == GTP_LOCAL_ORIGINATOR ?  xact->gnode->local_list :
            xact->gnode->remote_list, xact);
    pool_free_node(&gtp_xact_pool, xact);

    return CORE_OK;
}

status_t gtp_xact_associate(gtp_xact_t *xact1, gtp_xact_t *xact2)
{
    d_assert(xact1, return CORE_ERROR, "Null param");
    d_assert(xact2, return CORE_ERROR, "Null param");

    d_assert(xact1->assoc_xact == NULL, 
            return CORE_ERROR, "Already assocaited");
    d_assert(xact2->assoc_xact == NULL, 
            return CORE_ERROR, "Already assocaited");

    xact1->assoc_xact = xact2;
    xact2->assoc_xact = xact1;

    return CORE_OK;
}

status_t gtp_xact_deassociate(gtp_xact_t *xact1, gtp_xact_t *xact2)
{
    d_assert(xact1, return CORE_ERROR, "Null param");
    d_assert(xact2, return CORE_ERROR, "Null param");

    d_assert(xact1->assoc_xact != NULL, 
            return CORE_ERROR, "Already deassocaited");
    d_assert(xact2->assoc_xact != NULL, 
            return CORE_ERROR, "Already deassocaited");

    xact1->assoc_xact = NULL;
    xact2->assoc_xact = NULL;

    return CORE_OK;
}

status_t gtp_xact_commit(gtp_xact_t *xact)
{
    status_t rv;
    net_sock_t *sock = NULL;
    gtp_node_t *gnode = NULL;
    pkbuf_t *pkbuf = NULL;
    gtpv2c_header_t *h = NULL;

    d_assert(xact, goto out, "Null param");

    if (xact->retry_count > 0)
    {
        sock = xact->sock;
        d_assert(sock, goto out, "Null param");
        gnode = xact->gnode;
        d_assert(gnode, goto out, "Null param");
        pkbuf = xact->pkbuf;
        d_assert(pkbuf, goto out, "Null param");

        pkbuf_header(pkbuf, GTPV2C_HEADER_LEN);
        h = pkbuf->payload;
        d_assert(h, goto out, "Null param");

        h->version = 2;
        h->teid_presence = 1;
        h->type = xact->type;
        h->length = htons(pkbuf->len - 4);
        h->sqn = GTP_XID_TO_SQN(xact->xid);

        rv = gtp_send(sock, gnode, pkbuf);
        if (rv != CORE_OK)
        {
            d_error("failed to send GTP message");
            goto out;
        }

        xact->retry_count--;

        d_assert(xact->tm_wait, goto out, "Null param");
        tm_start(xact->tm_wait);
    }
    else
    {
        d_warn("No Response. Give up");
        gtp_xact_delete(xact);
    }

    return CORE_OK;

out:
    pkbuf_free(pkbuf);
    return CORE_ERROR;
}

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
            xact = list_first(&gnode->initial_list);
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
            xact = list_first(&gnode->triggered_list);
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

gtp_xact_t *gtp_xact_recv(gtp_xact_ctx_t *context,
        net_sock_t *sock, gtp_node_t *gnode, pkbuf_t *pkbuf)
{
    gtp_xact_t *xact = NULL;
    gtpv2c_header_t *h = NULL;

    d_assert(context, goto out, "Null param");
    d_assert(sock, goto out, "Null param");
    d_assert(gnode, goto out, "Null param");
    d_assert(pkbuf, goto out, "Null param");

    h = pkbuf->payload;
    d_assert(h, goto out, "Null param");
    
    xact = gtp_xact_find(gnode, pkbuf);
    if (!xact)
    {
        xact = gtp_xact_create(context, sock, gnode, GTP_REMOTE_ORIGINATOR, 
                GTP_SQN_TO_XID(h->sqn), h->type, pkbuf);
    }

    if (h->teid_presence)
        pkbuf_header(pkbuf, -GTPV2C_HEADER_LEN);
    else
        pkbuf_header(pkbuf, -(GTPV2C_HEADER_LEN-GTPV2C_TEID_LEN));

    return xact;

out:
    pkbuf_free(pkbuf);
    return NULL;
}

gtp_xact_t *gtp_xact_send(gtp_xact_ctx_t *context,
        net_sock_t *sock, gtp_node_t *gnode, c_uint8_t type, pkbuf_t *pkbuf)
{
    status_t rv;
    gtp_xact_t *xact = NULL;

    d_assert(context, goto out, "Null param");
    d_assert(sock, goto out, "Null param");
    d_assert(gnode, goto out, "Null param");
    d_assert(pkbuf, goto out, "Null param");

    xact = gtp_xact_create(context, sock, gnode, GTP_LOCAL_ORIGINATOR, 
            GTP_XACT_NEXT_ID(context->g_xact_id), type, pkbuf); 
    d_assert(xact, return NULL, "Null param");

    rv = gtp_xact_commit(xact);
    d_assert(rv == CORE_OK, return NULL, "Null param");

    return xact;

out:
    pkbuf_free(pkbuf);
    return NULL;
}
