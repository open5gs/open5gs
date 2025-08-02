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
    OGS_DIAM_STATS_MTX(
        HSS_DIAM_PRIV_STATS_INC(swx.rx_unknown);
    )

    return ENOTSUP;
}

/* Callback for incoming Multimedia-Auth-Request messages */
static int hss_ogs_diam_swx_mar_cb(struct msg **msg, struct avp *avp,
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

    int error_occurred = 0;

    ogs_debug("Rx Multimedia-Auth-Request");

    /* Validate input parameters */
    if (!msg || !*msg) {
        ogs_error("Invalid message pointer");
        return EINVAL;
    }

    /* Initialize variables */
    memset(imsi_bcd, 0, sizeof(imsi_bcd));
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

    ogs_extract_digit_from_string(imsi_bcd, user_name);

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

    /* EAP-AKA is only supported */
    if (authentication_scheme &&
        ogs_strcasecmp(authentication_scheme,
            OGS_DIAM_SWX_AUTH_SCHEME_EAP_AKA) != 0) {
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
                ans, OGS_DIAM_SWX_APPLICATION_ID);
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
        val.os.data = (uint8_t *)OGS_DIAM_SWX_AUTH_SCHEME_EAP_AKA;
        val.os.len = strlen(OGS_DIAM_SWX_AUTH_SCHEME_EAP_AKA);
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
            HSS_DIAM_PRIV_STATS_INC(swx.rx_mar);
            HSS_DIAM_PRIV_STATS_INC(swx.tx_maa);
        )

        /* Cleanup resources */
        if (authentication_scheme) ogs_free(authentication_scheme);
        if (user_name) ogs_free(user_name);

        return 0;
    }

out:
    /* Error handling */
    if (ans) {
        /* Set Vendor-Specific-Application-Id AVP */
        ret = ogs_diam_message_vendor_specific_appid_set(
                ans, OGS_DIAM_SWX_APPLICATION_ID);

        /* Set the Experimental-Result, Origin-Host and Origin-Realm AVPs */
        if (result_code != 0) {
            ret = ogs_diam_message_experimental_rescode_set(ans, result_code);
        }

        /* Set the User-Name AVP */
        if (user_name) {
            ret = fd_msg_avp_new(ogs_diam_user_name, 0, &avp);
            if (ret == 0) {
                val.os.data = (uint8_t *)user_name;
                val.os.len = strlen(user_name);
                ret = fd_msg_avp_setvalue(avp, &val);
                if (ret == 0) {
                    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
                }
            }
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
        HSS_DIAM_PRIV_STATS_INC(swx.rx_mar);
        HSS_DIAM_PRIV_STATS_INC(swx.rx_mar_error);
    )

    /* Cleanup resources */
    if (authentication_scheme) ogs_free(authentication_scheme);
    if (user_name) ogs_free(user_name);

    return 0;
}

/* Callback for incoming Server-Assignment-Request messages */
static int hss_ogs_diam_swx_sar_cb(struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    int rv, ret;
    uint32_t result_code = 0;
    int error_occurred = 0;

    struct msg *ans = NULL, *qry = NULL;
    struct avp_hdr *hdr = NULL;
    union avp_value val;

    char *user_name = NULL;
    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];

    ogs_subscription_data_t subscription_data;
    ogs_slice_data_t *slice_data = NULL;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;

    ogs_debug("Rx Server-Assignment-Request");

    /* Validate input parameters */
    if (!msg || !*msg) {
        ogs_error("Invalid message pointer");
        return EINVAL;
    }

    /* Initialize variables */
    memset(imsi_bcd, 0, sizeof(imsi_bcd));
    memset(&subscription_data, 0, sizeof(ogs_subscription_data_t));
    memset(&sin, 0, sizeof(sin));
    memset(&sin6, 0, sizeof(sin6));

    /* Create answer header */
    qry = *msg;
    ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    if (ret != 0) {
        ogs_error("Failed to create answer message");
        error_occurred = 1;
        result_code = OGS_DIAM_UNABLE_TO_DELIVER;
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

    ogs_extract_digit_from_string(imsi_bcd, user_name);

    /* DB : HSS Subscription Data */
    rv = hss_db_subscription_data(imsi_bcd, &subscription_data);
    if (rv != OGS_OK) {
        ogs_error("Cannot get Subscription-Data for IMSI:'%s'", imsi_bcd);
        result_code = OGS_DIAM_S6A_ERROR_USER_UNKNOWN;
        error_occurred = 1;
        goto out;
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
        OGS_DIAM_CX_SERVER_ASSIGNMENT_AAA_USER_DATA_REQUEST) {

        struct avp *avp_subscription_id = NULL;
        struct avp *avp_subscription_id_type = NULL;
        struct avp *avp_subscription_id_data = NULL;
        struct avp *avp_non_3gpp_ip_access = NULL;
        struct avp *avp_non_3gpp_ip_access_apn = NULL;
        struct avp *avp_ambr = NULL;
        struct avp *avp_max_bandwidth_ul = NULL;
        struct avp *avp_max_bandwidth_dl = NULL;

        /* Set the APN Configuration Profile */
        struct avp *context_identifier = NULL;

        int i;

        /* Set the Non-3GPP-User-Data */
        ret = fd_msg_avp_new(ogs_diam_swx_non_3gpp_user_data, 0, &avp);
        if (ret != 0) {
            ogs_error("Failed to create Non-3GPP-User-Data AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        /* Set Subscription-Id */
        if (subscription_data.num_of_msisdn >= 1) {
            ret = fd_msg_avp_new(ogs_diam_subscription_id, 0,
                    &avp_subscription_id);
            if (ret != 0) {
                ogs_error("Failed to create Subscription-Id AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_new(ogs_diam_subscription_id_type, 0,
                    &avp_subscription_id_type);
            if (ret != 0) {
                ogs_error("Failed to create Subscription-Id-Type AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            val.i32 = OGS_DIAM_SUBSCRIPTION_ID_TYPE_END_USER_E164;
            ret = fd_msg_avp_setvalue(avp_subscription_id_type, &val);
            if (ret != 0) {
                ogs_error("Failed to set Subscription-Id-Type value");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(avp_subscription_id, MSG_BRW_LAST_CHILD,
                    avp_subscription_id_type);
            if (ret != 0) {
                ogs_error("Failed to add Subscription-Id-Type AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_new(ogs_diam_subscription_id_data, 0,
                    &avp_subscription_id_data);
            if (ret != 0) {
                ogs_error("Failed to create Subscription-Id-Data AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            val.os.data = (uint8_t *)subscription_data.msisdn[0].bcd;
            val.os.len = strlen(subscription_data.msisdn[0].bcd);
            ret = fd_msg_avp_setvalue(avp_subscription_id_data, &val);
            if (ret != 0) {
                ogs_error("Failed to set Subscription-Id-Data value");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(avp_subscription_id, MSG_BRW_LAST_CHILD,
                    avp_subscription_id_data);
            if (ret != 0) {
                ogs_error("Failed to add Subscription-Id-Data AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_subscription_id);
            if (ret != 0) {
                ogs_error("Failed to add Subscription-Id AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }
        }

        /* Set Non-3GPP-IP-Access */
        ret = fd_msg_avp_new(ogs_diam_swx_non_3gpp_ip_access, 0,
                &avp_non_3gpp_ip_access);
        if (ret != 0) {
            ogs_error("Failed to create Non-3GPP-IP-Access AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        val.i32 = OGS_DIAM_SWX_NON_3GPP_SUBSCRIPTION_ALLOWED;
        ret = fd_msg_avp_setvalue(avp_non_3gpp_ip_access, &val);
        if (ret != 0) {
            ogs_error("Failed to set Non-3GPP-IP-Access value");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_non_3gpp_ip_access);
        if (ret != 0) {
            ogs_error("Failed to add Non-3GPP-IP-Access AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        /* Set Non-3GPP-IP-Access-APN */
        ret = fd_msg_avp_new(ogs_diam_swx_non_3gpp_ip_access_apn, 0,
                &avp_non_3gpp_ip_access_apn);
        if (ret != 0) {
            ogs_error("Failed to create Non-3GPP-IP-Access-APN AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        val.i32 = OGS_DIAM_SWX_NON_3GPP_APNS_ENABLE;
        ret = fd_msg_avp_setvalue(avp_non_3gpp_ip_access_apn, &val);
        if (ret != 0) {
            ogs_error("Failed to set Non-3GPP-IP-Access-APN value");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD,
                avp_non_3gpp_ip_access_apn);
        if (ret != 0) {
            ogs_error("Failed to add Non-3GPP-IP-Access-APN AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        /* Set the AMBR */
        ret = fd_msg_avp_new(ogs_diam_s6a_ambr, 0, &avp_ambr);
        if (ret != 0) {
            ogs_error("Failed to create AMBR AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_new(
                ogs_diam_s6a_max_bandwidth_ul, 0, &avp_max_bandwidth_ul);
        if (ret != 0) {
            ogs_error("Failed to create Max-Bandwidth-UL AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        val.u32 = ogs_uint64_to_uint32(subscription_data.ambr.uplink);
        ret = fd_msg_avp_setvalue(avp_max_bandwidth_ul, &val);
        if (ret != 0) {
            ogs_error("Failed to set Max-Bandwidth-UL value");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_add(
                avp_ambr, MSG_BRW_LAST_CHILD, avp_max_bandwidth_ul);
        if (ret != 0) {
            ogs_error("Failed to add Max-Bandwidth-UL AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_new(
                ogs_diam_s6a_max_bandwidth_dl, 0, &avp_max_bandwidth_dl);
        if (ret != 0) {
            ogs_error("Failed to create Max-Bandwidth-DL AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        val.u32 = ogs_uint64_to_uint32(subscription_data.ambr.downlink);
        ret = fd_msg_avp_setvalue(avp_max_bandwidth_dl, &val);
        if (ret != 0) {
            ogs_error("Failed to set Max-Bandwidth-DL value");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_add(
                avp_ambr, MSG_BRW_LAST_CHILD, avp_max_bandwidth_dl);
        if (ret != 0) {
            ogs_error("Failed to add Max-Bandwidth-DL AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, avp_ambr);
        if (ret != 0) {
            ogs_error("Failed to add AMBR AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        /* For EPC, we'll use first Slice in Subscription */
        if (subscription_data.num_of_slice)
            slice_data = &subscription_data.slice[0];

        if (!slice_data) {
            ogs_error("[%s] Cannot find S-NSSAI", imsi_bcd);
            result_code = OGS_DIAM_S6A_ERROR_UNKNOWN_EPS_SUBSCRIPTION;
            error_occurred = 1;
            goto out;
        }

        if (!slice_data->num_of_session) {
            ogs_error("[%s] No PDN", imsi_bcd);
            result_code = OGS_DIAM_S6A_ERROR_UNKNOWN_EPS_SUBSCRIPTION;
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_new(ogs_diam_s6a_context_identifier, 0,
                &context_identifier);
        if (ret != 0) {
            ogs_error("Failed to create Context-Identifier AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        val.i32 = 1; /* Context Identifier : 1 */
        ret = fd_msg_avp_setvalue(context_identifier, &val);
        if (ret != 0) {
            ogs_error("Failed to set Context-Identifier value");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        ret = fd_msg_avp_add(avp, MSG_BRW_LAST_CHILD, context_identifier);
        if (ret != 0) {
            ogs_error("Failed to add Context-Identifier AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }

        for (i = 0; i < slice_data->num_of_session; i++) {
            /* Set the APN Configuration */
            struct avp *apn_configuration = NULL;
            struct avp *context_identifier_sub = NULL;
            struct avp *pdn_type = NULL;
            struct avp *served_party_ip_address = NULL;
            struct avp *service_selection = NULL;
            struct avp *eps_subscribed_qos_profile = NULL;
            struct avp *qos_class_identifier = NULL;
            struct avp *allocation_retention_priority = NULL;
            struct avp *priority_level = NULL;
            struct avp *pre_emption_capability = NULL;
            struct avp *pre_emption_vulnerability = NULL;
            struct avp *mip6_agent_info = NULL;
            struct avp *mip_home_agent_address = NULL;
            struct avp *pdn_gw_allocation_type = NULL;
            struct avp *vplmn_dynamic_address_allowed = NULL;

            ogs_session_t *session = NULL;

            if (i >= OGS_MAX_NUM_OF_SESS) {
                ogs_warn("Ignore max session count overflow [%d>=%d]",
                    slice_data->num_of_session, OGS_MAX_NUM_OF_SESS);
                break;
            }

            session = &slice_data->session[i];
            if (!session) {
                ogs_error("Invalid session at index %d", i);
                continue;
            }
            session->context_identifier = i+1;

            ret = fd_msg_avp_new(ogs_diam_s6a_apn_configuration, 0,
                &apn_configuration);
            if (ret != 0) {
                ogs_error("Failed to create APN-Configuration AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            /* Set Context-Identifier */
            ret = fd_msg_avp_new(ogs_diam_s6a_context_identifier, 0,
                    &context_identifier_sub);
            if (ret != 0) {
                ogs_error("Failed to create Context-Identifier AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            val.i32 = session->context_identifier;
            ret = fd_msg_avp_setvalue(context_identifier_sub, &val);
            if (ret != 0) {
                ogs_error("Failed to set Context-Identifier value");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(apn_configuration,
                    MSG_BRW_LAST_CHILD, context_identifier_sub);
            if (ret != 0) {
                ogs_error("Failed to add Context-Identifier AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            /* Set PDN-Type */
            ret = fd_msg_avp_new(ogs_diam_s6a_pdn_type, 0, &pdn_type);
            if (ret != 0) {
                ogs_error("Failed to create PDN-Type AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            val.i32 = OGS_PDU_SESSION_TYPE_TO_DIAMETER(session->session_type);
            ret = fd_msg_avp_setvalue(pdn_type, &val);
            if (ret != 0) {
                ogs_error("Failed to set PDN-Type value");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(apn_configuration,
                    MSG_BRW_LAST_CHILD, pdn_type);
            if (ret != 0) {
                ogs_error("Failed to add PDN-Type AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            /* Set Served-Party-IP-Address */
            if ((session->session_type == OGS_PDU_SESSION_TYPE_IPV4 ||
                 session->session_type == OGS_PDU_SESSION_TYPE_IPV4V6) &&
                session->ue_ip.ipv4) {
                ret = fd_msg_avp_new(ogs_diam_s6a_served_party_ip_address,
                        0, &served_party_ip_address);
                if (ret != 0) {
                    ogs_error("Failed to create Served-Party-IP-Address AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                sin.sin_family = AF_INET;
                sin.sin_addr.s_addr = session->ue_ip.addr;
                ret = fd_msg_avp_value_encode(&sin, served_party_ip_address);
                if (ret != 0) {
                    ogs_error("Failed to encode IPv4 address");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                ret = fd_msg_avp_add(apn_configuration, MSG_BRW_LAST_CHILD,
                        served_party_ip_address);
                if (ret != 0) {
                    ogs_error("Failed to add Served-Party-IP-Address AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }
            }

            if ((session->session_type == OGS_PDU_SESSION_TYPE_IPV6 ||
                 session->session_type == OGS_PDU_SESSION_TYPE_IPV4V6) &&
                session->ue_ip.ipv6) {
                ret = fd_msg_avp_new(ogs_diam_s6a_served_party_ip_address,
                        0, &served_party_ip_address);
                if (ret != 0) {
                    ogs_error("Failed to create Served-Party-IP-Address AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                sin6.sin6_family = AF_INET6;
                memcpy(sin6.sin6_addr.s6_addr,
                        session->ue_ip.addr6, OGS_IPV6_LEN);
                ret = fd_msg_avp_value_encode(&sin6, served_party_ip_address);
                if (ret != 0) {
                    ogs_error("Failed to encode IPv6 address");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                ret = fd_msg_avp_add(apn_configuration, MSG_BRW_LAST_CHILD,
                        served_party_ip_address);
                if (ret != 0) {
                    ogs_error("Failed to add Served-Party-IP-Address AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }
            }

            /* Set Service-Selection */
            ret = fd_msg_avp_new(ogs_diam_service_selection, 0,
                    &service_selection);
            if (ret != 0) {
                ogs_error("Failed to create Service-Selection AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            if (!session->name) {
                ogs_error("Session name is NULL");
                result_code = OGS_DIAM_INVALID_AVP_VALUE;
                error_occurred = 1;
                goto out;
            }

            val.os.data = (uint8_t *)session->name;
            val.os.len = strlen(session->name);
            ret = fd_msg_avp_setvalue(service_selection, &val);
            if (ret != 0) {
                ogs_error("Failed to set Service-Selection value");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(apn_configuration,
                    MSG_BRW_LAST_CHILD, service_selection);
            if (ret != 0) {
                ogs_error("Failed to add Service-Selection AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            /* Set the EPS Subscribed QoS Profile */
            ret = fd_msg_avp_new(ogs_diam_s6a_eps_subscribed_qos_profile, 0,
                    &eps_subscribed_qos_profile);
            if (ret != 0) {
                ogs_error("Failed to create EPS-Subscribed-QoS-Profile AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_new(ogs_diam_s6a_qos_class_identifier, 0,
                    &qos_class_identifier);
            if (ret != 0) {
                ogs_error("Failed to create QoS-Class-Identifier AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            val.i32 = session->qos.index;
            ret = fd_msg_avp_setvalue(qos_class_identifier, &val);
            if (ret != 0) {
                ogs_error("Failed to set QoS-Class-Identifier value");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(eps_subscribed_qos_profile,
                    MSG_BRW_LAST_CHILD, qos_class_identifier);
            if (ret != 0) {
                ogs_error("Failed to add QoS-Class-Identifier AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            /* Set Allocation retention priority */
            ret = fd_msg_avp_new(ogs_diam_s6a_allocation_retention_priority, 0,
                    &allocation_retention_priority);
            if (ret != 0) {
                ogs_error("Failed to create Allocation-Retention-Priority");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_new(
                    ogs_diam_s6a_priority_level, 0, &priority_level);
            if (ret != 0) {
                ogs_error("Failed to create Priority-Level AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            val.u32 = session->qos.arp.priority_level;
            ret = fd_msg_avp_setvalue(priority_level, &val);
            if (ret != 0) {
                ogs_error("Failed to set Priority-Level value");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(allocation_retention_priority,
                MSG_BRW_LAST_CHILD, priority_level);
            if (ret != 0) {
                ogs_error("Failed to add Priority-Level AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_new(ogs_diam_s6a_pre_emption_capability, 0,
                    &pre_emption_capability);
            if (ret != 0) {
                ogs_error("Failed to create Pre-emption-Capability AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            val.u32 = OGS_EPC_PRE_EMPTION_DISABLED;
            if (session->qos.arp.pre_emption_capability ==
                    OGS_5GC_PRE_EMPTION_ENABLED)
                val.u32 = OGS_EPC_PRE_EMPTION_ENABLED;
            ret = fd_msg_avp_setvalue(pre_emption_capability, &val);
            if (ret != 0) {
                ogs_error("Failed to set Pre-emption-Capability value");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(allocation_retention_priority,
                MSG_BRW_LAST_CHILD, pre_emption_capability);
            if (ret != 0) {
                ogs_error("Failed to add Pre-emption-Capability AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_new(ogs_diam_s6a_pre_emption_vulnerability, 0,
                    &pre_emption_vulnerability);
            if (ret != 0) {
                ogs_error("Failed to create Pre-emption-Vulnerability AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            val.u32 = OGS_EPC_PRE_EMPTION_DISABLED;
            if (session->qos.arp.pre_emption_vulnerability ==
                    OGS_5GC_PRE_EMPTION_ENABLED)
                val.u32 = OGS_EPC_PRE_EMPTION_ENABLED;
            ret = fd_msg_avp_setvalue(pre_emption_vulnerability, &val);
            if (ret != 0) {
                ogs_error("Failed to set Pre-emption-Vulnerability value");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(allocation_retention_priority,
                MSG_BRW_LAST_CHILD, pre_emption_vulnerability);
            if (ret != 0) {
                ogs_error("Failed to add Pre-emption-Vulnerability AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(eps_subscribed_qos_profile,
                MSG_BRW_LAST_CHILD, allocation_retention_priority);
            if (ret != 0) {
                ogs_error("Failed to add Allocation-Retention-Priority AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(apn_configuration,
                MSG_BRW_LAST_CHILD, eps_subscribed_qos_profile);
            if (ret != 0) {
                ogs_error("Failed to add EPS-Subscribed-QoS-Profile AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            /* Set MIP6-Agent-Info */
            if (session->smf_ip.ipv4 || session->smf_ip.ipv6) {
                ret = fd_msg_avp_new(ogs_diam_mip6_agent_info, 0,
                            &mip6_agent_info);
                if (ret != 0) {
                    ogs_error("Failed to create MIP6-Agent-Info AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                if (session->smf_ip.ipv4) {
                    ret = fd_msg_avp_new(ogs_diam_mip_home_agent_address, 0,
                                &mip_home_agent_address);
                    if (ret != 0) {
                        ogs_error("Failed to create MIP-Home-Agent-Address");
                        result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                        error_occurred = 1;
                        goto out;
                    }

                    sin.sin_family = AF_INET;
                    sin.sin_addr.s_addr = session->smf_ip.addr;
                    ret = fd_msg_avp_value_encode(&sin, mip_home_agent_address);
                    if (ret != 0) {
                        ogs_error("Failed to encode SMF IPv4 address");
                        result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                        error_occurred = 1;
                        goto out;
                    }

                    ret = fd_msg_avp_add(mip6_agent_info,
                            MSG_BRW_LAST_CHILD, mip_home_agent_address);
                    if (ret != 0) {
                        ogs_error("Failed to add MIP-Home-Agent-Address AVP");
                        result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                        error_occurred = 1;
                        goto out;
                    }
                }

                if (session->smf_ip.ipv6) {
                    ret = fd_msg_avp_new(ogs_diam_mip_home_agent_address, 0,
                                &mip_home_agent_address);
                    if (ret != 0) {
                        ogs_error("Failed to create MIP-Home-Agent-Address");
                        result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                        error_occurred = 1;
                        goto out;
                    }

                    sin6.sin6_family = AF_INET6;
                    memcpy(sin6.sin6_addr.s6_addr, session->smf_ip.addr6,
                            sizeof session->smf_ip.addr6);
                    ret = fd_msg_avp_value_encode(&sin6, mip_home_agent_address);
                    if (ret != 0) {
                        ogs_error("Failed to encode SMF IPv6 address");
                        result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                        error_occurred = 1;
                        goto out;
                    }

                    ret = fd_msg_avp_add(mip6_agent_info,
                            MSG_BRW_LAST_CHILD, mip_home_agent_address);
                    if (ret != 0) {
                        ogs_error("Failed to add MIP-Home-Agent-Address AVP");
                        result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                        error_occurred = 1;
                        goto out;
                    }
                }

                ret = fd_msg_avp_add(apn_configuration,
                        MSG_BRW_LAST_CHILD, mip6_agent_info);
                if (ret != 0) {
                    ogs_error("Failed to add MIP6-Agent-Info AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                ret = fd_msg_avp_new(ogs_diam_s6a_pdn_gw_allocation_type, 0,
                            &pdn_gw_allocation_type);
                if (ret != 0) {
                    ogs_error("Failed to create PDN-GW-Allocation-Type AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                val.u32 = OGS_DIAM_S6A_PDN_GW_ALLOCATION_DYNAMIC;
                ret = fd_msg_avp_setvalue(pdn_gw_allocation_type, &val);
                if (ret != 0) {
                    ogs_error("Failed to set PDN-GW-Allocation-Type value");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                ret = fd_msg_avp_add(apn_configuration,
                        MSG_BRW_LAST_CHILD, pdn_gw_allocation_type);
                if (ret != 0) {
                    ogs_error("Failed to add PDN-GW-Allocation-Type AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }
            }

            /* Set VPLMN-Dynamic-Address-Allowed */
            ret = fd_msg_avp_new(ogs_diam_s6a_vplmn_dynamic_address_allowed, 0,
                    &vplmn_dynamic_address_allowed);
            if (ret != 0) {
                ogs_error("Failed to create VPLMN-Dynamic-Address-Allowed");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            val.u32 = OGS_DIAM_S6A_VPLMN_DYNAMIC_ADDRESS_ALLOWED;
            ret = fd_msg_avp_setvalue(vplmn_dynamic_address_allowed, &val);
            if (ret != 0) {
                ogs_error("Failed to set VPLMN-Dynamic-Address-Allowed value");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            ret = fd_msg_avp_add(apn_configuration,
                    MSG_BRW_LAST_CHILD, vplmn_dynamic_address_allowed);
            if (ret != 0) {
                ogs_error("Failed to add VPLMN-Dynamic-Address-Allowed AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }

            /* Set AMBR */
            if (session->ambr.downlink || session->ambr.uplink) {
                struct avp *session_ambr = NULL;
                struct avp *session_max_bandwidth_ul = NULL;
                struct avp *session_max_bandwidth_dl = NULL;

                ret = fd_msg_avp_new(ogs_diam_s6a_ambr, 0, &session_ambr);
                if (ret != 0) {
                    ogs_error("Failed to create session AMBR AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                ret = fd_msg_avp_new(ogs_diam_s6a_max_bandwidth_ul, 0,
                            &session_max_bandwidth_ul);
                if (ret != 0) {
                    ogs_error("Failed to create session Max-Bandwidth-UL");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                val.u32 = ogs_uint64_to_uint32(session->ambr.uplink);
                ret = fd_msg_avp_setvalue(session_max_bandwidth_ul, &val);
                if (ret != 0) {
                    ogs_error("Failed to set session Max-Bandwidth-UL value");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                ret = fd_msg_avp_add(session_ambr, MSG_BRW_LAST_CHILD,
                            session_max_bandwidth_ul);
                if (ret != 0) {
                    ogs_error("Failed to add session Max-Bandwidth-UL AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                ret = fd_msg_avp_new(ogs_diam_s6a_max_bandwidth_dl, 0,
                            &session_max_bandwidth_dl);
                if (ret != 0) {
                    ogs_error("Failed to create session Max-Bandwidth-DL");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                val.u32 = ogs_uint64_to_uint32(session->ambr.downlink);
                ret = fd_msg_avp_setvalue(session_max_bandwidth_dl, &val);
                if (ret != 0) {
                    ogs_error("Failed to set session Max-Bandwidth-DL value");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                ret = fd_msg_avp_add(session_ambr, MSG_BRW_LAST_CHILD,
                            session_max_bandwidth_dl);
                if (ret != 0) {
                    ogs_error("Failed to add session Max-Bandwidth-DL AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }

                ret = fd_msg_avp_add(apn_configuration,
                        MSG_BRW_LAST_CHILD, session_ambr);
                if (ret != 0) {
                    ogs_error("Failed to add session AMBR AVP");
                    result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                    error_occurred = 1;
                    goto out;
                }
            }

            ret = fd_msg_avp_add(avp,
                    MSG_BRW_LAST_CHILD, apn_configuration);
            if (ret != 0) {
                ogs_error("Failed to add APN-Configuration AVP");
                result_code = OGS_DIAM_UNABLE_TO_DELIVER;
                error_occurred = 1;
                goto out;
            }
        }

        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        if (ret != 0) {
            ogs_error("Failed to add Non-3GPP-User-Data AVP");
            result_code = OGS_DIAM_UNABLE_TO_DELIVER;
            error_occurred = 1;
            goto out;
        }
    }

    if (!error_occurred) {
        /* Set Vendor-Specific-Application-Id AVP */
        ret = ogs_diam_message_vendor_specific_appid_set(
                ans, OGS_DIAM_SWX_APPLICATION_ID);
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

        /* Set the Origin-Host, Origin-Realm, and Result-Code AVPs */
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

        /* Send the answer */
        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send message");
            error_occurred = 1;
            goto out;
        }

        ogs_debug("Tx Server-Assignment-Answer");

        /* Add this value to the stats */
        OGS_DIAM_STATS_MTX(
            OGS_DIAM_STATS_INC(nb_echoed);
            HSS_DIAM_PRIV_STATS_INC(swx.rx_sar);
            HSS_DIAM_PRIV_STATS_INC(swx.tx_saa);
        )

        /* Cleanup resources */
        ogs_subscription_data_free(&subscription_data);
        if (user_name) ogs_free(user_name);

        return 0;
    }

out:
    /* Error handling */
    if (ans) {
        /* Set Vendor-Specific-Application-Id AVP */
        ret = ogs_diam_message_vendor_specific_appid_set(
                ans, OGS_DIAM_SWX_APPLICATION_ID);

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

        /* Set the User-Name AVP */
        if (user_name) {
            ret = fd_msg_avp_new(ogs_diam_user_name, 0, &avp);
            if (ret == 0) {
                val.os.data = (uint8_t *)user_name;
                val.os.len = strlen(user_name);
                ret = fd_msg_avp_setvalue(avp, &val);
                if (ret == 0) {
                    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
                }
            }
        }

        ret = fd_msg_send(msg, NULL, NULL);
        if (ret != 0) {
            ogs_error("Failed to send error response");
        }
    }

    /* Update error stats */
    OGS_DIAM_STATS_MTX(
        HSS_DIAM_PRIV_STATS_INC(swx.rx_sar);
        HSS_DIAM_PRIV_STATS_INC(swx.rx_sar_error);
    )

    /* Cleanup resources */
    ogs_subscription_data_free(&subscription_data);
    if (user_name) ogs_free(user_name);

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
