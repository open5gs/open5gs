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

#include "nudm-handler.h"

#include "sbi-path.h"
#include "nas-path.h"

int amf_nudm_sdm_handle_provisioned(
        amf_ue_t *amf_ue, int state, ogs_sbi_message_t *recvmsg)
{
    int i, r;

    ogs_assert(amf_ue);
    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[1])
    CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)

        if (recvmsg->AccessAndMobilitySubscriptionData) {
            OpenAPI_list_t *gpsiList =
                recvmsg->AccessAndMobilitySubscriptionData->gpsis;
            OpenAPI_ambr_rm_t *SubscribedUeAmbr =
                recvmsg->AccessAndMobilitySubscriptionData->subscribed_ue_ambr;
            OpenAPI_nssai_t *NSSAI =
                recvmsg->AccessAndMobilitySubscriptionData->nssai;
            OpenAPI_list_t *RatRestrictions =
                recvmsg->AccessAndMobilitySubscriptionData->rat_restrictions;

            OpenAPI_lnode_t *node = NULL;

            /* Clear MSISDN */
            for (i = 0; i < amf_ue->num_of_msisdn; i++) {
                ogs_assert(amf_ue->msisdn[i]);
                ogs_free(amf_ue->msisdn[i]);
            }
            amf_ue->num_of_msisdn = 0;

            if (gpsiList) {
                OpenAPI_list_for_each(gpsiList, node) {
                    if (node->data) {
                        char *gpsi = NULL;

                        gpsi = ogs_id_get_type(node->data);
                        if (gpsi) {
                            if (strncmp(gpsi, OGS_ID_GPSI_TYPE_MSISDN,
                                    strlen(OGS_ID_GPSI_TYPE_MSISDN)) == 0) {
                                amf_ue->msisdn[amf_ue->num_of_msisdn] =
                                    ogs_id_get_value(node->data);
                                ogs_assert(amf_ue->
                                        msisdn[amf_ue->num_of_msisdn]);

                                amf_ue->num_of_msisdn++;
                            }
                            ogs_free(gpsi);
                        }
                    }
                }
            }

            /* Clear Subscribed-UE-AMBR */
            amf_ue->ue_ambr.uplink = 0;
            amf_ue->ue_ambr.downlink = 0;

            if (SubscribedUeAmbr) {
                amf_ue->ue_ambr.uplink =
                    ogs_sbi_bitrate_from_string(SubscribedUeAmbr->uplink);
                amf_ue->ue_ambr.downlink =
                    ogs_sbi_bitrate_from_string(SubscribedUeAmbr->downlink);
            }

            if (NSSAI) {
                OpenAPI_list_t *DefaultSingleNssaiList = NULL;
                OpenAPI_list_t *SingleNssaiList = NULL;

                /* Clear SubscribedInfo */
                amf_clear_subscribed_info(amf_ue);

                DefaultSingleNssaiList = NSSAI->default_single_nssais;
                if (DefaultSingleNssaiList) {
                    OpenAPI_list_for_each(DefaultSingleNssaiList, node) {
                        OpenAPI_snssai_t *Snssai = node->data;

                        ogs_slice_data_t *slice =
                            &amf_ue->slice[amf_ue->num_of_slice];
                        if (Snssai) {
                            slice->s_nssai.sst = Snssai->sst;
                            slice->s_nssai.sd =
                                ogs_s_nssai_sd_from_string(Snssai->sd);
                        }

                        /* DEFAULT S-NSSAI */
                        slice->default_indicator = true;

                        amf_ue->num_of_slice++;
                    }

                    SingleNssaiList = NSSAI->single_nssais;
                    if (SingleNssaiList) {
                        OpenAPI_list_for_each(SingleNssaiList, node) {
                            OpenAPI_snssai_t *Snssai = node->data;

                            ogs_slice_data_t *slice =
                                &amf_ue->slice[amf_ue->num_of_slice];
                            if (Snssai) {
                                slice->s_nssai.sst = Snssai->sst;
                                slice->s_nssai.sd =
                                    ogs_s_nssai_sd_from_string(Snssai->sd);
                            }

                            /* Non default S-NSSAI */
                            slice->default_indicator = false;

                            amf_ue->num_of_slice++;
                        }
                    }
                }
            }

            OpenAPI_list_clear(amf_ue->rat_restrictions);
            if (RatRestrictions) {
                OpenAPI_list_for_each(RatRestrictions, node) {
                    OpenAPI_list_add(amf_ue->rat_restrictions, node->data);
                }
            }
        }

        if (amf_update_allowed_nssai(amf_ue) == false) {
            ogs_error("No Allowed-NSSAI");
            r = nas_5gs_send_gmm_reject(
                    amf_ue->ran_ue, amf_ue,
                    OGS_5GMM_CAUSE_NO_NETWORK_SLICES_AVAILABLE);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return OGS_ERROR;
        }

        if (amf_ue_is_rat_restricted(amf_ue)) {
            ogs_error("Registration rejected due to RAT restrictions");
            r = nas_5gs_send_gmm_reject(
                    amf_ue->ran_ue, amf_ue,
                    OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return OGS_ERROR;
        }

        r = amf_ue_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NUDM_SDM, NULL,
                amf_nudm_sdm_build_get,
                amf_ue, state, (char *)OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        break;

    CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
        if (recvmsg->SmfSelectionSubscriptionData) {
            OpenAPI_list_t *SubscribedSnssaiInfoList = NULL;
            OpenAPI_map_t *SubscribedSnssaiInfoMap = NULL;
            OpenAPI_snssai_info_t *SubscribedSnssaiInfo = NULL;

            OpenAPI_list_t *DnnInfoList = NULL;
            OpenAPI_dnn_info_t *DnnInfo = NULL;

            OpenAPI_lnode_t *node = NULL, *node2 = NULL;

            SubscribedSnssaiInfoList = recvmsg->
                SmfSelectionSubscriptionData->subscribed_snssai_infos;
            if (SubscribedSnssaiInfoList) {

                OpenAPI_list_for_each(SubscribedSnssaiInfoList, node) {
                    SubscribedSnssaiInfoMap = node->data;
                    if (SubscribedSnssaiInfoMap &&
                            SubscribedSnssaiInfoMap->key) {
                        ogs_slice_data_t *slice = NULL;
                        ogs_s_nssai_t s_nssai;

                        bool rc = ogs_sbi_s_nssai_from_string(
                                &s_nssai, SubscribedSnssaiInfoMap->key);
                        if (rc == false) {
                            ogs_error("Invalid S-NSSAI format [%s]",
                                SubscribedSnssaiInfoMap->key);
                            continue;
                        }

                        ogs_assert(amf_ue->num_of_slice);
                        slice = ogs_slice_find_by_s_nssai(
                                    amf_ue->slice, amf_ue->num_of_slice,
                                    &s_nssai);
                        if (!slice) {
                            ogs_error("Cannt find S-NSSAI[SST:%d SD:0x%x]",
                                    s_nssai.sst, s_nssai.sd.v);
                            continue;
                        }

                        SubscribedSnssaiInfo = SubscribedSnssaiInfoMap->value;
                        if (SubscribedSnssaiInfo) {
                            DnnInfoList = SubscribedSnssaiInfo->dnn_infos;
                            if (DnnInfoList) {
                                OpenAPI_list_for_each(DnnInfoList, node2) {
                                    DnnInfo = node2->data;
                                    if (DnnInfo) {
                                        ogs_session_t *session =
                                            &slice->session
                                                [slice->num_of_session];
                                        session->name =
                                            ogs_strdup(DnnInfo->dnn);
                                        ogs_assert(session->name);
                                        if (DnnInfo->is_default_dnn_indicator ==
                                                true) {
                                            session->default_dnn_indicator =
                                                DnnInfo->default_dnn_indicator;
                                        }
                                        slice->num_of_session++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        r = amf_ue_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NUDM_SDM, NULL,
                amf_nudm_sdm_build_get,
                amf_ue, state,
                (char *)OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        break;

    CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
        if (UDM_SDM_SUBSCRIBED(amf_ue)) {
            /* we already have a SDM subscription to UDM; continue without
             * subscribing again */
            r = amf_ue_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL, NULL,
                    amf_npcf_am_policy_control_build_create,
                    amf_ue, state, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        else {
            r = amf_ue_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NUDM_SDM, NULL,
                    amf_nudm_sdm_build_subscription,
                    amf_ue, state, (char *)OGS_SBI_RESOURCE_NAME_AM_DATA);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        break;

    CASE(OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS)

        int rv;
        ogs_sbi_message_t message;
        ogs_sbi_header_t header;

        bool rc;
        ogs_sbi_client_t *client = NULL;
        OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
        char *fqdn = NULL;
        uint16_t fqdn_port = 0;
        ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

        if (!recvmsg->http.location) {
            ogs_error("[%s] No http.location", amf_ue->supi);
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

        if (!message.h.resource.component[2]) {
            ogs_error("[%s] No Subscription ID [%s]",
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

        UDM_SDM_STORE(amf_ue, header.uri, message.h.resource.component[2]);

        ogs_sbi_header_free(&header);

        r = amf_ue_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NPCF_AM_POLICY_CONTROL, NULL,
                amf_npcf_am_policy_control_build_create, amf_ue, state, NULL);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        break;

    DEFAULT
    END

    return OGS_OK;
}
