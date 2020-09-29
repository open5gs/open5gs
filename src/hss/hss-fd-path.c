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
static struct disp_hdl *hdl_s6a_fb = NULL; 
/* handler for Authentication-Information-Request cb */
static struct disp_hdl *hdl_s6a_air = NULL; 
/* handler for Update-Location-Request cb */
static struct disp_hdl *hdl_s6a_ulr = NULL; 

/* Default callback for the application. */
static int hss_ogs_diam_s6a_fb_cb(struct msg **msg, struct avp *avp, 
        struct session *session, void *opaque, enum disp_action *act)
{
	/* This CB should never be called */
	ogs_warn("Unexpected message received!");
	
	return ENOTSUP;
}

/* Callback for incoming Authentication-Information-Request messages */
static int hss_ogs_diam_s6a_air_cb( struct msg **msg, struct avp *avp, 
        struct session *session, void *opaque, enum disp_action *act)
{
    int ret;

	struct msg *ans, *qry;
    struct avp *avpch;
    struct avp *avp_e_utran_vector, *avp_xres, *avp_kasme, *avp_rand, *avp_autn;
    struct avp_hdr *hdr;
    union avp_value val;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];
    uint8_t opc[OGS_KEY_LEN];
    uint8_t sqn[OGS_SQN_LEN];
    uint8_t autn[OGS_AUTN_LEN];
    uint8_t ik[OGS_KEY_LEN];
    uint8_t ck[OGS_KEY_LEN];
    uint8_t ak[OGS_AK_LEN];
    uint8_t xres[OGS_MAX_RES_LEN];
    uint8_t kasme[OGS_SHA256_DIGEST_SIZE];
    size_t xres_len = 8;

    uint8_t mac_s[OGS_MAC_S_LEN];

    ogs_dbi_auth_info_t auth_info;
    uint8_t zero[OGS_RAND_LEN];
    int rv;
    uint32_t result_code = 0;
	
    ogs_assert(msg);

    ogs_debug("[HSS] Authentication-Information-Request\n");
	
	/* Create answer header */
	qry = *msg;
	ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ogs_assert(ret == 0);
    ans = *msg;

    ret = fd_msg_search_avp(qry, ogs_diam_user_name, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);
    ogs_cpystrn(imsi_bcd, (char*)hdr->avp_value->os.data, 
        ogs_min(hdr->avp_value->os.len, OGS_MAX_IMSI_BCD_LEN)+1);

    rv = hss_db_auth_info(imsi_bcd, &auth_info);
    if (rv != OGS_OK) {
        result_code = OGS_DIAM_S6A_ERROR_USER_UNKNOWN;
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

    ret = fd_msg_search_avp(qry, ogs_diam_s6a_req_eutran_auth_info, &avp);
    ogs_assert(ret == 0);
    if (avp) {
        ret = fd_avp_search_avp(
                avp, ogs_diam_s6a_re_synchronization_info, &avpch);
        ogs_assert(ret == 0);
        if (avpch) {
            ret = fd_msg_avp_hdr(avpch, &hdr);
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
                result_code = OGS_DIAM_S6A_AUTHENTICATION_DATA_UNAVAILABLE;
                goto out;
            }
        }
    }

    rv = hss_db_update_rand_and_sqn(imsi_bcd, auth_info.rand, auth_info.sqn);
    if (rv != OGS_OK) {
        ogs_error("Cannot update rand and sqn for IMSI:'%s'", imsi_bcd);
        result_code = OGS_DIAM_S6A_AUTHENTICATION_DATA_UNAVAILABLE;
        goto out;
    }

    rv = hss_db_increment_sqn(imsi_bcd);
    if (rv != OGS_OK) {
        ogs_error("Cannot increment sqn for IMSI:'%s'", imsi_bcd);
        result_code = OGS_DIAM_S6A_AUTHENTICATION_DATA_UNAVAILABLE;
        goto out;
    }

    ret = fd_msg_search_avp(qry, ogs_diam_s6a_visited_plmn_id, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);
#if 0  // TODO : check visited_plmn_id
    memcpy(visited_plmn_id, hdr->avp_value->os.data, hdr->avp_value->os.len);
#endif

    milenage_generate(opc, auth_info.amf, auth_info.k,
        ogs_uint64_to_buffer(auth_info.sqn, OGS_SQN_LEN, sqn), auth_info.rand,
        autn, ik, ck, ak, xres, &xres_len);
    ogs_auc_kasme(ck, ik, hdr->avp_value->os.data, sqn, ak, kasme);

    /* Set the Authentication-Info */
    ret = fd_msg_avp_new(ogs_diam_s6a_authentication_info, 0, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_new(ogs_diam_s6a_e_utran_vector, 0, &avp_e_utran_vector);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_s6a_rand, 0, &avp_rand);
    ogs_assert(ret == 0);
    val.os.data = auth_info.rand;
    val.os.len = OGS_KEY_LEN;
    ret = fd_msg_avp_setvalue(avp_rand, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_rand);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_s6a_xres, 0, &avp_xres);
    ogs_assert(ret == 0);
    val.os.data = xres;
    val.os.len = xres_len;
    ret = fd_msg_avp_setvalue(avp_xres, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_xres);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_s6a_autn, 0, &avp_autn);
    ogs_assert(ret == 0);
    val.os.data = autn;
    val.os.len = OGS_AUTN_LEN;
    ret = fd_msg_avp_setvalue(avp_autn, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_autn);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_new(ogs_diam_s6a_kasme, 0, &avp_kasme);
    ogs_assert(ret == 0);
    val.os.data = kasme;
    val.os.len = OGS_SHA256_DIGEST_SIZE;
    ret = fd_msg_avp_setvalue(avp_kasme, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(avp_e_utran_vector, MSG_BRW_LAST_CHILD, avp_kasme);
    ogs_assert(ret == 0);

    ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_e_utran_vector);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	ret = fd_msg_rescode_set(ans, (char*)"DIAMETER_SUCCESS", NULL, NULL, 1);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_S6A_APPLICATION_ID);
    ogs_assert(ret == 0);

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    ogs_debug("[HSS] Authentication-Information-Answer\n");
	
	/* Add this value to the stats */
	ogs_assert(pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
	ogs_diam_logger_self()->stats.nb_echoed++;
	ogs_assert(pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);

	return 0;

out:
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

    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_S6A_APPLICATION_ID);
    ogs_assert(ret == 0);

	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    return 0;
}

/* Callback for incoming Update-Location-Request messages */
static int hss_ogs_diam_s6a_ulr_cb( struct msg **msg, struct avp *avp, 
        struct session *session, void *opaque, enum disp_action *act)
{
    int ret;
	struct msg *ans, *qry;

    struct avp_hdr *hdr;
    union avp_value val;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    int rv;
    uint32_t result_code = 0;
    ogs_subscription_data_t subscription_data;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;

    ogs_assert(msg);

    ogs_debug("[HSS] Update-Location-Request\n");
	
	/* Create answer header */
	qry = *msg;
	ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ogs_assert(ret == 0);
    ans = *msg;

    ret = fd_msg_search_avp(qry, ogs_diam_user_name, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);
    ogs_cpystrn(imsi_bcd, (char*)hdr->avp_value->os.data, 
        ogs_min(hdr->avp_value->os.len, OGS_MAX_IMSI_BCD_LEN)+1);

    rv = hss_db_subscription_data(imsi_bcd, &subscription_data);
    if (rv != OGS_OK) {
        ogs_error("Cannot get Subscription-Data for IMSI:'%s'", imsi_bcd);
        result_code = OGS_DIAM_S6A_ERROR_USER_UNKNOWN;
        goto out;
    }

    ret = fd_msg_search_avp(qry, ogs_diam_s6a_visited_plmn_id, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);
#if 0  // TODO : check visited_plmn_id
    memcpy(visited_plmn_id, hdr->avp_value->os.data, hdr->avp_value->os.len);
#endif

	/* Set the Origin-Host, Origin-Realm, andResult-Code AVPs */
	ret = fd_msg_rescode_set(ans, (char*)"DIAMETER_SUCCESS", NULL, NULL, 1);
    ogs_assert(ret == 0);

    /* Set the Auth-Session-State AVP */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = 1;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Set the ULA Flags */
    ret = fd_msg_avp_new(ogs_diam_s6a_ula_flags, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_S6A_ULA_FLAGS_MME_REGISTERED_FOR_SMS;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    ret = fd_msg_search_avp(qry, ogs_diam_s6a_ulr_flags, &avp);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);
    if (!(hdr->avp_value->u32 & OGS_DIAM_S6A_ULR_SKIP_SUBSCRIBER_DATA)) {
        struct avp *avp_msisdn, *avp_a_msisdn;
        struct avp *avp_access_restriction_data;
        struct avp *avp_subscriber_status, *avp_network_access_mode;
        struct avp *avp_ambr, *avp_max_bandwidth_ul, *avp_max_bandwidth_dl;
        struct avp *avp_rau_tau_timer;
        int i;

        /* Set the Subscription Data */

        ret = fd_msg_avp_new(ogs_diam_s6a_subscription_data, 0, &avp);
        ogs_assert(ret == 0);

        /*
         * TS29.328
         * 6.3.2 MSISDN AVP
         *
         * The MSISDN AVP is of type OctetString.
         * This AVP contains an MSISDN, in international number format
         * as described in ITU-T Rec E.164 [8], encoded as a TBCD-string,
         * i.e. digits from 0 through 9 are encoded 0000 to 1001;
         * 1111 is used as a filler when there is an odd number of digits;
         * bits 8 to 5 of octet n encode digit 2n;
         * bits 4 to 1 of octet n encode digit 2(n-1)+1.
         */
        if (subscription_data.num_of_msisdn >= 1)  {
            ret = fd_msg_avp_new(ogs_diam_s6a_msisdn, 0, &avp_msisdn);
            ogs_assert(ret == 0);
            val.os.data = subscription_data.msisdn[0].buf;
            val.os.len = subscription_data.msisdn[0].len;
            ret = fd_msg_avp_setvalue(avp_msisdn, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_msisdn);
            ogs_assert(ret == 0);
        }

        if (subscription_data.num_of_msisdn >= 2)  {
            ret = fd_msg_avp_new(ogs_diam_s6a_a_msisdn, 0, &avp_a_msisdn);
            ogs_assert(ret == 0);
            val.os.data = subscription_data.msisdn[1].buf;
            val.os.len = subscription_data.msisdn[1].len;
            ret = fd_msg_avp_setvalue(avp_a_msisdn, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_a_msisdn);
            ogs_assert(ret == 0);
        }

        if (subscription_data.access_restriction_data) {
            ret = fd_msg_avp_new(ogs_diam_s6a_access_restriction_data, 0,
                    &avp_access_restriction_data);
            ogs_assert(ret == 0);
            val.i32 = subscription_data.access_restriction_data;
            ret = fd_msg_avp_setvalue( avp_access_restriction_data, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, 
                    avp_access_restriction_data);
            ogs_assert(ret == 0);
        }

        ret = fd_msg_avp_new(
                ogs_diam_s6a_subscriber_status, 0, &avp_subscriber_status);
        ogs_assert(ret == 0);
        val.i32 = subscription_data.subscriber_status;
        ret = fd_msg_avp_setvalue(avp_subscriber_status, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_subscriber_status);
        ogs_assert(ret == 0);

        ret = fd_msg_avp_new(ogs_diam_s6a_network_access_mode, 0, 
                    &avp_network_access_mode);
        ogs_assert(ret == 0);
        val.i32 = subscription_data.network_access_mode;
        ret = fd_msg_avp_setvalue(avp_network_access_mode, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_network_access_mode);
        ogs_assert(ret == 0);

        /* Set the AMBR */
        ret = fd_msg_avp_new(ogs_diam_s6a_ambr, 0, &avp_ambr);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_new(
                ogs_diam_s6a_max_bandwidth_ul, 0, &avp_max_bandwidth_ul);
        ogs_assert(ret == 0);
        val.u32 = subscription_data.ambr.uplink;
        ret = fd_msg_avp_setvalue(avp_max_bandwidth_ul, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(
                avp_ambr, MSG_BRW_LAST_CHILD, avp_max_bandwidth_ul);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_new(
                ogs_diam_s6a_max_bandwidth_dl, 0, &avp_max_bandwidth_dl);
        ogs_assert(ret == 0);
        val.u32 = subscription_data.ambr.downlink;
        ret = fd_msg_avp_setvalue(avp_max_bandwidth_dl, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(
                avp_ambr, MSG_BRW_LAST_CHILD, avp_max_bandwidth_dl);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_ambr);
        ogs_assert(ret == 0);

        /* Set the Subscribed RAU TAU Timer */
        ret = fd_msg_avp_new(
                ogs_diam_s6a_subscribed_rau_tau_timer, 0, &avp_rau_tau_timer);
        ogs_assert(ret == 0);
        val.i32 = subscription_data.subscribed_rau_tau_timer * 60; /* seconds */
        ret = fd_msg_avp_setvalue(avp_rau_tau_timer, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_rau_tau_timer);
        ogs_assert(ret == 0);

        if (subscription_data.num_of_pdn) {
            /* Set the APN Configuration Profile */
            struct avp *apn_configuration_profile;
            struct avp *context_identifier;
            struct avp *all_apn_configuration_included_indicator;

            ret = fd_msg_avp_new(ogs_diam_s6a_apn_configuration_profile, 0, 
                    &apn_configuration_profile);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_new(ogs_diam_s6a_context_identifier, 0, 
                    &context_identifier);
            ogs_assert(ret == 0);
            val.i32 = 1; /* Context Identifier : 1 */
            ret = fd_msg_avp_setvalue(context_identifier, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(apn_configuration_profile, 
                    MSG_BRW_LAST_CHILD, context_identifier);
            ogs_assert(ret == 0);

            ret = fd_msg_avp_new(
                    ogs_diam_s6a_all_apn_configuration_included_indicator, 0, 
                    &all_apn_configuration_included_indicator);
            ogs_assert(ret == 0);
            val.i32 = 0;
            ret = fd_msg_avp_setvalue(
                    all_apn_configuration_included_indicator, &val);
            ogs_assert(ret == 0);
            ret = fd_msg_avp_add(apn_configuration_profile, 
                    MSG_BRW_LAST_CHILD, 
                    all_apn_configuration_included_indicator);
            ogs_assert(ret == 0);

            for (i = 0; i < subscription_data.num_of_pdn; i++) {
                /* Set the APN Configuration */
                struct avp *apn_configuration, *context_identifier, *pdn_type;
                struct avp *served_party_ip_address, *service_selection;
                struct avp *eps_subscribed_qos_profile, *qos_class_identifier;
                struct avp *allocation_retention_priority, *priority_level;
                struct avp *pre_emption_capability, *pre_emption_vulnerability;
                struct avp *mip6_agent_info, *mip_home_agent_address;

                ogs_pdn_t *pdn = &subscription_data.pdn[i];
                ogs_assert(pdn);
                pdn->context_identifier = i+1;

                ret = fd_msg_avp_new(ogs_diam_s6a_apn_configuration, 0, 
                    &apn_configuration);
                ogs_assert(ret == 0);

                /* Set Context-Identifier */
                ret = fd_msg_avp_new(ogs_diam_s6a_context_identifier, 0, 
                        &context_identifier);
                ogs_assert(ret == 0);
                val.i32 = pdn->context_identifier;
                ret = fd_msg_avp_setvalue(context_identifier, &val);
                ogs_assert(ret == 0);
                ret = fd_msg_avp_add(apn_configuration, 
                        MSG_BRW_LAST_CHILD, context_identifier);
                ogs_assert(ret == 0);

                /* Set PDN-Type */
                ret = fd_msg_avp_new(ogs_diam_s6a_pdn_type, 0, &pdn_type);
                ogs_assert(ret == 0);
                val.i32 = pdn->pdn_type;
                ret = fd_msg_avp_setvalue(pdn_type, &val);
                ogs_assert(ret == 0);
                ret = fd_msg_avp_add(apn_configuration, 
                        MSG_BRW_LAST_CHILD, pdn_type);
                ogs_assert(ret == 0);

                /* Set Served-Party-IP-Address */
                if ((pdn->pdn_type == OGS_DIAM_PDN_TYPE_IPV4 ||
                     pdn->pdn_type == OGS_DIAM_PDN_TYPE_IPV4V6) &&
                    pdn->ue_ip.ipv4) {
                    ret = fd_msg_avp_new(ogs_diam_s6a_served_party_ip_address,
                            0, &served_party_ip_address);
                    ogs_assert(ret == 0);
                    sin.sin_family = AF_INET;
                    sin.sin_addr.s_addr = pdn->ue_ip.addr;
                    ret = fd_msg_avp_value_encode(
                            &sin, served_party_ip_address);
                    ogs_assert(ret == 0);
                    ret = fd_msg_avp_add(apn_configuration, MSG_BRW_LAST_CHILD,
                            served_party_ip_address);
                    ogs_assert(ret == 0);
                }

                if ((pdn->pdn_type == OGS_DIAM_PDN_TYPE_IPV6 ||
                     pdn->pdn_type == OGS_DIAM_PDN_TYPE_IPV4V6) &&
                    pdn->ue_ip.ipv6) {
                    ret = fd_msg_avp_new(ogs_diam_s6a_served_party_ip_address,
                            0, &served_party_ip_address);
                    ogs_assert(ret == 0);
                    sin6.sin6_family = AF_INET6;
                    memcpy(sin6.sin6_addr.s6_addr,
                            pdn->ue_ip.addr6, OGS_IPV6_LEN);
                    ret = fd_msg_avp_value_encode(
                            &sin6, served_party_ip_address);
                    ogs_assert(ret == 0);
                    ret = fd_msg_avp_add(apn_configuration, MSG_BRW_LAST_CHILD,
                            served_party_ip_address);
                    ogs_assert(ret == 0);
                }

                /* Set Service-Selection */
                ret = fd_msg_avp_new(ogs_diam_s6a_service_selection, 0, 
                        &service_selection);
                ogs_assert(ret == 0);
                val.os.data = (uint8_t *)pdn->apn;
                val.os.len = strlen(pdn->apn);
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
                val.i32 = pdn->qos.qci;
                ret = fd_msg_avp_setvalue(qos_class_identifier, &val);
                ogs_assert(ret == 0);
                ret = fd_msg_avp_add(eps_subscribed_qos_profile, 
                        MSG_BRW_LAST_CHILD, qos_class_identifier);
                ogs_assert(ret == 0);

                        /* Set Allocation retention priority */
                ret = fd_msg_avp_new(
                        ogs_diam_s6a_allocation_retention_priority, 0, 
                        &allocation_retention_priority);
                ogs_assert(ret == 0);

                ret = fd_msg_avp_new(
                        ogs_diam_s6a_priority_level, 0, &priority_level);
                ogs_assert(ret == 0);
                val.u32 = pdn->qos.arp.priority_level;
                ret = fd_msg_avp_setvalue(priority_level, &val);
                ogs_assert(ret == 0);
                ret = fd_msg_avp_add(allocation_retention_priority, 
                    MSG_BRW_LAST_CHILD, priority_level);
                ogs_assert(ret == 0);

                ret = fd_msg_avp_new(ogs_diam_s6a_pre_emption_capability, 0, 
                        &pre_emption_capability);
                ogs_assert(ret == 0);
                val.u32 = pdn->qos.arp.pre_emption_capability;
                ret = fd_msg_avp_setvalue(pre_emption_capability, &val);
                ogs_assert(ret == 0);
                ret = fd_msg_avp_add(allocation_retention_priority, 
                    MSG_BRW_LAST_CHILD, pre_emption_capability);
                ogs_assert(ret == 0);

                ret = fd_msg_avp_new(ogs_diam_s6a_pre_emption_vulnerability, 0, 
                        &pre_emption_vulnerability);
                ogs_assert(ret == 0);
                val.u32 = pdn->qos.arp.pre_emption_vulnerability;
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
                if (pdn->pgw_ip.ipv4 || pdn->pgw_ip.ipv6) {
                    ret = fd_msg_avp_new(ogs_diam_mip6_agent_info, 0,
                                &mip6_agent_info);
                    ogs_assert(ret == 0);

                    if (pdn->pgw_ip.ipv4) {
                        ret = fd_msg_avp_new(ogs_diam_mip_home_agent_address, 0,
                                    &mip_home_agent_address);
                        ogs_assert(ret == 0);
                        sin.sin_family = AF_INET;
                        sin.sin_addr.s_addr = pdn->pgw_ip.addr;
                        ret = fd_msg_avp_value_encode (
                                    &sin, mip_home_agent_address );
                        ogs_assert(ret == 0);
                        ret = fd_msg_avp_add(mip6_agent_info,
                                MSG_BRW_LAST_CHILD, mip_home_agent_address);
                        ogs_assert(ret == 0);
                    }

                    if (pdn->pgw_ip.ipv6) {
                        ret = fd_msg_avp_new(ogs_diam_mip_home_agent_address, 0,
                                    &mip_home_agent_address);
                        ogs_assert(ret == 0);
                        sin6.sin6_family = AF_INET6;
                        memcpy(sin6.sin6_addr.s6_addr, pdn->pgw_ip.addr6,
                                sizeof pdn->pgw_ip.addr6);
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
                }

                /* Set AMBR */
                if (pdn->ambr.downlink || pdn->ambr.uplink) {
                    ret = fd_msg_avp_new(ogs_diam_s6a_ambr, 0, &avp_ambr);
                    ogs_assert(ret == 0);
                    ret = fd_msg_avp_new(ogs_diam_s6a_max_bandwidth_ul, 0, 
                                &avp_max_bandwidth_ul);
                    ogs_assert(ret == 0);
                    val.u32 = pdn->ambr.uplink;
                    ret = fd_msg_avp_setvalue(avp_max_bandwidth_ul, &val);
                    ogs_assert(ret == 0);
                    ret = fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD, 
                                avp_max_bandwidth_ul);
                    ogs_assert(ret == 0);
                    ret = fd_msg_avp_new(ogs_diam_s6a_max_bandwidth_dl, 0, 
                                &avp_max_bandwidth_dl);
                    ogs_assert(ret == 0);
                    val.u32 = pdn->ambr.downlink;
                    ret = fd_msg_avp_setvalue(avp_max_bandwidth_dl, &val);
                    ogs_assert(ret == 0);
                    ret = fd_msg_avp_add(avp_ambr, MSG_BRW_LAST_CHILD, 
                                avp_max_bandwidth_dl);
                    ogs_assert(ret == 0);

                    ret = fd_msg_avp_add(apn_configuration, 
                            MSG_BRW_LAST_CHILD, avp_ambr);
                    ogs_assert(ret == 0);
                }

                ret = fd_msg_avp_add(apn_configuration_profile, 
                        MSG_BRW_LAST_CHILD, apn_configuration);
                ogs_assert(ret == 0);
            }
            ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, 
                    apn_configuration_profile);
            ogs_assert(ret == 0);
        }

        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);
    }

    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_S6A_APPLICATION_ID);
    ogs_assert(ret == 0);

	/* Send the answer */
	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    ogs_debug("[HSS] Update-Location-Answer\n");
	
	/* Add this value to the stats */
	ogs_assert( pthread_mutex_lock(&ogs_diam_logger_self()->stats_lock) == 0);
	ogs_diam_logger_self()->stats.nb_echoed++;
	ogs_assert( pthread_mutex_unlock(&ogs_diam_logger_self()->stats_lock) == 0);

	return 0;

out:
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

    /* Set Vendor-Specific-Application-Id AVP */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_S6A_APPLICATION_ID);
    ogs_assert(ret == 0);

	ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    return 0;
}

int hss_fd_init(void)
{
    int ret;
	struct disp_when data;
    struct dict_object *s6a_app, *vnd;
    struct dict_vendor_data vnd_data;
    struct dict_application_data s6a_app_data;

    ret = ogs_diam_init(FD_MODE_SERVER,
                hss_self()->diam_conf_path, hss_self()->diam_config);
    ogs_assert(ret == 0);

    vnd_data.vendor_id = 10415;
    vnd_data.vendor_name = (char *) "3GPP";

    ret = fd_dict_new(fd_g_config->cnf_dict,
            DICT_VENDOR, &vnd_data, NULL, &vnd);
    ogs_assert(ret == 0);

    s6a_app_data.application_id = 16777251;
    s6a_app_data.application_name = (char *) "S6A";

    ret = fd_dict_new(fd_g_config->cnf_dict, DICT_APPLICATION,
            &s6a_app_data, NULL, &s6a_app);
    ogs_assert(ret == 0);

    ret = fd_disp_app_support(s6a_app, vnd, 1, 0);
    ogs_assert(ret == 0);

	/* Install objects definitions for this application */
	ret = ogs_diam_s6a_init();
    ogs_assert(ret == 0);

	memset(&data, 0, sizeof(data));
	data.app = ogs_diam_s6a_application;
	
	/* Fallback CB if command != unexpected message received */
	ret = fd_disp_register(hss_ogs_diam_s6a_fb_cb, DISP_HOW_APPID, &data, NULL,
                &hdl_s6a_fb);
    ogs_assert(ret == 0);
	
	/* Specific handler for Authentication-Information-Request */
	data.command = ogs_diam_s6a_cmd_air;
	ret = fd_disp_register(hss_ogs_diam_s6a_air_cb, DISP_HOW_CC, &data, NULL,
                &hdl_s6a_air);
    ogs_assert(ret == 0);

	/* Specific handler for Location-Update-Request */
	data.command = ogs_diam_s6a_cmd_ulr;
	ret = fd_disp_register(hss_ogs_diam_s6a_ulr_cb, DISP_HOW_CC, &data, NULL, 
                &hdl_s6a_ulr);
    ogs_assert(ret == 0);

	/* Advertise the support for the application in the peer */
	ret = fd_disp_app_support(ogs_diam_s6a_application, ogs_diam_vendor, 1, 0);
    ogs_assert(ret == 0);

	return OGS_OK;
}

void hss_fd_final(void)
{
	if (hdl_s6a_fb)
		(void) fd_disp_unregister(&hdl_s6a_fb, NULL);
	if (hdl_s6a_air)
		(void) fd_disp_unregister(&hdl_s6a_air, NULL);
	if (hdl_s6a_ulr)
		(void) fd_disp_unregister(&hdl_s6a_ulr, NULL);

    ogs_diam_final();
}
