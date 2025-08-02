/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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
/* handler for Location-Info-Request cb */
static struct disp_hdl *hdl_cx_lir = NULL;

/* Default callback for the application. */
static int hss_ogs_diam_cx_fb_cb(struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    /* This CB should never be called */
    ogs_warn("Unexpected message received!");
    OGS_DIAM_STATS_MTX(
        HSS_DIAM_PRIV_STATS_INC(cx.rx_unknown);
    )

    return ENOTSUP;
}

/* Callback for incoming User-Authorization-Request messages */
static int hss_ogs_diam_cx_uar_cb(struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    int rv, ret;
    uint32_t result_code = 0;
    struct msg *ans = NULL, *qry = NULL;
    struct avp_hdr *hdr = NULL;
    union avp_value val;

    char *user_name = NULL;
    char *public_identity = NULL;
    char *visited_network_identifier = NULL;
    char *server_name = NULL;
    char imsi_or_msisdn_bcd[OGS_MAX_IMSI_BCD_LEN+1];
    ogs_msisdn_data_t msisdn_data;
    int error_occurred = 0;

    ogs_debug("Rx User-Authorization-Request");

    /* Validate input parameters */
    if (!msg || !*msg) {
        ogs_error("Invalid message pointer");
        return EINVAL;
    }

    /* Initialize variables */
    memset(imsi_or_msisdn_bcd, 0, sizeof(imsi_or_msisdn_bcd));
    memset(&msisdn_data, 0, sizeof(ogs_msisdn_data_t));

    /* Create answer header */
    qry = *msg;
    ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    if (ret != 0) {
        ogs_error("Failed to create answer message");
        error_occurred = 1;
        goto out;
    }
    ans = *msg;

    /* Get User-Name AVP (Mandatory) */
    ret = fd_msg_search_avp(qry, ogs_diam_user_name, &avp);
    if (ret != 0) {
        ogs_error("Failed to search User-Name AVP");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    if (!avp) {
        ogs_error("No User-Name AVP found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ret = fd_msg_avp_hdr(avp, &hdr);
    if (ret != 0 || !hdr) {
        ogs_error("Failed to get User-Name AVP header");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    user_name = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    if (!user_name) {
        ogs_error("Failed to duplicate User-Name");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    ogs_extract_digit_from_string(imsi_or_msisdn_bcd, user_name);

    /* Get Public-Identity AVP (Mandatory) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_public_identity, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Public-Identity AVP");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    if (!avp) {
        ogs_error("No Public-Identity AVP found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ret = fd_msg_avp_hdr(avp, &hdr);
    if (ret != 0 || !hdr) {
        ogs_error("Failed to get Public-Identity AVP header");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    public_identity = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    if (!public_identity) {
        ogs_error("Failed to duplicate Public-Identity");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    /* Get Visited-Network-Identifier AVP (Mandatory) */
    ret = fd_msg_search_avp(qry, ogs_diam_visited_network_identifier, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Visited-Network-Identifier AVP");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    if (!avp) {
        ogs_error("No Visited-Network-Identifier AVP found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ret = fd_msg_avp_hdr(avp, &hdr);
    if (ret != 0 || !hdr) {
        ogs_error("Failed to get Visited-Network-Identifier AVP header");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    visited_network_identifier = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    if (!visited_network_identifier) {
        ogs_error("Failed to duplicate Visited-Network-Identifier");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    /* Get MSISDN data from database */
    rv = hss_db_msisdn_data(imsi_or_msisdn_bcd, &msisdn_data);
    if (rv != OGS_OK) {
        ogs_error("Cannot get MSISDN-Data for IMSI or MSISDN: %s",
                    imsi_or_msisdn_bcd);
        result_code = OGS_DIAM_CX_ERROR_USER_UNKNOWN;
        error_occurred = 1;
        goto out;
    }

    if (!error_occurred) {
        /* Set Vendor-Specific-Application-Id AVP */
        ret = ogs_diam_message_vendor_specific_appid_set(
                ans, OGS_DIAM_CX_APPLICATION_ID);
        if (ret != 0) {
            ogs_error("Failed to set Vendor-Specific-Application-Id");
            error_occurred = 1;
            goto out;
        }

        /* Associate IMPI(User-Name) with IMPU(Public-Identity) */
        hss_cx_associate_identity(user_name, public_identity);

        /* Set IMSI for IMPI(User-Name) */
        hss_cx_set_imsi_bcd(user_name,
                msisdn_data.imsi.bcd, visited_network_identifier);

        /* Get Server-Name by IMPU(Public-Identity) */
        server_name = hss_cx_get_server_name(public_identity);
        if (!server_name)
            result_code = OGS_DIAM_CX_FIRST_REGISTRATION;
        else
            result_code = OGS_DIAM_CX_SUBSEQUENT_REGISTRATION;

        /* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
        ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
        if (ret != 0) {
            ogs_error("Failed to set experimental result code");
            error_occurred = 1;
            goto out;
        }

        /* Set the Auth-Session-State AVP */
        ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create Auth-Session-State AVP");
            error_occurred = 1;
            goto out;
        }
        val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
        ret = fd_msg_avp_setvalue(avp, &val);
        if (ret != 0) {
            ogs_error("Failed to set Auth-Session-State value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add Auth-Session-State AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set the Server-Name AVP */
        if (server_name) {
            ret = fd_msg_avp_new(ogs_diam_cx_server_name, 0, &avp);
            if (ret != 0) {
                ogs_error("Failed to create Server-Name AVP");
                error_occurred = 1;
                goto out;
            }
            val.os.data = (uint8_t *)server_name;
            val.os.len  = strlen(server_name);
            ret = fd_msg_avp_setvalue(avp, &val);
            if (ret != 0) {
                ogs_error("Failed to set Server-Name value");
                error_occurred = 1;
                goto out;
            }
            ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
            if (ret != 0) {
                ogs_error("Failed to add Server-Name AVP");
                error_occurred = 1;
                goto out;
            }
        }

        /* Send the answer */
        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send message");
            error_occurred = 1;
            goto out;
        }

        ogs_debug("Tx User-Authorization-Answer");

        /* Add to stats */
        OGS_DIAM_STATS_MTX(
            OGS_DIAM_STATS_INC(nb_echoed);
            HSS_DIAM_PRIV_STATS_INC(cx.rx_uar);
            HSS_DIAM_PRIV_STATS_INC(cx.tx_uaa);
        )

        /* Cleanup resources */
        if (user_name) ogs_free(user_name);
        if (public_identity) ogs_free(public_identity);
        if (visited_network_identifier) ogs_free(visited_network_identifier);

        return 0;
    }

out:
    /* Error handling */
    if (ans) {
        /* Set Vendor-Specific-Application-Id AVP */
        ret = ogs_diam_message_vendor_specific_appid_set(
                ans, OGS_DIAM_CX_APPLICATION_ID);

        /* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
        if (result_code != 0) {
            ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
        }

        /* Set the Auth-Session-State AVP */
        ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
        if (ret == 0) {
            val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
            ret = fd_msg_avp_setvalue(avp, &val);
            if (ret == 0) {
                ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
            }
        }

        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send error response");
        }
    }

    /* Update error stats */
    OGS_DIAM_STATS_MTX(
        HSS_DIAM_PRIV_STATS_INC(cx.rx_uar);
        HSS_DIAM_PRIV_STATS_INC(cx.rx_uar_error);
    )

    /* Cleanup resources */
    if (user_name) ogs_free(user_name);
    if (public_identity) ogs_free(public_identity);
    if (visited_network_identifier) ogs_free(visited_network_identifier);

    return 0;
}

/* Callback for incoming Multimedia-Auth-Request messages */
static int hss_ogs_diam_cx_mar_cb(struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    int rv, ret;
    uint32_t result_code = 0;
    struct msg *ans = NULL, *qry = NULL;
    struct avp *sip_auth_data_item_avp = NULL;
    struct avp *authentication_scheme_avp = NULL;
    struct avp *sip_authorization_avp = NULL;
    struct avp *avpch = NULL;
    struct avp_hdr *hdr = NULL;
    union avp_value val;

    char *user_name = NULL;
    char *public_identity = NULL;
    char *server_name = NULL;
    char *authentication_scheme = NULL;
    char *imsi_bcd = NULL;

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

    bool matched = false;
    int error_occurred = 0;

    ogs_debug("Rx Multimedia-Auth-Request");

    /* Validate input parameters */
    if (!msg || !*msg) {
        ogs_error("Invalid message pointer");
        return EINVAL;
    }

    /* Initialize variables */
    memset(&auth_info, 0, sizeof(auth_info));

    /* Create answer header */
    qry = *msg;
    ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    if (ret != 0) {
        ogs_error("Failed to create answer message");
        error_occurred = 1;
        goto out;
    }
    ans = *msg;

    /* Get User-Name AVP (Mandatory) */
    ret = fd_msg_search_avp(qry, ogs_diam_user_name, &avp);
    if (ret != 0) {
        ogs_error("Failed to search User-Name AVP");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    if (!avp) {
        ogs_error("No User-Name AVP found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ret = fd_msg_avp_hdr(avp, &hdr);
    if (ret != 0 || !hdr) {
        ogs_error("Failed to get User-Name AVP header");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    user_name = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    if (!user_name) {
        ogs_error("Failed to duplicate User-Name");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    /* Get Public-Identity AVP (Mandatory) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_public_identity, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Public-Identity AVP");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    if (!avp) {
        ogs_error("No Public-Identity AVP found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ret = fd_msg_avp_hdr(avp, &hdr);
    if (ret != 0 || !hdr) {
        ogs_error("Failed to get Public-Identity AVP header");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    public_identity = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    if (!public_identity) {
        ogs_error("Failed to duplicate Public-Identity");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    /* Get Server-Name AVP (Mandatory) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_server_name, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Server-Name AVP");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    if (!avp) {
        ogs_error("No Server-Name AVP found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ret = fd_msg_avp_hdr(avp, &hdr);
    if (ret != 0 || !hdr) {
        ogs_error("Failed to get Server-Name AVP header");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    server_name = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    if (!server_name) {
        ogs_error("Failed to duplicate Server-Name");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    /* Check if IMPI(User-Name) + IMPU(Public-Identity) is associated */
    matched = hss_cx_identity_is_associated(user_name, public_identity);
    if (!matched) {
        ogs_error("User-Name[%s] Public-Identity[%s] is not associated",
                    user_name, public_identity);
        result_code = OGS_DIAM_CX_ERROR_IDENTITIES_DONT_MATCH;
        error_occurred = 1;
        goto out;
    }

    /* Check if IMSI */
    imsi_bcd = hss_cx_get_imsi_bcd(public_identity);
    if (!imsi_bcd) {
        ogs_error("Cannot find IMSI for User-Name[%s] Public-Identity[%s]",
                    user_name, public_identity);
        result_code = OGS_DIAM_CX_ERROR_IDENTITY_NOT_REGISTERED;
        error_occurred = 1;
        goto out;
    }

    /* Get the SIP-Auth-Data-Item AVP (Mandatory) */
    ret = fd_msg_search_avp(
            qry, ogs_diam_cx_sip_auth_data_item, &sip_auth_data_item_avp);
    if (ret != 0) {
        ogs_error("Failed to search SIP-Auth-Data-Item AVP");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    if (!sip_auth_data_item_avp) {
        ogs_error("No SIP-Auth-Data-Item AVP found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ret = fd_msg_avp_hdr(sip_auth_data_item_avp, &hdr);
    if (ret != 0 || !hdr) {
        ogs_error("Failed to get SIP-Auth-Data-Item AVP header");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    /* Get the Authentication-Scheme AVP */
    ret = fd_msg_search_avp(sip_auth_data_item_avp,
            ogs_diam_cx_sip_authentication_scheme, &authentication_scheme_avp);
    if (ret == 0 && authentication_scheme_avp) {
        ret = fd_msg_avp_hdr(authentication_scheme_avp, &hdr);
        if (ret == 0 && hdr) {
            authentication_scheme = ogs_strndup(
                (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
            if (!authentication_scheme) {
                ogs_error("Failed to duplicate Authentication-Scheme");
                result_code = OGS_DIAM_INVALID_AVP_VALUE;
                error_occurred = 1;
                goto out;
            }
        }
    }

    /* IMS_AKA is only supported */
    if (authentication_scheme &&
        ogs_strcasecmp(authentication_scheme,
            OGS_DIAM_CX_AUTH_SCHEME_IMS_AKA) != 0 &&
        ogs_strcasecmp(authentication_scheme,
            OGS_DIAM_CX_AUTH_SCHEME_UNKNOWN) != 0) {
        ogs_error("Authentication-Scheme[%s] is not supported",
                    authentication_scheme);
        result_code = OGS_DIAM_CX_ERROR_AUTH_SCHEME_NOT_SUPPORTED;
        error_occurred = 1;
        goto out;
    }

    /* DB : HSS Auth-Info */
    rv = hss_db_auth_info(imsi_bcd, &auth_info);
    if (rv != OGS_OK) {
        ogs_error("Cannot get IMS-Data for IMSI: %s", imsi_bcd);
        result_code = OGS_DIAM_CX_ERROR_USER_UNKNOWN;
        error_occurred = 1;
        goto out;
    }

    /* Overwrite Server-Name for IMPU(Public-Identity) */
    hss_cx_set_server_name(public_identity, server_name, true);

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
    if (ret == 0 && sip_authorization_avp) {
        ret = fd_msg_avp_hdr(sip_authorization_avp, &hdr);
        if (ret == 0 && hdr) {
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
                ogs_error("Re-synch MAC failed for IMSI: %s", imsi_bcd);

                ogs_log_print(OGS_LOG_ERROR, "MAC_S: ");
                ogs_log_hexdump(OGS_LOG_ERROR, mac_s, OGS_MAC_S_LEN);
                ogs_log_hexdump(OGS_LOG_ERROR,
                    (void*)(hdr->avp_value->os.data +
                        OGS_RAND_LEN + OGS_SQN_LEN),
                    OGS_MAC_S_LEN);
                ogs_log_print(OGS_LOG_ERROR, "SQN: ");
                ogs_log_hexdump(OGS_LOG_ERROR, sqn, OGS_SQN_LEN);

                result_code = OGS_DIAM_CX_ERROR_AUTH_SCHEME_NOT_SUPPORTED;
                error_occurred = 1;
                goto out;
            }
        }
    }

    rv = hss_db_update_sqn(imsi_bcd, auth_info.rand, auth_info.sqn);
    if (rv != OGS_OK) {
        ogs_error("Cannot update rand and sqn for IMSI: %s", imsi_bcd);
        result_code = OGS_DIAM_CX_ERROR_IN_ASSIGNMENT_TYPE;
        error_occurred = 1;
        goto out;
    }

    rv = hss_db_increment_sqn(imsi_bcd);
    if (rv != OGS_OK) {
        ogs_error("Cannot increment sqn for IMSI: %s", imsi_bcd);
        result_code = OGS_DIAM_CX_ERROR_IN_ASSIGNMENT_TYPE;
        error_occurred = 1;
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

    if (!error_occurred) {
        /* Set Vendor-Specific-Application-Id AVP */
        ret = ogs_diam_message_vendor_specific_appid_set(
                ans, OGS_DIAM_CX_APPLICATION_ID);
        if (ret != 0) {
            ogs_error("Failed to set Vendor-Specific-Application-Id");
            error_occurred = 1;
            goto out;
        }

        /* Set the Auth-Session-State AVP */
        ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create Auth-Session-State AVP");
            error_occurred = 1;
            goto out;
        }
        val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
        ret = fd_msg_avp_setvalue(avp, &val);
        if (ret != 0) {
            ogs_error("Failed to set Auth-Session-State value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add Auth-Session-State AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set success result code */
        ret = fd_msg_rescode_set(ans, (char*)"DIAMETER_SUCCESS", NULL, NULL, 1);
        if (ret != 0) {
            ogs_error("Failed to set success result code");
            error_occurred = 1;
            goto out;
        }

        /* Set the User-Name AVP */
        ret = fd_msg_avp_new(ogs_diam_user_name, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create User-Name AVP");
            error_occurred = 1;
            goto out;
        }
        val.os.data = (uint8_t *)user_name;
        val.os.len = strlen(user_name);
        ret = fd_msg_avp_setvalue(avp, &val);
        if (ret != 0) {
            ogs_error("Failed to set User-Name value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add User-Name AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set the Public-Identity AVP */
        ret = fd_msg_avp_new(ogs_diam_cx_public_identity, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create Public-Identity AVP");
            error_occurred = 1;
            goto out;
        }
        val.os.data = (uint8_t *)public_identity;
        val.os.len = strlen(public_identity);
        ret = fd_msg_avp_setvalue(avp, &val);
        if (ret != 0) {
            ogs_error("Failed to set Public-Identity value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add Public-Identity AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set the SIP-Number-Auth-Items AVP */
        ret = fd_msg_avp_new(ogs_diam_cx_sip_number_auth_items, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create SIP-Number-Auth-Items AVP");
            error_occurred = 1;
            goto out;
        }
        val.u32 = 1;
        ret = fd_msg_avp_setvalue(avp, &val);
        if (ret != 0) {
            ogs_error("Failed to set SIP-Number-Auth-Items value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add SIP-Number-Auth-Items AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set the SIP-Auth-Data-Item AVP */
        ret = fd_msg_avp_new(ogs_diam_cx_sip_auth_data_item, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create SIP-Auth-Data-Item AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set the SIP-Item-Number AVP */
        ret = fd_msg_avp_new(ogs_diam_cx_sip_item_number, 0, &avpch);
        if (ret != 0) {
            ogs_error("Failed to create SIP-Item-Number AVP");
            error_occurred = 1;
            goto out;
        }
        val.u32 = 1;
        ret = fd_msg_avp_setvalue(avpch, &val);
        if (ret != 0) {
            ogs_error("Failed to set SIP-Item-Number value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
        if (ret != 0) {
            ogs_error("Failed to add SIP-Item-Number AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set the SIP-Authentication-Scheme AVP */
        ret = fd_msg_avp_new(ogs_diam_cx_sip_authentication_scheme, 0, &avpch);
        if (ret != 0) {
            ogs_error("Failed to create SIP-Authentication-Scheme AVP");
            error_occurred = 1;
            goto out;
        }
        val.os.data = (uint8_t *)OGS_DIAM_CX_AUTH_SCHEME_IMS_AKA;
        val.os.len = strlen(OGS_DIAM_CX_AUTH_SCHEME_IMS_AKA);
        ret = fd_msg_avp_setvalue(avpch, &val);
        if (ret != 0) {
            ogs_error("Failed to set SIP-Authentication-Scheme value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
        if (ret != 0) {
            ogs_error("Failed to add SIP-Authentication-Scheme AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set the SIP-Authenticate AVP */
        ret = fd_msg_avp_new(ogs_diam_cx_sip_authenticate, 0, &avpch);
        if (ret != 0) {
            ogs_error("Failed to create SIP-Authenticate AVP");
            error_occurred = 1;
            goto out;
        }
        val.os.data = authenticate;
        val.os.len = OGS_KEY_LEN * 2;
        ret = fd_msg_avp_setvalue(avpch, &val);
        if (ret != 0) {
            ogs_error("Failed to set SIP-Authenticate value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
        if (ret != 0) {
            ogs_error("Failed to add SIP-Authenticate AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set the SIP-Authorization AVP */
        ret = fd_msg_avp_new(ogs_diam_cx_sip_authorization, 0, &avpch);
        if (ret != 0) {
            ogs_error("Failed to create SIP-Authorization AVP");
            error_occurred = 1;
            goto out;
        }
        val.os.data = xres;
        val.os.len = xres_len;
        ret = fd_msg_avp_setvalue(avpch, &val);
        if (ret != 0) {
            ogs_error("Failed to set SIP-Authorization value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
        if (ret != 0) {
            ogs_error("Failed to add SIP-Authorization AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set the Confidentiality-Key AVP */
        ret = fd_msg_avp_new(ogs_diam_cx_confidentiality_key, 0, &avpch);
        if (ret != 0) {
            ogs_error("Failed to create Confidentiality-Key AVP");
            error_occurred = 1;
            goto out;
        }
        val.os.data = ck;
        val.os.len = OGS_KEY_LEN;
        ret = fd_msg_avp_setvalue(avpch, &val);
        if (ret != 0) {
            ogs_error("Failed to set Confidentiality-Key value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
        if (ret != 0) {
            ogs_error("Failed to add Confidentiality-Key AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set the Integrity-Key AVP */
        ret = fd_msg_avp_new(ogs_diam_cx_integrity_key, 0, &avpch);
        if (ret != 0) {
            ogs_error("Failed to create Integrity-Key AVP");
            error_occurred = 1;
            goto out;
        }
        val.os.data = ik;
        val.os.len = OGS_KEY_LEN;
        ret = fd_msg_avp_setvalue(avpch, &val);
        if (ret != 0) {
            ogs_error("Failed to set Integrity-Key value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
        if (ret != 0) {
            ogs_error("Failed to add Integrity-Key AVP");
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add SIP-Auth-Data-Item AVP");
            error_occurred = 1;
            goto out;
        }

        /* Send the answer */
        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send message");
            error_occurred = 1;
            goto out;
        }

        ogs_debug("Tx Multimedia-Auth-Answer");

        /* Add to stats */
        OGS_DIAM_STATS_MTX(
            OGS_DIAM_STATS_INC(nb_echoed);
            HSS_DIAM_PRIV_STATS_INC(cx.rx_mar);
            HSS_DIAM_PRIV_STATS_INC(cx.tx_maa);
        )

        /* Cleanup resources */
        if (authentication_scheme) ogs_free(authentication_scheme);
        if (user_name) ogs_free(user_name);
        if (public_identity) ogs_free(public_identity);
        if (server_name) ogs_free(server_name);

        return 0;
    }

out:
    /* Error handling */
    if (ans) {
        /* Set Vendor-Specific-Application-Id AVP */
        ret = ogs_diam_message_vendor_specific_appid_set(
                ans, OGS_DIAM_CX_APPLICATION_ID);

        /* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
        if (result_code != 0) {
            ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
        }

        /* Set the Auth-Session-State AVP */
        ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
        if (ret == 0) {
            val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
            ret = fd_msg_avp_setvalue(avp, &val);
            if (ret == 0) {
                ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
            }
        }

        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send error response");
        }
    }

    /* Update error stats */
    OGS_DIAM_STATS_MTX(
        HSS_DIAM_PRIV_STATS_INC(cx.rx_mar);
        HSS_DIAM_PRIV_STATS_INC(cx.rx_mar_error);
    )

    /* Cleanup resources */
    if (authentication_scheme) ogs_free(authentication_scheme);
    if (user_name) ogs_free(user_name);
    if (public_identity) ogs_free(public_identity);
    if (server_name) ogs_free(server_name);

    return 0;
}

/* Callback for incoming Server-Assignment-Request messages */
static int hss_ogs_diam_cx_sar_cb(struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    int rv, ret;
    uint32_t result_code = 0;
    struct msg *ans = NULL, *qry = NULL;
    bool matched = false;
    struct avp *user_name_avp = NULL;
    struct avp *avpch = NULL;
    struct avp_hdr *hdr = NULL;
    union avp_value val;

    char *user_name = NULL;
    char *public_identity = NULL;
    char *server_name = NULL;
    char *user_data = NULL;
    char *imsi_bcd = NULL;
    char *visited_network_identifier = NULL;

    ogs_ims_data_t ims_data;
    int error_occurred = 0;

    ogs_debug("Rx Server-Assignment-Request");

    /* Validate input parameters */
    if (!msg || !*msg) {
        ogs_error("Invalid message pointer");
        return EINVAL;
    }

    /* Initialize variables */
    memset(&ims_data, 0, sizeof(ogs_ims_data_t));

    /* Create answer header */
    qry = *msg;
    ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    if (ret != 0) {
        ogs_error("Failed to create answer message");
        error_occurred = 1;
        goto out;
    }
    ans = *msg;

    /* Get Public-Identity AVP (Mandatory) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_public_identity, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Public-Identity AVP");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    if (!avp) {
        ogs_error("No Public-Identity AVP found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ret = fd_msg_avp_hdr(avp, &hdr);
    if (ret != 0 || !hdr) {
        ogs_error("Failed to get Public-Identity AVP header");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    public_identity = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    if (!public_identity) {
        ogs_error("Failed to duplicate Public-Identity");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    /* Get Server-Name AVP (Mandatory) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_server_name, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Server-Name AVP");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    if (!avp) {
        ogs_error("No Server-Name AVP found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ret = fd_msg_avp_hdr(avp, &hdr);
    if (ret != 0 || !hdr) {
        ogs_error("Failed to get Server-Name AVP header");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    server_name = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    if (!server_name) {
        ogs_error("Failed to duplicate Server-Name");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    /* Get User-Name AVP (Optional) */
    ret = fd_msg_search_avp(qry, ogs_diam_user_name, &user_name_avp);
    if (ret == 0 && user_name_avp) {
        ret = fd_msg_avp_hdr(user_name_avp, &hdr);
        if (ret != 0 || !hdr) {
            ogs_error("Failed to get User-Name AVP header");
            result_code = OGS_DIAM_INVALID_AVP_VALUE;
            error_occurred = 1;
            goto out;
        }

        user_name = ogs_strndup(
            (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
        if (!user_name) {
            ogs_error("Failed to duplicate User-Name");
            result_code = OGS_DIAM_INVALID_AVP_VALUE;
            error_occurred = 1;
            goto out;
        }

        /* Check if IMPI(User-Name) + IMPU(Public-Identity) is associated */
        matched = hss_cx_identity_is_associated(user_name, public_identity);
        if (!matched) {
            ogs_error("User-Name[%s] Public-Identity[%s] is not associated",
                        user_name, public_identity);
            result_code = OGS_DIAM_CX_ERROR_IDENTITIES_DONT_MATCH;
            error_occurred = 1;
            goto out;
        }
    } else {
        const char *temp_user_name = hss_cx_get_user_name(public_identity);
        if (!temp_user_name) {
            ogs_error("Cannot find User-Name for Public-Identity[%s]",
                        public_identity);
            result_code = OGS_DIAM_CX_ERROR_USER_UNKNOWN;
            error_occurred = 1;
            goto out;
        }

        user_name = ogs_strdup(temp_user_name);
        if (!user_name) {
            ogs_error("Failed to duplicate User-Name from context");
            result_code = OGS_DIAM_INVALID_AVP_VALUE;
            error_occurred = 1;
            goto out;
        }
    }

    /* Check if IMSI */
    imsi_bcd = hss_cx_get_imsi_bcd(public_identity);
    if (!imsi_bcd) {
        ogs_error("Cannot find IMSI for User-Name[%s] Public-Identity[%s]",
                    user_name, public_identity);
        result_code = OGS_DIAM_CX_ERROR_IDENTITY_NOT_REGISTERED;
        error_occurred = 1;
        goto out;
    }

    /* Check if Visited-Network-Identifier */
    visited_network_identifier =
        hss_cx_get_visited_network_identifier(public_identity);
    if (!visited_network_identifier) {
        ogs_error("Cannot find Visited-Network-Identifier "
                    "for User-Name[%s] Public-Identity[%s]",
                    user_name, public_identity);
        result_code = OGS_DIAM_CX_ERROR_IDENTITY_NOT_REGISTERED;
        error_occurred = 1;
        goto out;
    }

    /* DB : HSS IMS Service Profile */
    rv = hss_db_ims_data(imsi_bcd, &ims_data);
    if (rv != OGS_OK) {
        ogs_error("Cannot get IMS-Data for IMSI: %s", imsi_bcd);
        result_code = OGS_DIAM_CX_ERROR_USER_UNKNOWN;
        error_occurred = 1;
        goto out;
    }

    /* Overwrite Server-Name for IMPU(Public-Identity) */
    hss_cx_set_server_name(public_identity, server_name, true);

    if (!error_occurred) {
        /* Set Vendor-Specific-Application-Id AVP */
        ret = ogs_diam_message_vendor_specific_appid_set(
                ans, OGS_DIAM_CX_APPLICATION_ID);
        if (ret != 0) {
            ogs_error("Failed to set Vendor-Specific-Application-Id");
            error_occurred = 1;
            goto out;
        }

        /* Set the Auth-Session-State AVP */
        ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create Auth-Session-State AVP");
            error_occurred = 1;
            goto out;
        }
        val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
        ret = fd_msg_avp_setvalue(avp, &val);
        if (ret != 0) {
            ogs_error("Failed to set Auth-Session-State value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add Auth-Session-State AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set success result code */
        ret = fd_msg_rescode_set(ans, (char*)"DIAMETER_SUCCESS", NULL, NULL, 1);
        if (ret != 0) {
            ogs_error("Failed to set success result code");
            error_occurred = 1;
            goto out;
        }

        if (user_name_avp) {
            /* Set the User-Name AVP */
            ret = fd_msg_avp_new(ogs_diam_user_name, 0, &avp);
            if (ret != 0) {
                ogs_error("Failed to create User-Name AVP");
                error_occurred = 1;
                goto out;
            }
            val.os.data = (uint8_t *)user_name;
            val.os.len = strlen(user_name);
            ret = fd_msg_avp_setvalue(avp, &val);
            if (ret != 0) {
                ogs_error("Failed to set User-Name value");
                error_occurred = 1;
                goto out;
            }
            ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
            if (ret != 0) {
                ogs_error("Failed to add User-Name AVP");
                error_occurred = 1;
                goto out;
            }
        }

        /* Get Server-Assignment-Type AVP (Mandatory) */
        ret = fd_msg_search_avp(qry,
                ogs_diam_cx_server_assignment_type, &avp);
        if (ret != 0) {
            ogs_error("Failed to search Server-Assignment-Type AVP");
            result_code = OGS_DIAM_MISSING_AVP;
            error_occurred = 1;
            goto out;
        }

        if (!avp) {
            ogs_error("No Server-Assignment-Type AVP found");
            result_code = OGS_DIAM_MISSING_AVP;
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_hdr(avp, &hdr);
        if (ret != 0 || !hdr) {
            ogs_error("Failed to get Server-Assignment-Type AVP header");
            result_code = OGS_DIAM_INVALID_AVP_VALUE;
            error_occurred = 1;
            goto out;
        }

        if (hdr->avp_value->i32 == OGS_DIAM_CX_SERVER_ASSIGNMENT_REGISTRATION ||
            hdr->avp_value->i32 ==
                OGS_DIAM_CX_SERVER_ASSIGNMENT_RE_REGISTRATION) {

            /* Get User-Data-Already-Available AVP (Mandatory) */
            ret = fd_msg_search_avp(
                    qry, ogs_diam_cx_user_data_already_available, &avp);
            if (ret != 0) {
                ogs_error("Failed to search User-Data-Already-Available AVP");
                result_code = OGS_DIAM_MISSING_AVP;
                error_occurred = 1;
                goto out;
            }

            if (!avp) {
                ogs_error("No User-Data-Already-Available AVP found");
                result_code = OGS_DIAM_MISSING_AVP;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_hdr(avp, &hdr);
            if (ret != 0 || !hdr) {
                ogs_error("Failed to get User-Data-Already-Available header");
                result_code = OGS_DIAM_INVALID_AVP_VALUE;
                error_occurred = 1;
                goto out;
            }

            if (hdr->avp_value->i32 ==
                    OGS_DIAM_CX_USER_DATA_ALREADY_AVAILABLE) {
                /* Nothing to do */
            } else {
                /* Set the User-Data AVP */
                user_data = hss_cx_download_user_data(
                        user_name, visited_network_identifier, &ims_data);
                if (!user_data) {
                    ogs_error("Failed to download user data");
                    result_code = OGS_DIAM_CX_ERROR_USER_UNKNOWN;
                    error_occurred = 1;
                    goto out;
                }

                ret = fd_msg_avp_new(ogs_diam_cx_user_data, 0, &avp);
                if (ret != 0) {
                    ogs_error("Failed to create User-Data AVP");
                    error_occurred = 1;
                    goto out;
                }
                val.os.data = (uint8_t *)user_data;
                val.os.len = strlen(user_data);
                ret = fd_msg_avp_setvalue(avp, &val);
                if (ret != 0) {
                    ogs_error("Failed to set User-Data value");
                    error_occurred = 1;
                    goto out;
                }
                ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
                if (ret != 0) {
                    ogs_error("Failed to add User-Data AVP");
                    error_occurred = 1;
                    goto out;
                }

                /* Set the Charging-Information AVP */
                ret = fd_msg_avp_new(ogs_diam_cx_charging_information, 0, &avp);
                if (ret != 0) {
                    ogs_error("Failed to create Charging-Information AVP");
                    error_occurred = 1;
                    goto out;
                }

                /* Set the Primary-Charging-Collection-Function-Name AVP */
                ret = fd_msg_avp_new(
                        ogs_diam_cx_primary_charging_collection_function_name, 0,
                        &avpch);
                if (ret != 0) {
                    ogs_error("Failed to create Primary-Charging-Collection-"
                             "Function-Name AVP");
                    error_occurred = 1;
                    goto out;
                }
#define PRIMARY_CHARGING_COLLECTION_FUNCTION_NAME "pcrf"
                val.os.data =
                    (uint8_t *)PRIMARY_CHARGING_COLLECTION_FUNCTION_NAME;
                val.os.len = strlen(PRIMARY_CHARGING_COLLECTION_FUNCTION_NAME);
                ret = fd_msg_avp_setvalue(avpch, &val);
                if (ret != 0) {
                    ogs_error("Failed to set Primary-Charging-Collection-"
                             "Function-Name value");
                    error_occurred = 1;
                    goto out;
                }
                ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avpch);
                if (ret != 0) {
                    ogs_error("Failed to add Primary-Charging-Collection-"
                             "Function-Name AVP");
                    error_occurred = 1;
                    goto out;
                }

                ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
                if (ret != 0) {
                    ogs_error("Failed to add Charging-Information AVP");
                    error_occurred = 1;
                    goto out;
                }
            }
        }

        /* Send the answer */
        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send message");
            error_occurred = 1;
            goto out;
        }

        ogs_debug("Tx Server-Assignment-Answer");

        /* Add to stats */
        OGS_DIAM_STATS_MTX(
            OGS_DIAM_STATS_INC(nb_echoed);
            HSS_DIAM_PRIV_STATS_INC(cx.rx_sar);
            HSS_DIAM_PRIV_STATS_INC(cx.tx_saa);
        )

        /* Cleanup resources */
        if (user_data) ogs_free(user_data);
        if (user_name) ogs_free(user_name);
        if (public_identity) ogs_free(public_identity);
        if (server_name) ogs_free(server_name);

        return 0;
    }

out:
    /* Error handling */
    if (ans) {
        /* Set Vendor-Specific-Application-Id AVP */
        ret = ogs_diam_message_vendor_specific_appid_set(
                ans, OGS_DIAM_CX_APPLICATION_ID);

        /* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
        if (result_code != 0) {
            ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
        }

        /* Set the Auth-Session-State AVP */
        ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
        if (ret == 0) {
            val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
            ret = fd_msg_avp_setvalue(avp, &val);
            if (ret == 0) {
                ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
            }
        }

        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send error response");
        }
    }

    /* Update error stats */
    OGS_DIAM_STATS_MTX(
        HSS_DIAM_PRIV_STATS_INC(cx.rx_sar);
        HSS_DIAM_PRIV_STATS_INC(cx.rx_sar_error);
    )

    /* Cleanup resources */
    if (user_data) ogs_free(user_data);
    if (user_name) ogs_free(user_name);
    if (public_identity) ogs_free(public_identity);
    if (server_name) ogs_free(server_name);

    return 0;
}

/* Callback for incoming Location-Info-Request messages */
static int hss_ogs_diam_cx_lir_cb(struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    int ret;
    uint32_t result_code = 0;
    struct msg *ans = NULL, *qry = NULL;
    struct avp_hdr *hdr = NULL;
    union avp_value val;

    char *public_identity = NULL;
    char *server_name = NULL;
    int error_occurred = 0;

    ogs_debug("Rx Location-Info-Request");

    /* Validate input parameters */
    if (!msg || !*msg) {
        ogs_error("Invalid message pointer");
        return EINVAL;
    }

    /* Create answer header */
    qry = *msg;
    ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    if (ret != 0) {
        ogs_error("Failed to create answer message");
        error_occurred = 1;
        goto out;
    }
    ans = *msg;

    /* Get Public-Identity AVP (Mandatory) */
    ret = fd_msg_search_avp(qry, ogs_diam_cx_public_identity, &avp);
    if (ret != 0) {
        ogs_error("Failed to search Public-Identity AVP");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    if (!avp) {
        ogs_error("No Public-Identity AVP found");
        result_code = OGS_DIAM_MISSING_AVP;
        error_occurred = 1;
        goto out;
    }

    ret = fd_msg_avp_hdr(avp, &hdr);
    if (ret != 0 || !hdr) {
        ogs_error("Failed to get Public-Identity AVP header");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    public_identity = ogs_strndup(
        (char*)hdr->avp_value->os.data, hdr->avp_value->os.len);
    if (!public_identity) {
        ogs_error("Failed to duplicate Public-Identity");
        result_code = OGS_DIAM_INVALID_AVP_VALUE;
        error_occurred = 1;
        goto out;
    }

    /* Get Server-Name by IMPU(Public-Identity) */
    server_name = hss_cx_get_server_name(public_identity);
    if (!server_name) {
        ogs_error("No Server-Name in Public-Identity[%s]", public_identity);
        result_code = OGS_DIAM_CX_SERVER_NAME_NOT_STORED;
        error_occurred = 1;
        goto out;
    }

    if (!error_occurred) {
        /* Set Vendor-Specific-Application-Id AVP */
        ret = ogs_diam_message_vendor_specific_appid_set(
                ans, OGS_DIAM_CX_APPLICATION_ID);
        if (ret != 0) {
            ogs_error("Failed to set Vendor-Specific-Application-Id");
            error_occurred = 1;
            goto out;
        }

        /* Set the Auth-Session-State AVP */
        ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create Auth-Session-State AVP");
            error_occurred = 1;
            goto out;
        }
        val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
        ret = fd_msg_avp_setvalue(avp, &val);
        if (ret != 0) {
            ogs_error("Failed to set Auth-Session-State value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add Auth-Session-State AVP");
            error_occurred = 1;
            goto out;
        }

        /* Set success result code */
        ret = fd_msg_rescode_set(ans, (char*)"DIAMETER_SUCCESS", NULL, NULL, 1);
        if (ret != 0) {
            ogs_error("Failed to set success result code");
            error_occurred = 1;
            goto out;
        }

        /* Set Server-Name AVPs */
        ret = fd_msg_avp_new(ogs_diam_cx_server_name, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create Server-Name AVP");
            error_occurred = 1;
            goto out;
        }
        val.os.data = (uint8_t *)server_name;
        val.os.len  = strlen(server_name);
        ret = fd_msg_avp_setvalue(avp, &val);
        if (ret != 0) {
            ogs_error("Failed to set Server-Name value");
            error_occurred = 1;
            goto out;
        }
        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add Server-Name AVP");
            error_occurred = 1;
            goto out;
        }

        /* Send the answer */
        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send message");
            error_occurred = 1;
            goto out;
        }

        ogs_debug("Tx Location-Info-Answer");

        /* Add to stats */
        OGS_DIAM_STATS_MTX(
            OGS_DIAM_STATS_INC(nb_echoed);
            HSS_DIAM_PRIV_STATS_INC(cx.rx_lir);
            HSS_DIAM_PRIV_STATS_INC(cx.tx_lia);
        )

        /* Cleanup resources */
        if (public_identity) ogs_free(public_identity);

        return 0;
    }

out:
    /* Error handling */
    if (ans) {
        /* Set Vendor-Specific-Application-Id AVP */
        ret = ogs_diam_message_vendor_specific_appid_set(
                ans, OGS_DIAM_CX_APPLICATION_ID);
        if (ret != 0) {
            ogs_warn("Failed to set Vendor-Specific-Application-Id in error");
        }

        /* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
        if (result_code != 0) {
            ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
            if (ret != 0) {
                ogs_warn("Failed to set experimental result code");
            }
        }

        /* Set the Auth-Session-State AVP */
        ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
        if (ret == 0) {
            val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
            ret = fd_msg_avp_setvalue(avp, &val);
            if (ret == 0) {
                ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
                if (ret != 0) {
                    ogs_warn("Failed to add Auth-Session-State AVP in error");
                }
            } else {
                ogs_warn("Failed to set Auth-Session-State value in error");
            }
        } else {
            ogs_warn("Failed to create Auth-Session-State AVP in error");
        }

        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send error response");
        }
    }

    /* Update error stats */
    OGS_DIAM_STATS_MTX(
        HSS_DIAM_PRIV_STATS_INC(cx.rx_lir);
        HSS_DIAM_PRIV_STATS_INC(cx.rx_lir_error);
    )

    /* Cleanup resources */
    if (public_identity) ogs_free(public_identity);

    return 0;
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

    /* Specific handler for Location-Info-Request */
    data.command = ogs_diam_cx_cmd_lir;
    ret = fd_disp_register(hss_ogs_diam_cx_lir_cb, DISP_HOW_CC, &data, NULL,
                &hdl_cx_lir);
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
    if (hdl_cx_lir)
        (void) fd_disp_unregister(&hdl_cx_lir, NULL);
}
