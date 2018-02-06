#define TRACE_MODULE _pcscf_fd_path

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"
#include "core_network.h"
#include "3gpp_types.h"

#include "gtp/gtp_xact.h"

#include "fd/fd_lib.h"
#include "fd/rx/rx_dict.h"
#include "fd/rx/rx_message.h"

#include "pcscf_fd_path.h"

#define MAX_NUM_SESSION_STATE 32

static struct session_handler *pcscf_rx_reg = NULL;
static struct disp_hdl *hdl_rx_fb = NULL; 
static struct disp_hdl *hdl_rx_asr = NULL; 
static fd_config_t fd_config;

struct sess_state {
    os0_t       sid;            /* Rx Session-Id */

    struct timespec ts;         /* Time of sending the message */
};

pool_declare(pcscf_rx_sess_pool, struct sess_state, MAX_NUM_SESSION_STATE);

static void pcscf_rx_aaa_cb(void *data, struct msg **msg);
static void pcscf_rx_sta_cb(void *data, struct msg **msg);

static __inline__ struct sess_state *new_state(os0_t sid)
{
    struct sess_state *new = NULL;

    pool_alloc_node(&pcscf_rx_sess_pool, &new);
    d_assert(new, return NULL,);
    memset(new, 0, sizeof *new);

    new->sid = (os0_t)core_strdup((char *)sid);
    d_assert(new->sid, return NULL,);

    return new;
}

static void state_cleanup(struct sess_state *sess_data, os0_t sid, void *opaque)
{
    if (sess_data->sid)
        CORE_FREE(sess_data->sid);

    pool_free_node(&pcscf_rx_sess_pool, sess_data);
}

static int pcscf_rx_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

void pcscf_rx_send_aar(c_uint8_t **rx_sid, const char *ip,
        int qos_type, int flow_presence)
{
    status_t rv;
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch1, *avpch2;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    int new;

    paa_t paa;
    ipsubnet_t ipsub;

    d_assert(rx_sid, return,);

    d_assert(ip, return,);
    rv = core_ipsubnet(&ipsub, ip, NULL);
    d_assert(rv == CORE_OK, return,);

    /* Create the request */
    ret = fd_msg_new(rx_cmd_aar, MSGFL_ALLOC_ETEID, &req);
    d_assert(ret == 0, return,);
    {
        struct msg_hdr * h;
        ret = fd_msg_hdr( req, &h );
        d_assert(ret == 0, return,);
        h->msg_appl = RX_APPLICATION_ID;
    }

    /* Find Diameter Rx Session */
    if (*rx_sid)
    {
        /* Retrieve session by Session-Id */
        size_t sidlen = strlen((char *)*rx_sid);
		ret = fd_sess_fromsid_msg(*rx_sid, sidlen, &session, &new);
        d_assert(ret == 0, return,);
        d_assert(new == 0, return,);

        /* Add Session-Id to the message */
        ret = fd_message_session_id_set(req, *rx_sid, sidlen);
        d_assert(ret == 0, return,);
        /* Save the session associated with the message */
        ret = fd_msg_sess_set(req, session);
    }
    else
    {
        /* Create a new session */
        #define RX_APP_SID_OPT  "app_rx"
        ret = fd_msg_new_session(req, (os0_t)RX_APP_SID_OPT, 
                CONSTSTRLEN(RX_APP_SID_OPT));
        d_assert(ret == 0, return,);
        ret = fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL);
        d_assert(ret == 0, return,);
    }

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(pcscf_rx_reg, session, &sess_data);
    if (!sess_data)
    {
        os0_t sid;
        size_t sidlen;

        ret = fd_sess_getsid(session, &sid, &sidlen);
        d_assert(ret == 0, return,);

        /* Allocate new session state memory */
        sess_data = new_state(sid);
        d_assert(sess_data, return,);

        /* Save Session-Id to PGW Session Context */
        *rx_sid = sess_data->sid;
    }
    
    /* Set Origin-Host & Origin-Realm */
    ret = fd_msg_add_origin(req, 0);
    d_assert(ret == 0, return,);
    
    /* Set the Destination-Realm AVP */
    ret = fd_msg_avp_new(fd_destination_realm, 0, &avp);
    d_assert(ret == 0, return,);
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(fd_auth_application_id, 0, &avp);
    d_assert(ret == 0, return,);
    val.i32 = RX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set Subscription-Id */
    ret = fd_msg_avp_new(rx_subscription_id, 0, &avp);
    d_assert(ret == 0, return,);

    ret = fd_msg_avp_new(rx_subscription_id_type, 0, &avpch1);
    d_assert(ret == 0, return,);
    val.i32 = RX_SUBSCRIPTION_ID_TYPE_END_USER_IMSI;
    ret = fd_msg_avp_setvalue (avpch1, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    d_assert(ret == 0, return,);

    #define RX_APP_IMSI_BCD  "0123456789012345"
    ret = fd_msg_avp_new(rx_subscription_id_data, 0, &avpch1);
    d_assert(ret == 0, return,);
    val.os.data = (c_uint8_t *)RX_APP_IMSI_BCD;
    val.os.len  = strlen(RX_APP_IMSI_BCD);
    ret = fd_msg_avp_setvalue (avpch1, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    d_assert(ret == 0, return,);

    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    if (ipsub.family == AF_INET)
    {
        /* Set Framed-IP-Address */
        ret = fd_msg_avp_new(rx_framed_ip_address, 0, &avp);
        d_assert(ret == 0, return,);
        val.os.data = (c_uint8_t*)ipsub.sub;
        val.os.len = IPV4_LEN;
        ret = fd_msg_avp_setvalue(avp, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        d_assert(ret == 0, return,);
    }
    else if (ipsub.family == AF_INET6)
    {
        /* Set Framed-IPv6-Prefix */
        ret = fd_msg_avp_new(rx_framed_ipv6_prefix, 0, &avp);
        d_assert(ret == 0, return,);
        memset(&paa, 0, sizeof(paa_t));

        memcpy(paa.addr6, ipsub.sub, IPV6_LEN);
        paa.pdn_type = 0x03;
#define FRAMED_IPV6_PREFIX_LENGTH 128  /* from spec document */
        paa.len = FRAMED_IPV6_PREFIX_LENGTH; 
        val.os.data = (c_uint8_t*)&paa;
        val.os.len = PAA_IPV6_LEN;
        ret = fd_msg_avp_setvalue(avp, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        d_assert(ret == 0, return,);
    }

    /* Set Media-Component-Description */
    ret = fd_msg_avp_new(rx_media_component_description, 0, &avp);
    d_assert(ret == 0, return,);

    ret = fd_msg_avp_new(rx_media_component_number, 0, &avpch1);
    d_assert(ret == 0, return,);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue (avpch1, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    d_assert(ret == 0, return,);

    ret = fd_msg_avp_new(rx_media_type, 0, &avpch1);
    d_assert(ret == 0, return,);
    val.i32 = RX_MEDIA_TYPE_AUDIO;
    ret = fd_msg_avp_setvalue (avpch1, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    d_assert(ret == 0, return,);

    if (qos_type == 1)
    {
        ret = fd_msg_avp_new(rx_max_requested_bandwidth_dl, 0, &avpch1);
        d_assert(ret == 0, return,);
        val.i32 = 96000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(rx_max_requested_bandwidth_ul, 0, &avpch1);
        d_assert(ret == 0, return,);
        val.i32 = 96000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(rx_rr_bandwidth, 0, &avpch1);
        d_assert(ret == 0, return,);
        val.i32 = 2400;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(rx_rs_bandwidth, 0, &avpch1);
        d_assert(ret == 0, return,);
        val.i32 = 2400;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);
    }
    else if (qos_type == 2)
    {
        ret = fd_msg_avp_new(rx_max_requested_bandwidth_dl, 0, &avpch1);
        d_assert(ret == 0, return,);
        val.i32 = 96000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(rx_max_requested_bandwidth_ul, 0, &avpch1);
        d_assert(ret == 0, return,);
        val.i32 = 96000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(rx_min_requested_bandwidth_dl, 0, &avpch1);
        d_assert(ret == 0, return,);
        val.i32 = 88000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(rx_min_requested_bandwidth_ul, 0, &avpch1);
        d_assert(ret == 0, return,);
        val.i32 = 88000;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);
    }

    if (flow_presence)
    {
        /* Set Media-Sub-Component #1 */
        ret = fd_msg_avp_new(rx_media_sub_component, 0, &avpch1);

        ret = fd_msg_avp_new(rx_flow_number, 0, &avpch2);
        d_assert(ret == 0, return,);
        val.i32 = 1;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(rx_flow_description, 0, &avpch2);
        d_assert(ret == 0, return,);
        #define TEST_RX_FLOW_DESC1  \
            "permit out 17 from 172.20.166.84 to 172.18.128.20 20001"
        val.os.data = (c_uint8_t *)TEST_RX_FLOW_DESC1;
        val.os.len  = strlen(TEST_RX_FLOW_DESC1);
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(rx_flow_description, 0, &avpch2);
        d_assert(ret == 0, return,);
        #define TEST_RX_FLOW_DESC2  \
            "permit in 17 from 172.18.128.20 to 172.20.166.84 20360"
        val.os.data = (c_uint8_t *)TEST_RX_FLOW_DESC2;
        val.os.len  = strlen(TEST_RX_FLOW_DESC2);
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);

        /* Set Media-Sub-Component #2 */
        ret = fd_msg_avp_new(rx_media_sub_component, 0, &avpch1);

        ret = fd_msg_avp_new(rx_flow_number, 0, &avpch2);
        d_assert(ret == 0, return,);
        val.i32 = 2;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(rx_flow_usage, 0, &avpch2);
        d_assert(ret == 0, return,);
        val.i32 = RX_FLOW_USAGE_RTCP;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(rx_flow_description, 0, &avpch2);
        d_assert(ret == 0, return,);
        #define TEST_RX_FLOW_DESC3  \
            "permit out 17 from 172.20.166.84 to 172.18.128.20 20002"
        val.os.data = (c_uint8_t *)TEST_RX_FLOW_DESC3;
        val.os.len  = strlen(TEST_RX_FLOW_DESC3);
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(rx_flow_description, 0, &avpch2);
        d_assert(ret == 0, return,);
        #define TEST_RX_FLOW_DESC4  \
            "permit in 17 from 172.18.128.20 to 172.20.166.84 20361"
        val.os.data = (c_uint8_t *)TEST_RX_FLOW_DESC4;
        val.os.len  = strlen(TEST_RX_FLOW_DESC4);
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);
    }

    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    ret = clock_gettime(CLOCK_REALTIME, &sess_data->ts);
    d_assert(ret == 0, return,);
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = sess_data;
    
    /* Store this value in the session */
    ret = fd_sess_state_store(pcscf_rx_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data == NULL,,);
    
    /* Send the request */
    ret = fd_msg_send(&req, pcscf_rx_aaa_cb, svg);
    d_assert(ret == 0,,);

    /* Increment the counter */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,,);
    fd_logger_self()->stats.nb_sent++;
    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,,);
}

static void pcscf_rx_aaa_cb(void *data, struct msg **msg)
{
    int ret;

    struct sess_state *sess_data = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch1;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;
    c_int32_t result_code = 0;

    ret = clock_gettime(CLOCK_REALTIME, &ts);
    d_assert(ret == 0, return,);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    d_assert(ret == 0, return,);
    d_assert(new == 0, return, );
    
    ret = fd_sess_state_retrieve(pcscf_rx_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data && (void *)sess_data == data, return, );

    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, fd_result_code, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        result_code = hdr->avp_value->i32;
        d_trace(3, "Result Code: %d\n", hdr->avp_value->i32);
    }
    else
    {
        ret = fd_msg_search_avp(*msg, fd_experimental_result, &avp);
        d_assert(ret == 0, return,);
        if (avp)
        {
            ret = fd_avp_search_avp(avp, fd_experimental_result_code, &avpch1);
            d_assert(ret == 0, return,);
            if (avpch1)
            {
                ret = fd_msg_avp_hdr(avpch1, &hdr);
                d_assert(ret == 0, return,);
                result_code = hdr->avp_value->i32;
                d_trace(3, "Experimental Result Code: %d\n",
                        result_code);
            }
        }
        else
        {
            d_error("no Result-Code");
            error++;
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, fd_origin_host, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        d_trace(3, "From '%.*s' ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Host ");
        error++;
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, fd_origin_realm, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        d_trace(3, "('%.*s') ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Realm ");
        error++;
    }

    if (result_code != ER_DIAMETER_SUCCESS)
    {
        d_warn("ERROR DIAMETER Result Code(%d)", result_code);
        error++;
        goto out;
    }

out:
    /* Free the message */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,, );
    dur = ((ts.tv_sec - sess_data->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    if (fd_logger_self()->stats.nb_recv)
    {
        /* Ponderate in the avg */
        fd_logger_self()->stats.avg = (fd_logger_self()->stats.avg * 
            fd_logger_self()->stats.nb_recv + dur) /
            (fd_logger_self()->stats.nb_recv + 1);
        /* Min, max */
        if (dur < fd_logger_self()->stats.shortest)
            fd_logger_self()->stats.shortest = dur;
        if (dur > fd_logger_self()->stats.longest)
            fd_logger_self()->stats.longest = dur;
    }
    else
    {
        fd_logger_self()->stats.shortest = dur;
        fd_logger_self()->stats.longest = dur;
        fd_logger_self()->stats.avg = dur;
    }
    if (error)
        fd_logger_self()->stats.nb_errs++;
    else 
        fd_logger_self()->stats.nb_recv++;

    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,, );
    
    /* Display how long it took */
    if (ts.tv_nsec > sess_data->ts.tv_nsec)
        d_trace(3, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        d_trace(3, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);

    ret = fd_sess_state_store(pcscf_rx_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data == NULL, return,);
    
    ret = fd_msg_free(*msg);
    d_assert(ret == 0,,);
    *msg = NULL;

    return;
}

static int pcscf_rx_asr_cb( struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
    status_t rv;
    int ret;

	struct msg *ans, *qry;
    struct avp *avpch1, *avpch2, *avpch3;
    struct avp_hdr *hdr;
    union avp_value val;
    struct sess_state *sess_data = NULL;
    os0_t sid;
    size_t sidlen;

    d_assert(msg, return EINVAL,);
    d_assert(sess, return EINVAL,);

    ret = fd_sess_state_retrieve(pcscf_rx_reg, sess, &sess_data);
    d_assert(ret == 0, return EINVAL,);
    d_assert(sess_data, return EINVAL,);

	/* Create answer header */
	qry = *msg;
	ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    d_assert(ret == 0, return EINVAL,);
    ans = *msg;

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(fd_auth_application_id, 0, &avp);
    d_assert(ret == 0, return EINVAL,);
    val.i32 = RX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return EINVAL,);

    /* Set the Auth-Request-Type AVP */
    ret = fd_msg_avp_new(fd_auth_request_type, 0, &avp);
    d_assert(ret == 0, return EINVAL,);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return EINVAL,);

    /* Get Abort-Cause */
    ret = fd_msg_search_avp(qry, rx_abort_cause, &avp);
    d_assert(ret == 0, return EINVAL,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return EINVAL,);
    }
    else
    {
        d_error("no_Abort-Cause ");
    }

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	ret = fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1);
    d_assert(ret == 0, return EINVAL,);

    /* Store this value in the session */
    sid = sess_data->sid;
    d_assert(sid,,);

    ret = fd_sess_state_store(pcscf_rx_reg, sess, &sess_data);
    d_assert(ret == 0,,);
    d_assert(sess_data == NULL,,);

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    d_assert(ret == 0,,);

	/* Add this value to the stats */
	d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,,);
	fd_logger_self()->stats.nb_echoed++;
	d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,,);

    pcscf_rx_send_str(sid);

    return 0;
}

void pcscf_rx_send_str(c_uint8_t *rx_sid)
{
    status_t rv;
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch1, *avpch2;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    int new;

    d_assert(rx_sid, return,);

    /* Create the request */
    ret = fd_msg_new(rx_cmd_str, MSGFL_ALLOC_ETEID, &req);
    d_assert(ret == 0, return,);
    {
        struct msg_hdr * h;
        ret = fd_msg_hdr( req, &h );
        d_assert(ret == 0, return,);
        h->msg_appl = RX_APPLICATION_ID;
    }

    /* Retrieve session by Session-Id */
    size_t sidlen = strlen((char*)rx_sid);
    ret = fd_sess_fromsid_msg(rx_sid, sidlen, &session, &new);
    d_assert(ret == 0, return,);
    d_assert(new == 0, return,);

    /* Add Session-Id to the message */
    ret = fd_message_session_id_set(req, rx_sid, sidlen);
    d_assert(ret == 0, return,);
    /* Save the session associated with the message */
    ret = fd_msg_sess_set(req, session);

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(pcscf_rx_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data, return,);
    
    /* Set Origin-Host & Origin-Realm */
    ret = fd_msg_add_origin(req, 0);
    d_assert(ret == 0, return,);
    
    /* Set the Destination-Realm AVP */
    ret = fd_msg_avp_new(fd_destination_realm, 0, &avp);
    d_assert(ret == 0, return,);
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(fd_auth_application_id, 0, &avp);
    d_assert(ret == 0, return,);
    val.i32 = RX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set the Termination-Cause AVP */
    ret = fd_msg_avp_new(rx_termination_cause, 0, &avp);
    d_assert(ret == 0, return,);
    val.i32 = RX_TERMINATION_CAUSE_DIAMETER_LOGOUT;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = sess_data;
    
    /* Store this value in the session */
    ret = fd_sess_state_store(pcscf_rx_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data == NULL,,);
    
    /* Send the request */
    ret = fd_msg_send(&req, pcscf_rx_sta_cb, svg);
    d_assert(ret == 0,,);

    /* Increment the counter */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,,);
    fd_logger_self()->stats.nb_sent++;
    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,,);
}

static void pcscf_rx_sta_cb(void *data, struct msg **msg)
{
    int ret;

    struct sess_state *sess_data = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch1;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;
    c_int32_t result_code = 0;

    ret = clock_gettime(CLOCK_REALTIME, &ts);
    d_assert(ret == 0, return,);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    d_assert(ret == 0, return,);
    d_assert(new == 0, return, );
    
    ret = fd_sess_state_retrieve(pcscf_rx_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data && (void *)sess_data == data, return, );

    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, fd_result_code, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        result_code = hdr->avp_value->i32;
        d_trace(3, "Result Code: %d\n", hdr->avp_value->i32);
    }
    else
    {
        ret = fd_msg_search_avp(*msg, fd_experimental_result, &avp);
        d_assert(ret == 0, return,);
        if (avp)
        {
            ret = fd_avp_search_avp(avp, fd_experimental_result_code, &avpch1);
            d_assert(ret == 0, return,);
            if (avpch1)
            {
                ret = fd_msg_avp_hdr(avpch1, &hdr);
                d_assert(ret == 0, return,);
                result_code = hdr->avp_value->i32;
                d_trace(3, "Experimental Result Code: %d\n",
                        result_code);
            }
        }
        else
        {
            d_error("no Result-Code");
            error++;
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, fd_origin_host, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        d_trace(3, "From '%.*s' ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Host ");
        error++;
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, fd_origin_realm, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        d_trace(3, "('%.*s') ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Realm ");
        error++;
    }

    if (result_code != ER_DIAMETER_SUCCESS)
    {
        d_warn("ERROR DIAMETER Result Code(%d)", result_code);
        error++;
        goto out;
    }

out:
    /* Free the message */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,, );
    dur = ((ts.tv_sec - sess_data->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    if (fd_logger_self()->stats.nb_recv)
    {
        /* Ponderate in the avg */
        fd_logger_self()->stats.avg = (fd_logger_self()->stats.avg * 
            fd_logger_self()->stats.nb_recv + dur) /
            (fd_logger_self()->stats.nb_recv + 1);
        /* Min, max */
        if (dur < fd_logger_self()->stats.shortest)
            fd_logger_self()->stats.shortest = dur;
        if (dur > fd_logger_self()->stats.longest)
            fd_logger_self()->stats.longest = dur;
    }
    else
    {
        fd_logger_self()->stats.shortest = dur;
        fd_logger_self()->stats.longest = dur;
        fd_logger_self()->stats.avg = dur;
    }
    if (error)
        fd_logger_self()->stats.nb_errs++;
    else 
        fd_logger_self()->stats.nb_recv++;

    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,, );
    
    /* Display how long it took */
    if (ts.tv_nsec > sess_data->ts.tv_nsec)
        d_trace(3, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        d_trace(3, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);

    state_cleanup(sess_data, NULL, NULL);
    
    ret = fd_msg_free(*msg);
    d_assert(ret == 0,,);
    *msg = NULL;

    return;
}

void pcscf_fd_config()
{
    memset(&fd_config, 0, sizeof(fd_config_t));

    fd_config.cnf_diamid = "pcscf.open-ims.test";
    fd_config.cnf_diamrlm = "open-ims.test";
    fd_config.cnf_port = DIAMETER_PORT;
    fd_config.cnf_port_tls = DIAMETER_SECURE_PORT;
    fd_config.cnf_flags.no_sctp = 1;
    fd_config.cnf_addr = "127.0.0.1";

    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dbg_msg_dumps/.libs/dbg_msg_dumps.so";
    fd_config.ext[fd_config.num_of_ext].conf = "0x8888";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_rfc5777/.libs/dict_rfc5777.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_mip6i/.libs/dict_mip6i.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_nasreq/.libs/dict_nasreq.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_nas_mipv6/.libs/dict_nas_mipv6.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_dcca/.libs/dict_dcca.so";
    fd_config.num_of_ext++;
    fd_config.ext[fd_config.num_of_ext].module =
        FD_EXT_DIR "/dict_dcca_3gpp/.libs/dict_dcca_3gpp.so";
    fd_config.num_of_ext++;

    fd_config.conn[fd_config.num_of_conn].identity = "pcrf.open-ims.test";
    fd_config.conn[fd_config.num_of_conn].addr = "127.0.0.5";
    fd_config.num_of_conn++;
}

status_t pcscf_fd_init(void)
{
    int ret;
	struct disp_when data;

    pool_init(&pcscf_rx_sess_pool, MAX_NUM_SESSION_STATE);

    pcscf_fd_config();

    ret = fd_init(FD_MODE_CLIENT, NULL, &fd_config);
    d_assert(ret == 0, return CORE_ERROR,);

	/* Install objects definitions for this application */
	ret = rx_dict_init();
    d_assert(ret == 0, return CORE_ERROR,);

    /* Create handler for sessions */
	ret = fd_sess_handler_create(&pcscf_rx_reg, state_cleanup, NULL, NULL);
    d_assert(ret == 0, return CORE_ERROR,);

	/* Fallback CB if command != unexpected message received */
	memset(&data, 0, sizeof(data));
	data.app = rx_application;

	ret = fd_disp_register(pcscf_rx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_rx_fb);
    d_assert(ret == 0, return CORE_ERROR,);
	
	/* Specific handler for Abort-Session-Request */
	data.command = rx_cmd_asr;
	ret = fd_disp_register(pcscf_rx_asr_cb, DISP_HOW_CC, &data, NULL,
                &hdl_rx_asr);
    d_assert(ret == 0, return CORE_ERROR,);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(rx_application, fd_vendor, 1, 0);
    d_assert(ret == 0, return CORE_ERROR,);
	
	return 0;
}

void pcscf_fd_final(void)
{
    int ret;
	ret = fd_sess_handler_destroy(&pcscf_rx_reg, NULL);
    d_assert(ret == 0,,);

	if (hdl_rx_fb)
		(void) fd_disp_unregister(&hdl_rx_fb, NULL);
	if (hdl_rx_asr)
		(void) fd_disp_unregister(&hdl_rx_asr, NULL);

    fd_final();

    if (pool_used(&pcscf_rx_sess_pool))
        d_error("%d not freed in pcscf_rx_sess_pool[%d] of S6A-SM",
                pool_used(&pcscf_rx_sess_pool), pool_size(&pcscf_rx_sess_pool));
    d_trace(5, "%d not freed in pcscf_rx_sess_pool[%d] of S6A-SM\n",
            pool_used(&pcscf_rx_sess_pool), pool_size(&pcscf_rx_sess_pool));

    pool_final(&pcscf_rx_sess_pool);
}
