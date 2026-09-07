/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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
#include "pfcp-path.h"
#include "gtp-path.h"
#include "n4-handler.h"

static void upf_n4_handle_create_urr(upf_sess_t *sess,
        ogs_pfcp_tlv_create_urr_t *create_urr,
        uint8_t *cause_value, uint8_t *offending_ie_value)
{
    int i;

    *cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;

    for (i = 0; i < OGS_MAX_NUM_OF_URR; i++) {
        if (!ogs_pfcp_handle_create_urr(&sess->pfcp, &create_urr[i],
                    cause_value, offending_ie_value))
            return;
    }
}

/*
 * Start new URR timers only after every rejectable step has succeeded.
 * Cleanup can then free new URRs without leaving timer callbacks behind.
 * This does not change timer ownership for previously installed URRs.
 */
static void upf_n4_setup_urr_timers(upf_sess_t *sess,
        ogs_pfcp_tlv_create_urr_t *create_urr)
{
    ogs_pfcp_urr_t *urr = NULL;
    int i;

    for (i = 0; i < OGS_MAX_NUM_OF_URR; i++) {
        if (!create_urr[i].presence)
            break;

        /* A Create followed by Remove leaves no URR to start. */
        urr = ogs_pfcp_urr_find(&sess->pfcp, create_urr[i].urr_id.u32);
        if (!urr) {
            ogs_debug("Skip timers for removed URR-ID[%u]",
                    create_urr[i].urr_id.u32);
            continue;
        }
        if (urr->meas_info.istm) {
            ogs_debug("Start timers for accepted URR-ID[%u]", urr->id);
            upf_sess_urr_acc_timers_setup(sess, urr);
        }
    }
}

/* Accepted and rejected modifications both account for surviving QERs. */
static void upf_n4_update_qer_metrics(upf_sess_t *sess, int old_count)
{
    int count = ogs_list_count(&sess->pfcp.qer_list);
    int delta = count - old_count;

    if (delta) {
        ogs_debug("Update QoS-flow gauge [QER:%d -> %d delta:%+d]",
                old_count, count, delta);
        upf_metrics_inst_by_dnn_add(sess->apn_dnn,
                UPF_METR_GAUGE_UPF_QOSFLOWS, delta);
    }
}

void upf_n4_handle_session_establishment_request(
        upf_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_establishment_request_t *req)
{
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_pdr_t *created_pdr[OGS_MAX_NUM_OF_PDR];
    int num_of_created_pdr = 0;
    uint8_t cause_value = 0;
    uint8_t offending_ie_value = 0;
    int i;

    ogs_pfcp_sereq_flags_t sereq_flags;
    bool restoration_indication = false;

    upf_metrics_inst_global_inc(UPF_METR_GLOB_CTR_SM_N4SESSIONESTABREQ);

    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("Session Establishment Request");

    cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_error("No Context");
        ogs_pfcp_send_error_message(xact, 0,
                OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE,
                OGS_PFCP_CAUSE_MANDATORY_IE_MISSING, 0);
        upf_metrics_inst_by_cause_add(OGS_PFCP_CAUSE_MANDATORY_IE_MISSING,
                UPF_METR_CTR_SM_N4SESSIONESTABFAIL, 1);
        return;
    }

    memset(&sereq_flags, 0, sizeof(sereq_flags));
    if (req->pfcpsereq_flags.presence == 1)
        sereq_flags.value = req->pfcpsereq_flags.u8;

    for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
        created_pdr[i] = ogs_pfcp_handle_create_pdr(&sess->pfcp,
                &req->create_pdr[i], &sereq_flags,
                &cause_value, &offending_ie_value);
        if (created_pdr[i] == NULL)
            break;
    }
    num_of_created_pdr = i;
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    for (i = 0; i < OGS_MAX_NUM_OF_FAR; i++) {
        if (ogs_pfcp_handle_create_far(&sess->pfcp, &req->create_far[i],
                    &cause_value, &offending_ie_value) == NULL)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    upf_n4_handle_create_urr(
            sess, &req->create_urr[0], &cause_value, &offending_ie_value);
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    if (req->apn_dnn.presence) {
        char apn_dnn[OGS_MAX_DNN_LEN+1];

        if (ogs_fqdn_parse(apn_dnn, req->apn_dnn.data,
            ogs_min(req->apn_dnn.len, OGS_MAX_DNN_LEN)) <= 0) {
            ogs_error("Invalid APN");
            cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_INCORRECT;
            goto cleanup;
        }

        if (sess->apn_dnn)
            ogs_free(sess->apn_dnn);
        sess->apn_dnn = ogs_strdup(apn_dnn);
        ogs_assert(sess->apn_dnn);
    }

    for (i = 0; i < OGS_MAX_NUM_OF_QER; i++) {
        if (ogs_pfcp_handle_create_qer(&sess->pfcp, &req->create_qer[i],
                    &cause_value, &offending_ie_value) == NULL)
            break;
        upf_metrics_inst_by_dnn_add(sess->apn_dnn,
                UPF_METR_GAUGE_UPF_QOSFLOWS, 1);
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    ogs_pfcp_handle_create_bar(&sess->pfcp, &req->create_bar,
                &cause_value, &offending_ie_value);
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    /* Setup GTP Node */
    ogs_list_for_each(&sess->pfcp.far_list, far) {
        if (OGS_ERROR == ogs_pfcp_setup_far_gtpu_node(far)) {
            ogs_fatal("CHECK CONFIGURATION: upf.gtpu");
            ogs_fatal("ogs_pfcp_setup_far_gtpu_node() failed");
            cause_value = OGS_PFCP_CAUSE_SYSTEM_FAILURE;
            goto cleanup;
        }
        if (far->gnode)
            ogs_pfcp_far_f_teid_hash_set(far);
    }

    /* PFCPSEReq-Flags */
    if (sereq_flags.restoration_indication == 1) {
        for (i = 0; i < num_of_created_pdr; i++) {
            pdr = created_pdr[i];
            ogs_assert(pdr);

    /*
     * Only perform TEID restoration via swap when F-TEID.ch is false.
     *
     * When F-TEID.ch is false, it means the TEID has already been assigned, and
     * the restoration process can safely perform the swap.
     *
     * If F-TEID.ch is true, it indicates that the UPF needs to assign
     * a new TEID for the first time, so performing a swap is not appropriate
     * in this case.
     */
            if (pdr->f_teid_len > 0 && pdr->f_teid.ch == false) {
                cause_value = ogs_pfcp_pdr_swap_teid(pdr);
                if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
                    goto cleanup;
            }
        }
        restoration_indication = true;
    }

    for (i = 0; i < num_of_created_pdr; i++) {
        pdr = created_pdr[i];
        ogs_assert(pdr);

        /* Setup UE IP address */
        if (pdr->ue_ip_addr_len) {
            if (req->pdn_type.presence == 1) {
                cause_value = upf_sess_set_ue_ip(sess, req->pdn_type.u8, pdr);
                if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
                    goto cleanup;
            } else {
                ogs_error("No PDN Type");
            }
        }

        if (pdr->ipv4_framed_routes) {
            cause_value =
                upf_sess_set_ue_ipv4_framed_routes(sess,
                        pdr->ipv4_framed_routes);
            if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
                goto cleanup;
        }

        if (pdr->ipv6_framed_routes) {
            cause_value =
                upf_sess_set_ue_ipv6_framed_routes(sess,
                        pdr->ipv6_framed_routes);
            if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
                goto cleanup;
        }

        /* Setup UPF-N3-TEID & QFI Hash */
        if (pdr->f_teid_len) {
            cause_value = ogs_pfcp_object_teid_hash_set(
                    OGS_PFCP_OBJ_SESS_TYPE, pdr);
            if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
                goto cleanup;
        }
    }

    upf_n4_setup_urr_timers(sess, &req->create_urr[0]);

    /* Send Buffered Packet to gNB/SGW */
    ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
        if (pdr->src_if == OGS_PFCP_INTERFACE_CORE) { /* Downlink */
            ogs_pfcp_send_buffered_gtpu(pdr);
        }
    }

    if (restoration_indication == true ||
        ogs_pfcp_self()->up_function_features.ftup == 0)
        ogs_assert(OGS_OK ==
            upf_pfcp_send_session_establishment_response(
                xact, sess, NULL, 0));
    else
        ogs_assert(OGS_OK ==
            upf_pfcp_send_session_establishment_response(
                xact, sess, created_pdr, num_of_created_pdr));

    return;

cleanup:
    upf_metrics_inst_by_cause_add(cause_value,
            UPF_METR_CTR_SM_N4SESSIONESTABFAIL, 1);
    ogs_pfcp_sess_clear(&sess->pfcp);
    ogs_pfcp_send_error_message(xact, sess ? sess->smf_n4_f_seid.seid : 0,
            OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE,
            cause_value, offending_ie_value);
}

void upf_n4_handle_session_modification_request(
        upf_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_modification_request_t *req)
{
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_pdr_t *created_pdr[OGS_MAX_NUM_OF_PDR];
    int num_of_created_pdr = 0;
    ogs_pfcp_sess_mark_t mark;
    uint8_t failed_rule_type = 0;
    uint32_t failed_rule_id = 0;
    uint8_t cause_value = 0;
    uint8_t offending_ie_value = 0;
    int i, j;

    ogs_assert(xact);
    ogs_assert(req);

    cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_error("Session Modification has no context [xid:%u]", xact->xid);
        ogs_pfcp_send_error_message(xact, 0,
                OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE,
                OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND, 0);
        return;
    }

    ogs_info("Session Modification Request [xid:%u] "
            "[UP-SEID:0x%llx CP-SEID:0x%llx]",
            xact->xid,
            (unsigned long long)sess->upf_n4_seid,
            (unsigned long long)sess->smf_n4_f_seid.seid);

    /*
     * Validate all Create IDs before mutation. A preflight rejection needs
     * no cleanup because no rule has been created, changed or removed yet.
     */
    if (!ogs_pfcp_validate_create_rules(&sess->pfcp, req,
                &failed_rule_type, &failed_rule_id)) {
        ogs_error("Reject modification before changes [xid:%u] "
                "[UP-SEID:0x%llx CP-SEID:0x%llx] "
                "[rule-type:%u rule-id:%u]",
                xact->xid,
                (unsigned long long)sess->upf_n4_seid,
                (unsigned long long)sess->smf_n4_f_seid.seid,
                failed_rule_type, failed_rule_id);
        ogs_pfcp_send_session_modification_rule_error(xact,
                sess->smf_n4_f_seid.seid, failed_rule_type, failed_rule_id);
        return;
    }

    ogs_pfcp_sess_mark(&sess->pfcp, &mark);

    for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
        created_pdr[i] = ogs_pfcp_handle_create_pdr(&sess->pfcp,
                &req->create_pdr[i], NULL, &cause_value, &offending_ie_value);
        if (created_pdr[i] == NULL)
            break;
    }
    num_of_created_pdr = i;
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
        if (ogs_pfcp_handle_update_pdr(
                    OGS_PFCP_OBJ_SESS_TYPE,
                    &sess->pfcp, &req->update_pdr[i],
                    &cause_value, &offending_ie_value) == NULL)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    for (i = 0; i < OGS_MAX_NUM_OF_PDR; i++) {
        if (ogs_pfcp_handle_remove_pdr(&sess->pfcp, &req->remove_pdr[i],
                &cause_value, &offending_ie_value) == false)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    /*
     * Removed PDR pointers are compared only, never dereferenced. Exclude
     * them from subsequent hash setup and the Created PDR response list.
     */
    for (i = 0, j = 0; i < num_of_created_pdr; i++) {
        if (ogs_list_exists(&sess->pfcp.pdr_list, created_pdr[i]))
            created_pdr[j++] = created_pdr[i];
    }
    if (j != num_of_created_pdr)
        ogs_error("[xid:%u] Omit %d removed PDRs from Created PDRs",
                xact->xid, num_of_created_pdr - j);
    num_of_created_pdr = j;

    for (i = 0; i < OGS_MAX_NUM_OF_FAR; i++) {
        if (ogs_pfcp_handle_create_far(&sess->pfcp, &req->create_far[i],
                    &cause_value, &offending_ie_value) == NULL)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    for (i = 0; i < OGS_MAX_NUM_OF_FAR; i++) {
        if (ogs_pfcp_handle_update_far_flags(&sess->pfcp, &req->update_far[i],
                    &cause_value, &offending_ie_value) == NULL)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    /* Send End Marker to gNB */
    ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
        if (pdr->src_if == OGS_PFCP_INTERFACE_CORE) { /* Downlink */
            far = pdr->far;
            if (far && far->smreq_flags.send_end_marker_packets)
                ogs_assert(OGS_ERROR != ogs_pfcp_send_end_marker(pdr));
        }
    }
    /* Clear PFCPSMReq-Flags */
    ogs_list_for_each(&sess->pfcp.far_list, far)
        far->smreq_flags.value = 0;

    for (i = 0; i < OGS_MAX_NUM_OF_FAR; i++) {
        if (ogs_pfcp_handle_update_far(&sess->pfcp, &req->update_far[i],
                    &cause_value, &offending_ie_value) == NULL)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    for (i = 0; i < OGS_MAX_NUM_OF_FAR; i++) {
        if (ogs_pfcp_handle_remove_far(&sess->pfcp, &req->remove_far[i],
                &cause_value, &offending_ie_value) == false)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    upf_n4_handle_create_urr(
            sess, &req->create_urr[0], &cause_value, &offending_ie_value);
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    for (i = 0; i < OGS_MAX_NUM_OF_URR; i++) {
        if (ogs_pfcp_handle_update_urr(&sess->pfcp, &req->update_urr[i],
                    &cause_value, &offending_ie_value) == NULL)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    for (i = 0; i < OGS_MAX_NUM_OF_URR; i++) {
        if (ogs_pfcp_handle_remove_urr(&sess->pfcp, &req->remove_urr[i],
                &cause_value, &offending_ie_value) == false)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    for (i = 0; i < OGS_MAX_NUM_OF_QER; i++) {
        if (ogs_pfcp_handle_create_qer(&sess->pfcp, &req->create_qer[i],
                    &cause_value, &offending_ie_value) == NULL)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    for (i = 0; i < OGS_MAX_NUM_OF_QER; i++) {
        if (ogs_pfcp_handle_update_qer(&sess->pfcp, &req->update_qer[i],
                    &cause_value, &offending_ie_value) == NULL)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    for (i = 0; i < OGS_MAX_NUM_OF_QER; i++) {
        if (ogs_pfcp_handle_remove_qer(&sess->pfcp, &req->remove_qer[i],
                &cause_value, &offending_ie_value) == false)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    ogs_pfcp_handle_create_bar(&sess->pfcp, &req->create_bar,
                &cause_value, &offending_ie_value);
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    ogs_pfcp_handle_remove_bar(&sess->pfcp, &req->remove_bar,
            &cause_value, &offending_ie_value);
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    /* Unchanged FARs are visited too; shared F-TEIDs are normal here. */
    ogs_list_for_each(&sess->pfcp.far_list, far) {
        if (OGS_ERROR == ogs_pfcp_setup_far_gtpu_node(far)) {
            ogs_fatal("CHECK CONFIGURATION: upf.gtpu");
            ogs_fatal("ogs_pfcp_setup_far_gtpu_node() failed");
            cause_value = OGS_PFCP_CAUSE_SYSTEM_FAILURE;
            goto cleanup;
        }
        if (far->gnode)
            ogs_pfcp_far_f_teid_hash_set(far);
    }

    for (i = 0; i < num_of_created_pdr; i++) {
        pdr = created_pdr[i];
        ogs_assert(pdr);

        /* Setup UPF-N3-TEID & QFI Hash */
        if (pdr->f_teid_len) {
            cause_value = ogs_pfcp_object_teid_hash_set(
                    OGS_PFCP_OBJ_SESS_TYPE, pdr);
            if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
                goto cleanup;
        }
    }

    upf_n4_setup_urr_timers(sess, &req->create_urr[0]);
    upf_n4_update_qer_metrics(sess, mark.num_of_qer);

    /* Send Buffered Packet to gNB/SGW */
    ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
        if (pdr->src_if == OGS_PFCP_INTERFACE_CORE) { /* Downlink */
            ogs_pfcp_send_buffered_gtpu(pdr);
        }
    }

    if (ogs_pfcp_self()->up_function_features.ftup == 0)
        ogs_assert(OGS_OK ==
            upf_pfcp_send_session_modification_response(
                xact, sess, NULL, 0));
    else
        ogs_assert(OGS_OK ==
            upf_pfcp_send_session_modification_response(
                xact, sess, created_pdr, num_of_created_pdr));
    return;

cleanup:
    ogs_error("Reject modification [xid:%u] "
            "[UP-SEID:0x%llx CP-SEID:0x%llx] "
            "[Cause:%u Offending-IE:%u]",
            xact->xid,
            (unsigned long long)sess->upf_n4_seid,
            (unsigned long long)sess->smf_n4_f_seid.seid,
            cause_value, offending_ie_value);
    /*
     * Keep applied Updates and Removes; reclaim only newly created rules.
     * Common cleanup logs each removed ID. Count survivors only here,
     * on rejection, so normal requests avoid diagnostic list walks.
     * Offending-IE == 0 means no Offending IE is included in the response.
     */
    ogs_pfcp_sess_clear_since_mark(&sess->pfcp, &mark);
    upf_n4_update_qer_metrics(sess, mark.num_of_qer);
    ogs_info("Rejected modification cleanup complete [xid:%u] "
            "[UP-SEID:0x%llx CP-SEID:0x%llx] "
            "[PDR:%d FAR:%d URR:%d QER:%d BAR:%d]",
            xact->xid,
            (unsigned long long)sess->upf_n4_seid,
            (unsigned long long)sess->smf_n4_f_seid.seid,
            ogs_list_count(&sess->pfcp.pdr_list),
            ogs_list_count(&sess->pfcp.far_list),
            ogs_list_count(&sess->pfcp.urr_list),
            ogs_list_count(&sess->pfcp.qer_list), sess->pfcp.bar != NULL);
    ogs_pfcp_send_error_message(xact, sess ? sess->smf_n4_f_seid.seid : 0,
            OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE,
            cause_value, offending_ie_value);
}

void upf_n4_handle_session_deletion_request(
        upf_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_deletion_request_t *req)
{
    ogs_pfcp_qer_t *qer = NULL;

    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("Session Deletion Request");

    if (!sess) {
        ogs_error("No Context");
        ogs_pfcp_send_error_message(xact, 0,
                OGS_PFCP_SESSION_DELETION_RESPONSE_TYPE,
                OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND, 0);
        return;
    }
    upf_pfcp_send_session_deletion_response(xact, sess);

    ogs_list_for_each(&sess->pfcp.qer_list, qer) {
        upf_metrics_inst_by_dnn_add(sess->apn_dnn,
                UPF_METR_GAUGE_UPF_QOSFLOWS, -1);
    }
    upf_sess_remove(sess);
}

void upf_n4_handle_session_report_response(
        upf_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_report_response_t *rsp)
{
    uint8_t cause_value = 0;

    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_pfcp_xact_commit(xact);

    ogs_debug("Session Report Response");

    cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_warn("No Context");
        cause_value = OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND;
    }

    if (rsp->cause.presence) {
        if (rsp->cause.u8 != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
            ogs_error("PFCP Cause[%d] : Not Accepted", rsp->cause.u8);
            cause_value = rsp->cause.u8;
        }
    } else {
        ogs_error("No Cause");
        cause_value = OGS_PFCP_CAUSE_MANDATORY_IE_MISSING;
    }

    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED) {
        ogs_error("Cause request not accepted[%d]", cause_value);
        return;
    } else {
        upf_metrics_inst_global_inc(UPF_METR_GLOB_CTR_SM_N4SESSIONREPORTSUCC);
    }

}
