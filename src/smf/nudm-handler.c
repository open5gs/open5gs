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

bool smf_nudm_sdm_handle_get(smf_sess_t *sess, ogs_sbi_stream_t *stream,
        ogs_sbi_message_t *recvmsg)
{
    char *strerror = NULL;
    smf_ue_t *smf_ue = NULL;
    ogs_pkbuf_t *n1smbuf = NULL;

    OpenAPI_sm_context_created_data_t SmContextCreatedData;

    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_session_management_subscription_data_t
        *SessionManagementSubscriptionData = NULL;
    OpenAPI_list_t *dnnConfigurationList = NULL;
    OpenAPI_map_t *dnnConfigurationMap = NULL;
    OpenAPI_dnn_configuration_t *dnnConfiguration = NULL;
    OpenAPI_pdu_session_types_t *pduSessionTypeList = NULL;
    OpenAPI_ssc_modes_t *sscModeList = NULL;
    OpenAPI_subscribed_default_qos_t *_5gQoSProfile = NULL;
    OpenAPI_ambr_t *sessionAmbr = NULL;
    OpenAPI_list_t *staticIpAddress = NULL;
    OpenAPI_ip_address_t *ipAddress = NULL;
    OpenAPI_lnode_t *node = NULL, *node2 = NULL;

    ogs_assert(sess);
    ogs_assert(stream);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);


    if ((!recvmsg->SessionManagementSubscriptionDataList) ||
        (recvmsg->SessionManagementSubscriptionDataList->count == 0))
    {
        strerror = ogs_msprintf("[%s:%d] No SessionManagementSubscriptionDataList",
                smf_ue->supi, sess->psi);
        goto cleanup;
    }

    OpenAPI_list_for_each(recvmsg->SessionManagementSubscriptionDataList, node)
    {
        SessionManagementSubscriptionData = node->data;

        /* currently supported to parse only first element of the array */
        break;
    }
    
    if (!SessionManagementSubscriptionData) {
        strerror = ogs_msprintf("[%s:%d] No SessionManagementSubscriptionData",
                smf_ue->supi, sess->psi);
        goto cleanup;
    }

    dnnConfigurationList =
        SessionManagementSubscriptionData->dnn_configurations;
    if (!dnnConfigurationList) {
        strerror = ogs_msprintf("[%s:%d] No dnnConfigurations",
                smf_ue->supi, sess->psi);
        ogs_assert(strerror);

        n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
            OGS_5GSM_CAUSE_MISSING_OR_UNKNOWN_DNN);
        ogs_assert(n1smbuf);

        ogs_warn("%s", strerror);
        smf_sbi_send_sm_context_create_error(stream,
                OGS_SBI_HTTP_STATUS_NOT_FOUND, strerror, NULL, n1smbuf);
        ogs_free(strerror);

        return false;
    }

    OpenAPI_list_for_each(dnnConfigurationList, node) {
        dnnConfigurationMap = node->data;
        if (dnnConfigurationMap) {
            if (!dnnConfigurationMap->key) {
                ogs_error("No dnnConfigurationMap->key");
                continue;
            }

            dnnConfiguration = dnnConfigurationMap->value;
            if (!dnnConfiguration) {
                ogs_error("No dnnConfiguration");
                continue;
            }

            pduSessionTypeList = dnnConfiguration->pdu_session_types;
            if (!pduSessionTypeList) {
                ogs_error("No pduSessionTypes");
                continue;
            }

            sscModeList = dnnConfiguration->ssc_modes;
            if (!sscModeList) {
                ogs_error("No sscModes");
                continue;
            }

            if (sess->session.name &&
                ogs_strcasecmp(sess->session.name,
                    dnnConfigurationMap->key) != 0)
                continue;

            if (sess->ue_session_type) {
                OpenAPI_list_for_each(
                    pduSessionTypeList->allowed_session_types, node2) {
                    if (node2->data) {
                        uint8_t allowed_session_type = (uintptr_t)node2->data;
                        if (sess->ue_session_type == allowed_session_type) {
                            sess->session.session_type =
                                sess->ue_session_type;
                            break;
                        }
                    }
                }
            }

            if (!sess->session.session_type)
                sess->session.session_type =
                    pduSessionTypeList->default_session_type;

            if (sess->ue_ssc_mode) {
                OpenAPI_list_for_each(sscModeList->allowed_ssc_modes, node2) {
                    if (node2->data) {
                        uint8_t allowed_ssc_mode = (uintptr_t)node2->data;
                        if (sess->ue_ssc_mode == allowed_ssc_mode) {
                            sess->session.ssc_mode = sess->ue_ssc_mode;
                            break;
                        }
                    }
                }
            } else {
                sess->session.ssc_mode = sscModeList->default_ssc_mode;
            }

            if (!sess->session.ssc_mode) {
                ogs_error("SSCMode is not allowed");
                continue;
            }

            sessionAmbr = dnnConfiguration->session_ambr;
            if (!sessionAmbr) {
                ogs_error("No Session-AMBR");
                continue;
            }

            sess->session.ambr.uplink =
                ogs_sbi_bitrate_from_string(sessionAmbr->uplink);
            sess->session.ambr.downlink =
                ogs_sbi_bitrate_from_string(sessionAmbr->downlink);

            _5gQoSProfile = dnnConfiguration->_5g_qos_profile;
            if (_5gQoSProfile) {
                sess->session.qos.index = _5gQoSProfile->_5qi;
                sess->session.qos.arp.priority_level =
                    _5gQoSProfile->priority_level;
                if (_5gQoSProfile->arp) {
                    sess->session.qos.arp.priority_level =
                            _5gQoSProfile->arp->priority_level;
                    if (_5gQoSProfile->arp->preempt_cap ==
                            OpenAPI_preemption_capability_MAY_PREEMPT)
                        sess->session.qos.arp.pre_emption_capability =
                            OGS_5GC_PRE_EMPTION_ENABLED;
                    else if (_5gQoSProfile->arp->preempt_cap ==
                            OpenAPI_preemption_capability_NOT_PREEMPT)
                        sess->session.qos.arp.pre_emption_capability =
                            OGS_5GC_PRE_EMPTION_DISABLED;
                    ogs_assert(sess->session.qos.arp.pre_emption_capability);

                    if (_5gQoSProfile->arp->preempt_vuln ==
                            OpenAPI_preemption_vulnerability_PREEMPTABLE)
                        sess->session.qos.arp.pre_emption_vulnerability =
                            OGS_5GC_PRE_EMPTION_ENABLED;
                    else if (_5gQoSProfile->arp->preempt_vuln ==
                            OpenAPI_preemption_vulnerability_NOT_PREEMPTABLE)
                        sess->session.qos.arp.pre_emption_vulnerability =
                            OGS_5GC_PRE_EMPTION_DISABLED;
                    ogs_assert(sess->session.qos.arp.pre_emption_vulnerability);
                }
            }

            staticIpAddress = dnnConfiguration->static_ip_address;
            if (staticIpAddress) {
                OpenAPI_list_for_each(staticIpAddress, node2) {
                    if (node2->data) {
                        ipAddress = node2->data;
                        if (ipAddress) {
                            int rv;
                            bool ipv4 = false, ipv6 = false;
                            ogs_ipsubnet_t ipsub4, ipsub6;
                            if (ipAddress->ipv4_addr) {
                                rv = ogs_ipsubnet(&ipsub4,
                                        ipAddress->ipv4_addr, NULL);
                                if (rv == OGS_OK) ipv4 = true;
                            }
                            if (ipAddress->ipv6_addr) {
                                rv = ogs_ipsubnet(&ipsub6,
                                        ipAddress->ipv6_addr, NULL);
                                if (rv == OGS_OK) ipv6 = true;
                            }

                            if (ipv4 && ipv6) {
                                sess->session.ue_ip.addr = ipsub4.sub[0];
                                memcpy(sess->session.ue_ip.addr6,
                                        ipsub6.sub, OGS_IPV6_LEN);
                            } else if (ipv4) {
                                sess->session.ue_ip.addr = ipsub4.sub[0];
                            } else if (ipv6) {
                                memcpy(sess->session.ue_ip.addr6,
                                        ipsub6.sub, OGS_IPV6_LEN);
                            }
                        }
                    }
                }
            }

            /* Succeeded to get PDU Session */
            if (!sess->session.name)
                sess->session.name = ogs_strdup(dnnConfigurationMap->key);
            ogs_assert(sess->session.name);

            break;
        }
    }

    if (!sess->session.name) {
        strerror = ogs_msprintf("[%s:%d] No dnnConfiguration",
                smf_ue->supi, sess->psi);
        ogs_assert(strerror);
        return false;
    }

    /* Set UE IP Address to the Default DL PDR */
    ogs_assert(OGS_PFCP_CAUSE_REQUEST_ACCEPTED == smf_sess_set_ue_ip(sess));

    /*********************************************************************
     * Send HTTP_STATUS_CREATED(/nsmf-pdusession/v1/sm-context) to the AMF
     *********************************************************************/

    memset(&SmContextCreatedData, 0, sizeof(SmContextCreatedData));

    memset(&sendmsg, 0, sizeof(sendmsg));

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SM_CONTEXTS;
    header.resource.component[1] = sess->sm_context_ref;

    sendmsg.http.location = ogs_sbi_server_uri(server, &header);
    ogs_assert(sendmsg.http.location);

    sendmsg.SmContextCreatedData = &SmContextCreatedData;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_CREATED);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_free(sendmsg.http.location);

    ogs_assert(true ==
        smf_sbi_discover_and_send(OpenAPI_nf_type_PCF, sess, stream,
            0, NULL, smf_npcf_smpolicycontrol_build_create));

    return true;

cleanup:
    ogs_assert(strerror);

    ogs_error("%s", strerror);
    ogs_assert(true ==
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            recvmsg, strerror, NULL));
    ogs_free(strerror);

    return false;
}
