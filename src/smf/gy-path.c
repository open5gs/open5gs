/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "fd-path.h"

static struct session_handler *smf_gy_reg = NULL;
static struct disp_hdl *hdl_gy_fb = NULL;
static struct disp_hdl *hdl_gy_rar = NULL;

struct sess_state {
    os0_t       gy_sid;             /* Gy Session-Id */

    os0_t       peer_host;          /* Peer Host */

#define MAX_CC_REQUEST_NUMBER 32
    smf_sess_t *sess;
    struct {
        bool pfcp;
        void *ptr; /* INITIAL: ogs_gtp_xact_t, UPDATE: ogs_pfcp_xact_t */
    } xact_data[MAX_CC_REQUEST_NUMBER];
    uint32_t cc_request_type;
    uint32_t cc_request_number;

    struct timespec ts; /* Time of sending the message */
};

static OGS_POOL(sess_state_pool, struct sess_state);
static ogs_thread_mutex_t sess_state_mutex;

static int decode_granted_service_unit(
        ogs_diam_gy_service_unit_t *su, struct avp *avpch1, int *perror);
static void smf_gy_cca_cb(void *data, struct msg **msg);

static __inline__ struct sess_state *new_state(os0_t sid)
{
    struct sess_state *new = NULL;

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_alloc(&sess_state_pool, &new);
    ogs_expect_or_return_val(new, NULL);
    memset(new, 0, sizeof(*new));
    ogs_thread_mutex_unlock(&sess_state_mutex);

    new->gy_sid = (os0_t)ogs_strdup((char *)sid);
    ogs_expect_or_return_val(new->gy_sid, NULL);

    return new;
}

static void state_cleanup(struct sess_state *sess_data, os0_t sid, void *opaque)
{
    if (sess_data->gy_sid)
        ogs_free(sess_data->gy_sid);

    if (sess_data->peer_host)
        ogs_free(sess_data->peer_host);

    ogs_thread_mutex_lock(&sess_state_mutex);
    ogs_pool_free(&sess_state_pool, sess_data);
    ogs_thread_mutex_unlock(&sess_state_mutex);
}

/* TS 32.299 7.1.9 Multiple-Services-Credit-Control AVP for CCR */
static void fill_multiple_services_credit_control_ccr(smf_sess_t *sess,
                                                      uint32_t cc_request_type, struct msg *req)
{
    int ret;
    union avp_value val;
    struct avp *avp;
    struct avp *avpch1, *avpch2, *avpch3;

    /* Multiple-Services-Credit-Control */
    ret = fd_msg_avp_new(ogs_diam_gy_multiple_services_cc, 0, &avp);
    ogs_assert(ret == 0);

    /* Requested-Service-Unit, RFC4006 8.18 */
    if (cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_INITIAL_REQUEST ||
        cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_UPDATE_REQUEST) {
        ret = fd_msg_avp_new(ogs_diam_gy_requested_service_unit, 0, &avpch1);
        ogs_assert(ret == 0);

        /* CC-Time, RFC4006 8.21 */
        /* CC-Money, RFC4006 8.22. Not used in 3GPP. */
        /* CC-Total-Octets, RFC4006 8.23 */
        /* CC-Input-Octets, RFC4006 8.24 */
        /* CC-Output-Octets, RFC4006 8.25 */
        /* CC-Service-Specific-Units, RFC4006 8.26 */

        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);
    }

    /* Used-Service-Unit, RFC4006 8.18 */
    ret = fd_msg_avp_new(ogs_diam_gy_used_service_unit, 0, &avpch1);
    ogs_assert(ret == 0);

    /* Reporting-Reason, TS 32.299 7.2.175 */
    /*  "values THRESHOLD, QUOTA_EXHAUSTED and OTHER_QUOTA_TYPE apply to one
        particular quota type and shall occur only in the Used-Service-Units AVP" */
    if (cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_UPDATE_REQUEST) {
        switch (sess->gy.reporting_reason) {
        case OGS_DIAM_GY_REPORTING_REASON_THRESHOLD:
        case OGS_DIAM_GY_REPORTING_REASON_QUOTA_EXHAUSTED:
        case OGS_DIAM_GY_REPORTING_REASON_OTHER_QUOTA_TYPE:
            ret = fd_msg_avp_new(ogs_diam_gy_reporting_reason, 0, &avpch2);
            ogs_assert(ret == 0);
            val.u32 = sess->gy.reporting_reason;
            ret = fd_msg_avp_setvalue (avpch2, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
            ogs_assert(ret == 0);
            break;
        default:
            break;
        }
    }

    /* Tariff-Change-Usage */

    /* CC-Time, RFC4006 8.21 */
    ret = fd_msg_avp_new(ogs_diam_gy_cc_time, 0, &avpch2);
    ogs_assert(ret == 0);
    val.u32 = sess->gy.duration - sess->gy.last_report.duration;
    sess->gy.last_report.duration = sess->gy.duration;
    ret = fd_msg_avp_setvalue (avpch2, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    /* CC-Total-Octets, RFC4006 8.23 */

    /* CC-Input-Octets, RFC4006 8.24 */
    ret = fd_msg_avp_new(ogs_diam_gy_cc_input_octets, 0, &avpch2);
    ogs_assert(ret == 0);
    val.u64 = sess->gy.ul_octets - sess->gy.last_report.ul_octets;
    sess->gy.last_report.ul_octets = sess->gy.ul_octets;
    ret = fd_msg_avp_setvalue (avpch2, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    /* CC-Output-Octets, RFC4006 8.25 */
    ret = fd_msg_avp_new(ogs_diam_gy_cc_output_octets, 0, &avpch2);
    ogs_assert(ret == 0);
    val.u64 = sess->gy.dl_octets - sess->gy.last_report.dl_octets;
    sess->gy.last_report.dl_octets = sess->gy.dl_octets;
    ret = fd_msg_avp_setvalue (avpch2, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    /* CC-Service-Specific-Units, RFC4006 8.26 */

    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

    /* Service-Identifier, RFC4006 8.28. Not used in Gy. */
    /* Rating-Group */

    /* Reporting-Reason, TS 32.299 7.2.175 */
    /* "values QHT, FINAL, VALIDITY_TIME, FORCED_REAUTHORISATION,
     * RATING_CONDITION_CHANGE, UNUSED_QUOTA_TIMER apply for all quota types
     * and are used directly in the Multiple-Services-Credit-Control AVP"
     */
    if (cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_UPDATE_REQUEST ||
        cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_TERMINATION_REQUEST) {
        switch (sess->gy.reporting_reason) {
        case OGS_DIAM_GY_REPORTING_REASON_QHT:
        case OGS_DIAM_GY_REPORTING_REASON_FINAL:
        case OGS_DIAM_GY_REPORTING_REASON_FORCED_REAUTHORISATION:
        case OGS_DIAM_GY_REPORTING_REASON_VALIDITY_TIME:
        case OGS_DIAM_GY_REPORTING_REASON_RATING_CONDITION_CHANGE:
        case OGS_DIAM_GY_REPORTING_REASON_UNUSED_QUOTA_TIMER:
            ret = fd_msg_avp_new(ogs_diam_gy_reporting_reason, 0, &avpch1);
            ogs_assert(ret == 0);
            val.u32 = sess->gy.reporting_reason;
            ret = fd_msg_avp_setvalue (avpch1, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
            ogs_assert(ret == 0);
            break;
        default:
            break;
        }
    }

    /* ... lots of AVPs ... */

    /* QoS-Information */
    ret = fd_msg_avp_new(ogs_diam_gx_qos_information, 0, &avpch1);
    ogs_assert(ret == 0);

    /* QoS-Class-Identifier */
    ret = fd_msg_avp_new(ogs_diam_gy_qos_class_identifier, 0, &avpch2);
    ogs_assert(ret == 0);
    val.u32 = sess->session.qos.index;
    ret = fd_msg_avp_setvalue (avpch2, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    /* Allocation-Retention-Priority */
    ret = fd_msg_avp_new(
            ogs_diam_gy_allocation_retention_priority, 0, &avpch2);
    ogs_assert(ret == 0);

    /* Priority-Level */
    ret = fd_msg_avp_new(ogs_diam_gy_priority_level, 0, &avpch3);
    ogs_assert(ret == 0);
    val.u32 = sess->session.qos.arp.priority_level;
    ret = fd_msg_avp_setvalue (avpch3, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
    ogs_assert(ret == 0);

    /* Pre-emption-Capability */
    ret = fd_msg_avp_new(ogs_diam_gy_pre_emption_capability, 0, &avpch3);
    ogs_assert(ret == 0);
    val.u32 = sess->session.qos.arp.pre_emption_capability;
    ret = fd_msg_avp_setvalue (avpch3, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
    ogs_assert(ret == 0);

    /* Pre-emption-Vulnerability */
    ret = fd_msg_avp_new(ogs_diam_gy_pre_emption_vulnerability, 0, &avpch3);
    ogs_assert(ret == 0);
    val.u32 = sess->session.qos.arp.pre_emption_vulnerability;
    ret = fd_msg_avp_setvalue (avpch3, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avpch2, MSG_BRW_LAST_CHILD, avpch3);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    if (sess->session.ambr.uplink) {
        ret = fd_msg_avp_new(ogs_diam_gy_apn_aggregate_max_bitrate_ul,
                0, &avpch2);
        ogs_assert(ret == 0);
        val.u32 = sess->session.ambr.uplink;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    }
    if (sess->session.ambr.downlink) {
        ret = fd_msg_avp_new(
                ogs_diam_gy_apn_aggregate_max_bitrate_dl, 0, &avpch2);
        ogs_assert(ret == 0);
        val.u32 = sess->session.ambr.downlink;
        ret = fd_msg_avp_setvalue (avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    }
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

    /* 3GPP-RAT-Type, TS 29.061 16.4.7.2 21 */
    /* GGSN: TS 29.060 7.7.50, PGW: TS 29.274 8.17 */
    ret = fd_msg_avp_new(ogs_diam_gy_3gpp_rat_type, 0, &avpch1);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t*)&sess->gtp_rat_type;
    val.os.len = 1;
    ret = fd_msg_avp_setvalue (avpch1, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

    /* Multiple Services AVP add to req: */
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);
}

/* TS 32.299 7.2.192 Service-Information AVP for CCR */
static void fill_service_information_ccr(smf_sess_t *sess,
                                         uint32_t cc_request_type, struct msg *req)
{
    int ret;
    union avp_value val;
    struct avp *avp;
    struct avp *avpch1, *avpch2, *avpch3;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
    char buf[OGS_PLMNIDSTRLEN];
    char digit;

    /* Service-Information, TS 32.299 sec 7.2.192 */
    ret = fd_msg_avp_new(ogs_diam_gy_service_information, 0, &avp);

    /* PS-Information, TS 32.299 sec 7.2.158 */
    ret = fd_msg_avp_new(ogs_diam_gy_ps_information, 0, &avpch1);

    /* 3GPP-Charging-Id, 3GPP TS 29.061 16.4.7.2 2 */
    ret = fd_msg_avp_new(ogs_diam_gy_3gpp_charging_id, 0, &avpch2);
    ogs_assert(ret == 0);
    val.u32 = sess->charging.id;
    ret = fd_msg_avp_setvalue(avpch2, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    /* 3GPP-PDP-Type, 3GPP TS 29.061 16.4.7.2 3 */
    if (cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_INITIAL_REQUEST) {
        ret = fd_msg_avp_new(ogs_diam_gy_3gpp_pdp_type, 0, &avpch2);
        ogs_assert(ret == 0);
        switch (sess->session.session_type) {
        case OGS_PDU_SESSION_TYPE_IPV4:
            val.i32 = OGS_DIAM_GY_3GPP_PDP_TYPE_IPv4;
            break;
        case OGS_PDU_SESSION_TYPE_IPV6:
            val.i32 = OGS_DIAM_GY_3GPP_PDP_TYPE_IPv6;
            break;
        case OGS_PDU_SESSION_TYPE_IPV4V6:
            val.i32 = OGS_DIAM_GY_3GPP_PDP_TYPE_IPv4v6;
            break;
        case OGS_PDU_SESSION_TYPE_UNSTRUCTURED:
            val.i32 = OGS_DIAM_GY_3GPP_PDP_TYPE_UNSTRUCTURED;
            break;
        case OGS_PDU_SESSION_TYPE_ETHERNET:
            val.i32 = OGS_DIAM_GY_3GPP_PDP_TYPE_ETHERNET;
            break;
        default:
            val.i32 = OGS_DIAM_GY_3GPP_PDP_TYPE_NON_IP;
        }
        ret = fd_msg_avp_setvalue(avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    }

    /* PDP-Address, TS 32.299 7.2.137 */
    if (sess->ipv4) {
        ret = fd_msg_avp_new(ogs_diam_gy_pdp_address, 0, &avpch2);
        sin.sin_family = AF_INET;
        memcpy(&sin.sin_addr.s_addr, (uint8_t*)&sess->ipv4->addr[0], OGS_IPV4_LEN);
        ret = fd_msg_avp_value_encode(&sin, avpch2);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
    }
    if (sess->ipv6) {
        ret = fd_msg_avp_new(ogs_diam_gy_pdp_address, 0, &avpch2);
        sin6.sin6_family = AF_INET6;
        memcpy(&sin6.sin6_addr.s6_addr, (uint8_t*)&sess->ipv6->addr[0], OGS_IPV6_LEN);
        ret = fd_msg_avp_value_encode(&sin6, avpch2);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        /* PDP-Address-Prefix-Length, TS 32.299 7.2.137 */
        /* TODO: not yet needed since used OGS_IPV6_DEFAULT_PREFIX_LEN is 64.
        if (OGS_IPV6_DEFAULT_PREFIX_LEN != 64) {
            .u32 = sess->ipv6->subnet->prefixlen;
        }
        */
    }

    /* SGSN-Address */
    if (sess->sgw_s5c_ip.ipv4) {
        ret = fd_msg_avp_new(ogs_diam_gy_sgsn_address, 0, &avpch2);
        sin.sin_family = AF_INET;
        memcpy(&sin.sin_addr.s_addr, (uint8_t*)&sess->sgw_s5c_ip.addr, OGS_IPV4_LEN);
        ret = fd_msg_avp_value_encode(&sin, avpch2);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    }
    if (sess->sgw_s5c_ip.ipv6) {
        ret = fd_msg_avp_new(ogs_diam_gy_sgsn_address, 0, &avpch2);
        sin6.sin6_family = AF_INET6;
        memcpy(&sin6.sin6_addr.s6_addr, (uint8_t*)&sess->sgw_s5c_ip.addr6[0], OGS_IPV6_LEN);
        ret = fd_msg_avp_value_encode(&sin6, avpch2);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    }

    /* GGSN-Address */
    if (ogs_gtp_self()->gtpc_addr) {
        ret = fd_msg_avp_new(ogs_diam_gy_ggsn_address, 0, &avpch2);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_value_encode(&ogs_gtp_self()->gtpc_addr->sin, avpch2);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    }
    if (ogs_gtp_self()->gtpc_addr6) {
        ret = fd_msg_avp_new(ogs_diam_gy_ggsn_address, 0, &avpch2);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_value_encode(&ogs_gtp_self()->gtpc_addr->sin6, avpch2);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    }

    /* Called-Station-Id */
    ret = fd_msg_avp_new(ogs_diam_gy_called_station_id, 0, &avpch2);
    ogs_assert(ret == 0);
    ogs_assert(sess->session.name);
    val.os.data = (uint8_t*)sess->session.name;
    val.os.len = strlen(sess->session.name);
    ret = fd_msg_avp_setvalue(avpch2, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    /* 3GPP-Selection-Mode, 3GPP TS 29.061 16.4.7.2 12 */
    ret = fd_msg_avp_new(ogs_diam_gy_3gpp_selection_mode, 0, &avpch2);
    ogs_assert(ret == 0);
    digit = sess->gtp.selection_mode + '0';
    val.os.data = (uint8_t*)&digit;
    val.os.len = 1;
    ret = fd_msg_avp_setvalue(avpch2, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    /* 3GPP-Charging-Characteristics, 3GPP TS 29.061 16.4.7.2 13 */
    if (sess->gtp.charging_characteristics.presence &&
        sess->gtp.charging_characteristics.len > 0) {
        uint8_t oct1, oct2;
        char digits[5];
        ret = fd_msg_avp_new(ogs_diam_gy_3gpp_charging_characteristics, 0, &avpch2);
        ogs_assert(ret == 0);
        oct1 = ((uint8_t*)sess->gtp.charging_characteristics.data)[0];
        oct2 = (sess->gtp.charging_characteristics.len > 1) ?
                        ((uint8_t*)sess->gtp.charging_characteristics.data)[1] : 0;
        snprintf(digits, sizeof(digits), "%02x%02x", oct1, oct2);
        val.os.data = (uint8_t*)&digits[0];
        val.os.len = 4;
        ret = fd_msg_avp_setvalue(avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    }

    /* 3GPP-SGSN-MCC-MNC */
    ret = fd_msg_avp_new(ogs_diam_gy_3gpp_sgsn_mcc_mnc, 0, &avpch2);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)ogs_plmn_id_to_string(&sess->plmn_id, buf);
    val.os.len = strlen(buf);
    ret = fd_msg_avp_setvalue(avpch2, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
    ogs_assert(ret == 0);

    /* 3GPP-NSAPI, 3GPP TS 29.061 16.4.7.2 10 */
    if (sess->gtp.version == 1) {
        char nsapi_hex;
        if (sess->gtp.v1.nsapi <= 9)
            nsapi_hex = sess->gtp.v1.nsapi + '0';
        else
            nsapi_hex = sess->gtp.v1.nsapi - 10 + 'A';
        ret = fd_msg_avp_new(ogs_diam_gy_3gpp_nsapi, 0, &avpch2);
        ogs_assert(ret == 0);
        val.os.data = (uint8_t *)&nsapi_hex;
        val.os.len = 1;
        ret = fd_msg_avp_setvalue(avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    } else {
        /* GTPv2C: For PGW, EPS Bearer ID as specified in 3GPP TS 29.274 8.8 */
        smf_bearer_t *bearer = smf_default_bearer_in_sess(sess);
        ret = fd_msg_avp_new(ogs_diam_gy_3gpp_nsapi, 0, &avpch2);
        ogs_assert(ret == 0);
        val.os.data = (uint8_t *)&bearer->ebi;
        val.os.len = 1;
        ret = fd_msg_avp_setvalue(avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    }

    /* 3GPP-MS-Timezone */
    if (sess->gtp.ue_timezone.presence &&
            sess->gtp.ue_timezone.len && sess->gtp.ue_timezone.data) {
        ret = fd_msg_avp_new(ogs_diam_gy_3gpp_ms_timezone, 0, &avpch2);
        ogs_assert(ret == 0);
        val.os.data = sess->gtp.ue_timezone.data;
        val.os.len = sess->gtp.ue_timezone.len;
        ret = fd_msg_avp_setvalue(avpch2, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    }

    /* 3GPP-User-Location-Info, 3GPP TS 29.061 16.4.7.2 22 */
    smf_fd_msg_avp_add_3gpp_uli(sess, avpch1);

    if (sess->smf_ue->imeisv_len > 0) {
        /* User-Equipment-Info, 3GPP TS 32.299 7.1.17 */
        ret = fd_msg_avp_new(ogs_diam_gy_user_equipment_info, 0, &avpch2);

        /* User-Equipment-Info-Type 0 (IMEI) */
        ret = fd_msg_avp_new(ogs_diam_gy_user_equipment_info_type, 0, &avpch3);
        ogs_assert(ret == 0);
        val.i32 = 0;
        ret = fd_msg_avp_setvalue(avpch3, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch2, MSG_BRW_LAST_CHILD, avpch3);
        ogs_assert(ret == 0);

        /* User-Equipment-Info-Val */
        ret = fd_msg_avp_new(ogs_diam_gy_user_equipment_info_value, 0, &avpch3);
        ogs_assert(ret == 0);
        digit = '0';
        val.os.data = (uint8_t*)&sess->smf_ue->imeisv_bcd[0];
        val.os.len = 16;
        ret = fd_msg_avp_setvalue(avpch3, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avpch2, MSG_BRW_LAST_CHILD, avpch3);
        ogs_assert(ret == 0);

        /* User-Equipment-Info AVP add to PS-Information: */
        ret = fd_msg_avp_add (avpch1, MSG_BRW_LAST_CHILD, avpch2);
        ogs_assert(ret == 0);
    }

    /* PS-Information AVP add to req: */
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

    /* Service-Information AVP add to req: */
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);
}

/* 3GPP TS 32.299  6.4.2 Credit-Control-Request message */
void smf_gy_send_ccr(smf_sess_t *sess, void *xact,
        uint32_t cc_request_type)
{
    int ret;
    smf_ue_t *smf_ue = NULL;

    struct msg *req = NULL;
    struct avp *avp;
    struct avp *avpch1;
    union avp_value val;
    struct sess_state *sess_data = NULL, *svg;
    struct session *session = NULL;
    int new;
    const char *service_context_id = "open5gs-smfd@open5gs.org";
    uint32_t timestamp;

    ogs_assert(xact);
    ogs_assert(sess);

    ogs_assert(sess->ipv4 || sess->ipv6);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_debug("[Gy][Credit-Control-Request]");

    /* Create the request */
    ret = fd_msg_new(ogs_diam_gy_cmd_ccr, MSGFL_ALLOC_ETEID, &req);
    ogs_assert(ret == 0);
    {
        struct msg_hdr *h;
        ret = fd_msg_hdr(req, &h);
        ogs_assert(ret == 0);
        h->msg_appl = OGS_DIAM_GY_APPLICATION_ID;
    }

    /* Find Diameter Gy Session */
    if (sess->gy_sid) {
        /* Retrieve session by Session-Id */
        size_t sidlen = strlen(sess->gy_sid);
		ret = fd_sess_fromsid_msg((os0_t)sess->gy_sid, sidlen, &session, &new);
        ogs_assert(ret == 0);
        ogs_assert(new == 0);

        ogs_debug("    Found Gy Session-Id: [%s]", sess->gy_sid);

        /* Add Session-Id to the message */
        ret = ogs_diam_message_session_id_set(req, (os0_t)sess->gy_sid, sidlen);
        ogs_assert(ret == 0);
        /* Save the session associated with the message */
        ret = fd_msg_sess_set(req, session);
    } else {
        /* Create a new session */
        #define OGS_DIAM_GY_APP_SID_OPT  "app_gy"
        ret = fd_msg_new_session(req, (os0_t)OGS_DIAM_GY_APP_SID_OPT,
                CONSTSTRLEN(OGS_DIAM_GY_APP_SID_OPT));
        ogs_assert(ret == 0);
        ret = fd_msg_sess_get(fd_g_config->cnf_dict, req, &session, NULL);
        ogs_assert(ret == 0);
        ogs_debug("    Create a New Gy Session");
    }

    /* Retrieve session state in this session */
    ret = fd_sess_state_retrieve(smf_gy_reg, session, &sess_data);
    if (!sess_data) {
        os0_t sid;
        size_t sidlen;

        ret = fd_sess_getsid(session, &sid, &sidlen);
        ogs_assert(ret == 0);

        /* Allocate new session state memory */
        sess_data = new_state(sid);
        ogs_assert(sess_data);

        ogs_debug("    Allocate new Gy session: [%s]", sess_data->gy_sid);

        /* Save Session-Id to SMF Session Context */
        sess->gy_sid = (char *)sess_data->gy_sid;
    } else
        ogs_debug("    Retrieve Gy session: [%s]", sess_data->gy_sid);

    /*
     * 8.2.  CC-Request-Number AVP
     *
     *  The CC-Request-Number AVP (AVP Code 415) is of type Unsigned32 and
     *  identifies this request within one session.  As Session-Id AVPs are
     * globally unique, the combination of Session-Id and CC-Request-Number
     * AVPs is also globally unique and can be used in matching credit-
     * control messages with confirmations.  An easy way to produce unique
     * numbers is to set the value to 0 for a credit-control request of type
     * INITIAL_REQUEST and EVENT_REQUEST and to set the value to 1 for the
     * first UPDATE_REQUEST, to 2 for the second, and so on until the value
     * for TERMINATION_REQUEST is one more than for the last UPDATE_REQUEST.
     */

    sess_data->cc_request_type = cc_request_type;
    if (cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_INITIAL_REQUEST ||
        cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_EVENT_REQUEST)
        sess_data->cc_request_number = 0;
    else
        sess_data->cc_request_number++;

    ogs_debug("    CC Request Type[%d] Number[%d]",
        sess_data->cc_request_type, sess_data->cc_request_number);
    ogs_assert(sess_data->cc_request_number <= MAX_CC_REQUEST_NUMBER);

    /* Update session state */
    sess_data->sess = sess;
    if (cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_UPDATE_REQUEST)
        sess_data->xact_data[sess_data->cc_request_number].pfcp = true;
    else
        sess_data->xact_data[sess_data->cc_request_number].pfcp = false;
    sess_data->xact_data[sess_data->cc_request_number].ptr = xact;

    /* Origin-Host & Origin-Realm */
    ret = fd_msg_add_origin(req, 0);
    ogs_assert(ret == 0);

    /* the Destination-Realm AVP */
    ret = fd_msg_avp_new(ogs_diam_destination_realm, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (unsigned char *)(fd_g_config->cnf_diamrlm);
    val.os.len = strlen(fd_g_config->cnf_diamrlm);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_application_id, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_GY_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Service-Context-Id */
    ret = fd_msg_avp_new(ogs_diam_service_context_id, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (unsigned char *)service_context_id;
    val.os.len = strlen(service_context_id);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* CC-Request-Type, CC-Request-Number */
    ret = fd_msg_avp_new(ogs_diam_gy_cc_request_type, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = sess_data->cc_request_type;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_gy_cc_request_number, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = sess_data->cc_request_number;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Destination-Host AVP */
    if (sess_data->peer_host) {
        ret = fd_msg_avp_new(ogs_diam_destination_host, 0, &avp);
        ogs_assert(ret == 0);
        val.os.data = sess_data->peer_host;
        val.os.len  = strlen((char *)sess_data->peer_host);
        ret = fd_msg_avp_setvalue(avp, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);
    }

    /* User-Name */
#if 0
    ret = fd_msg_avp_new(ogs_diam_user_name, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = ?;
    val.os.len  = strlen(?);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);
#endif

    /* Origin-State-Id */
#if 0
    /* TODO: implement restart counter */
    ret = fd_msg_avp_new(ogs_diam_gy_origin_state_id, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = ?;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);
#endif

    /* Event-Timestamp (rfc6733 8.21, type in 4.3.1) */
    ret = fd_msg_avp_new(ogs_diam_event_timestamp, 0, &avp);
    ogs_assert(ret == 0);
    timestamp = htobe32(ogs_time_ntp32_now());
    val.os.data = (unsigned char *)&timestamp;
    val.os.len  = 4;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Subscription-Id (IMSI) */
    ret = fd_msg_avp_new(ogs_diam_subscription_id, 0, &avp);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_subscription_id_type, 0, &avpch1);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_SUBSCRIPTION_ID_TYPE_END_USER_IMSI;
    ret = fd_msg_avp_setvalue (avpch1, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_subscription_id_data, 0, &avpch1);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)smf_ue->imsi_bcd;
    val.os.len = strlen(smf_ue->imsi_bcd);
    ret = fd_msg_avp_setvalue (avpch1, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Subscription-Id (MSISDN) */
    if (smf_ue->msisdn_len > 0) {
        ret = fd_msg_avp_new(ogs_diam_subscription_id, 0, &avp);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(ogs_diam_subscription_id_type, 0, &avpch1);
        ogs_assert(ret == 0);
        val.i32 = OGS_DIAM_SUBSCRIPTION_ID_TYPE_END_USER_E164;
        ret = fd_msg_avp_setvalue (avpch1, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(ogs_diam_subscription_id_data, 0, &avpch1);
        ogs_assert(ret == 0);
        val.os.data = (uint8_t *)smf_ue->msisdn_bcd;
        val.os.len = strlen(smf_ue->msisdn_bcd);
        ret = fd_msg_avp_setvalue (avpch1, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add (avp, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);
    }

    /* Termination-Cause */
    if (cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_TERMINATION_REQUEST) {
        ret = fd_msg_avp_new(ogs_diam_termination_cause, 0, &avp);
        ogs_assert(ret == 0);
        val.i32 = OGS_DIAM_TERMINATION_CAUSE_DIAMETER_LOGOUT; /* TODO: set specific cause */
        ret = fd_msg_avp_setvalue(avp, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);
    }

    /* Requested-Action */
    ret = fd_msg_avp_new(ogs_diam_gy_requested_action, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_GY_REQUESTED_ACTION_DIRECT_DEBITING;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* AoC-Request-Type */
    ret = fd_msg_avp_new(ogs_diam_gy_aoc_request_type, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_GY_AoC_FULL;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Multiple-Services-Indicator */
    if (cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_INITIAL_REQUEST) {
        ret = fd_msg_avp_new(ogs_diam_gy_multiple_services_ind, 0, &avp);
        ogs_assert(ret == 0);
        val.i32 = OGS_DIAM_GY_MULTIPLE_SERVICES_NOT_SUPPORTED;
        ret = fd_msg_avp_setvalue(avp, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);
    }

    /* TS 32.299 7.1.9 Multiple-Services-Credit-Control AVP */
    fill_multiple_services_credit_control_ccr(sess, cc_request_type, req);

    /* OC-Supported-Features */

    /* Service-Information */
    fill_service_information_ccr(sess, cc_request_type, req);


    ret = clock_gettime(CLOCK_REALTIME, &sess_data->ts);

    /* Keep a pointer to the session data for debug purpose,
     * in real life we would not need it */
    svg = sess_data;

    /* Store this value in the session */
    ret = fd_sess_state_store(smf_gy_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);

    /* Send the request */
    ret = fd_msg_send(&req, smf_gy_cca_cb, svg);
    ogs_assert(ret == 0);

    /* Increment the counter */
    ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
    ogs_diam_logger_self()->stats.nb_sent++;
    ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);
}

static void smf_gy_cca_cb(void *data, struct msg **msg)
{
    int rv;
    int ret;

    struct sess_state *sess_data = NULL;
    struct timespec ts;
    struct session *session;
    struct avp *avp, *avpch1;
    struct avp_hdr *hdr;
    unsigned long dur;
    int error = 0;
    int new;
    struct msg *req = NULL;
    smf_event_t *e = NULL;
    void *xact = NULL;
    smf_sess_t *sess = NULL;
    ogs_diam_gy_message_t *gy_message = NULL;
    uint32_t cc_request_number = 0;

    ogs_debug("[Gy][Credit-Control-Answer]");

    ret = clock_gettime(CLOCK_REALTIME, &ts);
    ogs_assert(ret == 0);

    /* Get originating request of received message, if any */
    ret = fd_msg_answ_getq(*msg, &req);
    ogs_assert(ret == 0);

    /* Search the session, retrieve its data */
    ret = fd_msg_sess_get(fd_g_config->cnf_dict, *msg, &session, &new);
    ogs_assert(ret == 0);
    ogs_assert(new == 0);

    ogs_debug("    Search the Gy session");

    ret = fd_sess_state_retrieve(smf_gy_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data);
    ogs_assert((void *)sess_data == data);

    ogs_debug("    Retrieve its data: [%s]", sess_data->gy_sid);

    /* Value of CC-Request-Number */
    ret = fd_msg_search_avp(*msg, ogs_diam_gy_cc_request_number, &avp);
    ogs_assert(ret == 0);
    if (!avp && req) {
        /* Attempt searching for CC-Request-* in original request. Error
         * messages (like DIAMETER_UNABLE_TO_DELIVER) crafted internally may not
         * have them. */
        ret = fd_msg_search_avp(req, ogs_diam_gy_cc_request_number, &avp);
        ogs_assert(ret == 0);
    }
    if (!avp) {
        ogs_error("no_CC-Request-Number");
        ogs_assert_if_reached();
    }
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);
    cc_request_number = hdr->avp_value->i32;

    ogs_debug("    CC-Request-Number[%d]", cc_request_number);

    xact = sess_data->xact_data[cc_request_number].ptr;
    ogs_assert(xact);
    sess = sess_data->sess;
    ogs_assert(sess);

    gy_message = ogs_calloc(1, sizeof(ogs_diam_gy_message_t));
    ogs_assert(gy_message);

    /* Set Credit Control Command */
    gy_message->cmd_code = OGS_DIAM_GY_CMD_CODE_CREDIT_CONTROL;

    /* Value of Result Code */
    ret = fd_msg_search_avp(*msg, ogs_diam_result_code, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        gy_message->result_code = hdr->avp_value->i32;
        gy_message->err = &gy_message->result_code;
        ogs_debug("    Result Code: %d", hdr->avp_value->i32);
    } else {
        ret = fd_msg_search_avp(*msg, ogs_diam_experimental_result, &avp);
        ogs_assert(ret == 0);
        if (avp) {
            ret = fd_avp_search_avp(
                    avp, ogs_diam_experimental_result_code, &avpch1);
            ogs_assert(ret == 0);
            if (avpch1) {
                ret = fd_msg_avp_hdr(avpch1, &hdr);
                ogs_assert(ret == 0);
                gy_message->result_code = hdr->avp_value->i32;
                gy_message->exp_err = &gy_message->result_code;
                ogs_debug("    Experimental Result Code: %d",
                        gy_message->result_code);
            }
        } else {
            ogs_error("no Result-Code");
            error++;
        }
    }

    /* Value of Origin-Host */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_host, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("    From '%.*s'",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Host");
        error++;
    }

    /* Value of Origin-Realm */
    ret = fd_msg_search_avp(*msg, ogs_diam_origin_realm, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        ogs_debug("         ('%.*s')",
                (int)hdr->avp_value->os.len, hdr->avp_value->os.data);
    } else {
        ogs_error("no_Origin-Realm");
        error++;
    }

    /* Value of CC-Request-Type */
    ret = fd_msg_search_avp(*msg, ogs_diam_gy_cc_request_type, &avp);
    ogs_assert(ret == 0);
    if (!avp && req) {
        /* Attempt searching for CC-Request-* in original request. Error
         * messages (like DIAMETER_UNABLE_TO_DELIVER) crafted internally may not
         * have them. */
        ret = fd_msg_search_avp(req, ogs_diam_gy_cc_request_type, &avp);
        ogs_assert(ret == 0);
    }
    if (!avp) {
        ogs_error("no_CC-Request-Number");
        error++;
    }
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);
    gy_message->cc_request_type = hdr->avp_value->i32;

    if (gy_message->result_code != ER_DIAMETER_SUCCESS) {
        ogs_warn("ERROR DIAMETER Result Code(%d)", gy_message->result_code);
        goto out;
    }

    ret = fd_msg_browse(*msg, MSG_BRW_FIRST_CHILD, &avp, NULL);
    ogs_assert(ret == 0);
    while (avp) {
        ret = fd_msg_avp_hdr(avp, &hdr);
        ogs_assert(ret == 0);
        switch (hdr->avp_code) {
        case AC_SESSION_ID:
        case AC_ORIGIN_HOST:
            if (sess_data->peer_host)
                ogs_free(sess_data->peer_host);
            sess_data->peer_host =
                (os0_t)ogs_strdup((char *)hdr->avp_value->os.data);
            ogs_assert(sess_data->peer_host);
            break;
        case AC_ORIGIN_REALM:
        case AC_DESTINATION_REALM:
        case AC_RESULT_CODE:
        case AC_ROUTE_RECORD:
        case AC_PROXY_INFO:
        case AC_AUTH_APPLICATION_ID:
            break;
        case OGS_DIAM_GY_AVP_CODE_CC_REQUEST_TYPE:
        case OGS_DIAM_GY_AVP_CODE_CC_REQUEST_NUMBER:
        case OGS_DIAM_GY_AVP_CODE_SUPPORTED_FEATURES:
            break;
        case OGS_DIAM_GY_AVP_CODE_MULTIPLE_SERVICES_CREDIT_CONTROL:
            ret = fd_msg_browse(avp, MSG_BRW_FIRST_CHILD, &avpch1, NULL);
            ogs_assert(ret == 0);
            while (avpch1) {
                ret = fd_msg_avp_hdr(avpch1, &hdr);
                ogs_assert(ret == 0);
                switch (hdr->avp_code) {
                case OGS_DIAM_GY_AVP_CODE_GRANTED_SERVICE_UNIT:
                    rv = decode_granted_service_unit(
                            &gy_message->cca.granted, avpch1, &error);
                    ogs_assert(rv == OGS_OK);
                    /*TODO: apply gsu */
                    break;
                case OGS_DIAM_GY_AVP_CODE_VALIDITY_TIME:
                    gy_message->cca.validity_time = hdr->avp_value->u32;
                    break;
                case OGS_DIAM_GY_AVP_CODE_TIME_QUOTA_THRESHOLD:
                    gy_message->cca.time_threshold = hdr->avp_value->u32;
                    break;
                case OGS_DIAM_GY_AVP_CODE_VOLUME_QUOTA_THRESHOLD:
                    gy_message->cca.volume_threshold = hdr->avp_value->u32;
                    break;
                default:
                    ogs_warn("Not supported(%d)", hdr->avp_code);
                    break;
                }
                fd_msg_browse(avpch1, MSG_BRW_NEXT, &avpch1, NULL);
            }
            break;
        default:
            ogs_warn("Not supported(%d)", hdr->avp_code);
            break;
        }
        fd_msg_browse(avp, MSG_BRW_NEXT, &avp, NULL);
    }

out:
    if (!error) {
        e = smf_event_new(SMF_EVT_GY_MESSAGE);
        ogs_assert(e);

        e->sess = sess;
        e->gy_message = gy_message;
        if (gy_message->cc_request_type == OGS_DIAM_GY_CC_REQUEST_TYPE_UPDATE_REQUEST) {
            ogs_assert(sess_data->xact_data[sess_data->cc_request_number].pfcp == true);
            e->pfcp_xact = xact;
        } else {
            ogs_assert(sess_data->xact_data[sess_data->cc_request_number].pfcp == false);
            e->gtp_xact = xact;
        }
        rv = ogs_queue_push(ogs_app()->queue, e);
        if (rv != OGS_OK) {
            ogs_warn("ogs_queue_push() failed:%d", (int)rv);
            ogs_free(gy_message);
            smf_event_free(e);
        } else {
            ogs_pollset_notify(ogs_app()->pollset);
        }
    } else {
        ogs_free(gy_message);
    }

    /* Free the message */
    ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
    dur = ((ts.tv_sec - sess_data->ts.tv_sec) * 1000000) +
        ((ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    if (ogs_diam_logger_self()->stats.nb_recv) {
        /* Ponderate in the avg */
        ogs_diam_logger_self()->stats.avg = (ogs_diam_logger_self()->stats.avg *
            ogs_diam_logger_self()->stats.nb_recv + dur) /
            (ogs_diam_logger_self()->stats.nb_recv + 1);
        /* Min, max */
        if (dur < ogs_diam_logger_self()->stats.shortest)
            ogs_diam_logger_self()->stats.shortest = dur;
        if (dur > ogs_diam_logger_self()->stats.longest)
            ogs_diam_logger_self()->stats.longest = dur;
    } else {
        ogs_diam_logger_self()->stats.shortest = dur;
        ogs_diam_logger_self()->stats.longest = dur;
        ogs_diam_logger_self()->stats.avg = dur;
    }
    if (error)
        ogs_diam_logger_self()->stats.nb_errs++;
    else
        ogs_diam_logger_self()->stats.nb_recv++;

    ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);

    /* Display how long it took */
    if (ts.tv_nsec > sess_data->ts.tv_nsec)
        ogs_trace("in %d.%06ld sec",
                (int)(ts.tv_sec - sess_data->ts.tv_sec),
                (long)(ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);
    else
        ogs_trace("in %d.%06ld sec",
                (int)(ts.tv_sec + 1 - sess_data->ts.tv_sec),
                (long)(1000000000 + ts.tv_nsec - sess_data->ts.tv_nsec) / 1000);

    ogs_debug("    CC-Request-Type[%d] Number[%d] in Session Data",
        sess_data->cc_request_type, sess_data->cc_request_number);
    ogs_debug("    Current CC-Request-Number[%d]", cc_request_number);
    if (sess_data->cc_request_type ==
            OGS_DIAM_GY_CC_REQUEST_TYPE_TERMINATION_REQUEST &&
        sess_data->cc_request_number <= cc_request_number) {
        ogs_debug("    [LAST] state_cleanup(): [%s]", sess_data->gy_sid);
        state_cleanup(sess_data, NULL, NULL);
    } else {
        ogs_debug("    fd_sess_state_store(): [%s]", sess_data->gy_sid);
        ret = fd_sess_state_store(smf_gy_reg, session, &sess_data);
        ogs_assert(ret == 0);
        ogs_assert(sess_data == NULL);
    }

    ret = fd_msg_free(*msg);
    ogs_assert(ret == 0);
    *msg = NULL;

    return;
}

static int smf_gy_fb_cb(struct msg **msg, struct avp *avp,
        struct session *sess, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	ogs_warn("Unexpected message received!");

	return ENOTSUP;
}

static int smf_gy_rar_cb( struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    int rv;
    int ret;

	struct msg *ans;
    union avp_value val;
    struct sess_state *sess_data = NULL;

    smf_event_t *e = NULL;
    smf_sess_t *sess = NULL;
    ogs_diam_gy_message_t *gy_message = NULL;

    uint32_t result_code = OGS_DIAM_UNKNOWN_SESSION_ID;

    ogs_assert(msg);

    ogs_debug("Re-Auth-Request");

    gy_message = ogs_calloc(1, sizeof(ogs_diam_gy_message_t));
    ogs_assert(gy_message);

    /* Set Credit Control Command */
    gy_message->cmd_code = OGS_DIAM_GY_CMD_RE_AUTH;

	/* Create answer header */
	ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ogs_assert(ret == 0);
    ans = *msg;

    ret = fd_sess_state_retrieve(smf_gy_reg, session, &sess_data);
    ogs_assert(ret == 0);
    if (!sess_data) {
        ogs_error("No Session Data");
        goto out;
    }

    /* Get Session Information */
    sess = sess_data->sess;
    ogs_assert(sess);

    /* TODO: parsing of msg into gy_message */

    /* Send Gy Event to SMF State Machine */
    e = smf_event_new(SMF_EVT_GY_MESSAGE);
    ogs_assert(e);

    e->sess = sess;
    e->gy_message = gy_message;
    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        ogs_free(gy_message);
        smf_event_free(e);
    } else {
        ogs_pollset_notify(ogs_app()->pollset);
    }

    /* Set the Auth-Application-Id AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_application_id, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_GY_APPLICATION_ID;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	ret = fd_msg_rescode_set(ans, (char *)"DIAMETER_SUCCESS", NULL, NULL, 1);
    ogs_assert(ret == 0);

    /* Store this value in the session */
    ret = fd_sess_state_store(smf_gy_reg, session, &sess_data);
    ogs_assert(ret == 0);
    ogs_assert(sess_data == NULL);

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    ogs_debug("Re-Auth-Answer");

	/* Add this value to the stats */
	ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
	ogs_diam_logger_self()->stats.nb_echoed++;
	ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);

    return 0;

out:
    if (result_code == OGS_DIAM_UNKNOWN_SESSION_ID) {
        ret = fd_msg_rescode_set(ans,
                    (char *)"DIAMETER_UNKNOWN_SESSION_ID", NULL, NULL, 1);
        ogs_assert(ret == 0);
    } else {
        ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
        ogs_assert(ret == 0);
    }

    /* Store this value in the session */
    ret = fd_sess_state_store(smf_gy_reg, session, &sess_data);
    ogs_assert(sess_data == NULL);

    ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    ogs_free(gy_message);

    return 0;
}

int smf_gy_init(void)
{
    int ret;
	struct disp_when data;

    ogs_thread_mutex_init(&sess_state_mutex);
    ogs_pool_init(&sess_state_pool, ogs_app()->pool.sess);

	/* Install objects definitions for this application */
	ret = ogs_diam_gy_init();
    ogs_assert(ret == 0);

    /* Create handler for sessions */
	ret = fd_sess_handler_create(&smf_gy_reg, state_cleanup, NULL, NULL);
    ogs_assert(ret == 0);

	memset(&data, 0, sizeof(data));
	data.app = ogs_diam_gy_application;

	ret = fd_disp_register(smf_gy_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_gy_fb);
    ogs_assert(ret == 0);

	data.command = ogs_diam_gy_cmd_rar;
	ret = fd_disp_register(smf_gy_rar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_gy_rar);
    ogs_assert(ret == 0);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(ogs_diam_gy_application, NULL, 1, 0);
    ogs_assert(ret == 0);

	return OGS_OK;
}

void smf_gy_final(void)
{
    int ret;

	ret = fd_sess_handler_destroy(&smf_gy_reg, NULL);
    ogs_assert(ret == 0);

	if (hdl_gy_fb)
		(void) fd_disp_unregister(&hdl_gy_fb, NULL);
	if (hdl_gy_rar)
		(void) fd_disp_unregister(&hdl_gy_rar, NULL);

    ogs_pool_final(&sess_state_pool);
    ogs_thread_mutex_destroy(&sess_state_mutex);
}

static int decode_granted_service_unit(
        ogs_diam_gy_service_unit_t *su, struct avp *avpch1, int *perror)
{
    int ret = 0, error = 0;
    struct avp *avpch2;
    struct avp_hdr *hdr;

    ogs_assert(su);
    ogs_assert(avpch1);
    memset(su, 0, sizeof(*su));

    ret = fd_msg_browse(avpch1, MSG_BRW_FIRST_CHILD, &avpch2, NULL);
    ogs_assert(ret == 0);
    while (avpch2) {
        ret = fd_msg_avp_hdr(avpch2, &hdr);
        ogs_assert(ret == 0);
        switch (hdr->avp_code) {
        case OGS_DIAM_GY_AVP_CODE_CC_TIME:
            su->cc_time_present = true;
            su->cc_time = hdr->avp_value->u32;
            break;
        case OGS_DIAM_GY_AVP_CODE_CC_TOTAL_OCTETS:
            su->cc_total_octets_present = true;
            su->cc_total_octets = hdr->avp_value->u64;
            break;
        case OGS_DIAM_GY_AVP_CODE_CC_INPUT_OCTETS:
            su->cc_input_octets_present = true;
            su->cc_input_octets = hdr->avp_value->u64;
            break;
        case OGS_DIAM_GY_AVP_CODE_CC_OUTPUT_OCTETS:
            su->cc_output_octets_present = true;
            su->cc_output_octets = hdr->avp_value->u64;
            break;
        default:
            ogs_error("Not implemented(%d)", hdr->avp_code);
            break;
        }
        fd_msg_browse(avpch2, MSG_BRW_NEXT, &avpch2, NULL);
    }

    if (perror)
        *perror = error;

    return OGS_OK;
}
