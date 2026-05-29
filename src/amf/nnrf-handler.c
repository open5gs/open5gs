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

#include "sbi-path.h"
#include "nas-path.h"
#include "ngap-path.h"
#include "nnrf-handler.h"


static bool amf_nnrf_try_old_amf_discovery_fallback(
        amf_ue_t *amf_ue, OpenAPI_nf_type_e requester_nf_type,
        ogs_sbi_discovery_option_t *discovery_option)
{
    int r;

    ogs_assert(amf_ue);

    /*
     * TS 23.502
     * 4.2.2.2.2 General Registration
     * If the SUCI is not provided by the UE nor retrieved from the old AMF,
     * the Identity Request procedure is initiated by AMF sending an Identity
     * Request message to the UE requesting the SUCI.
     */
    if (amf_ue->nas.message_type != OGS_NAS_5GS_REGISTRATION_REQUEST ||
            amf_ue->nas.registration.value !=
                OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL ||
            requester_nf_type != OpenAPI_nf_type_AMF ||
            !discovery_option || !discovery_option->guami_presence)
        return false;

    /*
     * Returns true only when Identity Request is sent.
     * The context transfer state is reset even if SUCI/SUPI already exists,
     * matching the original old-AMF discovery fallback behavior.
     */
    amf_ue->amf_ue_context_transfer_state = UE_CONTEXT_INITIAL_STATE;

    if (AMF_UE_HAVE_SUCI(amf_ue) || AMF_UE_HAVE_SUPI(amf_ue))
        return false;

    CLEAR_AMF_UE_TIMER(amf_ue->t3570);
    r = nas_5gs_send_identity_request(amf_ue);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return true;
}

static void amf_nnrf_send_session_failure_to_ran(
        amf_ue_t *amf_ue, amf_sess_t *sess, ogs_pool_id_t ran_ue_id)
{
    int r;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(amf_ue);
    ogs_assert(sess);

    if (ran_ue_id < OGS_MIN_POOL_ID || ran_ue_id > OGS_MAX_POOL_ID) {
        ogs_error("[%s:%s:%d:%d] No assoc RAN-UE id [%d]",
                amf_ue->supi, amf_ue->suci, sess->psi, sess->pti,
                (int)ran_ue_id);
        return;
    }

    ran_ue = ran_ue_find_by_id(ran_ue_id);
    if (!ran_ue) {
        ogs_error("[%s:%s:%d:%d] NG Context has already been removed",
                amf_ue->supi, amf_ue->suci, sess->psi, sess->pti);
        return;
    }

    if (ran_ue->amf_ue_id == OGS_INVALID_POOL_ID) {
        ogs_error("[%s:%s:%d:%d] RAN-UE has already been deassociated",
                amf_ue->supi, amf_ue->suci, sess->psi, sess->pti);
        return;
    }

    if (amf_ue->id != ran_ue->amf_ue_id) {
        ogs_error("[%s:%s:%d:%d] AMF-UE mismatched [%d!=%d]",
                amf_ue->supi, amf_ue->suci, sess->psi, sess->pti,
                amf_ue->id, ran_ue->amf_ue_id);
        return;
    }

    if (sess->payload_container_type) {
        r = nas_5gs_send_back_gsm_message(
                ran_ue, sess,
                OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                AMF_NAS_BACKOFF_TIME);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else {
        r = ngap_send_error_indication2(
                ran_ue,
                NGAP_Cause_PR_transport,
                NGAP_CauseTransport_transport_resource_unavailable);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }
}

void amf_nnrf_handle_nf_discover(
        ogs_sbi_xact_t *xact, ogs_sbi_message_t *recvmsg)
{
    int r;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_object_t *sbi_object = NULL;
    ogs_pool_id_t sbi_object_id = OGS_INVALID_POOL_ID;
    ogs_sbi_service_type_e service_type = OGS_SBI_SERVICE_TYPE_NULL;
    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;
    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;

    ogs_pool_id_t ran_ue_id = OGS_INVALID_POOL_ID;

    OpenAPI_search_result_t *SearchResult = NULL;

    ogs_assert(recvmsg);
    ogs_assert(xact);
    sbi_object = xact->sbi_object;
    ogs_assert(sbi_object);
    service_type = xact->service_type;
    ogs_assert(service_type);
    target_nf_type = ogs_sbi_service_type_to_nf_type(service_type);
    ogs_assert(target_nf_type);
    requester_nf_type = xact->requester_nf_type;
    ogs_assert(requester_nf_type);

    sbi_object_id = xact->sbi_object_id;
    ogs_assert(sbi_object_id >= OGS_MIN_POOL_ID &&
            sbi_object_id <= OGS_MAX_POOL_ID);

    discovery_option = xact->discovery_option;

    SearchResult = recvmsg->SearchResult;
    if (!SearchResult) {
        ogs_error("No SearchResult");
        amf_nnrf_handle_failed_amf_discovery(xact);
        return;
    }

    if (sbi_object->type == OGS_SBI_OBJ_UE_TYPE) {
        amf_ue = amf_ue_find_by_id(sbi_object_id);
        ogs_assert(amf_ue);
    } else if (sbi_object->type == OGS_SBI_OBJ_SESS_TYPE) {
        sess = amf_sess_find_by_id(sbi_object_id);
        ogs_assert(sess);
        amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
        ogs_assert(amf_ue);

        if (xact->user_data) {
            amf_sbi_xact_ctx_t *ctx = xact->user_data;

            if (ctx->ran_ue_id != OGS_INVALID_POOL_ID)
                ran_ue_id = ctx->ran_ue_id;
        }

        /*
         * ran_ue_id can remain OGS_INVALID_POOL_ID if this
         * transaction was created without an associated NG context.
         * Resolve ran_ue only where it is actually needed below.
         */
    } else {
        ogs_fatal("(NF discover) Not implemented [%s:%d]",
            ogs_sbi_service_type_to_name(service_type), sbi_object->type);
        ogs_assert_if_reached();
    }

    ogs_nnrf_disc_handle_nf_discover_search_result(SearchResult);

    nf_instance = ogs_sbi_nf_instance_find_by_discovery_param(
                    target_nf_type, requester_nf_type, discovery_option);
    if (!nf_instance) {
        ogs_sbi_xact_remove(xact);

        switch(sbi_object->type) {
        case OGS_SBI_OBJ_UE_TYPE:
            ogs_warn("[%s] (NF discover) No [%s]", amf_ue->suci,
                        ogs_sbi_service_type_to_name(service_type));

            if (amf_nnrf_try_old_amf_discovery_fallback(
                    amf_ue, requester_nf_type, discovery_option))
                break;

            if (amf_ue->nas.message_type ==
                    OGS_NAS_5GS_DEREGISTRATION_REQUEST_FROM_UE) {
                ogs_error("[%s:%s] Deregistration request failed",
                        amf_ue->supi, amf_ue->suci);
                break;
            }

            r = nas_5gs_send_gmm_reject_from_sbi(amf_ue,
                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            break;
        case OGS_SBI_OBJ_SESS_TYPE:
            ogs_warn("[%d:%d] (NF discover) No [%s]", sess->psi, sess->pti,
                        ogs_sbi_service_type_to_name(service_type));

            amf_nnrf_send_session_failure_to_ran(amf_ue, sess, ran_ue_id);
            break;
        default:
            ogs_fatal("(NF discover) Not implemented [%s:%d]",
                ogs_sbi_service_type_to_name(service_type), sbi_object->type);
            ogs_assert_if_reached();
        }

        return;
    }

    OGS_SBI_SETUP_NF_INSTANCE(
            sbi_object->service_type_array[service_type], nf_instance);

    ogs_expect(true == amf_sbi_send_request(nf_instance, xact));
}

void amf_nnrf_handle_failed_amf_discovery(
        ogs_sbi_xact_t *sbi_xact)
{
    int r;

    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;
    ogs_sbi_service_type_e service_type = OGS_SBI_SERVICE_TYPE_NULL;
    ogs_sbi_object_t *sbi_object = NULL;
    ogs_pool_id_t sbi_object_id = OGS_INVALID_POOL_ID;
    ogs_pool_id_t ran_ue_id = OGS_INVALID_POOL_ID;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;

    ogs_assert(sbi_xact);
    sbi_object = sbi_xact->sbi_object;
    ogs_assert(sbi_object);
    service_type = sbi_xact->service_type;
    ogs_assert(service_type);
    requester_nf_type = sbi_xact->requester_nf_type;
    ogs_assert(requester_nf_type);

    sbi_object_id = sbi_xact->sbi_object_id;
    ogs_assert(sbi_object_id >= OGS_MIN_POOL_ID &&
            sbi_object_id <= OGS_MAX_POOL_ID);

    discovery_option = sbi_xact->discovery_option;

    if (sbi_xact->user_data) {
        amf_sbi_xact_ctx_t *ctx = sbi_xact->user_data;

        if (ctx->ran_ue_id != OGS_INVALID_POOL_ID)
            ran_ue_id = ctx->ran_ue_id;
    }

    ogs_assert(sbi_object->type > OGS_SBI_OBJ_BASE &&
                sbi_object->type < OGS_SBI_OBJ_TOP);

    ogs_sbi_xact_remove(sbi_xact);

    switch(sbi_object->type) {
    case OGS_SBI_OBJ_UE_TYPE:
        amf_ue = amf_ue_find_by_id(sbi_object_id);
        if (!amf_ue) {
            ogs_error("UE(amf_ue) Context has already been removed");
            break;
        }

        ogs_error("[%s:%s] Cannot receive SBI message "
                "[type:%d,value:%d]", amf_ue->supi, amf_ue->suci,
                amf_ue->nas.message_type,
                amf_ue->nas.registration.value);

        if (amf_nnrf_try_old_amf_discovery_fallback(
                amf_ue, requester_nf_type, discovery_option))
            break;

        if (amf_ue->nas.message_type ==
                OGS_NAS_5GS_DEREGISTRATION_REQUEST_FROM_UE) {
            ogs_error("[%s:%s] Deregistration request failed",
                    amf_ue->supi, amf_ue->suci);
            break;
        }

        r = nas_5gs_send_gmm_reject_from_sbi(amf_ue,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        break;

    case OGS_SBI_OBJ_SESS_TYPE:
        sess = amf_sess_find_by_id(sbi_object_id);
        if (!sess) {
            ogs_error("Session has already been removed");
            break;
        }

        amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
        if (!amf_ue) {
            ogs_error("UE(amf_ue) Context has already been removed");
            break;
        }

        ogs_error("[%s:%s:%d:%d] Cannot receive SBI message",
                amf_ue->supi, amf_ue->suci, sess->psi, sess->pti);

        amf_nnrf_send_session_failure_to_ran(amf_ue, sess, ran_ue_id);
        break;

    default:
        ogs_fatal("Not implemented [%s:%d]",
            ogs_sbi_service_type_to_name(service_type), sbi_object->type);
        ogs_assert_if_reached();
    }
}
