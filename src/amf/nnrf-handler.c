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
    } else {
        ogs_fatal("(NF discover) Not implemented [%s:%d]",
            ogs_sbi_service_type_to_name(service_type), sbi_object->type);
        ogs_assert_if_reached();
    }

    ogs_nnrf_disc_handle_nf_discover_search_result(SearchResult);

    nf_instance = ogs_sbi_nf_instance_find_by_discovery_param(
                    target_nf_type, requester_nf_type, discovery_option);
    if (!nf_instance) {
        switch(sbi_object->type) {
        case OGS_SBI_OBJ_UE_TYPE:
            ogs_error("[%s] (NF discover) No [%s]", amf_ue->suci,
                        ogs_sbi_service_type_to_name(service_type));
            r = nas_5gs_send_gmm_reject_from_sbi(amf_ue,
                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            break;
        case OGS_SBI_OBJ_SESS_TYPE:
            ogs_error("[%d:%d] (NF discover) No [%s]", sess->psi, sess->pti,
                        ogs_sbi_service_type_to_name(service_type));
            if (sess->payload_container_type) {
                r = nas_5gs_send_back_gsm_message(
                        ran_ue_find_by_id(sess->ran_ue_id), sess,
                        OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED,
                        AMF_NAS_BACKOFF_TIME);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            } else {
                r = ngap_send_error_indication2(
                        ran_ue_find_by_id(sess->ran_ue_id),
                        NGAP_Cause_PR_transport,
                        NGAP_CauseTransport_transport_resource_unavailable);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
            }
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
