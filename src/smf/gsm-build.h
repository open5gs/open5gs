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

#ifndef GSM_BUILD_H
#define GSM_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *gsm_build_pdu_session_establishment_accept(smf_sess_t *sess);
ogs_pkbuf_t *gsm_build_pdu_session_establishment_reject(
        smf_sess_t *sess, ogs_nas_5gsm_cause_t gsm_cause);

ogs_pkbuf_t *gsm_build_pdu_session_modification_command(
        smf_sess_t *sess,
        uint8_t qos_rule_code, uint8_t qos_flow_description_code);
ogs_pkbuf_t *gsm_build_pdu_session_modification_reject(
        smf_sess_t *sess, ogs_nas_5gsm_cause_t gsm_cause);

ogs_pkbuf_t *gsm_build_pdu_session_release_command(
        smf_sess_t *sess, ogs_nas_5gsm_cause_t gsm_cause);
ogs_pkbuf_t *gsm_build_pdu_session_release_reject(
        smf_sess_t *sess, ogs_nas_5gsm_cause_t gsm_cause);

ogs_pkbuf_t *gsm_build_status(smf_sess_t *sess, ogs_nas_5gsm_cause_t cause);

#ifdef __cplusplus
}
#endif

#endif /* GSM_BUILD_H */
