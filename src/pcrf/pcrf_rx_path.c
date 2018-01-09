#define TRACE_MODULE _pcrf_rx_path

#include "core_debug.h"
#include "core_pool.h"
#include "core_pkbuf.h"
#include "core_network.h"

#include "fd/fd_lib.h"
#include "fd/rx/rx_dict.h"
#include "fd/rx/rx_message.h"

#include "pcrf_context.h"
#include "pcrf_fd_path.h"

struct sess_state {
    os0_t    rx_sid;            /* Rx Session-Id */
    os0_t    gx_sid;            /* Gx Session-Id */
};

static struct session_handler *pcrf_rx_reg = NULL;
static struct disp_hdl *hdl_rx_fb = NULL; 
static struct disp_hdl *hdl_rx_aar = NULL; 

pool_declare(pcrf_rx_sess_pool, struct sess_state, MAX_POOL_OF_DIAMETER_SESS);

static __inline__ struct sess_state *new_state(os0_t sid)
{
    struct sess_state *new = NULL;
    pool_alloc_node(&pcrf_rx_sess_pool, &new);
    d_assert(new, return NULL,);
    memset(new, 0, sizeof *new);

    new->rx_sid = (os0_t)core_strdup((char *)sid);
    d_assert(new->rx_sid, return NULL,);

    return new;
}

static void state_cleanup(
        struct sess_state *sess_data, os0_t sid, void *opaque)
{
    d_assert(sess_data, return,);

    if (sess_data->rx_sid)
        core_free((char *)sess_data->rx_sid);
    if (sess_data->gx_sid)
        core_free((char *)sess_data->gx_sid);

    pool_free_node(&pcrf_rx_sess_pool, sess_data);
}

static int pcrf_rx_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

static int pcrf_rx_aar_cb( struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
    status_t rv;

	struct msg *ans, *qry;
#if 0
    struct avp *avpch1, *avpch2, *avpch3, *avpch4;
#endif
    struct avp_hdr *hdr;
    union avp_value val;
    struct sess_state *sess_data = NULL;

#if 0
    gx_cca_message_t cca_message;
    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];
    c_int8_t apn[MAX_APN_LEN+1];
    int i, j;

    c_uint32_t cc_request_type = 0;
#endif
    char buf[CORE_ADDRSTRLEN];
    os0_t rx_sid = NULL;
    os0_t gx_sid = NULL;
    size_t sidlen;
    c_uint32_t result_code = RX_DIAMETER_IP_CAN_SESSION_NOT_AVAILABLE;
	
    d_assert(msg, return EINVAL,);
    d_assert(sess, return EINVAL,);

    d_assert( fd_sess_state_retrieve(pcrf_rx_reg, sess, &sess_data) == 0,
            return EINVAL,);
    if (!sess_data)
    {
        d_assert( fd_sess_getsid(sess, &rx_sid, &sidlen) == 0, return EINVAL,);

        sess_data = new_state(rx_sid);
        d_assert(sess_data, return EINVAL,);
    }

#if 0
    /* Initialize Message */
    memset(&cca_message, 0, sizeof(gx_cca_message_t));
#endif

	/* Create answer header */
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0) );
    ans = *msg;

    /* Set the Auth-Application-Id AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_application_id, 0, &avp), goto out );
    val.i32 = RX_APPLICATION_ID;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set the Auth-Request-Type AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_request_type, 0, &avp), goto out );
    val.i32 = 1;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Get Framed-IP-Address */
    CHECK_FCT( fd_msg_search_avp(qry, rx_framed_ip_address, &avp) );
    if (avp)
    {
        CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
        gx_sid = (os0_t)pcrf_sess_find_by_ipv4(hdr->avp_value->os.data);
        if (!gx_sid)
        {
            d_warn("Cannot find Gx Sesson for IPv4:%s\n",
                    INET_NTOP(hdr->avp_value->os.data, buf));
        }
    }

    if (!gx_sid)
    {
        /* Get Framed-IPv6-Prefix */
        CHECK_FCT( fd_msg_search_avp(qry, rx_framed_ipv6_prefix, &avp) );
        if (avp)
        {
            paa_t *paa = NULL;

            CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
            paa = (paa_t *)hdr->avp_value->os.data;
            d_assert(paa, goto out,);
            d_assert(paa->len == IPV6_LEN * 8 /* 128bit */, goto out,
                    "Invalid Framed-IPv6-Prefix Length:%d", paa->len);
            gx_sid = (os0_t)pcrf_sess_find_by_ipv6(paa->addr6);
            if (!gx_sid)
            {
                d_warn("Cannot find Gx Sesson for IPv6:%s\n",
                        INET6_NTOP(hdr->avp_value->os.data, buf));
            }
        }
    }

    if (!gx_sid) goto out;

    /* Associate Gx-session with Rx-session */
    rv = pcrf_sess_gx_associate_rx(gx_sid, rx_sid);
    d_assert(rv == CORE_OK, goto out,);

    /* Store Gx Session-Id in this session */
    if (sess_data->gx_sid)
        core_free(sess_data->gx_sid);
    sess_data->gx_sid = (os0_t)core_strdup((char *)gx_sid);
    d_assert(sess_data->gx_sid, goto out,);

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) );

    /* Store this value in the session */
    CHECK_FCT_DO( fd_sess_state_store(pcrf_rx_reg, sess, &sess_data),
            goto out );
    d_assert(sess_data == NULL,,);

	/* Send the answer */
	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );

	/* Add this value to the stats */
	CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
	fd_logger_self()->stats.nb_echoed++;
	CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

#if 0
    gx_cca_message_free(&cca_message);
#endif

    return 0;

out:
    if (result_code == RX_DIAMETER_REQUESTED_SERVICE_TEMPORARILY_NOT_AUTHORIZED)
    {
        CHECK_FCT( fd_msg_rescode_set(ans,
                "RX_DIAMETER_REQUESTED_SERVICE_TEMPORARILY_NOT_AUTHORIZED",
                NULL, NULL, 1) );
    }
    else
    {
        CHECK_FCT( fd_message_experimental_rescode_set(ans, result_code) );
    }

	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );

    state_cleanup(sess_data, NULL, NULL);
#if 0
    gx_cca_message_free(&cca_message);
#endif

    return 0;
}

int pcrf_rx_init(void)
{
	struct disp_when data;

    pool_init(&pcrf_rx_sess_pool, MAX_POOL_OF_DIAMETER_SESS);

	/* Install objects definitions for this application */
	CHECK_FCT( rx_dict_init() );

    /* Create handler for sessions */
	CHECK_FCT( fd_sess_handler_create(&pcrf_rx_reg, state_cleanup,
                NULL, NULL) );

	memset(&data, 0, sizeof(data));
	data.app = rx_application;
	
	CHECK_FCT( fd_disp_register(pcrf_rx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_rx_fb) );
	
	data.command = rx_cmd_aar;
	CHECK_FCT( fd_disp_register(pcrf_rx_aar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_rx_aar) );

	/* Advertise the support for the application in the peer */
	CHECK_FCT( fd_disp_app_support(rx_application, fd_vendor, 1, 0) );

	return 0;
}

void pcrf_rx_final(void)
{
	CHECK_FCT_DO( fd_sess_handler_destroy(&pcrf_rx_reg, NULL), );
	if (hdl_rx_fb)
		(void) fd_disp_unregister(&hdl_rx_fb, NULL);
	if (hdl_rx_aar)
		(void) fd_disp_unregister(&hdl_rx_aar, NULL);

    if (pool_used(&pcrf_rx_sess_pool))
        d_error("%d not freed in pcrf_rx_sess_pool[%d] of GX-SM",
                pool_used(&pcrf_rx_sess_pool), pool_size(&pcrf_rx_sess_pool));
    d_trace(3, "%d not freed in pcrf_rx_sess_pool[%d] of GX-SM\n",
            pool_used(&pcrf_rx_sess_pool), pool_size(&pcrf_rx_sess_pool));

    pool_final(&pcrf_rx_sess_pool);
}
