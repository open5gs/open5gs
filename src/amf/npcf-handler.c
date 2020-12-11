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

#include "npcf-handler.h"

#include "sbi-path.h"
#include "nas-path.h"

int amf_npcf_am_policy_control_handle_create(
        amf_ue_t *amf_ue, ogs_sbi_message_t *recvmsg)
{
    int rv;

    OpenAPI_policy_association_t *PolicyAssociation = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_header_t header;

    if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_CREATED) {
        ogs_error("[%s] HTTP response error [%d]",
                amf_ue->supi, recvmsg->res_status);
        nas_5gs_send_gmm_reject_from_sbi(amf_ue, recvmsg->res_status);
        return OGS_ERROR;
    }

    if (!recvmsg->http.location) {
        ogs_error("[%s] No http.location", amf_ue->supi);
        nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        return OGS_ERROR;
    }

    PolicyAssociation = recvmsg->PolicyAssociation;
    if (!PolicyAssociation) {
        ogs_error("No PolicyAssociation");
        nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        return OGS_ERROR;
    }

    if (!PolicyAssociation->supp_feat) {
        ogs_error("No suppFeat");
        nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        return OGS_ERROR;
    }

    memset(&header, 0, sizeof(header));
    header.uri = recvmsg->http.location;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        ogs_error("[%s] Cannot parse http.location [%s]",
                amf_ue->supi, recvmsg->http.location);
        nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        return OGS_ERROR;
    }

    if (!message.h.resource.component[1]) {
        ogs_error("[%s] No Assocation ID [%s]",
                amf_ue->supi, recvmsg->http.location);

        ogs_sbi_header_free(&header);
        nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        return OGS_ERROR;
    }

    if (amf_ue->policy_association_id)
        ogs_free(amf_ue->policy_association_id);
    amf_ue->policy_association_id = ogs_strdup(message.h.resource.component[1]);

    ogs_sbi_header_free(&header);

    return OGS_OK;
}
