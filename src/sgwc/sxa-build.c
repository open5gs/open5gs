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

#include "sxa-build.h"

ogs_pkbuf_t *sgwc_sxa_build_session_establishment_request(
        uint8_t type, sgwc_sess_t *sess)
{
    ogs_pfcp_message_t pfcp_message;
    ogs_pfcp_session_establishment_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_urr_t *urr = NULL;
    ogs_pfcp_qer_t *qer = NULL;
    int i, rv;

    ogs_pfcp_node_id_t node_id;
    ogs_pfcp_f_seid_t f_seid;
    int len;

    ogs_debug("Session Establishment Request");
    ogs_assert(sess);

    req = &pfcp_message.pfcp_session_establishment_request;
    memset(&pfcp_message, 0, sizeof(ogs_pfcp_message_t));

    /* Node ID */
    rv = ogs_pfcp_sockaddr_to_node_id(
            ogs_pfcp_self()->pfcp_addr, ogs_pfcp_self()->pfcp_addr6,
            ogs_app()->parameter.prefer_ipv4,
            &node_id, &len);
    ogs_expect_or_return_val(rv == OGS_OK, NULL);
    req->node_id.presence = 1;
    req->node_id.data = &node_id;
    req->node_id.len = len;

    /* F-SEID */
    rv = ogs_pfcp_sockaddr_to_f_seid(
            ogs_pfcp_self()->pfcp_addr, ogs_pfcp_self()->pfcp_addr6,
            &f_seid, &len);
    ogs_expect_or_return_val(rv == OGS_OK, NULL);
    f_seid.seid = htobe64(sess->sgwc_sxa_seid);
    req->cp_f_seid.presence = 1;
    req->cp_f_seid.data = &f_seid;
    req->cp_f_seid.len = len;

    ogs_pfcp_pdrbuf_init();

    /* Create PDR */
    i = 0;
    ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
        ogs_pfcp_build_create_pdr(&req->create_pdr[i], i, pdr);
        i++;
    }

    /* Create FAR */
    i = 0;
    ogs_list_for_each(&sess->pfcp.far_list, far) {
        ogs_pfcp_build_create_far(&req->create_far[i], i, far);
        i++;
    }

    /* Create URR */
    i = 0;
    ogs_list_for_each(&sess->pfcp.urr_list, urr) {
        ogs_pfcp_build_create_urr(&req->create_urr[i], i, urr);
        i++;
    }

    /* Create QER */
    i = 0;
    ogs_list_for_each(&sess->pfcp.qer_list, qer) {
        ogs_pfcp_build_create_qer(&req->create_qer[i], i, qer);
        i++;
    }

    /* Create BAR */
    if (sess->pfcp.bar) {
        ogs_pfcp_build_create_bar(&req->create_bar, sess->pfcp.bar);
    }

    pfcp_message.h.type = type;
    pkbuf = ogs_pfcp_build_msg(&pfcp_message);

    ogs_pfcp_pdrbuf_clear();

    return pkbuf;
}

ogs_pkbuf_t *sgwc_sxa_build_sess_modification_request(
        uint8_t type, sgwc_sess_t *sess, uint64_t modify_flags)
{
    ogs_pfcp_message_t pfcp_message;
    ogs_pfcp_session_modification_request_t *req = NULL;
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    sgwc_bearer_t *bearer = NULL;
    sgwc_tunnel_t *tunnel = NULL;

    int num_of_remove_pdr = 0;
    int num_of_remove_far = 0;
    int num_of_create_pdr = 0;
    int num_of_create_far = 0;
    int num_of_update_far = 0;

    ogs_debug("Session Modification Request");
    ogs_assert(sess);
    ogs_assert(modify_flags);

    req = &pfcp_message.pfcp_session_modification_request;
    memset(&pfcp_message, 0, sizeof(ogs_pfcp_message_t));

    ogs_list_for_each(&sess->bearer_list, bearer) {
        ogs_list_for_each(&bearer->tunnel_list, tunnel) {
            if (((modify_flags &
                  (OGS_PFCP_MODIFY_DL_ONLY|
                   OGS_PFCP_MODIFY_UL_ONLY|
                   OGS_PFCP_MODIFY_INDIRECT)) == 0) ||

                ((modify_flags & OGS_PFCP_MODIFY_DL_ONLY) &&
                 (tunnel->interface_type == OGS_GTP2_F_TEID_S5_S8_SGW_GTP_U)) ||

                ((modify_flags & OGS_PFCP_MODIFY_UL_ONLY) &&
                 (tunnel->interface_type == OGS_GTP2_F_TEID_S1_U_SGW_GTP_U)) ||

                (((modify_flags & OGS_PFCP_MODIFY_INDIRECT) &&
                  ((tunnel->interface_type ==
                      OGS_GTP2_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING) ||
                   (tunnel->interface_type ==
                      OGS_GTP2_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING))))) {

                if (modify_flags & OGS_PFCP_MODIFY_REMOVE) {
                    pdr = tunnel->pdr;
                    if (pdr) {
                        ogs_pfcp_tlv_remove_pdr_t *message =
                            &req->remove_pdr[num_of_remove_pdr];

                        message->presence = 1;
                        message->pdr_id.presence = 1;
                        message->pdr_id.u16 = pdr->id;

                        num_of_remove_pdr++;
                    } else
                        ogs_assert_if_reached();

                    far = tunnel->far;
                    if (far) {
                        ogs_pfcp_tlv_remove_far_t *message =
                            &req->remove_far[num_of_remove_far];

                        message->presence = 1;
                        message->far_id.presence = 1;
                        message->far_id.u32 = far->id;

                        num_of_remove_far++;
                    } else
                        ogs_assert_if_reached();

                } else {
                    if (modify_flags & OGS_PFCP_MODIFY_CREATE) {
                        pdr = tunnel->pdr;
                        if (pdr) {
                            ogs_pfcp_build_create_pdr(
                                    &req->create_pdr[num_of_create_pdr],
                                    num_of_create_pdr, pdr);

                            num_of_create_pdr++;
                        } else
                            ogs_assert_if_reached();

                        far = tunnel->far;
                        if (far) {
                            ogs_pfcp_build_create_far(
                                    &req->create_far[num_of_create_far],
                                    num_of_create_far, far);

                            num_of_create_far++;
                        } else
                            ogs_assert_if_reached();
                    }
                    if (modify_flags & OGS_PFCP_MODIFY_DEACTIVATE) {
                        far = tunnel->far;
                        if (far) {
                            ogs_pfcp_build_update_far_deactivate(
                                    &req->update_far[num_of_update_far],
                                    num_of_update_far, far);

                            num_of_update_far++;
                        } else
                            ogs_assert_if_reached();
                    }
                }
            }

        }
    }

    pfcp_message.h.type = type;
    pkbuf = ogs_pfcp_build_msg(&pfcp_message);

    return pkbuf;
}

ogs_pkbuf_t *sgwc_sxa_build_bearer_modification_request(
        uint8_t type, sgwc_bearer_t *bearer, uint64_t modify_flags)
{
    ogs_pfcp_message_t pfcp_message;
    ogs_pfcp_session_modification_request_t *req = NULL;
    sgwc_tunnel_t *tunnel = NULL;
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    int num_of_remove_pdr = 0;
    int num_of_remove_far = 0;
    int num_of_create_pdr = 0;
    int num_of_create_far = 0;
    int num_of_update_far = 0;

    sgwc_sess_t *sess = NULL;

    ogs_debug("Session Modification Request");
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    ogs_assert(modify_flags);

    req = &pfcp_message.pfcp_session_modification_request;
    memset(&pfcp_message, 0, sizeof(ogs_pfcp_message_t));

    if (modify_flags & OGS_PFCP_MODIFY_CREATE) {
        ogs_pfcp_pdrbuf_init();
    }

    ogs_list_for_each(&bearer->tunnel_list, tunnel) {
        if (((modify_flags &
              (OGS_PFCP_MODIFY_DL_ONLY|
               OGS_PFCP_MODIFY_UL_ONLY|
               OGS_PFCP_MODIFY_INDIRECT)) == 0) ||

            ((modify_flags & OGS_PFCP_MODIFY_DL_ONLY) &&
             (tunnel->interface_type == OGS_GTP2_F_TEID_S5_S8_SGW_GTP_U)) ||

            ((modify_flags & OGS_PFCP_MODIFY_UL_ONLY) &&
             (tunnel->interface_type == OGS_GTP2_F_TEID_S1_U_SGW_GTP_U)) ||

            (((modify_flags & OGS_PFCP_MODIFY_INDIRECT) &&
              ((tunnel->interface_type ==
                  OGS_GTP2_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING) ||
               (tunnel->interface_type ==
                  OGS_GTP2_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING))))) {

            if (modify_flags & OGS_PFCP_MODIFY_REMOVE) {
                pdr = tunnel->pdr;
                if (pdr) {
                    ogs_pfcp_tlv_remove_pdr_t *message =
                        &req->remove_pdr[num_of_remove_pdr];

                    message->presence = 1;
                    message->pdr_id.presence = 1;
                    message->pdr_id.u16 = pdr->id;

                    num_of_remove_pdr++;
                } else
                    ogs_assert_if_reached();

                far = tunnel->far;
                if (far) {
                    ogs_pfcp_tlv_remove_far_t *message =
                        &req->remove_far[num_of_remove_far];

                    message->presence = 1;
                    message->far_id.presence = 1;
                    message->far_id.u32 = far->id;

                    num_of_remove_far++;
                } else
                    ogs_assert_if_reached();

            } else {
                if (modify_flags & OGS_PFCP_MODIFY_CREATE) {
                    pdr = tunnel->pdr;
                    if (pdr) {
                        ogs_pfcp_build_create_pdr(
                                &req->create_pdr[num_of_create_pdr],
                                num_of_create_pdr, pdr);

                        num_of_create_pdr++;
                    } else
                        ogs_assert_if_reached();

                    far = tunnel->far;
                    if (far) {
                        ogs_pfcp_build_create_far(
                                &req->create_far[num_of_create_far],
                                num_of_create_far, far);

                        num_of_create_far++;
                    } else
                        ogs_assert_if_reached();
                }

                if (modify_flags & OGS_PFCP_MODIFY_ACTIVATE) {
                    far = tunnel->far;
                    if (far) {
                        if (modify_flags & OGS_PFCP_MODIFY_END_MARKER) {
                            far->smreq_flags.send_end_marker_packets = 1;
                        }

                        ogs_pfcp_build_update_far_activate(
                                &req->update_far[num_of_update_far],
                                num_of_update_far, far);

                        num_of_update_far++;

                        /* Clear all FAR flags */
                        tunnel->far->smreq_flags.value = 0;
                    } else
                        ogs_assert_if_reached();
                }
            }
        }
    }

    pfcp_message.h.type = type;
    pkbuf = ogs_pfcp_build_msg(&pfcp_message);

    if (modify_flags & OGS_PFCP_MODIFY_CREATE) {
        ogs_pfcp_pdrbuf_clear();
    }

    return pkbuf;
}

ogs_pkbuf_t *sgwc_sxa_build_session_deletion_request(
        uint8_t type, sgwc_sess_t *sess)
{
    ogs_pfcp_message_t pfcp_message;

    ogs_debug("Session Deletion Request");
    ogs_assert(sess);

    pfcp_message.h.type = type;
    return ogs_pfcp_build_msg(&pfcp_message);
}
