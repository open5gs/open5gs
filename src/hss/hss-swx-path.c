/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-crypt.h"

#include "hss-context.h"
#include "hss-fd-path.h"

/* handler for fallback cb */
static struct disp_hdl *hdl_swx_fb = NULL;
/* handler for Multimedia-Auth-Request cb */
static struct disp_hdl *hdl_swx_mar = NULL;
/* handler for Server-Assignment-Request cb */
static struct disp_hdl *hdl_swx_sar = NULL;

/* Default callback for the application. */
static int hss_ogs_diam_swx_fb_cb(struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	ogs_warn("Unexpected message received!");
	
	return ENOTSUP;
}

/* Callback for incoming Multimedia-Auth-Request messages */
static int hss_ogs_diam_swx_mar_cb( struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    int rv, ret;
    uint32_t result_code = 0;

	struct msg *ans, *qry;

    struct avp *sip_auth_data_item_avp = NULL;
    struct avp *authentication_scheme_avp = NULL;
    struct avp *sip_authorization_avp = NULL;

    struct avp *avpch = NULL;
    struct avp_hdr *hdr;
    union avp_value val;

    char *user_name = NULL;
    char *authentication_scheme = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    ogs_dbi_auth_info_t auth_info;
    uint8_t zero[OGS_RAND_LEN];

    uint8_t authenticate[OGS_KEY_LEN*2];

    uint8_t opc[OGS_KEY_LEN];
    uint8_t sqn[OGS_SQN_LEN];

    uint8_t autn[OGS_AUTN_LEN];
    uint8_t ik[OGS_KEY_LEN];
    uint8_t ck[OGS_KEY_LEN];
    uint8_t ak[OGS_AK_LEN];
    uint8_t xres[OGS_MAX_RES_LEN];
    size_t xres_len = 8;

    uint8_t mac_s[OGS_MAC_S_LEN];

    ogs_assert(msg);

    ogs_debug("Multimedia-Auth-Request");

	/* Create answer header */
	qry = *msg;
	ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ogs_assert(ret == 0);
    ans = *msg;

    /* Get User-Name AVP (Mandatory) */
    ret = fd_msg_search_avp(qry, ogs_diam_user_name, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);

    user_name = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    ogs_assert(user_name);

    ogs_extract_digit_from_string(imsi_bcd, user_name);

    /* Get the SIP-Auth-Data-Item AVP (Mandatory) */
    ret = fd_msg_search_avp(
            qry, ogs_diam_cx_sip_auth_data_item, &sip_auth_data_item_avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(sip_auth_data_item_avp, &hdr);
    ogs_assert(ret == 0);

    /* Get the Authentication-Scheme AVP */
    ret = fd_msg_search_avp(sip_auth_data_item_avp,
            ogs_diam_cx_sip_authentication_scheme, &authentication_scheme_avp);
    ogs_assert(ret == 0);
    if (authentication_scheme_avp) {
        ret = fd_msg_avp_hdr(authentication_scheme_avp, &hdr);
        ogs_assert(ret == 0);

        authentication_scheme = ogs_strndup(
            (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
        ogs_assert(authentication_scheme);
    }

    /* EAP-AKA is only supported */
    if (authentication_scheme &&
        ogs_strcasecmp(authentication_scheme,
            OGS_DIAM_SWX_AUTH_SCHEME_EAP_AKA) != 0) {
        ogs_error("Authentication-Scheme[%s] is not supported",
                    authentication_scheme);
        result_code = OGS_DIAM_CX_ERROR_AUTH_SCHEME_NOT_SUPPORTED;
        goto out;
    }

    /* DB : HSS Auth-Info */
    rv = hss_db_auth_info(imsi_bcd, &auth_info);
    if (rv != OGS_OK) {
        ogs_error("Cannot get IMS-Data for IMSI:'%s'", imsi_bcd);
        result_code = OGS_DIAM_CX_ERROR_USER_UNKNOWN;
        goto out;
    }

    memset(zero, 0, sizeof(zero));
    if (memcmp(auth_info.rand, zero, OGS_RAND_LEN) == 0) {
        ogs_random(auth_info.rand, OGS_RAND_LEN);
    }

    if (auth_info.use_opc)
        memcpy(opc, auth_info.opc, sizeof(opc));
    else
        milenage_opc(auth_info.k, auth_info.op, opc);

    /* Get the SIP-Authorization AVP */
    ret = fd_msg_search_avp(sip_auth_data_item_avp,
            ogs_diam_cx_sip_authorization, &sip_authorization_avp);
    ogs_assert(ret == 0);
    if (sip_authorization_avp) {
        ret = fd_msg_avp_hdr(sip_authorization_avp, &hdr);
        ogs_assert(ret == 0);

        ogs_auc_sqn(opc, auth_info.k,
                hdr->avp_value->os.data,
                hdr->avp_value->os.data + OGS_RAND_LEN,
                sqn, mac_s);
        if (memcmp(mac_s, hdr->avp_value->os.data +
                    OGS_RAND_LEN + OGS_SQN_LEN, OGS_MAC_S_LEN) == 0) {
            ogs_random(auth_info.rand, OGS_RAND_LEN);
            auth_info.sqn = ogs_buffer_to_uint64(sqn, OGS_SQN_LEN);
            /* 33.102 C.3.4 Guide : IND + 1 */
            auth_info.sqn = (auth_info.sqn + 32 + 1) & OGS_MAX_SQN;
        } else {
            ogs_error("Re-synch MAC failed for IMSI:`%s`", imsi_bcd);
            ogs_log_print(OGS_LOG_ERROR, "MAC_S: ");
            ogs_log_hexdump(OGS_LOG_ERROR, mac_s, OGS_MAC_S_LEN);
            ogs_log_hexdump(OGS_LOG_ERROR,
                (void*)(hdr->avp_value->os.data +
                    OGS_RAND_LEN + OGS_SQN_LEN),
                OGS_MAC_S_LEN);
            ogs_log_print(OGS_LOG_ERROR, "SQN: ");
            ogs_log_hexdump(OGS_LOG_ERROR, sqn, OGS_SQN_LEN);
            result_code = OGS_DIAM_CX_ERROR_AUTH_SCHEME_NOT_SUPPORTED;
            goto out;
        }
    }

    rv = hss_db_update_sqn(imsi_bcd, auth_info.rand, auth_info.sqn);
    if (rv != OGS_OK) {
        ogs_error("Cannot update rand and sqn for IMSI:'%s'", imsi_bcd);
        result_code = OGS_DIAM_CX_ERROR_IN_ASSIGNMENT_TYPE;
        goto out;
    }

    rv = hss_db_increment_sqn(imsi_bcd);
    if (rv != OGS_OK) {
        ogs_error("Cannot increment sqn for IMSI:'%s'", imsi_bcd);
        result_code = OGS_DIAM_CX_ERROR_IN_ASSIGNMENT_TYPE;
        goto out;
    }

    milenage_generate(opc, auth_info.amf, auth_info.k,
        ogs_uint64_to_buffer(auth_info.sqn, OGS_SQN_LEN, sqn), auth_info.rand,
        autn, ik, ck, ak, xres, &xres_len);

    memcpy(authenticate, auth_info.rand, OGS_RAND_LEN);
    memcpy(authenticate + OGS_RAND_LEN, autn, OGS_AUTN_LEN);

    ogs_log_print(OGS_LOG_DEBUG, "K - ");
    ogs_log_hexdump(OGS_LOG_DEBUG, auth_info.k, OGS_KEY_LEN);
    ogs_log_print(OGS_LOG_DEBUG, "AMF - ");
    ogs_log_hexdump(OGS_LOG_DEBUG, auth_info.amf, OGS_AMF_LEN);
    ogs_log_print(OGS_LOG_DEBUG, "OPc - ");
    ogs_log_hexdump(OGS_LOG_DEBUG, opc, OGS_KEY_LEN);
    ogs_log_print(OGS_LOG_DEBUG, "RAND - ");
    ogs_log_hexdump(OGS_LOG_DEBUG, auth_info.rand, OGS_RAND_LEN);
    ogs_log_print(OGS_LOG_DEBUG, "SQN - ");
    ogs_log_hexdump(OGS_LOG_DEBUG, sqn, OGS_SQN_LEN);

    ogs_log_print(OGS_LOG_DEBUG, "AUTN - ");
    ogs_log_hexdump(OGS_LOG_DEBUG, autn, OGS_KEY_LEN);
    ogs_log_print(OGS_LOG_DEBUG, "ck - ");
    ogs_log_hexdump(OGS_LOG_DEBUG, ck, OGS_KEY_LEN);
    ogs_log_print(OGS_LOG_DEBUG, "ik - ");
    ogs_log_hexdump(OGS_LOG_DEBUG, ik, OGS_KEY_LEN);
    ogs_log_print(OGS_LOG_DEBUG, "ak - ");
    ogs_log_hexdump(OGS_LOG_DEBUG, ak, OGS_KEY_LEN);
    ogs_log_print(OGS_LOG_DEBUG, "xles - ");
    ogs_log_hexdump(OGS_LOG_DEBUG, xres, xres_len);

    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_SWX_APPLICATION_ID);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	/* Set the Origin-Host, Origin-Realm, and Result-Code AVPs */
	ret = fd_msg_rescode_set(ans, (char*)"DIAMETER_SUCCESS", NULL, NULL, 1);
    ogs_assert(ret == 0);

    /* Set the User-Name AVP */
    ret = fd_msg_avp_new(ogs_diam_user_name, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)user_name;
    val.os.len = strlen(user_name);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the SIP-Number-Auth-Items AVP */
    ret = fd_msg_avp_new(ogs_diam_cx_sip_number_auth_items, 0, &avp);
    ogs_assert(ret == 0);
    val.u32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the SIP-Auth-Data-Item AVP */
    ret = fd_msg_avp_new(ogs_diam_cx_sip_auth_data_item, 0, &avp);
    ogs_assert(ret == 0);

    /* Set the SIP-Item-Number AVP */
    ret = fd_msg_avp_new(ogs_diam_cx_sip_item_number, 0, &avpch);
    ogs_assert(ret == 0);
    val.u32 = 1;
    ret = fd_msg_avp_setvalue(avpch, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
    ogs_assert(ret == 0);

    /* Set the SIP-Authentication-Scheme AVP */
    ret = fd_msg_avp_new(ogs_diam_cx_sip_authentication_scheme, 0, &avpch);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)OGS_DIAM_SWX_AUTH_SCHEME_EAP_AKA;
    val.os.len = strlen(OGS_DIAM_SWX_AUTH_SCHEME_EAP_AKA);
    ret = fd_msg_avp_setvalue(avpch, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
    ogs_assert(ret == 0);

    /* Set the SIP-Authenticatie AVP */
    ret = fd_msg_avp_new(ogs_diam_cx_sip_authenticate, 0, &avpch);
    ogs_assert(ret == 0);
    val.os.data = authenticate;
    val.os.len = OGS_KEY_LEN * 2;
    ret = fd_msg_avp_setvalue(avpch, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
    ogs_assert(ret == 0);

    /* Set the SIP-Authorization AVP */
    ret = fd_msg_avp_new(ogs_diam_cx_sip_authorization, 0, &avpch);
    ogs_assert(ret == 0);
    val.os.data = xres;
    val.os.len = xres_len;
    ret = fd_msg_avp_setvalue(avpch, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
    ogs_assert(ret == 0);

    /* Set the Confidentiality-Key AVP */
    ret = fd_msg_avp_new(ogs_diam_cx_confidentiality_key, 0, &avpch);
    ogs_assert(ret == 0);
    val.os.data = ck;
    val.os.len = OGS_KEY_LEN;
    ret = fd_msg_avp_setvalue(avpch, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
    ogs_assert(ret == 0);

    /* Set the Integirty-Key AVP */
    ret = fd_msg_avp_new(ogs_diam_cx_integrity_key, 0, &avpch);
    ogs_assert(ret == 0);
    val.os.data = ik;
    val.os.len = OGS_KEY_LEN;
    ret = fd_msg_avp_setvalue(avpch, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    ogs_debug("Multimedia-Auth-Answer");

	/* Add this value to the stats */
	ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
	ogs_diam_logger_self()->stats.nb_echoed++;
	ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);

    if (authentication_scheme)
        ogs_free(authentication_scheme);

    ogs_free(user_name);

	return 0;

out:
    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_SWX_APPLICATION_ID);
    ogs_assert(ret == 0);

	/* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
    ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    if (authentication_scheme)
        ogs_free(authentication_scheme);

    ogs_free(user_name);

    return 0;
}

/* Callback for incoming Server-Assignment-Request messages */
static int hss_ogs_diam_swx_sar_cb( struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    int rv, ret;
    uint32_t result_code = 0;

	struct msg *ans, *qry;

    struct avp_hdr *hdr;
    union avp_value val;

    char *user_name = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    ogs_subscription_data_t subscription_data;
    ogs_slice_data_t *slice_data = NULL;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;

    ogs_assert(msg);

    ogs_debug("Server-Assignment-Request");

    memset(&subscription_data, 0, sizeof(ogs_subscription_data_t));

	/* Create answer header */
	qry = *msg;
	ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ogs_assert(ret == 0);
    ans = *msg;

    /* Get User-Name AVP (Mandatory) */
    ret = fd_msg_search_avp(qry, ogs_diam_user_name, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);

    user_name = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    ogs_assert(user_name);

    ogs_extract_digit_from_string(imsi_bcd, user_name);

    /* DB : HSS Subscription Data */
    rv = hss_db_subscription_data(imsi_bcd, &subscription_data);
    if (rv != OGS_OK) {
        ogs_error("Cannot get Subscription-Data for IMSI:'%s'", imsi_bcd);
        result_code = OGS_DIAM_S6A_ERROR_USER_UNKNOWN;
        goto out;
    }

    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_SWX_APPLICATION_ID);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	/* Set the Origin-Host, Origin-Realm, and Result-Code AVPs */
	ret = fd_msg_rescode_set(ans, (char*)"DIAMETER_SUCCESS", NULL, NULL, 1);
    ogs_assert(ret == 0);

    /* Set the User-Name AVP */
    ret = fd_msg_avp_new(ogs_diam_user_name, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)user_name;
    val.os.len = strlen(user_name);
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Get Server-Assignment-Type AVP (Mandatory) */
    ret = fd_msg_search_avp(qry,
            ogs_diam_cx_server_assignment_type, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);
    if (hdr->avp_value->i32 == OGS_DIAM_CX_SERVER_ASSIGNMENT_REGISTRATION ||
        hdr->avp_value->i32 ==
        OGS_DIAM_CX_SERVER_ASSIGNMENT_AAA_USER_DATA_REQUEST) {

        struct avp *avp_subscription_id;
        struct avp *avp_subscription_id_type, *avp_subscription_id_data;
        struct avp *avp_non_3gpp_ip_access, *avp_non_3gpp_ip_access_apn;
        struct avp *avp_ambr, *avp_max_bandwidth_ul, *avp_max_bandwidth_dl;

        /* Set the APN Configuration Profile */
        struct avp *context_identifier;

        int i;

        /* Set the Non-3GPP-User-Data */
        ret = fd_msg_avp_new(ogs_diam_swx_non_3gpp_user_data, 0, &avp);
        ogs_assert(ret == 0);

        /* Set Subscription-Id */
        if (subscription_data.num_of_msisdn >= 1)  {
            ret = fd_msg_avp_new(ogs_diam_subscription_id, 0,
                    &avp_subscription_id);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_new(ogs_diam_subscription_id_type, 0,
                    &avp_subscription_id_type);
            ogs_assert(ret == 0);
            val.i32 = OGS_DIAM_SUBSCRIPTION_ID_TYPE_END_USER_E164;
            ret = fd_msg_avp_setvalue (avp_subscription_id_type, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(avp_subscription_id, MSG_BRW_LAST_CHILD,
                    avp_subscription_id_type);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_new(ogs_diam_subscription_id_data, 0,
                    &avp_subscription_id_data);
            ogs_assert(ret == 0);
            val.os.data = (uint8_t *)subscription_data.msisdn[0].bcd;
            val.os.len = strlen(subscription_data.msisdn[0].bcd);
            ret = fd_msg_avp_setvalue (avp_subscription_id_data, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add (avp_subscription_id, MSG_BRW_LAST_CHILD,
                    avp_subscription_id_data);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_subscription_id);
            ogs_assert(ret == 0);
        }

        /* Set Non-3GPP-IP-Access */
        ret = fd_msg_avp_new(ogs_diam_swx_non_3gpp_ip_access, 0,
                &avp_non_3gpp_ip_access);
        ogs_assert(ret == 0);
        val.i32 = OGS_DIAM_SWX_NON_3GPP_SUBSCRIPTION_ALLOWED;
        ret = fd_msg_avp_setvalue(avp_non_3gpp_ip_access, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_non_3gpp_ip_access);
        ogs_assert(ret == 0);

        /* Set Non-3GPP-IP-Access-APN */
        ret = fd_msg_avp_new(ogs_diam_swx_non_3gpp_ip_access_apn, 0,
                &avp_non_3gpp_ip_access_apn);
        ogs_assert(ret == 0);
        val.i32 = OGS_DIAM_SWX_NON_3GPP_APNS_ENABLE;
        ret = fd_msg_avp_setvalue(avp_non_3gpp_ip_access_apn, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD,
                avp_non_3gpp_ip_access_apn);
        ogs_assert(ret == 0);

        /* Set the AMBR */
        ret = fd_msg_avp_new(ogs_diam_s6a_ambr, 0, &avp_ambr);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_new(
                ogs_diam_s6a_max_bandwidth_ul, 0, &avp_max_bandwidth_ul);
        ogs_assert(ret == 0);
        val.u32 = ogs_uint64_to_uint32(subscription_data.ambr.uplink);
        ret = fd_msg_avp_setvalue(avp_max_bandwidth_ul, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(
                avp_ambr, MSG_BRW_LAST_CHILD, avp_max_bandwidth_ul);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_new(
                ogs_diam_s6a_max_bandwidth_dl, 0, &avp_max_bandwidth_dl);
        ogs_assert(ret == 0);
        val.u32 = ogs_uint64_to_uint32(subscription_data.ambr.downlink);
        ret = fd_msg_avp_setvalue(avp_max_bandwidth_dl, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(
                avp_ambr, MSG_BRW_LAST_CHILD, avp_max_bandwidth_dl);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_ambr);
        ogs_assert(ret == 0);

        /* For EPC, we'll use first Slice in Subscription */
        if (subscription_data.num_of_slice)
            slice_data = &subscription_data.slice[0];

        if (!slice_data) {
            ogs_error("[%s] Cannot find S-NSSAI", imsi_bcd);
            result_code = OGS_DIAM_S6A_ERROR_UNKNOWN_EPS_SUBSCRIPTION;
            goto out;
        }

        if (!slice_data->num_of_session) {
            ogs_error("[%s] No PDN", imsi_bcd);
            result_code = OGS_DIAM_S6A_ERROR_UNKNOWN_EPS_SUBSCRIPTION;
            goto out;
        }

        ret = fd_msg_avp_new(ogs_diam_s6a_context_identifier, 0,
                &context_identifier);
        ogs_assert(ret == 0);
        val.i32 = 1; /* Context Identifier : 1 */
        ret = fd_msg_avp_setvalue(context_identifier, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, context_identifier);
        ogs_assert(ret == 0);

        for (i = 0; i < slice_data->num_of_session; i++) {
            /* Set the APN Configuration */
            struct avp *apn_configuration, *context_identifier, *pdn_type;
            struct avp *served_party_ip_address, *service_selection;
            struct avp *eps_subscribed_qos_profile, *qos_class_identifier;
            struct avp *allocation_retention_priority, *priority_level;
            struct avp *pre_emption_capability, *pre_emption_vulnerability;
            struct avp *mip6_agent_info, *mip_home_agent_address;
            struct avp *pdn_gw_allocation_type;
            struct avp *vplmn_dynamic_address_allowed;

            ogs_session_t *session = NULL;

            if (i >= OGS_MAX_NUM_OF_SESS) {
                ogs_warn("Ignore max session count overflow [%d>=%d]",
                    slice_data->num_of_session, OGS_MAX_NUM_OF_SESS);
                break;
            }

            session = &slice_data->session[i];
            ogs_assert(session);
            session->context_identifier = i+1;

            ret = fd_msg_avp_new(ogs_diam_s6a_apn_configuration, 0,
                &apn_configuration);
            ogs_assert(ret == 0);

            /* Set Context-Identifier */
            ret = fd_msg_avp_new(ogs_diam_s6a_context_identifier, 0,
                    &context_identifier);
            ogs_assert(ret == 0);
            val.i32 = session->context_identifier;
            ret = fd_msg_avp_setvalue(context_identifier, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(apn_configuration,
                    MSG_BRW_LAST_CHILD, context_identifier);
            ogs_assert(ret == 0);

            /* Set PDN-Type */
            ret = fd_msg_avp_new(ogs_diam_s6a_pdn_type, 0, &pdn_type);
            ogs_assert(ret == 0);
            val.i32 = OGS_PDU_SESSION_TYPE_TO_DIAMETER(session->session_type);
            ret = fd_msg_avp_setvalue(pdn_type, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(apn_configuration,
                    MSG_BRW_LAST_CHILD, pdn_type);
            ogs_assert(ret == 0);

            /* Set Served-Party-IP-Address */
            if ((session->session_type == OGS_PDU_SESSION_TYPE_IPV4 ||
                 session->session_type == OGS_PDU_SESSION_TYPE_IPV4V6) &&
                session->ue_ip.ipv4) {
                ret = fd_msg_avp_new(ogs_diam_s6a_served_party_ip_address,
                        0, &served_party_ip_address);
                ogs_assert(ret == 0);
                sin.sin_family = AF_INET;
                sin.sin_addr.s_addr = session->ue_ip.addr;
                ret = fd_msg_avp_value_encode(&sin, served_party_ip_address);
                ogs_assert(ret == 0);
                ret = fd_msg_avp_add(apn_configuration, MSG_BRW_LAST_CHILD,
                        served_party_ip_address);
                ogs_assert(ret == 0);
            }

            if ((session->session_type == OGS_PDU_SESSION_TYPE_IPV6 ||
                 session->session_type == OGS_PDU_SESSION_TYPE_IPV4V6) &&
                session->ue_ip.ipv6) {
                ret = fd_msg_avp_new(ogs_diam_s6a_served_party_ip_address,
                        0, &served_party_ip_address);
                ogs_assert(ret == 0);
                sin6.sin6_family = AF_INET6;
                memcpy(sin6.sin6_addr.s6_addr,
                        session->ue_ip.addr6, OGS_IPV6_LEN);
                ret = fd_msg_avp_value_encode(&sin6, served_party_ip_address);
                ogs_assert(ret == 0);
                ret = fd_msg_avp_add(apn_configuration, MSG_BRW_LAST_CHILD,
                        served_party_ip_address);
                ogs_assert(ret == 0);
            }

            /* Set Service-Selection */
            ret = fd_msg_avp_new(ogs_diam_service_selection, 0,
                    &service_selection);
            ogs_assert(ret == 0);
            ogs_assert(session->name);
            val.os.data = (uint8_t *)session->name;
            val.os.len = strlen(session->name);
            ret = fd_msg_avp_setvalue(service_selection, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(apn_configuration,
                    MSG_BRW_LAST_CHILD, service_selection);
            ogs_assert(ret == 0);

            /* Set the EPS Subscribed QoS Profile */
            ret = fd_msg_avp_new(ogs_diam_s6a_eps_subscribed_qos_profile, 0,
                    &eps_subscribed_qos_profile);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_new(ogs_diam_s6a_qos_class_identifier, 0,
                    &qos_class_identifier);
            ogs_assert(ret == 0);
            val.i32 = session->qos.index;
            ret = fd_msg_avp_setvalue(qos_class_identifier, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(eps_subscribed_qos_profile,
                    MSG_BRW_LAST_CHILD, qos_class_identifier);
            ogs_assert(ret == 0);

                    /* Set Allocation retention priority */
            ret = fd_msg_avp_new(ogs_diam_s6a_allocation_retention_priority, 0,
                    &allocation_retention_priority);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_new(
                    ogs_diam_s6a_priority_level, 0, &priority_level);
            ogs_assert(ret == 0);
            val.u32 = session->qos.arp.priority_level;
            ret = fd_msg_avp_setvalue(priority_level, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(allocation_retention_priority,
                MSG_BRW_LAST_CHILD, priority_level);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_new(ogs_diam_s6a_pre_emption_capability, 0,
                    &pre_emption_capability);
            ogs_assert(ret == 0);
            val.u32 = OGS_EPC_PRE_EMPTION_DISABLED;
            if (session->qos.arp.pre_emption_capability ==
                    OGS_5GC_PRE_EMPTION_ENABLED)
                val.u32 = OGS_EPC_PRE_EMPTION_ENABLED;
            ret = fd_msg_avp_setvalue(pre_emption_capability, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(allocation_retention_priority,
                MSG_BRW_LAST_CHILD, pre_emption_capability);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_new(ogs_diam_s6a_pre_emption_vulnerability, 0,
                    &pre_emption_vulnerability);
            ogs_assert(ret == 0);
            val.u32 = OGS_EPC_PRE_EMPTION_DISABLED;
            if (session->qos.arp.pre_emption_vulnerability ==
                    OGS_5GC_PRE_EMPTION_ENABLED)
                val.u32 = OGS_EPC_PRE_EMPTION_ENABLED;
            ret = fd_msg_avp_setvalue(pre_emption_vulnerability, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(allocation_retention_priority,
                MSG_BRW_LAST_CHILD, pre_emption_vulnerability);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_add(eps_subscribed_qos_profile,
                MSG_BRW_LAST_CHILD, allocation_retention_priority);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_add(apn_configuration,
                MSG_BRW_LAST_CHILD, eps_subscribed_qos_profile);
            ogs_assert(ret == 0);

            /* Set MIP6-Agent-Info */
            if (session->smf_ip.ipv4 || session->smf_ip.ipv6) {
                ret = fd_msg_avp_new(ogs_diam_mip6_agent_info, 0,
                            &mip6_agent_info);
                ogs_assert(ret == 0);

                if (session->smf_ip.ipv4) {
                    ret = fd_msg_avp_new(ogs_diam_mip_home_agent_address, 0,
                                &mip_home_agent_address);
                    ogs_assert(ret == 0);
                    sin.sin_family = AF_INET;
                    sin.sin_addr.s_addr = session->smf_ip.addr;
                    ret = fd_msg_avp_value_encode (
                                &sin, mip_home_agent_address );
                    ogs_assert(ret == 0);
                    ret = fd_msg_avp_add(mip6_agent_info,
                            MSG_BRW_LAST_CHILD, mip_home_agent_address);
                    ogs_assert(ret == 0);
                }

                if (session->smf_ip.ipv6) {
                    ret = fd_msg_avp_new(ogs_diam_mip_home_agent_address, 0,
                                &mip_home_agent_address);
                    ogs_assert(ret == 0);
                    sin6.sin6_family = AF_INET6;
                    memcpy(sin6.sin6_addr.s6_addr, session->smf_ip.addr6,
                            sizeof session->smf_ip.addr6);
                    ret = fd_msg_avp_value_encode (
                                &sin6, mip_home_agent_address );
                    ogs_assert(ret == 0);
                    ret = fd_msg_avp_add(mip6_agent_info,
                            MSG_BRW_LAST_CHILD, mip_home_agent_address);
                    ogs_assert(ret == 0);
                }

                ret = fd_msg_avp_add(apn_configuration,
                        MSG_BRW_LAST_CHILD, mip6_agent_info);
                ogs_assert(ret == 0);

                ret = fd_msg_avp_new(ogs_diam_s6a_pdn_gw_allocation_type, 0,
                            &pdn_gw_allocation_type);
                ogs_assert(ret == 0);

                val.u32 = OGS_DIAM_S6A_PDN_GW_ALLOCATION_DYNAMIC;
                ret = fd_msg_avp_setvalue(pdn_gw_allocation_type, &val);
                ogs_assert(ret == 0);

                ret = fd_msg_avp_add(apn_configuration,
                        MSG_BRW_LAST_CHILD, pdn_gw_allocation_type);
                ogs_assert(ret == 0);
            }

            /* Set VPLMN-Dynamic-Address-Allowed */
            ret = fd_msg_avp_new(ogs_diam_s6a_vplmn_dynamic_address_allowed, 0,
                    &vplmn_dynamic_address_allowed);
            ogs_assert(ret == 0);

            val.u32 = OGS_DIAM_S6A_VPLMN_DYNAMIC_ADDRESS_ALLOWED;
            ret = fd_msg_avp_setvalue(vplmn_dynamic_address_allowed, &val);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_add(apn_configuration,
                    MSG_BRW_LAST_CHILD, vplmn_dynamic_address_allowed);
            ogs_assert(ret == 0);

            /* Set AMBR */
            if (session->ambr.downlink || session->ambr.uplink) {
                ret = fd_msg_avp_new(ogs_diam_s6a_ambr, 0, &avp_ambr);
                ogs_assert(ret == 0);
                ret = fd_msg_avp_new(ogs_diam_s6a_max_bandwidth_ul, 0,
                            &avp_max_bandwidth_ul);
                ogs_assert(ret == 0);
                val.u32 = ogs_uint64_to_uint32(session->ambr.uplink);
                ret = fd_msg_avp_setvalue(avp_max_bandwidth_ul, &val);
                ogs_assert(ret == 0);
                ret = fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD,
                            avp_max_bandwidth_ul);
                ogs_assert(ret == 0);
                ret = fd_msg_avp_new(ogs_diam_s6a_max_bandwidth_dl, 0,
                            &avp_max_bandwidth_dl);
                ogs_assert(ret == 0);
                val.u32 = ogs_uint64_to_uint32(session->ambr.downlink);
                ret = fd_msg_avp_setvalue(avp_max_bandwidth_dl, &val);
                ogs_assert(ret == 0);
                ret = fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD,
                            avp_max_bandwidth_dl);
                ogs_assert(ret == 0);

                ret = fd_msg_avp_add(apn_configuration,
                        MSG_BRW_LAST_CHILD, avp_ambr);
                ogs_assert(ret == 0);
            }

            ret = fd_msg_avp_add(avp,
                    MSG_BRW_LAST_CHILD, apn_configuration);
            ogs_assert(ret == 0);
        }

        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);
    }

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    ogs_debug("Server-Assignment-Answer");

	/* Add this value to the stats */
	ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
	ogs_diam_logger_self()->stats.nb_echoed++;
	ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);

    ogs_subscription_data_free(&subscription_data);
    ogs_free(user_name);

	return 0;

out:
    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_SWX_APPLICATION_ID);
    ogs_assert(ret == 0);

	/* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
    ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    ogs_subscription_data_free(&subscription_data);
    ogs_free(user_name);

    return 0;
}

int hss_swx_init(void)
{
    int ret;
	struct disp_when data;

	/* Install objects definitions for this application */
	ret = ogs_diam_swx_init();
    ogs_assert(ret == 0);

	memset(&data, 0, sizeof(data));
	data.app = ogs_diam_swx_application;
	
	/* Fallback CB if command != unexpected message received */
	ret = fd_disp_register(hss_ogs_diam_swx_fb_cb, DISP_HOW_APPID,
                                &data, NULL, &hdl_swx_fb);
    ogs_assert(ret == 0);
	
	/* Specific handler for Multimedia-Auth-Request */
	data.command = ogs_diam_cx_cmd_mar;
	ret = fd_disp_register(hss_ogs_diam_swx_mar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_swx_mar);
    ogs_assert(ret == 0);

	/* Specific handler for Server-Assignment-Request */
	data.command = ogs_diam_cx_cmd_sar;
	ret = fd_disp_register(hss_ogs_diam_swx_sar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_swx_sar);
    ogs_assert(ret == 0);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(ogs_diam_swx_application, ogs_diam_vendor, 1, 0);
    ogs_assert(ret == 0);

	return OGS_OK;
}

void hss_swx_final(void)
{
	if (hdl_swx_fb)
		(void) fd_disp_unregister(&hdl_swx_fb, NULL);
	if (hdl_swx_mar)
		(void) fd_disp_unregister(&hdl_swx_mar, NULL);
	if (hdl_swx_sar)
		(void) fd_disp_unregister(&hdl_swx_sar, NULL);
}
