/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "npcf-build.h"

ogs_sbi_request_t *amf_npcf_am_policy_control_build_create(
        amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_header_t header;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;

    OpenAPI_policy_association_request_t PolicyAssociationRequest;

    int i;
    OpenAPI_lnode_t *node = NULL;

    OpenAPI_list_t *AllowedSnssais = NULL;
    OpenAPI_user_location_t ueLocation;
    OpenAPI_ambr_t UeAmbr;

    ogs_assert(amf_ue);
    ogs_assert(amf_ue->supi);
    ogs_assert(ran_ue_cycle(amf_ue->ran_ue));

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name =
        (char *)OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_POLICIES;

    memset(&PolicyAssociationRequest, 0, sizeof(PolicyAssociationRequest));
    memset(&ueLocation, 0, sizeof(ueLocation));
    memset(&UeAmbr, 0, sizeof(UeAmbr));

    server = ogs_sbi_server_first();
    if (!server) {
        ogs_error("No server");
        goto end;
    }

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_CALLBACK;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = amf_ue->supi;
    header.resource.component[1] =
            (char *)OGS_SBI_RESOURCE_NAME_AM_POLICY_NOTIFY;
    PolicyAssociationRequest.notification_uri =
                        ogs_sbi_server_uri(server, &header);
    if (!PolicyAssociationRequest.notification_uri) {
        ogs_error("No notifiation_uri");
        goto end;
    }

    PolicyAssociationRequest.supi = amf_ue->supi;

    if (amf_ue->num_of_msisdn) {
        if (amf_ue->msisdn[0]) {
            PolicyAssociationRequest.gpsi = ogs_msprintf("%s-%s",
                        OGS_ID_GPSI_TYPE_MSISDN, amf_ue->msisdn[0]);
            if (!PolicyAssociationRequest.gpsi) {
                ogs_error("No gpsi");
                goto end;
            }
        }
    }

    PolicyAssociationRequest.access_type = amf_ue->nas.access_type; 
    PolicyAssociationRequest.pei = amf_ue->pei;

    ueLocation.nr_location = ogs_sbi_build_nr_location(
            &amf_ue->nr_tai, &amf_ue->nr_cgi);
    if (!ueLocation.nr_location) {
        ogs_error("No ueLocation.nr_location");
        goto end;
    }
    ueLocation.nr_location->ue_location_timestamp =
        ogs_sbi_gmtime_string(amf_ue->ue_location_timestamp);
    if (!ueLocation.nr_location->ue_location_timestamp) {
        ogs_error("No ueLocation.nr_location->ue_location_timestamp");
        goto end;
    }
    PolicyAssociationRequest.user_loc = &ueLocation;

    PolicyAssociationRequest.time_zone =
        ogs_sbi_timezone_string(ogs_timezone());
    if (!PolicyAssociationRequest.time_zone) {
        ogs_error("No time_zone");
        goto end;
    }

    PolicyAssociationRequest.serving_plmn =
        ogs_sbi_build_plmn_id_nid(&amf_ue->nr_tai.plmn_id);
    if (!PolicyAssociationRequest.serving_plmn) {
        ogs_error("No serving_plmn");
        goto end;
    }

    PolicyAssociationRequest.rat_type = amf_ue_rat_type(amf_ue);

    if (OGS_SBI_FEATURES_IS_SET(amf_ue->am_policy_control_features,
                OGS_SBI_NPCF_AM_POLICY_CONTROL_UE_AMBR_AUTHORIZATION)) {
        if (amf_ue->ue_ambr.uplink) {
            UeAmbr.uplink = ogs_sbi_bitrate_to_string(
                amf_ue->ue_ambr.uplink, OGS_SBI_BITRATE_KBPS);
        }
        if (amf_ue->ue_ambr.downlink) {
            UeAmbr.downlink = ogs_sbi_bitrate_to_string(
                amf_ue->ue_ambr.downlink, OGS_SBI_BITRATE_KBPS);
        }
        if (UeAmbr.downlink || UeAmbr.uplink) {
            PolicyAssociationRequest.ue_ambr = &UeAmbr;
        }
    }

    AllowedSnssais = OpenAPI_list_create();
    ogs_assert(AllowedSnssais);

    for (i = 0; i < amf_ue->allowed_nssai.num_of_s_nssai; i++) {
        struct OpenAPI_snssai_s *Snssai = ogs_calloc(1, sizeof(*Snssai));
        if (!Snssai) {
            ogs_error("No Snssai");
            goto end;
        }

        Snssai->sst = amf_ue->allowed_nssai.s_nssai[i].sst;
        Snssai->sd = ogs_s_nssai_sd_to_string(
            amf_ue->allowed_nssai.s_nssai[i].sd);

        OpenAPI_list_add(AllowedSnssais, Snssai);
    }

    if (AllowedSnssais->count)
        PolicyAssociationRequest.allowed_snssais = AllowedSnssais;
    else
        OpenAPI_list_free(AllowedSnssais);

    PolicyAssociationRequest.guami = ogs_sbi_build_guami(amf_ue->guami);
    if (!PolicyAssociationRequest.guami) {
        ogs_error("No guami");
        goto end;
    }

    PolicyAssociationRequest.service_name =
        (char *)OGS_SBI_SERVICE_NAME_NAMF_CALLBACK;

    PolicyAssociationRequest.supp_feat =
        ogs_uint64_to_string(amf_ue->am_policy_control_features);
    if (!PolicyAssociationRequest.supp_feat) {
        ogs_error("No supp_feat");
        goto end;
    }

    message.PolicyAssociationRequest = &PolicyAssociationRequest;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (PolicyAssociationRequest.notification_uri)
        ogs_free(PolicyAssociationRequest.notification_uri);
    if (PolicyAssociationRequest.supp_feat)
        ogs_free(PolicyAssociationRequest.supp_feat);

    if (PolicyAssociationRequest.gpsi)
        ogs_free(PolicyAssociationRequest.gpsi);

    if (ueLocation.nr_location) {
        if (ueLocation.nr_location->ue_location_timestamp)
            ogs_free(ueLocation.nr_location->ue_location_timestamp);
        ogs_sbi_free_nr_location(ueLocation.nr_location);
    }
    if (PolicyAssociationRequest.time_zone)
        ogs_free(PolicyAssociationRequest.time_zone);

    if (PolicyAssociationRequest.serving_plmn)
        ogs_sbi_free_plmn_id_nid(PolicyAssociationRequest.serving_plmn);

    if (UeAmbr.downlink) ogs_free(UeAmbr.downlink);
    if (UeAmbr.uplink) ogs_free(UeAmbr.uplink);

    OpenAPI_list_for_each(PolicyAssociationRequest.allowed_snssais, node) {
        struct OpenAPI_snssai_s *Snssai = node->data;
        if (Snssai) {
            if (Snssai->sd)
                ogs_free(Snssai->sd);
            ogs_free(Snssai);
        }
    }
    OpenAPI_list_free(PolicyAssociationRequest.allowed_snssais);

    if (PolicyAssociationRequest.guami)
        ogs_sbi_free_guami(PolicyAssociationRequest.guami);

    return request;
}

ogs_sbi_request_t *amf_npcf_am_policy_control_build_delete(
        amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(amf_ue);
    ogs_assert(amf_ue->supi);
    ogs_assert(amf_ue->policy_association.resource_uri);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.uri = amf_ue->policy_association.resource_uri;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}
