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

#ifndef TEST_ESM_BUILD_H
#define TEST_ESM_BUILD_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *testesm_build_pdn_connectivity_request(test_sess_t *sess);
ogs_pkbuf_t *testesm_build_pdn_disconnect_request(test_sess_t *sess);
ogs_pkbuf_t *testesm_build_esm_information_response(test_sess_t *sess);
ogs_pkbuf_t *testesm_build_activate_default_eps_bearer_context_accept(
        test_bearer_t *bearer, bool integrity_protected_and_ciphered);
ogs_pkbuf_t *testesm_build_activate_dedicated_eps_bearer_context_accept(
        test_bearer_t *bearer);
ogs_pkbuf_t *testesm_build_modify_eps_bearer_context_accept(
        test_bearer_t *bearer);
ogs_pkbuf_t *testesm_build_deactivate_eps_bearer_context_accept(
        test_bearer_t *bearer);

ogs_pkbuf_t *testesm_build_bearer_resource_allocation_request(
        test_bearer_t *bearer);
ogs_pkbuf_t *testesm_build_bearer_resource_modification_request(
    test_bearer_t *bearer, uint8_t tft_code, uint8_t qci, uint8_t esm_cause);

#ifdef __cplusplus
}
#endif

#endif /* TEST_ESM_BUILD_H */
