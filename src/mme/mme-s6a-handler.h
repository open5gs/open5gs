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

#ifndef MME_S6A_HANDLER_H
#define MME_S6A_HANDLER_H

#include "mme-context.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t mme_s6a_handle_aia(
        mme_ue_t *mme_ue, ogs_diam_s6a_message_t *s6a_message);
uint8_t mme_s6a_handle_ula(
        mme_ue_t *mme_ue, ogs_diam_s6a_message_t *s6a_message);
uint8_t mme_s6a_handle_pua(
        mme_ue_t *mme_ue, ogs_diam_s6a_message_t *s6a_message);
uint8_t mme_s6a_handle_idr(
        mme_ue_t *mme_ue, ogs_diam_s6a_message_t *s6a_message);
void mme_s6a_handle_clr(
        mme_ue_t *mme_ue, ogs_diam_s6a_message_t *s6a_message);

#ifdef __cplusplus
}
#endif

#endif /* MME_S6A_HANDLER_H */
