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

#include "pfcp-path.h"
#include "gtp-path.h"
#include "sxa-handler.h"

void sgwu_sxa_handle_session_establishment_request(
        sgwu_sess_t *sess, ogs_pfcp_xact_t *xact, 
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

    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("Session Establishment Request");

    cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_error("No Context");
        ogs_pfcp_send_error_message(xact, 0,
                OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE,
                OGS_PFCP_CAUSE_MANDATORY_IE_MISSING, 0);
        return;
    }

    /* PFCPSEReq-Flags */
    memset(&sereq_flags, 0, sizeof(sereq_flags));
    if (req->pfcpsereq_flags.presence == 1) {
        sereq_flags.value = req->pfcpsereq_flags.u8;
    }

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

    for (i = 0; i < OGS_MAX_NUM_OF_QER; i++) {
        if (ogs_pfcp_handle_create_qer(&sess->pfcp, &req->create_qer[i],
                    &cause_value, &offending_ie_value) == NULL)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

    ogs_pfcp_handle_create_bar(&sess->pfcp, &req->create_bar,
                &cause_value, &offending_ie_value);
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

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
            if (pdr->f_teid.ch == false && pdr->f_teid_len)
                ogs_pfcp_pdr_swap_teid(pdr);
        }
        restoration_indication = true;
    }

    /* Setup GTP Node */
    ogs_list_for_each(&sess->pfcp.far_list, far) {
        if (OGS_ERROR == ogs_pfcp_setup_far_gtpu_node(far)) {
            ogs_fatal("CHECK CONFIGURATION: sgwu.gtpu");
            ogs_fatal("ogs_pfcp_setup_far_gtpu_node() failed");
            goto cleanup;
        }
        if (far->gnode)
            ogs_pfcp_far_f_teid_hash_set(far);
    }

    for (i = 0; i < num_of_created_pdr; i++) {
        pdr = created_pdr[i];
        ogs_assert(pdr);

        /* Setup TEID Hash */
        if (pdr->f_teid_len)
            ogs_pfcp_object_teid_hash_set(
                    OGS_PFCP_OBJ_PDR_TYPE, pdr, restoration_indication);
    }

    /* Send Buffered Packet to gNB */
    ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
        if (pdr->src_if == OGS_PFCP_INTERFACE_CORE) { /* Downlink */
            ogs_pfcp_send_buffered_packet(pdr);
        }
    }

    if (restoration_indication == true ||
        ogs_pfcp_self()->up_function_features.ftup == 0)
        ogs_assert(OGS_OK ==
            sgwu_pfcp_send_session_establishment_response(
                xact, sess, NULL, 0));
    else
        ogs_assert(OGS_OK ==
            sgwu_pfcp_send_session_establishment_response(
                xact, sess, created_pdr, num_of_created_pdr));
    return;

cleanup:
    ogs_pfcp_sess_clear(&sess->pfcp);
    ogs_pfcp_send_error_message(xact, sess ? sess->sgwc_sxa_f_seid.seid : 0,
            OGS_PFCP_SESSION_ESTABLISHMENT_RESPONSE_TYPE,
            cause_value, offending_ie_value);
}

void sgwu_sxa_handle_session_modification_request(
        sgwu_sess_t *sess, ogs_pfcp_xact_t *xact, 
        ogs_pfcp_session_modification_request_t *req)
{
    ogs_pfcp_pdr_t *pdr = NULL;
    ogs_pfcp_far_t *far = NULL;
    ogs_pfcp_pdr_t *created_pdr[OGS_MAX_NUM_OF_PDR];
    int num_of_created_pdr = 0;
    uint8_t cause_value = 0;
    uint8_t offending_ie_value = 0;
    int i;

    ogs_assert(xact);
    ogs_assert(req);

    ogs_debug("Session Modification Request");

    cause_value = OGS_PFCP_CAUSE_REQUEST_ACCEPTED;

    if (!sess) {
        ogs_error("No Context");
        ogs_pfcp_send_error_message(xact, 0,
                OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE,
                OGS_PFCP_CAUSE_SESSION_CONTEXT_NOT_FOUND, 0);
        return;
    }

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
        if (ogs_pfcp_handle_remove_pdr(&sess->pfcp, &req->remove_pdr[i],
                &cause_value, &offending_ie_value) == false)
            break;
    }
    if (cause_value != OGS_PFCP_CAUSE_REQUEST_ACCEPTED)
        goto cleanup;

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

    /* Setup GTP Node */
    ogs_list_for_each(&sess->pfcp.far_list, far) {
        if (OGS_ERROR == ogs_pfcp_setup_far_gtpu_node(far)) {
            ogs_fatal("CHECK CONFIGURATION: sgwu.gtpu");
            ogs_fatal("ogs_pfcp_setup_far_gtpu_node() failed");
            goto cleanup;
        }
        if (far->gnode)
            ogs_pfcp_far_f_teid_hash_set(far);
    }

    for (i = 0; i < num_of_created_pdr; i++) {
        pdr = created_pdr[i];
        ogs_assert(pdr);

        /* Setup TEID Hash */
        if (pdr->f_teid_len)
            ogs_pfcp_object_teid_hash_set(OGS_PFCP_OBJ_PDR_TYPE, pdr, false);
    }

    /* Send Buffered Packet to gNB */
    ogs_list_for_each(&sess->pfcp.pdr_list, pdr) {
        if (pdr->src_if == OGS_PFCP_INTERFACE_CORE) { /* Downlink */
            ogs_pfcp_send_buffered_packet(pdr);
        }
    }

    if (ogs_pfcp_self()->up_function_features.ftup == 0)
        ogs_assert(OGS_OK ==
            sgwu_pfcp_send_session_modification_response(
                xact, sess, NULL, 0));
    else
        ogs_assert(OGS_OK ==
            sgwu_pfcp_send_session_modification_response(
                xact, sess, created_pdr, num_of_created_pdr));
    return;

cleanup:
    ogs_pfcp_sess_clear(&sess->pfcp);
    ogs_pfcp_send_error_message(xact, sess ? sess->sgwc_sxa_f_seid.seid : 0,
            OGS_PFCP_SESSION_MODIFICATION_RESPONSE_TYPE,
            cause_value, offending_ie_value);
}

void sgwu_sxa_handle_session_deletion_request(
        sgwu_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_deletion_request_t *req)
{
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

    ogs_assert(sess);

    sgwu_pfcp_send_session_deletion_response(xact, sess);

    sgwu_sess_remove(sess);
}

void sgwu_sxa_handle_session_report_response(
        sgwu_sess_t *sess, ogs_pfcp_xact_t *xact,
        ogs_pfcp_session_report_response_t *rsp)
{
    uint8_t cause_value = 0;

    ogs_assert(xact);
    ogs_assert(rsp);

    ogs_pfcp_xact_commit(xact);

    ogs_debug("Session report resopnse");

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
    }
}
