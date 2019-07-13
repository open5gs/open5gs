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

#ifndef EMM_HANDLER_H
#define EMM_HANDLER_H

#include "nas/nas-message.h"

#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

int emm_handle_attach_request(
    mme_ue_t *mme_ue, nas_attach_request_t *attach_request);
int emm_handle_attach_complete(
    mme_ue_t *mme_ue, nas_attach_complete_t *attach_complete);

int emm_handle_identity_response(
        mme_ue_t *mme_ue, nas_identity_response_t *identity_response);

int emm_handle_detach_request(
    mme_ue_t *mme_ue, nas_detach_request_from_ue_t *detach_request);

int emm_handle_service_request(
    mme_ue_t *mme_ue, nas_service_request_t *service_request);

int emm_handle_tau_request(
    mme_ue_t *mme_ue, nas_tracking_area_update_request_t *tau_request);

int emm_handle_extended_service_request(
    mme_ue_t *mme_ue, nas_extended_service_request_t *extended_service_request);

#ifdef __cplusplus
}
#endif

#endif /* EMM_HANDLER_H */
