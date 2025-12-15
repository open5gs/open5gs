/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef MME_PATH_H
#define MME_PATH_H

#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

void mme_send_delete_session_or_detach(enb_ue_t *enb_ue, mme_ue_t *mme_ue);
void mme_send_delete_session_or_mme_ue_context_release(
        enb_ue_t *enb_ue, mme_ue_t *mme_ue);
void mme_send_release_access_bearer_or_ue_context_release(enb_ue_t *enb_ue);

void mme_send_after_paging(mme_ue_t *mme_ue, bool failed);

#ifdef __cplusplus
}
#endif

#endif /* MME_PATH_H */
