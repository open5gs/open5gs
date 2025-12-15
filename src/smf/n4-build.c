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

#include "context.h"
#include "n4-build.h"

ogs_pkbuf_t *smf_n4_build_session_establishment_request(
        uint8_t type, smf_sess_t *sess, ogs_pfcp_xact_t *xact)
{
    ogs_pfcp_message_t *pfcp_message = NULL;
    ogs_pfcp_session_establishment_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_urr_t *urr = NULL;
    ogs_pfcp_qer_t *qer = NULL;
    int i, rv;

    ogs_pfcp_node_id_t node_id;
    ogs_pfcp_f_seid_t f_seid;
    char apn_dnn[OGS_MAX_DNN_LEN+1];
    int len;

    smf_ue_t *smf_ue = NULL;
    ogs_pfcp_user_id_t user_id;
    char user_id_buf[sizeof(ogs_pfcp_user_id_t)];

    ogs_debug("Session Establishment Request");
    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);
    ogs_assert(xact);

    pfcp_message = ogs_calloc(1, sizeof(*pfcp_message));
    if (!pfcp_message) {
        ogs_error("ogs_calloc() failed");
        return NULL;
    }

    req = &pfcp_message->pfcp_session_establishment_request;

    /* Node ID */
    rv = ogs_pfcp_sockaddr_to_node_id(&node_id, &len);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_sockaddr_to_node_id() failed");
        ogs_free(pfcp_message);
        return NULL;
    }
    req->node_id.presence = 1;
    req->node_id.data = &node_id;
    req->node_id.len = len;

    /* F-SEID */
    rv = ogs_pfcp_sockaddr_to_f_seid(&f_seid, &len);
    if (rv != OGS_OK) {
        ogs_error("ogs_pfcp_sockaddr_to_f_seid() failed");
        ogs_free(pfcp_message);
        return NULL;
    }
    f_seid.seid = htobe64(sess->smf_n4_seid);
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

    /* PDN Type */
    req->pdn_type.presence = 1;
    req->pdn_type.u8 = sess->session.session_type;

    /* User ID */
    memset(&user_id, 0, sizeof(ogs_pfcp_user_id_t));
    if (smf_ue->imsi_len) {
        user_id.imsif = 1;
        user_id.imsi_len = smf_ue->imsi_len;
        ogs_assert(smf_ue->imsi_len <= OGS_MAX_IMSI_LEN);
        memcpy(user_id.imsi, smf_ue->imsi, smf_ue->imsi_len);
    }
    if (smf_ue->imeisv_len) {
        user_id.imeif = 1;
        user_id.imeisv_len = smf_ue->imeisv_len;
        ogs_assert(smf_ue->imeisv_len <= OGS_MAX_IMEISV_LEN);
        memcpy(user_id.imeisv, smf_ue->imeisv, smf_ue->imeisv_len);
    }
    if (smf_ue->msisdn_len) {
        user_id.msisdnf = 1;
        user_id.msisdn_len = smf_ue->msisdn_len;
        ogs_assert(smf_ue->msisdn_len <= OGS_MAX_MSISDN_LEN);
        memcpy(user_id.msisdn, smf_ue->msisdn, smf_ue->msisdn_len);
    }

    if (user_id.flags) {
        ogs_pfcp_build_user_id(
            &req->user_id, &user_id, user_id_buf, sizeof(user_id_buf));
        req->user_id.presence = 1;
    }

    /* APN/DNN */
    len = ogs_fqdn_build(apn_dnn, sess->session.name, strlen(sess->session.name));
    req->apn_dnn.presence = 1;
    req->apn_dnn.len = len;
    req->apn_dnn.data = apn_dnn;

    /* S-NSSAI */
    if (!sess->epc) {
        req->s_nssai.presence = 1;
        req->s_nssai.len = 4;
        req->s_nssai.data = &sess->s_nssai;
    }

    /* Restoration Indication */
    if (xact->create_flags & OGS_PFCP_CREATE_RESTORATION_INDICATION) {
        ogs_pfcp_sereq_flags_t sereq_flags;
        sereq_flags.value = 0;

        sereq_flags.restoration_indication = 1;
        req->pfcpsereq_flags.presence = 1;
        req->pfcpsereq_flags.u8 = sereq_flags.value;
    }

    pfcp_message->h.type = type;
    pkbuf = ogs_pfcp_build_msg(pfcp_message);
    ogs_expect(pkbuf);

    ogs_pfcp_pdrbuf_clear();
    ogs_free(pfcp_message);

    return pkbuf;
}

ogs_pkbuf_t *smf_n4_build_pdr_to_modify_list(
        uint8_t type, smf_sess_t *sess, ogs_pfcp_xact_t *xact)
{
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_urr_t *urr = NULL;
    int i;

    ogs_pfcp_message_t *pfcp_message = NULL;
    ogs_pfcp_session_modification_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    int num_of_remove_pdr = 0;
    int num_of_remove_far = 0;
    int num_of_create_pdr = 0;
    int num_of_create_far = 0;
    int num_of_update_far = 0;

    uint64_t modify_flags = 0;

    ogs_debug("Session Modification Request");
    ogs_assert(sess);
    ogs_assert(xact);
    modify_flags = xact->modify_flags;
    ogs_assert(modify_flags);

    pfcp_message = ogs_calloc(1, sizeof(*pfcp_message));
    if (!pfcp_message) {
        ogs_error("ogs_calloc() failed");
        return NULL;
    }

    req = &pfcp_message->pfcp_session_modification_request;

    if (modify_flags & OGS_PFCP_MODIFY_CREATE) {
        ogs_pfcp_pdrbuf_init();
    }

    ogs_list_for_each_entry(&sess->pdr_to_modify_list, pdr, to_modify_node) {
        ogs_pfcp_far_t *far = pdr->far;
        ogs_assert(far);

        if (((modify_flags &
              (OGS_PFCP_MODIFY_DL_ONLY|
               OGS_PFCP_MODIFY_UL_ONLY|
               OGS_PFCP_MODIFY_INDIRECT)) == 0) ||

            ((modify_flags & OGS_PFCP_MODIFY_DL_ONLY) &&
             (pdr->src_if == OGS_PFCP_INTERFACE_CORE) &&
             (far->dst_if == OGS_PFCP_INTERFACE_ACCESS)) ||

            ((modify_flags & OGS_PFCP_MODIFY_UL_ONLY) &&
             (pdr->src_if == OGS_PFCP_INTERFACE_ACCESS) &&
             (far->dst_if == OGS_PFCP_INTERFACE_CORE)) ||

            ((modify_flags & OGS_PFCP_MODIFY_INDIRECT) &&
             (pdr->src_if == OGS_PFCP_INTERFACE_ACCESS) &&
             (far->dst_if == OGS_PFCP_INTERFACE_ACCESS))) {

            if (modify_flags & OGS_PFCP_MODIFY_REMOVE) {
                ogs_pfcp_tlv_remove_pdr_t *remove_pdr = NULL;
                ogs_pfcp_tlv_remove_far_t *remove_far = NULL;

                remove_pdr = &req->remove_pdr[num_of_remove_pdr];
                remove_pdr->presence = 1;
                remove_pdr->pdr_id.presence = 1;
                remove_pdr->pdr_id.u16 = pdr->id;
                num_of_remove_pdr++;

                remove_far = &req->remove_far[num_of_remove_far];
                remove_far->presence = 1;
                remove_far->far_id.presence = 1;
                remove_far->far_id.u32 = far->id;
                num_of_remove_far++;
            } else if (modify_flags & OGS_PFCP_MODIFY_CREATE) {
                ogs_pfcp_build_create_pdr(
                        &req->create_pdr[num_of_create_pdr],
                        num_of_create_pdr, pdr);
                num_of_create_pdr++;

                ogs_list_add(&xact->pdr_to_create_list, &pdr->to_create_node);

                ogs_pfcp_build_create_far(
                        &req->create_far[num_of_create_far],
                        num_of_create_far, far);

                num_of_create_far++;
            } else if (modify_flags & OGS_PFCP_MODIFY_ACTIVATE) {
                if (far->apply_action & OGS_PFCP_APPLY_ACTION_FORW) {

                    if (modify_flags & OGS_PFCP_MODIFY_END_MARKER) {
                        far->smreq_flags.send_end_marker_packets = 1;
                    }

                    ogs_pfcp_build_update_far_activate(
                            &req->update_far[num_of_update_far],
                            num_of_update_far, far);

                    /* Clear all FAR flags */
                    far->smreq_flags.value = 0;

                    num_of_update_far++;
                }
            } else if (modify_flags & OGS_PFCP_MODIFY_DEACTIVATE) {
                ogs_pfcp_build_update_far_deactivate(
                        &req->update_far[num_of_update_far],
                        num_of_update_far, far);
                num_of_update_far++;
            } else if (modify_flags == 0) {
                ogs_fatal("Invalid modify_flags = %lld",
                        (long long)modify_flags);
                ogs_assert_if_reached();
            }
        }

    }

    /* Update URR */
    i = 0;
    ogs_list_for_each(&sess->pfcp.urr_list, urr) {
        ogs_pfcp_build_update_urr(&req->update_urr[i], i, urr, modify_flags);
        i++;
    }

    pfcp_message->h.type = type;
    pkbuf = ogs_pfcp_build_msg(pfcp_message);
    ogs_expect(pkbuf);

    if (modify_flags & OGS_PFCP_MODIFY_CREATE) {
        ogs_pfcp_pdrbuf_clear();
    }

    ogs_free(pfcp_message);

    return pkbuf;
}

ogs_pkbuf_t *smf_n4_build_qos_flow_to_modify_list(
        uint8_t type, smf_sess_t *sess, ogs_pfcp_xact_t *xact)
{
    ogs_pfcp_message_t *pfcp_message = NULL;
    ogs_pfcp_session_modification_request_t *req = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    smf_bearer_t *qos_flow = NULL;

    int num_of_remove_pdr = 0;
    int num_of_remove_far = 0;
    int num_of_remove_qer = 0;
    int num_of_remove_urr = 0;
    int num_of_create_pdr = 0;
    int num_of_create_far = 0;
    int num_of_create_qer = 0;
    int num_of_create_urr = 0;
    int num_of_update_pdr = 0;
    int num_of_update_far = 0;
    int num_of_update_qer = 0;
    int num_of_update_urr = 0;

    uint64_t modify_flags = 0;

    ogs_debug("Bearer Modification Request");

    ogs_assert(sess);
    ogs_assert(xact);
    modify_flags = xact->modify_flags;
    ogs_assert(modify_flags);

    pfcp_message = ogs_calloc(1, sizeof(*pfcp_message));
    if (!pfcp_message) {
        ogs_error("ogs_calloc() failed");
        return NULL;
    }

    req = &pfcp_message->pfcp_session_modification_request;

    if (modify_flags &
            (OGS_PFCP_MODIFY_CREATE|
             OGS_PFCP_MODIFY_TFT_NEW|OGS_PFCP_MODIFY_TFT_ADD|
             OGS_PFCP_MODIFY_TFT_REPLACE|OGS_PFCP_MODIFY_TFT_DELETE|
             OGS_PFCP_MODIFY_EPC_TFT_UPDATE)) {
        ogs_pfcp_pdrbuf_init();
    }

    ogs_list_for_each_entry(
            &sess->qos_flow_to_modify_list, qos_flow, to_modify_node) {

        if (modify_flags & OGS_PFCP_MODIFY_REMOVE) {
            /* Remove PDR */
            if (qos_flow->dl_pdr) {
                ogs_pfcp_tlv_remove_pdr_t *message =
                    &req->remove_pdr[num_of_remove_pdr];

                message->presence = 1;
                message->pdr_id.presence = 1;
                message->pdr_id.u16 = qos_flow->dl_pdr->id;
                num_of_remove_pdr++;
            }
            if (qos_flow->ul_pdr) {
                ogs_pfcp_tlv_remove_pdr_t *message =
                    &req->remove_pdr[num_of_remove_pdr];

                message->presence = 1;
                message->pdr_id.presence = 1;
                message->pdr_id.u16 = qos_flow->ul_pdr->id;
                num_of_remove_pdr++;
            }

            /* Remove FAR */
            if (qos_flow->dl_far) {
                ogs_pfcp_tlv_remove_far_t *message =
                    &req->remove_far[num_of_remove_far];

                message->presence = 1;
                message->far_id.presence = 1;
                message->far_id.u32 = qos_flow->dl_far->id;
                num_of_remove_far++;
            }
            if (qos_flow->ul_far) {
                ogs_pfcp_tlv_remove_far_t *message =
                    &req->remove_far[num_of_remove_far];

                message->presence = 1;
                message->far_id.presence = 1;
                message->far_id.u32 = qos_flow->ul_far->id;
                num_of_remove_far++;
            }

            /* Remove QER */
            if (qos_flow->qer) {
                ogs_pfcp_tlv_remove_qer_t *message =
                    &req->remove_qer[num_of_remove_qer];

                message->presence = 1;
                message->qer_id.presence = 1;
                message->qer_id.u32 = qos_flow->qer->id;
                num_of_remove_qer++;
            }

            /* Remove URR */
            if (qos_flow->urr) {
                ogs_pfcp_tlv_remove_urr_t *message =
                    &req->remove_urr[num_of_remove_urr];

                message->presence = 1;
                message->urr_id.presence = 1;
                message->urr_id.u32 = qos_flow->urr->id;
                num_of_remove_urr++;
            }

        } else {
            if (modify_flags & OGS_PFCP_MODIFY_CREATE) {

                /* Create PDR */
                if (qos_flow->dl_pdr) {
                    ogs_pfcp_build_create_pdr(
                            &req->create_pdr[num_of_create_pdr],
                            num_of_create_pdr, qos_flow->dl_pdr);
                    num_of_create_pdr++;

                    ogs_list_add(&xact->pdr_to_create_list,
                                    &qos_flow->dl_pdr->to_create_node);
                }
                if (qos_flow->ul_pdr) {
                    ogs_pfcp_build_create_pdr(
                            &req->create_pdr[num_of_create_pdr],
                            num_of_create_pdr, qos_flow->ul_pdr);
                    num_of_create_pdr++;

                    ogs_list_add(&xact->pdr_to_create_list,
                                    &qos_flow->ul_pdr->to_create_node);
                }

                /* Create FAR */
                if (qos_flow->dl_far) {
                    ogs_pfcp_build_create_far(
                            &req->create_far[num_of_create_far],
                            num_of_create_far, qos_flow->dl_far);
                    num_of_create_far++;
                }
                if (qos_flow->ul_far) {
                    ogs_pfcp_build_create_far(
                            &req->create_far[num_of_create_far],
                            num_of_create_far, qos_flow->ul_far);
                    num_of_create_far++;
                }

                /* Create QER */
                if (qos_flow->qer) {
                    ogs_pfcp_build_create_qer(
                            &req->create_qer[num_of_create_qer],
                            num_of_create_qer, qos_flow->qer);
                    num_of_create_qer++;
                }

                /* Create URR */
                if (qos_flow->urr) {
                    ogs_pfcp_build_create_urr(
                            &req->create_urr[num_of_create_urr],
                            num_of_create_urr, qos_flow->urr);
                    num_of_create_urr++;
                }
            }
            if (modify_flags &
                    (OGS_PFCP_MODIFY_TFT_NEW|OGS_PFCP_MODIFY_TFT_ADD|
                     OGS_PFCP_MODIFY_TFT_REPLACE|OGS_PFCP_MODIFY_TFT_DELETE|
                     OGS_PFCP_MODIFY_EPC_TFT_UPDATE)) {

                /* Update PDR */
                if (qos_flow->dl_pdr) {
                    ogs_pfcp_build_update_pdr(
                            &req->update_pdr[num_of_update_pdr],
                            num_of_update_pdr, qos_flow->dl_pdr, modify_flags);
                    num_of_update_pdr++;
                }
                if (qos_flow->ul_pdr) {
                    ogs_pfcp_build_update_pdr(
                            &req->update_pdr[num_of_update_pdr],
                            num_of_update_pdr, qos_flow->ul_pdr, modify_flags);
                    num_of_update_pdr++;
                }
                if (qos_flow->urr) {
                    ogs_pfcp_build_update_urr(
                            &req->update_urr[num_of_update_urr],
                            num_of_update_urr, qos_flow->urr, modify_flags);
                    num_of_update_urr++;
                }
            }
            if (modify_flags & OGS_PFCP_MODIFY_ACTIVATE) {
                /* Update FAR - Only DL */
                if (qos_flow->dl_far) {
                    if (qos_flow->dl_far->apply_action &
                            OGS_PFCP_APPLY_ACTION_FORW) {

                        if (modify_flags & OGS_PFCP_MODIFY_END_MARKER) {
                            qos_flow->dl_far->
                                smreq_flags.send_end_marker_packets = 1;
                        }

                        ogs_pfcp_build_update_far_activate(
                                &req->update_far[num_of_update_far],
                                num_of_update_far, qos_flow->dl_far);
                        num_of_update_far++;

                        /* Clear all FAR flags */
                        qos_flow->dl_far->smreq_flags.value = 0;
                    }
                }
            } else if (modify_flags & OGS_PFCP_MODIFY_DEACTIVATE) {
                if (qos_flow->dl_far) {
                    ogs_pfcp_build_update_far_deactivate(
                            &req->update_far[num_of_update_far],
                            num_of_update_far, qos_flow->dl_far);
                    num_of_update_far++;
                }
            }
            if (modify_flags &
                    (OGS_PFCP_MODIFY_QOS_MODIFY|
                     OGS_PFCP_MODIFY_EPC_QOS_UPDATE)) {
                /* Update QER */
                if (qos_flow->qer) {
                    ogs_pfcp_build_update_qer(
                            &req->update_qer[num_of_update_qer],
                            num_of_update_qer, qos_flow->qer);
                    num_of_update_qer++;
                }
            }
        }
    }

    pfcp_message->h.type = type;
    pkbuf = ogs_pfcp_build_msg(pfcp_message);
    ogs_expect(pkbuf);

    if (modify_flags &
            (OGS_PFCP_MODIFY_CREATE|
             OGS_PFCP_MODIFY_TFT_NEW|OGS_PFCP_MODIFY_TFT_ADD|
             OGS_PFCP_MODIFY_TFT_REPLACE|OGS_PFCP_MODIFY_TFT_DELETE|
             OGS_PFCP_MODIFY_EPC_TFT_UPDATE)) {
        ogs_pfcp_pdrbuf_clear();
    }

    ogs_free(pfcp_message);

    return pkbuf;
}

ogs_pkbuf_t *smf_n4_build_session_deletion_request(
        uint8_t type, smf_sess_t *sess)
{
    ogs_pfcp_message_t *pfcp_message = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_debug("Session Deletion Request");
    ogs_assert(sess);

    pfcp_message = ogs_calloc(1, sizeof(*pfcp_message));
    if (!pfcp_message) {
        ogs_error("ogs_calloc() failed");
        return NULL;
    }

    pfcp_message->h.type = type;
    pkbuf = ogs_pfcp_build_msg(pfcp_message);
    ogs_expect(pkbuf);

    ogs_free(pfcp_message);

    return pkbuf;
}
