/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

#include "nsmf-build.h"

ogs_sbi_request_t *smf_nsmf_pdusession_build_create_sm_context(
        smf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;

    smf_ue_t *smf_ue = NULL;

    OpenAPI_sm_context_create_data_t SmContextCreateData;
    OpenAPI_snssai_t sNssai;
    OpenAPI_snssai_t hplmnSnssai;
    OpenAPI_ref_to_binary_data_t n1SmMsg;
    OpenAPI_user_location_t ueLocation;
#if 0 /* Needs to be checked against AMF's nsmf-builc.c */
    ogs_sbi_nf_instance_t *pcf_nf_instance = NULL;
#endif

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SM_CONTEXTS;

    memset(&SmContextCreateData, 0, sizeof(SmContextCreateData));
    memset(&sNssai, 0, sizeof(sNssai));
    memset(&hplmnSnssai, 0, sizeof(hplmnSnssai));
    memset(&header, 0, sizeof(header));
    memset(&ueLocation, 0, sizeof(ueLocation));

    SmContextCreateData.serving_nf_id =
        NF_INSTANCE_ID(ogs_sbi_self()->nf_instance);
    if (!SmContextCreateData.serving_nf_id) {
        ogs_error("No serving_nf_id");
        goto end;
    }

    SmContextCreateData.serving_network =
        ogs_sbi_build_plmn_id_nid(&sess->nr_tai.plmn_id);
    if (!SmContextCreateData.serving_network) {
        ogs_error("No serving_network");
        goto end;
    }

    SmContextCreateData.supi = smf_ue->supi;
#if 0 /* Needs to be checked against AMF's nsmf-builc.c */
    SmContextCreateData.pei = smf_ue->pei;
#endif
    if (smf_ue->msisdn_len) {
        SmContextCreateData.gpsi = ogs_msprintf("%s-%s",
                OGS_ID_GPSI_TYPE_MSISDN, smf_ue->msisdn_bcd);
    }
    SmContextCreateData.is_pdu_session_id = true;
    SmContextCreateData.pdu_session_id = sess->psi;
    if (!SmContextCreateData.pdu_session_id) {
        ogs_error("No pdu_session_id");
        goto end;
    }

    /*
     * TS29.502
     * 6.1 Nsmf_PDUSession Service API
     * Table 6.1.6.2.2-1: Definition of type SmContextCreateData
     *
     * NAME: dnn
     * Data type: Dnn
     * P: C
     * Cardinality: 0..1
     *
     * This IE shall be present, except during an EPS to 5GS Idle mode mobility
     * or handover using the N26 interface.
     *
     * When present, it shall contain the requested DNN; the DNN shall
     * be the full DNN (i.e. with both the Network Identifier and
     * Operator Identifier) for a HR PDU session, and it should be
     * the full DNN in LBO and non-roaming scenarios. If the Operator Identifier
     * is absent, the serving core network operator shall be assumed.
     */
    if (ogs_sbi_plmn_id_in_vplmn(&sess->home_plmn_id) == true) {
        SmContextCreateData.dnn = sess->full_dnn;
    } else {
        SmContextCreateData.dnn = sess->session.name;

    }

    sNssai.sst = sess->s_nssai.sst;
    sNssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
    SmContextCreateData.s_nssai = &sNssai;

    if (sess->mapped_hplmn.sst) {
        hplmnSnssai.sst = sess->mapped_hplmn.sst;
        hplmnSnssai.sd = ogs_s_nssai_sd_to_string(sess->mapped_hplmn.sd);
        SmContextCreateData.hplmn_snssai = &hplmnSnssai;
    }

#if 0 /* Needs to be checked against AMF's nsmf-builc.c */
    SmContextCreateData.guami = ogs_sbi_build_guami(amf_ue->guami);
    if (!SmContextCreateData.guami) {
        ogs_error("No guami");
        goto end;
    }
#endif

    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_CALLBACK;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = smf_ue->supi;
    header.resource.component[1] =
        (char *)OGS_SBI_RESOURCE_NAME_SM_CONTEXT_STATUS;
    header.resource.component[2] = ogs_msprintf("%d", sess->psi);
    if (!header.resource.component[2]) {
        ogs_error("No header.resource.component[2]");
        goto end;
    }

    server = ogs_sbi_server_first();
    if (!server) {
        ogs_error("No server");
        goto end;
    }
    SmContextCreateData.sm_context_status_uri =
        ogs_sbi_server_uri(server, &header);

    n1SmMsg.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
    SmContextCreateData.n1_sm_msg = &n1SmMsg;

    SmContextCreateData.an_type = sess->an_type;
    SmContextCreateData.rat_type = sess->sbi_rat_type;

    ueLocation.nr_location = ogs_sbi_build_nr_location(
            &sess->nr_tai, &sess->nr_cgi);
    if (!ueLocation.nr_location) {
        ogs_error("No ueLocation.nr_location");
        goto end;
    }
    ueLocation.nr_location->ue_location_timestamp =
        ogs_sbi_gmtime_string(sess->ue_location_timestamp);
    if (!ueLocation.nr_location->ue_location_timestamp) {
        ogs_error("No ue_location_timestamp");
        goto end;
    }

    SmContextCreateData.ue_location = &ueLocation;
    SmContextCreateData.ue_time_zone = ogs_sbi_timezone_string(ogs_timezone());
    if (!SmContextCreateData.ue_time_zone) {
        ogs_error("No ue_time_zone");
        goto end;
    }

    /*
     * We're experiencing an issue after changing SearchResult.validityTime
     * from 3600 seconds to 30 seconds. (#3210)
     *
     * When AMF finds a PCF through Discovery, it can be deleted
     * after 30 seconds by ValidityTime.
     *
     * We have changed our implementation to not send the PCF-ID in this case.
     *
     * What we need to do is proactively add a part that will re-discover
     * the PCF when a situation arises where we really need the PCF-ID.
     */
#if 0 /* Needs to be checked against AMF's nsmf-builc.c */
    pcf_nf_instance = OGS_SBI_GET_NF_INSTANCE(
            amf_ue->sbi.service_type_array[
            OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL]);
    if (pcf_nf_instance)
        SmContextCreateData.pcf_id = pcf_nf_instance->id;
    else
        ogs_error("No pcf_nf_instance");
#endif

    message.SmContextCreateData = &SmContextCreateData;

#if 0 /* Needs to be checked against AMF's nsmf-builc.c */
    message.part[message.num_of_part].pkbuf = sess->payload_container;
    if (message.part[message.num_of_part].pkbuf) {
        message.part[message.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        message.part[message.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
        message.num_of_part++;
    }
#endif

    message.http.accept = (char *)(OGS_SBI_CONTENT_JSON_TYPE ","
        OGS_SBI_CONTENT_NGAP_TYPE "," OGS_SBI_CONTENT_PROBLEM_TYPE);

    message.http.custom.callback =
        (char *)OGS_SBI_CALLBACK_NSMF_PDUSESSION_STATUS_NOTIFY;

#if 0 /* Needs to be checked against AMF's nsmf-builc.c */
    if (param && param->nrf_uri) {
        message.http.custom.nrf_uri =
            ogs_msprintf("%s: \"%s\"",
                    OGS_SBI_SERVICE_NAME_NNRF_DISC, param->nrf_uri);
    }
#endif

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (SmContextCreateData.serving_network)
        ogs_sbi_free_plmn_id_nid(SmContextCreateData.serving_network);
    if (SmContextCreateData.dnn)
        ogs_free(SmContextCreateData.dnn);
    if (SmContextCreateData.sm_context_status_uri)
        ogs_free(SmContextCreateData.sm_context_status_uri);
    if (header.resource.component[2])
        ogs_free(header.resource.component[2]);
    if (sNssai.sd)
        ogs_free(sNssai.sd);
    if (hplmnSnssai.sd)
        ogs_free(hplmnSnssai.sd);
    if (SmContextCreateData.guami)
        ogs_sbi_free_guami(SmContextCreateData.guami);
    if (SmContextCreateData.gpsi)
        ogs_free(SmContextCreateData.gpsi);
    if (ueLocation.nr_location) {
        if (ueLocation.nr_location->ue_location_timestamp)
            ogs_free(ueLocation.nr_location->ue_location_timestamp);
        ogs_sbi_free_nr_location(ueLocation.nr_location);
    }
    if (SmContextCreateData.ue_time_zone)
        ogs_free(SmContextCreateData.ue_time_zone);

    if (SmContextCreateData.h_smf_uri)
        ogs_free(SmContextCreateData.h_smf_uri);

    if (message.http.custom.nrf_uri)
        ogs_free(message.http.custom.nrf_uri);

    return request;
}
