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
    gtp_xact_t *xact;
    pgw_sess_t *sess;
    pkbuf_t *gtpbuf;
    c_uint32_t cc_request_type;
    c_uint32_t cc_request_number;
    struct timespec ts; /* Time of sending the message */
};

pool_declare(pgw_gx_sess_pool, struct sess_state, MAX_POOL_OF_DIAMETER_SESS);

static void pgw_gx_cca_cb(void *data, struct msg **msg);

void state_cleanup(
        struct sess_state *sess_data, os0_t sid, void * opaque)
{
    pool_free_node(&pgw_gx_sess_pool, sess_data);
}

void pgw_gx_send_ccr(gtp_xact_t *xact, pgw_sess_t *sess,
        pkbuf_t *gtpbuf, c_uint32_t cc_request_type)
{
    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch1, *avpch2;
    union avp_value val;
    struct sess_state *mi = NULL, *svg;
    struct session *session = NULL;
    int new;
    gtp_message_t *message = NULL;
    paa_t paa; /* For changing Framed-IPv6-Prefix Length to 128 */

    d_assert(sess, return,);
    d_assert(sess->ipv4 || sess->ipv6, return,);
    d_assert(gtpbuf, return, );
    message = gtpbuf->payload;
    d_assert(message, return, );

    /* Create the request */
    CHECK_FCT_DO( fd_msg_new(gx_cmd_ccr, MSGFL_ALLOC_ETEID, &req), goto out );

    if (sess->gx_sid)
    {
        /* Retrieve session by Session-Id */
        size_t sidlen = strlen(sess->gx_sid);
		CHECK_FCT_DO( fd_sess_fromsid_msg((os0_t)sess->gx_sid, sidlen,
                    &session, &new), goto out );
        d_assert(new == 0, return,);

        /* Add Session-Id to the message */
        CHECK_FCT_DO( fd_message_session_id_set(
                    req, (os0_t)sess->gx_sid, sidlen), goto out );
        /* Save the session associated with the message */
        CHECK_FCT_DO( fd_msg_sess_set(req, session), goto out );
    }
    else
    {
        size_t sidlen;
        /* Create a new session */
        #define GX_APP_SID_OPT  "app_gx"
        CHECK_FCT_DO( fd_msg_new_session(req, (os0_t)GX_APP_SID_OPT, 
                CONSTSTRLEN(GX_APP_SID_OPT)), goto out );
        CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL),
                goto out );

        /* Store Session-Id in PGW Session Context */
        CHECK_FCT_DO( fd_sess_getsid(session, (os0_t *)&sess->gx_sid, &sidlen),
                goto out );
        d_assert(sidlen == strlen(sess->gx_sid), return,);
    }

    /* Retrieve session state in this session */
    CHECK_FCT_DO( fd_sess_state_retrieve(pgw_gx_reg, session, &mi),
            goto out );
    if (!mi)
    {
        /* Allocate new session state memory */
        pool_alloc_node(&pgw_gx_sess_pool, &mi);
        d_assert(mi, return, "malloc failed: %s", strerror(errno));
        memset(mi, 0, sizeof *mi);
    }

    /* Update session state */
    mi->xact = xact;
    mi->sess = sess;
    mi->gtpbuf = gtpbuf;

    mi->cc_request_type = cc_request_type;
    if (cc_request_type == GX_CC_REQUEST_TYPE_INITIAL_REQUEST ||
        cc_request_type == GX_CC_REQUEST_TYPE_EVENT_REQUEST)
        mi->cc_request_number = 0;
    else
        mi->cc_request_number++;

    /* Set Origin-Host & Origin-Realm */
    CHECK_FCT_DO( fd_msg_add_origin(req, 0), goto out );
    
    /* Set the Destination-Realm AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_destination_realm, 0, &avp), goto out );
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len  = strlen(fd_g_config->cnf_diamrlm);
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set the Auth-Application-Id AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_application_id, 0, &avp), goto out );
    val.i32 = GX_APPLICATION_ID;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set CC-Request-Type, CC-Request-Number */
    CHECK_FCT_DO( fd_msg_avp_new(gx_cc_request_type, 0, &avp), goto out );
    val.i32 = mi->cc_request_type;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    CHECK_FCT_DO( fd_msg_avp_new(gx_cc_request_number, 0, &avp), goto out );
    val.i32 = mi->cc_request_number;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set Subscription-Id */
    CHECK_FCT_DO( fd_msg_avp_new(gx_subscription_id, 0, &avp),
            goto out );

    CHECK_FCT_DO( fd_msg_avp_new(gx_subscription_id_type, 0, &avpch1),
            goto out );
    val.i32 = GX_SUBSCRIPTION_ID_TYPE_END_USER_IMSI;
    CHECK_FCT_DO( fd_msg_avp_setvalue (avpch1, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1), goto out );

    CHECK_FCT_DO( fd_msg_avp_new(gx_subscription_id_data, 0, &avpch1),
            goto out );
    val.os.data = (c_uint8_t *)sess->imsi_bcd;
    val.os.len  = strlen(sess->imsi_bcd);
    CHECK_FCT_DO( fd_msg_avp_setvalue (avpch1, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1), goto out );

    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    if (cc_request_type != GX_CC_REQUEST_TYPE_TERMINATION_REQUEST)
    {
        /* Set Supported-Features */
        CHECK_FCT_DO( fd_msg_avp_new(gx_supported_features, 0, &avp),
                goto out );

        CHECK_FCT_DO( fd_msg_avp_new(gx_feature_list_id, 0, &avpch1),
                goto out );
        val.i32 = 1;
        CHECK_FCT_DO( fd_msg_avp_setvalue (avpch1, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1),
                goto out );

        CHECK_FCT_DO( fd_msg_avp_new(gx_feature_list, 0, &avpch1),
                goto out );
        val.u32 = 0x0000000b;
        CHECK_FCT_DO( fd_msg_avp_setvalue (avpch1, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1),
                goto out );

        CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp),
                goto out );

        /* Set Network-Request-Support */
        CHECK_FCT_DO( fd_msg_avp_new(gx_network_request_support, 0, &avp),
                goto out );
        val.i32 = 1;
        CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

        /* Set Framed-IP-Address */
        if (sess->ipv4)
        {
            CHECK_FCT_DO( fd_msg_avp_new(gx_framed_ip_address, 0, &avp),
                    goto out );
            val.os.data = (c_uint8_t*)&sess->ipv4->addr;
            val.os.len = IPV4_LEN;
            CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
            CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp),
                    goto out );
        }

        /* Set Framed-IPv6-Prefix */
        if (sess->ipv6)
        {
            CHECK_FCT_DO( fd_msg_avp_new(gx_framed_ipv6_prefix, 0, &avp),
                    goto out );
            memcpy(&paa, &sess->pdn.paa, PAA_IPV6_LEN);
#define FRAMED_IPV6_PREFIX_LENGTH 128  /* from spec document */
            paa.len = FRAMED_IPV6_PREFIX_LENGTH; 
            val.os.data = (c_uint8_t*)&paa;
            val.os.len = PAA_IPV6_LEN;
            CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
            CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp),
                    goto out );
        }

        /* Set IP-Can-Type */
        CHECK_FCT_DO( fd_msg_avp_new(gx_ip_can_type, 0, &avp),
                goto out );
        val.i32 = GX_IP_CAN_TYPE_3GPP_EPS;
        CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

        /* Set RAT-Type */
        CHECK_FCT_DO( fd_msg_avp_new(gx_rat_type, 0, &avp),
                goto out );
        val.i32 = GX_RAT_TYPE_EUTRAN;
        CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

        /* Set QoS-Information */
        if (sess->pdn.ambr.downlink || sess->pdn.ambr.uplink)
        {
            CHECK_FCT_DO( fd_msg_avp_new(gx_qos_information, 0, &avp),
                    goto out );

            if (sess->pdn.ambr.uplink)
            {
                CHECK_FCT_DO( fd_msg_avp_new(gx_apn_aggregate_max_bitrate_ul,
                        0, &avpch1), goto out );
                val.u32 = sess->pdn.ambr.uplink;
                CHECK_FCT_DO( fd_msg_avp_setvalue (avpch1, &val), goto out );
                CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1),
                        goto out );
            }
            
            if (sess->pdn.ambr.downlink)
            {
                CHECK_FCT_DO( fd_msg_avp_new(gx_apn_aggregate_max_bitrate_dl, 0,
                        &avpch1), goto out );
                val.u32 = sess->pdn.ambr.downlink;
                CHECK_FCT_DO( fd_msg_avp_setvalue (avpch1, &val), goto out );
                CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1),
                        goto out );
            }

            CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp),
                    goto out );
        }

        /* Set Default-EPS-Bearer-QoS */
        CHECK_FCT_DO( fd_msg_avp_new(gx_default_eps_bearer_qos, 0, &avp),
                goto out );

        CHECK_FCT_DO( fd_msg_avp_new(gx_qos_class_identifier, 0, &avpch1),
                goto out );
        val.u32 = sess->pdn.qos.qci;
        CHECK_FCT_DO( fd_msg_avp_setvalue (avpch1, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1),
                goto out );

        CHECK_FCT_DO( fd_msg_avp_new(gx_allocation_retention_priority, 0,
                &avpch1), goto out );

        CHECK_FCT_DO( fd_msg_avp_new(gx_priority_level, 0, &avpch2), goto out );
        val.u32 = sess->pdn.qos.arp.priority_level;
        CHECK_FCT_DO( fd_msg_avp_setvalue (avpch2, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2),
                goto out );

        CHECK_FCT_DO( fd_msg_avp_new(gx_pre_emption_capability, 0, &avpch2),
                goto out );
        val.u32 = sess->pdn.qos.arp.pre_emption_capability;
        CHECK_FCT_DO( fd_msg_avp_setvalue (avpch2, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2),
                goto out );

        CHECK_FCT_DO( fd_msg_avp_new(gx_pre_emption_vulnerability, 0, &avpch2),
                goto out );
        val.u32 = sess->pdn.qos.arp.pre_emption_vulnerability;
        CHECK_FCT_DO( fd_msg_avp_setvalue (avpch2, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2),
                goto out );

        CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1),
                goto out );

        CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

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

            CHECK_FCT_DO( fd_msg_avp_new(gx_3gpp_user_location_info, 0, &avp),
                    goto out );
            val.os.data = (c_uint8_t*)&gx_uli;
            val.os.len = sizeof(gx_uli);
            CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
            CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp),
                    goto out );
        }

        /* Set 3GPP-MS-Timezone */
        if (message->create_session_request.ue_time_zone.presence)
        {
            CHECK_FCT_DO( fd_msg_avp_new(gx_3gpp_ms_timezone, 0, &avp),
                    goto out );
            val.os.data = message->create_session_request.ue_time_zone.data;
            val.os.len = message->create_session_request.ue_time_zone.len;
            CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
            CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp),
                    goto out );
        }
    }

    /* Set Called-Station-Id */
    CHECK_FCT_DO( fd_msg_avp_new(gx_called_station_id, 0, &avp),
            goto out );
    val.os.data = (c_uint8_t*)sess->pdn.apn;
    val.os.len = strlen(sess->pdn.apn);
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );
    
    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &mi->ts), goto out );
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = mi;
    
    /* Store this value in the session */
    CHECK_FCT_DO( fd_sess_state_store(pgw_gx_reg, session, &mi), goto out );
    d_assert(mi == NULL,,);
    
    /* Send the request */
    CHECK_FCT_DO( fd_msg_send(&req, pgw_gx_cca_cb, svg), goto out );

    /* Increment the counter */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
    fd_logger_self()->stats.nb_sent++;
    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

    d_trace(3, "[Gx] Credit-Control-Request : PGW[%d] --> PCRF\n", 
            sess->pgw_s5c_teid);

out:
    return;
}

static void pgw_gx_cca_cb(void *data, struct msg **msg)
{
    struct sess_state *mi = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch1, *avpch2, *avpch3, *avpch4;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;

    event_t e;
    gtp_xact_t *xact = NULL;
    pgw_sess_t *sess = NULL;
    pkbuf_t *gxbuf = NULL, *gtpbuf = NULL;
    gx_message_t *gx_message = NULL;
    gx_cca_message_t *cca_message = NULL;
    c_uint16_t gxbuf_len = 0;
    
    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), return );

    /* Search the session, retrieve its data */
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new),
            return );
    d_assert(new == 0, return, );
    
    CHECK_FCT_DO( fd_sess_state_retrieve(pgw_gx_reg, session, &mi), return );
    d_assert(mi && (void *)mi == data, return, );

    xact = mi->xact;
    d_assert(xact, return, "Null param");
    sess = mi->sess;
    d_assert(sess, return, "Null param");
    gtpbuf = mi->gtpbuf;
    d_assert(gtpbuf, return, "Null param");

    gxbuf_len = sizeof(gx_message_t);
    d_assert(gxbuf_len < 8192, return, "Not supported size:%d", gxbuf_len);
    gxbuf = pkbuf_alloc(0, gxbuf_len);
    d_assert(gxbuf, return, "Null param");
    gx_message = gxbuf->payload;
    d_assert(gx_message, return, "Null param");

    d_trace(3, "[Gx] Credit-Control-Answer : PGW <-- PCRF\n");

    /* Set Credit Control Command */
    memset(gx_message, 0, gxbuf_len);
    gx_message->cmd_code = GX_CMD_CODE_CREDIT_CONTROL;
    cca_message = &gx_message->cca_message;
    
    /* Value of Result Code */
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_result_code, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return);
        gx_message->result_code = hdr->avp_value->i32;
        d_trace(3, "Result Code: %d\n", hdr->avp_value->i32);
    }
    else
    {
        CHECK_FCT_DO( fd_msg_search_avp(*msg, 
                fd_experimental_result, &avp), return );
        if (avp)
        {
            CHECK_FCT_DO( fd_avp_search_avp(avp, 
                    fd_experimental_result_code, &avpch1), return );
            if (avpch1)
            {
                CHECK_FCT_DO( fd_msg_avp_hdr(avpch1, &hdr), return);
                gx_message->result_code = hdr->avp_value->i32;
                d_trace(3, "Experimental Result Code: %d\n",
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
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_origin_host, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return );
        d_trace(3, "From '%.*s' ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Host ");
        error++;
    }

    /* Value of Origin-Realm */
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_origin_realm, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return );
        d_trace(3, "('%.*s') ",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    }
    else
    {
        d_error("no_Origin-Realm ");
        error++;
    }

    if (gx_message->result_code != ER_DIAMETER_SUCCESS)
    {
        d_warn("ERROR DIAMETER Result Code(%d)", gx_message->result_code);
        error++;
        goto out;
    }

    CHECK_FCT_DO(
        fd_msg_search_avp(*msg, gx_cc_request_type, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return );
        cca_message->cc_request_type = hdr->avp_value->i32;
    }
    else
        error++;

    CHECK_FCT_DO(
        fd_msg_search_avp(*msg, gx_charging_rule_install, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_browse(avp, MSG_BRW_FIRST_CHILD, &avpch1, NULL),
                return );
        while(avpch1)
        {
            CHECK_FCT_DO( fd_msg_avp_hdr(avpch1, &hdr), return );
            switch(hdr->avp_code)
            {
                case GX_AVP_CODE_CHARGING_RULE_DEFINITION:
                {
                    pcc_rule_t *pcc_rule = 
                        &cca_message->pcc_rule[cca_message->num_of_pcc_rule];

                    CHECK_FCT_DO( fd_msg_browse(avpch1,
                            MSG_BRW_FIRST_CHILD, &avpch2, NULL), return );
                    while(avpch2)
                    {

                        CHECK_FCT_DO( fd_msg_avp_hdr(avpch2, &hdr), return );
                        switch(hdr->avp_code)
                        {
                            case GX_AVP_CODE_CHARGING_RULE_NAME:
                            {
                                core_cpystrn(pcc_rule->name,
                                    (char*)hdr->avp_value->os.data,
                                    c_min(hdr->avp_value->os.len,
                                        MAX_PCC_RULE_NAME_LEN)+1);
                                break;
                            }
                            case GX_AVP_CODE_FLOW_INFORMATION:
                            {
                                flow_t *flow =
                                    &pcc_rule->flow[pcc_rule->num_of_flow];

                                CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                                        gx_flow_direction, &avpch3), return );
                                if (avpch3)
                                {
                                    CHECK_FCT_DO( fd_msg_avp_hdr(
                                                avpch3, &hdr), return );
                                    flow->direction = hdr->avp_value->i32;
                                }

                                CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                                        gx_flow_description, &avpch3), return );
                                if (avpch3)
                                {
                                    CHECK_FCT_DO( fd_msg_avp_hdr(
                                            avpch3, &hdr), return );
                                    flow->description =
                                        core_malloc(hdr->avp_value->os.len+1);
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
                                CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                                    gx_qos_class_identifier, &avpch3), return );
                                if (avpch3)
                                {
                                    CHECK_FCT_DO( fd_msg_avp_hdr(
                                            avpch3, &hdr), return );
                                    pcc_rule->qos.qci = hdr->avp_value->u32;
                                }
                                else
                                    error++;

                                CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                                    gx_allocation_retention_priority,
                                    &avpch3), return );
                                if (avpch3)
                                {
                                    CHECK_FCT_DO( fd_avp_search_avp(avpch3,
                                        gx_priority_level, &avpch4), return );
                                    if (avpch4)
                                    {
                                        CHECK_FCT_DO( fd_msg_avp_hdr(avpch4,
                                            &hdr), return );
                                        pcc_rule->qos.arp.priority_level =
                                            hdr->avp_value->u32;
                                    }
                                    else
                                        error++;

                                    CHECK_FCT_DO( fd_avp_search_avp(avpch3,
                                        gx_pre_emption_capability, &avpch4),
                                        return );
                                    if (avpch4)
                                    {
                                        CHECK_FCT_DO( fd_msg_avp_hdr(
                                                avpch4, &hdr), return );
                                        pcc_rule->qos.arp.
                                            pre_emption_capability =
                                                hdr->avp_value->u32;
                                    }
                                    else
                                        error++;

                                    CHECK_FCT_DO( fd_avp_search_avp(avpch3,
                                            gx_pre_emption_vulnerability,
                                            &avpch4), return );
                                    if (avpch4)
                                    {
                                        CHECK_FCT_DO( fd_msg_avp_hdr(avpch4,
                                                &hdr), return );
                                        pcc_rule->qos.arp.
                                            pre_emption_vulnerability =
                                                hdr->avp_value->u32;
                                    }
                                    else
                                        error++;
                                }
                                else
                                    error++;

                                CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                                        gx_max_requested_bandwidth_ul,
                                        &avpch3), return );
                                if (avpch3)
                                {
                                    CHECK_FCT_DO( fd_msg_avp_hdr(
                                            avpch3, &hdr), return );
                                    pcc_rule->qos.mbr.uplink =
                                        hdr->avp_value->u32;
                                }
                                CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                                    gx_max_requested_bandwidth_dl,
                                    &avpch3), return );
                                if (avpch3)
                                {
                                    CHECK_FCT_DO( fd_msg_avp_hdr(
                                        avpch3, &hdr), return );
                                    pcc_rule->qos.mbr.downlink =
                                        hdr->avp_value->u32;
                                }
                                CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                                        gx_guaranteed_bitrate_ul, &avpch3),
                                        return );
                                if (avpch3)
                                {
                                    CHECK_FCT_DO( fd_msg_avp_hdr(
                                        avpch3, &hdr), return );
                                    pcc_rule->qos.gbr.uplink =
                                        hdr->avp_value->u32;
                                }
                                CHECK_FCT_DO( fd_avp_search_avp(avpch2,
                                    gx_guaranteed_bitrate_dl, &avpch3),
                                    return );
                                if (avpch3)
                                {
                                    CHECK_FCT_DO( fd_msg_avp_hdr(avpch3,
                                        &hdr), return );
                                    pcc_rule->qos.gbr.downlink =
                                        hdr->avp_value->u32;
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

                    cca_message->num_of_pcc_rule++;

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
    }

    CHECK_FCT_DO(fd_msg_search_avp(*msg, gx_qos_information, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_avp_search_avp(avp,
                gx_apn_aggregate_max_bitrate_ul, &avpch1), return );
        if (avpch1)
        {
            CHECK_FCT_DO( fd_msg_avp_hdr(avpch1, &hdr), return );
            cca_message->pdn.ambr.uplink = hdr->avp_value->u32;
        }
        CHECK_FCT_DO( fd_avp_search_avp(avp,
                gx_apn_aggregate_max_bitrate_dl, &avpch1), return );
        if (avpch1)
        {
            CHECK_FCT_DO( fd_msg_avp_hdr(avpch1, &hdr), return );
            cca_message->pdn.ambr.downlink = hdr->avp_value->u32;
        }
    }

    CHECK_FCT_DO(fd_msg_search_avp(*msg, gx_default_eps_bearer_qos, &avp),
            return );
    if (avp)
    {
        CHECK_FCT_DO( fd_avp_search_avp(avp,
                    gx_qos_class_identifier, &avpch1), return );
        if (avpch1)
        {
            CHECK_FCT_DO( fd_msg_avp_hdr(avpch1, &hdr), return );
            cca_message->pdn.qos.qci = hdr->avp_value->u32;
        }

        CHECK_FCT_DO( fd_avp_search_avp(avp,
                    gx_allocation_retention_priority, &avpch1), return );
        if (avpch1)
        {
            CHECK_FCT_DO( fd_avp_search_avp(avpch1,
                        gx_priority_level, &avpch4), return );
            if (avpch4)
            {
                CHECK_FCT_DO( fd_msg_avp_hdr(avpch4, &hdr), return );
                cca_message->pdn.qos.arp.priority_level = hdr->avp_value->u32;
            }

            CHECK_FCT_DO( fd_avp_search_avp(avpch1,
                        gx_pre_emption_capability, &avpch4), return );
            if (avpch4)
            {
                CHECK_FCT_DO( fd_msg_avp_hdr(avpch4, &hdr), return );
                cca_message->pdn.qos.arp.pre_emption_capability =
                    hdr->avp_value->u32;
            }

            CHECK_FCT_DO( fd_avp_search_avp(avpch1,
                        gx_pre_emption_vulnerability, &avpch4), return );
            if (avpch4)
            {
                CHECK_FCT_DO( fd_msg_avp_hdr(avpch4, &hdr), return );
                cca_message->pdn.qos.arp.pre_emption_vulnerability =
                    hdr->avp_value->u32;
            }
        }
    }

out:
    event_set(&e, PGW_EVT_GX_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)xact->index);
    event_set_param2(&e, (c_uintptr_t)sess->index);
    event_set_param3(&e, (c_uintptr_t)gxbuf);
    event_set_param4(&e, (c_uintptr_t)gtpbuf);
    pgw_event_send(&e);

    /* Free the message */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
    dur = ((ts.tv_sec - mi->ts.tv_sec) * 1000000) + 
        ((ts.tv_nsec - mi->ts.tv_nsec) / 1000);
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

    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );
    
    /* Display how long it took */
    if (ts.tv_nsec > mi->ts.tv_nsec)
        d_trace(3, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec - mi->ts.tv_sec),
                (long)(ts.tv_nsec - mi->ts.tv_nsec) / 1000);
    else
        d_trace(3, "in %d.%06ld sec\n", 
                (int)(ts.tv_sec + 1 - mi->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - mi->ts.tv_nsec) / 1000);

    if (mi->cc_request_type != GX_CC_REQUEST_TYPE_TERMINATION_REQUEST)
    {
        CHECK_FCT_DO( fd_sess_state_store(pgw_gx_reg, session, &mi), goto out );
        d_assert(mi == NULL,,);
    }
    else
    {
        state_cleanup(mi, NULL, NULL);
    }

    CHECK_FCT_DO( fd_msg_free(*msg), return );
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
        struct session *sess, void *opaque, enum disp_action *act)
{
#if 0
    status_t rv;
#endif

	struct msg *ans, *qry;
#if 0
    struct avp *avpch1, *avpch2, *avpch3, *avpch4;
    struct avp_hdr *hdr;
#endif
    union avp_value val;
    struct sess_state *mi = NULL;

    c_uint32_t result_code = GX_DIAMETER_ERROR_USER_UNKNOWN;
	
    d_assert(msg, return EINVAL,);

    d_assert( fd_sess_state_retrieve(pgw_gx_reg, sess, &mi) == 0,
            return EINVAL,);
    if (mi)
    {
    }

	/* Create answer header */
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0) );
    ans = *msg;

    /* Set the Auth-Application-Id AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_application_id, 0, &avp), goto out );
    val.i32 = GX_APPLICATION_ID;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) );

    /* Store this value in the session */
    CHECK_FCT_DO( fd_sess_state_store(pgw_gx_reg, sess, &mi),
            goto out );
    d_assert(mi == NULL,,);

	/* Send the answer */
	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );

	/* Add this value to the stats */
	CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
	fd_logger_self()->stats.nb_echoed++;
	CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

    return 0;

out:
    if (result_code == GX_DIAMETER_ERROR_USER_UNKNOWN)
    {
        CHECK_FCT( fd_msg_rescode_set(ans,
                    "DIAMETER_ERROR_USER_UNKNOWN", NULL, NULL, 1) );
    }
    else
    {
        CHECK_FCT( fd_message_experimental_rescode_set(ans, result_code) );
    }

	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );

    state_cleanup(mi, NULL, NULL);

    return 0;
}

int pgw_fd_init(void)
{
	struct disp_when data;

    pool_init(&pgw_gx_sess_pool, MAX_POOL_OF_DIAMETER_SESS);

    CHECK_FCT( fd_init(FD_MODE_CLIENT|FD_MODE_SERVER,
                pgw_self()->fd_conf_path, pgw_self()->fd_config) );

	/* Install objects definitions for this application */
	CHECK_FCT( gx_dict_init() );

    /* Create handler for sessions */
	CHECK_FCT( fd_sess_handler_create(&pgw_gx_reg, state_cleanup,
                NULL, NULL) );

	memset(&data, 0, sizeof(data));
	data.app = gx_application;

	CHECK_FCT( fd_disp_register(pgw_gx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_gx_fb) );

	data.command = gx_cmd_rar;
	CHECK_FCT( fd_disp_register(pgw_gx_rar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_gx_rar) );

	/* Advertise the support for the application in the peer */
	CHECK_FCT( fd_disp_app_support(gx_application, fd_vendor, 1, 0) );

	return 0;
}

void pgw_fd_final(void)
{
	CHECK_FCT_DO( fd_sess_handler_destroy(&pgw_gx_reg, NULL), );
	if (hdl_gx_fb)
		(void) fd_disp_unregister(&hdl_gx_fb, NULL);
	if (hdl_gx_rar)
		(void) fd_disp_unregister(&hdl_gx_rar, NULL);

    fd_final();

    if (pool_used(&pgw_gx_sess_pool))
        d_error("%d not freed in pgw_gx_sess_pool[%d] of GX-SM",
                pool_used(&pgw_gx_sess_pool), pool_size(&pgw_gx_sess_pool));
    d_trace(3, "%d not freed in pgw_gx_sess_pool[%d] of GX-SM\n",
            pool_used(&pgw_gx_sess_pool), pool_size(&pgw_gx_sess_pool));

    pool_final(&pgw_gx_sess_pool);
}
