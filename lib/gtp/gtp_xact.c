#define TRACE_MODULE _gtp_xact
#include "core_debug.h"
#include "core_pool.h"
#include "core_event.h"

#include "3gpp_types.h"
#include "gtp_message.h"
#include "gtp_node.h"
#include "gtp_path.h"

#include "gtp_xact.h"

#define SIZE_OF_GTP_XACT_POOL       64
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
static c_uintptr_t g_holding_event = 0;
static c_uint32_t g_xact_id = 0;

index_declare(gtp_xact_pool, gtp_xact_t, SIZE_OF_GTP_XACT_POOL);

static gtp_xact_stage_t gtp_xact_get_stage(c_uint8_t type, c_uint32_t sqn);
static status_t gtp_xact_delete(gtp_xact_t *xact);

status_t gtp_xact_init(tm_service_t *tm_service,
        c_uintptr_t response_event, c_uintptr_t holding_event)
{
    d_assert(gtp_xact_initialized == 0, return CORE_ERROR,
        "GTP Transaction already has been initialized");

    index_init(&gtp_xact_pool, SIZE_OF_GTP_XACT_POOL);

    g_xact_id = 0;
    g_tm_service = tm_service;
    g_response_event = response_event;
    g_holding_event = holding_event;

    gtp_xact_initialized = 1;

    return CORE_OK;
}

status_t gtp_xact_final(void)
{
    d_assert(gtp_xact_initialized == 1, return CORE_ERROR,
        "GTP Transaction context already has been finalized");

    if (pool_used(&gtp_xact_pool))
        d_error("%d not freed in gtp_xact_pool[%d] of GTP Transaction",
                pool_used(&gtp_xact_pool), pool_size(&gtp_xact_pool));
    d_trace(15, "%d not freed in gtp_xact_pool[%d] of GTP Transaction\n",
            pool_used(&gtp_xact_pool), pool_size(&gtp_xact_pool));
    index_final(&gtp_xact_pool);

    gtp_xact_initialized = 0;

    return CORE_OK;
}

gtp_xact_t *gtp_xact_local_create(
        gtp_node_t *gnode, gtp_header_t *hdesc, pkbuf_t *pkbuf)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];
    gtp_xact_t *xact = NULL;

    d_assert(gnode, return NULL, "Null param");

    index_alloc(&gtp_xact_pool, &xact);
    d_assert(xact, return NULL, "Transaction allocation failed");

    xact->org = GTP_LOCAL_ORIGINATOR;
    xact->xid = NEXT_ID(g_xact_id, GTP_MIN_XACT_ID, GTP_MAX_XACT_ID);
    xact->gnode = gnode;

    if (g_response_event)
    {
        xact->tm_response = timer_create(g_tm_service,
                g_response_event, GTP_T3_RESPONSE_DURATION);
        d_assert(xact->tm_response, return NULL, "Timer allocation failed");
        timer_set_param1(xact->tm_response, xact->index);
        xact->response_rcount = GTP_T3_RESPONSE_RETRY_COUNT;
    }

    if (g_holding_event)
    {
        xact->tm_holding = timer_create(g_tm_service,
                g_holding_event, GTP_T3_DUPLICATED_DURATION);
        d_assert(xact->tm_holding, return NULL, "Timer allocation failed");
        timer_set_param1(xact->tm_holding, xact->index);
        xact->holding_rcount = GTP_T3_DUPLICATED_RETRY_COUNT;
    }

    list_append(xact->org == GTP_LOCAL_ORIGINATOR ?  
            &xact->gnode->local_list : &xact->gnode->remote_list, xact);

    rv = gtp_xact_update_tx(xact, hdesc, pkbuf);
    d_assert(rv == CORE_OK, return NULL, "Update Tx failed");

    d_trace(15, "[%d] %s Create  peer [%s]:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            CORE_ADDR(sock_remote_addr(gnode->sock), buf),
            CORE_PORT(sock_remote_addr(gnode->sock)));

    return xact;
}

gtp_xact_t *gtp_xact_remote_create(gtp_node_t *gnode, c_uint32_t sqn)
{
    char buf[CORE_ADDRSTRLEN];
    gtp_xact_t *xact = NULL;

    d_assert(gnode, return NULL, "Null param");

    index_alloc(&gtp_xact_pool, &xact);
    d_assert(xact, return NULL, "Transaction allocation failed");

    xact->org = GTP_REMOTE_ORIGINATOR;
    xact->xid = GTP_SQN_TO_XID(sqn);
    xact->gnode = gnode;

    if (g_response_event)
    {
        xact->tm_response = timer_create(g_tm_service,
                g_response_event, GTP_T3_RESPONSE_DURATION);
        d_assert(xact->tm_response, return NULL, "Timer allocation failed");
        timer_set_param1(xact->tm_response, xact->index);
        xact->response_rcount = GTP_T3_RESPONSE_RETRY_COUNT;
    }

    if (g_holding_event)
    {
        xact->tm_holding = timer_create(g_tm_service,
                g_holding_event, GTP_T3_DUPLICATED_DURATION);
        d_assert(xact->tm_holding, return NULL, "Timer allocation failed");
        timer_set_param1(xact->tm_holding, xact->index);
        xact->holding_rcount = GTP_T3_DUPLICATED_RETRY_COUNT;
    }

    list_append(xact->org == GTP_LOCAL_ORIGINATOR ?  
            &xact->gnode->local_list : &xact->gnode->remote_list, xact);

    d_trace(15, "[%d] %s Create  peer [%s]:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            CORE_ADDR(sock_remote_addr(gnode->sock), buf),
            CORE_PORT(sock_remote_addr(gnode->sock)));

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
        gtp_header_t *hdesc, pkbuf_t *pkbuf)
{
    char buf[CORE_ADDRSTRLEN];
    gtp_xact_stage_t stage;
    gtp_header_t *h = NULL;
    
    d_assert(xact, return CORE_ERROR, "Null param");
    d_assert(xact->gnode, return CORE_ERROR, "Null param");
    d_assert(hdesc, return CORE_ERROR, "Null param");
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    d_trace(15, "[%d] %s UPD TX-%d  peer [%s]:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            hdesc->type,
            CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
            CORE_PORT(sock_remote_addr(xact->gnode->sock)));

    stage = gtp_xact_get_stage(hdesc->type, xact->xid);
    if (xact->org == GTP_LOCAL_ORIGINATOR)
    {
        switch(stage)
        {
            case GTP_XACT_INITIAL_STAGE:
                d_assert(xact->step == 0, return CORE_ERROR,
                    "[%d] %s invalid step %d for type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, hdesc->type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));
                break;

            case GTP_XACT_INTERMEDIATE_STAGE:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);

            case GTP_XACT_FINAL_STAGE:
                d_assert(xact->step == 2, return CORE_ERROR,
                    "[%d] %s invalid step %d for type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, hdesc->type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));
                break;

            default:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);
        }
    }
    else if (xact->org == GTP_REMOTE_ORIGINATOR)
    {
        switch(stage)
        {
            case GTP_XACT_INITIAL_STAGE:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);

            case GTP_XACT_INTERMEDIATE_STAGE:
            case GTP_XACT_FINAL_STAGE:
                d_assert(xact->step == 1, return CORE_ERROR,
                    "[%d] %s invalid step %d for type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, hdesc->type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));
                break;

            default:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);
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
    h->type = hdesc->type;
    h->length = htons(pkbuf->len - 4);
    h->teid = htonl(hdesc->teid);
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
    char buf[CORE_ADDRSTRLEN];
    gtp_xact_stage_t stage;

    d_trace(15, "[%d] %s UPD RX-%d  peer [%s]:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            type,
            CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
            CORE_PORT(sock_remote_addr(xact->gnode->sock)));

    stage = gtp_xact_get_stage(type, xact->xid);
    if (xact->org == GTP_LOCAL_ORIGINATOR)
    {
        switch(stage)
        {
            case GTP_XACT_INITIAL_STAGE:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);

            case GTP_XACT_INTERMEDIATE_STAGE:
                if (xact->seq[1].type == type)
                {
                    pkbuf_t *pkbuf = NULL;

                    d_assert(xact->step == 2 || xact->step == 3,
                        return CORE_ERROR,
                        "[%d] %s invalid step %d for type %d peer [%s]:%d",
                        xact->xid,
                        xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                        xact->step, type,
                        CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                        CORE_PORT(sock_remote_addr(xact->gnode->sock)));

                    pkbuf = xact->seq[2].pkbuf;
                    if (pkbuf)
                    {
                        if (xact->tm_holding)
                            tm_start(xact->tm_holding);

                        d_warn("[%d] %s Request Duplicated. Retransmit!"
                                " for type %d peer [%s]:%d",
                                xact->xid,
                                xact->org == GTP_LOCAL_ORIGINATOR ?
                                    "LOCAL " : "REMOTE",
                                xact->step, type,
                                CORE_ADDR(sock_remote_addr(xact->gnode->sock),
                                    buf),
                                CORE_PORT(sock_remote_addr(xact->gnode->sock)));
                        rv = gtp_send(xact->gnode, pkbuf);
                        d_assert(rv == CORE_OK, return CORE_ERROR,
                                "gtp_send error");
                    }
                    else
                    {
                        d_warn("[%d] %s Request Duplicated. Discard!"
                                " for type %d peer [%s]:%d",
                                xact->xid,
                                xact->org == GTP_LOCAL_ORIGINATOR ?
                                    "LOCAL " : "REMOTE",
                                xact->step, type,
                                CORE_ADDR(sock_remote_addr(xact->gnode->sock),
                                    buf),
                                CORE_PORT(sock_remote_addr(xact->gnode->sock)));
                    }

                    return CORE_EAGAIN;
                }

                d_assert(xact->step == 1, return CORE_ERROR,
                    "[%d] %s invalid step %d for type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));

                if (xact->tm_holding)
                    tm_start(xact->tm_holding);

                break;

            case GTP_XACT_FINAL_STAGE:
                d_assert(xact->step == 1, return CORE_ERROR,
                    "[%d] %s invalid step %d for type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));

                break;

            default:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);
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

                    d_assert(xact->step == 1 || xact->step == 2,
                        return CORE_ERROR,
                        "[%d] %s invalid step %d for type %d peer [%s]:%d",
                        xact->xid,
                        xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                        xact->step, type,
                        CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                        CORE_PORT(sock_remote_addr(xact->gnode->sock)));

                    pkbuf = xact->seq[1].pkbuf;
                    if (pkbuf)
                    {
                        if (xact->tm_holding)
                            tm_start(xact->tm_holding);

                        d_warn("[%d] %s Request Duplicated. Retransmit!"
                                " for step %d type %d peer [%s]:%d",
                                xact->xid,
                                xact->org == GTP_LOCAL_ORIGINATOR ?
                                    "LOCAL " : "REMOTE",
                                xact->step, type,
                                CORE_ADDR(sock_remote_addr(xact->gnode->sock),
                                    buf),
                                CORE_PORT(sock_remote_addr(xact->gnode->sock)));
                        rv = gtp_send(xact->gnode, pkbuf);
                        d_assert(rv == CORE_OK, return CORE_ERROR,
                                "gtp_send error");
                    }
                    else
                    {
                        d_warn("[%d] %s Request Duplicated. Discard!"
                                " for step %d type %d peer [%s]:%d",
                                xact->xid,
                                xact->org == GTP_LOCAL_ORIGINATOR ?
                                    "LOCAL " : "REMOTE",
                                xact->step, type,
                                CORE_ADDR(sock_remote_addr(xact->gnode->sock),
                                    buf),
                                CORE_PORT(sock_remote_addr(xact->gnode->sock)));
                    }

                    return CORE_EAGAIN;
                }

                d_assert(xact->step == 0, return CORE_ERROR,
                    "[%d] %s invalid step %d for type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));

                if (xact->tm_holding)
                    tm_start(xact->tm_holding);

                break;

            case GTP_XACT_INTERMEDIATE_STAGE:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);

            case GTP_XACT_FINAL_STAGE:
                d_assert(xact->step == 2, return CORE_ERROR,
                    "[%d] %s invalid step %d for type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));

                /* continue */
                break;

            default:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);
        }
    }
    else
        d_assert(0, return CORE_ERROR, "invalid orginator(%d)", xact->org);

    if (xact->tm_response)
        tm_stop(xact->tm_response);

    /* Save Message type of this step */
    xact->seq[xact->step].type = type;

    /* Step */
    xact->step++;

    return CORE_OK;
}


status_t gtp_xact_commit(gtp_xact_t *xact)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    c_uint8_t type;
    pkbuf_t *pkbuf = NULL;
    gtp_xact_stage_t stage;
    
    d_assert(xact, return CORE_ERROR, "Null param");
    d_assert(xact->gnode, return CORE_ERROR, "Null param");

    d_trace(15, "[%d] %s Commit  peer [%s]:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
            CORE_PORT(sock_remote_addr(xact->gnode->sock)));

    type = xact->seq[xact->step-1].type;
    stage = gtp_xact_get_stage(type, xact->xid);

    if (xact->org == GTP_LOCAL_ORIGINATOR)
    {
        switch(stage)
        {
            case GTP_XACT_INITIAL_STAGE:
            {
                d_assert(xact->step == 1, return CORE_ERROR,
                    "[%d] %s invalid step %d for type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));

                if (xact->tm_response)
                    tm_start(xact->tm_response);

                break;
            }

            case GTP_XACT_INTERMEDIATE_STAGE:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);

            case GTP_XACT_FINAL_STAGE:
                d_assert(xact->step == 2 || xact->step == 3, return CORE_ERROR,
                    "[%d] %s invalid step %d for type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));

                if (xact->step == 2)
                {
                    gtp_xact_delete(xact);
                    return CORE_OK;
                }

                break;

            default:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);
        }
    }
    else if (xact->org == GTP_REMOTE_ORIGINATOR)
    {
        switch(stage)
        {
            case GTP_XACT_INITIAL_STAGE:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);

            case GTP_XACT_INTERMEDIATE_STAGE:
                d_assert(xact->step == 2, return CORE_ERROR,
                    "[%d] %s invalid step %d for type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));

                if (xact->tm_response)
                    tm_start(xact->tm_response);

                break;

            case GTP_XACT_FINAL_STAGE:
                d_assert(xact->step == 2 || xact->step == 3,
                    return CORE_ERROR,
                    "[%d] %s invalid step %d for type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));

                if (xact->step == 3)
                {
                    gtp_xact_delete(xact);
                    return CORE_OK;
                }

                break;

            default:
                d_assert(0, return CORE_ERROR, "invalid step(%d)", xact->step);
        }
    }
    else
        d_assert(0, return CORE_ERROR, "invalid orginator(%d)", xact->org);

    pkbuf = xact->seq[xact->step-1].pkbuf;
    d_assert(pkbuf, return CORE_ERROR, "Null param");

    rv = gtp_send(xact->gnode, pkbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "gtp_send error");

    return CORE_OK;
}

status_t gtp_xact_timeout(index_t index, c_uintptr_t event)
{
    char buf[CORE_ADDRSTRLEN];
    gtp_xact_t *xact = NULL;
    
    d_assert(index, goto out, "Invalid Index");
    xact = index_find(&gtp_xact_pool, index);
    d_assert(xact, goto out, "Null param");
    d_assert(xact->gnode, goto out, "Null param");

    if (event == g_response_event)
    {
        d_trace(15, "[%d] %s Response Timeout "
                "for step %d type %d peer [%s]:%d\n",
                xact->xid,
                xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                xact->step, xact->seq[xact->step-1].type,
                CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                CORE_PORT(sock_remote_addr(xact->gnode->sock)));

        if (--xact->response_rcount > 0)
        {
            pkbuf_t *pkbuf = NULL;

            if (xact->tm_response)
                tm_start(xact->tm_response);

            pkbuf = xact->seq[xact->step-1].pkbuf;
            d_assert(pkbuf, return CORE_ERROR, "Null param");

            if (gtp_send(xact->gnode, pkbuf) != CORE_OK)
            {
                d_error("gtp_send() failed");
                goto out;
            }
        }
        else
        {
            d_warn("[%d] %s No Reponse. Give up! "
                    "for step %d type %d peer [%s]:%d",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, xact->seq[xact->step-1].type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));
            gtp_xact_delete(xact);
        }
    }
    else if (event == g_holding_event)
    {
        d_trace(15, "[%d] %s Holding Timeout "
                "for step %d type %d peer [%s]:%d\n",
                xact->xid,
                xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                xact->step, xact->seq[xact->step-1].type,
                CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                CORE_PORT(sock_remote_addr(xact->gnode->sock)));

        if (--xact->holding_rcount > 0)
        {
            if (xact->tm_holding)
                tm_start(xact->tm_holding);
        }
        else
        {
            d_trace(15, "[%d] %s Delete Transaction "
                    "for step %d type %d peer [%s]:%d\n",
                    xact->xid,
                    xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                    xact->step, xact->seq[xact->step-1].type,
                    CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
                    CORE_PORT(sock_remote_addr(xact->gnode->sock)));
            gtp_xact_delete(xact);
        }
    }

    return CORE_OK;

out:
    gtp_xact_delete(xact);
    return CORE_ERROR;
}

status_t gtp_xact_receive(
        gtp_node_t *gnode, gtp_header_t *h, gtp_xact_t **xact)
{
    char buf[CORE_ADDRSTRLEN];
    status_t rv;
    gtp_xact_t *new = NULL;

    d_assert(gnode, return CORE_ERROR, "Null param");
    d_assert(h, return CORE_ERROR, "Null param");

    new = gtp_xact_find_by_xid(gnode, h->type, GTP_SQN_TO_XID(h->sqn));
    if (!new)
        new = gtp_xact_remote_create(gnode, h->sqn);
    d_assert(new, return CORE_ERROR, "Null param");

    d_trace(15, "[%d] %s Receive peer [%s]:%d\n",
            new->xid,
            new->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            CORE_ADDR(sock_remote_addr(gnode->sock), buf),
            CORE_PORT(sock_remote_addr(gnode->sock)));

    rv = gtp_xact_update_rx(new, h->type);
    if (rv != CORE_OK)
    {
        return rv;
    }

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
    char buf[CORE_ADDRSTRLEN];
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
        d_trace(15, "[%d] %s Find    peer [%s]:%d\n",
                xact->xid,
                xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
                CORE_ADDR(sock_remote_addr(gnode->sock), buf),
                CORE_PORT(sock_remote_addr(gnode->sock)));
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
    char buf[CORE_ADDRSTRLEN];

    d_assert(xact, , "Null param");
    d_assert(xact->gnode, , "Null param");

    d_trace(15, "[%d] %s Delete  peer [%s]:%d\n",
            xact->xid,
            xact->org == GTP_LOCAL_ORIGINATOR ? "LOCAL " : "REMOTE",
            CORE_ADDR(sock_remote_addr(xact->gnode->sock), buf),
            CORE_PORT(sock_remote_addr(xact->gnode->sock)));

    if (xact->seq[0].pkbuf)
        pkbuf_free(xact->seq[0].pkbuf);
    if (xact->seq[1].pkbuf)
        pkbuf_free(xact->seq[1].pkbuf);
    if (xact->seq[2].pkbuf)
        pkbuf_free(xact->seq[2].pkbuf);

    if (xact->tm_response)
        tm_delete(xact->tm_response);
    if (xact->tm_holding)
        tm_delete(xact->tm_holding);

    if (xact->assoc_xact)
        gtp_xact_deassociate(xact, xact->assoc_xact);

    list_remove(xact->org == GTP_LOCAL_ORIGINATOR ? &xact->gnode->local_list :
            &xact->gnode->remote_list, xact);
    index_free(&gtp_xact_pool, xact);

    return CORE_OK;
}

