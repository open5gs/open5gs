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

#ifndef TEST_EMM_BUILD_H
#define TEST_EMM_BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *testemm_build_attach_request(
        test_ue_t *test_ue, ogs_pkbuf_t *esmbuf,
        bool integrity_protected, bool ciphered);
ogs_pkbuf_t *testemm_build_identity_response(test_ue_t *test_ue);
ogs_pkbuf_t *testemm_build_authentication_response(test_ue_t *test_ue);
ogs_pkbuf_t *testemm_build_authentication_failure(
        test_ue_t *test_ue, ogs_nas_emm_cause_t emm_cause, uint64_t sqn_ms);
ogs_pkbuf_t *testemm_build_security_mode_complete(test_ue_t *test_ue);
ogs_pkbuf_t *testemm_build_attach_complete(
        test_ue_t *test_ue, ogs_pkbuf_t *esmbuf);
ogs_pkbuf_t *testemm_build_tau_complete(test_ue_t *test_ue);

ogs_pkbuf_t *testemm_build_detach_request(
        test_ue_t *test_ue, bool switch_off,
        bool integrity_protected, bool ciphered);

ogs_pkbuf_t *testemm_build_service_request(test_ue_t *test_ue);
ogs_pkbuf_t *testemm_build_tau_request(
        test_ue_t *test_ue, bool active_flag, uint8_t update_type,
        bool integrity_protected, bool ciphered);
ogs_pkbuf_t *testemm_build_emm_status(
        test_ue_t *test_ue, ogs_nas_emm_cause_t emm_cause);

ogs_pkbuf_t *testemm_build_extended_service_request(
        test_ue_t *test_ue, uint8_t type,
        bool integrity_protected, bool ciphered);

ogs_pkbuf_t *testemm_build_uplink_nas_transport(test_ue_t *test_ue);

#ifdef __cplusplus
}
#endif

#endif /* TEST_EMM_BUILD_H */
