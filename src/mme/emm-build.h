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

#ifndef EMM_BUILD_H
#define EMM_BUILD_H

#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *emm_build_attach_accept(
        mme_ue_t *mme_ue, ogs_pkbuf_t *esmbuf);
ogs_pkbuf_t *emm_build_attach_reject(
        ogs_nas_emm_cause_t emm_cause, ogs_pkbuf_t *esmbuf);

ogs_pkbuf_t *emm_build_identity_request(mme_ue_t *mme_ue);
ogs_pkbuf_t *emm_build_security_mode_command(mme_ue_t *mme_ue);

ogs_pkbuf_t *emm_build_authentication_request(mme_ue_t *mme_ue);
ogs_pkbuf_t *emm_build_authentication_reject(void);

ogs_pkbuf_t *emm_build_detach_request(mme_ue_t *mme_ue);
ogs_pkbuf_t *emm_build_detach_accept(mme_ue_t *mme_ue);

ogs_pkbuf_t *emm_build_tau_accept(mme_ue_t *mme_ue);
ogs_pkbuf_t *emm_build_tau_reject(
        ogs_nas_emm_cause_t emm_cause, mme_ue_t *mme_ue);

ogs_pkbuf_t *emm_build_service_reject(
        ogs_nas_emm_cause_t emm_cause, mme_ue_t *mme_ue);

ogs_pkbuf_t *emm_build_cs_service_notification(mme_ue_t *mme_ue);
ogs_pkbuf_t *emm_build_downlink_nas_transport(
        mme_ue_t *mme_ue, uint8_t *buffer, uint8_t length);

#ifdef __cplusplus
}
#endif

#endif /* EMM_BUILD_H */
