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
static struct disp_hdl *hdl_cx_fb = NULL;
/* handler for User-Authorization-Request cb */
static struct disp_hdl *hdl_cx_uar = NULL;
/* handler for Multimedia-Auth-Request cb */
static struct disp_hdl *hdl_cx_mar = NULL;
/* handler for Server-Assignment-Request cb */
static struct disp_hdl *hdl_cx_sar = NULL;

static char *download_user_data(
        char *user_name, ogs_subscription_data_t *subscription_data);

/* Default callback for the application. */
static int hss_ogs_diam_cx_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *session, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	ogs_warn("Unexpected message received!");
	
	return ENOTSUP;
}

/* Callback for incoming User-Authorization-Request messages */
static int hss_ogs_diam_cx_uar_cb( struct msg **msg, struct avp *avp, 
        struct session *session, void *opaque, enum disp_action *act)
{
    int rv, ret;
    uint32_t result_code = 0;

	struct msg *ans, *qry;

    struct avp_hdr *hdr;
    union avp_value val;

    char *user_name = NULL;
    char *public_identity = NULL;
    char *server_name = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];
    char msisdn_bcd[OGS_MAX_MSISDN_BCD_LEN+1];

    ogs_subscription_data_t subscription_data;
	
    ogs_assert(msg);

    ogs_debug("User-Authorization-Request");
	
	/* Create answer header */
	qry = *msg;
	ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ogs_assert(ret == 0);
    ans = *msg;

    /* Get User-Name AVP (Mandatory has already been checked) */
    ret = fd_msg_search_avp(qry, ogs_diam_user_name, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);

    user_name = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    ogs_assert(user_name);

    ogs_extract_digit_from_string(imsi_bcd, user_name);

    /* Get Public-Identity AVP (Mandatory has already been checked) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_public_identity, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);

    public_identity = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    ogs_assert(public_identity);

    ogs_extract_digit_from_string(msisdn_bcd, public_identity);

    memset(&subscription_data, 0, sizeof(ogs_subscription_data_t));
    rv = hss_db_subscription_data(imsi_bcd, &subscription_data);
    if (rv != OGS_OK) {
        ogs_error("Cannot get Subscription-Data for IMSI:'%s'", imsi_bcd);
        result_code = OGS_DIAM_CX_ERROR_USER_UNKNOWN;
        goto out;
    }

    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_CX_APPLICATION_ID);
    ogs_assert(ret == 0);

    /* Associate IMPI(User-Name) + IMPU(Public-Identity) */
    hss_cx_associate_identity(user_name, public_identity);

    /* Get Server-Name for IMPI(User-Name) + IMPU(Public-Identity) */
    server_name = hss_cx_get_server_name(user_name, public_identity);
    if (!server_name)
        result_code = OGS_DIAM_CX_FIRST_REGISTRATION;
    else
        result_code = OGS_DIAM_CX_SUBSEQUENT_REGISTRATION;

	/* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
    ret = ogs_diam_message_experimental_rescode_set(
            ans, result_code);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the Server-Name AVP */
    if (server_name) {
        ret = fd_msg_avp_new(ogs_diam_cx_server_name, 0, &avp);
        ogs_assert(ret == 0);
        val.os.data = (uint8_t *)server_name;
        val.os.len  = strlen(server_name);
        ret = fd_msg_avp_setvalue(avp, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);
    }

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    ogs_debug("User-Authorization-Answer");
	
	/* Add this value to the stats */
	ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
	ogs_diam_logger_self()->stats.nb_echoed++;
	ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);

    ogs_subscription_data_free(&subscription_data);

    ogs_free(user_name);
    ogs_free(public_identity);

	return 0;

out:
    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_CX_APPLICATION_ID);
    ogs_assert(ret == 0);

	/* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
    ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    ogs_subscription_data_free(&subscription_data);

    ogs_free(user_name);
    ogs_free(public_identity);

    return 0;
}

/* Callback for incoming Multimedia-Auth-Request messages */
static int hss_ogs_diam_cx_mar_cb( struct msg **msg, struct avp *avp,
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
    char *public_identity = NULL;
    char *server_name = NULL;
    char *authentication_scheme = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];
    char msisdn_bcd[OGS_MAX_MSISDN_BCD_LEN+1];

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

    /* Get User-Name AVP (Mandatory has already been checked) */
    ret = fd_msg_search_avp(qry, ogs_diam_user_name, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);

    user_name = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    ogs_assert(user_name);

    ogs_extract_digit_from_string(imsi_bcd, user_name);

    /* Get Public-Identity AVP (Mandatory has already been checked) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_public_identity, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);

    public_identity = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    ogs_assert(public_identity);

    ogs_extract_digit_from_string(msisdn_bcd, public_identity);

    /* Get Server-Name AVP (Mandatory has already been checked) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_server_name, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);

    server_name = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    ogs_assert(server_name);

    /* Check if IMPI(User-Name) + IMPU(Public-Identity) is associated */
    bool matched = hss_cx_identity_is_associated(user_name, public_identity);
    if (!matched) {
        ogs_error("User-Name[%s] Public-Identity[%s] is not assocated",
                    user_name, public_identity);
        result_code = OGS_DIAM_CX_ERROR_IDENTITIES_DONT_MATCH;
        goto out;
    }

    /* Get the SIP-Auth-Data-Item AVP (Mandatory has already been checked) */
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

    /* IMS_AKA is only supported */
    if (authentication_scheme &&
        strcmp(authentication_scheme, OGS_DIAM_CX_AUTH_SCHEME_IMS_AKA) != 0 &&
        strcmp(authentication_scheme, OGS_DIAM_CX_AUTH_SCHEME_UNKNOWN) != 0) {
        ogs_error("Authentication-Scheme[%s] is not supported",
                    authentication_scheme);
        result_code = OGS_DIAM_CX_ERROR_AUTH_SCHEME_NOT_SUPPORTED;
        goto out;
    }

    rv = hss_db_auth_info(imsi_bcd, &auth_info);
    if (rv != OGS_OK) {
        ogs_error("Cannot get Subscription-Data for IMSI:'%s'", imsi_bcd);
        result_code = OGS_DIAM_CX_ERROR_USER_UNKNOWN;
        goto out;
    }

    /* Overwrite Server-Name for IMPI(User-Name) + IMPU(Public-Identity) */
    hss_cx_set_server_name(user_name, public_identity, server_name, true);

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

#if 0 /* Test Vector for ipsec_reg.pcapng */
    /*
     * Ki : 8baf473f2f8fd09487cccbd7097c6862
     * OP : 11111111111111111111111111111111
     * OPc : 8E27B6AF0E692E750F32667A3B14605D
     * open5gs SQN : 44254 (dec)
     * Fhoss SQN : 00000000ad25 (hex)
     * RAND : a0944c75ff3c4f0853a2f910aa1f104f
     * AMF : c6e2f46c8f4280007f3bb9b84b7c3ff6
     *
     * New SQN : 000017242898 (hex)
     */

    auth_info.sqn = 0x17242898;
#define RAND "a0944c75ff3c4f0853a2f910aa1f104f"
    OGS_HEX(RAND, strlen(RAND), auth_info.rand);
#endif

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
            ans, OGS_DIAM_CX_APPLICATION_ID);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
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

    /* Set the Public-Identity AVP */
    ret = fd_msg_avp_new(ogs_diam_cx_public_identity, 0, &avp);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)public_identity;
    val.os.len = strlen(public_identity);
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
    val.os.data = (uint8_t *)OGS_DIAM_CX_AUTH_SCHEME_IMS_AKA;
    val.os.len = strlen(OGS_DIAM_CX_AUTH_SCHEME_IMS_AKA);
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
    ogs_free(public_identity);
    ogs_free(server_name);

	return 0;

out:
    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_CX_APPLICATION_ID);
    ogs_assert(ret == 0);

	/* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
    ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    if (authentication_scheme)
        ogs_free(authentication_scheme);

    ogs_free(user_name);
    ogs_free(public_identity);
    ogs_free(server_name);

    return 0;
}

/* Callback for incoming Server-Assignment-Request messages */
static int hss_ogs_diam_cx_sar_cb( struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    int rv, ret;
    uint32_t result_code = 0;

	struct msg *ans, *qry;

    struct avp *avpch = NULL;
    struct avp_hdr *hdr;
    union avp_value val;

    char *user_name = NULL;
    char *public_identity = NULL;
    char *server_name = NULL;
    char *user_data = NULL;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];
    char msisdn_bcd[OGS_MAX_MSISDN_BCD_LEN+1];

    ogs_subscription_data_t subscription_data;

    ogs_assert(msg);

    ogs_debug("Server-Assignment-Request");

	/* Create answer header */
	qry = *msg;
	ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ogs_assert(ret == 0);
    ans = *msg;

    /* Get User-Name AVP (Mandatory has already been checked) */
    ret = fd_msg_search_avp(qry, ogs_diam_user_name, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);

    user_name = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    ogs_assert(user_name);

    ogs_extract_digit_from_string(imsi_bcd, user_name);

    /* Get Public-Identity AVP (Mandatory has already been checked) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_public_identity, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);

    public_identity = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    ogs_assert(public_identity);

    ogs_extract_digit_from_string(msisdn_bcd, public_identity);

    memset(&subscription_data, 0, sizeof(ogs_subscription_data_t));
    rv = hss_db_subscription_data(imsi_bcd, &subscription_data);
    if (rv != OGS_OK) {
        ogs_error("Cannot get Subscription-Data for IMSI:'%s'", imsi_bcd);
        result_code = OGS_DIAM_CX_ERROR_USER_UNKNOWN;
        goto out;
    }

    /* Get Server-Name AVP (Mandatory has already been checked) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_server_name, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);

    server_name = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    ogs_assert(server_name);

    /* Check if IMPI(User-Name) + IMPU(Public-Identity) is associated */
    bool matched = hss_cx_identity_is_associated(user_name, public_identity);
    if (!matched) {
        ogs_error("User-Name[%s] Public-Identity[%s] is not assocated",
                    user_name, public_identity);
        result_code = OGS_DIAM_CX_ERROR_IDENTITIES_DONT_MATCH;
        goto out;
    }

    /* Overwrite Server-Name for IMPI(User-Name) + IMPU(Public-Identity) */
    hss_cx_set_server_name(user_name, public_identity, server_name, true);

    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_CX_APPLICATION_ID);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
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

    /* Get User-Data-Already-Available AVP (Mandatory has already been checked) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_user_data_already_available, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);

    if (hdr->avp_value->i32 == OGS_DIAM_CX_USER_DATA_ALREADY_AVAILABLE) {
        /* Nothing to do */
    } else {
        /* Set the User-Data AVP */
        user_data = download_user_data(user_name, &subscription_data);
        ogs_assert(user_data);

        ret = fd_msg_avp_new(ogs_diam_cx_user_data, 0, &avp);
        ogs_assert(ret == 0);
        val.os.data = (uint8_t *)user_data;
        val.os.len = strlen(user_data);
        ret = fd_msg_avp_setvalue(avp, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);

        /* Set the Charging-Information AVP */
        ret = fd_msg_avp_new(ogs_diam_cx_charging_information, 0, &avp);
        ogs_assert(ret == 0);

        /* Set the Charging-Information AVP */
        ret = fd_msg_avp_new(ogs_diam_cx_charging_information, 0, &avp);
        ogs_assert(ret == 0);

        /* Set the Primary-Charging-Collection-Function-Name AVP */
        ret = fd_msg_avp_new(
                ogs_diam_cx_primary_charging_collection_function_name, 0,
                &avpch);
        ogs_assert(ret == 0);
#define PRIMARY_CHARGING_COLLECTION_FUNCTION_NAME "pcrf"
        val.os.data = (uint8_t *)PRIMARY_CHARGING_COLLECTION_FUNCTION_NAME;
        val.os.len = strlen(PRIMARY_CHARGING_COLLECTION_FUNCTION_NAME);
        ret = fd_msg_avp_setvalue(avpch, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
        ogs_assert(ret == 0);

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

    if (user_data)
        ogs_free(user_data);
    ogs_free(user_name);
    ogs_free(public_identity);
    ogs_free(server_name);

	return 0;

out:
    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_CX_APPLICATION_ID);
    ogs_assert(ret == 0);

	/* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
    ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    ogs_subscription_data_free(&subscription_data);

    if (user_data)
        ogs_free(user_data);
    ogs_free(user_name);
    ogs_free(public_identity);
    ogs_free(server_name);

    return 0;
}

static char *download_user_data(
        char *user_name, ogs_subscription_data_t *subscription_data)
{
    char *user_data = NULL;
    int i;

    ogs_assert(user_name);
    ogs_assert(subscription_data);

    user_data = ogs_strdup(ogs_diam_cx_xml_version);
    ogs_assert(user_data);

    user_data = ogs_mstrcatf(user_data, "%s",
                ogs_diam_cx_xml_ims_subscription_s);
    ogs_assert(user_data);

      user_data = ogs_mstrcatf(user_data, "%s%s%s",
                  ogs_diam_cx_xml_private_id_s,
                  user_name,
                  ogs_diam_cx_xml_private_id_e);
      ogs_assert(user_data);

      user_data = ogs_mstrcatf(user_data, "%s",
                  ogs_diam_cx_xml_service_profile_s);
      ogs_assert(user_data);

        user_data = ogs_mstrcatf(user_data, "%s",
                    ogs_diam_cx_xml_public_id_s);
        ogs_assert(user_data);

          user_data = ogs_mstrcatf(user_data, "%s%s%s",
                      ogs_diam_cx_xml_barring_indication_s,
                      "1",
                      ogs_diam_cx_xml_barring_indication_e);
          ogs_assert(user_data);

          user_data = ogs_mstrcatf(user_data, "%ssip:%s%s",
                      ogs_diam_cx_xml_identity_s,
                      user_name,
                      ogs_diam_cx_xml_identity_e);
          ogs_assert(user_data);

          user_data = ogs_mstrcatf(user_data, "%s",
                      ogs_diam_cx_xml_extension_s);
          ogs_assert(user_data);

            user_data = ogs_mstrcatf(user_data, "%s%s%s",
                        ogs_diam_cx_xml_identity_type_s,
                        "0",
                        ogs_diam_cx_xml_identity_type_e);
            ogs_assert(user_data);

          user_data = ogs_mstrcatf(user_data, "%s",
                      ogs_diam_cx_xml_extension_e);
          ogs_assert(user_data);

        user_data = ogs_mstrcatf(user_data, "%s",
                    ogs_diam_cx_xml_public_id_e);
        ogs_assert(user_data);

        for (i = 0; i < subscription_data->num_of_msisdn; i++) {
            user_data = ogs_mstrcatf(user_data, "%s",
                        ogs_diam_cx_xml_public_id_s);
            ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%stel:%s%s",
                          ogs_diam_cx_xml_identity_s,
                          subscription_data->msisdn[i].bcd,
                          ogs_diam_cx_xml_identity_e);
              ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%s",
                          ogs_diam_cx_xml_extension_s);
              ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s%s%s",
                            ogs_diam_cx_xml_identity_type_s,
                            "0",
                            ogs_diam_cx_xml_identity_type_e);
                ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%s",
                          ogs_diam_cx_xml_extension_e);
              ogs_assert(user_data);

            user_data = ogs_mstrcatf(user_data, "%s",
                        ogs_diam_cx_xml_public_id_e);
            ogs_assert(user_data);
            user_data = ogs_mstrcatf(user_data, "%s",
                        ogs_diam_cx_xml_public_id_s);
            ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%ssip:%s%s",
                          ogs_diam_cx_xml_identity_s,
                          subscription_data->msisdn[i].bcd,
                          ogs_diam_cx_xml_identity_e);
              ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%s",
                          ogs_diam_cx_xml_extension_s);
              ogs_assert(user_data);

                user_data = ogs_mstrcatf(user_data, "%s%s%s",
                            ogs_diam_cx_xml_identity_type_s,
                            "0",
                            ogs_diam_cx_xml_identity_type_e);
                ogs_assert(user_data);

              user_data = ogs_mstrcatf(user_data, "%s",
                          ogs_diam_cx_xml_extension_e);
              ogs_assert(user_data);

            user_data = ogs_mstrcatf(user_data, "%s",
                        ogs_diam_cx_xml_public_id_e);
            ogs_assert(user_data);
        }

      user_data = ogs_mstrcatf(user_data, "%s",
                  ogs_diam_cx_xml_service_profile_e);
      ogs_assert(user_data);

    user_data = ogs_mstrcatf(user_data,
                "%s", ogs_diam_cx_xml_ims_subscription_e);
    ogs_assert(user_data);

    return user_data;
}

int hss_cx_init(void)
{
    int ret;
	struct disp_when data;

	/* Install objects definitions for this application */
	ret = ogs_diam_cx_init();
    ogs_assert(ret == 0);

	memset(&data, 0, sizeof(data));
	data.app = ogs_diam_cx_application;
	
	/* Fallback CB if command != unexpected message received */
	ret = fd_disp_register(hss_ogs_diam_cx_fb_cb, DISP_HOW_APPID,
                                &data, NULL, &hdl_cx_fb);
    ogs_assert(ret == 0);
	
	/* Specific handler for User-Authorization-Request */
	data.command = ogs_diam_cx_cmd_uar;
	ret = fd_disp_register(hss_ogs_diam_cx_uar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_cx_uar);
    ogs_assert(ret == 0);

	/* Specific handler for Multimedia-Auth-Request */
	data.command = ogs_diam_cx_cmd_mar;
	ret = fd_disp_register(hss_ogs_diam_cx_mar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_cx_mar);
    ogs_assert(ret == 0);

	/* Specific handler for Server-Assignment-Request */
	data.command = ogs_diam_cx_cmd_sar;
	ret = fd_disp_register(hss_ogs_diam_cx_sar_cb, DISP_HOW_CC, &data, NULL,
                &hdl_cx_sar);
    ogs_assert(ret == 0);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(ogs_diam_cx_application, ogs_diam_vendor, 1, 0);
    ogs_assert(ret == 0);

	return OGS_OK;
}

void hss_cx_final(void)
{
	if (hdl_cx_fb)
		(void) fd_disp_unregister(&hdl_cx_fb, NULL);
	if (hdl_cx_uar)
		(void) fd_disp_unregister(&hdl_cx_uar, NULL);
	if (hdl_cx_mar)
		(void) fd_disp_unregister(&hdl_cx_mar, NULL);
	if (hdl_cx_sar)
		(void) fd_disp_unregister(&hdl_cx_sar, NULL);
}
