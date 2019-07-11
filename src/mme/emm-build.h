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

int emm_build_attach_accept(
        ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue, ogs_pkbuf_t *esmbuf);
int emm_build_attach_reject(
        ogs_pkbuf_t **emmbuf, nas_emm_cause_t emm_cause, ogs_pkbuf_t *esmbuf);

int emm_build_identity_request(
        ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue);
int emm_build_security_mode_command(
        ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue);

int emm_build_authentication_request(
        ogs_pkbuf_t **emmbuf, e_utran_vector_t *e_utran_vector);
int emm_build_authentication_reject(ogs_pkbuf_t **emmbuf);

int emm_build_detach_accept(
        ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue);

int emm_build_tau_accept(ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue);
int emm_build_tau_reject(ogs_pkbuf_t **emmbuf, 
        nas_emm_cause_t emm_cause,mme_ue_t *mme_ue);

int emm_build_service_reject(ogs_pkbuf_t **emmbuf, 
        nas_emm_cause_t emm_cause, mme_ue_t *mme_ue);

int emm_build_cs_service_notification(ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue);

#ifdef __cplusplus
}
#endif

#endif /* EMM_BUILD_H */
