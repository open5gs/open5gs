#define TRACE_MODULE _pcrf_rx_path

#include "core_debug.h"
#include "core_pool.h"
#include "core_pkbuf.h"
#include "core_network.h"
#include "core_lib.h"

#include "fd/fd_lib.h"
#include "fd/rx/rx_dict.h"
#include "fd/rx/rx_message.h"

#include "pcrf_context.h"
#include "pcrf_fd_path.h"

struct sess_state {
    os0_t   rx_sid;             /* Rx Session-Id */
    os0_t   gx_sid;             /* Gx Session-Id */

    os0_t   peer_host;          /* Peer Host */

#define SESSION_ABORTED         1
    int     state;

    int     abort_cause;
    int     termination_cause;

    struct timespec ts;         /* Time of sending the message */
};

static struct session_handler *pcrf_rx_reg = NULL;
static struct disp_hdl *hdl_rx_fb = NULL; 
static struct disp_hdl *hdl_rx_aar = NULL; 
static struct disp_hdl *hdl_rx_str = NULL; 

pool_declare(pcrf_rx_sess_pool, struct sess_state, MAX_POOL_OF_DIAMETER_SESS);

static void pcrf_rx_asa_cb(void *data, struct msg **msg);

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

static void state_cleanup(struct sess_state *sess_data, os0_t sid, void *opaque)
{
    d_assert(sess_data, return,);

    if (sess_data->rx_sid)
        CORE_FREE((char *)sess_data->rx_sid);
    if (sess_data->gx_sid)
        CORE_FREE((char *)sess_data->gx_sid);

    if (sess_data->peer_host)
        CORE_FREE(sess_data->peer_host);

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
    int ret;

	struct msg *ans, *qry;
    struct avp *avpch1, *avpch2, *avpch3;
    struct avp_hdr *hdr;
    union avp_value val;
    struct sess_state *sess_data = NULL;
    size_t sidlen;

    rx_message_t rx_message;

    char buf[CORE_ADDRSTRLEN];
    os0_t gx_sid = NULL;
    c_uint32_t result_code = RX_DIAMETER_IP_CAN_SESSION_NOT_AVAILABLE;

    d_trace(3, "[PCRF] AA-Request\n");
	
    d_assert(msg, return EINVAL,);
    d_assert(sess, return EINVAL,);

    ret = fd_sess_state_retrieve(pcrf_rx_reg, sess, &sess_data);
    d_assert(ret == 0, return EINVAL,);
    if (!sess_data)
    {
        os0_t sid = NULL;
        ret = fd_sess_getsid(sess, &sid, &sidlen);
        d_assert(ret == 0, return EINVAL,);

        sess_data = new_state(sid);
        d_assert(sess_data, return EINVAL,);
    }

    /* Initialize Message */
    memset(&rx_message, 0, sizeof(rx_message_t));
    rx_message.cmd_code = RX_CMD_CODE_AA;

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

    /* Get Framed-IP-Address */
    ret = fd_msg_search_avp(qry, rx_framed_ip_address, &avp);
    d_assert(ret == 0, return EINVAL,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return EINVAL,);
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
        ret = fd_msg_search_avp(qry, rx_framed_ipv6_prefix, &avp);
        d_assert(ret == 0, return EINVAL,);
        if (avp)
        {
            paa_t *paa = NULL;

            ret = fd_msg_avp_hdr(avp, &hdr);
            d_assert(ret == 0, return EINVAL,);
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

    if (!gx_sid)
    {
        d_error("No Gx Session");
        goto out;
    }

    ret = fd_msg_browse(qry, MSG_BRW_FIRST_CHILD, &avpch1, NULL);
    d_assert(ret == 0, return EINVAL,);
    while(avpch1)
    {
        ret = fd_msg_avp_hdr(avpch1, &hdr);
        d_assert(ret == 0, return EINVAL,);
        switch(hdr->avp_code)
        {
            case AC_SESSION_ID:
            case AC_ORIGIN_HOST:
            {
                if (sess_data->peer_host)
                    CORE_FREE(sess_data->peer_host);
                sess_data->peer_host =
                    (os0_t)core_strdup((char *)hdr->avp_value->os.data);
                d_assert(sess_data->peer_host, return CORE_ERROR,);
                break;
            }
            case AC_ORIGIN_REALM:
            case AC_DESTINATION_REALM:
            case AC_ROUTE_RECORD:
            case AC_PROXY_INFO:
            case AC_AUTH_APPLICATION_ID:
            case FD_AVP_CODE_FRAME_IP_ADDRESS:
            case FD_AVP_CODE_FRAME_IPV6_PREFIX:
            case RX_AVP_CODE_SUBSCRIPTION_ID:
                break;
            /* Gwt Specific-Action */
            case RX_AVP_CODE_SPECIFIC_ACTION:
                break;
            /* Gwt Media-Component-Description */
            case RX_AVP_CODE_MEDIA_COMPONENT_DESCRIPTION:
            {
                rx_media_component_t *media_component = &rx_message.
                        media_component[rx_message.num_of_media_component];

                ret = fd_msg_browse(avpch1, MSG_BRW_FIRST_CHILD, &avpch2, NULL);
                d_assert(ret == 0, return EINVAL,);
                while(avpch2)
                {
                    ret = fd_msg_avp_hdr(avpch2, &hdr);
                    d_assert(ret == 0, return EINVAL,);
                    switch(hdr->avp_code)
                    {
                        case RX_AVP_CODE_MEDIA_COMPONENT_NUMBER:
                        {
                            media_component->media_component_number =
                                hdr->avp_value->i32;
                            break;
                        }
                        case RX_AVP_CODE_MEDIA_TYPE:
                        {
                            media_component->media_type = hdr->avp_value->i32;
                            break;
                        }
                        case RX_AVP_CODE_MAX_REQUESTED_BANDWIDTH_DL:
                        {
                            media_component->max_requested_bandwidth_dl =
                                hdr->avp_value->i32;
                            break;
                        }
                        case RX_AVP_CODE_MAX_REQUESTED_BANDWIDTH_UL:
                        {
                            media_component->max_requested_bandwidth_ul =
                                hdr->avp_value->i32;
                            break;
                        }
                        case RX_AVP_CODE_RR_BANDWIDTH:
                        {
                            media_component->rr_bandwidth = hdr->avp_value->i32;
                            break;
                        }
                        case RX_AVP_CODE_RS_BANDWIDTH:
                        {
                            media_component->rs_bandwidth = hdr->avp_value->i32;
                            break;
                        }
                        case RX_AVP_CODE_MIN_REQUESTED_BANDWIDTH_DL:
                        {
                            media_component->min_requested_bandwidth_dl =
                                hdr->avp_value->i32;
                            break;
                        }
                        case RX_AVP_CODE_MIN_REQUESTED_BANDWIDTH_UL:
                        {
                            media_component->min_requested_bandwidth_ul =
                                hdr->avp_value->i32;
                            break;
                        }
                        case RX_AVP_CODE_MEDIA_SUB_COMPONENT:
                        {
                            rx_media_sub_component_t *sub = &media_component->
                                sub[media_component->num_of_sub];

                            ret = fd_msg_browse(avpch2, MSG_BRW_FIRST_CHILD,
                                    &avpch3, NULL);
                            d_assert(ret == 0, return EINVAL,);
                            while(avpch3)
                            {
                                ret = fd_msg_avp_hdr(avpch3, &hdr);
                                d_assert(ret == 0, return EINVAL,);
                                switch(hdr->avp_code)
                                {
                                    case RX_AVP_CODE_FLOW_NUMBER:
                                        sub->flow_number =
                                            hdr->avp_value->i32;
                                        break;
                                    case RX_AVP_CODE_FLOW_USAGE:
                                    {
                                        sub->flow_usage =
                                            hdr->avp_value->i32;
                                        break;
                                    }
                                    case RX_AVP_CODE_FLOW_DESCRIPTION:
                                    {
                                        flow_t *flow = &sub->flow
                                            [sub->num_of_flow];

                                        flow->description = core_malloc(
                                                hdr->avp_value->os.len+1);
                                        core_cpystrn(
                                            flow->description,
                                            (char*)hdr->avp_value->os.data,
                                            hdr->avp_value->os.len+1);

                                        sub->num_of_flow++;
                                        break;
                                    }
                                    default:
                                    {
                                        d_error("Not supported(%d)",
                                                hdr->avp_code);
                                        break;
                                    }
                                }
                                fd_msg_browse(avpch3, MSG_BRW_NEXT, &avpch3, NULL);
                            }

                            media_component->num_of_sub++;
                            break;
                        }
                        default:
                        {
                            d_warn("Not supported(%d)", hdr->avp_code);
                            break;
                        }
                    }

                    fd_msg_browse(avpch2, MSG_BRW_NEXT, &avpch2, NULL);
                }

                rx_message.num_of_media_component++;
                break;
            }
            default:
            {
                d_warn("Not supported(%d)", hdr->avp_code);
                break;
            }
        }
        fd_msg_browse(avpch1, MSG_BRW_NEXT, &avpch1, NULL);
    }

    /* Send Re-Auth Request */
    rv = pcrf_gx_send_rar(gx_sid, sess_data->rx_sid, &rx_message);
    if (rv != CORE_OK)
    {
        result_code = rx_message.result_code;
        if (result_code != ER_DIAMETER_SUCCESS)
        {
            d_error("pcrf_gx_send_rar() failed");
            goto out;
        }
    }

    /* Store Gx Session-Id in this session */
    if (!sess_data->gx_sid)
        sess_data->gx_sid = (os0_t)core_strdup((char *)gx_sid);
    d_assert(sess_data->gx_sid, goto out,);

    /* Set IP-Can-Type */
    ret = fd_msg_avp_new(rx_ip_can_type, 0, &avp);
    d_assert(ret == 0, return EINVAL,);
    val.i32 = RX_IP_CAN_TYPE_3GPP_EPS;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return EINVAL,);

    /* Set RAT-Type */
    ret = fd_msg_avp_new(rx_rat_type, 0, &avp);
    d_assert(ret == 0, return EINVAL,);
    val.i32 = RX_RAT_TYPE_EUTRAN;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return EINVAL,);

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	ret = fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1);
    d_assert(ret == 0, return EINVAL,);

    /* Store this value in the session */
    ret = fd_sess_state_store(pcrf_rx_reg, sess, &sess_data);
    d_assert(ret == 0,,);
    d_assert(sess_data == NULL,,);

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    d_assert(ret == 0,,);

    d_trace(3, "[PCRF] AA-Answer\n");

	/* Add this value to the stats */
	d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,,);
	fd_logger_self()->stats.nb_echoed++;
	d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,,);

    rx_message_free(&rx_message);
    
    return 0;

out:
    if (result_code == FD_DIAMETER_AVP_UNSUPPORTED)
    {
        ret = fd_msg_rescode_set(ans,
                    "DIAMETER_AVP_UNSUPPORTED", NULL, NULL, 1);
        d_assert(ret == 0, return EINVAL,);
    }
    else if (result_code == FD_DIAMETER_UNKNOWN_SESSION_ID)
    {
        ret = fd_msg_rescode_set(ans,
                    "DIAMETER_UNKNOWN_SESSION_ID", NULL, NULL, 1);
        d_assert(ret == 0, return EINVAL,);
    }
    else if (result_code == FD_DIAMETER_MISSING_AVP)
    {
        ret = fd_msg_rescode_set(ans,
                    "DIAMETER_MISSING_AVP", NULL, NULL, 1);
        d_assert(ret == 0, return EINVAL,);
    }
    else
    {
        ret = fd_message_experimental_rescode_set(ans, result_code);
        d_assert(ret == 0, return EINVAL,);
    }
    
	ret = fd_msg_send(msg, NULL, NULL);
    d_assert(ret == 0,,);

    state_cleanup(sess_data, NULL, NULL);
    rx_message_free(&rx_message);

    return 0;
}

status_t pcrf_rx_send_asr(c_uint8_t *rx_sid, c_uint32_t abort_cause)
{
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    int new;
    size_t sidlen;

    d_assert(rx_sid, return CORE_ERROR,);

    d_trace(3, "[PCRF] Abort-Session-Request\n");

    /* Create the request */
    ret = fd_msg_new(rx_cmd_asr, MSGFL_ALLOC_ETEID, &req);
    d_assert(ret == 0, return CORE_ERROR,);
    {
        struct msg_hdr * h;
        ret = fd_msg_hdr( req, &h );
        d_assert(ret == 0, return CORE_ERROR,);
        h->msg_appl = RX_APPLICATION_ID;
    }

    /* Retrieve session by Session-Id */
    sidlen = strlen((char *)rx_sid);
    ret = fd_sess_fromsid_msg((os0_t)(rx_sid), sidlen, &session, &new);
    d_assert(ret == 0, return CORE_ERROR,);
    d_assert(new == 0, return CORE_ERROR,);

    /* Add Session-Id to the message */
    ret = fd_message_session_id_set(req, (os0_t)(rx_sid), sidlen);
    d_assert(ret == 0, return CORE_ERROR,);

    /* Save the session associated with the message */
    ret = fd_msg_sess_set(req, session);

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(pcrf_rx_reg, session, &sess_data);
    d_assert(sess_data, return CORE_ERROR,);

    /* Update State */
    sess_data->state = SESSION_ABORTED;
    sess_data->abort_cause = abort_cause;
    
    /* Set Origin-Host & Origin-Realm */
    ret = fd_msg_add_origin(req, 0);
    d_assert(ret == 0, return CORE_ERROR,);
    
    /* Set the Destination-Realm AVP */
    ret = fd_msg_avp_new(fd_destination_realm, 0, &avp);
    d_assert(ret == 0, return CORE_ERROR,);
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return CORE_ERROR,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return CORE_ERROR,);

    /* Set the Destination-Host AVP */
    ret = fd_msg_avp_new(fd_destination_host, 0, &avp);
    d_assert(ret == 0, return CORE_ERROR,);
    val.os.data = sess_data->peer_host;
    val.os.len  = strlen((char *)sess_data->peer_host);
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return CORE_ERROR,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return CORE_ERROR,);

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(fd_auth_application_id, 0, &avp);
    d_assert(ret == 0, return CORE_ERROR,);
    val.i32 = RX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return CORE_ERROR,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return CORE_ERROR,);

    /* Set the Abort-Cause AVP */
    ret = fd_msg_avp_new(rx_abort_cause, 0, &avp);
    d_assert(ret == 0, return CORE_ERROR,);
    val.i32 = sess_data->abort_cause;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return CORE_ERROR,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return CORE_ERROR,);

    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = sess_data;
    
    /* Store this value in the session */
    ret = fd_sess_state_store(pcrf_rx_reg, session, &sess_data);
    d_assert(ret == 0, return CORE_ERROR,);
    d_assert(sess_data == NULL,,);
    
    /* Send the request */
    ret = fd_msg_send(&req, pcrf_rx_asa_cb, svg);
    d_assert(ret == 0,,);

    /* Increment the counter */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,,);
    fd_logger_self()->stats.nb_sent++;
    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,,);

    return CORE_OK;
}

static void pcrf_rx_asa_cb(void *data, struct msg **msg)
{
    int ret;

    struct session *session;
    struct avp *avp, *avpch1;
    struct avp_hdr *hdr;
    int new;
    c_int32_t result_code = 0;

    d_trace(3, "[PCRF] Abort-Session-Answer\n");

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    d_assert(ret == 0, return,);
    d_assert(new == 0, return, );
    
    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, fd_result_code, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        result_code = hdr->avp_value->i32;
        d_trace(5, "    Result Code: %d\n", hdr->avp_value->i32);
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
                d_trace(5, "    Experimental Result Code: %d\n",
                        result_code);
            }
        }
        else
        {
            d_error("no Result-Code");
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, fd_origin_host, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        d_trace(5, "    From '%.*s'\n",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Host ");
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, fd_origin_realm, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        d_trace(5, "         ('%.*s')\n",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Realm ");
    }

    if (result_code != ER_DIAMETER_SUCCESS)
    {
        d_error("ERROR DIAMETER Result Code(%d)", result_code);
    }

    ret = fd_msg_free(*msg);
    d_assert(ret == 0,,);
    *msg = NULL;

    return;
}

static int pcrf_rx_str_cb( struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
    status_t rv;
    int ret;

	struct msg *ans, *qry;
    struct avp_hdr *hdr;
    union avp_value val;
    struct sess_state *sess_data = NULL;

    rx_message_t rx_message;

    c_uint32_t result_code = RX_DIAMETER_IP_CAN_SESSION_NOT_AVAILABLE;

    d_trace(3, "[PCRF] Session-Termination-Request\n");
	
    d_assert(msg, return EINVAL,);
    d_assert(sess, return EINVAL,);

    ret = fd_sess_state_retrieve(pcrf_rx_reg, sess, &sess_data);
    d_assert(ret == 0, return EINVAL,);
    d_assert(sess_data, return EINVAL,);
    d_assert(sess_data->rx_sid, return EINVAL,);
    d_assert(sess_data->gx_sid, return EINVAL,);

    /* Initialize Message */
    memset(&rx_message, 0, sizeof(rx_message_t));
    rx_message.cmd_code = RX_CMD_CODE_SESSION_TERMINATION;

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

    /* Get Termination-Cause */
    ret = fd_msg_search_avp(qry, rx_termination_cause, &avp);
    d_assert(ret == 0, return EINVAL,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return EINVAL,);
        sess_data->termination_cause = hdr->avp_value->i32;
        switch(sess_data->termination_cause)
        {
            case RX_TERMINATION_CAUSE_DIAMETER_LOGOUT:
                break;
            default:
                d_error("Termination-Cause Error : [%d]",
                        sess_data->termination_cause);
                break;
        }
    }
    else
    {
        d_error("no_Termination-Cause");
    }

    if (sess_data->state != SESSION_ABORTED)
    {
        /* Send Re-Auth Request if Abort-Session-Request is not initaited */
        rv = pcrf_gx_send_rar(sess_data->gx_sid, sess_data->rx_sid, &rx_message);
        if (rv != CORE_OK)
        {
            result_code = rx_message.result_code;
            d_error("pcrf_gx_send_rar() failed");
            goto out;
        }
    }

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	ret = fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1);
    d_assert(ret == 0, return EINVAL,);

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    d_assert(ret == 0,,);

    d_trace(3, "[PCRF] Session-Termination-Answer\n");

	/* Add this value to the stats */
	d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,,);
	fd_logger_self()->stats.nb_echoed++;
	d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,,);

    state_cleanup(sess_data, NULL, NULL);
    rx_message_free(&rx_message);
    
    return 0;

out:
    if (result_code == FD_DIAMETER_AVP_UNSUPPORTED)
    {
        ret = fd_msg_rescode_set(ans,
                    "DIAMETER_AVP_UNSUPPORTED", NULL, NULL, 1);
        d_assert(ret == 0, return EINVAL,);
    }
    else if (result_code == FD_DIAMETER_UNKNOWN_SESSION_ID)
    {
        ret = fd_msg_rescode_set(ans,
                    "DIAMETER_UNKNOWN_SESSION_ID", NULL, NULL, 1);
        d_assert(ret == 0, return EINVAL,);
    }
    else if (result_code == FD_DIAMETER_MISSING_AVP)
    {
        ret = fd_msg_rescode_set(ans,
                    "DIAMETER_MISSING_AVP", NULL, NULL, 1);
        d_assert(ret == 0, return EINVAL,);
    }
    else
    {
        ret = fd_msg_rescode_set(ans,
                    "DIAMETER_MISSING_AVP", NULL, NULL, 1);
        d_assert(ret == 0, return EINVAL,);
    }
    
	ret = fd_msg_send(msg, NULL, NULL);
    d_assert(ret == 0,,);
    d_trace(3, "[PCRF] Session-Termination-Answer\n");

    state_cleanup(sess_data, NULL, NULL);
    rx_message_free(&rx_message);

    return 0;
}

status_t pcrf_rx_init(void)
{
    int ret;
	struct disp_when data;

    pool_init(&pcrf_rx_sess_pool, MAX_POOL_OF_DIAMETER_SESS);

	/* Install objects definitions for this application */
	ret = rx_dict_init();
    d_assert(ret == 0, return CORE_ERROR,);

    /* Create handler for sessions */
	ret = fd_sess_handler_create(&pcrf_rx_reg, state_cleanup, NULL, NULL);
    d_assert(ret == 0, return CORE_ERROR,);

	/* Fallback CB if command != unexpected message received */
	memset(&data, 0, sizeof(data));
	data.app = rx_application;
	
	ret = fd_disp_register(pcrf_rx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_rx_fb);
    d_assert(ret == 0, return CORE_ERROR,);
	
	/* Specific handler for AA-Request */
	data.command = rx_cmd_aar;
	ret = fd_disp_register(pcrf_rx_aar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_rx_aar);
    d_assert(ret == 0, return CORE_ERROR,);

	/* Specific handler for STR-Request */
	data.command = rx_cmd_str;
	ret = fd_disp_register(pcrf_rx_str_cb, DISP_HOW_CC, &data, NULL,
                &hdl_rx_str);
    d_assert(ret == 0, return CORE_ERROR,);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(rx_application, fd_vendor, 1, 0);
    d_assert(ret == 0, return CORE_ERROR,);

	return CORE_OK;
}

void pcrf_rx_final(void)
{
    int ret;

	ret = fd_sess_handler_destroy(&pcrf_rx_reg, NULL);
    d_assert(ret == 0,,);

	if (hdl_rx_fb)
		(void) fd_disp_unregister(&hdl_rx_fb, NULL);
	if (hdl_rx_aar)
		(void) fd_disp_unregister(&hdl_rx_aar, NULL);
	if (hdl_rx_str)
		(void) fd_disp_unregister(&hdl_rx_str, NULL);

    if (pool_used(&pcrf_rx_sess_pool))
        d_error("%d not freed in pcrf_rx_sess_pool[%d] of GX-SM",
                pool_used(&pcrf_rx_sess_pool), pool_size(&pcrf_rx_sess_pool));
    d_trace(9, "%d not freed in pcrf_rx_sess_pool[%d] of GX-SM\n",
            pool_used(&pcrf_rx_sess_pool), pool_size(&pcrf_rx_sess_pool));

    pool_final(&pcrf_rx_sess_pool);
}
