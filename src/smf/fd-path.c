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

#include "fd-path.h"

int smf_fd_init(void)
{
    int rv;

    if (smf_self()->diam_conf_path == NULL &&
        (smf_self()->diam_config->cnf_diamid == NULL ||
        smf_self()->diam_config->cnf_diamrlm == NULL ||
        smf_self()->diam_config->cnf_addr == NULL)) {
        ogs_warn("No diameter configuration");
        return OGS_OK;
    }

    rv = ogs_diam_init(FD_MODE_CLIENT|FD_MODE_SERVER,
                smf_self()->diam_conf_path, smf_self()->diam_config);
    ogs_assert(rv == 0);

    rv = smf_gx_init();
    ogs_assert(rv == OGS_OK);

    rv = smf_gy_init();
    ogs_assert(rv == OGS_OK);

    rv = ogs_diam_rx_init();
    ogs_assert(rv == 0);
    rv = smf_s6b_init();
    ogs_assert(rv == OGS_OK);

    rv = ogs_diam_start();
    ogs_assert(rv == 0);

    return OGS_OK;
}

void smf_fd_final(void)
{
    if (smf_self()->diam_conf_path == NULL &&
        (smf_self()->diam_config->cnf_diamid == NULL ||
        smf_self()->diam_config->cnf_diamrlm == NULL ||
        smf_self()->diam_config->cnf_addr == NULL)) {
        return;
    }

    smf_gx_final();
    smf_gy_final();
    smf_s6b_final();

    ogs_diam_final();
}

/* Append 3GPP-User-Location-Info, 3GPP TS 29.061 16.4.7.2 22 */
void smf_fd_msg_avp_add_3gpp_uli(smf_sess_t *sess, struct msg *req)
{
    struct avp *avpch1;
    union avp_value val;
    ogs_gtp2_uli_t uli;
    int16_t uli_len;
    uint8_t uli_buf[OGS_GTP2_MAX_ULI_LEN], reencoded_uli_buf[OGS_GTP2_MAX_ULI_LEN];
    uint8_t uli_type;
    int ret;
    ogs_gtp2_tlv_uli_t reencoded_uli;

    if (sess->gtp.user_location_information.presence == 0)
        return;

    if (sess->gtp.version == 1) {
        /* For GTPv1C, it's a 1-1 coding match with TS 29.060 7.7.51: */
        ret = fd_msg_avp_new(
                ogs_diam_gy_3gpp_user_location_info, 0, &avpch1);
        ogs_assert(ret == 0);
        val.os.data = sess->gtp.user_location_information.data;
        val.os.len = sess->gtp.user_location_information.len;
        ret = fd_msg_avp_setvalue(avpch1, &val);
        ogs_assert(ret == 0);
        ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avpch1);
        ogs_assert(ret == 0);
        return;
    }

    /* GTPv2C and Diameter 3GPP-User-Location-Information encoding don't match */
    uli_len = ogs_gtp2_parse_uli(
            &uli, &sess->gtp.user_location_information);
    if (sess->gtp.user_location_information.len != uli_len) {
        ogs_error("Invalid User Location Information(ULI)");
        return;
    }

    ogs_assert(sess->gtp.user_location_information.data);
    ogs_assert(sess->gtp.user_location_information.len);
    memcpy(&uli_buf, sess->gtp.user_location_information.data,
            sess->gtp.user_location_information.len);

    /* Update Gy ULI Type */
    if (uli.flags.cgi) {
        uli_type = OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_CGI;
        uli.flags.octet = 0; uli.flags.cgi = 1;
    } else if (uli.flags.sai) {
        uli_type = OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_SAI;
        uli.flags.octet = 0; uli.flags.sai = 1;
    } else if (uli.flags.rai) {
        uli_type = OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_RAI;
        uli.flags.octet = 0; uli.flags.rai = 1;
    } else if (uli.flags.tai && uli.flags.e_cgi) {
        uli_type = OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_TAI_AND_ECGI;
        uli.flags.octet = 0; uli.flags.tai = 1;  uli.flags.e_cgi = 1;
    } else if (uli.flags.tai && uli.flags.enodeb_id) {
        uli_type = OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_TAI_AND_ENODEB_ID;
        uli.flags.octet = 0; uli.flags.tai = 1;  uli.flags.enodeb_id = 1;
    } else if (uli.flags.tai && uli.flags.ext_enodeb_id) {
        uli_type = OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_TAI_AND_EXT_ENODEB_ID;
        uli.flags.octet = 0; uli.flags.tai = 1;  uli.flags.ext_enodeb_id = 1;
    } else if (uli.flags.tai) {
        uli_type = OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_TAI;
        uli.flags.octet = 0; uli.flags.tai = 1;
    } else if (uli.flags.e_cgi) {
        uli_type = OGS_DIAM_GY_3GPP_USER_LOCATION_INFO_TYPE_ECGI;
        uli.flags.octet = 0; uli.flags.e_cgi = 1;
    } else {
        ogs_error("Unexpected ULI content, unable to convert to Diameter!");
        return;
    }

    /* Reencode ULI dropping unrelated values: */
    uli_len = ogs_gtp2_build_uli(&reencoded_uli, &uli,
            reencoded_uli_buf, sizeof(reencoded_uli_buf));
    reencoded_uli_buf[0] = uli_type;

    ret = fd_msg_avp_new(
            ogs_diam_gy_3gpp_user_location_info, 0, &avpch1);
    ogs_assert(ret == 0);
    val.os.data = (uint8_t *)&reencoded_uli_buf;
    val.os.len = uli_len;
    ret = fd_msg_avp_setvalue(avpch1, &val);
    ogs_assert(ret == 0);
    ret = fd_msg_avp_add(req, MSG_BRW_LAST_CHILD, avpch1);
    ogs_assert(ret == 0);

}
