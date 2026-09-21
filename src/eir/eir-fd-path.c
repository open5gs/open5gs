/*
 * Copyright (C) 2026 by LetMeConnect
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

#include "context.h"
#include "eir-fd-path.h"

/* handler for fallback cb */
static struct disp_hdl *hdl_s13_fb = NULL;
/* handler for ME-Identity-Check-Request cb */
static struct disp_hdl *hdl_s13_ecr = NULL;

/* Default callback for the application. */
static int eir_ogs_diam_s13_fb_cb(struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    /* This CB should never be called */
    ogs_warn("Unexpected S13 message received!");

    return ENOTSUP;
}

static uint32_t equipment_status_from_dbi(ogs_dbi_eir_status_t status)
{
    switch (status) {
    case OGS_DBI_EIR_STATUS_WHITELISTED:
        return OGS_DIAM_S13_EQUIPMENT_WHITELIST;
    case OGS_DBI_EIR_STATUS_BLACKLISTED:
        return OGS_DIAM_S13_EQUIPMENT_BLACKLIST;
    case OGS_DBI_EIR_STATUS_GREYLISTED:
        return OGS_DIAM_S13_EQUIPMENT_GREYLIST;
    default:
        /* ogs_dbi_eir_check_equipment() never returns OGS_OK with an
         * unsupported status */
        ogs_assert_if_reached();
        return OGS_DIAM_S13_EQUIPMENT_BLACKLIST;
    }
}

/*
 * Callback for incoming ME-Identity-Check-Request (TS 29.272 7.2.19).
 *
 * Same lookup and the same outcomes as the N5g-eir handler: the (SUPI, PEI)
 * pair built from User-Name and Terminal-Information is checked against the
 * eir collection. A known device is answered with Equipment-Status, an
 * unknown one with DIAMETER_ERROR_EQUIPMENT_UNKNOWN, a database problem
 * with DIAMETER_UNABLE_TO_COMPLY. Runs in a freeDiameter thread.
 */
static int eir_ogs_diam_s13_ecr_cb(struct msg **msg, struct avp *avp,
        struct session *session, void *opaque, enum disp_action *act)
{
    int ret, rv;
    struct msg *ans = NULL, *qry = NULL;
    struct avp *avpch = NULL;
    struct avp_hdr *hdr = NULL;
    union avp_value val;

    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN+1];
    char supi[sizeof(OGS_ID_SUPI_TYPE_IMSI "-") + OGS_MAX_IMSI_BCD_LEN];
    char pei[OGS_DIAM_S13_MAX_PEI_LEN+1];
    const char *imei = NULL, *svn = NULL;
    size_t imei_len = 0, svn_len = 0;

    ogs_dbi_eir_record_t record;
    uint32_t equipment_status = 0;
    bool found = false;
    const char *base_error = NULL;  /* Result-Code, base protocol errors */
    uint32_t exp_error = 0;         /* Experimental-Result-Code, 3GPP */

    ogs_assert(msg);
    ogs_assert(*msg);

    ogs_debug("Rx ME-Identity-Check-Request");

    /* Create answer header */
    qry = *msg;
    ret = fd_msg_new_answer_from_req(fd_g_config->cnf_dict, msg, 0);
    ogs_assert(ret == 0);
    ans = *msg;

    /* User-Name: the IMSI */
    ret = fd_msg_search_avp(qry, ogs_diam_user_name, &avp);
    ogs_assert(ret == 0);
    if (!avp) {
        ogs_error("No User-Name AVP");
        base_error = "DIAMETER_MISSING_AVP";
        goto out;
    }
    ret = fd_msg_avp_hdr(avp, &hdr);
    ogs_assert(ret == 0);
    if (!hdr->avp_value->os.data || hdr->avp_value->os.len == 0 ||
            hdr->avp_value->os.len > OGS_MAX_IMSI_BCD_LEN) {
        ogs_error("Invalid User-Name AVP length [%u]",
                (unsigned)hdr->avp_value->os.len);
        base_error = "DIAMETER_INVALID_AVP_VALUE";
        goto out;
    }
    ogs_cpystrn(imsi_bcd, (const char *)hdr->avp_value->os.data,
            hdr->avp_value->os.len + 1);
    if (!ogs_imsi_bcd_is_valid(imsi_bcd)) {
        ogs_error("Invalid User-Name IMSI");
        base_error = "DIAMETER_INVALID_AVP_VALUE";
        goto out;
    }

    /* Terminal-Information { IMEI, [Software-Version] } */
    ret = fd_msg_search_avp(qry, ogs_diam_s13_terminal_information, &avp);
    ogs_assert(ret == 0);
    if (!avp) {
        ogs_error("No Terminal-Information AVP");
        base_error = "DIAMETER_MISSING_AVP";
        goto out;
    }
    ret = fd_avp_search_avp(avp, ogs_diam_s13_imei, &avpch);
    ogs_assert(ret == 0);
    if (!avpch) {
        ogs_error("No IMEI AVP in Terminal-Information");
        base_error = "DIAMETER_MISSING_AVP";
        goto out;
    }
    ret = fd_msg_avp_hdr(avpch, &hdr);
    ogs_assert(ret == 0);
    imei = (const char *)hdr->avp_value->os.data;
    imei_len = hdr->avp_value->os.len;

    ret = fd_avp_search_avp(avp, ogs_diam_s13_software_version, &avpch);
    ogs_assert(ret == 0);
    if (avpch) {
        ret = fd_msg_avp_hdr(avpch, &hdr);
        ogs_assert(ret == 0);
        svn = (const char *)hdr->avp_value->os.data;
        svn_len = hdr->avp_value->os.len;
    }

    if (ogs_diam_s13_pei_from_terminal_info(
                imei, imei_len, svn, svn_len, pei, sizeof(pei)) != OGS_OK) {
        ogs_error("Invalid Terminal-Information AVP");
        base_error = "DIAMETER_INVALID_AVP_VALUE";
        goto out;
    }
    ogs_snprintf(supi, sizeof(supi), "%s-%s", OGS_ID_SUPI_TYPE_IMSI, imsi_bcd);

    ogs_debug("    ME-Identity-Check [supi:%s,pei:%s]", supi, pei);

    /* The N5g-eir handler queries the same mongoc client from the
     * main thread */
    ogs_thread_mutex_lock(&eir_self()->db_lock);
    rv = ogs_dbi_eir_check_equipment(supi, pei, &record);
    ogs_thread_mutex_unlock(&eir_self()->db_lock);

    switch (rv) {
    case OGS_OK:
        equipment_status = equipment_status_from_dbi(record.status);
        ogs_dbi_eir_record_free(&record);
        found = true;
        ogs_info("Equipment status [pei:%s,status:%u]", pei, equipment_status);
        break;
    case OGS_NOTFOUND:
        ogs_info("Unknown equipment [pei:%s]", pei);
        exp_error = OGS_DIAM_S13_ERROR_EQUIPMENT_UNKNOWN;
        break;
    default:
        ogs_error("EIR database lookup failed [pei:%s,rv:%d]", pei, rv);
        base_error = "DIAMETER_UNABLE_TO_COMPLY";
        break;
    }

out:
    if (found) {
        /* Result-Code, Origin-Host and Origin-Realm */
        ret = fd_msg_rescode_set(ans, (char *)"DIAMETER_SUCCESS",
                NULL, NULL, 1);
        ogs_assert(ret == 0);

        /* Equipment-Status */
        ret = fd_msg_avp_new(ogs_diam_s13_equipment_status, 0, &avp);
        ogs_assert(ret == 0);
        val.i32 = equipment_status;
        ret = fd_msg_avp_setvalue(avp, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
        ogs_assert(ret == 0);
    } else if (exp_error) {
        /* Experimental-Result, Origin-Host and Origin-Realm */
        ret = ogs_diam_message_experimental_rescode_set(ans, exp_error);
        ogs_assert(ret == 0);
    } else {
        ogs_assert(base_error);
        ret = fd_msg_rescode_set(ans, (char *)base_error, NULL, NULL, 1);
        ogs_assert(ret == 0);
    }

    /* Auth-Session-State */
    ret = fd_msg_avp_new(ogs_diam_auth_session_state, 0, &avp);
    ogs_assert(ret == 0);
    val.i32 = OGS_DIAM_AUTH_SESSION_NO_STATE_MAINTAINED;
    ret = fd_msg_avp_setvalue(avp, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(ans, MSG_BRW_LAST_CHILD, avp);
    ogs_assert(ret == 0);

    /* Vendor-Specific-Application-Id */
    ret = ogs_diam_message_vendor_specific_appid_set(
            ans, OGS_DIAM_S13_APPLICATION_ID);
    ogs_assert(ret == 0);

    /* Send the answer */
    ret = fd_msg_send(msg, NULL, NULL);
    ogs_assert(ret == 0);

    ogs_debug("Tx ME-Identity-Check-Answer");

    OGS_DIAM_STATS_MTX(
        OGS_DIAM_STATS_INC(nb_echoed);
    )

    return 0;
}

int eir_fd_init(void)
{
    int ret;
    struct disp_when data;

    ret = ogs_diam_init(FD_MODE_SERVER,
                eir_self()->diam_conf_path, eir_self()->diam_config);
    ogs_assert(ret == 0);

    /* Install objects definitions for this application */
    ret = ogs_diam_s13_init();
    ogs_assert(ret == 0);

    memset(&data, 0, sizeof(data));
    data.app = ogs_diam_s13_application;

    /* Fallback CB if command != unexpected message received */
    ret = fd_disp_register(eir_ogs_diam_s13_fb_cb, DISP_HOW_APPID, &data,
                NULL, &hdl_s13_fb);
    ogs_assert(ret == 0);

    /* Specific handler for ME-Identity-Check-Request */
    data.command = ogs_diam_s13_cmd_ecr;
    ret = fd_disp_register(eir_ogs_diam_s13_ecr_cb, DISP_HOW_CC, &data,
                NULL, &hdl_s13_ecr);
    ogs_assert(ret == 0);

    /* Advertise the support for the application in the peer */
    ret = fd_disp_app_support(ogs_diam_s13_application, ogs_diam_vendor, 1, 0);
    ogs_assert(ret == 0);

    ret = ogs_diam_start();
    ogs_assert(ret == 0);

    return OGS_OK;
}

void eir_fd_final(void)
{
    if (hdl_s13_fb)
        (void) fd_disp_unregister(&hdl_s13_fb, NULL);
    if (hdl_s13_ecr)
        (void) fd_disp_unregister(&hdl_s13_ecr, NULL);

    ogs_diam_final();
}
