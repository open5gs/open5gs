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

#include "nausf-handler.h"
#include "nas-path.h"

int amf_nausf_auth_handle_authenticate(
        amf_ue_t *amf_ue, ogs_sbi_message_t *message)
{
    int r;
    OpenAPI_ue_authentication_ctx_t *UeAuthenticationCtx = NULL;
    OpenAPI_ue_authentication_ctx_5g_auth_data_t *AV5G_AKA = NULL;
    OpenAPI_links_value_schema_t *LinksValueSchemeValue = NULL;
    OpenAPI_map_t *LinksValueScheme = NULL;
    OpenAPI_lnode_t *node = NULL;

    bool rc;
    ogs_sbi_client_t *client = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    char *fqdn = NULL;
    uint16_t fqdn_port = 0;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

    ogs_assert(amf_ue);
    ogs_assert(message);

    if (!message->http.location) {
        ogs_error("[%s] No http.location", amf_ue->suci);
        return OGS_ERROR;
    }

    UeAuthenticationCtx = message->UeAuthenticationCtx;
    if (!UeAuthenticationCtx) {
        ogs_error("[%s] No UeAuthenticationCtx", amf_ue->suci);
        return OGS_ERROR;
    }

    if (UeAuthenticationCtx->auth_type != OpenAPI_auth_type_5G_AKA &&
        UeAuthenticationCtx->auth_type != OpenAPI_auth_type_EAP_AKA_PRIME) {
        ogs_error("[%s] Not supported Auth Method [%d]",
            amf_ue->suci, UeAuthenticationCtx->auth_type);
        return OGS_ERROR;
    }

    amf_ue->auth_type = UeAuthenticationCtx->auth_type;

    if (!UeAuthenticationCtx->_links) {
        ogs_error("[%s] No _links", amf_ue->suci);
        return OGS_ERROR;
    }

    OpenAPI_list_for_each(UeAuthenticationCtx->_links, node) {
        LinksValueScheme = node->data;
        if (LinksValueScheme) {
            if (strcmp(LinksValueScheme->key,
                        OGS_SBI_RESOURCE_NAME_5G_AKA) == 0 ||
                strcmp(LinksValueScheme->key,
                        OGS_SBI_RESOURCE_NAME_EAP_SESSION) == 0) {
                LinksValueSchemeValue = LinksValueScheme->value;
                break;
            }
        }
    }

    if (!LinksValueSchemeValue || !LinksValueSchemeValue->href) {
        ogs_error("[%s] No confirmation _links.href", amf_ue->suci);
        return OGS_ERROR;
    }

    rc = ogs_sbi_getaddr_from_uri(
            &scheme, &fqdn, &fqdn_port, &addr, &addr6, message->http.location);
    if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
        ogs_error("[%s] Invalid URI [%s]",
                amf_ue->suci, message->http.location);
        return OGS_ERROR;
    }

    client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
    if (!client) {
        ogs_debug("[%s] ogs_sbi_client_add()", amf_ue->suci);
        client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            ogs_error("[%s] ogs_sbi_client_add() failed", amf_ue->suci);

            ogs_free(fqdn);
            ogs_freeaddrinfo(addr);
            ogs_freeaddrinfo(addr6);

            return OGS_ERROR;
        }
    }

    OGS_SBI_SETUP_CLIENT(&amf_ue->confirmation_for_5g_aka, client);

    ogs_free(fqdn);
    ogs_freeaddrinfo(addr);
    ogs_freeaddrinfo(addr6);

    STORE_5G_AKA_CONFIRMATION(amf_ue, LinksValueSchemeValue->href);

    if (UeAuthenticationCtx->auth_type == OpenAPI_auth_type_EAP_AKA_PRIME) {
        int decoded;

        AV5G_AKA = UeAuthenticationCtx->_5g_auth_data;
        if (!AV5G_AKA || !AV5G_AKA->eap_payload) {
            ogs_error("[%s] No 5gAuthData.eapPayload", amf_ue->suci);
            return OGS_ERROR;
        }

        decoded = ogs_base64_decode_to_buffer(
                amf_ue->eap, sizeof(amf_ue->eap),
                AV5G_AKA->eap_payload);
        if (decoded <= 0) {
            ogs_error("[%s] EAP payload decode failed", amf_ue->suci);
            return OGS_ERROR;
        }
        amf_ue->eap_len = decoded;
    } else {
        OpenAPI_av5g_aka_t *Av5gAka = NULL;

        AV5G_AKA = UeAuthenticationCtx->_5g_auth_data;
        if (!AV5G_AKA || !AV5G_AKA->av5g_aka) {
            ogs_error("[%s] No Av5gAka", amf_ue->suci);
            return OGS_ERROR;
        }
        Av5gAka = AV5G_AKA->av5g_aka;
        if (!Av5gAka->rand || !Av5gAka->hxres_star || !Av5gAka->autn) {
            ogs_error("[%s] Incomplete Av5gAka", amf_ue->suci);
            return OGS_ERROR;
        }

        if (ogs_hex_from_string_checked(Av5gAka->rand,
                    amf_ue->rand, sizeof(amf_ue->rand)) != OGS_OK) {
            ogs_error("[%s] Invalid Av5gAka.rand", amf_ue->suci);
            return OGS_ERROR;
        }
        if (ogs_hex_from_string_checked(Av5gAka->hxres_star,
                    amf_ue->hxres_star, sizeof(amf_ue->hxres_star)) != OGS_OK) {
            ogs_error("[%s] Invalid Av5gAka.hxresStar", amf_ue->suci);
            return OGS_ERROR;
        }
        if (ogs_hex_from_string_checked(Av5gAka->autn,
                    amf_ue->autn, sizeof(amf_ue->autn)) != OGS_OK) {
            ogs_error("[%s] Invalid Av5gAka.autn", amf_ue->suci);
            return OGS_ERROR;
        }
    }

    /* Clear Security Context */
    CLEAR_SECURITY_CONTEXT(amf_ue);

    if (amf_ue->nas.amf.ksi < (OGS_NAS_KSI_NO_KEY_IS_AVAILABLE - 1))
        amf_ue->nas.amf.ksi++;
    else
        amf_ue->nas.amf.ksi = 0;

    amf_ue->nas.ue.ksi = amf_ue->nas.amf.ksi;

    r = nas_5gs_send_authentication_request(amf_ue);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return OGS_OK;
}

int amf_nausf_auth_handle_authenticate_confirmation(
        amf_ue_t *amf_ue, ogs_sbi_message_t *message)
{
    int r;
    uint8_t kseaf[OGS_SHA256_DIGEST_SIZE];

    OpenAPI_confirmation_data_response_t *ConfirmationDataResponse;

    ogs_assert(amf_ue);
    ogs_assert(message);

    if (amf_ue->auth_type == OpenAPI_auth_type_EAP_AKA_PRIME) {
        OpenAPI_eap_session_t *EapSession = message->EapSession;
        int decoded;

        if (!EapSession) {
            ogs_error("[%s] No EapSession", amf_ue->suci);
            return OGS_ERROR;
        }

        amf_ue->auth_result = EapSession->auth_result;

        if (amf_ue->auth_result ==
                OpenAPI_auth_result_AUTHENTICATION_ONGOING) {
            /*
             * Multi-round EAP-AKA' (e.g. after a synchronization failure):
             * relay the next EAP-Request to the UE and keep authenticating.
             */
            if (!EapSession->eap_payload) {
                ogs_error("[%s] No eapPayload in ongoing EapSession",
                        amf_ue->suci);
                return OGS_ERROR;
            }
            decoded = ogs_base64_decode_to_buffer(
                    amf_ue->eap, sizeof(amf_ue->eap), EapSession->eap_payload);
            if (decoded <= 0) {
                ogs_error("[%s] EAP payload decode failed", amf_ue->suci);
                return OGS_ERROR;
            }
            amf_ue->eap_len = decoded;

            r = nas_5gs_send_authentication_request(amf_ue);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            return OGS_OK;
        }

        if (amf_ue->auth_result !=
                OpenAPI_auth_result_AUTHENTICATION_SUCCESS) {
            ogs_error("[%s] EAP-AKA' authentication failed", amf_ue->suci);
            return OGS_ERROR;
        }

        if (!EapSession->supi || !EapSession->k_seaf ||
                !EapSession->eap_payload) {
            ogs_error("[%s] Incomplete EapSession", amf_ue->suci);
            return OGS_ERROR;
        }

        amf_ue_set_supi(amf_ue, EapSession->supi);
        if (ogs_hex_from_string_checked(EapSession->k_seaf,
                    kseaf, sizeof(kseaf)) != OGS_OK) {
            ogs_error("[%s] Invalid EapSession.kSeaf", amf_ue->suci);
            return OGS_ERROR;
        }
        ogs_kdf_kamf(amf_ue->supi, amf_ue->abba, amf_ue->abba_len,
                kseaf, amf_ue->kamf);

        /* Relay the terminating EAP-Success to the UE over NAS */
        decoded = ogs_base64_decode_to_buffer(
                amf_ue->eap, sizeof(amf_ue->eap), EapSession->eap_payload);
        amf_ue->eap_len = (decoded > 0) ? decoded : 0;

        return OGS_OK;
    }

    ConfirmationDataResponse = message->ConfirmationDataResponse;
    if (!ConfirmationDataResponse) {
        ogs_error("[%s] No ConfirmationDataResponse", amf_ue->suci);
        return OGS_ERROR;
    }

    if (!ConfirmationDataResponse->supi) {
        ogs_error("[%s] No supi", amf_ue->suci);
        return OGS_ERROR;
    }

    if (!ConfirmationDataResponse->kseaf) {
        ogs_error("[%s] No Kseaf", amf_ue->suci);
        return OGS_ERROR;
    }

    if (ConfirmationDataResponse->auth_result ==
            OpenAPI_auth_result_AUTHENTICATION_SUCCESS) {
        if (ogs_hex_from_string_checked(ConfirmationDataResponse->kseaf,
                    kseaf, sizeof(kseaf)) != OGS_OK) {
            ogs_error("[%s] Invalid Kseaf", amf_ue->suci);
            return OGS_ERROR;
        }
    }

    amf_ue->auth_result = ConfirmationDataResponse->auth_result;
    if (amf_ue->auth_result == OpenAPI_auth_result_AUTHENTICATION_SUCCESS) {

        amf_ue_set_supi(amf_ue, ConfirmationDataResponse->supi);

        ogs_kdf_kamf(amf_ue->supi, amf_ue->abba, amf_ue->abba_len,
                kseaf, amf_ue->kamf);

        return OGS_OK;

    } else {

        ogs_error("[%s] Authentication failed", amf_ue->suci);
        return OGS_ERROR;
    }
}
