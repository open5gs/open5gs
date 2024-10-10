/*
 * Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>
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

ogs_sbi_request_t *smf_nsmf_pdusession_build_create_pdu_session(
        smf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;

    smf_ue_t *smf_ue = NULL;

    OpenAPI_pdu_session_create_data_t PduSessionCreateData;
    OpenAPI_snssai_t sNssai;
    OpenAPI_snssai_t hplmnSnssai;
    OpenAPI_tunnel_info_t vcnTunnelInfo;
    OpenAPI_ref_to_binary_data_t n1SmInfoFromUe;
    OpenAPI_user_location_t ueLocation;

#if 0 /* Needs to be checked against AMF's nsmf-build.c */
    ogs_sbi_nf_instance_t *pcf_nf_instance = NULL;
#endif

    int rv;
    ogs_nas_5gs_message_t nas_message;
    ogs_nas_5gs_pdu_session_establishment_request_t
        *pdu_session_establishment_request = NULL;
    ogs_nas_integrity_protection_maximum_data_rate_t
        *integrity_protection_maximum_data_rate = NULL;
    ogs_pkbuf_t *n1SmBufFromUe = NULL;

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    ogs_assert(sess->h_smf_uri);
    message.h.uri = sess->h_smf_uri;

    memset(&header, 0, sizeof(header));
    memset(&PduSessionCreateData, 0, sizeof(PduSessionCreateData));
    memset(&sNssai, 0, sizeof(sNssai));
    memset(&hplmnSnssai, 0, sizeof(hplmnSnssai));
    memset(&vcnTunnelInfo, 0, sizeof(vcnTunnelInfo));
    memset(&ueLocation, 0, sizeof(ueLocation));

    PduSessionCreateData.supi = smf_ue->supi;
    if (smf_ue->imeisv_len) {
        PduSessionCreateData.pei = ogs_msprintf("%s-%s",
                OGS_ID_SUPI_TYPE_IMEISV, smf_ue->imeisv_bcd);
    }
    if (smf_ue->msisdn_len) {
        PduSessionCreateData.gpsi = ogs_msprintf("%s-%s",
                OGS_ID_GPSI_TYPE_MSISDN, smf_ue->msisdn_bcd);
    }
    PduSessionCreateData.is_pdu_session_id = true;
    PduSessionCreateData.pdu_session_id = sess->psi;
    if (!PduSessionCreateData.pdu_session_id) {
        ogs_error("No pdu_session_id");
        goto end;
    }

    /*
     * TS29.502
     * 6.1 Nsmf_PDUSession Service API
     * Table 6.1.6.2.2-1: Definition of type PduSessionCreateData
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
    PduSessionCreateData.dnn = ogs_strdup(sess->full_dnn);
    ogs_assert(PduSessionCreateData.dnn);

    sNssai.sst = sess->s_nssai.sst;
    sNssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
    PduSessionCreateData.s_nssai = &sNssai;

    if (sess->mapped_hplmn.sst) {
        hplmnSnssai.sst = sess->mapped_hplmn.sst;
        hplmnSnssai.sd = ogs_s_nssai_sd_to_string(sess->mapped_hplmn.sd);
        PduSessionCreateData.hplmn_snssai = &hplmnSnssai;
    }

    PduSessionCreateData.vsmf_id = NF_INSTANCE_ID(ogs_sbi_self()->nf_instance);
    if (!PduSessionCreateData.vsmf_id) {
        ogs_error("No vsmfId");
        goto end;
    }

    PduSessionCreateData.serving_network =
        ogs_sbi_build_plmn_id_nid(&sess->nr_tai.plmn_id);
    if (!PduSessionCreateData.serving_network) {
        ogs_error("No serving_network");
        goto end;
    }

    if (sess->request_type >= OpenAPI_request_type_INITIAL_REQUEST &&
            sess->request_type <=
            OpenAPI_request_type_EXISTING_EMERGENCY_PDU_SESSION)
        PduSessionCreateData.request_type = sess->request_type;

    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_CALLBACK;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = smf_ue->supi;
    header.resource.component[1] =
        (char *)OGS_SBI_RESOURCE_NAME_PDU_SESSION_STATUS;
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
    PduSessionCreateData.vsmf_pdu_session_uri =
        ogs_sbi_server_uri(server, &header);

    if (sess->local_dl_addr)
        vcnTunnelInfo.ipv4_addr = ogs_ipstrdup(sess->local_dl_addr);
    if (sess->local_dl_addr6)
        vcnTunnelInfo.ipv6_addr = ogs_ipstrdup(sess->local_dl_addr6);
    vcnTunnelInfo.gtp_teid = ogs_uint32_to_0string(sess->local_dl_teid);
    PduSessionCreateData.vcn_tunnel_info = &vcnTunnelInfo;

    PduSessionCreateData.an_type = sess->an_type;
    PduSessionCreateData.rat_type = sess->sbi_rat_type;

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

    PduSessionCreateData.ue_location = &ueLocation;
    PduSessionCreateData.ue_time_zone = ogs_sbi_timezone_string(ogs_timezone());
    if (!PduSessionCreateData.ue_time_zone) {
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
#if 0 /* Needs to be checked against AMF's nsmf-build.c */
    pcf_nf_instance = OGS_SBI_GET_NF_INSTANCE(
            amf_ue->sbi.service_type_array[
            OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL]);
    if (pcf_nf_instance)
        PduSessionCreateData.pcf_id = pcf_nf_instance->id;
    else
        ogs_error("No pcf_nf_instance");
#endif

    ogs_assert(sess->amf_nf_id);
    PduSessionCreateData.amf_nf_id = sess->amf_nf_id;

    PduSessionCreateData.guami = ogs_sbi_build_guami(&sess->guami);
    if (!PduSessionCreateData.guami) {
        ogs_error("No guami");
        goto end;
    }

    ogs_assert(sess->n1smbuf);
    rv = ogs_nas_5gsm_decode(&nas_message, sess->n1smbuf);

    if (rv == OGS_OK) {
        n1SmBufFromUe = gsmue_encode_n1_sm_info(&nas_message);
        message.part[message.num_of_part].pkbuf = n1SmBufFromUe;
        if (message.part[message.num_of_part].pkbuf) {
            message.part[message.num_of_part].content_id =
                (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
            message.part[message.num_of_part].content_type =
                (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
            message.num_of_part++;

            n1SmInfoFromUe.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
            PduSessionCreateData.n1_sm_info_from_ue = &n1SmInfoFromUe;

            pdu_session_establishment_request =
                &nas_message.gsm.pdu_session_establishment_request;
            integrity_protection_maximum_data_rate =
                &pdu_session_establishment_request->
                integrity_protection_maximum_data_rate;

            switch (integrity_protection_maximum_data_rate->ul) {
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_64KBPS:
                PduSessionCreateData.max_integrity_protected_data_rate_ul =
                    OpenAPI_max_integrity_protected_data_rate__64_KBPS;
                break;
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_NULL:
                PduSessionCreateData.max_integrity_protected_data_rate_ul =
                    OpenAPI_max_integrity_protected_data_rate_NULL;
                break;
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_FULL:
                PduSessionCreateData.max_integrity_protected_data_rate_ul =
                    OpenAPI_max_integrity_protected_data_rate_MAX_UE_RATE;
                break;
            default:
                ogs_error("Unknown Integrity Protection "
                        "Maximum Data Rate [UL:%d]",
                        integrity_protection_maximum_data_rate->ul);
                break;
            }

            switch (integrity_protection_maximum_data_rate->dl) {
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_64KBPS:
                PduSessionCreateData.max_integrity_protected_data_rate_dl =
                    OpenAPI_max_integrity_protected_data_rate__64_KBPS;
                break;
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_NULL:
                PduSessionCreateData.max_integrity_protected_data_rate_dl =
                    OpenAPI_max_integrity_protected_data_rate_NULL;
                break;
            case OGS_NAS_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_FULL:
                PduSessionCreateData.max_integrity_protected_data_rate_dl =
                    OpenAPI_max_integrity_protected_data_rate_MAX_UE_RATE;
                break;
            defadlt:
                ogs_error("Unknown Integrity Protection "
                        "Maximum Data Rate [DL:%d]",
                        integrity_protection_maximum_data_rate->dl);
                break;
            }
        } else {
            ogs_error("gsm_encode_n1_sm_info() failed [%d]", rv);
            ogs_log_hexdump(OGS_LOG_ERROR,
                    sess->n1smbuf->data, sess->n1smbuf->len);
        }
    } else {
        ogs_error("ogs_nas_5gsm_decode() failed [%d]", rv);
        ogs_log_hexdump(OGS_LOG_ERROR, sess->n1smbuf->data, sess->n1smbuf->len);
    }

    message.PduSessionCreateData = &PduSessionCreateData;

    message.http.accept = (char *)(OGS_SBI_CONTENT_JSON_TYPE ","
        OGS_SBI_CONTENT_NGAP_TYPE "," OGS_SBI_CONTENT_PROBLEM_TYPE);

    message.http.custom.callback =
        (char *)OGS_SBI_CALLBACK_NSMF_PDUSESSION_STATUS_NOTIFY;

#if 0 /* Needs to be checked against AMF's nsmf-build.c */
    if (param && param->nrf_uri) {
        message.http.custom.nrf_uri =
            ogs_msprintf("%s: \"%s\"",
                    OGS_SBI_SERVICE_NAME_NNRF_DISC, param->nrf_uri);
    }
#endif

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    if (header.resource.component[2])
        ogs_free(header.resource.component[2]);

    if (PduSessionCreateData.serving_network)
        ogs_sbi_free_plmn_id_nid(PduSessionCreateData.serving_network);
    if (PduSessionCreateData.dnn)
        ogs_free(PduSessionCreateData.dnn);
    if (PduSessionCreateData.vsmf_pdu_session_uri)
        ogs_free(PduSessionCreateData.vsmf_pdu_session_uri);
    if (vcnTunnelInfo.ipv4_addr)
        ogs_free(vcnTunnelInfo.ipv4_addr);
    if (vcnTunnelInfo.ipv6_addr)
        ogs_free(vcnTunnelInfo.ipv6_addr);
    if (vcnTunnelInfo.gtp_teid)
        ogs_free(vcnTunnelInfo.gtp_teid);
    if (sNssai.sd)
        ogs_free(sNssai.sd);
    if (hplmnSnssai.sd)
        ogs_free(hplmnSnssai.sd);
    if (PduSessionCreateData.guami)
        ogs_sbi_free_guami(PduSessionCreateData.guami);
    if (PduSessionCreateData.pei)
        ogs_free(PduSessionCreateData.pei);
    if (PduSessionCreateData.gpsi)
        ogs_free(PduSessionCreateData.gpsi);
    if (ueLocation.nr_location) {
        if (ueLocation.nr_location->ue_location_timestamp)
            ogs_free(ueLocation.nr_location->ue_location_timestamp);
        ogs_sbi_free_nr_location(ueLocation.nr_location);
    }
    if (PduSessionCreateData.ue_time_zone)
        ogs_free(PduSessionCreateData.ue_time_zone);

    if (n1SmBufFromUe)
        ogs_pkbuf_free(n1SmBufFromUe);

    if (message.http.custom.nrf_uri)
        ogs_free(message.http.custom.nrf_uri);

    return request;
}
