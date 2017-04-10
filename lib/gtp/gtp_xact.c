#define TRACE_MODULE _gtp_xact
#include "core_debug.h"
#include "core_pool.h"
#include "core_event.h"

#include "3gpp_common.h"
#include "gtp_xact.h"

#define SIZE_OF_GTP_XACT_POOL       32
#define GTP_MAX_XACT_ID             0x800000

#define GTP_XACT_LOCAL_DURATION     3000 /* 3 seconds */
#define GTP_XACT_LOCAL_RETRY_COUNT  3 
#define GTP_XACT_REMOTE_DURATION \
    (GTP_XACT_LOCAL_DURATION * GTP_XACT_LOCAL_RETRY_COUNT) /* 9 seconds */
#define GTP_XACT_REMOTE_RETRY_COUNT 1 

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
#define GTP_XID_TO_SQN(__xid) htonl(((__xid) << 8))
#define GTP_SQN_TO_XID(__sqn) (ntohl(__sqn) >> 8)
            c_uint32_t sqn; 
        };
        /* sqn : 31bit ~ 8bit, spare : 7bit ~ 0bit */
        c_uint32_t sqn_only;
    };
} __attribute__ ((packed)) gtpv2c_header_t;

static int gtp_xact_pool_initialized = 0;
index_declare(gtp_xact_pool, gtp_xact_t, SIZE_OF_GTP_XACT_POOL);

status_t gtp_xact_init(gtp_xact_ctx_t *context, 
        tm_service_t *tm_service, c_uintptr_t event)
{
    if (gtp_xact_pool_initialized == 0)
    {
        index_init(&gtp_xact_pool, SIZE_OF_GTP_XACT_POOL);
    }
    gtp_xact_pool_initialized = 1;

    memset(context, 0, sizeof(gtp_xact_ctx_t));

    context->g_xact_id = 0;

    context->tm_service = tm_service;
    context->event = event;;

    return CORE_OK;
}

status_t gtp_xact_final(void)
{
    if (gtp_xact_pool_initialized == 1)
    {
        d_print("%d not freed in gtp_xact_pool[%d] of S11/S5-SM\n",
                pool_size(&gtp_xact_pool) - pool_avail(&gtp_xact_pool),
                pool_size(&gtp_xact_pool));
        index_final(&gtp_xact_pool);
    }
    gtp_xact_pool_initialized = 0;

    return CORE_OK;
}

static gtp_xact_t *gtp_xact_create(gtp_xact_ctx_t *context, 
    net_sock_t *sock, gtp_node_t *gnode, c_uint8_t org, c_uint32_t xid, 
    c_uint32_t duration, c_uint8_t retry_count)
{
    gtp_xact_t *xact = NULL;

    d_assert(context, return NULL, "Null param");
    d_assert(sock, return NULL, "Null param");
    d_assert(gnode, return NULL, "Null param");

    index_alloc(&gtp_xact_pool, &xact);
    d_assert(xact, return NULL, "Transaction allocation failed");

    xact->org = org;
    xact->xid = xid;
    xact->sock = sock;
    xact->gnode = gnode;

    xact->tm_wait = event_timer(context->tm_service, context->event, 
            duration, xact->index);
    d_assert(xact->tm_wait, 
            index_free(&gtp_xact_pool, xact); return NULL,
            "Timer allocation failed");
    xact->retry_count = retry_count;
    tm_start(xact->tm_wait);

    list_append(xact->org == GTP_LOCAL_ORIGINATOR ?  
            &xact->gnode->local_list : &xact->gnode->remote_list, xact);

    d_trace(1, "[%d]%s Create  : xid = 0x%x\n",
            gnode->port,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            xact->xid);

    return xact;
}

static status_t gtp_xact_delete(gtp_xact_t *xact)
{
    d_assert(xact, , "Null param");
    d_assert(xact->gnode, , "Null param");
    d_assert(xact->tm_wait, , "Null param");

    d_trace(1, "[%d]%s Delete  : xid = 0x%x\n",
            xact->gnode->port,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            xact->xid);

    if (xact->pkbuf)
        pkbuf_free(xact->pkbuf);

    tm_delete(xact->tm_wait);

    list_remove(xact->org == GTP_LOCAL_ORIGINATOR ? &xact->gnode->local_list :
            &xact->gnode->remote_list, xact);
    index_free(&gtp_xact_pool, xact);

    return CORE_OK;
}

static void gtp_xact_associate(gtp_xact_t *xact1, gtp_xact_t *xact2)
{
    d_assert(xact1, return, "Null param");
    d_assert(xact2, return, "Null param");

    d_assert(xact1->assoc_xact == NULL, return, "Already assocaited");
    d_assert(xact2->assoc_xact == NULL, return, "Already assocaited");

    xact1->assoc_xact = xact2;
    xact2->assoc_xact = xact1;
}

static void gtp_xact_deassociate(gtp_xact_t *xact1, gtp_xact_t *xact2)
{
    d_assert(xact1, return, "Null param");
    d_assert(xact2, return, "Null param");

    d_assert(xact1->assoc_xact != NULL, return, "Already deassocaited");
    d_assert(xact2->assoc_xact != NULL, return, "Already deassocaited");

    xact1->assoc_xact = NULL;
    xact2->assoc_xact = NULL;
}

gtp_xact_t *gtp_xact_local_create(
        gtp_xact_ctx_t *context, net_sock_t *sock, gtp_node_t *gnode)
{
    return gtp_xact_create(context, sock, gnode, GTP_LOCAL_ORIGINATOR, 
            NEXT_ID(context->g_xact_id, GTP_MAX_XACT_ID),
            GTP_XACT_LOCAL_DURATION, GTP_XACT_LOCAL_RETRY_COUNT);
}

gtp_xact_t *gtp_xact_remote_create(gtp_xact_ctx_t *context, 
    net_sock_t *sock, gtp_node_t *gnode, c_uint32_t sqn)
{
    return gtp_xact_create(context, sock, gnode, 
            GTP_REMOTE_ORIGINATOR, GTP_SQN_TO_XID(sqn),
            GTP_XACT_REMOTE_DURATION, GTP_XACT_REMOTE_RETRY_COUNT);
}

status_t gtp_xact_commit(gtp_xact_t *xact, 
    c_uint8_t type, c_uint32_t teid, pkbuf_t *pkbuf)
{
    return gtp_xact_associated_commit(xact, NULL, type, teid, pkbuf);
}

status_t gtp_xact_associated_commit(gtp_xact_t *xact, 
    gtp_xact_t *assoc_xact, c_uint8_t type, c_uint32_t teid, pkbuf_t *pkbuf)
{
    gtpv2c_header_t *h = NULL;
    
    d_assert(xact, goto out, "Null param");
    d_assert(xact->sock, goto out, "Null param");
    d_assert(xact->gnode, goto out, "Null param");
    d_assert(pkbuf, goto out, "Null param");

    d_trace(1, "[%d]%s Commit  : xid = 0x%x\n",
            xact->gnode->port,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            xact->xid);

    xact->pkbuf = pkbuf;

    pkbuf_header(pkbuf, GTPV2C_HEADER_LEN);
    h = pkbuf->payload;
    d_assert(h, goto out, "Null param");

    h->version = 2;
    h->teid_presence = 1;
    h->type = type;
    h->length = htons(pkbuf->len - 4);
    h->teid = htonl(teid);
    h->sqn = GTP_XID_TO_SQN(xact->xid);

    d_assert(gtp_send(xact->sock, xact->gnode, xact->pkbuf) == CORE_OK,
            goto out, "gtp_send error");

    if (assoc_xact)
        gtp_xact_associate(xact, assoc_xact);

    return CORE_OK;

out:
    gtp_xact_delete(xact);
    return CORE_ERROR;
}

status_t gtp_xact_timeout(index_t index)
{
    gtp_xact_t *xact = NULL;
    
    d_assert(index, goto out, "Invalid Index");
    xact = index_find(&gtp_xact_pool, index);
    d_assert(xact, goto out, "Null param");
    d_assert(xact->sock, goto out, "Null param");
    d_assert(xact->gnode, goto out, "Null param");

    d_trace(1, "[%d]%s Timeout : xid = 0x%x\n",
            xact->gnode->port,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            xact->xid);

    if (xact->org == GTP_LOCAL_ORIGINATOR)
    {
        if (--xact->retry_count > 0)
        {
            tm_start(xact->tm_wait);

            d_assert(xact->pkbuf, goto out, "Null param");
            d_assert(gtp_send(xact->sock, xact->gnode, xact->pkbuf) == CORE_OK,
                    goto out, "gtp_send error");
        }
        else
        {
            d_warn("[%d]%s No Reponse. Give up",
                    xact->gnode->port,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE");
            gtp_xact_delete(xact);
        }
    }
    else
    {
        gtp_xact_delete(xact);
    }

    return CORE_OK;

out:
    gtp_xact_delete(xact);
    return CORE_ERROR;
}

static gtp_xact_t *gtp_xact_find(
        gtp_node_t *gnode, c_uint8_t type, c_uint32_t sqn)
{
    c_uint32_t xid;
    gtp_xact_t *xact = NULL;

    d_assert(gnode, return NULL, "Null param");
    switch(type)
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
            xact = list_first(&gnode->remote_list);
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
            xact = list_first(&gnode->local_list);
            break;

        default:
            d_error("Not implemented GTPv2 Message Type(%d)", type);
            return NULL;
    }

    xid = GTP_SQN_TO_XID(sqn);
    while(xact)
    {
        if (xact->xid == xid)
            break;
        xact = list_next(xact);
    }

    if (xact)
    {
        d_trace(1, "[%d]%s Find    : xid = 0x%x\n",
                gnode->port,
                xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                xact->xid);
    }

    return xact;
}

status_t gtp_xact_receive(
        gtp_xact_ctx_t *context, net_sock_t *sock, gtp_node_t *gnode, 
        gtp_xact_t **xact, 
        c_uint8_t *type, c_uint32_t *teid, gtp_message_t *gtp_message, 
        pkbuf_t *pkbuf)
{
    status_t rv;
    gtpv2c_header_t *h = NULL;
    gtp_xact_t *new = NULL;

    d_assert(pkbuf, return CORE_ERROR, "Null param");

    d_assert(context, goto out1, "Null param");
    d_assert(sock, goto out1, "Null param");
    d_assert(gnode, goto out1, "Null param");
    d_assert(type, goto out1, "Null param");

    h = pkbuf->payload;
    d_assert(h, goto out1, "Null param");

    new = gtp_xact_find(gnode, h->type, h->sqn);
    if (!new)
    {
        new = gtp_xact_remote_create(context, sock, gnode, h->sqn);
    }
    d_assert(new, goto out1, "Null param");

    d_trace(1, "[%d]%s Receive : xid = 0x%x\n",
            gnode->port,
            new->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            new->xid);

    if (new->org == GTP_LOCAL_ORIGINATOR)
    {
        gtp_xact_t *assoc_xact = new->assoc_xact;

        if (assoc_xact)
            gtp_xact_deassociate(new, assoc_xact);

        gtp_xact_delete(new);
        new = assoc_xact;
    }
    else
    {
        if (new->pkbuf)
        {
            d_warn("[%d]%s Request Duplicated. Retransmit!",
                    new->gnode->port,
                    new->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE");
            rv = gtp_send(new->sock, new->gnode, new->pkbuf);
            d_assert(rv == CORE_OK, goto out2, "gtp_send error");

            pkbuf_free(pkbuf);
            return CORE_EAGAIN;
        }
        else if (new->assoc_xact)
        {
            d_warn("[%d]%s Request Duplicated. Discard Associated transaction!",
                    new->gnode->port,
                    new->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE");

            pkbuf_free(pkbuf);
            return CORE_EAGAIN;
        }
    }

    if (h->teid_presence)
    {
        pkbuf_header(pkbuf, -GTPV2C_HEADER_LEN);
        *teid = ntohl(h->teid);
    }
    else
    {
        pkbuf_header(pkbuf, -(GTPV2C_HEADER_LEN-GTPV2C_TEID_LEN));
        *teid = 0;
    }

    *type = h->type;
    
    rv = gtp_parse_msg(gtp_message, *type, pkbuf);
    d_assert(rv == CORE_OK, 
            pkbuf_free(pkbuf); return CORE_ERROR, "parse error");

    *xact = new;
    return CORE_OK;

out2:
    gtp_xact_delete(new);

out1:
    pkbuf_free(pkbuf);
    return CORE_ERROR;
}
