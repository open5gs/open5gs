#define TRACE_MODULE _gtp_xact
#include "core_debug.h"
#include "core_pool.h"
#include "core_event.h"

#include "types.h"
#include "gtp_xact.h"
#include "gtp_message.h"

#define SIZE_OF_GTP_XACT_POOL       32
#define GTP_MIN_XACT_ID             1
#define GTP_MAX_XACT_ID             0x800000

#define GTP_T3_RESPONSE_DURATION     3000 /* 3 seconds */
#define GTP_T3_RESPONSE_RETRY_COUNT  3 
#define GTP_T3_DUPLICATED_DURATION \
    (GTP_T3_RESPONSE_DURATION * GTP_T3_RESPONSE_RETRY_COUNT) /* 9 seconds */
#define GTP_T3_DUPLICATED_RETRY_COUNT 1 

typedef enum {
    GTP_XACT_UNKNOWN_STAGE,
    GTP_XACT_INITIAL_STAGE,
    GTP_XACT_INTERMEDIATE_STAGE,
    GTP_XACT_FINAL_STAGE,
} gtp_xact_stage_t;

static int gtp_xact_initialized = 0;
static tm_service_t *g_tm_service = NULL;
static c_uintptr_t g_response_event = 0;
static c_uintptr_t g_duplicated_event = 0;
static c_uint32_t g_xact_id = 0;

index_declare(gtp_xact_pool, gtp_xact_t, SIZE_OF_GTP_XACT_POOL);

static gtp_xact_stage_t gtp_xact_get_stage(c_uint8_t type, c_uint32_t sqn);
static status_t gtp_xact_delete(gtp_xact_t *xact);

status_t gtp_xact_init(tm_service_t *tm_service,
        c_uintptr_t response_event, c_uintptr_t duplicated_event)
{
    d_assert(gtp_xact_initialized == 0, return CORE_ERROR,
        "GTP Transaction already has been initialized");

    index_init(&gtp_xact_pool, SIZE_OF_GTP_XACT_POOL);

    g_xact_id = 0;
    g_tm_service = tm_service;
    g_response_event = response_event;
    g_duplicated_event = duplicated_event;

    gtp_xact_initialized = 1;

    return CORE_OK;
}

status_t gtp_xact_final(void)
{
    d_assert(gtp_xact_initialized == 1, return CORE_ERROR,
        "GTP Transaction context already has been finalized");

    if (pool_size(&gtp_xact_pool) != pool_avail(&gtp_xact_pool))
        d_error("%d not freed in gtp_xact_pool[%d] of GTP Transaction",
                pool_size(&gtp_xact_pool) - pool_avail(&gtp_xact_pool),
                pool_size(&gtp_xact_pool));

    d_trace(3, "%d not freed in gtp_xact_pool[%d] of GTP Transaction\n",
            pool_size(&gtp_xact_pool) - pool_avail(&gtp_xact_pool),
            pool_size(&gtp_xact_pool));
    index_final(&gtp_xact_pool);

    gtp_xact_initialized = 0;

    return CORE_OK;
}

gtp_xact_t *gtp_xact_local_create(net_sock_t *sock, gtp_node_t *gnode)
{
    char buf[INET_ADDRSTRLEN];
    gtp_xact_t *xact = NULL;

    d_assert(sock, return NULL, "Null param");
    d_assert(gnode, return NULL, "Null param");

    index_alloc(&gtp_xact_pool, &xact);
    d_assert(xact, return NULL, "Transaction allocation failed");

    xact->org = GTP_LOCAL_ORIGINATOR;
    xact->xid = NEXT_ID(g_xact_id, GTP_MIN_XACT_ID, GTP_MAX_XACT_ID);
    xact->sock = sock;
    xact->gnode = gnode;

    if (g_response_event)
    {
        xact->tm_response = event_timer(g_tm_service,
                g_response_event, GTP_T3_RESPONSE_DURATION, xact->index);
        d_assert(xact->tm_response, return NULL, "Timer allocation failed");
        xact->response_rcount = GTP_T3_RESPONSE_RETRY_COUNT;
    }

    if (g_duplicated_event)
    {
        xact->tm_duplicated = event_timer(g_tm_service,
                g_duplicated_event, GTP_T3_DUPLICATED_DURATION, xact->index);
        d_assert(xact->tm_duplicated, return NULL, "Timer allocation failed");
        xact->duplicated_rcount = GTP_T3_DUPLICATED_RETRY_COUNT;
    }

    list_append(xact->org == GTP_LOCAL_ORIGINATOR ?  
            &xact->gnode->local_list : &xact->gnode->remote_list, xact);

    d_trace(3, "[%d] %s Create  peer %s:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            INET_NTOP(&gnode->addr, buf), gnode->port);

    return xact;
}

gtp_xact_t *gtp_xact_remote_create(
        net_sock_t *sock, gtp_node_t *gnode, c_uint32_t sqn)
{
    char buf[INET_ADDRSTRLEN];
    gtp_xact_t *xact = NULL;

    d_assert(sock, return NULL, "Null param");
    d_assert(gnode, return NULL, "Null param");

    index_alloc(&gtp_xact_pool, &xact);
    d_assert(xact, return NULL, "Transaction allocation failed");

    xact->org = GTP_REMOTE_ORIGINATOR;
    xact->xid = GTP_SQN_TO_XID(sqn);
    xact->sock = sock;
    xact->gnode = gnode;

    if (g_response_event)
    {
        xact->tm_response = event_timer(g_tm_service,
                g_response_event, GTP_T3_RESPONSE_DURATION, xact->index);
        d_assert(xact->tm_response, return NULL, "Timer allocation failed");
        xact->response_rcount = GTP_T3_RESPONSE_RETRY_COUNT;
    }

    if (g_duplicated_event)
    {
        xact->tm_duplicated = event_timer(g_tm_service,
                g_duplicated_event, GTP_T3_DUPLICATED_DURATION, xact->index);
        d_assert(xact->tm_duplicated, return NULL, "Timer allocation failed");
        xact->duplicated_rcount = GTP_T3_DUPLICATED_RETRY_COUNT;
    }

    list_append(xact->org == GTP_LOCAL_ORIGINATOR ?  
            &xact->gnode->local_list : &xact->gnode->remote_list, xact);

    d_trace(3, "[%d] %s Create  peer %s:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            INET_NTOP(&gnode->addr, buf), gnode->port);

    return xact;
}

void gtp_xact_delete_all(gtp_node_t *gnode)
{
    gtp_xact_t *xact = NULL;
    
    xact = list_first(&gnode->local_list);
    while(xact)
    {
        gtp_xact_delete(xact);
        xact = list_next(xact);
    }
    xact = list_first(&gnode->remote_list);
    while(xact)
    {
        gtp_xact_delete(xact);
        xact = list_next(xact);
    }
}

status_t gtp_xact_update_tx(gtp_xact_t *xact,
    c_uint8_t type, c_uint32_t teid, pkbuf_t *pkbuf)
{
    char buf[INET_ADDRSTRLEN];
    gtp_xact_stage_t stage;
    gtp_header_t *h = NULL;
    
    d_assert(xact, return CORE_ERROR, "Null param");
    d_assert(xact->sock, return CORE_ERROR, "Null param");
    d_assert(xact->gnode, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    d_trace(3, "[%d] %s UPD-TX  peer %s:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

    stage = gtp_xact_get_stage(type, xact->xid);
    if (xact->org == GTP_LOCAL_ORIGINATOR)
    {
        switch(stage)
        {
            case GTP_XACT_INITIAL_STAGE:
                d_assert(xact->step == 0, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n", xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);
                break;
            case GTP_XACT_INTERMEDIATE_STAGE:
                d_assert(0, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

            case GTP_XACT_FINAL_STAGE:
                d_assert(xact->step == 2, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n", xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);
                break;
            default:
                d_assert(0, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);
        }
    }
    else if (xact->org == GTP_REMOTE_ORIGINATOR)
    {
        switch(stage)
        {
            case GTP_XACT_INITIAL_STAGE:
                d_assert(0, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

            case GTP_XACT_INTERMEDIATE_STAGE:
            case GTP_XACT_FINAL_STAGE:
                d_assert(xact->step == 1, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n", xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);
                break;
            default:
                d_assert(0, return CORE_ERROR,
                    "[%d] %s invalid stage  peer %s:%d\n",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);
        }
    }
    else
        d_assert(0, return CORE_ERROR, "invalid orginator(%d)", xact->org);


    pkbuf_header(pkbuf, GTPV2C_HEADER_LEN);
    h = pkbuf->payload;
    d_assert(h, return CORE_ERROR, "Null param");

    memset(h, 0, sizeof(gtp_header_t));
    h->version = 2;
    h->teid_presence = 1;
    h->type = type;
    h->length = htons(pkbuf->len - 4);
    h->teid = htonl(teid);
    h->sqn = GTP_XID_TO_SQN(xact->xid);

    /* Save Message type and packet of this step */
    xact->seq[xact->step].type = h->type;
    xact->seq[xact->step].pkbuf = pkbuf;

    /* Step */
    xact->step++;

    return CORE_OK;
}

status_t gtp_xact_update_rx(gtp_xact_t *xact, c_uint8_t type)
{
    status_t rv = CORE_OK;
    char buf[INET_ADDRSTRLEN];
    gtp_xact_stage_t stage;

    d_trace(3, "[%d] %s UPD-RX  peer %s:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

    stage = gtp_xact_get_stage(type, xact->xid);
    if (xact->org == GTP_LOCAL_ORIGINATOR)
    {
        switch(stage)
        {
            case GTP_XACT_INITIAL_STAGE:
                d_assert(0, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

            case GTP_XACT_INTERMEDIATE_STAGE:
                if (xact->seq[1].type == type)
                {
                    pkbuf_t *pkbuf = NULL;

                    d_assert(xact->step == 3, return CORE_ERROR,
                        "[%d] %s invalid stage peer %s:%d\n", xact->xid,
                        xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                        INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

                    pkbuf = xact->seq[2].pkbuf;
                    if (pkbuf)
                    {
                        if (xact->tm_duplicated)
                            tm_start(xact->tm_duplicated);

                        d_warn("[%d]%s Request Duplicated. Retransmit!",
                                xact->gnode->port,
                                xact->org == GTP_LOCAL_ORIGINATOR ?
                                    "LOCAL " : "REMOTE");
                        rv = gtp_send(xact->sock, xact->gnode, pkbuf);
                        d_assert(rv == CORE_OK, return CORE_ERROR,
                                "gtp_send error");
                    }
                    else
                    {
                        d_warn("[%d]%s Request Duplicated. Discard!",
                                xact->gnode->port,
                                xact->org == GTP_LOCAL_ORIGINATOR ?
                                    "LOCAL " : "REMOTE");
                    }

                    return CORE_EAGAIN;
                }

                d_assert(xact->step == 1, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n", xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

                if (xact->tm_duplicated)
                    tm_start(xact->tm_duplicated);

                break;

            case GTP_XACT_FINAL_STAGE:
                d_assert(xact->step == 1, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

                /* continue */
                break;

            default:
                d_assert(0, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);
        }
    }
    else if (xact->org == GTP_REMOTE_ORIGINATOR)
    {
        switch(stage)
        {
            case GTP_XACT_INITIAL_STAGE:
                if (xact->seq[0].type == type)
                {
                    pkbuf_t *pkbuf = NULL;

                    d_assert(xact->step == 2, return CORE_ERROR,
                        "[%d] %s invalid stage peer %s:%d\n", xact->xid,
                        xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                        INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

                    pkbuf = xact->seq[1].pkbuf;
                    if (pkbuf)
                    {
                        if (xact->tm_duplicated)
                            tm_start(xact->tm_duplicated);

                        d_warn("[%d]%s Request Duplicated. Retransmit!",
                                xact->gnode->port,
                                xact->org == GTP_LOCAL_ORIGINATOR ?
                                    "LOCAL " : "REMOTE");
                        rv = gtp_send(xact->sock, xact->gnode, pkbuf);
                        d_assert(rv == CORE_OK, return CORE_ERROR,
                                "gtp_send error");
                    }
                    else
                    {
                        d_warn("[%d]%s Request Duplicated. Discard!",
                                xact->gnode->port,
                                xact->org == GTP_LOCAL_ORIGINATOR ?
                                    "LOCAL " : "REMOTE");
                    }

                    return CORE_EAGAIN;
                }

                d_assert(xact->step == 0, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n", xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

                if (xact->tm_duplicated)
                    tm_start(xact->tm_duplicated);

                break;

            case GTP_XACT_INTERMEDIATE_STAGE:
                d_assert(0, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

            case GTP_XACT_FINAL_STAGE:
                d_assert(xact->step == 2, return CORE_ERROR,
                    "[%d] %s invalid stage peer %s:%d\n", xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

                /* continue */
                break;

            default:
                d_assert(0, return CORE_ERROR,
                    "[%d] %s invalid stage  peer %s:%d\n",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);
        }
    }
    else
        d_assert(0, return CORE_ERROR, "invalid orginator(%d)", xact->org);

    /* Save Message type of this step */
    xact->seq[xact->step].type = type;

    /* Step */
    xact->step++;

    return CORE_OK;
}


status_t gtp_xact_commit(gtp_xact_t *xact)
{
    char buf[INET_ADDRSTRLEN];
    pkbuf_t *pkbuf = NULL;
    c_uint8_t type;
    
    d_assert(xact, return CORE_ERROR, "Null param");
    d_assert(xact->sock, return CORE_ERROR, "Null param");
    d_assert(xact->gnode, return CORE_ERROR, "Null param");

    d_trace(3, "[%d] %s Commit  peer %s:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

    if (xact->org == GTP_LOCAL_ORIGINATOR)
    {
        type = xact->seq[xact->step-1].type;
        gtp_xact_stage_t stage = gtp_xact_get_stage(type, xact->xid);
        if (stage == GTP_XACT_FINAL_STAGE)
        {
            gtp_xact_delete(xact);
            return CORE_OK;
        }
    }

    pkbuf = xact->seq[xact->step-1].pkbuf;
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    d_assert(gtp_send(xact->sock, xact->gnode, pkbuf) == CORE_OK,
            return CORE_ERROR, "gtp_send error");

    return CORE_OK;
}

status_t gtp_xact_timeout(index_t index, c_uintptr_t event)
{
    char buf[INET_ADDRSTRLEN];
    gtp_xact_t *xact = NULL;
    
    d_assert(index, goto out, "Invalid Index");
    xact = index_find(&gtp_xact_pool, index);
    d_assert(xact, goto out, "Null param");
    d_assert(xact->sock, goto out, "Null param");
    d_assert(xact->gnode, goto out, "Null param");

    d_trace(3, "[%d] %s Timeout  peer %s:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

    if (event == g_response_event)
    {
        if (--xact->response_rcount > 0)
        {
            pkbuf_t *pkbuf = NULL;

            if (xact->tm_response)
                tm_start(xact->tm_response);

            pkbuf = xact->seq[xact->step-1].pkbuf;
            d_assert(pkbuf, return CORE_ERROR, "Null param");

            d_assert(gtp_send(xact->sock, xact->gnode, pkbuf) == CORE_OK,
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
    else if (event == g_duplicated_event)
    {
        if (--xact->duplicated_rcount > 0)
        {
            if (xact->tm_duplicated)
                tm_start(xact->tm_duplicated);
        }
        else
        {
            gtp_xact_delete(xact);
        }
    }

    return CORE_OK;

out:
    gtp_xact_delete(xact);
    return CORE_ERROR;
}

status_t gtp_xact_receive(
        net_sock_t *sock, gtp_node_t *gnode, pkbuf_t *pkbuf,
        gtp_xact_t **xact, gtp_message_t *message)
{
    char buf[INET_ADDRSTRLEN];
    status_t rv;
    gtp_xact_t *new = NULL;
    gtp_header_t *h = NULL;

    d_assert(sock, return CORE_ERROR, "Null param");
    d_assert(gnode, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");
    d_assert(pkbuf->payload, return CORE_ERROR, "Null param");
    h = pkbuf->payload;
    d_assert(h, return CORE_ERROR, "Null param");

    new = gtp_xact_find_by_xid(gnode, h->type, GTP_SQN_TO_XID(h->sqn));
    if (!new)
    {
        new = gtp_xact_remote_create(sock, gnode, h->sqn);
    }
    d_assert(new, return CORE_ERROR, "Null param");

    d_trace(3, "[%d] %s Receive peer %s:%d\n",
            new->xid,
            new->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            INET_NTOP(&gnode->addr, buf), gnode->port);

    rv = gtp_xact_update_rx(new, h->type);
    if (rv != CORE_OK)
    {
        pkbuf_free(pkbuf);
        return rv;
    }

    rv = gtp_parse_msg(message, pkbuf);
    d_assert(rv == CORE_OK, 
            pkbuf_free(pkbuf); return CORE_ERROR, "parse error");

    *xact = new;
    return CORE_OK;
}

gtp_xact_t *gtp_xact_find(index_t index)
{
    d_assert(index, return NULL, "Invalid Index");
    return index_find(&gtp_xact_pool, index);
}

static gtp_xact_stage_t gtp_xact_get_stage(c_uint8_t type, c_uint32_t xid)
{
    gtp_xact_stage_t stage = GTP_XACT_UNKNOWN_STAGE;

    switch(type)
    {
        case GTP_CREATE_SESSION_REQUEST_TYPE:
        case GTP_MODIFY_BEARER_REQUEST_TYPE:
        case GTP_DELETE_SESSION_REQUEST_TYPE:
        case GTP_MODIFY_BEARER_COMMAND_TYPE:
        case GTP_DELETE_BEARER_COMMAND_TYPE:
        case GTP_BEARER_RESOURCE_COMMAND_TYPE:
        case GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE:
        case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
        case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE:
        case GTP_DOWNLINK_DATA_NOTIFICATION_TYPE:
            stage = GTP_XACT_INITIAL_STAGE;
            break;
        case GTP_CREATE_BEARER_REQUEST_TYPE:
        case GTP_UPDATE_BEARER_REQUEST_TYPE:
        case GTP_DELETE_BEARER_REQUEST_TYPE:
            if (xid & GTP_MAX_XACT_ID)
                stage = GTP_XACT_INTERMEDIATE_STAGE;
            else
                stage = GTP_XACT_INITIAL_STAGE;
            break;
        case GTP_CREATE_SESSION_RESPONSE_TYPE:
        case GTP_MODIFY_BEARER_RESPONSE_TYPE:
        case GTP_DELETE_SESSION_RESPONSE_TYPE:
        case GTP_MODIFY_BEARER_FAILURE_INDICATION_TYPE:
        case GTP_DELETE_BEARER_FAILURE_INDICATION_TYPE:
        case GTP_BEARER_RESOURCE_FAILURE_INDICATION_TYPE:
        case GTP_CREATE_BEARER_RESPONSE_TYPE:
        case GTP_UPDATE_BEARER_RESPONSE_TYPE:
        case GTP_DELETE_BEARER_RESPONSE_TYPE:
        case GTP_RELEASE_ACCESS_BEARERS_RESPONSE_TYPE:
        case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
        case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE_TYPE:
        case GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE_TYPE:
            stage = GTP_XACT_FINAL_STAGE;
            break;

        default:
            d_error("Not implemented GTPv2 Message Type(%d)", type);
            break;
    }

    return stage;
}

gtp_xact_t *gtp_xact_find_by_xid(
        gtp_node_t *gnode, c_uint8_t type, c_uint32_t xid)
{
    char buf[INET_ADDRSTRLEN];
    gtp_xact_t *xact = NULL;

    d_assert(gnode, return NULL, "Null param");

    switch(gtp_xact_get_stage(type, xid))
    {
        case GTP_XACT_INITIAL_STAGE:
            xact = list_first(&gnode->remote_list);
            break;
        case GTP_XACT_INTERMEDIATE_STAGE:
            xact = list_first(&gnode->local_list);
            break;
        case GTP_XACT_FINAL_STAGE:
            if (xid & GTP_MAX_XACT_ID)
                xact = list_first(&gnode->remote_list);
            else
                xact = list_first(&gnode->local_list);
            break;

        default:
            d_assert(0, return NULL, "Unknown stage");
    }

    while(xact)
    {
        if (xact->xid == xid)
            break;
        xact = list_next(xact);
    }

    if (xact)
    {
        d_trace(3, "[%d] %s Find    peer %s:%d\n",
                xact->xid,
                xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                INET_NTOP(&gnode->addr, buf), gnode->port);
    }

    return xact;
}

void gtp_xact_associate(gtp_xact_t *xact1, gtp_xact_t *xact2)
{
    d_assert(xact1, return, "Null param");
    d_assert(xact2, return, "Null param");

    d_assert(xact1->assoc_xact == NULL, return, "Already assocaited");
    d_assert(xact2->assoc_xact == NULL, return, "Already assocaited");

    xact1->assoc_xact = xact2;
    xact2->assoc_xact = xact1;
}

void gtp_xact_deassociate(gtp_xact_t *xact1, gtp_xact_t *xact2)
{
    d_assert(xact1, return, "Null param");
    d_assert(xact2, return, "Null param");

    d_assert(xact1->assoc_xact != NULL, return, "Already deassocaited");
    d_assert(xact2->assoc_xact != NULL, return, "Already deassocaited");

    xact1->assoc_xact = NULL;
    xact2->assoc_xact = NULL;
}

static status_t gtp_xact_delete(gtp_xact_t *xact)
{
    char buf[INET_ADDRSTRLEN];

    d_assert(xact, , "Null param");
    d_assert(xact->gnode, , "Null param");

    d_trace(3, "[%d] %s Delete  peer %s:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            INET_NTOP(&xact->gnode->addr, buf), xact->gnode->port);

    if (xact->seq[0].pkbuf)
        pkbuf_free(xact->seq[0].pkbuf);
    if (xact->seq[1].pkbuf)
        pkbuf_free(xact->seq[1].pkbuf);
    if (xact->seq[2].pkbuf)
        pkbuf_free(xact->seq[2].pkbuf);

    if (xact->tm_response)
        tm_delete(xact->tm_response);
    if (xact->tm_duplicated)
        tm_delete(xact->tm_duplicated);

    if (xact->assoc_xact)
        gtp_xact_deassociate(xact, xact->assoc_xact);

    list_remove(xact->org == GTP_LOCAL_ORIGINATOR ? &xact->gnode->local_list :
            &xact->gnode->remote_list, xact);
    index_free(&gtp_xact_pool, xact);

    return CORE_OK;
}

