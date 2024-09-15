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

#include "nnssf-handler.h"
#include "nas-path.h"
#include "sbi-path.h"

int amf_nnssf_nsselection_handle_get(
        amf_sess_t *sess, int state, ogs_sbi_message_t *recvmsg)
{
    bool rc;
    int r, i;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    ogs_sbi_client_t *client = NULL, *scp_client = NULL;
    char *fqdn = NULL;
    uint16_t fqdn_port = 0;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;

    OpenAPI_authorized_network_slice_info_t *AuthorizedNetworkSliceInfo = NULL;
    OpenAPI_nsi_information_t *NsiInformation = NULL;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(state);
    ogs_assert(recvmsg);
    ogs_assert(!SESSION_CONTEXT_IN_SMF(sess));

    if (!sess) {
        ogs_error("Session has already been removed");
        return OGS_ERROR;
    }

    amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
    if (!amf_ue) {
        ogs_error("UE(amf_ue) Context has already been removed");
        return OGS_ERROR;
    }

    ran_ue = ran_ue_find_by_id(sess->ran_ue_id);
    if (!ran_ue) {
        ogs_error("[%s] RAN-NG Context has already been removed", amf_ue->supi);
        return OGS_ERROR;
    }

    if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_OK) {
        ogs_error("[%s] HTTP response error [%d]",
                amf_ue->supi, recvmsg->res_status);
        r = nas_5gs_send_gmm_status(amf_ue, recvmsg->res_status);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    AuthorizedNetworkSliceInfo = recvmsg->AuthorizedNetworkSliceInfo;
    if (!AuthorizedNetworkSliceInfo) {
        ogs_error("No AuthorizedNetworkSliceInfo");
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    NsiInformation = AuthorizedNetworkSliceInfo->nsi_information;
    if (!NsiInformation) {
        ogs_error("No NsiInformation");
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    if (!NsiInformation->nrf_id) {
        ogs_error("No nrfId");
        r = nas_5gs_send_gmm_reject_from_sbi(
                amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return OGS_ERROR;
    }

    if (state == AMF_SMF_SELECTION_IN_HPLMN_IN_HOME_ROUTED) {
        if (!sess->nssf.nrf_uri) {
            ogs_error("No VPLMN nrf_uri");
            r = nas_5gs_send_gmm_reject_from_sbi(
                    amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return OGS_ERROR;
        }

        if (sess->nssf.hnrf_uri)
            ogs_free(sess->nssf.hnrf_uri);
        sess->nssf.hnrf_uri = ogs_strdup(NsiInformation->nrf_id);
        ogs_assert(sess->nssf.hnrf_uri);
    } else {
        if (sess->nssf.nrf_uri)
            ogs_free(sess->nssf.nrf_uri);
        sess->nssf.nrf_uri = ogs_strdup(NsiInformation->nrf_id);
        ogs_assert(sess->nssf.nrf_uri);
    }

    discovery_option = ogs_sbi_discovery_option_new();
    ogs_assert(discovery_option);

    ogs_sbi_discovery_option_add_snssais(discovery_option, &sess->s_nssai);
    ogs_sbi_discovery_option_set_dnn(discovery_option, sess->dnn);
    ogs_sbi_discovery_option_set_tai(discovery_option, &amf_ue->nr_tai);

    if (state == AMF_SMF_SELECTION_IN_HPLMN_IN_HOME_ROUTED) {
        /*
         * In order for Home Routed to find NRFs that are on HPLMN,
         * we need to include Home PLMN information in the Discovery Option.
         */
        ogs_sbi_discovery_option_add_target_plmn_list(
                discovery_option, &amf_ue->home_plmn_id);

        ogs_assert(ogs_local_conf()->num_of_serving_plmn_id);
        for (i = 0; i < ogs_local_conf()->num_of_serving_plmn_id; i++) {
            ogs_sbi_discovery_option_add_requester_plmn_list(
                    discovery_option,
                    &ogs_local_conf()->serving_plmn_id[i]);
        }

        ogs_assert(sess->nssf.hnrf_uri);
        ogs_sbi_discovery_option_set_hnrf_uri(
                discovery_option, sess->nssf.hnrf_uri);
    }

    /*
     * SCP can only be used with Non-Roaming or LBO.
     *
     * In Home Routed, obtaining NRF from NSSF and selecting SMF
     * cannot be done via SCP.
     */
    if (state == AMF_SMF_SELECTION_IN_VPLMN_IN_NON_ROAMING_OR_LBO)
        scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);

    if (scp_client) {
        /* SCP can only be used with Non-Roaming or LBO. */
        amf_nsmf_pdusession_sm_context_param_t param;

        memset(&param, 0, sizeof(param));
        param.nrf_uri = sess->nssf.nrf_uri;

        r = amf_sess_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, discovery_option,
                amf_nsmf_pdusession_build_create_sm_context,
                ran_ue, sess, AMF_CREATE_SM_CONTEXT_NO_STATE, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else {
        /*
         * In Home Routed, obtaining NRF from NSSF and selecting SMF
         * cannot be done via SCP.
         */
        rc = ogs_sbi_getaddr_from_uri(
                &scheme, &fqdn, &fqdn_port, &addr, &addr6,
                NsiInformation->nrf_id);
        if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
            ogs_error("[%s:%d] Invalid URI [%s]",
                    amf_ue->supi, sess->psi, NsiInformation->nrf_id);
            r = nas_5gs_send_gmm_reject_from_sbi(
                    amf_ue, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            ogs_sbi_discovery_option_free(discovery_option);

            return OGS_ERROR;
        }

        client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            ogs_debug("%s: ogs_sbi_client_add()", OGS_FUNC);
            client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
            if (!client) {
                ogs_error("%s: ogs_sbi_client_add() failed", OGS_FUNC);

                ogs_sbi_discovery_option_free(discovery_option);
                ogs_free(fqdn);
                ogs_freeaddrinfo(addr);
                ogs_freeaddrinfo(addr6);

                return OGS_ERROR;
            }
        }
        OGS_SBI_SETUP_CLIENT(&sess->nssf.nrf, client);

        ogs_free(fqdn);
        ogs_freeaddrinfo(addr);
        ogs_freeaddrinfo(addr6);

        r = amf_sess_sbi_discover_by_nsi(
                ran_ue, sess,
                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, discovery_option, state);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }

    return OGS_OK;
}
