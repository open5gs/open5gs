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
    int r, rv;

    uint64_t supported_features;

    OpenAPI_policy_association_t *PolicyAssociation = NULL;
    OpenAPI_lnode_t *node = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_header_t header;

    bool rc;
    ogs_sbi_client_t *client = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    char *fqdn = NULL;
    uint16_t fqdn_port = 0;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

    if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_CREATED) {
        ogs_error("[%s] HTTP response error [%d]",
                amf_ue->supi, recvmsg->res_status);
        r = nas_5gs_send_gmm_reject_from_sbi(amf_ue, recvmsg->res_status);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    if (!recvmsg->http.location) {
        ogs_error("[%s] No http.location", amf_ue->supi);
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    PolicyAssociation = recvmsg->PolicyAssociation;
    if (!PolicyAssociation) {
        ogs_error("No PolicyAssociation");
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    if (!PolicyAssociation->supp_feat) {
        ogs_error("No suppFeat");
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    memset(&header, 0, sizeof(header));
    header.uri = recvmsg->http.location;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        ogs_error("[%s] Cannot parse http.location [%s]",
                amf_ue->supi, recvmsg->http.location);
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    if (!message.h.resource.component[1]) {
        ogs_error("[%s] No Assocation ID [%s]",
                amf_ue->supi, recvmsg->http.location);

        ogs_sbi_header_free(&header);
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    rc = ogs_sbi_getaddr_from_uri(
            &scheme, &fqdn, &fqdn_port, &addr, &addr6, header.uri);
    if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
        ogs_error("[%s] Invalid URI [%s]", amf_ue->supi, header.uri);

        ogs_sbi_header_free(&header);
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        return OGS_ERROR;
    }

    client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
    if (!client) {
        ogs_debug("[%s] ogs_sbi_client_add()", amf_ue->supi);
        client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            ogs_error("[%s] ogs_sbi_client_add() failed", amf_ue->supi);

            ogs_sbi_header_free(&header);
            r = nas_5gs_send_gmm_reject_from_sbi(
                    amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            ogs_free(fqdn);
            ogs_freeaddrinfo(addr);
            ogs_freeaddrinfo(addr6);

            return OGS_ERROR;
        }
    }

    OGS_SBI_SETUP_CLIENT(&amf_ue->policy_association, client);

    ogs_free(fqdn);
    ogs_freeaddrinfo(addr);
    ogs_freeaddrinfo(addr6);

    PCF_AM_POLICY_STORE(amf_ue, header.uri, message.h.resource.component[1]);

    /* SBI Features */
    supported_features = ogs_uint64_from_string(PolicyAssociation->supp_feat);
    amf_ue->am_policy_control_features &= supported_features;

    OpenAPI_list_for_each(PolicyAssociation->triggers, node) {
        if (node->data) {
            OpenAPI_request_trigger_e trigger = (intptr_t)node->data;
            OpenAPI_ambr_t *UeAmbr = NULL;

            switch (trigger) {
            case OpenAPI_request_trigger_UE_AMBR_CH:
                UeAmbr = PolicyAssociation->ue_ambr;
                if (UeAmbr) {
                    amf_ue->ue_ambr.uplink =
                        ogs_sbi_bitrate_from_string(UeAmbr->uplink);
                    amf_ue->ue_ambr.downlink =
                        ogs_sbi_bitrate_from_string(UeAmbr->downlink);
                }
                break;
            default:
                ogs_error("Not implemented [%d]", trigger);
                break;
            }
        }
    }

    ogs_sbi_header_free(&header);

    return OGS_OK;
}
