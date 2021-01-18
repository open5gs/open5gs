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
        amf_ue_t *amf_ue, ogs_sbi_message_t *recvmsg)
{
    int i;

    ogs_assert(amf_ue);
    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[1])
    CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
        if (recvmsg->AccessAndMobilitySubscriptionData) {
            OpenAPI_list_t *gpsiList =
                recvmsg->AccessAndMobilitySubscriptionData->gpsis;
            OpenAPI_ambr_rm_t *SubscribedUeAmbr =
                recvmsg->AccessAndMobilitySubscriptionData->subscribed_ue_ambr;
            OpenAPI_list_t *SubscribedDnnList =
                recvmsg->AccessAndMobilitySubscriptionData->subscribed_dnn_list;
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
                        ogs_assert(gpsi);

                        if (strncmp(gpsi, OGS_ID_GPSI_TYPE_MSISDN,
                                    strlen(OGS_ID_GPSI_TYPE_MSISDN)) != 0) {
                            ogs_error("Unknown GPSI Type [%s]", gpsi);

                        } else {
                            amf_ue->msisdn[amf_ue->num_of_msisdn] =
                                ogs_id_get_value(node->data);
                            ogs_assert(amf_ue->msisdn[amf_ue->num_of_msisdn]);

                            amf_ue->num_of_msisdn++;
                        }

                        ogs_free(gpsi);
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

            /* Clear Subscribed-DNN */
            for (i = 0; i < amf_ue->num_of_subscribed_dnn; i++) {
                ogs_assert(amf_ue->subscribed_dnn[i]);
                ogs_free(amf_ue->subscribed_dnn[i]);
            }
            amf_ue->num_of_subscribed_dnn = 0;

            if (SubscribedDnnList) {
                OpenAPI_list_for_each(SubscribedDnnList, node) {
                    if (node->data) {
                        amf_ue->subscribed_dnn[amf_ue->num_of_subscribed_dnn] =
                            ogs_strdup(node->data);
                        amf_ue->num_of_subscribed_dnn++;
                    }
                }
            }
        }

        amf_ue_sbi_discover_and_send(OpenAPI_nf_type_UDM, amf_ue,
            (char *)OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA,
            amf_nudm_sdm_build_get);
        break;

    CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
        amf_ue_sbi_discover_and_send(OpenAPI_nf_type_UDM, amf_ue,
            (char *)OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA,
            amf_nudm_sdm_build_get);
        break;

    CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
        amf_ue_sbi_discover_and_send(OpenAPI_nf_type_PCF, amf_ue,
            NULL, amf_npcf_am_policy_control_build_create);
        break;

    DEFAULT
    END

    return OGS_OK;
}
