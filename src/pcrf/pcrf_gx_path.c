#define TRACE_MODULE _pcrf_gx_path

#include "core_lib.h"
#include "core_debug.h"
#include "core_pool.h"
#include "core_pkbuf.h"

#include "fd/fd_lib.h"
#include "fd/gx/gx_dict.h"
#include "fd/gx/gx_message.h"

#include "pcrf_context.h"
#include "pcrf_fd_path.h"

struct sess_state {
    c_uint32_t  cc_request_type;    /* CC-Request-Type */
    os0_t       gx_sid;             /* Gx Session-Id */
    os0_t       rx_sid;             /* Rx Session-Id */

ED3(c_uint8_t   ipv4:1;,
    c_uint8_t   ipv6:1;,
    c_uint8_t   reserved:6;)
    c_uint32_t  addr;               /* Framed-IPv4-Address */
    c_uint8_t   addr6[IPV6_LEN];    /* Framed-IPv6-Prefix */

    struct timespec ts;             /* Time of sending the message */
};

static struct session_handler *pcrf_gx_reg = NULL;
static struct disp_hdl *hdl_gx_fb = NULL; 
static struct disp_hdl *hdl_gx_ccr = NULL; 

pool_declare(pcrf_gx_sess_pool, struct sess_state, MAX_POOL_OF_DIAMETER_SESS);

static void pcrf_gx_raa_cb(void *data, struct msg **msg);

static __inline__ struct sess_state *new_state(os0_t sid)
{
    struct sess_state *new = NULL;
    pool_alloc_node(&pcrf_gx_sess_pool, &new);
    d_assert(new, return NULL,);
    memset(new, 0, sizeof *new);

    new->gx_sid = (os0_t)core_strdup((char *)sid);
    d_assert(new->gx_sid, return NULL,);

    return new;
}

static void state_cleanup(
        struct sess_state *sess_data, os0_t sid, void *opaque)
{
    d_assert(sess_data, return,);

    if (sess_data->ipv4)
        pcrf_sess_set_ipv4(&sess_data->addr, NULL);
    if (sess_data->ipv6)
        pcrf_sess_set_ipv6(sess_data->addr6, NULL);

    if (sess_data->gx_sid)
        core_free(sess_data->gx_sid);
    if (sess_data->rx_sid)
        core_free(sess_data->rx_sid);

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
    status_t rv;
    int ret = 0;

	struct msg *ans, *qry;
    struct avp *avpch1, *avpch2, *avpch3, *avpch4;
    struct avp_hdr *hdr;
    union avp_value val;
    struct sess_state *sess_data = NULL;

    gx_cca_message_t cca_message;
    c_int8_t imsi_bcd[MAX_IMSI_BCD_LEN+1];
    c_int8_t apn[MAX_APN_LEN+1];
    int i, j;

    c_uint32_t cc_request_number = 0;
    c_uint32_t result_code = GX_DIAMETER_ERROR_USER_UNKNOWN;
	
    d_assert(msg, return EINVAL,);

    ret = fd_sess_state_retrieve(pcrf_gx_reg, sess, &sess_data);
    d_assert(ret == 0, return EINVAL,);

    if (!sess_data)
    {
        os0_t sid;
        size_t sidlen;

        ret = fd_sess_getsid(sess, &sid, &sidlen);
        d_assert(ret == 0, return EINVAL,);

        sess_data = new_state(sid);
        d_assert(sess_data, return EINVAL,);
    }

    /* Initialize Message */
    memset(&cca_message, 0, sizeof(gx_cca_message_t));

	/* Create answer header */
	qry = *msg;
	ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    d_assert(ret == 0, return EINVAL,);
    ans = *msg;

    /* Get CC-Request-Type */
    ret = fd_msg_search_avp(qry, gx_cc_request_type, &avp);
    d_assert(ret == 0, return EINVAL,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return EINVAL,);
        sess_data->cc_request_type = hdr->avp_value->i32;
    }
    else
    {
        d_error("no_CC-Request-Type ");
        goto out;
    }

    /* Get CC-Request-Number */
    ret = fd_msg_search_avp(qry, gx_cc_request_number, &avp);
    d_assert(ret == 0, return EINVAL,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return EINVAL,);
        cc_request_number = hdr->avp_value->i32;
    }

    /* Get Framed-IP-Address */
    ret = fd_msg_search_avp(qry, gx_framed_ip_address, &avp);
    d_assert(ret == 0, return EINVAL,);
    if (avp)
    {
        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return EINVAL,);

        memcpy(&sess_data->addr, hdr->avp_value->os.data,
                sizeof sess_data->addr);
        pcrf_sess_set_ipv4(&sess_data->addr, sess_data->gx_sid);
        sess_data->ipv4 = 1;
    }

    /* Get Framed-IPv6-Prefix */
    ret = fd_msg_search_avp(qry, gx_framed_ipv6_prefix, &avp);
    d_assert(ret == 0, return EINVAL,);
    if (avp)
    {
        paa_t *paa = NULL;

        ret = fd_msg_avp_hdr(avp, &hdr);
        d_assert(ret == 0, return EINVAL,);

        paa = (paa_t *)hdr->avp_value->os.data;
        d_assert(paa, return EINVAL,);
        d_assert(paa->len == IPV6_LEN * 8 /* 128bit */, return EINVAL,
                "Invalid Framed-IPv6-Prefix Length:%d", paa->len);
        memcpy(sess_data->addr6, paa->addr6, sizeof sess_data->addr6);
        pcrf_sess_set_ipv6(sess_data->addr6, sess_data->gx_sid);
        sess_data->ipv6 = 1;
    }

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(fd_auth_application_id, 0, &avp);
    d_assert(ret == 0, return EINVAL,);
    val.i32 = GX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return EINVAL,);

    /* Set CC-Request-Type */
    ret = fd_msg_avp_new(gx_cc_request_type, 0, &avp);
    d_assert(ret == 0, return EINVAL,);
    val.i32 = sess_data->cc_request_type;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return EINVAL,);

    /* Set CC-Request-Number */
    ret = fd_msg_avp_new(gx_cc_request_number, 0, &avp);
    d_assert(ret == 0, return EINVAL,);
    val.i32 = cc_request_number;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return EINVAL,);

    /* Get IMSI + APN */
    ret = fd_msg_search_avp(qry, gx_subscription_id, &avp);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_hdr(avp, &hdr);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_avp_search_avp(avp, gx_subscription_id_type, &avpch1);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_hdr(avpch1, &hdr);
    d_assert(ret == 0, return EINVAL,);
    if (hdr->avp_value->i32 != GX_SUBSCRIPTION_ID_TYPE_END_USER_IMSI)
    {
        d_error("Not implemented Subscription-Id-Type(%d)",
                hdr->avp_value->i32);
        goto out;
    }
    ret = fd_avp_search_avp(avp, gx_subscription_id_data, &avpch1);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_hdr(avpch1, &hdr);
    d_assert(ret == 0, return EINVAL,);
    core_cpystrn(imsi_bcd, (char*)hdr->avp_value->os.data, 
        c_min(hdr->avp_value->os.len, MAX_IMSI_BCD_LEN)+1);

    ret = fd_msg_search_avp(qry, gx_called_station_id, &avp);
    d_assert(ret == 0, return EINVAL,);
    ret = fd_msg_avp_hdr(avp, &hdr);
    d_assert(ret == 0, return EINVAL,);
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
        {
            ret = fd_msg_avp_new(gx_charging_rule_install, 0, &avp);
            d_assert(ret == 0, return EINVAL,);
        }

        for (i = 0; i < cca_message.num_of_pcc_rule; i++)
        {
            pcc_rule_t *pcc_rule = &cca_message.pcc_rule[i];

            ret = fd_msg_avp_new(gx_charging_rule_definition, 0, &avpch1);
            d_assert(ret == 0, return EINVAL,);
            ret = fd_msg_avp_new(gx_charging_rule_name, 0, &avpch2);
            d_assert(ret == 0, return EINVAL,);
            /* Charing-Rule-Name is automatically configured by order */
            sprintf(pcc_rule->name, "%s%d", apn, i+1);
            val.os.data = (c_uint8_t *)pcc_rule->name;
            val.os.len = strlen(pcc_rule->name);
            ret = fd_msg_avp_setvalue(avpch2, &val);
            d_assert(ret == 0, return EINVAL,);
            ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
            d_assert(ret == 0, return EINVAL,);

            for (j = 0; j < pcc_rule->num_of_flow; j++)
            {
                flow_t *flow = &pcc_rule->flow[j];

                ret = fd_msg_avp_new(gx_flow_information, 0, &avpch2);
                d_assert(ret == 0, return EINVAL,);

                ret = fd_msg_avp_new(gx_flow_direction, 0, &avpch3); 
                d_assert(ret == 0, return EINVAL,);
                val.i32 = flow->direction;
                ret = fd_msg_avp_setvalue(avpch3, &val);
                d_assert(ret == 0, return EINVAL,);
                ret = fd_msg_avp_add(avpch2, MSG_BRW_LAST_CHILD, avpch3);
                d_assert(ret == 0, return EINVAL,);

                ret = fd_msg_avp_new(gx_flow_description, 0, &avpch3); 
                d_assert(ret == 0, return EINVAL,);
                val.os.data = (c_uint8_t *)flow->description;
                val.os.len = strlen(flow->description);
                ret = fd_msg_avp_setvalue(avpch3, &val);
                d_assert(ret == 0, return EINVAL,);
                ret = fd_msg_avp_add(avpch2, MSG_BRW_LAST_CHILD, avpch3);
                d_assert(ret == 0, return EINVAL,);

                ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
                d_assert(ret == 0, return EINVAL,);
            }

            ret = fd_msg_avp_new(gx_flow_status, 0, &avpch2);
            d_assert(ret == 0, return EINVAL,);
            val.i32 = GX_FLOW_STATUS_ENABLED;
            ret = fd_msg_avp_setvalue(avpch2, &val);
            d_assert(ret == 0, return EINVAL,);
            ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
            d_assert(ret == 0, return EINVAL,);

            ret = fd_msg_avp_new(gx_qos_information, 0, &avpch2);
            d_assert(ret == 0, return EINVAL,);

            ret = fd_msg_avp_new(gx_qos_class_identifier, 0, &avpch3);
            d_assert(ret == 0, return EINVAL,);
            val.u32 = pcc_rule->qos.qci;
            ret = fd_msg_avp_setvalue (avpch3, &val);
            d_assert(ret == 0, return EINVAL,);
            ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
            d_assert(ret == 0, return EINVAL,);

            ret = fd_msg_avp_new(gx_allocation_retention_priority, 0, &avpch3);
            d_assert(ret == 0, return EINVAL,);

            ret = fd_msg_avp_new(gx_priority_level, 0, &avpch4);
            d_assert(ret == 0, return EINVAL,);
            val.u32 = pcc_rule->qos.arp.priority_level;
            ret = fd_msg_avp_setvalue (avpch4, &val);
            d_assert(ret == 0, return EINVAL,);
            ret = fd_msg_avp_add (avpch3, MSG_BRW_LAST_CHILD, avpch4);
            d_assert(ret == 0, return EINVAL,);

            ret = fd_msg_avp_new(gx_pre_emption_capability, 0, &avpch4);
            d_assert(ret == 0, return EINVAL,);
            val.u32 = pcc_rule->qos.arp.pre_emption_capability;
            ret = fd_msg_avp_setvalue (avpch4, &val);
            d_assert(ret == 0, return EINVAL,);
            ret = fd_msg_avp_add (avpch3, MSG_BRW_LAST_CHILD, avpch4);
            d_assert(ret == 0, return EINVAL,);

            ret = fd_msg_avp_new(gx_pre_emption_vulnerability, 0, &avpch4);
            d_assert(ret == 0, return EINVAL,);
            val.u32 = pcc_rule->qos.arp.pre_emption_vulnerability;
            ret = fd_msg_avp_setvalue (avpch4, &val);
            d_assert(ret == 0, return EINVAL,);
            ret = fd_msg_avp_add (avpch3, MSG_BRW_LAST_CHILD, avpch4);
            d_assert(ret == 0, return EINVAL,);

            ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
            d_assert(ret == 0, return EINVAL,);

            if (pcc_rule->qos.mbr.uplink)
            {
                ret = fd_msg_avp_new(gx_max_requested_bandwidth_ul, 0, &avpch3);
                d_assert(ret == 0, return EINVAL,);
                val.u32 = pcc_rule->qos.mbr.uplink;
                ret = fd_msg_avp_setvalue (avpch3, &val);
                d_assert(ret == 0, return EINVAL,);
                ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
                d_assert(ret == 0, return EINVAL,);
            }

            if (pcc_rule->qos.mbr.downlink)
            {
                ret = fd_msg_avp_new(gx_max_requested_bandwidth_dl, 0, &avpch3);
                d_assert(ret == 0, return EINVAL,);
                val.u32 = pcc_rule->qos.mbr.downlink;
                ret = fd_msg_avp_setvalue (avpch3, &val);
                d_assert(ret == 0, return EINVAL,);
                ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
                d_assert(ret == 0, return EINVAL,);
            }

            if (pcc_rule->qos.gbr.uplink)
            {
                ret = fd_msg_avp_new(gx_guaranteed_bitrate_ul, 0, &avpch3);
                d_assert(ret == 0, return EINVAL,);
                val.u32 = pcc_rule->qos.gbr.uplink;
                ret = fd_msg_avp_setvalue (avpch3, &val);
                d_assert(ret == 0, return EINVAL,);
                ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
                d_assert(ret == 0, return EINVAL,);
            }

            if (pcc_rule->qos.gbr.downlink)
            {
                ret = fd_msg_avp_new(gx_guaranteed_bitrate_dl, 0, &avpch3);
                d_assert(ret == 0, return EINVAL,);
                val.u32 = pcc_rule->qos.gbr.downlink;
                ret = fd_msg_avp_setvalue (avpch3, &val);
                d_assert(ret == 0, return EINVAL,);
                ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
                d_assert(ret == 0, return EINVAL,);
            }

            ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
            d_assert(ret == 0, return EINVAL,);

            ret = fd_msg_avp_new(gx_precedence, 0, &avpch2);
            d_assert(ret == 0, return EINVAL,);
            val.u32 = i + 1; /* Precendence is automatically configured by order */
            ret = fd_msg_avp_setvalue (avpch2, &val);
            d_assert(ret == 0, return EINVAL,);
            ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
            d_assert(ret == 0, return EINVAL,);

            ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch1);
            d_assert(ret == 0, return EINVAL,);
        }
        
        if (cca_message.num_of_pcc_rule)
        {
            ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
            d_assert(ret == 0, return EINVAL,);
        }

        /* Set QoS-Information */
        if (cca_message.pdn.ambr.downlink || cca_message.pdn.ambr.uplink)
        {
            ret = fd_msg_avp_new(gx_qos_information, 0, &avp);
            d_assert(ret == 0, return EINVAL,);

            if (cca_message.pdn.ambr.uplink)
            {
                ret = fd_msg_avp_new(gx_apn_aggregate_max_bitrate_ul, 0,
                        &avpch1);
                d_assert(ret == 0, return EINVAL,);
                val.u32 = cca_message.pdn.ambr.uplink;
                ret = fd_msg_avp_setvalue (avpch1, &val);
                d_assert(ret == 0, return EINVAL,);
                ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
                d_assert(ret == 0, return EINVAL,);
            }
            
            if (cca_message.pdn.ambr.downlink)
            {
                ret = fd_msg_avp_new(gx_apn_aggregate_max_bitrate_dl, 0,
                        &avpch1);
                d_assert(ret == 0, return EINVAL,);
                val.u32 = cca_message.pdn.ambr.downlink;
                ret = fd_msg_avp_setvalue (avpch1, &val);
                d_assert(ret == 0, return EINVAL,);
                ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
                d_assert(ret == 0, return EINVAL,);
            }

            ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
            d_assert(ret == 0, return EINVAL,);
        }

        /* Set Default-EPS-Bearer-QoS */
        ret = fd_msg_avp_new(gx_default_eps_bearer_qos, 0, &avp);
        d_assert(ret == 0, return EINVAL,);

        ret = fd_msg_avp_new(gx_qos_class_identifier, 0, &avpch1);
        d_assert(ret == 0, return EINVAL,);
        val.u32 = cca_message.pdn.qos.qci;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return EINVAL,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return EINVAL,);

        ret = fd_msg_avp_new(gx_allocation_retention_priority, 0, &avpch1);
        d_assert(ret == 0, return EINVAL,);

        ret = fd_msg_avp_new(gx_priority_level, 0, &avpch2);
        d_assert(ret == 0, return EINVAL,);
        val.u32 = cca_message.pdn.qos.arp.priority_level;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return EINVAL,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return EINVAL,);

        ret = fd_msg_avp_new(gx_pre_emption_capability, 0, &avpch2);
        d_assert(ret == 0, return EINVAL,);
        val.u32 = cca_message.pdn.qos.arp.pre_emption_capability;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return EINVAL,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return EINVAL,);

        ret = fd_msg_avp_new(gx_pre_emption_vulnerability, 0, &avpch2);
        d_assert(ret == 0, return EINVAL,);
        val.u32 = cca_message.pdn.qos.arp.pre_emption_vulnerability;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        d_assert(ret == 0, return EINVAL,);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        d_assert(ret == 0, return EINVAL,);

        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return EINVAL,);

        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        d_assert(ret == 0, return EINVAL,);

        /* Set Supported Features */
        ret = fd_msg_avp_new(gx_supported_features, 0, &avp);
        d_assert(ret == 0, return EINVAL,);

        ret = fd_msg_avp_new(gx_feature_list_id, 0, &avpch1);
        d_assert(ret == 0, return EINVAL,);
        val.i32 = 1;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return EINVAL,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return EINVAL,);

        ret = fd_msg_avp_new(gx_feature_list, 0, &avpch1);
        d_assert(ret == 0, return EINVAL,);
        val.u32 = 0x0000000b;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        d_assert(ret == 0, return EINVAL,);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        d_assert(ret == 0, return EINVAL,);

        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        d_assert(ret == 0, return EINVAL,);
    }

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	ret = fd_msg_rescode_set(ans, "DIAMETER_SUCCESS", NULL, NULL, 1);
    d_assert(ret == 0, return EINVAL,);

    if (sess_data->cc_request_type != GX_CC_REQUEST_TYPE_TERMINATION_REQUEST)
    {
        /* Store this value in the session */
        ret = fd_sess_state_store(pcrf_gx_reg, sess, &sess_data);
        d_assert(ret == 0,,);
        d_assert(sess_data == NULL,,);
    }
    else
    {
        state_cleanup(sess_data, NULL, NULL);
    }

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    d_assert(ret == 0,,);

	/* Add this value to the stats */
	d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,,);
	fd_logger_self()->stats.nb_echoed++;
	d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) ==0,, );

    gx_cca_message_free(&cca_message);

    return 0;

out:
	/* Set the Result-Code */
    if (result_code == GX_DIAMETER_ERROR_USER_UNKNOWN)
    {
        ret = fd_msg_rescode_set(ans,
                    "DIAMETER_ERROR_USER_UNKNOWN", NULL, NULL, 1);
        d_assert(ret == 0, return EINVAL,);
    }
    else
    {
        ret = fd_message_experimental_rescode_set(ans, result_code);
        d_assert(ret == 0, return EINVAL,);
    }

    if (sess_data->cc_request_type != GX_CC_REQUEST_TYPE_TERMINATION_REQUEST)
    {
        /* Store this value in the session */
        ret = fd_sess_state_store(pcrf_gx_reg, sess, &sess_data);
        d_assert(sess_data == NULL,,);
    }
    else
    {
        state_cleanup(sess_data, NULL, NULL);
    }

	ret = fd_msg_send(msg, NULL, NULL);
    d_assert(ret == 0,,);

    gx_cca_message_free(&cca_message);

    return 0;
}

void pcrf_gx_send_rar(c_uint8_t *gx_sid)
{
    int ret;

    struct msg *req = NULL;
    struct avp *avp;
#if 0
    struct avp *avpch1, *avpch2;
#endif
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    int new;
    size_t sidlen;

    d_assert(gx_sid, return,);

    /* Create the request */
    ret = fd_msg_new(gx_cmd_rar, MSGFL_ALLOC_ETEID, &req);
    d_assert(ret == 0, return,);

    /* Retrieve session by Session-Id */
    sidlen = strlen((char *)gx_sid);
    ret = fd_sess_fromsid_msg((os0_t)gx_sid, sidlen, &session, &new);
    d_assert(ret == 0, return,);
    d_assert(new == 0, return,);

    /* Add Session-Id to the message */
    ret = fd_message_session_id_set(req, (os0_t)gx_sid, sidlen);
    d_assert(ret == 0, return,);

    /* Save the session associated with the message */
    ret = fd_msg_sess_set(req, session);
    d_assert(ret == 0, return,);

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(pcrf_gx_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    if (sess_data == NULL)
    {
        d_error("No session data");
        ret = fd_msg_free(req);
        d_assert(ret == 0,,);
        return;
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
    val.i32 = GX_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    d_assert(ret == 0, return,);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    d_assert(ret == 0, return,);

    ret = clock_gettime(CLOCK_REALTIME, &sess_data->ts);
    d_assert(ret == 0, return,);
    
    /* Keep a pointer to the session data for debug purpose, 
     * in real life we would not need it */
    svg = sess_data;
    
    /* Store this value in the session */
    ret = fd_sess_state_store(pcrf_gx_reg, session, &sess_data);
    d_assert(ret == 0, return,);
    d_assert(sess_data == NULL, return,);
    
    /* Send the request */
    ret = fd_msg_send(&req, pcrf_gx_raa_cb, svg);
    d_assert(ret == 0, return,);

    /* Increment the counter */
    d_assert(pthread_mutex_lock(&fd_logger_self()->stats_lock) == 0,,);
    fd_logger_self()->stats.nb_sent++;
    d_assert(pthread_mutex_unlock(&fd_logger_self()->stats_lock) == 0,, );

    d_trace(3, "[Gx] Re-Auth-Request : PCRF --> PGW\n");
}

static void pcrf_gx_raa_cb(void *data, struct msg **msg)
{
}

status_t pcrf_gx_init(void)
{
    int ret;
	struct disp_when data;

    pool_init(&pcrf_gx_sess_pool, MAX_POOL_OF_DIAMETER_SESS);

	/* Install objects definitions for this application */
	ret = gx_dict_init();
    d_assert(ret == 0, return CORE_ERROR,);

    /* Create handler for sessions */
	ret = fd_sess_handler_create(&pcrf_gx_reg, state_cleanup, NULL, NULL);
    d_assert(ret == 0, return CORE_ERROR,);

	memset(&data, 0, sizeof(data));
	data.app = gx_application;
	
	ret = fd_disp_register(pcrf_gx_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_gx_fb);
    d_assert(ret == 0, return CORE_ERROR,);
	
	data.command = gx_cmd_ccr;
	ret = fd_disp_register(pcrf_gx_ccr_cb, DISP_HOW_CC, &data, NULL,
                &hdl_gx_ccr);
    d_assert(ret == 0, return CORE_ERROR,);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(gx_application, fd_vendor, 1, 0);
    d_assert(ret == 0, return CORE_ERROR,);

	return CORE_OK;
}

void pcrf_gx_final(void)
{
    int ret;

	ret = fd_sess_handler_destroy(&pcrf_gx_reg, NULL);
    d_assert(ret == 0,,); 

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

status_t pcrf_sess_gx_associate_rx(c_uint8_t *gx_sid, c_uint8_t *rx_sid)
{
    struct session *sess = NULL;
    struct sess_state *sess_data = NULL;
    int ret;
    size_t sidlen = 0;
    int new;

    d_assert(gx_sid, return CORE_ERROR,);
    d_assert(rx_sid, return CORE_ERROR,);

    sidlen = strlen((char *)gx_sid); 
    ret = fd_sess_fromsid((os0_t)gx_sid, sidlen, &sess, &new);
    d_assert(ret == 0, return CORE_ERROR,);
    d_assert(new == 0, return CORE_ERROR,);

    ret = fd_sess_state_retrieve(pcrf_gx_reg, sess, &sess_data);
    d_assert(ret == 0, return CORE_ERROR,);
    d_assert(sess_data, return CORE_ERROR,);

    if (sess_data->rx_sid)
        core_free(sess_data->rx_sid);
    sess_data->rx_sid = (os0_t)core_strdup((char *)rx_sid);
    d_assert(sess_data->rx_sid, return CORE_ERROR,);

    ret = fd_sess_state_store(pcrf_gx_reg, sess, &sess_data);
    d_assert(ret == 0, return CORE_ERROR,);
    d_assert(sess_data == NULL, return CORE_ERROR,);

    return CORE_OK;
}
