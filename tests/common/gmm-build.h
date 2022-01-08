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

#ifndef TEST_GMM_BUILD_H
#define TEST_GMM_BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *testgmm_build_registration_request(
        test_ue_t *test_ue, ogs_pkbuf_t *nasbuf,
        bool integrity_protected, bool ciphered);
ogs_pkbuf_t *testgmm_build_registration_complete(test_ue_t *test_ue);

ogs_pkbuf_t *testgmm_build_service_request(
        test_ue_t *test_ue, uint8_t service_type, ogs_pkbuf_t *nasbuf,
        bool integrity_protected, bool ciphered);
ogs_pkbuf_t *testgmm_build_de_registration_request(
        test_ue_t *test_ue, bool switch_off,
        bool integrity_protected, bool ciphered);

ogs_pkbuf_t *testgmm_build_identity_response(test_ue_t *test_ue);
ogs_pkbuf_t *testgmm_build_authentication_response(test_ue_t *test_ue);
ogs_pkbuf_t *testgmm_build_authentication_failure(
        test_ue_t *test_ue, ogs_nas_5gmm_cause_t gmm_cause, uint64_t sqn_ms);
ogs_pkbuf_t *testgmm_build_security_mode_complete(
        test_ue_t *test_ue, ogs_pkbuf_t *nasbuf);
ogs_pkbuf_t *testgmm_build_configuration_update_complete(test_ue_t *test_ue);
ogs_pkbuf_t *testgmm_build_ul_nas_transport(test_sess_t *test_sess,
        uint8_t payload_type, ogs_pkbuf_t *payload);
ogs_pkbuf_t *testgmm_build_gmm_status(
        test_ue_t *test_ue, ogs_nas_5gmm_cause_t gmm_cause);

#ifdef __cplusplus
}
#endif

#endif /* TEST_GMM_BUILD_H */
