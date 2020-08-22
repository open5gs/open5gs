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

#ifndef TEST_NAS_PATH_H
#define TEST_NAS_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

void testgmm_recv(test_ue_t *test_ue, ogs_pkbuf_t *pkbuf);
void testgsm_recv(test_sess_t *sess, ogs_pkbuf_t *pkbuf);

void testgmm_send_to_gsm(test_sess_t *sess,
        ogs_nas_payload_container_t *payload_container);

void testemm_recv(test_ue_t *test_ue, ogs_pkbuf_t *pkbuf);
void testesm_recv(test_ue_t *test_ue, ogs_pkbuf_t *pkbuf);

void testemm_send_to_esm(test_ue_t *test_ue,
    ogs_nas_esm_message_container_t *esm_message_container);

#ifdef __cplusplus
}
#endif

#endif /* TEST_NAS_PATH_H */
