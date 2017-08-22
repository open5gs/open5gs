#define TRACE_MODULE _pgw_fd_path

#include "core_debug.h"
#include "core_pool.h"

#include "fd_lib.h"
#include "gx_lib.h"

#include "pgw_event.h"
#include "pgw_fd_path.h"

#define MAX_NUM_SESSION_STATE 32

static struct session_handler *pgw_gx_reg = NULL;

struct sess_state {
    gtp_xact_t *xact;
    pgw_sess_t *sess;
    struct timespec ts; /* Time of sending the message */
};

pool_declare(pgw_gx_sess_pool, struct sess_state, MAX_NUM_SESSION_STATE);

static void pgw_gx_cca_cb(void *data, struct msg **msg);

void pgw_gx_send_ccr(
    gtp_xact_t *xact, pgw_sess_t *sess, c_uint32_t cc_request_type)
{
    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch1, *avpch2;
    union avp_value val;
    struct sess_state *mi = NULL, *svg;
    struct session *session = NULL;

    d_assert(sess, return, "Null param");
    d_assert(sess->ip_pool, return, "Null param");

    /* Create the random value to store with the session */
    pool_alloc_node(&pgw_gx_sess_pool, &mi);
    d_assert(mi, return, "malloc failed: %s", strerror(errno));
    
    mi->xact = xact;
    mi->sess = sess;
    
    /* Create the request */
    CHECK_FCT_DO( fd_msg_new(gx_cmd_ccr, MSGFL_ALLOC_ETEID, &req), goto out );
    
    /* Create a new session */
    #define GX_APP_SID_OPT  "app_gx"
    CHECK_FCT_DO( fd_msg_new_session(req, (os0_t)GX_APP_SID_OPT, 
            CONSTSTRLEN(GX_APP_SID_OPT)), goto out );
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL),
            goto out );

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
    val.i32 = cc_request_type;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    CHECK_FCT_DO( fd_msg_avp_new(gx_cc_request_number, 0, &avp), goto out );
    val.i32 = 0;
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

    /* Set Supported-Features */
    CHECK_FCT_DO( fd_msg_avp_new(gx_supported_features, 0, &avp),
            goto out );

    CHECK_FCT_DO( fd_msg_avp_new(gx_feature_list_id, 0, &avpch1),
            goto out );
    val.i32 = 1;
    CHECK_FCT_DO( fd_msg_avp_setvalue (avpch1, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1), goto out );

    CHECK_FCT_DO( fd_msg_avp_new(gx_feature_list, 0, &avpch1),
            goto out );
    val.u32 = 0x0000000b;
    CHECK_FCT_DO( fd_msg_avp_setvalue (avpch1, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1), goto out );

    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set Network-Request-Support */
    CHECK_FCT_DO( fd_msg_avp_new(gx_network_request_support, 0, &avp),
            goto out );
    val.i32 = 1;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set Framed-IP-Address */
    CHECK_FCT_DO( fd_msg_avp_new(gx_framed_ip_address, 0, &avp),
            goto out );
    val.os.data = (c_uint8_t*)&sess->ip_pool->ue_addr;
    val.os.len = sizeof(sess->ip_pool->ue_addr);
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );

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
            CHECK_FCT_DO( fd_msg_avp_new(gx_apn_aggregate_max_bitrate_ul, 0,
                    &avpch1), goto out );
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

        CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );
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

    CHECK_FCT_DO( fd_msg_avp_new(gx_allocation_retention_priority, 0, &avpch1),
            goto out );

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
        CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );
    }

    /* Set 3GPP-MS-Timezone */
    {
        gtp_ue_timezone_t ue_timezone;
        memset(&ue_timezone, 0, sizeof(ue_timezone));
        ue_timezone.timezone = 0x40;
        ue_timezone.daylight_saving_time = 
            GTP_UE_TIME_ZONE_NO_ADJUSTMENT_FOR_DAYLIGHT_SAVING_TIME;

        CHECK_FCT_DO( fd_msg_avp_new(gx_3gpp_ms_timezone, 0, &avp),
                goto out );
        val.os.data = (c_uint8_t*)&ue_timezone;
        val.os.len = sizeof(ue_timezone);
        CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
        CHECK_FCT_DO( fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp), goto out );
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
    
    /* Send the request */
    CHECK_FCT_DO( fd_msg_send(&req, pgw_gx_cca_cb, svg), goto out );

    /* Increment the counter */
    CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
    fd_logger_self()->stats.nb_sent++;
    CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

    d_trace(3, "[Gx] Credit-Control-Request : PGW[%d] --> PCRF\n", 
            sess->pgw_s5c_teid);

out:
    pool_free_node(&pgw_gx_sess_pool, mi);
    return;
}

static void pgw_gx_cca_cb(void *data, struct msg **msg)
{
    struct sess_state *mi = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch1;
#if 0
    struct avp *avp_e_utran_vector, *avp_xres, *avp_kasme, *avp_rand, *avp_autn;
#endif
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    c_uint32_t result_code = 0;
    c_uint32_t cc_request_type = 0;
    int new;

    event_t e;
    gtp_xact_t *xact = NULL;
    pgw_sess_t *sess = NULL;
    
    CHECK_SYS_DO( clock_gettime(CLOCK_REALTIME, &ts), return );

    /* Search the session, retrieve its data */
    CHECK_FCT_DO( fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new),
            return );
    d_assert(new == 0, return, );
    
    CHECK_FCT_DO( fd_sess_state_retrieve(pgw_gx_reg, session, &mi), return );
    d_assert(mi && (void *)mi == data, return, );

    xact = mi->xact;
    d_assert(xact, return, );
    sess = mi->sess;
    d_assert(sess, return, );

    d_trace(3, "[Gx] Credit-Control-Answer : PGW[%d] <-- PCRF\n", 
            sess->pgw_s5c_teid);
    
    /* Value of Result Code */
    CHECK_FCT_DO( fd_msg_search_avp(*msg, fd_result_code, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return);
        result_code = hdr->avp_value->i32;
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
                result_code = hdr->avp_value->i32;
                d_trace(3, "Experimental Result Code: %d\n", result_code);
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

    if (result_code != ER_DIAMETER_SUCCESS)
    {
        d_warn("ERROR DIAMETER Result Code(%d)", result_code);
        error++;
        goto out;
    }

    CHECK_FCT_DO(
        fd_msg_search_avp(*msg, gx_cc_request_type, &avp), return );
    if (avp)
    {
        CHECK_FCT_DO( fd_msg_avp_hdr(avp, &hdr), return );
        cc_request_type = hdr->avp_value->i32;
    }
    else
        error++;

out:
    event_set(&e, PGW_EVT_S5C_SESSION_FROM_GX);
    event_set_param1(&e, (c_uintptr_t)xact->index);
    event_set_param2(&e, (c_uintptr_t)sess->index);
    event_set_param3(&e, (c_uintptr_t)GX_CMD_CREDIT_CONTROL);
    event_set_param4(&e, (c_uintptr_t)cc_request_type);
    event_set_param5(&e, (c_uintptr_t)result_code);
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
    
    CHECK_FCT_DO( fd_msg_free(*msg), return );
    *msg = NULL;

    pool_free_node(&pgw_gx_sess_pool, mi);

    return;
}

int pgw_fd_init(void)
{
    pool_init(&pgw_gx_sess_pool, MAX_NUM_SESSION_STATE);

    CHECK_FCT( fd_init(FD_MODE_CLIENT, pgw_self()->fd_conf_path) );

	/* Install objects definitions for this application */
	CHECK_FCT( gx_dict_init() );

	CHECK_FCT( fd_sess_handler_create(&pgw_gx_reg, (void *)free, NULL, NULL) );

	/* Advertise the support for the application in the peer */
	CHECK_FCT( fd_disp_app_support(gx_application, fd_vendor, 1, 0) );
	
	return 0;
}

void pgw_fd_final(void)
{
	CHECK_FCT_DO( fd_sess_handler_destroy(&pgw_gx_reg, NULL), );

    fd_final();

    if (pool_size(&pgw_gx_sess_pool) != pool_avail(&pgw_gx_sess_pool))
        d_error("%d not freed in pgw_gx_sess_pool[%d] of S6A-SM",
                pool_size(&pgw_gx_sess_pool) - pool_avail(&pgw_gx_sess_pool),
                pool_size(&pgw_gx_sess_pool));

    d_trace(3, "%d not freed in pgw_gx_sess_pool[%d] of S6A-SM\n",
            pool_size(&pgw_gx_sess_pool) - pool_avail(&pgw_gx_sess_pool),
            pool_size(&pgw_gx_sess_pool));

    pool_final(&pgw_gx_sess_pool);
}
