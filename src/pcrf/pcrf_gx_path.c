#define TRACE_MODULE _pcrf_gx_path

#include "core_lib.h"
#include "core_debug.h"
#include "core_pool.h"

#include "fd/fd_lib.h"
#include "fd/gx/gx_dict.h"
#include "fd/gx/gx_message.h"

#include "pcrf_context.h"

struct sess_state {
    c_uint32_t cc_request_type;
    struct timespec ts; /* Time of sending the message */
};

static struct session_handler *pcrf_gx_reg = NULL;
static struct disp_hdl *hdl_gx_fb = NULL; 
static struct disp_hdl *hdl_gx_ccr = NULL; 

pool_declare(pcrf_gx_sess_pool, struct sess_state, MAX_POOL_OF_DIAMETER_SESS);

void pcrf_gx_sess_cleanup(
        struct sess_state *sess_data, os0_t sid, void *opaque)
{
    printf("cleanup\n");
    pool_free_node(&pcrf_gx_sess_pool, sess_data);
}

static int pcrf_gx_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	d_warn("Unexpected message received!");
	
	return ENOTSUP;
}

static int pcrf_gx_ccr_cb( struct msg **msg, struct avp *avp, 
        struct session *sess, void *opaque, enum disp_action *act)
{
	struct msg *ans, *qry;
    struct avp *avpch1, *avpch2, *avpch3, *avpch4;
    struct avp_hdr *hdr;
    union avp_value val;
    struct sess_state *sess_data = NULL;

    status_t rv;
    gx_cca_message_t cca_message;
    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];
    c_int8_t apn[MAX_APN_LEN+1];
    int i, j;

    c_uint32_t cc_request_number = 0;
    c_uint32_t result_code = GX_DIAMETER_ERROR_USER_UNKNOWN;
	
    d_assert(msg, return EINVAL,);

    d_assert( fd_sess_state_retrieve(pcrf_gx_reg, sess, &sess_data) == 0,
            return EINVAL,);
    if (!sess_data)
    {
        pool_alloc_node(&pcrf_gx_sess_pool, &sess_data);
        d_assert(sess_data, return EINVAL,);
        memset(sess_data, 0, sizeof *sess_data);
    }

    /* Initialize Message */
    memset(&cca_message, 0, sizeof(gx_cca_message_t));

	/* Create answer header */
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0) );
    ans = *msg;

    /* Get CC-Request-Type */
    CHECK_FCT( fd_msg_search_avp(qry, gx_cc_request_type, &avp) );
    CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
    sess_data->cc_request_type = hdr->avp_value->i32;

    /* Get CC-Request-Number */
    CHECK_FCT( fd_msg_search_avp(qry, gx_cc_request_number, &avp) );
    CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
    cc_request_number = hdr->avp_value->i32;

    /* Set the Auth-Application-Id AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_application_id, 0, &avp), goto out );
    val.i32 = GX_APPLICATION_ID;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set CC-Request-Type */
    CHECK_FCT_DO( fd_msg_avp_new(gx_cc_request_type, 0, &avp), goto out );
    val.i32 = sess_data->cc_request_type;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set CC-Request-Number */
    CHECK_FCT_DO( fd_msg_avp_new(gx_cc_request_number, 0, &avp), goto out );
    val.i32 = cc_request_number;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Get IMSI + APN */
    CHECK_FCT( fd_msg_search_avp(qry, gx_subscription_id, &avp) );
    CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
    CHECK_FCT( fd_avp_search_avp(avp, gx_subscription_id_type, &avpch1) );
    CHECK_FCT( fd_msg_avp_hdr(avpch1, &hdr) );
    if (hdr->avp_value->i32 != GX_SUBSCRIPTION_ID_TYPE_END_USER_IMSI)
    {
        d_error("Not implemented Subscription-Id-Type(%d)",
                hdr->avp_value->i32);
        goto out;
    }
    CHECK_FCT( fd_avp_search_avp(avp, gx_subscription_id_data, &avpch1) );
    CHECK_FCT( fd_msg_avp_hdr(avpch1, &hdr) );
    core_cpystrn(imsi_bcd, (char*)hdr->avp_value->os.data, 
        c_min(hdr->avp_value->os.len, MAX_IMSI_BCD_LEN)+1);

    CHECK_FCT( fd_msg_search_avp(qry, gx_called_station_id, &avp) );
    CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
    core_cpystrn(apn, (char*)hdr->avp_value->os.data, 
        c_min(hdr->avp_value->os.len, MAX_APN_LEN)+1);

    rv = pcrf_db_pdn_data(imsi_bcd, apn, &cca_message);
    if (rv != CORE_OK)
    {
        d_error("Cannot get data for IMSI(%s)+APN(%s)'\n", imsi_bcd, apn);
        goto out;
    }

    if (sess_data->cc_request_type != GX_CC_REQUEST_TYPE_TERMINATION_REQUEST)
    {
        /* Set Charging-Rule-Install */
        if (cca_message.num_of_pcc_rule)
            CHECK_FCT( fd_msg_avp_new(gx_charging_rule_install, 0, &avp) );

        for (i = 0; i < cca_message.num_of_pcc_rule; i++)
        {
            pcc_rule_t *pcc_rule = &cca_message.pcc_rule[i];

            CHECK_FCT( fd_msg_avp_new(gx_charging_rule_definition, 0,
                        &avpch1) );

            CHECK_FCT( fd_msg_avp_new(gx_charging_rule_name, 0, &avpch2) );
            /* Charing-Rule-Name is automatically configured by order */
            sprintf(pcc_rule->name, "%s%d", apn, i+1);
            val.os.data = (c_uint8_t *)pcc_rule->name;
            val.os.len = strlen(pcc_rule->name);
            CHECK_FCT( fd_msg_avp_setvalue(avpch2, &val) );
            CHECK_FCT( fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2) );

            for (j = 0; j < pcc_rule->num_of_flow; j++)
            {
                flow_t *flow = &pcc_rule->flow[j];

                CHECK_FCT( fd_msg_avp_new(gx_flow_information, 0, &avpch2) );

                CHECK_FCT( fd_msg_avp_new(gx_flow_direction, 0, &avpch3) ); 
                val.i32 = flow->direction;
                CHECK_FCT( fd_msg_avp_setvalue(avpch3, &val) );
                CHECK_FCT( fd_msg_avp_add(avpch2, MSG_BRW_LAST_CHILD, avpch3) );

                CHECK_FCT( fd_msg_avp_new(gx_flow_description, 0, &avpch3) ); 
                val.os.data = (c_uint8_t *)flow->description;
                val.os.len = strlen(flow->description);
                CHECK_FCT( fd_msg_avp_setvalue(avpch3, &val) );
                CHECK_FCT( fd_msg_avp_add(avpch2, MSG_BRW_LAST_CHILD, avpch3) );

                CHECK_FCT( fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2) );
            }

            CHECK_FCT( fd_msg_avp_new(gx_flow_status, 0, &avpch2) );
            val.i32 = GX_FLOW_STATUS_ENABLED;
            CHECK_FCT( fd_msg_avp_setvalue(avpch2, &val) );
            CHECK_FCT( fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2) );

            CHECK_FCT( fd_msg_avp_new(gx_qos_information, 0, &avpch2) )

            CHECK_FCT( fd_msg_avp_new(gx_qos_class_identifier, 0, &avpch3) );
            val.u32 = pcc_rule->qos.qci;
            CHECK_FCT( fd_msg_avp_setvalue (avpch3, &val) );
            CHECK_FCT( fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3) );

            CHECK_FCT( fd_msg_avp_new(gx_allocation_retention_priority, 0,
                        &avpch3) );

            CHECK_FCT( fd_msg_avp_new(gx_priority_level, 0, &avpch4) );
            val.u32 = pcc_rule->qos.arp.priority_level;
            CHECK_FCT( fd_msg_avp_setvalue (avpch4, &val) );
            CHECK_FCT( fd_msg_avp_add (avpch3, MSG_BRW_LAST_CHILD, avpch4) );

            CHECK_FCT( fd_msg_avp_new(gx_pre_emption_capability, 0, &avpch4) );
            val.u32 = pcc_rule->qos.arp.pre_emption_capability;
            CHECK_FCT( fd_msg_avp_setvalue (avpch4, &val) );
            CHECK_FCT( fd_msg_avp_add (avpch3, MSG_BRW_LAST_CHILD, avpch4) );

            CHECK_FCT( fd_msg_avp_new(gx_pre_emption_vulnerability, 0,
                        &avpch4) );
            val.u32 = pcc_rule->qos.arp.pre_emption_vulnerability;
            CHECK_FCT( fd_msg_avp_setvalue (avpch4, &val) );
            CHECK_FCT( fd_msg_avp_add (avpch3, MSG_BRW_LAST_CHILD, avpch4) );

            CHECK_FCT( fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3) );

            if (pcc_rule->qos.mbr.uplink)
            {
                CHECK_FCT(
                    fd_msg_avp_new(gx_max_requested_bandwidth_ul, 0, &avpch3) );
                val.u32 = pcc_rule->qos.mbr.uplink;
                CHECK_FCT( fd_msg_avp_setvalue (avpch3, &val) );
                CHECK_FCT(
                    fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3) );
            }

            if (pcc_rule->qos.mbr.downlink)
            {
                CHECK_FCT(
                    fd_msg_avp_new(gx_max_requested_bandwidth_dl, 0, &avpch3) );
                val.u32 = pcc_rule->qos.mbr.downlink;
                CHECK_FCT( fd_msg_avp_setvalue (avpch3, &val) );
                CHECK_FCT(
                    fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3) );
            }

            if (pcc_rule->qos.gbr.uplink)
            {
                CHECK_FCT(
                    fd_msg_avp_new(gx_guaranteed_bitrate_ul, 0, &avpch3) );
                val.u32 = pcc_rule->qos.gbr.uplink;
                CHECK_FCT( fd_msg_avp_setvalue (avpch3, &val) );
                CHECK_FCT(
                    fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3) );
            }

            if (pcc_rule->qos.gbr.downlink)
            {
                CHECK_FCT(
                    fd_msg_avp_new(gx_guaranteed_bitrate_dl, 0, &avpch3) );
                val.u32 = pcc_rule->qos.gbr.downlink;
                CHECK_FCT( fd_msg_avp_setvalue (avpch3, &val) );
                CHECK_FCT(
                    fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3) );
            }

            CHECK_FCT( fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2) );

            CHECK_FCT( fd_msg_avp_new(gx_precedence, 0, &avpch2) )
            val.u32 = i + 1; /* Precendence is automatically configured by order */
            CHECK_FCT( fd_msg_avp_setvalue (avpch2, &val) );
            CHECK_FCT( fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2) );

            CHECK_FCT( fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1) );
        }
        
        if (cca_message.num_of_pcc_rule)
            CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );

        /* Set QoS-Information */
        if (cca_message.pdn.ambr.downlink || cca_message.pdn.ambr.uplink)
        {
            CHECK_FCT( fd_msg_avp_new(gx_qos_information, 0, &avp) );

            if (cca_message.pdn.ambr.uplink)
            {
                CHECK_FCT(
                    fd_msg_avp_new(gx_apn_aggregate_max_bitrate_ul, 0, &avpch1) );
                val.u32 = cca_message.pdn.ambr.uplink;
                CHECK_FCT( fd_msg_avp_setvalue (avpch1, &val) );
                CHECK_FCT( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1) );
            }
            
            if (cca_message.pdn.ambr.downlink)
            {
                CHECK_FCT( fd_msg_avp_new(gx_apn_aggregate_max_bitrate_dl, 0,
                        &avpch1) );
                val.u32 = cca_message.pdn.ambr.downlink;
                CHECK_FCT( fd_msg_avp_setvalue (avpch1, &val) );
                CHECK_FCT( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1) );
            }

            CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );
        }

        /* Set Default-EPS-Bearer-QoS */
        CHECK_FCT( fd_msg_avp_new(gx_default_eps_bearer_qos, 0, &avp) );

        CHECK_FCT( fd_msg_avp_new(gx_qos_class_identifier, 0, &avpch1) );
        val.u32 = cca_message.pdn.qos.qci;
        CHECK_FCT( fd_msg_avp_setvalue (avpch1, &val) );
        CHECK_FCT( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1) );

        CHECK_FCT(
            fd_msg_avp_new(gx_allocation_retention_priority, 0, &avpch1) );

        CHECK_FCT( fd_msg_avp_new(gx_priority_level, 0, &avpch2) );
        val.u32 = cca_message.pdn.qos.arp.priority_level;
        CHECK_FCT( fd_msg_avp_setvalue (avpch2, &val) );
        CHECK_FCT( fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2) );

        CHECK_FCT( fd_msg_avp_new(gx_pre_emption_capability, 0, &avpch2) );
        val.u32 = cca_message.pdn.qos.arp.pre_emption_capability;
        CHECK_FCT( fd_msg_avp_setvalue (avpch2, &val) );
        CHECK_FCT( fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2) );

        CHECK_FCT( fd_msg_avp_new(gx_pre_emption_vulnerability, 0, &avpch2) );
        val.u32 = cca_message.pdn.qos.arp.pre_emption_vulnerability;
        CHECK_FCT( fd_msg_avp_setvalue (avpch2, &val) );
        CHECK_FCT( fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2) );

        CHECK_FCT( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1) );

        CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );

        /* Set Supported Features */
        CHECK_FCT( fd_msg_avp_new(gx_supported_features, 0, &avp) );

        CHECK_FCT( fd_msg_avp_new(gx_feature_list_id, 0, &avpch1) );
        val.i32 = 1;
        CHECK_FCT( fd_msg_avp_setvalue (avpch1, &val) );
        CHECK_FCT( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1) );

        CHECK_FCT( fd_msg_avp_new(gx_feature_list, 0, &avpch1) );
        val.u32 = 0x0000000b;
        CHECK_FCT( fd_msg_avp_setvalue (avpch1, &val) );
        CHECK_FCT( fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1) );

        CHECK_FCT( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp) );
    }

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) );

    if (sess_data->cc_request_type == GX_CC_REQUEST_TYPE_TERMINATION_REQUEST)
    {
        /* Store this value in the session */
        CHECK_FCT_DO( fd_sess_state_store(pcrf_gx_reg, sess, &sess_data),
                goto out );
        d_assert(sess_data == NULL,,);
    }
    else
    {
        pcrf_gx_sess_cleanup(sess_data, NULL, NULL);
    }

	/* Send the answer */
	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );

	/* Add this value to the stats */
	CHECK_POSIX_DO( pthread_mutex_lock(&fd_logger_self()->stats_lock), );
	fd_logger_self()->stats.nb_echoed++;
	CHECK_POSIX_DO( pthread_mutex_unlock(&fd_logger_self()->stats_lock), );

    gx_cca_message_free(&cca_message);

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

    gx_cca_message_free(&cca_message);

    return 0;
}

int pcrf_gx_init(void)
{
	struct disp_when data;

    pool_init(&pcrf_gx_sess_pool, MAX_POOL_OF_DIAMETER_SESS);

	/* Install objects definitions for this application */
	CHECK_FCT( gx_dict_init() );

    /* Create handler for sessions */
	CHECK_FCT( fd_sess_handler_create(&pcrf_gx_reg, pcrf_gx_sess_cleanup,
                NULL, NULL) );

	memset(&data, 0, sizeof(data));
	data.app = gx_application;
	
	CHECK_FCT( fd_disp_register(pcrf_gx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_gx_fb) );
	
	data.command = gx_cmd_ccr;
	CHECK_FCT( fd_disp_register(pcrf_gx_ccr_cb, DISP_HOW_CC, &data, NULL,
                &hdl_gx_ccr) );

	/* Advertise the support for the application in the peer */
	CHECK_FCT( fd_disp_app_support(gx_application, fd_vendor, 1, 0) );

	return 0;
}

void pcrf_gx_final(void)
{
	CHECK_FCT_DO( fd_sess_handler_destroy(&pcrf_gx_reg, NULL), );
	if (hdl_gx_fb)
		(void) fd_disp_unregister(&hdl_gx_fb, NULL);
	if (hdl_gx_ccr)
		(void) fd_disp_unregister(&hdl_gx_ccr, NULL);

    if (pool_used(&pcrf_gx_sess_pool))
        d_error("%d not freed in pcrf_gx_sess_pool[%d] of GX-SM",
                pool_used(&pcrf_gx_sess_pool), pool_size(&pcrf_gx_sess_pool));
    d_trace(3, "%d not freed in pcrf_gx_sess_pool[%d] of GX-SM\n",
            pool_used(&pcrf_gx_sess_pool), pool_size(&pcrf_gx_sess_pool));

    pool_final(&pcrf_gx_sess_pool);
}
