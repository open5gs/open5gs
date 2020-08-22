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

#ifndef TEST_NAS_5GS_ENCODER_H
#define TEST_NAS_5GS_ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *test_nas_5gmm_encode(ogs_nas_5gs_message_t *message);
ogs_pkbuf_t *test_nas_5gs_plain_encode(ogs_nas_5gs_message_t *message);

ogs_pkbuf_t *test_nas_emm_encode(ogs_nas_eps_message_t *message);
ogs_pkbuf_t *test_nas_eps_plain_encode(ogs_nas_eps_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* TEST_NAS_5GS_ENCODER_H */

