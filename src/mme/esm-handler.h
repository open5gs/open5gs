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

#ifndef ESM_HANDLER_H
#define ESM_HANDLER_H

#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

int esm_handle_pdn_connectivity_request(mme_bearer_t *bearer,
        ogs_nas_eps_pdn_connectivity_request_t *req,
        int create_action);
int esm_handle_information_response(mme_sess_t *sess, 
        ogs_nas_eps_esm_information_response_t *rsp);
int esm_handle_bearer_resource_allocation_request(
        mme_bearer_t *bearer, ogs_nas_eps_message_t *message);
int esm_handle_bearer_resource_modification_request(
        mme_bearer_t *bearer, ogs_nas_eps_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* ESM_HANDLER_H */
