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

#include "test-common.h"

void testemm_handle_identity_request(test_ue_t *test_ue,
        ogs_nas_eps_identity_request_t *identity_request)
{
    ogs_assert(test_ue);
    ogs_assert(identity_request);

    switch (identity_request->identity_type.type) {
    case OGS_NAS_IDENTITY_TYPE_2_IMSI:
        break;
    default:
        ogs_error("Not implemented [%d]",
                identity_request->identity_type.type);
        break;
    }
}

void testemm_handle_authentication_request(test_ue_t *test_ue,
        ogs_nas_eps_authentication_request_t *authentication_request)
{
    ogs_nas_authentication_parameter_rand_t *authentication_parameter_rand =
        NULL;
    ogs_nas_authentication_parameter_autn_t *authentication_parameter_autn =
        NULL;
    ogs_nas_key_set_identifier_t *ksi = NULL;

    ogs_assert(test_ue);
    ogs_assert(authentication_request);

    authentication_parameter_rand = &authentication_request->
        authentication_parameter_rand;
    authentication_parameter_autn = &authentication_request->
        authentication_parameter_autn;
    ksi = &authentication_request->nas_key_set_identifierasme;

    test_ue->nas.ksi = ksi->value;

    memcpy(test_ue->rand, authentication_parameter_rand->rand, OGS_RAND_LEN);
    memcpy(test_ue->autn, authentication_parameter_autn->autn, OGS_AUTN_LEN);
}

void testemm_handle_security_mode_command(test_ue_t *test_ue,
        ogs_nas_eps_security_mode_command_t *security_mode_command)
{
    ogs_nas_security_algorithms_t *selected_nas_security_algorithms = NULL;
    ogs_nas_key_set_identifier_t *ksi = NULL;

    ogs_assert(test_ue);
    ogs_assert(security_mode_command);

    selected_nas_security_algorithms =
        &security_mode_command->selected_nas_security_algorithms;
    ksi = &security_mode_command->nas_key_set_identifier;

    test_ue->selected_enc_algorithm =
        selected_nas_security_algorithms->type_of_ciphering_algorithm;
    test_ue->selected_int_algorithm =
        selected_nas_security_algorithms->
            type_of_integrity_protection_algorithm;

    ogs_kdf_nas_eps(OGS_KDF_NAS_INT_ALG, test_ue->selected_int_algorithm,
        test_ue->kasme, test_ue->knas_int);
    ogs_kdf_nas_eps(OGS_KDF_NAS_ENC_ALG, test_ue->selected_enc_algorithm,
        test_ue->kasme, test_ue->knas_enc);

    test_ue->nas.ksi = ksi->value;

    test_ue->security_context_available = 1;
}

void testemm_handle_attach_accept(test_ue_t *test_ue,
        ogs_nas_eps_attach_accept_t *attach_accept)
{
    ogs_nas_eps_mobile_identity_t *guti = NULL;

    ogs_assert(test_ue);
    ogs_assert(attach_accept);

    guti = &attach_accept->guti;

    if (attach_accept->presencemask & OGS_NAS_EPS_ATTACH_ACCEPT_GUTI_PRESENT) {
        memcpy(&test_ue->nas_eps_guti.nas_plmn_id,
                &guti->guti.nas_plmn_id, OGS_PLMN_ID_LEN);
        test_ue->nas_eps_guti.mme_gid = guti->guti.mme_gid;
        test_ue->nas_eps_guti.mme_code = guti->guti.mme_code;
        test_ue->nas_eps_guti.m_tmsi = guti->guti.m_tmsi;
    }

    testemm_send_to_esm(test_ue, &attach_accept->esm_message_container);
}
