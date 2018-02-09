#define TRACE_MODULE _pgw_fd_path

#include "core_debug.h"
#include "core_pool.h"
#include "core_lib.h"

#include "gtp/gtp_xact.h"

#include "fd/fd_lib.h"
#include "fd/gx/gx_dict.h"
#include "fd/gx/gx_message.h"

#include "pgw_event.h"
#include "pgw_fd_path.h"

static struct session_handler *pgw_gx_reg = NULL;
static struct disp_hdl *hdl_gx_fb = NULL; 
static struct disp_hdl *hdl_gx_rar = NULL; 

struct sess_state {
    os0_t       gx_sid;             /* Gx Session-Id */

    pgw_sess_t *sess;
    gtp_xact_t *xact;
    pkbuf_t *gtpbuf;

    c_uint32_t cc_request_type;
    c_uint32_t cc_request_number;

    struct timespec ts; /* Time of sending the message */
};

pool_declare(pgw_gx_sess_pool, struct sess_state, MAX_POOL_OF_DIAMETER_SESS);

static status_t decode_pcc_rule_definition(
        pcc_rule_t *pcc_rule, struct avp *avpch1, int *perror);
static void pgw_gx_cca_cb(void *data, struct msg **msg);

static __inline__ struct sess_state *new_state(os0_t sid)
{
    struct sess_state *new = NULL;

    pool_alloc_node(&pgw_gx_sess_pool, &new);
    d_assert(new, return NULL,);
    memset(new, 0, sizeof *new);

    new->gx_sid = (os0_t)core_strdup((char *)sid);
    d_assert(new->gx_sid, return NULL,);

    return new;
}

static void state_cleanup(struct sess_state *sess_data, os0_t sid, void *opaque)
{
    if (sess_data->gx_sid)
        CORE_FREE(sess_data->gx_sid);

    pool_free_node(&pgw_gx_sess_pool, sess_data);
}

void pgw_gx_send_ccr(pgw_sess_t *sess, gtp_xact_t *xact,
        pkbuf_t *gtpbuf, c_uint32_t cc_request_type)
{
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch1, *avpch2;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    int new;
    gtp_message_t *message = NULL;
    paa_t paa; /* For changing Framed-IPv6-Prefix Length to 128 */

    d_assert(sess, return,);
    d_assert(sess->ipv4 || sess->ipv6, return,);
    d_assert(gtpbuf, return, );
    message = gtpbuf->payload;
    d_assert(message, return, );

    d_trace(3, "[PGW] Credit-Control-Request\n");

    /* Create the request */
    ret = fd_msg_new(gx_cmd_ccr, MSGFL_ALLOC_ETEID, &req);
    d_assert(ret == 0, return,);

    /* Find Diameter Gx Session */
    if (sess->gx_sid)
    {
        /* Retrieve session by Session-Id */
        size_t sidlen = strlen(sess->gx_sid);
		ret = fd_sess_fromsid_msg((os0_t)sess->gx_sid, sidlen, &session, &new);
        d_assert(ret == 0, return,);
        d_assert(new == 0, return,);

        /* Add Session-Id to the message */
        ret = fd_message_session_id_set(req, (os0_t)sess->gx_sid, sidlen);
        d_assert(ret == 0, return,);
        /* Save the session associated with the message */
        ret = fd_msg_sess_set(req, session);
    }
    else
    {
        /* Create a new session */
        #define GX_APP_SID_OPT  "app_gx"
        ret = fd_msg_new_session(req, (os0_t)GX_APP_SID_OPT, 
                CONSTSTRLEN(GX_APP_SID_OPT));
        d_assert(ret == 0, return,);
        ret = fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL);
        d_assert(ret == 0, return,);
    }

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(pgw_gx_reg, session, &sess_data);
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
        sess->gx_sid = (c_int8_t *)sess_data->gx_sid;
    }

    /* Update session state */
    sess_data->sess = sess;
    sess_data->xact = xact;
    sess_data->gtpbuf = gtpbuf;

    sess_data->cc_request_type = cc_request_type;
    if (cc_request_type == GX_CC_REQUEST_TYPE_INITIAL_REQUEST ||
        cc_request_type == GX_CC_REQUEST_TYPE_EVENT_REQUEST)
        sess_data->cc_request_number = 0;
    else
        sess_data->cc_request_number++;

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
    val.i32 = GX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set CC-Request-Type, CC-Request-Number */
    ret = fd_msg_avp_new(gx_cc_request_type, 0, &avp);
    d_assert(ret == 0, return,);
    val.i32 = sess_data->cc_request_type;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    ret = fd_msg_avp_new(gx_cc_request_number, 0, &avp);
    d_assert(ret == 0, return,);
    val.i32 = sess_data->cc_request_number;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    /* Set Subscription-Id */
    ret = fd_msg_avp_new(gx_subscription_id, 0, &avp);
    d_assert(ret == 0, return,);

    ret = fd_msg_avp_new(gx_subscription_id_type, 0, &avpch1);
    d_assert(ret == 0, return,);
    val.i32 = GX_SUBSCRIPTION_ID_TYPE_END_USER_IMSI;
    ret = fd_msg_avp_setvalue (avpch1, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    d_assert(ret == 0, return,);

    ret = fd_msg_avp_new(gx_subscription_id_data, 0, &avpch1);
    d_assert(ret == 0, return,);
    val.os.data = (c_uint8_t *)sess->imsi_bcd;
    val.os.len  = strlen(sess->imsi_bcd);
    ret = fd_msg_avp_setvalue (avpch1, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    d_assert(ret == 0, return,);

    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    if (cc_request_type != GX_CC_REQUEST_TYPE_TERMINATION_REQUEST)
    {
        /* Set Supported-Features */
        ret = fd_msg_avp_new(gx_supported_features, 0, &avp);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(gx_feature_list_id, 0, &avpch1);
        d_assert(ret == 0, return,);
        val.i32 = 1;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(gx_feature_list, 0, &avpch1);
        d_assert(ret == 0, return,);
        val.u32 = 0x0000000b;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        d_assert(ret == 0, return,);

        /* Set Network-Request-Support */
        ret = fd_msg_avp_new(gx_network_request_support, 0, &avp);
        d_assert(ret == 0, return,);
        val.i32 = 1;
        ret = fd_msg_avp_setvalue(avp, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        d_assert(ret == 0, return,);

        /* Set Framed-IP-Address */
        if (sess->ipv4)
        {
            ret = fd_msg_avp_new(gx_framed_ip_address, 0, &avp);
            d_assert(ret == 0, return,);
            val.os.data = (c_uint8_t*)&sess->ipv4->addr;
            val.os.len = IPV4_LEN;
            ret = fd_msg_avp_setvalue(avp, &val);
            d_assert(ret == 0, return,);
            ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
            d_assert(ret == 0, return,);
        }

        /* Set Framed-IPv6-Prefix */
        if (sess->ipv6)
        {
            ret = fd_msg_avp_new(gx_framed_ipv6_prefix, 0, &avp);
            d_assert(ret == 0, return,);
            memcpy(&paa, &sess->pdn.paa, PAA_IPV6_LEN);
#define FRAMED_IPV6_PREFIX_LENGTH 128  /* from spec document */
            paa.len = FRAMED_IPV6_PREFIX_LENGTH; 
            val.os.data = (c_uint8_t*)&paa;
            val.os.len = PAA_IPV6_LEN;
            ret = fd_msg_avp_setvalue(avp, &val);
            d_assert(ret == 0, return,);
            ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
            d_assert(ret == 0, return,);
        }

        /* Set IP-Can-Type */
        ret = fd_msg_avp_new(gx_ip_can_type, 0, &avp);
        d_assert(ret == 0, return,);
        val.i32 = GX_IP_CAN_TYPE_3GPP_EPS;
        ret = fd_msg_avp_setvalue(avp, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        d_assert(ret == 0, return,);

        /* Set RAT-Type */
        ret = fd_msg_avp_new(gx_rat_type, 0, &avp);
        d_assert(ret == 0, return,);
        val.i32 = GX_RAT_TYPE_EUTRAN;
        ret = fd_msg_avp_setvalue(avp, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        d_assert(ret == 0, return,);

        /* Set QoS-Information */
        if (sess->pdn.ambr.downlink || sess->pdn.ambr.uplink)
        {
            ret = fd_msg_avp_new(gx_qos_information, 0, &avp);
            d_assert(ret == 0, return,);

            if (sess->pdn.ambr.uplink)
            {
                ret = fd_msg_avp_new(gx_apn_aggregate_max_bitrate_ul,
                        0, &avpch1);
                d_assert(ret == 0, return,);
                val.u32 = sess->pdn.ambr.uplink;
                ret = fd_msg_avp_setvalue (avpch1, &val);
                d_assert(ret == 0, return,);
                ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
                d_assert(ret == 0, return,);
            }
            
            if (sess->pdn.ambr.downlink)
            {
                ret = fd_msg_avp_new(gx_apn_aggregate_max_bitrate_dl, 0,
                        &avpch1);
                d_assert(ret == 0, return,);
                val.u32 = sess->pdn.ambr.downlink;
                ret = fd_msg_avp_setvalue (avpch1, &val);
                d_assert(ret == 0, return,);
                ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
                d_assert(ret == 0, return,);
            }

            ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
            d_assert(ret == 0, return,);
        }

        /* Set Default-EPS-Bearer-QoS */
        ret = fd_msg_avp_new(gx_default_eps_bearer_qos, 0, &avp);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(gx_qos_class_identifier, 0, &avpch1);
        d_assert(ret == 0, return,);
        val.u32 = sess->pdn.qos.qci;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(gx_allocation_retention_priority, 0, &avpch1);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(gx_priority_level, 0, &avpch2);
        d_assert(ret == 0, return,);
        val.u32 = sess->pdn.qos.arp.priority_level;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(gx_pre_emption_capability, 0, &avpch2);
        d_assert(ret == 0, return,);
        val.u32 = sess->pdn.qos.arp.pre_emption_capability;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_new(gx_pre_emption_vulnerability, 0, &avpch2);
        d_assert(ret == 0, return,);
        val.u32 = sess->pdn.qos.arp.pre_emption_vulnerability;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return,);

        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        d_assert(ret == 0, return,);

        /* Set 3GPP-User-Location-Info */
        {
            struct gx_uli_t {
                c_uint8_t type;
                tai_t tai;
                e_cgi_t e_cgi;
            } gx_uli;

            memset(&gx_uli, 0, sizeof(gx_uli));
            gx_uli.type = GX_3GPP_USER_LOCATION_INFO_TYPE_TAI_AND_ECGI;
            memcpy(&gx_uli.tai.plmn_id, &sess->tai.plmn_id, 
                    sizeof(sess->tai.plmn_id));
            gx_uli.tai.tac = htons(sess->tai.tac);
            memcpy(&gx_uli.e_cgi.plmn_id, &sess->e_cgi.plmn_id, 
                    sizeof(sess->e_cgi.plmn_id));
            gx_uli.e_cgi.cell_id = htonl(sess->e_cgi.cell_id);

            ret = fd_msg_avp_new(gx_3gpp_user_location_info, 0, &avp);
            d_assert(ret == 0, return,);
            val.os.data = (c_uint8_t*)&gx_uli;
            val.os.len = sizeof(gx_uli);
            ret = fd_msg_avp_setvalue(avp, &val);
            d_assert(ret == 0, return,);
            ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
            d_assert(ret == 0, return,);
        }

        /* Set 3GPP-MS-Timezone */
        if (message->create_session_request.ue_time_zone.presence)
        {
            ret = fd_msg_avp_new(gx_3gpp_ms_timezone, 0, &avp);
            d_assert(ret == 0, return,);
            val.os.data = message->create_session_request.ue_time_zone.data;
            val.os.len = message->create_session_request.ue_time_zone.len;
            ret = fd_msg_avp_setvalue(avp, &val);
            d_assert(ret == 0, return,);
            ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
            d_assert(ret == 0, return,);
        }
    }

    /* Set Called-Station-Id */
    ret = fd_msg_avp_new(gx_called_station_id, 0, &avp);
    d_assert(ret == 0, return,);
    val.os.data = (c_uint8_t*)sess->pdn.apn;
    val.os.len = strlen(sess->pdn.apn);
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);
    
    ret = clock_gettime(CLOCK_REALTIME, &sess_data->ts);
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = sess_data;
    
    /* Store this value in the session */
    ret = fd_sess_state_store(pgw_gx_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data == NULL,,);
    
    /* Send the request */
    ret = fd_msg_send(&req, pgw_gx_cca_cb, svg);
    d_assert(ret == 0,,);

    /* Increment the counter */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,,);
    fd_logger_self()->stats.nb_sent++;
    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,,);
}

static void pgw_gx_cca_cb(void *data, struct msg **msg)
{
    status_t rv;
    int ret;

    struct sess_state *sess_data = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch1, *avpch2;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;

    event_t e;
    gtp_xact_t *xact = NULL;
    pgw_sess_t *sess = NULL;
    pkbuf_t *gxbuf = NULL, *gtpbuf = NULL;
    gx_message_t *gx_message = NULL;
    c_uint16_t gxbuf_len = 0;

    d_trace(3, "[PGW] Credit-Control-Answer\n");
    
    ret = clock_gettime(CLOCK_REALTIME, &ts);
    d_assert(ret == 0, return,);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    d_assert(ret == 0, return,);
    d_assert(new == 0, return, );
    
    ret = fd_sess_state_retrieve(pgw_gx_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data && (void *)sess_data == data, return, );

    xact = sess_data->xact;
    d_assert(xact, return, "Null param");
    sess = sess_data->sess;
    d_assert(sess, return, "Null param");
    gtpbuf = sess_data->gtpbuf;
    d_assert(gtpbuf, return, "Null param");

    gxbuf_len = sizeof(gx_message_t);
    d_assert(gxbuf_len < 8192, return, "Not supported size:%d", gxbuf_len);
    gxbuf = pkbuf_alloc(0, gxbuf_len);
    d_assert(gxbuf, return, "Null param");
    gx_message = gxbuf->payload;
    d_assert(gx_message, return, "Null param");

    /* Set Credit Control Command */
    memset(gx_message, 0, gxbuf_len);
    gx_message->cmd_code = GX_CMD_CODE_CREDIT_CONTROL;
    
    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, fd_result_code, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        gx_message->result_code = hdr->avp_value->i32;
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
                gx_message->result_code = hdr->avp_value->i32;
                d_trace(5, "    Experimental Result Code: %d\n",
                        gx_message->result_code);
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
        d_trace(5, "    From '%.*s'\n",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Host");
        error++;
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
        d_error("no_Origin-Realm");
        error++;
    }

    if (gx_message->result_code != ER_DIAMETER_SUCCESS)
    {
        d_warn("ERROR DIAMETER Result Code(%d)", gx_message->result_code);
        goto out;
    }

    ret = fd_msg_search_avp(*msg, gx_cc_request_type, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        gx_message->cc_request_type = hdr->avp_value->i32;
    }
    else
    {
        d_error("no_CC-Request-Type");
        error++;
    }

    ret = fd_msg_search_avp(*msg, gx_qos_information, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_avp_search_avp(avp, gx_apn_aggregate_max_bitrate_ul, &avpch1);
        d_assert(ret == 0, return,);
        if (avpch1)
        {
            ret = fd_msg_avp_hdr(avpch1, &hdr);
            d_assert(ret == 0, return,);
            gx_message->pdn.ambr.uplink = hdr->avp_value->u32;
        }
        ret = fd_avp_search_avp(avp, gx_apn_aggregate_max_bitrate_dl, &avpch1);
        d_assert(ret == 0, return,);
        if (avpch1)
        {
            ret = fd_msg_avp_hdr(avpch1, &hdr);
            d_assert(ret == 0, return,);
            gx_message->pdn.ambr.downlink = hdr->avp_value->u32;
        }
    }

    ret = fd_msg_search_avp(*msg, gx_default_eps_bearer_qos, &avp);
    d_assert(ret == 0, return,);
    if (avp)
    {
        ret = fd_avp_search_avp(avp, gx_qos_class_identifier, &avpch1);
        d_assert(ret == 0, return,);
        if (avpch1)
        {
            ret = fd_msg_avp_hdr(avpch1, &hdr);
            d_assert(ret == 0, return,);
            gx_message->pdn.qos.qci = hdr->avp_value->u32;
        }

        ret = fd_avp_search_avp(avp, gx_allocation_retention_priority, &avpch1);
        d_assert(ret == 0, return,);
        if (avpch1)
        {
            ret = fd_avp_search_avp(avpch1, gx_priority_level, &avpch2);
            d_assert(ret == 0, return,);
            if (avpch2)
            {
                ret = fd_msg_avp_hdr(avpch2, &hdr);
                d_assert(ret == 0, return,);
                gx_message->pdn.qos.arp.priority_level = hdr->avp_value->u32;
            }

            ret = fd_avp_search_avp(avpch1, gx_pre_emption_capability, &avpch2);
            d_assert(ret == 0, return,);
            if (avpch2)
            {
                ret = fd_msg_avp_hdr(avpch2, &hdr);
                d_assert(ret == 0, return,);
                gx_message->pdn.qos.arp.pre_emption_capability =
                    hdr->avp_value->u32;
            }

            ret = fd_avp_search_avp(avpch1,
                        gx_pre_emption_vulnerability, &avpch2);
            d_assert(ret == 0, return,);
            if (avpch2)
            {
                ret = fd_msg_avp_hdr(avpch2, &hdr);
                d_assert(ret == 0, return,);
                gx_message->pdn.qos.arp.pre_emption_vulnerability =
                    hdr->avp_value->u32;
            }
        }
    }

    ret = fd_msg_browse(*msg, MSG_BRW_FIRST_CHILD, &avp, NULL);
    d_assert(ret == 0, return,);
    while(avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return,);
        switch(hdr->avp_code)
        {
            case AC_SESSION_ID:
            case AC_ORIGIN_HOST:
            case AC_ORIGIN_REALM:
            case AC_DESTINATION_REALM:
            case AC_RESULT_CODE:
            case AC_ROUTE_RECORD:
            case AC_PROXY_INFO:
            case AC_AUTH_APPLICATION_ID:
                break;
            case GX_AVP_CODE_CC_REQUEST_TYPE:
            case GX_AVP_CODE_CC_REQUEST_NUMBER:
            case GX_AVP_CODE_SUPPORTED_FEATURES:
                break;
            case GX_AVP_CODE_QOS_INFORMATION:
            case GX_AVP_CODE_DEFAULT_EPS_BEARER_QOS:
                break;
            case GX_AVP_CODE_CHARGING_RULE_INSTALL:
            {
                ret = fd_msg_browse(avp, MSG_BRW_FIRST_CHILD, &avpch1, NULL);
                d_assert(ret == 0, return,);
                while(avpch1)
                {
                    ret = fd_msg_avp_hdr(avpch1, &hdr);
                    d_assert(ret == 0, return,);
                    switch(hdr->avp_code)
                    {
                        case GX_AVP_CODE_CHARGING_RULE_DEFINITION:
                        {
                            pcc_rule_t *pcc_rule = &gx_message->pcc_rule
                                [gx_message->num_of_pcc_rule];

                            rv = decode_pcc_rule_definition(
                                    pcc_rule, avpch1, &error);
                            d_assert(rv == CORE_OK, return,);

                            pcc_rule->type = PCC_RULE_TYPE_INSTALL;
                            gx_message->num_of_pcc_rule++;
                            break;
                        }
                        default:
                        {
                            d_error("Not supported(%d)", hdr->avp_code);
                            break;
                        }
                    }
                    fd_msg_browse(avpch1, MSG_BRW_NEXT, &avpch1, NULL);
                }
                break;
            }
            default:
            {
                d_warn("Not supported(%d)", hdr->avp_code);
                break;
            }
        }
        fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL);
    }

out:
    if (!error)
    {
        event_set(&e, PGW_EVT_GX_MESSAGE);
        event_set_param1(&e, (c_uintptr_t)sess->index);
        event_set_param2(&e, (c_uintptr_t)gxbuf);
        event_set_param3(&e, (c_uintptr_t)xact->index);
        event_set_param4(&e, (c_uintptr_t)gtpbuf);
        pgw_event_send(&e);
    }
    else
    {
        gx_message_free(gx_message);
        pkbuf_free(gxbuf);

        pkbuf_free(gtpbuf);
    }

    /* Free the message */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,,);
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

    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,,);
    
    /* Display how long it took */
    if (ts.tv_nsec > sess_data->ts.tv_nsec)
        d_trace(15, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        d_trace(15, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);

    if (sess_data->cc_request_type != GX_CC_REQUEST_TYPE_TERMINATION_REQUEST)
    {
        ret = fd_sess_state_store(pgw_gx_reg, session, &sess_data);
        d_assert(ret == 0, return,);
        d_assert(sess_data == NULL, return,);
    }
    else
    {
        state_cleanup(sess_data, NULL, NULL);
    }

    ret = fd_msg_free(*msg);
    d_assert(ret == 0,,);
    *msg = NULL;
    
    return;
}

static int pgw_gx_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

static int pgw_gx_rar_cb( struct msg **msg, struct avp *avp, 
        struct session *session, void *opaque, enum disp_action *act)
{
    status_t rv;
    int ret;

	struct msg *ans, *qry;
    struct avp *avpch1;
    struct avp_hdr *hdr;
    union avp_value val;
    struct sess_state *sess_data = NULL;

    event_t e;
    c_uint16_t gxbuf_len = 0;
    pkbuf_t *gxbuf = NULL;
    pgw_sess_t *sess = NULL;
    gx_message_t *gx_message = NULL;

    c_uint32_t result_code = FD_DIAMETER_UNKNOWN_SESSION_ID;
	
    d_assert(msg, return EINVAL,);

    d_trace(3, "[PGW] Re-Auth-Request\n");

    gxbuf_len = sizeof(gx_message_t);
    d_assert(gxbuf_len < 8192, return EINVAL,
            "Not supported size:%d", gxbuf_len);
    gxbuf = pkbuf_alloc(0, gxbuf_len);
    d_assert(gxbuf, return EINVAL, "Null param");
    gx_message = gxbuf->payload;
    d_assert(gx_message, return EINVAL, "Null param");

    /* Set Credit Control Command */
    memset(gx_message, 0, gxbuf_len);
    gx_message->cmd_code = GX_CMD_RE_AUTH;

	/* Create answer header */
	qry = *msg;
	ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    d_assert(ret == 0, return EINVAL,);
    ans = *msg;

    ret = fd_sess_state_retrieve(pgw_gx_reg, session, &sess_data);
    d_assert(ret == 0, return EINVAL,);
    if (!sess_data)
    {
        d_error("No Session Data");
        goto out;
    }

    /* Get Session Information */
    sess = sess_data->sess;
    d_assert(sess, return EINVAL,);

    ret = fd_msg_browse(qry, MSG_BRW_FIRST_CHILD, &avp, NULL);
    d_assert(ret == 0, return EINVAL,);
    while(avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return EINVAL,);
        switch(hdr->avp_code)
        {
            case AC_SESSION_ID:
            case AC_ORIGIN_HOST:
            case AC_ORIGIN_REALM:
            case AC_DESTINATION_REALM:
            case AC_DESTINATION_HOST:
            case AC_ROUTE_RECORD:
            case AC_PROXY_INFO:
            case AC_AUTH_APPLICATION_ID:
                break;
            case GX_AVP_CODE_RE_AUTH_REQUEST_TYPE:
                break;
            case GX_AVP_CODE_CHARGING_RULE_INSTALL:
            {
                ret = fd_msg_browse(avp, MSG_BRW_FIRST_CHILD, &avpch1, NULL);
                d_assert(ret == 0, return EINVAL,);
                while(avpch1)
                {
                    ret = fd_msg_avp_hdr(avpch1, &hdr);
                    d_assert(ret == 0, return EINVAL,);
                    switch(hdr->avp_code)
                    {
                        case GX_AVP_CODE_CHARGING_RULE_DEFINITION:
                        {
                            pcc_rule_t *pcc_rule = &gx_message->pcc_rule
                                [gx_message->num_of_pcc_rule];

                            rv = decode_pcc_rule_definition(
                                    pcc_rule, avpch1, NULL);
                            d_assert(rv == CORE_OK, return EINVAL,);

                            pcc_rule->type = PCC_RULE_TYPE_INSTALL;
                            gx_message->num_of_pcc_rule++;
                            break;
                        }
                        default:
                        {
                            d_error("Not supported(%d)", hdr->avp_code);
                            break;
                        }
                    }
                    fd_msg_browse(avpch1, MSG_BRW_NEXT, &avpch1, NULL);
                }
                break;
            }
            case GX_AVP_CODE_CHARGING_RULE_REMOVE:
            {
                ret = fd_msg_browse(avp, MSG_BRW_FIRST_CHILD, &avpch1, NULL);
                d_assert(ret == 0, return EINVAL,);
                while(avpch1)
                {
                    ret = fd_msg_avp_hdr(avpch1, &hdr);
                    d_assert(ret == 0, return EINVAL,);
                    switch(hdr->avp_code)
                    {
                        case GX_AVP_CODE_CHARGING_RULE_NAME:
                        {
                            pcc_rule_t *pcc_rule = &gx_message->pcc_rule
                                [gx_message->num_of_pcc_rule];

                            pcc_rule->name = 
                                core_strdup((char*)hdr->avp_value->os.data);
                            d_assert(pcc_rule->name, return CORE_ERROR,);

                            pcc_rule->type = PCC_RULE_TYPE_REMOVE;
                            gx_message->num_of_pcc_rule++;
                            break;
                        }
                        default:
                        {
                            d_error("Not supported(%d)", hdr->avp_code);
                            break;
                        }
                    }
                    fd_msg_browse(avpch1, MSG_BRW_NEXT, &avpch1, NULL);
                }
                break;
            }
            default:
            {
                d_warn("Not supported(%d)", hdr->avp_code);
                break;
            }
        }
        fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL);
    }

    /* Send Gx Event to PGW State Machine */
    event_set(&e, PGW_EVT_GX_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)sess->index);
    event_set_param2(&e, (c_uintptr_t)gxbuf);
    pgw_event_send(&e);

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(fd_auth_application_id, 0, &avp);
    d_assert(ret == 0, return EINVAL,);
    val.i32 = GX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return EINVAL,);

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	ret = fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1);
    d_assert(ret == 0, return EINVAL,);

    /* Store this value in the session */
    ret = fd_sess_state_store(pgw_gx_reg, session, &sess_data);
    d_assert(ret == 0, return EINVAL,);
    d_assert(sess_data == NULL,,);

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    d_assert(ret == 0,,);

    d_trace(3, "[PGW] Re-Auth-Answer\n");

	/* Add this value to the stats */
	d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,,);
	fd_logger_self()->stats.nb_echoed++;
	d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,,);

    return 0;

out:
    if (result_code == FD_DIAMETER_UNKNOWN_SESSION_ID)
    {
        ret = fd_msg_rescode_set(ans,
                    "DIAMETER_UNKNOWN_SESSION_ID", NULL, NULL, 1);
        d_assert(ret == 0, return EINVAL,);
    }
    else
    {
        ret = fd_message_experimental_rescode_set(ans, result_code);
        d_assert(ret == 0, return EINVAL,);
    }

    /* Store this value in the session */
    ret = fd_sess_state_store(pgw_gx_reg, session, &sess_data);
    d_assert(ret == 0, return EINVAL,);
    d_assert(sess_data == NULL,,);

	ret = fd_msg_send(msg, NULL, NULL);
    d_assert(ret == 0,,);

    gx_message_free(gx_message);
    pkbuf_free(gxbuf);

    return 0;
}

status_t pgw_fd_init(void)
{
    int ret;
	struct disp_when data;

    pool_init(&pgw_gx_sess_pool, MAX_POOL_OF_DIAMETER_SESS);

    ret = fd_init(FD_MODE_CLIENT|FD_MODE_SERVER,
                pgw_self()->fd_conf_path, pgw_self()->fd_config);
    d_assert(ret == 0, return CORE_ERROR,);

	/* Install objects definitions for this application */
	ret = gx_dict_init();
    d_assert(ret == 0, return CORE_ERROR,);

    /* Create handler for sessions */
	ret = fd_sess_handler_create(&pgw_gx_reg, state_cleanup, NULL, NULL);
    d_assert(ret == 0, return CORE_ERROR,);

	memset(&data, 0, sizeof(data));
	data.app = gx_application;

	ret = fd_disp_register(pgw_gx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_gx_fb);
    d_assert(ret == 0, return CORE_ERROR,);

	data.command = gx_cmd_rar;
	ret = fd_disp_register(pgw_gx_rar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_gx_rar);
    d_assert(ret == 0, return CORE_ERROR,);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(gx_application, fd_vendor, 1, 0);

	return CORE_OK;
}

void pgw_fd_final(void)
{
    int ret;

	ret = fd_sess_handler_destroy(&pgw_gx_reg, NULL);
    d_assert(ret == 0,,);

	if (hdl_gx_fb)
		(void) fd_disp_unregister(&hdl_gx_fb, NULL);
	if (hdl_gx_rar)
		(void) fd_disp_unregister(&hdl_gx_rar, NULL);

    fd_final();

    if (pool_used(&pgw_gx_sess_pool))
        d_error("%d not freed in pgw_gx_sess_pool[%d] of GX-SM",
                pool_used(&pgw_gx_sess_pool), pool_size(&pgw_gx_sess_pool));
    d_trace(9, "%d not freed in pgw_gx_sess_pool[%d] of GX-SM\n",
            pool_used(&pgw_gx_sess_pool), pool_size(&pgw_gx_sess_pool));

    pool_final(&pgw_gx_sess_pool);
}

static status_t decode_pcc_rule_definition(
        pcc_rule_t *pcc_rule, struct avp *avpch1, int *perror)
{
    int ret = 0, error = 0;
    struct avp *avpch2, *avpch3, *avpch4;
    struct avp_hdr *hdr;

    d_assert(pcc_rule, return CORE_ERROR,);
    d_assert(avpch1, return CORE_ERROR,);

    ret = fd_msg_browse(avpch1, MSG_BRW_FIRST_CHILD, &avpch2, NULL);
    d_assert(ret == 0, return CORE_ERROR,);
    while(avpch2)
    {
        ret = fd_msg_avp_hdr(avpch2, &hdr);
        d_assert(ret == 0, return CORE_ERROR,);
        switch(hdr->avp_code)
        {
            case GX_AVP_CODE_CHARGING_RULE_NAME:
            {
                if (pcc_rule->name)
                {
                    d_error("PCC Rule Name has already been defined");
                    CORE_FREE(pcc_rule->name);
                }
                pcc_rule->name = core_strdup((char*)hdr->avp_value->os.data);
                d_assert(pcc_rule->name, return CORE_ERROR,);
                break;
            }
            case GX_AVP_CODE_FLOW_INFORMATION:
            {
                flow_t *flow =
                    &pcc_rule->flow[pcc_rule->num_of_flow];

                ret = fd_avp_search_avp(avpch2, gx_flow_direction, &avpch3);
                d_assert(ret == 0, return CORE_ERROR,);
                if (avpch3)
                {
                    ret = fd_msg_avp_hdr( avpch3, &hdr);
                    d_assert(ret == 0, return CORE_ERROR,);
                    flow->direction = hdr->avp_value->i32;
                }

                ret = fd_avp_search_avp(avpch2, gx_flow_description, &avpch3);
                d_assert(ret == 0, return CORE_ERROR,);
                if (avpch3)
                {
                    ret = fd_msg_avp_hdr(avpch3, &hdr);
                    d_assert(ret == 0, return CORE_ERROR,);
                    flow->description = core_malloc(hdr->avp_value->os.len+1);
                    core_cpystrn(flow->description,
                        (char*)hdr->avp_value->os.data,
                        hdr->avp_value->os.len+1);
                }

                pcc_rule->num_of_flow++;
                break;
            }
            case GX_AVP_CODE_FLOW_STATUS:
            {
                pcc_rule->flow_status = hdr->avp_value->i32;
                break;
            }
            case GX_AVP_CODE_QOS_INFORMATION:
            {
                ret = fd_avp_search_avp(avpch2,
                    gx_qos_class_identifier, &avpch3);
                d_assert(ret == 0, return CORE_ERROR,);
                if (avpch3)
                {
                    ret = fd_msg_avp_hdr(avpch3, &hdr);
                    d_assert(ret == 0, return CORE_ERROR,);
                    pcc_rule->qos.qci = hdr->avp_value->u32;
                }
                else
                {
                    d_error("no_QCI");
                    error++;
                }

                ret = fd_avp_search_avp(avpch2,
                    gx_allocation_retention_priority, &avpch3);
                d_assert(ret == 0, return CORE_ERROR,);
                if (avpch3)
                {
                    ret = fd_avp_search_avp(avpch3, gx_priority_level, &avpch4);
                    d_assert(ret == 0, return CORE_ERROR,);
                    if (avpch4)
                    {
                        ret = fd_msg_avp_hdr(avpch4, &hdr);
                        d_assert(ret == 0, return CORE_ERROR,);
                        pcc_rule->qos.arp.priority_level = hdr->avp_value->u32;
                    }
                    else
                    {
                        d_error("no_Priority-Level");
                        error++;
                    }

                    ret = fd_avp_search_avp(avpch3,
                        gx_pre_emption_capability, &avpch4);
                    d_assert(ret == 0, return CORE_ERROR,);
                    if (avpch4)
                    {
                        ret = fd_msg_avp_hdr(avpch4, &hdr);
                        d_assert(ret == 0, return CORE_ERROR,);
                        pcc_rule->qos.arp.pre_emption_capability =
                                hdr->avp_value->u32;
                    }
                    else
                    {
                        d_error("no_Preemption-Capability");
                        error++;
                    }

                    ret = fd_avp_search_avp(avpch3,
                            gx_pre_emption_vulnerability, &avpch4);
                    d_assert(ret == 0, return CORE_ERROR,);
                    if (avpch4)
                    {
                        ret = fd_msg_avp_hdr(avpch4, &hdr);
                        d_assert(ret == 0, return CORE_ERROR,);
                        pcc_rule->qos.arp.pre_emption_vulnerability =
                                hdr->avp_value->u32;
                    }
                    else
                    {
                        d_error("no_Preemption-Vulnerability");
                        error++;
                    }
                }
                else
                {
                    d_error("no_ARP");
                    error++;
                }

                ret = fd_avp_search_avp(avpch2,
                        gx_max_requested_bandwidth_ul, &avpch3);
                d_assert(ret == 0, return CORE_ERROR,);
                if (avpch3)
                {
                    ret = fd_msg_avp_hdr(avpch3, &hdr);
                    d_assert(ret == 0, return CORE_ERROR,);
                    pcc_rule->qos.mbr.uplink = hdr->avp_value->u32;
                }
                ret = fd_avp_search_avp(avpch2,
                    gx_max_requested_bandwidth_dl, &avpch3);
                d_assert(ret == 0, return CORE_ERROR,);
                if (avpch3)
                {
                    ret = fd_msg_avp_hdr(avpch3, &hdr);
                    d_assert(ret == 0, return CORE_ERROR,);
                    pcc_rule->qos.mbr.downlink = hdr->avp_value->u32;
                }
                ret = fd_avp_search_avp(avpch2,
                        gx_guaranteed_bitrate_ul, &avpch3);
                d_assert(ret == 0, return CORE_ERROR,);
                if (avpch3)
                {
                    ret = fd_msg_avp_hdr(avpch3, &hdr);
                    d_assert(ret == 0, return CORE_ERROR,);
                    pcc_rule->qos.gbr.uplink = hdr->avp_value->u32;
                }
                ret = fd_avp_search_avp(avpch2,
                    gx_guaranteed_bitrate_dl, &avpch3);
                d_assert(ret == 0, return CORE_ERROR,);
                if (avpch3)
                {
                    ret = fd_msg_avp_hdr(avpch3, &hdr);
                    d_assert(ret == 0, return CORE_ERROR,);
                    pcc_rule->qos.gbr.downlink = hdr->avp_value->u32;
                }
                break;
            }
            case GX_AVP_CODE_PRECEDENCE:
            {
                pcc_rule->precedence = hdr->avp_value->i32;
                break;
            }
            default:
            {
                d_error("Not implemented(%d)", hdr->avp_code);
                break;
            }
        }
        fd_msg_browse(avpch2, MSG_BRW_NEXT, &avpch2, NULL);
    }

    if (perror)
        *perror = error;

    return CORE_OK;
}
