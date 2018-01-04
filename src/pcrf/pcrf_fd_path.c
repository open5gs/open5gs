#define TRACE_MODULE _pcrf_fd_path

#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"

#include "fd/fd_lib.h"
#include "fd/gx/gx_dict.h"
#include "fd/gx/gx_message.h"
#include "fd/rx/rx_dict.h"
#include "fd/rx/rx_message.h"

#include "pcrf_context.h"

/************************************************************************
 * GX handler ( PCRF - PGW)
 */
static struct disp_hdl *hdl_gx_fb = NULL; 
static struct disp_hdl *hdl_gx_ccr = NULL; 

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

    status_t rv;
    gx_cca_message_t cca_message;
    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];
    c_int8_t apn[MAX_APN_LEN+1];
    int i, j;

    c_uint32_t cc_request_type = 0;
    c_uint32_t result_code = GX_DIAMETER_ERROR_USER_UNKNOWN;
	
    d_assert(msg, return EINVAL,);

    /* Initialize Message */
    memset(&cca_message, 0, sizeof(gx_cca_message_t));

	/* Create answer header */
	qry = *msg;
	CHECK_FCT( fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0) );
    ans = *msg;

    /* Get CC-Request-Type */
    CHECK_FCT( fd_msg_search_avp(qry, gx_cc_request_type, &avp) );
    CHECK_FCT( fd_msg_avp_hdr(avp, &hdr) );
    cc_request_type = hdr->avp_value->i32;

    /* Set the Auth-Application-Id AVP */
    CHECK_FCT_DO( fd_msg_avp_new(fd_auth_application_id, 0, &avp), goto out );
    val.i32 = GX_APPLICATION_ID;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

    /* Set CC-Request-Type, CC-Request-Number */
    CHECK_FCT_DO( fd_msg_avp_new(gx_cc_request_type, 0, &avp), goto out );
    val.i32 = cc_request_type;
    CHECK_FCT_DO( fd_msg_avp_setvalue(avp, &val), goto out );
    CHECK_FCT_DO( fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp), goto out );

    CHECK_FCT_DO( fd_msg_avp_new(gx_cc_request_number, 0, &avp), goto out );
    val.i32 = 1;
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

    if (cc_request_type != GX_CC_REQUEST_TYPE_TERMINATION_REQUEST)
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

/************************************************************************
 * RX handler ( PCRF - PGW)
 */
static struct disp_hdl *hdl_rx_fb = NULL; 
static struct disp_hdl *hdl_rx_aar = NULL; 

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
	struct msg *ans;
#if 0
	struct msg *ans, *qry;
    struct avp *avpch1, *avpch2, *avpch3, *avpch4;
    struct avp_hdr *hdr;
#endif
    union avp_value val;

#if 0
    status_t rv;
    gx_cca_message_t cca_message;
    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];
    c_int8_t apn[MAX_APN_LEN+1];
    int i, j;

    c_uint32_t cc_request_type = 0;
    c_uint32_t result_code = RX_DIAMETER_ERROR_USER_UNKNOWN;
#endif
	
    d_assert(msg, return EINVAL,);

#if 0
    /* Initialize Message */
    memset(&cca_message, 0, sizeof(gx_cca_message_t));
#endif

	/* Create answer header */
#if 0
	qry = *msg;
#endif
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

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	CHECK_FCT( fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1) );

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
#if 0
    if (result_code == RX_DIAMETER_ERROR_USER_UNKNOWN)
    {
        CHECK_FCT( fd_msg_rescode_set(ans,
                    "DIAMETER_ERROR_USER_UNKNOWN", NULL, NULL, 1) );
    }
    else
    {
        CHECK_FCT( fd_message_experimental_rescode_set(ans, result_code) );
    }
#endif

	CHECK_FCT( fd_msg_send(msg, NULL, NULL) );

#if 0
    gx_cca_message_free(&cca_message);
#endif

    return 0;
}

int pcrf_fd_init(void)
{
	struct disp_when data;

    CHECK_FCT(
        fd_init(FD_MODE_CLIENT|FD_MODE_SERVER, pcrf_self()->fd_conf_path) );

	/* GX Interface */
	CHECK_FCT( gx_dict_init() );

	memset(&data, 0, sizeof(data));
	data.app = gx_application;
	
	CHECK_FCT( fd_disp_register(pcrf_gx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_gx_fb) );
	
	data.command = gx_cmd_ccr;
	CHECK_FCT( fd_disp_register(pcrf_gx_ccr_cb, DISP_HOW_CC, &data, NULL,
                &hdl_gx_ccr) );

	CHECK_FCT( fd_disp_app_support(gx_application, fd_vendor, 1, 0) );

	/* RX Interface */
	CHECK_FCT( rx_dict_init() );

	memset(&data, 0, sizeof(data));
	data.app = rx_application;
	
	CHECK_FCT( fd_disp_register(pcrf_rx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_rx_fb) );
	
	data.command = rx_cmd_aar;
	CHECK_FCT( fd_disp_register(pcrf_rx_aar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_rx_aar) );

	CHECK_FCT( fd_disp_app_support(rx_application, fd_vendor, 1, 0) );

	return 0;
}

void pcrf_fd_final(void)
{
	if (hdl_gx_fb) {
		(void) fd_disp_unregister(&hdl_gx_fb, NULL);
	}
	if (hdl_gx_ccr) {
		(void) fd_disp_unregister(&hdl_gx_ccr, NULL);
	}

	if (hdl_rx_fb) {
		(void) fd_disp_unregister(&hdl_rx_fb, NULL);
	}
	if (hdl_rx_aar) {
		(void) fd_disp_unregister(&hdl_rx_aar, NULL);
	}

    fd_final();
}
